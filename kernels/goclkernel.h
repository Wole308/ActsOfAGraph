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
	
	#define TOTALNUMKERNELS 16
	
	#define NUMHBMSPERKERNEL 1
	
	#define TOTALNUMACTCUSTORUN (NUMCPUTHREADS * NUMSUBCPUTHREADS)
	
	#define NUMACTSCUSPERKERNEL NUMINSTANCES
	
	#define TOTALAVAILABLEACTSCUS (TOTALNUMKERNELS * NUMACTSCUSPERKERNEL)
	
	#define NUMACTIVEKERNELS ((TOTALNUMACTCUSTORUN + (NUMACTSCUSPERKERNEL - 1)) / NUMACTSCUSPERKERNEL)

	#define NUMACTIVEINSTANCES 1
	// #if TOTALNUMACTCUSTORUN<NUMINSTANCES
	// #define NUMACTIVEINSTANCES 1
	// #else 
	// #define NUMACTIVEINSTANCES NUMINSTANCES
	// #endif 
	
	#ifdef FPGA_IMPL 
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	void writeVstokernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size);
	void readVsfromkernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size);
	
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
	cl_mem_ext_ptr_t inoutBufExt[TOTALAVAILABLEACTSCUS];

    cl::Buffer buffer_kvsourcedram[TOTALNUMACTCUSTORUN];
	
	cl::Kernel krnls[TOTALNUMKERNELS];
	
	cl::CommandQueue q;
	cl_int err;
	#endif 
};
#endif







