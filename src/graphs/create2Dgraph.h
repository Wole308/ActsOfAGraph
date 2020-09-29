#ifndef CREATE2DGRAPH
#define CREATE2DGRAPH
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../../include/host_common.h"
#include "../../include/common.h"

#define THRESHOLDLINECNT 10000000
#define YDIMENSIONTHRESHOLD 1000

class create2Dgraph {
public:
	create2Dgraph(graph * _graphobj, unsigned int dummy);
	create2Dgraph(graph * _graphobj);
	~create2Dgraph();
	
	void run();
	
	void initialize(unsigned int groupid);
	
	void start();
	
	void analyzegraph();
	void transformgraph();
	
	void summary();
	
	template <class T>
	void writeedgestofile(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS]);	
	void writeedgeoffsetbitstofile();
	void generateedgeoffsets();
	void writeedgeoffsetstofile();
	unsigned int getbank(vertex_t vertexid);
	template <class T>
	void clearedges(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS]);
	unsigned int getgroup(unsigned int vid);
	unsigned int gettransformedglobalid(unsigned int vertexid);
	unsigned int getlocalid(unsigned int srcv);
	void printworkloadstats();
	
private:
	graph * graphobj;
	
	edge_t * lvertexoutdegrees[MAXNUMEDGEBANKS];
	edge_t * lvertexindegrees[MAXNUMEDGEBANKS];
	edge_t bank_col_numoutvertices[MAXNUMEDGEBANKS];
	edge_t bank_col_numinvertices[MAXNUMEDGEBANKS];
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
	unsigned int * global_to_transfglobal_ids;

	utility * utilityobj;
	unsigned int groupid;
};
#endif








