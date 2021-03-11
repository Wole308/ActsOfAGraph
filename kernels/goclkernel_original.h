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
	
	size_t inputvdata_size_bytes;
	size_t inputdata_size_bytes;
	stats * statsobj;
};
#endif







