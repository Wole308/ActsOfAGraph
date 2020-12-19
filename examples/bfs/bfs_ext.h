#ifndef BFS_EXT_H
#define BFS_EXT_H
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
#include "bfs_ext.h"

class bfs_ext {
public:
	bfs_ext(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~bfs_ext();
	void finish();
	
	runsummary_t run();
	
	void WorkerThread(vector<vertex_t> &currentactivevertices, container_t * container, unsigned int GraphIter);
	
private:
	graph * graphobj;
	parameters * parametersobj;
	utility * utilityobj;
	postprocess * postprocessobj;
	loadgraph * loadgraphobj;
	setupkernel * setupkernelobj;
	stats * statsobj;
	algorithm * algorithmobj;
	hostglobalparams_t globalparams;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	value_t * tempvertexdatabuffer;
	edge_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS];
	edge_t * vertexptrs[NUMSUBCPUTHREADS];
	value_t * verticesdata[NUMSUBCPUTHREADS];
	edge_type * edges[NUMSUBCPUTHREADS];
};
#endif








