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
#ifdef ACTSMODEL
#include "../acts/acts1/acts.h"
#endif
#ifdef ACTSMODEL_LW
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
		#ifdef ACTSMODEL
		kernelobjs[i] = new acts();
		#endif 
		#ifdef ACTSMODEL_LW
		kernelobjs[i] = new actslw(); 
		#endif 
	}
	#endif 
}
swkernel::~swkernel(){} 

#ifdef SW
void swkernel::launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef ACTSMODEL_LW
	// #ifdef LOCKE // REMOVEME.
		for (int i = 0; i < NUMCPUTHREADS; i++){ 
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				// utilityobj->paddkeyvalues((keyvalue_t *)&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS], kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key, INVALIDDATA);
				workerthread_actslw(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j]); 
				// exit(EXIT_SUCCESS); 
				// break;
			}
		}
		// workerthread_actslw(0, kvsourcedram[0][0]);
		// workerthread_actslw(1, kvsourcedram[0][1]);
		// workerthread_actslw(1, kvsourcedram[0][2]);
		// workerthread_actslw(1, kvsourcedram[0][7]);
	/* #else 
		// FIXME.
		cout<<"swkernel::launchkernel:: ERROR. launch kernel with _NOLOCKE not yet implemented (swkernel.cpp). EXITING"<<endl; exit(EXIT_FAILURE);
	#endif */
	#endif 
	return;
}

// worker threads
#ifdef ACTSMODEL
void swkernel::workerthread_acts(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram, uint512_vec_dt * kvdestdram, keyvalue_t * kvstats){
	kernelobjs[ithreadidx]->topkernel((uint512_dt *)kvsourcedram, kvdestdram, kvstats);
	return;
}
#endif
#ifdef ACTSMODEL_LW
void swkernel::workerthread_actslw(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram){
	kernelobjs[ithreadidx]->topkernel((uint512_dt *)kvsourcedram);
	return;
}
#endif

void swkernel::finishOCL(){
	return;
}
#endif 












