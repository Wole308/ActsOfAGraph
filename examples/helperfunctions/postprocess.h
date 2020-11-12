#ifndef POSTPROCESS_H
#define POSTPROCESS_H
#include <mutex>
#include <thread>
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

class postprocess {
public:
	postprocess(graph * graphobj, stats * _statsobj);
	postprocess();
	~postprocess();
	
	void cummulateverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void workerthread_cummulateverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);

	void applyvertices(vector<value_t> &activeverticesbuffer, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset, unsigned int vsize, unsigned int GraphAlgo);
	void workerthread_applyvertices(int ithreadidx, vector<value_t> &activeverticesbuffer, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset, vertex_t offset, vertex_t size, unsigned int GraphAlgo);						

	unsigned int getflag(unsigned int globaliteration_idx);
	
private:
	kernel * kernelobj;
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	std::thread mythread[NUMUTILITYTHREADS];
	std::thread mykernelthread[NUMUTILITYTHREADS];
};
#endif







