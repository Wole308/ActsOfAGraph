#ifndef PAGERANK_H
#define PAGERANK_H
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../src/stats/stats.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../include/common.h"

class pagerank {
public:
	pagerank(unsigned int algorithmid, unsigned int datasetid);
	~pagerank();
	
	runsummary_t run();
	void WorkerThread(int threadidx, int threadidxoffset, unsigned int graph_iterationidx);
	
private:
	graph * graphobj;
	utility * utilityobj[NUMSUPERCPUTHREADS];
	edge_process * edgeprocessobj[NUMSUPERCPUTHREADS];
	helperfunctions * helperfunctionsobj[NUMSUPERCPUTHREADS];
	stats * statsobj[NUMSUPERCPUTHREADS];
	dataaccess * dataaccessobj[NUMSUPERCPUTHREADS];
	
	uint512_vec_dt * kvsourcedram[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	uint512_vec_dt * kvdestdram[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	keyvalue_t * kvstats[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








