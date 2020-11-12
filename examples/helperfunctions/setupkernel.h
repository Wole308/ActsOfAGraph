#ifndef SETUPKERNEL_H
#define SETUPKERNEL_H
#include <mutex>
#include <thread>
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "procedges.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

class setupkernel {
public:
	setupkernel(graph * graphobj, stats * _statsobj);
	setupkernel();
	~setupkernel();

	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t * vertexptrs, value_t * vertexdatabuffer, edge_type * edgedatabuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);					
	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], value_t * vertexdatabuffer, edge_type * edgedatabuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	unsigned int getflag(unsigned int globaliteration_idx);
	#ifdef FPGA_IMPL 
	void loadOCLstructures(std::string binaryFile, uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size);
	void readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size);				
	void finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP 
	void loadSRstructures();
	void finishSR();
	#endif 
	
private:
	kernel * kernelobj;
	#ifdef GRAFBOOST_SETUP
	sr * srkernelobj;
	#endif
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	#ifdef GRAFBOOST_SETUP 
	SortReduce<uint64_t,uint32_t>* _sr;
	VertexValues<uint32_t,uint32_t>* vertex_values;
	#endif 
	procedges * procedgesobj;

	std::thread mythread[NUMUTILITYTHREADS];
	std::thread mykernelthread[NUMUTILITYTHREADS];
};
#endif







