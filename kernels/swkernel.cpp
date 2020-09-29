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
#include "../acts/acts/acts.h"
#endif
#ifdef ACTSMODEL_LW
	#ifdef ACTSMODEL_LWTYPE1
	#include "../acts/acts_lw/actslw.h"
	// #include "../acts/acts_lw3/actslw.h"
	// #include "../acts/acts_lw4/actslw.h"
	// #include "../acts/acts_lw5/actslw.h"
	#endif 
	#ifdef ACTSMODEL_LWTYPE2
	#include "../acts/acts_lw2/actslw.h"
	// #include "../acts/acts_lw3/actslw.h"
	#endif
	#ifdef ACTSMODEL_LWTYPE3
	#include "../acts/acts_lw3/actslw.h"
	#endif 
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
void swkernel::launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("swkernel::launchkernel:: print kvdram (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printmessages("swkernel::launchkernel:: print messages (before kernel launch)", (uint512_vec_dt *)(&kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS])); }}
	#endif
	// exit(EXIT_SUCCESS);
	
	#ifdef ACTSMODEL
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->allignandappendinvalids((keyvalue_t *)kvsourcedram[i][j], kvstats[i][j][BASEOFFSET_STATSDRAM + 0].value); }} // edge conditions
	#endif 
	
	#ifdef ACTSMODEL
	#ifdef LOCKE
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ workerthread_launchkernel_acts(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j], kvdestdram[i][j], kvstats[i][j]); }}
	#else 
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mykernelthread[i][j] = std::thread(&helperfunctions::workerthread_launchkernel, this, i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j], kvdestdram[i][j], kvstats[i][j]); }}
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mykernelthread[i][j].join(); }}
	#endif
	#endif 
	
	#if defined(ACTSMODEL_LW) && defined(ACTSMODEL_LWTYPE1)
	#ifdef LOCKE
		for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ workerthread_launchkernel_actslwtype1(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j]); }}
		exit(EXIT_SUCCESS); // REMOVEME.
	#else 
		// FIXME.
	#endif
	#endif 
	
	#if defined(ACTSMODEL_LW) && defined(ACTSMODEL_LWTYPE2)
	#ifdef LOCKE
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ for (int i = 0; i < NUMCPUTHREADS; i+=2){ workerthread_launchkernel_actslwtype2(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j], kvsourcedram[i+1][j]); }}
	#else 
		// FIXME.
	#endif
	#endif
	
	#if defined(ACTSMODEL_LW) && defined(ACTSMODEL_LWTYPE3)
	#ifdef LOCKE
		workerthread_launchkernel_actslwtype3(0, 
							kvsourcedram[0][0], 
							kvsourcedram[0][1],
							kvsourcedram[0][2],
							kvsourcedram[0][3],
							kvsourcedram[0][4],
							kvsourcedram[0][5],
							kvsourcedram[0][6],
							kvsourcedram[0][7],
							kvsourcedram[0][8],
							kvsourcedram[0][9],
							kvsourcedram[0][10],
							kvsourcedram[0][11],
							kvsourcedram[0][12],
							kvsourcedram[0][13],
							kvsourcedram[0][14],
							kvsourcedram[0][15]);
	#else 
		// FIXME.
	#endif
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print results after Kernel run", (keyvalue_t *)kvsourcedram[i][j], 16); }}
	for(unsigned int value=0; value<24; value++){ for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->countkeyvalueswithvalueequalto("helperfunctions::launchkernel", (keyvalue_t *)kvdestdram[i][j], KVDATA_RANGE_PERSSDPARTITION, value); }}}			
	#endif
	return;
}

// worker threads
#ifdef ACTSMODEL
void swkernel::workerthread_launchkernel_acts(unsigned int ithreadidx, uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats){
	
	kernelobjs[ithreadidx]->topkernel(kvsourcedram, kvdestdram, kvstats);
	
	#ifdef TESTKERNEL
	exit(EXIT_SUCCESS); // REMOVEME.
	#endif
	return;
}
#endif
#if defined(ACTSMODEL_LW) && defined(ACTSMODEL_LWTYPE1)
void swkernel::workerthread_launchkernel_actslwtype1(unsigned int ithreadidx, uint512_dt * kvsourcedram){
	/* #ifndef TESTKERNEL
	return; // REMOVEME.
	#endif  */
	
	kernelobjs[ithreadidx]->topkernel(kvsourcedram);
	
	// #ifdef TESTKERNEL
	// exit(EXIT_SUCCESS); // REMOVEME.
	// #endif
	return;
}
#endif
#if defined(ACTSMODEL_LW) && defined(ACTSMODEL_LWTYPE2)
void swkernel::workerthread_launchkernel_actslwtype2(unsigned int ithreadidx, uint512_dt * kvsourcedram1, uint512_dt * kvsourcedram2){
	#ifndef TESTKERNEL
	return; // REMOVEME.
	#endif 

	kernelobjs[ithreadidx]->topkernel(kvsourcedram1, kvsourcedram2);
	
	#ifdef TESTKERNEL
	exit(EXIT_SUCCESS); // REMOVEME.
	#endif
	return;
}
#endif 
#if defined(ACTSMODEL_LW) && defined(ACTSMODEL_LWTYPE3)
void swkernel::workerthread_launchkernel_actslwtype3(unsigned int ithreadidx, 
										uint512_dt * kvsourcedram0, 
										uint512_dt * kvsourcedram1,
										uint512_dt * kvsourcedram2,
										uint512_dt * kvsourcedram3,
										uint512_dt * kvsourcedram4,
										uint512_dt * kvsourcedram5,
										uint512_dt * kvsourcedram6,
										uint512_dt * kvsourcedram7,
										uint512_dt * kvsourcedram8,
										uint512_dt * kvsourcedram9,
										uint512_dt * kvsourcedram10,
										uint512_dt * kvsourcedram11,
										uint512_dt * kvsourcedram12,
										uint512_dt * kvsourcedram13,
										uint512_dt * kvsourcedram14,
										uint512_dt * kvsourcedram15){
	#ifndef TESTKERNEL
	return; // REMOVEME.
	#endif 

	kernelobjs[ithreadidx]->topkernel(kvsourcedram0, 
										kvsourcedram1,
										kvsourcedram2,
										kvsourcedram3,
										kvsourcedram4,
										kvsourcedram5,
										kvsourcedram6,
										kvsourcedram7,
										kvsourcedram8,
										kvsourcedram9,
										kvsourcedram10,
										kvsourcedram11,
										kvsourcedram12,
										kvsourcedram13,
										kvsourcedram14,
										kvsourcedram15);
	
	#ifdef TESTKERNEL
	exit(EXIT_SUCCESS); // REMOVEME.
	#endif
	return;
}
#endif 

void swkernel::finishOCL(){
	return;
}
#endif 












