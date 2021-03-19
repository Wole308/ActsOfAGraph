#ifndef GOCLKERNEL_H_
#define GOCLKERNEL_H_
#ifdef FPGA_IMPL
#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <CL/cl_ext_xilinx.h>
#endif 
#include "../src/stats/stats.h"
#include "../src/utility/utility.h"
#include "../include/common.h"

// #define GOCLKERNEL_DEBUGMODE_HOSTPRINTS // 

class goclkernel {
public:
	goclkernel(stats * _statsobj);
	~goclkernel();
	
	long double getaveragetimeelapsed(long double kerneltimelapse[NUMSUBCPUTHREADS]);

	#ifdef FPGA_IMPL
	void runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);	

	void runapp_process(std::string binaryFile, auto devices, cl::Context context, cl::CommandQueue q, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	void runapp_synchronize(std::string binaryFile, auto devices, cl::Context context, cl::CommandQueue q, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	#endif 
private:
	utility * utilityobj;
	
	size_t inputvdata_size_bytes;
	size_t inputdata_size_bytes;
	stats * statsobj;
};
#endif







