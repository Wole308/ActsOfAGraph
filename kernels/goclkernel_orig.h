#ifndef GOCLKERNEL_H_
#define GOCLKERNEL_H_
#ifdef FPGA_IMPL
#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <CL/cl_ext_xilinx.h>
#endif 
#include "../acts/acts/actssync.h"
#include "../src/stats/stats.h"
#include "../src/graphs/graph.h"
#include "../src/utility/utility.h"
#include "../src/algorithm/algorithm.h"
#include "../include/common.h"
#include "../include/config_params.h"

// #define GOCLKERNEL_DEBUGMODE_HOSTPRINTS // 
#define GOCLKERNEL_DEBUGMODE_HOSTPRINTS3

class goclkernel {
public:
	goclkernel(graph * _graphobj, algorithm * _algorithmobj, stats * _statsobj);
	~goclkernel();
	
	long double getaveragetimeelapsed(long double kerneltimelapse[NUMSUBCPUTHREADS]);

	#ifdef FPGA_IMPL
	long double runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters);					
	#endif
	
	void verifyresults_hw(uint512_vec_dt * kvbuffer, unsigned int baseoffsetkvs_verticesdata);
	unsigned int ExitCheck(uint512_vec_dt * kvsourcedram, unsigned int GraphIter);
	
private:
	utility * utilityobj;
	stats * statsobj;
	graph * graphobj;
	algorithm * algorithmobj;
	
	size_t inputvdata_size_bytes;
	size_t inputdata_size_bytes;
	
	#ifndef ACTS_1by1
	// actssync * kernelobjs_synchronize;
	#endif 
	
	uint512_vec_dt * tempkvsourcedram_proc;
	uint512_vec_dt * tempkvsourcedrams_proc[NUMSUBCPUTHREADS]; // REMOVEME
	uint512_vec_dt * tempkvsourcedrams_sync[NUMSYNCTHREADS];
	uint512_vec_dt * tempvdram_sync;
};
#endif







