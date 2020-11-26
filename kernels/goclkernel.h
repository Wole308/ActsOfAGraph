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

	// #define TOTALNUMKERNELS 1
	#define TOTALNUMKERNELS NUMSUBCPUTHREADS //
	
	#define NUMHBMSPERKERNEL 1
	
	// #define TOTALNUMACTCUSTORUN 1
	#define TOTALNUMACTCUSTORUN NUMSUBCPUTHREADS //
	
	#define NUMACTSCUSPERKERNEL NUMINSTANCES
	
	#define TOTALAVAILABLEACTSCUS (TOTALNUMKERNELS * NUMACTSCUSPERKERNEL)
	
	#define NUMACTIVEKERNELS ((TOTALNUMACTCUSTORUN + (NUMACTSCUSPERKERNEL - 1)) / NUMACTSCUSPERKERNEL)

	#define NUMACTIVEINSTANCES 1
		
	/** #ifdef TESTKERNEL_ACTSMAX
		#define TOTALNUMKERNELS NUMSUBWORKERS
		
		#define NUMHBMSPERKERNEL 1
		
		#define TOTALNUMACTCUSTORUN NUMSUBWORKERS
		
		#define NUMACTSCUSPERKERNEL NUMINSTANCES
		
		#define TOTALAVAILABLEACTSCUS (TOTALNUMKERNELS * NUMACTSCUSPERKERNEL)
		
		// #define NUMACTIVEKERNELS 1
		#define NUMACTIVEKERNELS ((TOTALNUMACTCUSTORUN + (NUMACTSCUSPERKERNEL - 1)) / NUMACTSCUSPERKERNEL)
		
		#define NUMACTIVEINSTANCES 1
	#else
		// #define TOTALNUMKERNELS 1
		#define TOTALNUMKERNELS NUMSUBCPUTHREADS //
		
		#define NUMHBMSPERKERNEL 1
		
		// #define TOTALNUMACTCUSTORUN 1
		#define TOTALNUMACTCUSTORUN NUMSUBCPUTHREADS //
		
		#define NUMACTSCUSPERKERNEL NUMINSTANCES
		
		#define TOTALAVAILABLEACTSCUS (TOTALNUMKERNELS * NUMACTSCUSPERKERNEL)
		
		#define NUMACTIVEKERNELS ((TOTALNUMACTCUSTORUN + (NUMACTSCUSPERKERNEL - 1)) / NUMACTSCUSPERKERNEL)

		#define NUMACTIVEINSTANCES 1
	#endif */
	
	#ifdef FPGA_IMPL 
	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	void writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size);
	void writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS],  unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
	void readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size);
	void readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
	void loadOCLstructures(std::string binaryFile, uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);			
	void finishOCL();
	#endif 
private:
	utility * utilityobj;
	std::string binaryFile;
	
	size_t inputdata_size_bytes;
	stats * statsobj;
	
	#ifdef FPGA_IMPL 
	cl_mem_ext_ptr_t inoutBufExt[TOTALAVAILABLEACTSCUS];

    cl::Buffer buffer_kvsourcedram[TOTALNUMACTCUSTORUN];
	
	cl::Kernel krnls[TOTALNUMKERNELS];
	
	cl::CommandQueue q;
	cl_int err;
	
	vector<cl::Event> read_events;
	vector<cl::Event> kernel_events;
	vector<cl::Event> write_event;
	#endif 
};
#endif







