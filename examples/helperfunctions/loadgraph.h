#ifndef LOADGRAPH_H
#define LOADGRAPH_H
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/evalparams.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

class loadgraph {
public:
	loadgraph(graph * graphobj, stats * _statsobj);
	loadgraph();
	~loadgraph();
	
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth);

	globalparams_t loadedges_rowblockwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo, globalparams_t globalparams);					
	
	globalparams_t loadoffsetmarkers(edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], container_t * container, globalparams_t globalparams);
	
	globalparams_t loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer, vertex_t offset, vertex_t size, globalparams_t globalparams);
	
	void setrootvid(value_t * kvbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams);

	globalparams_t loadactvvertices(vector<vertex_t> &activevertices, keyy_t * kvbuffer, container_t * container, globalparams_t globalparams);
	
	void savevmasks(bool_type enable, uint512_vec_dt * kvbuffer, keyvalue_vec_bittype vmask[NUM_PARTITIONS][REDUCEBUFFERSZ], batch_type offset_kvs, buffer_type size_kvs);
	globalparams_t generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], globalparams_t globalparams);
	
	globalparams_t loadmessages(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo, globalparams_t globalparams);
	globalparams_t createmessages(
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int actvvsize,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			globalparams_t globalparams);	

	void setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], unsigned int evalid);

private:
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	evalparams * evalparamsobj;
};
#endif







