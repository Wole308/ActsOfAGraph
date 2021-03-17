#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#include "../include/common.h"
#include "../src/stats/stats.h"
#include "../acts/acts/acts.h"
#include "../acts/acts/actsproc.h"
#include "../acts/acts/actssync.h"
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
	actsproc * kernelobjs_process[NUMSUBCPUTHREADS];
	actssync * kernelobjs_synchronize;
	#endif 
	std::thread mykernelthread[NUMSUBCPUTHREADS];
};
#endif







