#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "helperfunctions2.h"
using namespace std;

helperfunctions2::helperfunctions2(graph * _graphobj, stats * _statsobj){
	parametersobj = new parameters();
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	kernelobj = new kernel();
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif
	statsobj = _statsobj;
}
helperfunctions2::helperfunctions2(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
	kernelobj = new kernel();
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif 
}
helperfunctions2::~helperfunctions2(){} 

void helperfunctions2::createmessages(
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int firstvid,
			unsigned int firstkey,
			unsigned int firstvalue,
			unsigned int treedepth,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			unsigned int batch_range,
			unsigned int batch_range_pow,
			unsigned int applyvertexbuffersz,
			unsigned int numlastlevelpartitions){
				// cout<<"------------- runsize: "<<runsize<<endl;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key = srcvoffset;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key = srcvsize;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE_KVS].data[0].key = (srcvsize + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE].data[0].key = edgessize;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE_KVS].data[0].key = (edgessize + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_DESTVOFFSET].data[0].key = destvoffset;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTVID].data[0].key = firstvid;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTKEY].data[0].key = firstkey;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTVALUE].data[0].key = firstvalue;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key = treedepth;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key = GraphIter;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key = GraphAlgo;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key = 0; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key = runsize; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key = runsize; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE_KVS].data[0].key = (runsize + (VECTOR_SIZE - 1)) / VECTOR_SIZE; 
	
	if(runsize == 0 || runsize >= KVSOURCEDRAMSZ){
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = 0;
	} else {
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 2;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1; // REMOVEME
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 1;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0; // REMOVEME
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
	}
	
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].data[0].key = batch_range; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_KVS].data[0].key = batch_range / VECTOR_SIZE; // batch_range_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].data[0].key = batch_range_pow;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].data[0].key = applyvertexbuffersz; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].data[0].key = applyvertexbuffersz / VECTOR_SIZE; // applyvertexbuffersz_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key = numlastlevelpartitions;
	return;
}

// launch kernel
void helperfunctions2::launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions2::launchkernel:: Print kvdram (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printmessages("helperfunctions2::launchkernel:: messages (before kernel launch)", (uint512_vec_dt *)(&kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS])); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions2::launchkernel:: Print vertex datas (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_VERTICESDATA_KVS]), 16); }}
	#endif
	#ifdef _DEBUGMODE_TIMERS2
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef ACTGRAPH_SETUP
	kernelobj->launchkernel(kvsourcedram, flag);
	#endif 
	#ifdef GRAFBOOST_SETUP
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			srkernelobj->srtopkernel(_sr, (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key);
		}
	}
	#endif 
	
	#ifdef _DEBUGMODE_TIMERS2
	long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions2::launchkernel:: Print results (after Kernel run)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int value=0; value<24; value++){ for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->countkeyvalueswithvalueequalto("helperfunctions2::launchkernel", (keyvalue_t *)kvdestdram[i][j], KVDATA_RANGE_PERSSDPARTITION, value); }}}		
	#endif
	return;
}

void helperfunctions2::cummulateverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"... cummulating vertex datas... ["<<NUMCPUTHREADS<<" threads, "<<NUMSUBCPUTHREADS<<" subthreads]"<<endl;
	#endif 
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_cummulateverticesdata(i, buffer, (i * (BATCH_RANGE / NUMUTILITYTHREADS)), (BATCH_RANGE / NUMUTILITYTHREADS)); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mykernelthread[i] = std::thread(&helperfunctions2::workerthread_cummulateverticesdata, this, i, buffer, (i * (BATCH_RANGE / NUMUTILITYTHREADS)), (BATCH_RANGE / NUMUTILITYTHREADS)); }
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mykernelthread[i].join(); }
	#endif 
	return;
}
void helperfunctions2::workerthread_cummulateverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	unsigned int baseoffset = BASEOFFSET_VERTICESDATA * (sizeof(keyvalue_t) / sizeof(value_t));
	unsigned int onceactivecnt = 0;
	value_t cumm = INFINITI;
	
	for(unsigned int k=offset; k<(offset + size); k++){
		cumm = buffer[0][0][baseoffset];
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			cumm = algorithmobj->cummulate(cumm, buffer[0][j][baseoffset + k]);
			
			if(cumm < INFINITI){ onceactivecnt += 1; }
			#ifdef _DEBUGMODE_HOSTPRINTS2
			if(cumm < INFINITI){ cout<<"cummulateverticesdata: once active vertex seen @ "<<k<<": cumm: "<<cumm<<endl; }
			#endif
		}
		buffer[0][0][baseoffset + k] = cumm;
	}
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"workerthread_cummulateverticesdata: number of vertex ids once active: "<<onceactivecnt<<endl;
	#endif 
	return;
}

void helperfunctions2::applyvertices(vector<value_t> &activeverticesbuffer, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset, unsigned int vsize){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"... applying vertex datas... ["<<NUMCPUTHREADS<<" threads, "<<NUMSUBCPUTHREADS<<" subthreads]"<<endl;
	#endif 
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_applyvertices(i, activeverticesbuffer, buffer, voffset, i * (vsize / NUMUTILITYTHREADS), (vsize / NUMUTILITYTHREADS)); }
	#else
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&helperfunctions2::workerthread_applyvertices, this, i, activeverticesbuffer, buffer, voffset, i * (vsize / NUMUTILITYTHREADS), (vsize / NUMUTILITYTHREADS)); }	
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif
	return;
}
void helperfunctions2::workerthread_applyvertices(int ithreadidx, vector<value_t> &activeverticesbuffer, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset, vertex_t offset, vertex_t size){		
	value_t * vertexdatabuffer = graphobj->getvertexdatabuffer();
	unsigned int baseoffset = BASEOFFSET_VERTICESDATA * (sizeof(keyvalue_t) / sizeof(value_t));
	unsigned int onceactivecnt = 0;
	
	for(unsigned int k=0; k<size; k++){
		value_t kvtempdata = buffer[0][0][baseoffset + k];
		value_t vdata = vertexdatabuffer[voffset + offset + k];
		value_t temp = algorithmobj->apply(kvtempdata, vdata);
		vertexdatabuffer[voffset + offset + k] = temp;
	
		if(temp != vdata){
			onceactivecnt += 1; 
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"applyvertices: active vertex seen @ "<<k<<": vid: "<<voffset + offset + k<<", temp: "<<temp<<", vdata: "<<vdata<<endl; 
			#endif 
			
			activeverticesbuffer.push_back((voffset + offset + k));
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"workerthread_applyvertices: number of vertex ids once active: "<<onceactivecnt<<endl;
	cout<<"workerthread_applyvertices: number of vertex ids in activeverticesbuffer: "<<activeverticesbuffer.size()<<endl;
	utilityobj->countvalueslessthan("workerthread_applyvertices", vertexdatabuffer, KVDATA_RANGE, INFINITI);
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"applyvertices: number of active vertices for next iteration: "<<activeverticesbuffer.size()<<endl;
	#endif 
	return;
}

void helperfunctions2::trim(container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			container->vertexptrs[i][j][0] = container->edgeoffset[i][j];
			container->vertexptrs[i][j][container->srcvsize[i][j]-1] = container->edgeoffset[i][j] + container->edgessize[i][j];
			for(unsigned int t=0; t<8; t++){ container->vertexptrs[i][j][container->srcvsize[i][j]-1+t] = container->edgeoffset[i][j] + container->edgessize[i][j]; } // error avoidance within ACTS
			
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"trim["<<i<<"]["<<j<<"]: first data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"][0]: "<<container->vertexptrs[i][j][0]<<endl;
			cout<<"trim["<<i<<"]["<<j<<"]: last data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"]["<<container->srcvsize[i][j]-1<<"]: "<<container->vertexptrs[i][j][container->srcvsize[i][j]-1]<<endl;
			#endif 
		}
	}
	return;
}
void helperfunctions2::trim2(container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			container->vertexptrs[i][j][0] = 0;
			container->vertexptrs[i][j][container->srcvsize[i][j]-1] = container->edgessize[i][j];// - 1; // NEWCHANGE.
			
			// vertexptrs[i][j][0] = container->edgeoffset[i][j]; // trim
			// vertexptrs[i][j][numvertexptrstoload-1] = container->edgeoffset[i][j] + container->edgessize[i][j];
			
			for(unsigned int t=0; t<488 * 8 * 8; t++){ container->vertexptrs[i][j][container->srcvsize[i][j]-1+t] = container->edgessize[i][j]; } // error avoidance
			
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"trim["<<i<<"]["<<j<<"]: first data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"][0]: "<<container->vertexptrs[i][j][0]<<endl;
			cout<<"trim["<<i<<"]["<<j<<"]: last data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"]["<<container->srcvsize[i][j]-1<<"]: "<<container->vertexptrs[i][j][container->srcvsize[i][j]-1]<<endl;
			#endif 
		}
	}
	return;
}
void helperfunctions2::loadsourcevertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int vid = 0; vid < container->srcvsize[i][j]; vid++){
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + vid].key = container->vertexptrs[i][j][vid];
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + vid].value = vertexdatabuffer[container->firstvid[i][j] + vid]; // 10000000 + vid; // vertexdatabuffer[vid]; // 10000000 + k;
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj->printkeyvalues("helperfunctions2::loadsourcevertices", &kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE], 16);
			#endif 
		}
	}
	return;
}
void helperfunctions2::loadsourcevertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vector<vertex_t> &srcvids, container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int k = 0; k < container->srcvsize[i][j]; k++){
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + k].key = container->vertexptrs[i][j][k];
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + k].value = vertexdatabuffer[srcvids[k]];
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj->printkeyvalues("helperfunctions2::loadsourcevertices", &kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE], 16);
			#endif 
		}
	}
	return;
}
void helperfunctions2::loaddestvertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t offset, vertex_t size){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			memcpy(&kvbuffer[i][j][BASEOFFSET_VERTICESDATA], &vertexdatabuffer[offset], size * sizeof(value_t));
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj->printkeyvalues("helperfunctions2::loaddestvertices", &kvbuffer[i][j][BASEOFFSET_VERTICESDATA], 16);
			#endif 
		}
	}
	return;
}
void helperfunctions2::loadedges(keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int k=0; k<container->edgessize[i][j]; k++){
				kvbuffer[i][j][BASEOFFSET_KVDRAM + k].key = container->edgesbuffer[i][j][k].dstvid;
				kvbuffer[i][j][BASEOFFSET_KVDRAM + k].value = container->edgesbuffer[i][j][k].srcvid;
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj->printkeyvalues("helperfunctions2::loadedges", &kvbuffer[i][j][BASEOFFSET_KVDRAM], 16);
			#endif 
		}
	}
	return;
}
void helperfunctions2::loadmessages(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo){	
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
			if(container->srcvsize[i][j] >= KVDRAMSZ){ cout<<"helperfunctions2::run::ERROR: fix this. srcvsize is greater than allowed. srcvsize["<<i<<"]["<<j<<"]: "<<container->srcvsize[i][j]<<", KVDRAMSZ: "<<KVDRAMSZ<<endl; exit(EXIT_FAILURE); }
				createmessages(
					kvbuffer[i][j], // uint512_vec_dt * kvstats,
					container->srcvoffset[i][j], // unsigned int srcvoffset,
					container->srcvsize[i][j], // unsigned int srcvsize,
					container->edgessize[i][j], // unsigned int edgessize,
					container->destvoffset[i][j], // unsigned int destvoffset,
					container->firstvid[i][j], // unsigned int firstvid,
					container->vertexptrs[i][j][0], // unsigned int firstkey,
					NAp, // unsigned int firstvalue,
					TREE_DEPTH, // unsigned int treedepth,
					0, // unsigned int GraphIter,
					GraphAlgo, // PAGERANK, // unsigned int GraphAlgo,
					container->edgessize[i][j], // unsigned int runsize,
					BATCH_RANGE, // unsigned int batch_range,
					BATCH_RANGE_POW, // unsigned int batch_range_pow,
					APPLYVERTEXBUFFERSZ, // unsigned int applyvertexbuffersz,
					NUMLASTLEVELPARTITIONS); // unsigned int numlastlevelpartitions
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"loadmessages:: running Acts... size: "<<container->edgessize[i][j]<<endl; 
			#endif 
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadmessages:: running Acts... sizes: ["; 
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ cout<<"["; for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ cout<<container->edgessize[i][j]; if(j<NUMSUBCPUTHREADS-1){ cout<<", "; }} cout<<"]"; }
	cout<<"]"<<endl;
	#endif 
	return;
}
edge_t helperfunctions2::countedges(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, container_t * container){
	edge_t edgessz = 0;
	for(unsigned int k=0; k<srcvids.size(); k++){
		graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[0][0], 0, 2); 
		edgessz += container->tempvertexptrs[0][0][1] - container->tempvertexptrs[0][0][0];
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"helperfunctions2::countedges:: total number of edges: "<<edgessz<<endl;
	#endif 
	return edgessz;
}

unsigned int helperfunctions2::getflag(unsigned int globaliteration_idx){
	#ifdef FPGA_IMPL
	int flag = globaliteration_idx % NUMFLAGS;
	#else 
	int flag = 0;
	#endif 
	return flag;
}
#ifdef FPGA_IMPL 
void helperfunctions2::loadOCLstructures(std::string binaryFile, uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram);
}
void helperfunctions2::writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->writetokernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void helperfunctions2::readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->readfromkernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void helperfunctions2::finishOCL(){
	kernelobj->finishOCL();
}
#endif 
#ifdef GRAFBOOST_SETUP 
inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	float af = *(float*)&a;
	float bf = *(float*)&b;
	float tf = af+bf;
	
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	float af = *(float*)&value;
	float tf = af/(float)fanout;

	uint32_t ret = *(uint32_t*)&tf;

	return ret;
}
size_t g_vertex_count = 1;
inline uint32_t finalize_program(uint32_t oldval, uint32_t val) {
	float damp = 0.15/(float)g_vertex_count;
	float af = *(float*)&val;
	float tf = damp + af*0.85;
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	if ( !marked ) return false;

	float af = *(float*)&old;
	float bf = *(float*)&newv;

	if ( af > bf ) {
		if (af-bf < 0.0001 ) return false;
		else return true;
	} else {
		if (bf-af < 0.0001 ) return false;
		else return true;
	}
}
void helperfunctions2::loadSRstructures(){
	string tmp_dir = graphobj->gettmp_dir();
	string idx_path = graphobj->getidx_path();
	string mat_path = graphobj->getmat_path();
	cout<<"helperfunctions2::loadSRstructures:: tmp_dir: "<<tmp_dir<<endl;
	cout<<"helperfunctions2::loadSRstructures:: idx_path: "<<idx_path<<endl;
	cout<<"helperfunctions2::loadSRstructures:: mat_path: "<<mat_path<<endl;
	cout<<"helperfunctions2::loadSRstructures:: vertex_count: "<<graphobj->get_num_vertices()<<endl;
	
	int max_thread_count = 12;
	int max_sr_thread_count = 20;
	int max_vertexval_thread_count = 8;
	int max_edgeproc_thread_count = 8;

	// int max_vertexval_thread_count = NUMCPUTHREADS;
	size_t vertex_count = graphobj->get_num_vertices();

	SortReduceTypes::Config<uint64_t,uint32_t>* conf = new SortReduceTypes::Config<uint64_t,uint32_t>(tmp_dir.c_str(), "output.dat", max_sr_thread_count); // (NUMCPUTHREADS + 2)
	conf->SetUpdateFunction(&vertex_update);
	_sr = new SortReduce<uint64_t,uint32_t>(conf);

	float init_val2 = 1.0/(float)vertex_count;
	vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, *(uint32_t*)&init_val2, &is_active, &finalize_program, max_vertexval_thread_count);	
}
void helperfunctions2::finishSR(){
	#ifdef _DEBUGMODE_TIMERS2
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	_sr->Finish();
	SortReduceTypes::Status status = _sr->CheckStatus();
	while ( status.done_external == false ) {
		sleep(1);
		status = _sr->CheckStatus();
		printf( "%s %s:%d-%d %s:%d-%d\n", status.done_input?"yes":"no", status.done_inmem?"yes":"no",status.internal_count, status.sorted_count, status.done_external?"yes":"no", status.external_count, status.file_count );		
		fflush(stdout);
	}
	
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::tuple<uint32_t,uint32_t,bool> res2 = _sr->Next();
	vertex_values->Start();
	while ( std::get<2>(res2) ) {
		uint32_t key = std::get<0>(res2);
		uint32_t val = std::get<1>(res2);

		// printf( "\t\t++ SRR %i %i\n", key, val );
		while ( !vertex_values->Update(key,val) ) ;

		res2 = _sr->Next();
	}
	vertex_values->Finish();
	#ifdef _DEBUGMODE_TIMERS2
	long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	#endif
	
	size_t active_cnt = vertex_values->GetActiveCount();
	cout<<">>> helperfunctions2::finishSR:: active vertices after iteration: "<<active_cnt<<endl;
	vertex_values->NextIteration();		
	delete _sr;
}
#endif 







