#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../../src/parameters/parameters.h"
#include "../../src/graphs/graph.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../src/stats/stats.h"
#include "../../src/graphs/create2Dgraph.h" // REMOVEME. just for debugging.
#include "../../include/common.h"
#include "edge_process.h"
using namespace std;

edge_process::edge_process(graph * _graphobj, stats * _statsobj){		
	cout<<"edge_process::edge_process:: constructor called. "<<endl;
	graphobj = _graphobj;
	parametersobj = new parameters();
	utilityobj = new utility();
	algorithmobj = new algorithm();
	dataaccessobj = new dataaccess();
	// statsobj = new stats(graphobj);
	statsobj = _statsobj;
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edgebuffer[i] = new edgeprop1_t[KVDATA_BATCHSIZE]; }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ vertexpointerbuffer[i] = new prvertexoffset_t[KVDATA_BATCHSIZE]; }
	for(unsigned int i=0; i<MAXNUMVERTEXBANKS; i++){ vertexpropertybuffer[i] = graphobj->getvertexpropertybuffer(i); }
	for(unsigned int i=0; i<MAXNUMVERTEXBANKS; i++){ vertexdatabuffer[i] = graphobj->getvertexdatabuffer(i); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ currentvid[i] = 0; }
	
	m_idx_buffer_offset = 0;
	m_idx_buffer_bytes = 0;
	mp_idx_buffer = aligned_alloc(512, m_buffer_alloc_bytes);
	m_edge_buffer_offset = 0;
	m_edge_buffer_bytes = 0;
	mp_edge_buffer = aligned_alloc(512, m_buffer_alloc_bytes);
	m_index_blocks_read = 0;
	m_edge_blocks_read = 0;
	
	#ifdef _DEBUGMODE_CHECKSXXX // REMOVEME. just for debugging.
	create2Dgraphobj = new create2Dgraph(graphobj);
	create2Dgraphobj->analyzegraph();
	create2Dgraphobj->rehashhighindegreevertices();
	#endif 
}
edge_process::~edge_process(){} 

void edge_process::generateupdates(unsigned int groupid, unsigned int bank, unsigned int col, unsigned int fdoffset[NUMCPUTHREADS], keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int datasize[NUMCPUTHREADS], unsigned int voffset){							
	#ifdef LOCKE
	for (int i = 0; i < NUMCPUTHREADS; i++){ generateupdates_stream(i, groupid, bank, col, fdoffset[i], batch[i], batchoffset[i], batchsize[i], datasize[i], voffset); }
	#else
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[i] = std::thread(&edge_process::generateupdates_stream, this, i, groupid, bank, col, fdoffset[i], batch[i], batchoffset[i], batchsize[i], datasize[i], voffset); }
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[i].join(); }
	#endif
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ statsobj->appendkeyvaluecount(bank, col, datasize[i]); }
	return;
}
unsigned int edge_process::generateupdates_stream(int ithreadidx, unsigned int groupid, unsigned int bank, unsigned int col, unsigned int fdoffset, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], vertex_t datasize, unsigned int voffset){			
	graphobj->loadedgepropertyfromfile(bank, col, fdoffset, edgebuffer[ithreadidx], 0, datasize); 
	
	graphobj->loadvertexpointersfromfile(bank, col, fdoffset, vertexpointerbuffer[ithreadidx], 0, datasize); 
	
	unsigned int kvcount = generatekeyvalues_stream(ithreadidx, groupid, bank, batch, batchoffset, batchsize, datasize, voffset); 
	return kvcount;
}
unsigned int edge_process::generatekeyvalues_stream(int ithreadidx, unsigned int groupid, unsigned int bank, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], vertex_t datasize, unsigned int voffset){
	unsigned int tempbatchoffset[NUMSUBCPUTHREADS]; // LEARNFROMME. avoids significant bottleneck
	unsigned int tempbatchsize[NUMSUBCPUTHREADS];
	utilityobj->copy(tempbatchoffset, batchoffset, NUMSUBCPUTHREADS);
	utilityobj->copy(tempbatchsize, batchsize, NUMSUBCPUTHREADS);
	
	vertexprop_t vertexprop; vertexprop.outdegree = 1;
	vertex_t tempcurrentvid = currentvid[ithreadidx];
	for(unsigned int i=0; i<datasize; i++){
		edgeprop1_t edge = edgebuffer[ithreadidx][i];
		
		unsigned int edgeoffsetbit = utilityobj->RetrieveBit((unsigned int *)vertexpointerbuffer[ithreadidx], i); 
		if(edgeoffsetbit==1){
			utilityobj->checkoutofbounds("edge_process::generatekeyvalues_stream 1", tempcurrentvid, KVDATA_RANGE, i, tempcurrentvid, NAp);
			vertexprop = vertexpropertybuffer[bank][tempcurrentvid]; 
			tempcurrentvid+=1;
		}

		unsigned int outdegree = vertexprop.outdegree;
		unsigned int vdata = 1; // FIXME.
		
		#ifdef PR_ALGORITHM
		if(outdegree == 0){ outdegree = 1; } // REMOVEME.
		value_t edgeval = 0.1 + ((1 - 0.1) * (vdata / outdegree));
		#elif defined(BFS_ALGORITHM)
		value_t edgeval = tempcurrentvid;
		#elif defined(BC_ALGORITHM)
		value_t edgeval = tempcurrentvid;
		#endif
		
		keyvalue_t data;
		data.key = edge.dstvid;
		data.value = edgeval;
		utilityobj->checkoutofbounds("edge_process::generatekeyvalues_stream 2", i, KVDATA_BATCHSIZE, NAp, NAp, NAp);
		
		insertkeyvaluetobuffer(batch, tempbatchoffset, tempbatchsize, data, voffset, groupid);
	}
	currentvid[ithreadidx] = tempcurrentvid;
	utilityobj->copy(batchsize, tempbatchsize, NUMSUBCPUTHREADS);
	return datasize;
}

void edge_process::generateupdates(unsigned int readerbank, unsigned int groupid, unsigned int bank, unsigned int col, keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], kvresults_t * results){
	for (int i = 0; i < NUMCPUTHREADS; i++){ generateupdates_random(i, readerbank, groupid, bank, col, batch[i], batchoffset[i], batchsize[i], results); }
	#ifdef XXX
	#ifdef LOCKE
	for (int i = 0; i < NUMCPUTHREADS; i++){ generateupdates_random(i, readerbank, bank, col, batch[i], batchoffset[i], batchsize[i], results); }
	#else
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[i] = std::thread(&edge_process::generateupdates_random, this, i, readerbank, bank, col, batch[i], batchoffset[i], batchsize[i], results); }
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[i].join(); }
	#endif
	#endif 
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ statsobj->appendkeyvaluecount(bank, col, results[i].datasize); }
	return;
}
void edge_process::generateupdates_random(int resultbank, unsigned int readerbank, unsigned int groupid, unsigned int bank, unsigned int col, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], kvresults_t * results){
	unsigned int kvthreshold = (KVDATA_BATCHSIZE / 10) * 9; // FIXME. REMOVEME. might not work for scale-free graphs
	unsigned int keyvaluesread = 0;
	
	std::tuple<uint32_t,uint32_t,bool> res = graphobj->getreader_activevertexids(readerbank)->Next();
	while ( std::get<2>(res) ) {
		uint32_t key = std::get<0>(res);
		uint32_t val = std::get<1>(res);
		
		res = graphobj->getreader_activevertexids(readerbank)->Next();
		generatekeyvalues_random(key, val, groupid, bank, col, batch, batchoffset, batchsize, &keyvaluesread); 
		
		if(keyvaluesread > kvthreshold){  
			cout<<"edge_process::generateupdates_random reached threshold. keyvaluesread: "<<keyvaluesread<<", status: NOTFINISHED "<<endl;
			results[resultbank].datasize = keyvaluesread;
			results[resultbank].message = NOTFINISHED;
			return;
		}
	}
	cout<<"edge_process::generateupdates_random. reached end-of-file. keyvaluesread: "<<keyvaluesread<<", status: FINISHED. "<<endl;
	results[resultbank].datasize = keyvaluesread;
	results[resultbank].message = FINISHED;
	return; 
}
void edge_process::generatekeyvalues_random(vertex_t key, value_t val, unsigned int groupid, unsigned int bank, unsigned int col, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], unsigned int * keyvaluesread){
	unsigned int tempbatchoffset[NUMSUBCPUTHREADS]; // LEARNFROMME. avoids significant bottleneck
	unsigned int tempbatchsize[NUMSUBCPUTHREADS];
	utilityobj->copy(tempbatchoffset, batchoffset, NUMSUBCPUTHREADS);
	utilityobj->copy(tempbatchsize, batchsize, NUMSUBCPUTHREADS);
	
	size_t edge_element_bytes = sizeof(edgeprop2_t);
	size_t byte_offset = ((size_t)key)*sizeof(bfsvertexoffset_t);
	bfsvertexoffset_t mp_vidx_buffer[2];
	pread(graphobj->getnvmeFd_edgeoffsets_r2()[bank][col], mp_vidx_buffer, (2 * sizeof(bfsvertexoffset_t)), byte_offset);

	bfsvertexoffset_t byte_offset_1 = mp_vidx_buffer[0];
	uint32_t fanout = mp_vidx_buffer[1] - mp_vidx_buffer[0];
	if(key >= (graphobj->get_num_vertices()-1)){ fanout = 0; } // FIXME. EDGE CONDITIONS
	
	value_t edgeval = algorithmobj->edge_program(key, val, fanout);
	
	for (vertex_t i = 0; i < fanout; i++){
		uint64_t edge_offset = (byte_offset_1*edge_element_bytes)+(i*edge_element_bytes);
		if ((edge_offset < m_edge_buffer_offset) || ((edge_offset + edge_element_bytes) > (m_edge_buffer_offset+m_edge_buffer_bytes))){
			uint64_t byte_offset_aligned = edge_offset&(~0x3ff); // 1 KB alignment
	
			pread(graphobj->getnvmeFd_edgeproperties_r2()[bank][col], mp_edge_buffer, m_buffer_alloc_bytes, byte_offset_aligned);
			m_edge_blocks_read++;
			m_edge_buffer_offset = byte_offset_aligned;
			m_edge_buffer_bytes = m_buffer_alloc_bytes;
		}
		
		uint64_t internal_offset = edge_offset - m_edge_buffer_offset;		
		vertex_t neighbor = *((vertex_t*)(((uint8_t*)mp_edge_buffer)+internal_offset));
		
		keyvalue_t data;
		data.key = neighbor;
		data.value = edgeval;
		
		insertkeyvaluetobuffer(batch, tempbatchoffset, tempbatchsize, data, 0, groupid);
		
		*keyvaluesread += 1;
	}
	utilityobj->copy(batchsize, tempbatchsize, NUMSUBCPUTHREADS);
	return;
}

void edge_process::generateupdates(unsigned int readerbank, unsigned int bank, unsigned int col, keyvalue_t * batch[NUMCPUTHREADS], vertex_t batchoffset, kvresults_t * results){
	for (int i = 0; i < NUMCPUTHREADS; i++){ generateupdates_random(i, readerbank, bank, col, batch[i], batchoffset, results); }
	#ifdef XXX
	#ifdef LOCKE
	for (int i = 0; i < NUMCPUTHREADS; i++){ generateupdates_random(i, readerbank, bank, col, batch[i], batchoffset, results); }
	#else
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[i] = std::thread(&edge_process::generateupdates_random, this, i, readerbank, bank, col, batch[i], batchoffset, results); }
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[i].join(); }
	#endif
	#endif 
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ statsobj->appendkeyvaluecount(bank, col, results[i].datasize); }
	return;
}
void edge_process::generateupdates_random(int resultbank, unsigned int readerbank, unsigned int bank, unsigned int col, keyvalue_t * batch, vertex_t batchoffset, kvresults_t * results){
	unsigned int kvthreshold = (KVDATA_BATCHSIZE / 10) * 9; // FIXME. REMOVEME. might not work for scale-free graphs
	unsigned int keyvaluesread = 0;
	
	std::tuple<uint32_t,uint32_t,bool> res = graphobj->getreader_activevertexids(readerbank)->Next();
	while ( std::get<2>(res) ) {
		uint32_t key = std::get<0>(res);
		uint32_t val = std::get<1>(res);
		
		res = graphobj->getreader_activevertexids(readerbank)->Next();
		generatekeyvalues_random(key, val, bank, col, batch, batchoffset, &keyvaluesread); 
		
		if(keyvaluesread > kvthreshold){  
			cout<<"edge_process::generateupdates_random reached threshold. keyvaluesread: "<<keyvaluesread<<", status: NOTFINISHED "<<endl;
			results[resultbank].datasize = keyvaluesread;
			results[resultbank].message = NOTFINISHED;
			return;
		}
	}
	cout<<"edge_process::generateupdates_random. reached end-of-file. keyvaluesread: "<<keyvaluesread<<", status: FINISHED. "<<endl;
	results[resultbank].datasize = keyvaluesread;
	results[resultbank].message = FINISHED;
	return; 
}
void edge_process::generatekeyvalues_random(vertex_t key, value_t val, unsigned int bank, unsigned int col, keyvalue_t * batch, vertex_t batchoffset, unsigned int * keyvaluesread){
	size_t edge_element_bytes = sizeof(edgeprop2_t);
	size_t byte_offset = ((size_t)key)*sizeof(bfsvertexoffset_t);
	bfsvertexoffset_t mp_vidx_buffer[2];
	pread(graphobj->getnvmeFd_edgeoffsets_r2()[bank][col], mp_vidx_buffer, (2 * sizeof(bfsvertexoffset_t)), byte_offset);

	bfsvertexoffset_t byte_offset_1 = mp_vidx_buffer[0];
	uint32_t fanout = mp_vidx_buffer[1] - mp_vidx_buffer[0];
	if(key >= (graphobj->get_num_vertices()-1)){ fanout = 0; } // FIXME. EDGE CONDITIONS
	
	value_t edgeval = algorithmobj->edge_program(key, val, fanout);
	
	for (vertex_t i = 0; i < fanout; i++){
		uint64_t edge_offset = (byte_offset_1*edge_element_bytes)+(i*edge_element_bytes);
		if ((edge_offset < m_edge_buffer_offset) || ((edge_offset + edge_element_bytes) > (m_edge_buffer_offset+m_edge_buffer_bytes))){
			uint64_t byte_offset_aligned = edge_offset&(~0x3ff); // 1 KB alignment
	
			pread(graphobj->getnvmeFd_edgeproperties_r2()[bank][col], mp_edge_buffer, m_buffer_alloc_bytes, byte_offset_aligned);
			m_edge_blocks_read++;
			m_edge_buffer_offset = byte_offset_aligned;
			m_edge_buffer_bytes = m_buffer_alloc_bytes;
		}
		
		uint64_t internal_offset = edge_offset - m_edge_buffer_offset;		
		vertex_t neighbor = *((vertex_t*)(((uint8_t*)mp_edge_buffer)+internal_offset));
		
		keyvalue_t kv;
		kv.key = neighbor;
		kv.value = edgeval;
		
		batch[batchoffset + *keyvaluesread] = kv;
		*keyvaluesread += 1;
	}
}

unsigned int edge_process::insertkeyvaluetobuffer(keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], keyvalue_t keyvalue, unsigned int voffset, unsigned int groupid){
	#if NUMSUBCPUTHREADS == 1
	unsigned int partition = 0;
	#else 
	unsigned int partition = (keyvalue.key - voffset) / parametersobj->GET_BATCH_RANGE(groupid);
	#endif 
	
	utilityobj->checkoutofbounds("edge_process::insertkeyvaluetobuffer 2", partition, NUMSUBCPUTHREADS, keyvalue.key, parametersobj->GET_BATCH_RANGE(groupid), voffset); //  (MESSAGESDRAMSZ + KVDRAMBUFFERSZ + KVDRAMSZ)

	batch[partition][batchoffset[partition] + batchsize[partition]] = keyvalue;
	batchsize[partition] += 1;
	return partition;
}

runsummary_t edge_process::timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms){
	return statsobj->timingandsummary(graph_iterationidx, totaltime_ms);
}
void edge_process::resetstatevariables(){
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ currentvid[i] = 0; }
	return;
}



