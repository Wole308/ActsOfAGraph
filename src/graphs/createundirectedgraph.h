#ifndef CREATEUNDIRECTEDGRAPH
#define CREATEUNDIRECTEDGRAPH
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../../include/host_common.h"
#include "../../include/common.h"

class createundirectedgraph {
public:
	createundirectedgraph(unsigned int datasetid);
	~createundirectedgraph();
	
	void start();
	
private:
	graph * graphobj;
	utility * utilityobj;
	
	edge2_type * edgedatabuffer_dup;
	edge2_type * edgedatabuffer;
	
	edge_t * vertexptrbuffer_dup;
	edge_t * vertexptrbuffer;
	
	int * inoutdegree_dup;
	
	FILE * nvmeFd_edges_w;	
	FILE * nvmeFd_vertexptrs_w;
	
	unsigned int num_edges;
};
#endif








