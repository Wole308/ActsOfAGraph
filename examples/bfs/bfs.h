#ifndef BFS_H
#define BFS_H
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../src/stats/stats.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../acts/include/actscommon.h" //
#include "../../include/common.h"

class bfs {
public:
	bfs(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~bfs();
	
	runsummary_t run();
	void WorkerThread1(int threadidx, hostglobalparams_t globalparams);
	void WorkerThread2(int superthreadidx, int threadidxoffset, hostglobalparams_t globalparams);
	
	void partitionupdates(unsigned int threadidx, keyvalue_t * buffer, unsigned int vbegin, unsigned int datasize);
	void appendupdatestobuffer(vector<keyvalue_t> (&sourcebuffer)[NUMSSDPARTITIONS], vector<keyvalue_t> (&destinationbuffer)[NUMSSDPARTITIONS]);
	void retrieveupdates(unsigned int superthreadidx, unsigned int bank, unsigned int fdoffset[NUMCPUTHREADS], keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int datasize[NUMCPUTHREADS], unsigned int voffset);			
	void workerthread_retrieveupdates(int ithreadidx, unsigned int superthreadidx, unsigned int bank, unsigned int fdoffset, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], unsigned int datasize, unsigned int voffset);				

private:
	graph * graphobj;
	utility * utilityobj[NUMSUPERCPUTHREADS];
	algorithm * algorithmobj[NUMSUPERCPUTHREADS];
	edge_process * edgeprocessobj[NUMSUPERCPUTHREADS];
	helperfunctions * helperfunctionsobj[NUMSUPERCPUTHREADS];
	stats * statsobj[NUMSUPERCPUTHREADS];
	dataaccess * dataaccessobj[NUMSUPERCPUTHREADS];
	
	uint512_vec_dt * kvsourcedram[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	uint512_vec_dt * kvdestdram[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	keyvalue_t * kvstats[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	uint512_vec_dt * kvintermediatedram[NUMSUPERCPUTHREADS][NUMCPUTHREADS];
	
	vector<keyvalue_t> VUbuffer[NUMSSDPARTITIONS];
	vector<keyvalue_t> intermediateVUbuffer[NUMSUPERCPUTHREADS][NUMSSDPARTITIONS];
	std::thread mythread[NUMSUPERCPUTHREADS][NUMCPUTHREADS];
	
	hostglobalparams_t globalparams;
};
#endif








