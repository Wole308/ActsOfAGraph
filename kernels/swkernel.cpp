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
#include "../acts/acts/acts_process.h"
#include "../acts/acts/acts_synchronize.h"
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
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i] = new acts_process(); }
	kernelobjs_synchronize = new acts_synchronize();
	#endif 
}
swkernel::~swkernel(){} 

#ifdef SW
void swkernel::runapp(std::string binaryFile, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef NACTS_IN_NCOMPUTEUNITS
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernel((uint512_dt *)kvsourcedram[i]); }
		
		kernelobjs_synchronize->topkernel(
			#if NUMCOMPUTEUNITS==1
			(uint512_dt *)kvsourcedram[0]
			#endif 
			
			#if NUMCOMPUTEUNITS==2
			(uint512_dt *)kvsourcedram[0],
			(uint512_dt *)kvsourcedram[1]
			#endif
			
			#if NUMCOMPUTEUNITS==4
			(uint512_dt *)kvsourcedram[0],
			(uint512_dt *)kvsourcedram[1],
			(uint512_dt *)kvsourcedram[2],
			(uint512_dt *)kvsourcedram[3]
			#endif
			
			#if NUMCOMPUTEUNITS==8
			(uint512_dt *)kvsourcedram[0],
			(uint512_dt *)kvsourcedram[1],
			(uint512_dt *)kvsourcedram[2],
			(uint512_dt *)kvsourcedram[3],
			(uint512_dt *)kvsourcedram[4],
			(uint512_dt *)kvsourcedram[5],
			(uint512_dt *)kvsourcedram[6],
			(uint512_dt *)kvsourcedram[7]
			#endif
			
			#if NUMCOMPUTEUNITS==12
			(uint512_dt *)kvsourcedram[0],
			(uint512_dt *)kvsourcedram[1],
			(uint512_dt *)kvsourcedram[2],
			(uint512_dt *)kvsourcedram[3],
			(uint512_dt *)kvsourcedram[4],
			(uint512_dt *)kvsourcedram[5],
			(uint512_dt *)kvsourcedram[6],
			(uint512_dt *)kvsourcedram[7],
			(uint512_dt *)kvsourcedram[8],
			(uint512_dt *)kvsourcedram[9],
			(uint512_dt *)kvsourcedram[10],
			(uint512_dt *)kvsourcedram[11]
			#endif
			
			#if NUMCOMPUTEUNITS==14
			(uint512_dt *)kvsourcedram[0],
			(uint512_dt *)kvsourcedram[1],
			(uint512_dt *)kvsourcedram[2],
			(uint512_dt *)kvsourcedram[3],
			(uint512_dt *)kvsourcedram[4],
			(uint512_dt *)kvsourcedram[5],
			(uint512_dt *)kvsourcedram[6],
			(uint512_dt *)kvsourcedram[7],
			(uint512_dt *)kvsourcedram[8],
			(uint512_dt *)kvsourcedram[9],
			(uint512_dt *)kvsourcedram[10],
			(uint512_dt *)kvsourcedram[11],
			(uint512_dt *)kvsourcedram[12],
			(uint512_dt *)kvsourcedram[13]
			#endif 
			
			#if NUMCOMPUTEUNITS==16
			(uint512_dt *)kvsourcedram[0],
			(uint512_dt *)kvsourcedram[1],
			(uint512_dt *)kvsourcedram[2],
			(uint512_dt *)kvsourcedram[3],
			(uint512_dt *)kvsourcedram[4],
			(uint512_dt *)kvsourcedram[5],
			(uint512_dt *)kvsourcedram[6],
			(uint512_dt *)kvsourcedram[7],
			(uint512_dt *)kvsourcedram[8],
			(uint512_dt *)kvsourcedram[9],
			(uint512_dt *)kvsourcedram[10],
			(uint512_dt *)kvsourcedram[11],
			(uint512_dt *)kvsourcedram[12],
			(uint512_dt *)kvsourcedram[13],
			(uint512_dt *)kvsourcedram[14],
			(uint512_dt *)kvsourcedram[15]
			#endif 
		);
	}
	#endif
	
	#ifdef NACTS_IN_1COMPUTEUNIT
	kernelobjs[0]->topkernel(
		#if NUMCOMPUTEUNITS==1
		(uint512_dt *)kvsourcedram[0]
		#endif 
		
		#if NUMCOMPUTEUNITS==2
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1]
		#endif
		
		#if NUMCOMPUTEUNITS==4
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3]
		#endif
		
		#if NUMCOMPUTEUNITS==8
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)kvsourcedram[7]
		#endif
		
		#if NUMCOMPUTEUNITS==12
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)kvsourcedram[7],
		(uint512_dt *)kvsourcedram[8],
		(uint512_dt *)kvsourcedram[9],
		(uint512_dt *)kvsourcedram[10],
		(uint512_dt *)kvsourcedram[11]
		#endif
		
		#if NUMCOMPUTEUNITS==14
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)kvsourcedram[7],
		(uint512_dt *)kvsourcedram[8],
		(uint512_dt *)kvsourcedram[9],
		(uint512_dt *)kvsourcedram[10],
		(uint512_dt *)kvsourcedram[11],
		(uint512_dt *)kvsourcedram[12],
		(uint512_dt *)kvsourcedram[13]
		#endif 
		
		#if NUMCOMPUTEUNITS==16
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)kvsourcedram[7],
		(uint512_dt *)kvsourcedram[8],
		(uint512_dt *)kvsourcedram[9],
		(uint512_dt *)kvsourcedram[10],
		(uint512_dt *)kvsourcedram[11],
		(uint512_dt *)kvsourcedram[12],
		(uint512_dt *)kvsourcedram[13],
		(uint512_dt *)kvsourcedram[14],
		(uint512_dt *)kvsourcedram[15]
		#endif 
	);
	#endif 
	
	#ifdef _DEBUGMODE_TIMERS3
	long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	#endif
	return;
}
#endif 












