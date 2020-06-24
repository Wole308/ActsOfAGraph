#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "host_common.h"
#include "common.h"
#include "../debugger/host_debugger.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#include "EdgeProcess.h" // Grafboost header
#include "VertexValues.h" // Grafboost header
#include "sortreduce.h" // Grafboost header
#include "filekvreader.h" // Grafboost header
#include "../kernels/kernelprocess.h"
#include "edge_process.h"
using namespace std;

edge_process::edge_process(graph * graphobj){			
	cout<<"edge_process::edge_process:: constructor called. "<<endl;
	
	numvertexbanks = graphobj->getnumvertexbanks();
	numedgebanks = graphobj->getnumedgebanks();
	numverticespervertexbank = KVDATA_RANGE / graphobj->getnumvertexbanks();
	numvertices = graphobj->get_num_vertices();
	
	edgesbuffer_stream = new edgeprop1_t[KVDATA_BATCHSIZE]; 
	edgesbuffer_contiguous = new edgeprop2_t[KVDATA_BATCHSIZE]; 
	edgeoffsetsbuffer = new prvertexoffset_t[(KVDATA_BATCHSIZE * 2)]; 
	
	// Courtesy: Grafboost	
	m_idx_buffer_offset = 0;
	m_idx_buffer_bytes = 0;
	mp_idx_buffer = aligned_alloc(512, m_buffer_alloc_bytes);
	m_edge_buffer_offset = 0;
	m_edge_buffer_bytes = 0;
	mp_edge_buffer = aligned_alloc(512, m_buffer_alloc_bytes);
	m_index_blocks_read = 0;
	m_edge_blocks_read = 0;
	
	algorithmobj = new algorithm();
	utilityobj = new utility();
}
edge_process::~edge_process(){} 

unsigned int edge_process::generateupdates_random(SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader, int nvmeFd_edgeoffsets_r2, int nvmeFd_edgeproperties_r2, uint512_vec_dt * kvdram, unsigned int kvoffset, unsigned int * status){
	#ifndef LOADKVDRAMS
	*status = FINISHED;
	return 0; 
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"edge_process::generateupdates_random started."<<endl;
	#endif	
	#ifdef _DEBUGMODE_HOSTPRINTS_X
	bfsvertexoffset_t vbuf[16]; // test
	pread(nvmeFd_edgeoffsets_r2, vbuf, (16 * sizeof(bfsvertexoffset_t)), 0);
	for(unsigned int i=0; i<16; i++){ cout<<">>> edge_process::generateupdates_random: vbuf["<<i<<"]: "<<vbuf[i]<<endl; }
	
	edgeprop2_t ebuf2[16]; // test
	pread(nvmeFd_edgeproperties_r2, ebuf2, (16 * sizeof(edgeprop2_t)), 0);
	for(unsigned int i=0; i<16; i++){ cout<<">>> edge_process::generateupdates_random: ebuf2["<<i<<"].srcvid: "<<ebuf2[i].srcvid<<", ebuf2["<<i<<"].dstvid: "<<ebuf2[i].dstvid<<endl; }
	#endif 
	
	unsigned int kvthreshold = (PADDEDKVDATA_BATCHSIZE / 10) * 9; // FIXME. might not work for scale-free graphs
	unsigned int keyvaluesread = 0;
	
	std::tuple<uint32_t,uint32_t,bool> res = reader->Next();
	while ( std::get<2>(res) ) {
		uint32_t key = std::get<0>(res);
		uint32_t val = std::get<1>(res);
		res = reader->Next();
		#ifdef _DEBUGMODE_HOSTPRINTS_X
		cout<<"$$$ edge_process::generateupdates_random. key: "<<key<<", val: "<<val<<endl;
		#endif 
		
		generatekeyvalues_random(key, val, nvmeFd_edgeoffsets_r2, nvmeFd_edgeproperties_r2, (keyvalue_t *) kvdram, kvoffset, &keyvaluesread); 
		
		#ifdef _DEBUGMODE_HOSTPRINTS_X
		cout<<"$$$ edge_process::generateupdates_random. key: "<<key<<", val: "<<val<<", keyvaluesread: "<<keyvaluesread<<endl;
		#endif 
		
		if(keyvaluesread > kvthreshold){  
			cout<<"edge_process::generateupdates_random reached threshold. keyvaluesread: "<<keyvaluesread<<", status: "<<*status<<endl;
			return keyvaluesread; 
		}
	}
	cout<<"$$$ edge_process::generateupdates_random. reached end-of-file. keyvaluesread: "<<keyvaluesread<<endl;
	*status = FINISHED;
	return keyvaluesread; 
}
unsigned int edge_process::generateupdates2_random(SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader, keyvalue_t * kvdram, unsigned int kvoffset, unsigned int * status){
	#ifndef LOADKVDRAMS
	*status = FINISHED;
	return 0;
	#endif
	
	unsigned int keyvaluesread = 0;
	std::tuple<uint32_t,uint32_t,bool> res = reader->Next();
	while ( std::get<2>(res) ) {
		uint32_t key = std::get<0>(res);
		uint32_t val = std::get<1>(res);
		res = reader->Next();
		#ifdef _DEBUGMODE_HOSTPRINTS_X
		cout<<"$$$ edge_process::generateupdates2_random. key: "<<key<<", val: "<<val<<endl;
		#endif 
		
		keyvalue_t kv;
		kv.key = key;
		kv.value = val;
		kvdram[(kvoffset + keyvaluesread)] = kv;
		keyvaluesread += 1;
		
		#ifdef _DEBUGMODE_HOSTPRINTS_X
		cout<<"$$$ edge_process::generateupdates2_random. key: "<<key<<", val: "<<val<<", keyvaluesread: "<<keyvaluesread<<endl;
		#endif
		
		if(keyvaluesread >= KVDATA_BATCHSIZE){  
			cout<<"edge_process::generateupdates2_random reached threshold. keyvaluesread: "<<keyvaluesread<<", status: "<<*status<<endl;
			return keyvaluesread;
		}
	}
	cout<<"$$$ edge_process::generateupdates2_random. reached end-of-file. keyvaluesread: "<<keyvaluesread<<endl;
	// exit(EXIT_SUCCESS);
	*status = FINISHED;
	return keyvaluesread; 
}
void edge_process::generatekeyvalues_random(vertex_t key, value_t val, int nvmeFd_edgeoffsets_r2, int nvmeFd_edgeproperties_r2, keyvalue_t * kvdram, unsigned int kvoffset, unsigned int * kvsize){
	#ifdef _DEBUGMODE_HOSTPRINTS_X
	cout<<"edge_process::generatekeyvalues_random: key: "<<key<<", val: "<<val<<", kvoffset: "<<kvoffset<<", *kvsize: "<<*kvsize<<endl;
	#endif 

	size_t edge_element_bytes = sizeof(edgeprop2_t);
	size_t byte_offset = ((size_t)key)*sizeof(bfsvertexoffset_t);
	bfsvertexoffset_t mp_vidx_buffer[2];
	pread(nvmeFd_edgeoffsets_r2, mp_vidx_buffer, (2 * sizeof(bfsvertexoffset_t)), byte_offset);
	
	bfsvertexoffset_t byte_offset_1 = mp_vidx_buffer[0];	
	uint32_t fanout = mp_vidx_buffer[1] - mp_vidx_buffer[0];
	if(key >= (numvertices-1)){ fanout = 0; } // FIXME. EDGE CONDITIONS
	
	#ifdef _DEBUGMODE_HOSTPRINTS_X
	if(fanout > 2000){ cout<<"edge_process::generatekeyvalues_random. WARNING: fan out too high? fanout: "<<fanout<<", key: "<<key<<", val: "<<val<<endl; exit(EXIT_FAILURE); } // FIXME // CHANGED
	#endif
	
	value_t edgeval = algorithmobj->edge_program(key, val, fanout);
	
	// m_edgebuffer_alloc_bytes depends on fanout 
	m_edgebuffer_alloc_bytes = (((fanout * sizeof(edgeprop2_t)) + (1024 - 1)) / 1024) * 1024;
	#ifdef _DEBUGMODE_HOSTPRINTS_X
	printf("edge_process::generatekeyvalues_random: m_edgebuffer_alloc_bytes: %i, fanout: %i \n", m_edgebuffer_alloc_bytes, fanout);
	// exit(EXIT_SUCCESS);
	#endif 

	for (vertex_t i = 0; i < fanout; i++){
		uint64_t edge_offset = (byte_offset_1*edge_element_bytes)+(i*edge_element_bytes);
		if ((edge_offset < m_edge_buffer_offset) || ((edge_offset + edge_element_bytes) > (m_edge_buffer_offset+m_edge_buffer_bytes))){
			uint64_t byte_offset_aligned = edge_offset&(~0x3ff); // 1 KB alignment
	
			pread(nvmeFd_edgeproperties_r2, mp_edge_buffer, m_edgebuffer_alloc_bytes, byte_offset_aligned);
			m_edge_blocks_read++;
			m_edge_buffer_offset = byte_offset_aligned;
			m_edge_buffer_bytes = m_edgebuffer_alloc_bytes;
		}
		
		uint64_t internal_offset = edge_offset - m_edge_buffer_offset;		
		vertex_t neighbor = *((vertex_t*)(((uint8_t*)mp_edge_buffer)+internal_offset));
		
		keyvalue_t kv;
		kv.key = neighbor;
		kv.value = edgeval;
		#ifdef _DEBUGMODE_HOSTPRINTS_X
		printf( "generatekeyvalues_random: key: %i, neighbor: %i, value: %i \n", key, neighbor, edgeval);
		#endif 
		kvdram[(kvoffset + *kvsize)] = kv;
		*kvsize += 1;
	}
}

unsigned int edge_process::generateupdates_stream(unsigned int edgesoffset, unsigned int basevertexoffset, unsigned int * runningvertexid, vertex_t edgespropsz, int nvmeFd_edgeproperties_r2, int nvmeFd_edgeoffsets_r2, vertexprop_t * vertexpropertiesbuffer, unsigned int * isactivevertexinfobuffer, uint512_vec_dt * kvdram, metadata_t * kvstats, unsigned int threadidx){		
	#ifndef LOADKVDRAMS
	return; 
	#endif	
	size_t epfileoffset = edgesoffset * sizeof(edgeprop1_t);
	size_t epfilenumbytestoread = edgespropsz * sizeof(edgeprop1_t);

	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_1 = std::chrono::steady_clock::now();
	#endif 
	loadedgepropertiesfromfile<edgeprop1_t>(nvmeFd_edgeproperties_r2, epfileoffset, edgesbuffer_stream, 0, epfilenumbytestoread);
	#ifdef _DEBUGMODE_TIMERS
	utilityobj->stopTIME("edge_process::generateupdates_stream: load edge properties Time Elapsed: ", begintime_1, NAp);
	#endif 
	
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_2 = std::chrono::steady_clock::now();
	#endif 
	loadedgeoffsetsfromfile<prvertexoffset_t>(nvmeFd_edgeoffsets_r2, (epfileoffset / NUMBITSINUNSIGNEDINT), edgeoffsetsbuffer, 0, (epfilenumbytestoread / NUMBITSINUNSIGNEDINT));
	#ifdef _DEBUGMODE_TIMERS
	utilityobj->stopTIME("edge_process::generateupdates_stream: load edge offsets Time Elapsed: ", begintime_2, NAp);
	#endif 
	
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_3 = std::chrono::steady_clock::now();
	#endif 
	unsigned int kvcount = generatekeyvalues_stream(edgesoffset, basevertexoffset, runningvertexid, vertexpropertiesbuffer, isactivevertexinfobuffer, edgespropsz, (keyvalue_t *) kvdram, 0);
	#ifdef _DEBUGMODE_TIMERS
	utilityobj->stopTIME("edge_process::generateupdates_stream: generate key-values Time Elapsed: ", begintime_3, NAp);
	#endif 
	
	#ifdef ACTGRAPH_SETUP
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_4 = std::chrono::steady_clock::now();
	#endif 
	collectstats(kvdram, kvstats, 0, kvcount, 0, pow(NUM_PARTITIONS, utilityobj->GETTREEDEPTH_(threadidx)), (utilityobj->GETKVDATA_RANGE_FORSSDPARTITION_(threadidx) / pow(NUM_PARTITIONS, utilityobj->GETTREEDEPTH_(threadidx)))); // edge->dstvid's in all workbanks are all local
	#ifdef _DEBUGMODE_TIMERS
	utilityobj->stopTIME("edge_process::generateupdates_stream: collect stats Time Elapsed: ", begintime_4, NAp);
	#endif
	#endif
	return kvcount;
}
unsigned int edge_process::generatekeyvalues_stream(unsigned int edgesoffset, unsigned int basevertexoffset, unsigned int * runningvertexid, vertexprop_t * vertexpropertiesbuffer, unsigned int * isactivevertexinfobuffer, vertex_t edgespropsz, keyvalue_t * kvdram, vertex_t kvdramoffset){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "edge_process::generatekeyvalues:: generating key values... edgespropsz: "<<edgespropsz<< std::endl;
	utilityobj->printbits((char *)edgeoffsetsbuffer, 4);
	#endif

	algorithm algorithmobjjj;
	size_t ledgeid = 0;
	unsigned int nextedge = edgesoffset;
	vertexprop_t vertexprop; vertexprop.outdegree = 1;
	unsigned int isvertexactive = 0;
	unsigned int outdegree = 0;
	unsigned int vdata = 0;
	unsigned int kvcount = 0;
	for(unsigned int i=0; i<edgespropsz; i++){
		edgeprop1_t edge = edgesbuffer_stream[i];
		
		unsigned int edgeoffsetbit = utilityobj->RetrieveBit((unsigned int *)edgeoffsetsbuffer, ledgeid);
		ledgeid += 1;
		if(edgeoffsetbit==1){ 
			vertexprop = vertexpropertiesbuffer[(*runningvertexid - basevertexoffset)];  // FIXME.
			#ifndef PR_ALGORITHM
			isvertexactive = utilityobj->RetrieveBit(isactivevertexinfobuffer, (*runningvertexid - basevertexoffset)); *runningvertexid+=1;
			#endif 
		}
		#ifdef _DEBUGMODE_CHECKSX
		if((*runningvertexid - basevertexoffset) > (KVDATA_RANGE / MAXNUMVERTEXBANKS)){ cout<<"edge_process::generatekeyvalues_stream: ERROR: out-of-range. i: "<<i<<", runningvertexid: "<<*runningvertexid<<", (KVDATA_RANGE / MAXNUMVERTEXBANKS): "<<(KVDATA_RANGE / MAXNUMVERTEXBANKS)<<endl; exit(EXIT_FAILURE); }
		if(edgeoffsetbit==1){ cout<<"i: "<<i<<", runningvertexid: "<<*runningvertexid<<", vertexprop.outdegree: "<<vertexprop.outdegree<<", vertexprop.data: "<<vertexprop.data<<endl; }
		#endif 

		outdegree = vertexprop.outdegree;
		vdata = 1; // FIXME.
		
		// value_t edgeval = algorithmobj->edge_program(*runningvertexid, 1, outdegree);
		#ifdef PR_ALGORITHM
		value_t edgeval = 0.1 + ((1 - 0.1) * (vdata / outdegree));
		#elif defined(BFS_ALGORITHM)
		value_t edgeval = *runningvertexid;
		#elif defined(BC_ALGORITHM)
		value_t edgeval = *runningvertexid;
		#endif
		
		#ifdef PR_ALGORITHM
		keyvalue_t data;
		data.key = edge.dstvid;
		data.value = edgeval;
		kvdram[i] = data;
		#else
		if(isvertexactive == 1){
			keyvalue_t data;
			data.key = edge.dstvid;
			data.value = edgeval;
			kvdram[kvcount++] = data;
			#ifdef XXX
			cout<<"generatekeyvalues_stream: vertexprop.vertexisactive == 1 seen. data.key: "<<data.key<<", data.value: "<<data.value<<endl;
			cout<<"generatekeyvalues_stream: (*runningvertexid - basevertexoffset): "<<(*runningvertexid - basevertexoffset)<<", basevertexoffset: "<<basevertexoffset<<", *runningvertexid: "<<*runningvertexid<<endl;
			#endif
		}
		#endif 
	}
	#ifdef PR_ALGORITHM
	return edgespropsz;
	#else
	#ifdef _DEBUGMODE_CHECKS3
	cout<<"$$$ edge_process::generatekeyvalues_stream. end of generate. "<<kvcount<<" edges from active vertices extracted"<<endl;
	#endif 
	return kvcount;
	#endif 
}

void edge_process::generateupdates_contiguous(size_t wordoffset, vertex_t edgespropsz, int nvmeFd_edgeproperties_r2, int nvmeFd_edgeoffsets_r2, vertexprop_t * vertexpropertiesbuffer, uint512_vec_dt * kvdram, metadata_t * kvstats, unsigned int threadidx){		
	#ifndef LOADKVDRAMS
	return; 
	#endif 
	
	loadedgepropertiesfromfile<edgeprop2_t>(nvmeFd_edgeproperties_r2, (wordoffset * sizeof(edgeprop2_t)), edgesbuffer_contiguous, 0, (edgespropsz * sizeof(edgeprop2_t)));
	vertex_t firstv = edgesbuffer_contiguous[0].srcvid;
	vertex_t secondv = edgesbuffer_contiguous[1].srcvid;
	vertex_t lastv; if(edgespropsz > 0){ lastv = edgesbuffer_contiguous[edgespropsz - 1].srcvid; } else { lastv = firstv; }
	vertex_t vertexpropsz = lastv - firstv;
	#ifdef _DEBUGMODE_CHECKS
	if(vertexpropsz < 0){ cout<<"edge_process::WorkerThread: ERROR 34. verticesbuffersize can not be negative."<<endl; exit(EXIT_FAILURE); }
	if(vertexpropsz >= KVDATA_RANGE){ cout<<"edge_process::WorkerThread: ERROR 34. verticesbuffersize can not be more than "<<KVDATA_RANGE<<"."<<endl; exit(EXIT_FAILURE); }
	#endif
	loadedgeoffsetsfromfile<prvertexoffset_t>(nvmeFd_edgeoffsets_r2, (firstv * sizeof(prvertexoffset_t)), edgeoffsetsbuffer, 0, (vertexpropsz * sizeof(prvertexoffset_t)));
	generatekeyvalues_contiguous(vertexpropertiesbuffer, (keyvalue_t *) kvdram, vertexpropsz, edgespropsz, 0);
	#ifdef ACTGRAPH_SETUP
	collectstats(kvdram, kvstats, 0, edgespropsz, 0, pow(NUM_PARTITIONS, utilityobj->GETTREEDEPTH_(threadidx)), (utilityobj->GETKVDATA_RANGE_FORSSDPARTITION_(threadidx) / pow(NUM_PARTITIONS, utilityobj->GETTREEDEPTH_(threadidx)))); // edge->dstvid's in all workbanks are all local
	#endif
	return;
}
void edge_process::generatekeyvalues_contiguous(vertexprop_t * vertexpropertiesbuffer, keyvalue_t * kvdram, vertex_t vertexpropsz, vertex_t edgespropsz, vertex_t kvdramoffset){
	if(vertexpropsz <= 0){ return; }
	if(vertexpropsz >= numverticespervertexbank) { std::cout<<"Host::generatekeyvalues::ERROR 45: out-of-bounds: vertexpropsz: "<<vertexpropsz<<", numverticespervertexbank: "<<numverticespervertexbank<<std::endl; exit(EXIT_FAILURE); }
	vertex_t kvdramoffset_kvs = kvdramoffset / VECTOR_SIZE; // vertexpropsz
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "edge_process::generatekeyvalues:: generate key values "<< std::endl;
	cout<<"edge_process::generatekeyvalues:: vertexpropsz: "<<vertexpropsz<<endl;
	cout<<"edge_process::generatekeyvalues:: edgespropsz: "<<edgespropsz<<endl;
	cout<<"edge_process::generatekeyvalues:: kvdramoffset: "<<kvdramoffset<<endl;
	cout<<"edge_process::generatekeyvalues:: kvdramoffset_kvs: "<<kvdramoffset_kvs<<endl;
	#endif 
	
	unsigned int errcount = 0;
	unsigned int edgeindex = 0;
	unsigned int num_edges_processed = 0;
	unsigned int ebase = edgeoffsetsbuffer[0];
	for(unsigned int i = 0; i<vertexpropsz ; i++){
		if(((i % 10000000) == 0) && (i > 0)){
			#ifdef _DEBUGMODE_HOSTPRINTS
			std::cout <<i <<" edges processed"<<std::endl;
			#endif
		}
		prvertexoffset_t voffset = edgeoffsetsbuffer[i];
		vertexprop_t vertexprop = vertexpropertiesbuffer[i];
		prvertexoffset_t voffset_plus1 = edgeoffsetsbuffer[(i + 1)];
		vertex_t outdegree;
		#ifdef ACTGRAPH_SETUP
		outdegree = vertexprop.outdegree;
		#else 
		outdegree = voffset_plus1 - voffset;
		#endif 
		#ifdef _DEBUGMODE_CHECKSX
		if((voffset_plus1 > (KVDATA_RANGE * 2)) || (voffset > (KVDATA_RANGE * 2))){ cout<<"edge_process::generatekeyvalues: WARNING: is this a problem? voffset: "<<voffset<<", voffset_plus1: "<<voffset_plus1<<", sizeof(prvertexoffset_t): "<<sizeof(prvertexoffset_t)<<" exiting...."<<endl; exit(EXIT_FAILURE); }
		#endif 
		
		// value_t edgeval = algorithmobj->edge_program(vertexprop.vid, vertexprop.data, outdegree);
		// value_t edgeval = algorithmobj->edge_program(vertexprop.vid, 1, outdegree); // FIXME.
		value_t edgeval = algorithmobj->edge_program(i, 1, outdegree); // FIXME.
		for(edge_t j = voffset; j<voffset_plus1; j++){
			if((edge_t)(j - ebase) >= edgespropsz){}
			
			unsigned int lvpoffset = (j - ebase);
			edgeprop2_t edge = edgesbuffer_contiguous[(lvpoffset % edgespropsz)]; // FIXME.
			
			#ifdef _DEBUGMODE_HOSTPRINTS
			num_edges_processed += 1;
			#endif

			keyvalue_t data;
			data.key = edge.dstvid;
			data.value = edgeval;
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kvdramoffset + edgeindex) >= (kvdramoffset + PADDEDKVDATA_BATCHSIZE)){ std::cout<<"Host::generatekeyvalues::ERROR 45: out-of-bounds: (kvdramoffset + edgeindex): "<<(kvdramoffset + edgeindex)<<", PADDEDKVDATA_BATCHSIZE: "<<PADDEDKVDATA_BATCHSIZE<<std::endl; exit(EXIT_FAILURE); }
			#endif			
			kvdram[((kvdramoffset + edgeindex) % KVDATA_BATCHSIZE)] = data; // FIXME. CORRECT THIS
			edgeindex ++;
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "edge_process::generatekeyvalues:: finished processing edges. "<< num_edges_processed <<" edges processed" << ", edgeindex: " << edgeindex << std::endl;
	#endif 
}

template <class T>
void edge_process::loadedgepropertiesfromfile(int nvmeFd_edgeproperties_r2, size_t fileoffset, T * buffer, edge_t bufferoffset, vertex_t numbytestoread){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "load edges from file. "<< std::endl;
	std::cout <<endl<< "Host::loadedgepropertiesfromfile. loading edges from file... edgesoffset: "<<bufferoffset<<", numbytestoread: "<<numbytestoread<<", fileoffset: "<<fileoffset<<endl;
	#endif
	
	if(numbytestoread > 0){ if(pread (nvmeFd_edgeproperties_r2, &buffer[bufferoffset], numbytestoread, fileoffset) <= 0){ cout<<"loadedgepropertiesfromfile::ERROR 37. edgesoffset: "<<bufferoffset<<", numbytestoread: "<<numbytestoread<<", fileoffset: "<<fileoffset<<endl; exit(EXIT_FAILURE); }}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	#ifdef STREAMEDGESSETUP
	std::cout <<"Host::loadedgepropertiesfromfile. edgesbuffer["<<0<<"].dstvid: "<<buffer[0].dstvid<<endl;
	std::cout <<"Host::loadedgepropertiesfromfile. edgesbuffer["<<1<<"].dstvid: "<<buffer[1].dstvid<<endl;
	#else 
	std::cout <<"Host::loadedgepropertiesfromfile. edgesbuffer["<<0<<"].srcvid: "<<buffer[0].srcvid<<", edgesbuffer["<<0<<"].dstvid: "<<buffer[0].dstvid<<endl;
	std::cout <<"Host::loadedgepropertiesfromfile. edgesbuffer["<<1<<"].srcvid: "<<buffer[1].srcvid<<", edgesbuffer["<<1<<"].dstvid: "<<buffer[1].dstvid<<endl;
	#endif 
	std::cout << endl;
	#endif	
	return;
}
template <class T>
void edge_process::loadedgeoffsetsfromfile(int nvmeFd_edgeoffsets_r2, size_t fileoffset, T * buffer, vertex_t bufferoffset, size_t numbytestoread){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "edge_process::loadedgeoffsetsfromfile. loading vertices from file... bufferoffset: "<<bufferoffset<<", numbytestoread: "<<numbytestoread<<", fileoffset: "<<fileoffset<<endl;
	#endif 
	
	if(numbytestoread > 0){ if(pread(nvmeFd_edgeoffsets_r2, &buffer[bufferoffset], numbytestoread, fileoffset) <= 0){ cout<<"edge_process::loadedgeoffsetsfromfile::ERROR 35"<<endl; exit(EXIT_FAILURE); }}

	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printbits(nvmeFd_edgeoffsets_r2, fileoffset, 4);
	#endif
	return;
}
void edge_process::collectstats(uint512_vec_dt * kvdram, metadata_t * kvstats, vertex_t kvoffset, vertex_t kvbatchsz, vertex_t kvrangeoffset, unsigned int finalnumpartitionsfordram_partition, unsigned int finalrangefordram_partition){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "Collect stats of KvDRAM. "<< std::endl;
	cout<<"edge_process::collectstats:: kvoffset: "<<kvoffset<<endl;
	cout<<"edge_process::collectstats:: kvbatchsz: "<<kvbatchsz<<endl;
	cout<<"edge_process::collectstats:: kvrangeoffset: "<<kvrangeoffset<<endl;
	cout<<"edge_process::collectstats:: finalnumpartitionsfordram_partition: "<<finalnumpartitionsfordram_partition<<", KVSTATS_SIZE: "<<KVSTATS_SIZE<<endl;
	cout<<"edge_process::collectstats:: finalrangefordram_partition: "<<finalrangefordram_partition<<endl;
	#endif 
	vertex_t kvoffset_kvs = kvoffset / VECTOR_SIZE;
	vertex_t kvbatchsz_kvs = kvbatchsz / VECTOR_SIZE;
	unsigned int numerrors = 0;
	
	for(int i = 0 ; i<KVSTATS_SIZE; i++){ kvstats[i].offset = 0; kvstats[i].size = 0; }

	unsigned int partition;
	for(unsigned int i=0; i<kvbatchsz_kvs; i++){
		for(int j=0; j<VECTOR_SIZE; j++){
			partition = (kvdram[(kvoffset_kvs + i)].data[j].key - kvrangeoffset) / finalrangefordram_partition;
			#ifdef _DEBUGMODE_CHECKS3
			if(partition >= finalnumpartitionsfordram_partition){
				cout<<"WARNING: edge_process::collectstats:: partition is out-of-range: i:"<<i<<", j:"<<j<<", partition: "<<partition<<", finalnump..: "<<finalnumpartitionsfordram_partition<<", kvdram["<<i<<"].data["<<j<<"].key: "<<kvdram[i].data[j].key<<", kvrangeoffset: "<<kvrangeoffset<<endl; 
				partition = 0;
				numerrors += 1; if(numerrors > (100)){ cout<<"edge_process::collectstats: too many ERRORS. check design."<<endl; exit(EXIT_FAILURE); }
			}
			#endif
			kvstats[partition].size += 1;
		}
	}
	
	// calculate offsets
	kvstats[0].offset = 0;
	for (unsigned int p = 1; p<finalnumpartitionsfordram_partition; p++){
		kvstats[p].offset = utilityobj->hallignup_KV((kvstats[p-1].offset + kvstats[p-1].size + ((KVDATA_BATCHSIZE_NUMPADS / 2) * VECTOR_SIZE))); // FIXME. POSSIBLE CAUSE OF ERROR
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int p=0; p<2; p++){ cout<<"edge_process::collectstats:: kvstats["<<p<<"].offset: "<<kvstats[p].offset<<", kvstats["<<p<<"].size: "<<kvstats[p].size<<" key value pairs"<<endl; }
	#endif 
	
	// assign initial workload sizes
	for(int i = 0 ; i<KVSTATS_SIZE; i++){ kvstats[i].size = 0; }
	kvstats[0].size = kvbatchsz;

	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"HOST::DEBUGGER:: checking last offset of collected stats (sanity test). "<<finalnumpartitionsfordram_partition<<", KVSTATS_SIZE: "<<KVSTATS_SIZE<<endl;
	cout<<"edge_process::collectstats:: kvstats["<<(finalnumpartitionsfordram_partition - 1)<<"].offset: "<<kvstats[(finalnumpartitionsfordram_partition - 1)].offset<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"edge_process::collectstats:: kvstats[0].size: "<<kvstats[0].size<<" key value pairs"<<", numerrors: "<<numerrors<<endl;
	// exit(EXIT_SUCCESS);
	#endif 
	return;
}
