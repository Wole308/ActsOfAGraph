#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <mutex>
#include <thread>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../kernels/kernel.h"
#include "../../include/common.h"
using namespace std;

class helperfunctions {
public:
	helperfunctions(graph * graphobj);
	helperfunctions();
	~helperfunctions();
	
	void replicateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	void workerthread_replicateverticesdata(int threadidx, keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);

	void cummulateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	void workerthread_cummulateverticesdata(int threadidx, keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);

	void applyvertices(unsigned int bank, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx);
	void workerthread_applyvertices(int ithreadidx, unsigned int bank, value_t * vertexdatabuffer, unsigned int * vertexisactivebuffer, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx);						

	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	void updatemessagesbeforelaunch(unsigned int globaliteration_idx, unsigned int graph_iterationidx, unsigned int graph_algorithmidx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS]);				
	void updatemessagesafterlaunch(unsigned int globaliteration_idx, keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
	void loadvertexpropertiesfromfile();
	void workerthread_loadvertexpropertiesfromfile(int ithreadidx, int fd, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t size);
	
	unsigned int getflag(unsigned int globaliteration_idx);
	
	#ifdef FPGA_IMPL 
	void loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void writeVstokernel(unsigned int flag);
	void readVsfromkernel(unsigned int flag);
	void finishOCL();
	#endif 
	
private:
	kernel * kernelobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	std::thread mythread[NUMCPUTHREADS];
	std::thread mykernelthread[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







