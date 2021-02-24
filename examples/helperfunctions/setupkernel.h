#ifndef SETUPKERNEL_H
#define SETUPKERNEL_H
#include <mutex>
#include <thread>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

class setupkernel {
public:
	setupkernel(graph * graphobj, stats * _statsobj);
	setupkernel(stats * _statsobj);
	~setupkernel();

	void launchkernel(uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int flag);
	void launchmykernel(uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int flag);
	
	unsigned int getflag(unsigned int globaliteration_idx);
	#ifdef FPGA_IMPL 
	void loadOCLstructures(std::string binaryFile, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	void finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP 
	void loadSRstructures();
	void startSRteration();
	unsigned int finishSRteration(unsigned int iteration, vector<value_t> &activevertices);
	#endif 
	
private:
	kernel * kernelobj;
	#ifdef GRAFBOOST_SETUP
	sr * srkernelobj;
	#endif
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	#ifdef GRAFBOOST_SETUP 
	SortReduce<uint64_t,uint32_t>* _sr;
	VertexValues<uint32_t,uint32_t>* vertex_values;
	#endif

	std::thread mythread[NUMUTILITYTHREADS];
	std::thread mykernelthread[NUMUTILITYTHREADS];
};
#endif







