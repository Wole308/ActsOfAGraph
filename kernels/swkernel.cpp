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
void swkernel::launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print results before Kernel run", (keyvalue_t *)kvsourcedram[i][j], 16); }}
	#endif
	
	// for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->allignandappendinvalids((keyvalue_t *)kvsourcedram[i][j], kvstats[i][j][BASEOFFSET_STATSDRAM + 0].value); }} // edge conditions
	#ifdef LOCKE
	// for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ workerthread_launchkernel(i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j], kvdestdram[i][j], kvstats[i][j]); }}
	
	
	// kernelobjs[0]->topkernel(kvsourcedram[0][0], kvsourcedram[0][1], kvsourcedram[0][2], kvsourcedram[0][3]); // FIXME. AUTOMATEME.
	kernelobjs[0]->topkernel(kvsourcedram[0][0]); // FIXME. AUTOMATEME.
	
	
	#else 
	for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mykernelthread[i][j] = std::thread(&helperfunctions::workerthread_launchkernel, this, i*NUMSUBCPUTHREADS + j, kvsourcedram[i][j], kvdestdram[i][j], kvstats[i][j]); }}
	for (int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mykernelthread[i][j].join(); }}
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print results after Kernel run", (keyvalue_t *)kvsourcedram[i][j], 16); }}
	for(unsigned int value=0; value<24; value++){ for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->countkeyvalueswithvalueequalto("helperfunctions::launchkernel", (keyvalue_t *)kvdestdram[i][j], KVDATA_RANGE_PERSSDPARTITION, value); }}}			
	#endif
	return;
}
void swkernel::workerthread_launchkernel(unsigned int ithreadidx, uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats){
	#ifdef SW 
	#ifdef ACTSMODEL
	kernelobjs[ithreadidx]->topkernel(kvsourcedram, kvdestdram, kvstats);
	#endif 
	#ifdef ACTSMODEL_LW
	#if NUMINSTANCES == 1
	kernelobjs[ithreadidx]->topkernel(kvsourcedram);
	#elif NUMINSTANCES == 2
	kernelobjs[ithreadidx]->topkernel(kvsourcedram, kvsourcedram);
	#elif NUMINSTANCES == 3
	kernelobjs[ithreadidx]->topkernel(kvsourcedram, kvsourcedram, kvsourcedram);
	#elif NUMINSTANCES == 4
	kernelobjs[ithreadidx]->topkernel(kvsourcedram, kvsourcedram, kvsourcedram, kvsourcedram);
	#else 
	kernelobjs[ithreadidx]->topkernel(kvsourcedram, kvsourcedram, kvsourcedram, kvsourcedram);
	#endif
	#endif 
	#endif
	exit(EXIT_SUCCESS);
	return;
}
void swkernel::finishOCL(){
	return;
}
#endif 












