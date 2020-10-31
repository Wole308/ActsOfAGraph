#ifndef CREATENDGRAPH
#define CREATENDGRAPH
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../../include/host_common.h"
#include "../../include/common.h"

#define CREATENDGRAPH_BATCHSIZE 10000000

// #define YDIMENSIONTHRESHOLD 1000
#define YDIMENSIONTHRESHOLD (1 << 29) // infinite

class creategraphs {
public:
	creategraphs(unsigned int datasetid);
	~creategraphs();
	
	void run();
	
	void createdatastructures();
	void resetdatastructures(unsigned int groupid);
	
	void start();
	
	void analyzegraph();
	void transformgraph();
	
	void summary();
	
	void writeedgestofile(std::vector<edge_type> (&edgesbuffer)[MAXNUMEDGEBANKS]);
	void writeedgestofile(std::vector<edge2_type> (&edges2buffer)[MAXNUMEDGEBANKS]);

	void clearedges(std::vector<edge_type> (&edgesbuffer)[MAXNUMEDGEBANKS]);
	void clearedges(std::vector<edge2_type> (&edges2buffer)[MAXNUMEDGEBANKS]);

	void writevertexptrstofile();

	unsigned int getbank(vertex_t vertexid);
	unsigned int getgroup(unsigned int vid);
	unsigned int gettransformedglobalid(unsigned int vertexid);
	unsigned int getlocalid(unsigned int srcv);
	void printworkloadstats();
	
private:
	graph * graphobj;
	
	edge_t * lvertexoutdegrees[MAXNUMEDGEBANKS];
	edge_t * lvertexindegrees[MAXNUMEDGEBANKS];
	edge_t numedges[MAXNUMEDGEBANKS];
	std::vector<edge_type> edgesbuffer[MAXNUMEDGEBANKS];
	// std::vector<edge2_type> edges2buffer[MAXNUMEDGEBANKS];
	edge_t * vertexptrs[MAXNUMEDGEBANKS];	
	edge_t totalnumedgeswritten[MAXNUMEDGEBANKS];
	edge_type firstedge[NUMSSDPARTITIONS];
	
	// analyze graph 
	unsigned int * vertexoutdegrees;
	unsigned int * vertexindegrees;
	unsigned int * global_to_transfglobal_ids;

	utility * utilityobj;
	unsigned int groupid;
};
#endif








