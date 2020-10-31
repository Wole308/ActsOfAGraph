#ifndef HELPERFUNCTIONS2_H
#define HELPERFUNCTIONS2_H
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
using namespace std;

class helperfunctions2 {
public:
	helperfunctions2(graph * graphobj, stats * _statsobj);
	helperfunctions2();
	~helperfunctions2();
	
	void createmessages(
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int destvoffset,
			unsigned int beginvid,
			unsigned int beginkey,
			unsigned int beginvalue,
			unsigned int treedepth,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			unsigned int batch_range,
			unsigned int batch_range_pow,
			unsigned int applyvertexbuffersz,
			unsigned int numlastlevelpartitions);

	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	void cummulateverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void workerthread_cummulateverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);

	void applyvertices(unsigned int fdoffset, vector<keyvalue_t> & activeverticesbuffer, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset);
	void workerthread_applyvertices(int ithreadidx, unsigned int fdoffset, vector<keyvalue_t> & activeverticesbuffer, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t bufferoffset, vertex_t datasize, unsigned int voffset);
	
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
	
	// kernel * kernelobj;
	// parameters * parametersobj;
	// utility * utilityobj;
	// graph * graphobj;
	// algorithm * algorithmobj;
	// stats * statsobj;
	
	std::thread mythread[NUMUTILITYTHREADS];
	std::thread mykernelthread[NUMUTILITYTHREADS];
};
#endif







