#ifndef LOADGRAPH_H
#define LOADGRAPH_H
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/postprocess.h"
#include "../../examples/helperfunctions/postprocess.h"
#include "../../examples/helperfunctions/evalparams.h"
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

	void loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer, vertex_t offset, vertex_t size);
	void loadactvvertices(vector<vertex_t> &srcvids, keyy_t * kvbuffer, container_t * container);
	
	void loadedges_columnwise(unsigned int col, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, value_t * vertexdatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo);
	
	#ifdef COMPACTEDGES
	void loadedges_rowwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, uuint64_dt * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], uuint64_dt * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo);
	#else 
	void loadedges_rowwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo);
	#endif
	
	#ifdef COMPACTEDGES
	void loadedges_rowblockwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, uuint64_dt * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], uuint64_dt * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo);
	#else 
	void loadedges_rowblockwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo);
	#endif
	
	#ifdef COMPACTEDGES
	void loadoffsetmarkers(uuint64_dt * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], container_t * container);
	#else 
	void loadoffsetmarkers(edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], container_t * container);
	#endif
	void loadvertexptrs(unsigned int col, edge_t * vertexptrbuffer, value_t * vertexdatabuffer, vptr_type * kvbuffer[NUMSUBCPUTHREADS], container_t * container);
	
	edge_t countedges(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int * srcvidsoffset, edge_t maxnumedgestoload, container_t * container);
	void loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], value_t * vertexdatabuffer, unsigned int balancededgesizes[NUMSUBCPUTHREADS], container_t * container);				
	void loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int srcvidsoffset, value_t * vertexdatabuffer, 
								edge_t * vertexptrs[NUMSUBCPUTHREADS], value_t * verticesdata[NUMSUBCPUTHREADS], edge2_type * edges[NUMSUBCPUTHREADS], unsigned int balancededgesizes[NUMSUBCPUTHREADS], container_t * container);
	vertex_t loadedges_columnwise(unsigned int col, vertex_t srcvoffset, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_type * edges[NUMSUBCPUTHREADS], edge_t baseoffset_edgedata, container_t * container, unsigned int GraphAlgo);
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth);
	void loadkvstats(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container);
	
	void loadmessages(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo);
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

	void setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], unsigned int evalid);

private:
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	postprocess * postprocessobj;
	evalparams * evalparamsobj;
};
#endif







