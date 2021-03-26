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
#include "../src/utility/utility.h"
#include "../include/common.h"

#define GOCLKERNEL_DEBUGMODE_HOSTPRINTS // 

class goclkernel {
public:
	goclkernel(stats * _statsobj);
	~goclkernel();
	
	long double getaveragetimeelapsed(long double kerneltimelapse[NUMSUBCPUTHREADS]);

	#ifdef FPGA_IMPL
	long double runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);	
	long double runapp1(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);	
	long double runapp2(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);	
	#endif
	
	unsigned int ExitCheck(uint512_vec_dt * kvsourcedram, unsigned int GraphIter);
	
private:
	utility * utilityobj;
	
	size_t inputvdata_size_bytes;
	size_t inputdata_size_bytes;
	stats * statsobj;
	
	actssync * kernelobjs_synchronize;
	uint512_vec_dt * tempkvsourcedram;
	uint512_vec_dt * tempkvsourcedrams[NUMSUBCPUTHREADS];
	uint512_vec_dt * tempvdram;
};
#endif







