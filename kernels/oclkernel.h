#ifndef OCLKERNEL_H
#define OCLKERNEL_H
#include "../../src/utility/utility.h"
#include "../../include/common.h"

class oclkernel {
public:
	oclkernel();
	~oclkernel();
	
	#ifdef FPGA_IMPL 
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);

	void loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);			
	void writeVstokernel(unsigned int flag);
	void readVsfromkernel(unsigned int flag);
	void finishOCL();
	#endif 

private:
	utility * utilityobj;
	
	size_t kvsource_size_bytes;
	size_t kvdest_size_bytes; 
	size_t kvstats_size_bytes;
	
	#ifdef FPGA_IMPL 
	cl_int err;	
	size_t global; 
	size_t local;
	
	cl_event kernel_events[2];
	cl_event read_event[NUMCPUTHREADS + NUMSUBCPUTHREADS][2];
	
	xcl_world world;
	cl_program program;
	cl_kernel kernel;
	
	cl_mem buffer_kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	cl_mem buffer_kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	cl_mem buffer_kvstatsdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	#endif 
};
#endif







