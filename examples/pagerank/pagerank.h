#ifndef PAGERANK_H
#define PAGERANK_H
#include <mutex>
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../src/stats/stats.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../acts/include/actscommon.h" //
#include "../../include/common.h"

class pagerank {
public:
	pagerank(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~pagerank();
	
	runsummary_t run();
	void WorkerThread(int superthreadidx, int threadidxoffset, hostglobalparams_t globalparams);
	
private:
	graph * graphobj;
	parameters * parametersobj[NUMSUPERCPUTHREADS];
	utility * utilityobj[NUMSUPERCPUTHREADS];
	edge_process * edgeprocessobj[NUMSUPERCPUTHREADS];
	helperfunctions * helperfunctionsobj[NUMSUPERCPUTHREADS];
	stats * statsobj[NUMSUPERCPUTHREADS];
	dataaccess * dataaccessobj[NUMSUPERCPUTHREADS];
	
	uint512_vec_dt * kvsourcedram[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	uint512_vec_dt * kvdestdram[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	keyvalue_t * kvstats[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	
	hostglobalparams_t globalparams;
};
#endif








