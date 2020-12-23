#ifndef MUTATEGRAPH_H
#define MUTATEGRAPH_H
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

class mutategraph {
public:
	mutategraph(graph * graphobj, stats * _statsobj);
	mutategraph();
	~mutategraph();

	mail_t shrink(unsigned int x);
	void push(uuint64_dt * longword, mail_t kv);
	void mutate(edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * packedvertexptrbuffer, uuint64_dt * packededgedatabuffer);		

private:
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	stats * statsobj;
};
#endif







