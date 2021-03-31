#ifndef LOADGRAPH_SW_H
#define LOADGRAPH_SW_H
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/evalparams.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

class loadgraph_sw {
public:
	loadgraph_sw(graph * graphobj, stats * _statsobj);
	loadgraph_sw();
	~loadgraph_sw();
	
	void loadedges_rowwise(graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS]);
	
private:
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	evalparams * evalparamsobj;
};
#endif







