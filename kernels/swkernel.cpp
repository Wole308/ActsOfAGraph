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
#include "../src/utility/utility.h"
#include "../include/common.h"
#include "swkernel.h"
using namespace std;

swkernel::swkernel(stats * _statsobj){
	utilityobj = new utility();
	statsobj = _statsobj;
	#ifdef SW 
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs[i] = new acts(); }
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i] = new actsproc(); }
	kernelobjs_synchronize = new actssync();
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ swkernelobjs_process[i] = new acts_sw(); }
	#endif 
}
swkernel::~swkernel(){} 

#ifdef SW
long double swkernel::runapp(edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], unsigned int * vdatas, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters){				
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<" ("<<actvvs.size()<<" active vertices)"<<endl;
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			swkernelobjs_process[i]->topkernelproc(edges[i], vptrs[i], vdatas, actvvs, actvvs_nextit, kvdram[i], GraphAlgo, GraphIter);
		}
		
		actvvs.clear();
		actvvs.assign(actvvs_nextit.begin(), actvvs_nextit.end());
		actvvs_nextit.clear();
		
		if(actvvs.size() == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
	}
	
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
}
/* long double swkernel::runapp(edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], unsigned int * vdatas, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters){				
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<" ("<<actvvs.size()<<" active vertices)"<<endl;
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			if(GraphIter % 2 == 0){
				swkernelobjs_process[i]->topkernelproc(edges[i], vptrs[i], vdatas, actvvs, actvvs_nextit, kvdram[i], GraphAlgo, GraphIter);
			} else {
				swkernelobjs_process[i]->topkernelproc(edges[i], vptrs[i], vdatas, actvvs_nextit, actvvs, kvdram[i], GraphAlgo, GraphIter);
			}
		}
		
		if(GraphIter % 2 == 0){
			actvvs.clear();
		} else {
			actvvs_nextit.clear();
		}
		
		// actvvs.clear();
		// actvvs.assign(actvvs_nextit.begin(), actvvs_nextit.end());
		// actvvs_nextit.clear();
		
		// if(actvvs.size() == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
	}
	
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
} */

long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef NACTS_IN_NCOMPUTEUNITS
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
		
		#ifndef SW_IMPL
		kernelobjs_synchronize->topkernelsync_sw(
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
#endif 












