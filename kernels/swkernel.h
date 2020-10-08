#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#include "../include/common.h"
#ifdef ACTSMODEL
#include "../acts/acts1/acts.h"
#endif
#ifdef ACTSMODEL_LW
	#ifdef ACTSMODEL_LWGROUP1
	// #include "../acts/acts_lw/actslw.h"
	#include "../acts/acts_lw1/actslw.h"
	#endif 
	#ifdef ACTSMODEL_LWGROUP2
	#include "../acts/acts_lw10/actslw.h"
	#endif
	#ifdef ACTSMODEL_LWGROUP3
	#include "../acts/acts_lw11/actslw.h"
	#endif 
#endif 
#include "../src/utility/utility.h"

class swkernel {
public:
	swkernel();
	~swkernel();
	
	#ifdef SW 
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);				
	
	void workerthread_launchkernel_acts(unsigned int ithreadidx, uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats);
	void workerthread_launchkernel_actslwtype1(unsigned int ithreadidx, uint512_dt * kvsourcedram);
	void workerthread_launchkernel_actslwtype2(unsigned int ithreadidx, uint512_dt * kvsourcedram1, uint512_dt * kvsourcedram2);
	void workerthread_launchkernel_actslwtype3(unsigned int ithreadidx, 
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
										uint512_dt * kvsourcedram15);
	
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







