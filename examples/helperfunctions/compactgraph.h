#ifndef COMPACTGRAPH_H
#define COMPACTGRAPH_H
#include <mutex>
#include <thread>
#include <string.h>
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

// #define _DEBUGMODE_COMPACTGRAPH

class compactgraph {
public:
	compactgraph(graph * graphobj, stats * _statsobj);
	compactgraph();
	~compactgraph();

	unsigned int getllpartition(unsigned int data);
	mail_t shrink(unsigned int x);
	void compact(edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * packedvertexptrbuffer, uuint64_dt * packededgedatabuffer);		
	
	void verify(edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * packedvertexptrbuffer, uuint64_dt * packededgedatabuffer, unsigned int * numitemspacked);
	
private:
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
};
#endif







