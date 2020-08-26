#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <mutex>
#include <thread>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../acts/acts.h"
#include "../../include/common.h"
using namespace std;

class helperfunctions {
public:
	helperfunctions(graph * graphobj);
	~helperfunctions();
	
	void replicateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	void workerthread_replicateverticesdata(int threadidx, keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);

	void cummulateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	void workerthread_cummulateverticesdata(int threadidx, keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);

	void applyvertices(unsigned int bank, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx);
	void workerthread_applyvertices(int ithreadidx, unsigned int bank, value_t * vertexdatabuffer, unsigned int * vertexisactivebuffer, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx);						

	void launchkernel(int threadidx, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void workerthread_launchkernel(unsigned int i, uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats);

	void updatemessagesbeforelaunch(unsigned int graph_iterationidx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void updatemessagesafterlaunch(int threadidx, unsigned int graph_iterationidx, keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
	void loadvertexpropertiesfromfile();
	void workerthread_loadvertexpropertiesfromfile(int ithreadidx, int fd, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t size);
	
	unsigned int getflag(unsigned int globaliteration_idx);
	
private:
	acts * kernelobjs[NUMCPUTHREADS];
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	std::thread mythread[NUMCPUTHREADS];
	std::thread mykernelthread[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







