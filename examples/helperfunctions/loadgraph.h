#ifndef LOADGRAPH_H
#define LOADGRAPH_H
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/postprocess.h"
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
	
	// edge_t countedges(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, container_t * container);
	
	edge_t countedges(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int * srcvidsoffset, edge_t maxnumedgestoload, container_t * container);
	
	void loadvertexptrs(unsigned int col, edge_t * vertexptrbuffer, value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container);
	void loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t offset, vertex_t size);
	void loadedges(unsigned int col, edge_t * vertexptrbuffer, edge_type * edgedatabuffer, edge_type * edges[NUMSUBCPUTHREADS], unsigned int edgesbaseoffset, container_t * container, unsigned int GraphAlgo);
	vertex_t loadedges(unsigned int col, vertex_t srcvoffset, edge_t * vertexptrbuffer, edge_type * edgedatabuffer, edge_type * edges[NUMSUBCPUTHREADS], unsigned int edgesbaseoffset, container_t * container, unsigned int GraphAlgo);
	void loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], value_t * vertexdatabuffer, unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);				
	
	// void loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, value_t * vertexdatabuffer, edge_t * vertexptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	
	
	
	void loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int srcvidsoffset, value_t * vertexdatabuffer, edge_t * vertexptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	
	
	
	void loadactvvertices(vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	void loadmessages(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo);
	void createmessages(
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int actvvsize,
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
	postprocess * postprocessobj;
};
#endif







