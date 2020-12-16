#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#include "../include/common.h"
#include "../src/stats/stats.h"
#include "../acts/acts/acts.h"
#include "../src/utility/utility.h"

class swkernel {
public:
	swkernel(stats * _statsobj);
	~swkernel();
	
	#ifdef SW 
	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	void finishOCL();
	#endif 
private:
	utility * utilityobj;
	stats * statsobj;
	#ifdef SW
	acts * kernelobjs[NUMCPUTHREADS * NUMSUBCPUTHREADS];
	#endif 
	std::thread mykernelthread[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







