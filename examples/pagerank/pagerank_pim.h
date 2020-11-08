#ifndef PAGERANK_PIM_H
#define PAGERANK_PIM_H
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
#include "pagerank_pim.h"

class pagerank_pim {
public:
	pagerank_pim(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~pagerank_pim();
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
};
#endif








