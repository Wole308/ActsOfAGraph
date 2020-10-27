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
#include "../acts/acts_lw/actslw.h"
#endif 
#include "../src/utility/utility.h"

class swkernel {
public:
	swkernel();
	~swkernel();
	
	#ifdef SW 
	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_vec_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);				
	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	void workerthread_acts(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram, uint512_vec_dt * kvdestdram, keyvalue_t * kvstats);
	void workerthread_actslw(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram);
	void workerthread_sortreduce(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram);
	
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







