#ifndef CREATEGRAPH_H
#define CREATEGRAPH_H
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/heuristics/heuristics.h"
#include "../../include/common.h"

class creategraph {
public:	
	creategraph();
	~creategraph();
	
	void create2Dgraph(unsigned int algorithmid, unsigned int datasetid);
};
#endif








