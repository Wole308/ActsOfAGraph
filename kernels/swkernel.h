#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#ifdef ACTSMODEL
#include "../acts/acts/acts.h"
#endif
#ifdef ACTSMODEL_LW
	#ifdef ACTSMODEL_LWTYPE1
	#include "../acts/acts_lw/actslw.h"
	// #include "../acts/acts_lw3/actslw.h"
	#endif 
	#ifdef ACTSMODEL_LWTYPE2
	#include "../acts/acts_lw2/actslw.h"
	// #include "../acts/acts_lw3/actslw.h"
	#endif
#endif 
#include "../src/utility/utility.h"
#include "../include/common.h"

class swkernel {
public:
	swkernel();
	~swkernel();
	
	#ifdef SW 
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);				
	
	void workerthread_launchkernel_acts(unsigned int ithreadidx, uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats);
	void workerthread_launchkernel_actslwtype1(unsigned int ithreadidx, uint512_dt * kvsourcedram);
	void workerthread_launchkernel_actslwtype2(unsigned int ithreadidx, uint512_dt * kvsourcedram1, uint512_dt * kvsourcedram2);
	
	void finishOCL();
	#endif 
private:
	utility * utilityobj;
	#ifdef SW 
	#ifdef ACTSMODEL
	acts * kernelobjs[NUMCPUTHREADS * NUMSUBCPUTHREADS];
	#endif 
	#ifdef ACTSMODEL_LW
	actslw * kernelobjs[NUMCPUTHREADS * NUMSUBCPUTHREADS];
	#endif 
	#endif 
	std::thread mykernelthread[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







