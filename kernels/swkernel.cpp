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
#ifdef TESTKERNEL_FULLBANDWIDTH
#include "../acts/acts_lw/actslw_maxbutil.h"
#else 
#include "../acts/acts_lw/actslw.h"
#endif 
#include "../src/utility/utility.h"
#include "../include/common.h"
#include "swkernel.h"
using namespace std;

swkernel::swkernel(){
	utilityobj = new utility();
	#ifdef SW 
	for(unsigned int i=0; i<NUMCPUTHREADS * NUMSUBCPUTHREADS; i++){ 
		#ifdef TESTKERNEL_FULLBANDWIDTH
		kernelobjs_maxbutil[i] = new actslw_maxbutil(); 
		#else 
		kernelobjs[i] = new actslw(); 
		#endif 
	}
	#endif 
}
swkernel::~swkernel(){} 

#ifdef SW
void swkernel::launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef TESTKERNEL_FULLBANDWIDTH
		#if NUMSUBWORKERS==2
		kernelobjs_maxbutil[0]->topkernel(
								  (uint512_dt *)kvsourcedram[0][0]
								, (uint512_dt *)kvsourcedram[0][1]
								); 
		#endif 
		#if NUMSUBWORKERS==4
		// kernelobjs_maxbutil[0]->topkernel(
								  // (uint512_dt *)kvsourcedram[0][0]
								// , (uint512_dt *)kvsourcedram[0][0]
								// , (uint512_dt *)kvsourcedram[0][0]
								// , (uint512_dt *)kvsourcedram[0][0]
								// ); 
		kernelobjs_maxbutil[0]->topkernel(
								  (uint512_dt *)kvsourcedram[0][0]
								, (uint512_dt *)kvsourcedram[0][1]
								, (uint512_dt *)kvsourcedram[0][2]
								, (uint512_dt *)kvsourcedram[0][3]
								); 
		#endif 
		#if NUMSUBWORKERS==8
		kernelobjs_maxbutil[0]->topkernel(
								  (uint512_dt *)kvsourcedram[0][0]
								, (uint512_dt *)kvsourcedram[0][1]
								, (uint512_dt *)kvsourcedram[0][2]
								, (uint512_dt *)kvsourcedram[0][3]
								, (uint512_dt *)kvsourcedram[0][4]
								, (uint512_dt *)kvsourcedram[0][5]
								, (uint512_dt *)kvsourcedram[0][6]
								, (uint512_dt *)kvsourcedram[0][7]
								); 
		#endif 
	#else 
		for (int i = 0; i < NUMCPUTHREADS; i++){ 
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				// utilityobj->paddkeyvalues((keyvalue_t *)&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS], kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key, INVALIDDATA);
				workerthread_actslw(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j]); 
				// exit(EXIT_SUCCESS); 
				break;
			}
		}
	#endif 
	return;
}

void swkernel::workerthread_actslw(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram){
	#ifndef TESTKERNEL_FULLBANDWIDTH
	kernelobjs[ithreadidx]->topkernel((uint512_dt *)kvsourcedram);
	#endif 
	return;
}

void swkernel::finishOCL(){
	return;
}
#endif 












