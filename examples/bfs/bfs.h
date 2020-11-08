#ifndef BFS_H
#define BFS_H
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
#include "bfs.h"

class bfs {
public:
	bfs(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~bfs();
	void finish();
	
	runsummary_t run();
	void WorkerThread(vector<vertex_t> &currentactivevertices, vector<vertex_t> &nextactivevertices, container_t * container, hostglobalparams_t globalparams);
	
	void loadgraphdata(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, container_t * container);
	void loadbalancedgraphdata(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	
private:
	graph * graphobj;
	parameters * parametersobj[NUMSUPERCPUTHREADS];
	utility * utilityobj[NUMSUPERCPUTHREADS];
	helperfunctions2 * helperfunctionsobj[NUMSUPERCPUTHREADS];
	loadgraph * loadgraphobj[NUMSUPERCPUTHREADS];
	stats * statsobj;
	hostglobalparams_t globalparams;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	edge_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








