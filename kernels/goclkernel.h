#ifndef GOCLKERNEL_H_
#define GOCLKERNEL_H_
#ifdef FPGA_IMPL
#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <CL/cl_ext_xilinx.h>
#endif 
#include "../../src/utility/utility.h"
#include "../../include/common.h"
// #include "xcl2.hpp"

class goclkernel {
public:
	goclkernel();
	~goclkernel();
	
	#ifdef FPGA_IMPL 
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	void writeVstokernel(unsigned int flag);
	void readVsfromkernel(unsigned int flag);
	
	void loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);			
	void finishOCL();
	#endif 
private:
	utility * utilityobj;
	std::string binaryFile;
	
	size_t kvsource_size_bytes;
	size_t kvdest_size_bytes; 
	size_t kvstats_size_bytes;
	
	cl_mem_ext_ptr_t inoutBufExt1[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
    cl_mem_ext_ptr_t inoutBufExt2[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
    cl_mem_ext_ptr_t inoutBufExt3[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];

    cl::Buffer buffer_kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
    cl::Buffer buffer_kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
    cl::Buffer buffer_kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	
	cl::Kernel krnls[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	
	cl::CommandQueue q;
	cl_int err;
};
#endif







