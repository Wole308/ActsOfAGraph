#ifndef CREATE2DGRAPH
#define CREATE2DGRAPH
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../../include/host_common.h"
#include "../../include/common.h"

class create2Dgraph {
public:
	create2Dgraph(graph * _graphobj);
	create2Dgraph(graph * _graphobj, unsigned int dummy);
	~create2Dgraph();
	
	unsigned int getbank(vertex_t vertexid);
	template <class T>
	void clearedges(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS]);
	size_t hceildiv(size_t val1, size_t val2);
	
	void start();
	void analyzegraph();
	void summary();
	
	template <class T>
	void writeedgestofile(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS]);	
	void writeedgeoffsetbitstofile();
	void generateedgeoffsets();
	void writeedgeoffsetstofile();
	
	void printworkloadstats();
	
private:
	graph * graphobj;
	
	edge_t * lvertexoutdegrees[MAXNUMEDGEBANKS];
	edge_t bank_col_numvertices[MAXNUMEDGEBANKS];
	edge_t bank_col_numedges[MAXNUMEDGEBANKS];
	#ifdef STREAMEDGESSETUP
	std::vector<edgeprop1_t> edgepropertiesbuffer[MAXNUMEDGEBANKS];
	#else 
	std::vector<edgeprop2_t> edgepropertiesbuffer[MAXNUMEDGEBANKS];	
	#endif 
	xvertexoffset_t * edgeoffsets[MAXNUMEDGEBANKS];	
	unsigned int * edgeoffsetbits[MAXNUMEDGEBANKS]; // smaller precision structures
	edge_t totalnumedgeswritten[MAXNUMEDGEBANKS];
	
	// analyze graph 
	unsigned int * vertexoutdegrees;
	unsigned int * vertexindegrees;

	utility * utilityobj;
};
#endif








