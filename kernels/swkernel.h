#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#include "../include/common.h"
#include "../src/stats/stats.h"
#include "../acts/acts/acts.h"
#include "../acts/acts/acts_process.h"
#include "../acts/acts/acts_synchronize.h"
#include "../src/utility/utility.h"

class swkernel {
public:
	swkernel(stats * _statsobj);
	~swkernel();
	
	#ifdef SW 
	void runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	#endif 
private:
	utility * utilityobj;
	stats * statsobj;
	#ifdef SW
	acts * kernelobjs[NUMSUBCPUTHREADS];
	acts_process * kernelobjs_process[NUMSUBCPUTHREADS];
	acts_synchronize * kernelobjs_synchronize;
	#endif 
	std::thread mykernelthread[NUMSUBCPUTHREADS];
};
#endif







