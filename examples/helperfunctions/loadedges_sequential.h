#ifndef LOADGRAPH_SPLITDSTVTXS_H
#define LOADGRAPH_SPLITDSTVTXS_H
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

typedef struct {
	edge2_type data[VECTOR_SIZE];
} uint512_evec_dt;

class loadedges_sequential {
public:
	loadedges_sequential(graph * graphobj, stats * _statsobj);
	loadedges_sequential();
	~loadedges_sequential();
	
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);

	unsigned int gethash(unsigned int vid);
	unsigned int allignlower(unsigned int val, unsigned int V_SIZE);
	unsigned int allignhigher(unsigned int val, unsigned int V_SIZE);
	
	globalparams_TWOt loadedges(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams);					
	
	/* unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth);
	globalparams_TWOt loadoffsetmarkers(vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams);
	void accumstats(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], globalparams_TWOt globalparams);
	
	globalparams_TWOt generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * vdram, globalparams_TWOt globalparams);
	
	void savevdata(uint512_vec_dt * kvdram, unsigned int buffer[VDATA_PACKINGSIZE][REDUCEPARTITIONSZ_KVS2], batch_type vbaseoffset_kvs, batch_type voffset_kvs);
	void setrootvid(unsigned int Algo, uint512_vec_dt * kvbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams); */
	
private:
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	evalparams * evalparamsobj;
};
#endif







