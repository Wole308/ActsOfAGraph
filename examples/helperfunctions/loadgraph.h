#ifndef LOADGRAPH_H
#define LOADGRAPH_H
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

class loadgraph {
public:
	loadgraph(graph * graphobj, stats * _statsobj);
	loadgraph();
	~loadgraph();
	
	void loadvertexptrs(unsigned int col, edge_t * vertexptrbuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container);
	void loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t offset, vertex_t size);
	void loadedgedata(unsigned int col, edge_t * vertexptrbuffer, edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container);					
	void loadmessages(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container);
	
	edge_t countedges(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, container_t * container);
	void loadgraphdata(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], value_t * vertexdatabuffer, unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);						

private:
	kernel * kernelobj;
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	helperfunctions2 * helperfunctionsobj;
};
#endif







