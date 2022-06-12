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
	void verifyresults(uint512_vec_dt * vdram, unsigned int id);
	
	long double runapp(std::string binaryFile[2], uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters,
		unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS], vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer);						
	
	void run3(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	void run12(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	void run16(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	void run18(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	void run20(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	void run22(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	void run24(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	void run25(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	void run32(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]);
	#endif
	
	void verifyresults_hw(uint512_vec_dt * kvdram, unsigned int _BASEOFFSETKVS_VERTICESDATA);
private:
	#ifndef FPGA_IMPL
	utility * utilityobj;
	
	acts_all * kernelobjs_process[MAXNUMSUBCPUTHREADS];
	mydebug * mydebugobj;
	universalparams_t universalparams;
	#endif 
};
#endif







