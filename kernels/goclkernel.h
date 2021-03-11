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

class goclkernel {
public:
	goclkernel(stats * _statsobj);
	~goclkernel();

	#ifdef NACTS_IN_NCOMPUTEUNITS
		#define TOTALNUMKERNELS NUMCOMPUTEUNITS
	#else 
		#define TOTALNUMKERNELS 1
	#endif

	// #define TOTALNUMBUFFERS 1
	#define TOTALNUMBUFFERS NUMCOMPUTEUNITS //
	
	#ifdef FPGA_IMPL
	void runapp(std::string _binaryFile, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);			
	#endif 
private:
	utility * utilityobj;
	// std::string binaryFile;
	
	size_t inputvdata_size_bytes;
	size_t inputdata_size_bytes;
	stats * statsobj;
	
	#ifdef FPGA_IMPL 
	cl::CommandQueue q;
	cl_int err;
	vector<cl::Event> read_events;
	vector<cl::Event> kernel_events;
	vector<cl::Event> write_event;
	cl_mem_ext_ptr_t inoutBufExt[TOTALNUMBUFFERS];
    cl::Buffer buffer_kvsourcedram[TOTALNUMBUFFERS];
	cl::Kernel krnls[TOTALNUMKERNELS];
	#endif 
};
#endif







