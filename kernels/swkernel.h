#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../acts/acts.h"
#include "../src/utility/utility.h"
#include "../include/common.h"

class swkernel {
public:
	swkernel();
	~swkernel();
	
	#ifdef SW 
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	void workerthread_launchkernel(unsigned int ithreadidx, uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats);
	void finishOCL();
	#endif 
private:
	utility * utilityobj;
	#ifdef SW 
	acts * kernelobjs[NUMCPUTHREADS * NUMSUBCPUTHREADS];
	#endif 
	std::thread mykernelthread[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







