#ifndef HEURISTICS_H
#define HEURISTICS_H
#include <mutex>
#include "../src/common.h"
#include "../src/host_common.h"

class heuristics {
public:
	heuristics();
	~heuristics();
	
	heuristicstype_t gettype(unsigned int algorithmID, unsigned int graph_iterationidx, size_t num_active_vertices);
	unsigned int getdefaultnumvertexbanks();
	unsigned int getdefaultnumedgebanks();
	
private:
	dataset_t dataset;
	heuristicstype_t heuristicstype;
};
#endif








