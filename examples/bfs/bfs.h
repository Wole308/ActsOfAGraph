#ifndef BFS_H
#define BFS_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../examples/helperfunctions/compactgraph.h"
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
	
	void verify(vector<vertex_t> &activevertices);
	void verifykvbuffer(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], unsigned int CLOP);

private:
	graph * graphobj;
	parameters * parametersobj;
	utility * utilityobj;
	postprocess * postprocessobj;
	loadgraph * loadgraphobj;
	compactgraph * compactgraphobj;
	setupkernel * setupkernelobj;
	stats * statsobj;
	algorithm * algorithmobj;
	hostglobalparams_t globalparams;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	value_t * tempvertexdatabuffer;
	edge2_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS];
	
	edge_t * packedvertexptrbuffer;
	uuint64_dt * packededgedatabuffer;
};
#endif








