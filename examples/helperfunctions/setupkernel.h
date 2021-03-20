#ifndef SETUPKERNEL_H
#define SETUPKERNEL_H
#include <mutex>
#include <thread>
#include "../../kernels/swkernel.h"
#include "../../kernels/goclkernel.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

class setupkernel {
public:
	setupkernel(graph * graphobj, stats * _statsobj);
	setupkernel(stats * _statsobj);
	~setupkernel();

	long double runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	
	#ifdef GRAFBOOST_SETUP 
	void loadSRstructures();
	void startSRteration();
	unsigned int finishSRteration(unsigned int iteration, vector<value_t> &activevertices);
	#endif 
	
private:
	#ifdef GRAFBOOST_SETUP
	sr * srkernelobj;
	#endif
	utility * utilityobj;
	graph * graphobj;
	stats * statsobj;
	#ifdef GRAFBOOST_SETUP 
	SortReduce<uint64_t,uint32_t>* _sr;
	VertexValues<uint32_t,uint32_t>* vertex_values;
	#endif
	
	#ifdef FPGA_IMPL
	goclkernel * kernelobj;
	#else 
	swkernel * kernelobj;	
	#endif

	std::thread mythread[NUMUTILITYTHREADS];
	std::thread mykernelthread[NUMUTILITYTHREADS];
};
#endif







