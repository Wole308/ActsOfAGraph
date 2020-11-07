#ifndef PAGERANK_H
#define PAGERANK_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "pagerank.h"

class pagerank {
public:
	pagerank(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~pagerank();
	void finish();
	
	runsummary_t run();				
private:
	graph * graphobj;
	parameters * parametersobj;
	utility * utilityobj;
	helperfunctions2 * helperfunctionsobj;
	loadgraph * loadgraphobj;
	stats * statsobj;
	hostglobalparams_t globalparams;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	edge_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	container_t * container;
};
#endif








