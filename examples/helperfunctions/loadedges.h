#ifndef LOADEDGES_H
#define LOADEDGES_H
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

class loadedges {
public:
	loadedges(graph * graphobj, stats * _statsobj);
	loadedges();
	~loadedges();
	
	globalparams_TWOt loadedgess(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams);					

	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth);
	globalparams_TWOt loadoffsetmarkers(vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams);
	void accumstats(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], globalparams_TWOt globalparams);
	
	void savevmasks(bool_type enable, uint512_vec_dt * kvbuffer, keyvalue_vec_bittype vmask[VMASK_PACKINGSIZE][REDUCEBUFFERSZ], batch_type offset_kvs, buffer_type size_kvs);
	globalparams_TWOt generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * vdram, globalparams_TWOt globalparams);
	
private:
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	evalparams * evalparamsobj;
};
#endif







