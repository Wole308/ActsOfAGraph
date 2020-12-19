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
	
	void cummulateandcommitverticesdata(value_t * buffer[NUMSUBCPUTHREADS], value_t * tempvertexdatabuffer, vertex_t voffset);
	void workerthread_cummulateandcommitverticesdata(int threadidx, value_t * buffer[NUMSUBCPUTHREADS], value_t * tempvertexdatabuffer, vertex_t voffset, unsigned int offset, unsigned int size); 
	
	void applyvertices2(value_t * tempvertexdatabuffer, value_t * vertexdatabuffer, vector<value_t> &activeverticesbuffer, unsigned int GraphAlgo);
	void workerthread_applyvertices2(int ithreadidx, value_t * tempvertexdatabuffer, value_t * vertexdatabuffer, vector<value_t> &activeverticesbuffer, vertex_t offset, vertex_t size, unsigned int GraphAlgo);
	
	unsigned int getflag(unsigned int globaliteration_idx);
	
private:
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj[NUMUTILITYTHREADS];
	stats * statsobj;
	std::thread mythread[NUMUTILITYTHREADS];
	
	vector<value_t> tempactvvsbuffer[NUMUTILITYTHREADS];
};
#endif







