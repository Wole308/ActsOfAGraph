#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "../include/config_params.h"
#include "../include/common.h"
#include "utility.h"
#include "algorithm.h"
#include "../acts/acts_all.h"
#include "../acts/mydebug.h"

class swkernel {
public:
	swkernel(universalparams_t _universalparams);
	~swkernel();
	
	#ifndef FPGA_IMPL
	long double runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs], long double timeelapsed_totals[128][8],
		unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS], vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, universalparams_t _universalparams);						
	
	void run3(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	void run12(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	void run16(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	void run18(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	void run20(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	void run22(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	void run24(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	void run25(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	void run32(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]);
	#endif
	
private:
	#ifndef FPGA_IMPL
	utility * utilityobj;
	
	acts_all * kernelobjs_process[MAXNUM_PEs];
	mydebug * mydebugobj;
	universalparams_t myuniversalparams;
	#endif 
};
#endif







