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

class goclkernel {
public:
	goclkernel();
	~goclkernel();
	
	#define NUMACTSINSTANCESTORUN 1//(NUMCPUTHREADS * NUMSUBCPUTHREADS) // 4
	// #define NUMKERNELS (NUMACTSINSTANCESTORUN / NUMINSTANCES) //1
	#define NUMKERNELS 1//(NUMACTSINSTANCESTORUN / 4) //1
	
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
	
	#ifdef FPGA_IMPL 
	cl_mem_ext_ptr_t inoutBufExt[NUMACTSINSTANCESTORUN];

    cl::Buffer buffer_kvsourcedram[NUMACTSINSTANCESTORUN];
	
	cl::Kernel krnls[NUMACTSINSTANCESTORUN];
	
	cl::CommandQueue q;
	cl_int err;
	#endif 
};
#endif







