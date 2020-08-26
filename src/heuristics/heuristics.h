#ifndef HEURISTICS_H
#define HEURISTICS_H
#include <mutex>
#include "../../include/common.h"

class heuristics {
public:
	heuristics();
	~heuristics();
	
	heuristicstype_t gettype(unsigned int algorithmID, unsigned int graph_iterationidx, size_t num_active_vertices);
	unsigned int getdefaultnumvertexbanks();
	unsigned int getdefaultnumedgebanks();
	
private:
	heuristicstype_t heuristicstype;
};
#endif








