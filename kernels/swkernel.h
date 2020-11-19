#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#include "../include/common.h"
#ifdef TESTKERNEL_ACTSMAX
#include "../acts/acts_lw/actsmax.h"
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
	
	void finishOCL();
	#endif 
private:
	utility * utilityobj;
	#ifdef SW
	#ifdef TESTKERNEL_ACTSMAX
	actsmax * kernelobjs[NUMCPUTHREADS * NUMSUBCPUTHREADS];
	#else 
	actslw * kernelobjs[NUMCPUTHREADS * NUMSUBCPUTHREADS];
	#endif
	#endif 
	std::thread mykernelthread[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







