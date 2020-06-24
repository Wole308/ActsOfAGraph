#ifndef GENERATEACTIVEVERTICES_H
#define GENERATEACTIVEVERTICES_H
#include <mutex>
#include "graph.h"
#include "../src/common.h"

class generateactivevertices {
public:
	generateactivevertices(graph * _graphobj);
	~generateactivevertices();
	
	void generate();
	
private:
	graph * graphobj;
};
#endif








