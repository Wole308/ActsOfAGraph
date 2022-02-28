#ifndef LOADGRAPH_RANDOM_H
#define LOADGRAPH_RANDOM_H
#include <mutex>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/evalparams.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "../../acts/include/actscommon.h"
#include "loadedges_random.h"
using namespace std;

#define EDGESTATUS_VALIDEDGE 5
#define EDGESTATUS_INVALIDEDGE 6
#define EDGESTATUS_BITMAP 7
#define EDGESTATUS_SOURCEV 8
#define EDGESTATUS_DUMMYEDGE 9

class loadedges_random {
public:
	loadedges_random(graph * graphobj, stats * _statsobj);
	loadedges_random();
	~loadedges_random();
	
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	unsigned int allignlower(unsigned int val, unsigned int V_SIZE);
	unsigned int allignhigher(unsigned int val, unsigned int V_SIZE);
	
	unsigned int gethash(unsigned int vid);
	unsigned int getlocalvid(unsigned int vid);

	unsigned int twist_getpartition(unsigned int partition, unsigned int s);
	
	unsigned int getpartition(unsigned int data, vertex_t upperlimit);
	unsigned int getpartition2(unsigned int data, vertex_t upperlimit);
	tuple_t get_partition_and_incr(unsigned int vid, vertex_t upperlimit);
	
	void calculateoffsets(keyvalue_t * buffer, unsigned int size);
	
	globalparams_TWOt loadedges(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams);					

private:
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
	evalparams * evalparamsobj;
};
#endif







