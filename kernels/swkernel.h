#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#include "../include/common.h"
#ifdef TESTKERNEL_FULLBANDWIDTH
#include "../acts/acts_lw/actslw_maxbutil.h"
#else 
#include "../acts/acts_lw/actslw.h"
#endif 
#include "../src/utility/utility.h"

class swkernel {
public:
	swkernel();
	~swkernel();
	
	#ifdef SW 
	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	void workerthread_acts(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram, uint512_vec_dt * kvdestdram, keyvalue_t * kvstats);
	void workerthread_actslw(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram);
	void workerthread_sortreduce(unsigned int ithreadidx, uint512_vec_dt * kvsourcedram);
	
	void finishOCL();
	#endif 
private:
	utility * utilityobj;
	#ifdef SW
	#ifdef TESTKERNEL_FULLBANDWIDTH
	actslw_maxbutil * kernelobjs_maxbutil[NUMCPUTHREADS * NUMSUBCPUTHREADS];
	#else 
	actslw * kernelobjs[NUMCPUTHREADS * NUMSUBCPUTHREADS];
	#endif
	#endif 
	std::thread mykernelthread[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







