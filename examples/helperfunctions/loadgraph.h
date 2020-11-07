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

// NEWCHANGE.
class loadgraph {
public:
	loadgraph(graph * graphobj, stats * _statsobj);
	loadgraph();
	~loadgraph();
	
	void loadvertexdata(edge_t * vertexptrbuffer, value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS]);
	void loadvertexdata_even(edge_t * vertexptrbuffer, value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container);
	void loadedgedata(edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS]);
	void loadedgedata_even(edge_t * vertexptrbuffer, edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container);
	void loadmessages(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container);
	void createmessages(
				uint512_vec_dt * kvstats,
				unsigned int srcvoffset,
				unsigned int srcvsize,
				unsigned int destvoffset,
				unsigned int firstvid,
				unsigned int firstkey,
				unsigned int firstvalue,
				unsigned int treedepth,
				unsigned int GraphIter,
				unsigned int GraphAlgo,
				unsigned int runsize,
				unsigned int batch_range,
				unsigned int batch_range_pow,
				unsigned int applyvertexbuffersz,
				unsigned int numlastlevelpartitions);
	
private:
	kernel * kernelobj;
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	helperfunctions2 * helperfunctionsobj;
};
// NEWCHANGE.
#endif







