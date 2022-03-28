#ifndef LOADGRAPH_H
#define LOADGRAPH_H
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/evalparams.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
// #include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

#define EDGESTATUS_VALIDEDGE 5
#define EDGESTATUS_INVALIDEDGE 6
#define EDGESTATUS_BITMAP 7
#define EDGESTATUS_SOURCEV 8
#define EDGESTATUS_DUMMYEDGE 9

class loadgraph {
public:
	loadgraph(graph * graphobj, stats * _statsobj);
	loadgraph();
	~loadgraph();
	
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	globalparams_t loadvertexdata(unsigned int Algo, value_t * vertexdatabuffer, keyvalue_t * kvbuffer, vertex_t kvbufferoffset_xxx, vertex_t size_xxx, globalparams_t globalparams, unsigned int edgesorkv, unsigned int srcordest);
	
	globalparams_TWOt loadactvvertices(vector<vertex_t> &activevertices, keyy_t * kvbuffer, container_t * container, globalparams_TWOt globalparams);
	
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth);
	globalparams_TWOt loadoffsetmarkers(edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams);
	void accumstats(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], globalparams_TWOt globalparams);
	
	globalparams_TWOt generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * vdram, globalparams_TWOt globalparams);
	
	void savevdata(uint512_vec_dt * kvdram, unsigned int buffer[VDATA_PACKINGSIZE][REDUCEPARTITIONSZ_KVS2], batch_type vbaseoffset_kvs, batch_type voffset_kvs);
	void setrootvid(unsigned int Algo, uint512_vec_dt * kvbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams);
	
	globalparams_TWOt loadmessages(uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo, globalparams_TWOt globalparams);
	globalparams_t createmessages(
			unsigned int id,
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int actvvsize,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			unsigned int _NUMPROCESSEDGESPARTITIONS,
			unsigned int _NUMREDUCEPARTITIONS,
			globalparams_t globalparams);	

	void setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], unsigned int evalid);
	
	globalparams_t finishglobaparamsV(globalparams_t globalparams);
	
	globalparams_t finishglobaparamsM(globalparams_t globalparams);

private:
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	evalparams * evalparamsobj;
};
#endif







