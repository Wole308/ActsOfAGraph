#ifndef CREATEGRAPH_H
#define CREATEGRAPH_H
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/heuristics/heuristics.h"
#include "../../include/common.h"
// #include "create2Dgraph.h"
#include "createNDgraph.h"

class creategraph {
public:	
	creategraph();
	~creategraph();
	
	void create2Dgraf(unsigned int datasetid);
	void analyzegraf(unsigned int datasetid);
};
#endif








