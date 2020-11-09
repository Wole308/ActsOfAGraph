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
void swkernel::launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_vec_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("swkernel::launchkernel:: print kvdram (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printmessages("swkernel::launchkernel:: print messages (before kernel launch)", (uint512_vec_dt *)(&kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS])); }}
	#endif
	
	#ifdef ACTSMODEL
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->allignandappendinvalids((keyvalue_t *)kvsourcedram[i][j], kvstats[i][j][BASEOFFSET_STATSDRAM + 0].value); }} // edge conditions
	#endif 
	
	#ifdef ACTSMODEL
	#ifdef LOCKE
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ workerthread_acts(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j], kvdestdram[i][j], kvstats[i][j]); }}
	#else 
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mykernelthread[i][j] = std::thread(&swkernel::workerthread_acts, this, i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j], kvdestdram[i][j], kvstats[i][j]); }}
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mykernelthread[i][j].join(); }}
	#endif
	#endif 
	
	#ifdef ACTSMODEL_LW
	#ifdef LOCKE
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ workerthread_actslw(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j]); }}
	#else 
		// FIXME.
		cout<<"swkernel::launchkernel:: ERROR. launch kernel with _NOLOCKE not yet implemented (swkernel.cpp). EXITING"<<endl; exit(EXIT_FAILURE);
	#endif
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("swkernel::launchkernel:: Print results after Kernel run", (keyvalue_t *)kvsourcedram[i][j], 16); }}
	for(unsigned int value=0; value<24; value++){ for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->countkeyvalueswithvalueequalto("swkernel::launchkernel", (keyvalue_t *)kvdestdram[i][j], KVDATA_RANGE_PERSSDPARTITION, value); }}}			
	#endif
	return;
}
void swkernel::launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef ACTSMODEL_LW
	#ifdef LOCKE
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ workerthread_actslw(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j]); 
		// exit(EXIT_SUCCESS); 
		break;
		}}
		// workerthread_actslw(0, kvsourcedram[0][0]);
		// workerthread_actslw(1, kvsourcedram[0][1]);
		// workerthread_actslw(1, kvsourcedram[0][2]);
		// workerthread_actslw(1, kvsourcedram[0][7]);
	#else 
		// FIXME.
		cout<<"swkernel::launchkernel:: ERROR. launch kernel with _NOLOCKE not yet implemented (swkernel.cpp). EXITING"<<endl; exit(EXIT_FAILURE);
	#endif
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












