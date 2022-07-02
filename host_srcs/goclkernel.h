#ifndef GOCLKERNEL_H_
#define GOCLKERNEL_H_
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <mutex>
#include "algorithm.h"
#include "utility.h"
#include "../include/common.h"
#include "../include/config_params.h"
#ifdef FPGA_IMPL
#include "xcl2.hpp"
#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <CL/cl_ext_xilinx.h>
#endif 
#include "utility.h"
#include "algorithm.h"
#include "../include/common.h"
#include "../include/config_params.h"

// #define GOCLKERNEL_DEBUGMODE_HOSTPRINTS // 
#define GOCLKERNEL_DEBUGMODE_HOSTPRINTS3

class goclkernel {
public:
	goclkernel(universalparams_t _universalparams);
	~goclkernel();
	
	long double getaveragetimeelapsed(long double kerneltimelapse[MAXNUM_PEs]);

	#ifdef FPGA_IMPL
	long double runapp(std::string binaryFile[2], uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs], long double timeelapsed_totals[128][8],
			unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS], vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, universalparams_t _universalparams);					
	#endif
	
private:
	utility * utilityobj;
	universalparams_t myuniversalparams;
	
	size_t inputvdata_size_bytes;
	size_t inputdata_size_bytes;
};
#endif







