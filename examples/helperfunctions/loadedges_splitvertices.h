#ifndef LOADGRAPH_SPLITVERTICES_H
#define LOADGRAPH_SPLITVERTICES_H
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

class loadedges_splitvertices {
public:
	loadedges_splitvertices(graph * graphobj, stats * _statsobj);
	loadedges_splitvertices();
	~loadedges_splitvertices();
	
	unsigned int ACTSUTIL_GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int ACTSUTIL_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void ACTSUTIL_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);

	unsigned int gethash(unsigned int vid);
	unsigned int allignlower(unsigned int val, unsigned int V_SIZE);
	
	globalparams_TWOt loadedges(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams);					
	void get_counts(unsigned int H, unsigned int * counts[NUMSUBCPUTHREADS], edge2_type edge, unsigned int isFirst, unsigned int isLast);
	
private:
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	evalparams * evalparamsobj;
};
#endif







