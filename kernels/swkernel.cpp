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
#include "../acts/acts/acts.h"
#include "../acts/acts/actsproc.h"
#include "../acts/acts/actssync.h"
#include "../acts/acts_sw/acts_sw.h"
#include "../src/stats/stats.h"
#include "../src/algorithm/algorithm.h"
#include "../src/graphs/graph.h"
#include "../src/utility/utility.h"
#include "../include/common.h"
#include "swkernel.h"
using namespace std;

// #define SYNC32

swkernel::swkernel(graph * _graphobj, algorithm * _algorithmobj, stats * _statsobj){
	utilityobj = new utility();
	statsobj = _statsobj;
	graphobj = _graphobj;
	algorithmobj = _algorithmobj;
	
	#ifdef SW
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs[i] = new acts(); }
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i] = new actsproc(); }
	kernelobjs_synchronize = new actssync();
	
	actssw_obj = new acts_sw();
	#endif
}
swkernel::~swkernel(){}

#ifdef SW
// #ifdef KOKOOOOOOOO
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef NACTS_IN_NCOMPUTEUNITS
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
		
		#ifndef SW_IMPL
		kernelobjs_synchronize->topkernelsync( // topkernelsync_sw
			(uint512_dt *)kvsourcedram[0],
			(uint512_dt *)kvsourcedram[1],
			#if NUMCOMPUTEUNITS>2
			(uint512_dt *)kvsourcedram[2],
			(uint512_dt *)kvsourcedram[3],
			#if NUMCOMPUTEUNITS>4
			(uint512_dt *)kvsourcedram[4],
			(uint512_dt *)kvsourcedram[5],
			(uint512_dt *)kvsourcedram[6],
			(uint512_dt *)kvsourcedram[7],
			#if NUMCOMPUTEUNITS>8
			(uint512_dt *)kvsourcedram[8],
			(uint512_dt *)kvsourcedram[9],
			(uint512_dt *)kvsourcedram[10],
			(uint512_dt *)kvsourcedram[11],
			#if NUMCOMPUTEUNITS>12
			(uint512_dt *)kvsourcedram[12],
			(uint512_dt *)kvsourcedram[13],
			(uint512_dt *)kvsourcedram[14],
			(uint512_dt *)kvsourcedram[15],
			#if NUMCOMPUTEUNITS>16
			(uint512_dt *)kvsourcedram[16],
			(uint512_dt *)kvsourcedram[17],
			(uint512_dt *)kvsourcedram[18],
			(uint512_dt *)kvsourcedram[19],
			#if NUMCOMPUTEUNITS>20
			(uint512_dt *)kvsourcedram[20],
			(uint512_dt *)kvsourcedram[21],
			(uint512_dt *)kvsourcedram[22],
			(uint512_dt *)kvsourcedram[23],
			#if NUMCOMPUTEUNITS>24
			(uint512_dt *)kvsourcedram[24],
			(uint512_dt *)kvsourcedram[25],
			(uint512_dt *)kvsourcedram[26],
			(uint512_dt *)kvsourcedram[27],
			#if NUMCOMPUTEUNITS>28
			(uint512_dt *)kvsourcedram[28],
			(uint512_dt *)kvsourcedram[29],
			(uint512_dt *)kvsourcedram[30],
			(uint512_dt *)kvsourcedram[31],
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif 
			(uint512_dt *)vdram
		);
		#endif
		
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		unsigned int BLOP = pow(NUM_PARTITIONS, (TREE_DEPTH-1));
		unsigned int totalactvvp = 0;
		cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		for(unsigned int i=0; i<256; i++){
			unsigned int gmask = kvsourcedram[0][_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
			totalactvvp += gmask;
			if(gmask > 0){ cout<<i<<", "; }
		}
		cout<<""<<endl;
		if(totalactvvp == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
	}
	#endif
	
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
}
// #endif 
#ifdef KOKOOOOOOOO
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	uint512_vec_dt * tempkvsourcedram[NUMSUBCPUTHREADS];
	uint512_vec_dt * tempvdram;
	
	unsigned int _BASEOFFSETKVS_MESSAGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].data[0].key;
	unsigned int _BASEOFFSETKVS_EDGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key;
	unsigned int _BASEOFFSETKVS_VERTICESDATA = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	unsigned int _BASEOFFSETKVS_KVDRAM = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key;
	unsigned int _MSIZE = _BASEOFFSETKVS_EDGESDATA - _BASEOFFSETKVS_MESSAGESDATA;
	unsigned int _VSIZE = _BASEOFFSETKVS_KVDRAM - _BASEOFFSETKVS_VERTICESDATA;
	unsigned int _TOTSIZE = _MSIZE + _VSIZE;
	
	for(unsigned int i=0; i<NUMSYNCTHREADS; i++){
		tempkvsourcedram[i] = new uint512_vec_dt[_TOTSIZE];
	}
	tempvdram = new uint512_vec_dt[_TOTSIZE];

	unsigned int _OFF_KVS = 0;
	for(unsigned int i=0; i<NUMSYNCTHREADS; i++){
		memcpy(&tempkvsourcedram[i][0], &kvsourcedram[i][_BASEOFFSETKVS_MESSAGESDATA], MESSAGESDRAMSZ * sizeof(uint512_vec_dt));
	}
	memcpy(&tempvdram[i][0], &vdram[i][_BASEOFFSETKVS_MESSAGESDATA], MESSAGESDRAMSZ * sizeof(uint512_vec_dt));
	_OFF_KVS += _MSIZE; // MESSAGESDRAMSZ;
	for(unsigned int i=0; i<NUMSYNCTHREADS; i++){
		memcpy(&tempkvsourcedram[i][0 + _OFF_KVS], &kvsourcedram[i][_BASEOFFSETKVS_VERTICESDATA], _TOTSIZE * sizeof(uint512_vec_dt));
	}
	memcpy(&tempvdram[i][0 + _OFF_KVS], &vdram[i][_BASEOFFSETKVS_VERTICESDATA], _TOTSIZE * sizeof(uint512_vec_dt));
	_OFF_KVS += _VSIZE; // _TOTSIZE;
	
	#ifdef NACTS_IN_NCOMPUTEUNITS
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
		
		#ifndef SW_IMPL
		kernelobjs_synchronize->topkernelsync( // topkernelsync_sw
			ON,
			ON,
			(uint512_dt *)tempkvsourcedram[0],
			(uint512_dt *)tempkvsourcedram[1],
			#if NUMSYNCTHREADS>2
			(uint512_dt *)tempkvsourcedram[2],
			(uint512_dt *)tempkvsourcedram[3],
			#if NUMSYNCTHREADS>4
			(uint512_dt *)tempkvsourcedram[4],
			(uint512_dt *)tempkvsourcedram[5],
			(uint512_dt *)tempkvsourcedram[6],
			(uint512_dt *)tempkvsourcedram[7],
			#if NUMSYNCTHREADS>8
			(uint512_dt *)tempkvsourcedram[8],
			(uint512_dt *)tempkvsourcedram[9],
			(uint512_dt *)tempkvsourcedram[10],
			(uint512_dt *)tempkvsourcedram[11],
			#if NUMSYNCTHREADS>12
			(uint512_dt *)tempkvsourcedram[12],
			(uint512_dt *)tempkvsourcedram[13],
			(uint512_dt *)tempkvsourcedram[14],
			(uint512_dt *)tempkvsourcedram[15],
			#if NUMSYNCTHREADS>16
			(uint512_dt *)tempkvsourcedram[16],
			(uint512_dt *)tempkvsourcedram[17],
			(uint512_dt *)tempkvsourcedram[18],
			(uint512_dt *)tempkvsourcedram[19],
			#if NUMSYNCTHREADS>20
			(uint512_dt *)tempkvsourcedram[20],
			(uint512_dt *)tempkvsourcedram[21],
			(uint512_dt *)tempkvsourcedram[22],
			(uint512_dt *)tempkvsourcedram[23],
			#if NUMSYNCTHREADS>24
			(uint512_dt *)tempkvsourcedram[24],
			(uint512_dt *)tempkvsourcedram[25],
			(uint512_dt *)tempkvsourcedram[26],
			(uint512_dt *)tempkvsourcedram[27],
			#if NUMSYNCTHREADS>28
			(uint512_dt *)tempkvsourcedram[28],
			(uint512_dt *)tempkvsourcedram[29],
			(uint512_dt *)tempkvsourcedram[30],
			(uint512_dt *)tempkvsourcedram[31],
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif 
			(uint512_dt *)tempvdram
		);
		#endif
		exit(EXIT_SUCCESS); // REMOVEME.
		
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		unsigned int BLOP = pow(NUM_PARTITIONS, (TREE_DEPTH-1));
		unsigned int totalactvvp = 0;
		cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		for(unsigned int i=0; i<256; i++){
			unsigned int gmask = kvsourcedram[0][_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
			totalactvvp += gmask;
			if(gmask > 0){ cout<<i<<", "; }
		}
		cout<<""<<endl;
		if(totalactvvp == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
	}
	#endif
	
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
}
#endif 
long double swkernel::runapp(edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], value_t * vprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters){				
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	actssw_obj->start(graphobj, edges, vptrs, vprops, actvvs, actvvs_nextit, kvdram, GraphAlgo, numIters);

	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
}
#endif 












