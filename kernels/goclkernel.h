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

	#define TOTALNUMKERNELS 1
	// #define TOTALNUMKERNELS NUMCOMPUTEUNITS //

	// #define TOTALNUMBUFFERS 1
	#define TOTALNUMBUFFERS (1 + NUMCOMPUTEUNITS) //

	#define NUMACTIVEKERNELS TOTALNUMKERNELS 

	#ifdef FPGA_IMPL 
	void launchkernel(uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int flag);
	
	void writetokernel(unsigned int flag, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int hostbeginoffset[NUMSUBCPUTHREADS],  unsigned int beginoffset[NUMSUBCPUTHREADS], unsigned int size[NUMSUBCPUTHREADS]);
	
	void readfromkernel(unsigned int flag, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int hostbeginoffset[NUMSUBCPUTHREADS], unsigned int beginoffset[NUMSUBCPUTHREADS], unsigned int size[NUMSUBCPUTHREADS]);			
	
	void loadOCLstructures(std::string binaryFile, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);			
	void finishOCL();
	#endif 
private:
	utility * utilityobj;
	std::string binaryFile;
	
	size_t inputdata_size_bytes;
	stats * statsobj;
	
	#ifdef FPGA_IMPL 
	cl_mem_ext_ptr_t inoutBufExt[TOTALNUMBUFFERS];

    cl::Buffer buffer_kvsourcedram[TOTALNUMBUFFERS];
	
	cl::Kernel krnls[TOTALNUMKERNELS];
	
	cl::CommandQueue q;
	cl_int err;
	
	vector<cl::Event> read_events;
	vector<cl::Event> kernel_events;
	vector<cl::Event> write_event;
	#endif 
};
#endif







