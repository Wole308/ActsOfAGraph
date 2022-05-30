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
	
	long double getaveragetimeelapsed(long double kerneltimelapse[MAXNUMSUBCPUTHREADS]);

	#ifdef FPGA_IMPL
	void print_active_vpartitions(unsigned int GraphIter, uint512_vec_dt * vdram);
	long double runapp(std::string binaryFile[2], uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters,
			unsigned int * vpmaskbuffer[MAXNUMGRAPHITERATIONS], unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS], vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer);					
	#endif
	
	void verifyresults_hw(uint512_vec_dt * kvbuffer, unsigned int baseoffsetkvs_verticesdata);
	unsigned int ExitCheck(uint512_vec_dt * kvsourcedram, unsigned int GraphIter);
	
private:
	utility * utilityobj;
	universalparams_t universalparams;
	
	size_t inputvdata_size_bytes;
	size_t inputdata_size_bytes;
};
#endif







