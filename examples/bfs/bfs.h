#ifndef BFS_H
#define BFS_H
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
#include "bfs.h"

class bfs {
public:
	bfs(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~bfs();
	void finish();
	
	runsummary_t run();
	
	void WorkerThread(vector<vertex_t> &currentactivevertices, container_t * container, unsigned int GraphIter);
	
private:
	graph * graphobj;
	parameters * parametersobj[NUMSUPERCPUTHREADS];
	utility * utilityobj[NUMSUPERCPUTHREADS];
	postprocess * postprocessobj[NUMSUPERCPUTHREADS];
	loadgraph * loadgraphobj[NUMSUPERCPUTHREADS];
	setupkernel * setupkernelobj[NUMSUPERCPUTHREADS];
	stats * statsobj;
	algorithm * algorithmobj;
	hostglobalparams_t globalparams;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	value_t * tempvertexdatabuffer;
	edge2_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	value_t * verticesdata[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	edge2_type * edges[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








