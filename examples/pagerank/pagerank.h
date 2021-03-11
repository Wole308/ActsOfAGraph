#ifndef PAGERANK_H
#define PAGERANK_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../examples/helperfunctions/setupkernel.h"
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
	utility * utilityobj;
	loadgraph * loadgraphobj;
	setupkernel * setupkernelobj;
	stats * statsobj;

	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	value_t * tempvertexdatabuffer;
	edge2_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS];
	uint512_vec_dt * vdram;
	edge2_type * edges[NUMSUBCPUTHREADS];
	
	std::string binaryFile;
};
#endif








