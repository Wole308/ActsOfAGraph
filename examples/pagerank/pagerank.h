#ifndef PAGERANK_H
#define PAGERANK_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../examples/helperfunctions/setupkernel.h"
#include "../../examples/helperfunctions/postprocess.h"
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
	
	void WorkerThread(unsigned int superthreadidx, unsigned int col, vector<vertex_t> &nextactivevertices, container_t * container, unsigned int GraphIter);
					
private:
	graph * graphobj;
	parameters * parametersobj[NUMSUPERCPUTHREADS];
	utility * utilityobj[NUMSUPERCPUTHREADS];
	postprocess * postprocessobj[NUMSUPERCPUTHREADS];
	loadgraph * loadgraphobj[NUMSUPERCPUTHREADS];
	setupkernel * setupkernelobj[NUMSUPERCPUTHREADS];
	stats * statsobj;
	hostglobalparams_t globalparams;

	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	value_t * tempvertexdatabuffer;
	edge_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	edge_type * edges[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








