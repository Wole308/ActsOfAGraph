#ifndef CREATE2DGRAPH
#define CREATE2DGRAPH
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../src/host_common.h"
#include "../src/common.h"

class create2Dgraph {
public:
	create2Dgraph(graph * _graphobj, unsigned int _startworkbank, unsigned int _numworkbanks);
	~create2Dgraph();
	void reload(unsigned int _startbank, unsigned int _numbanks);
	
	void start();
	void deletedynamicmemories();
	
	void writeedgeoffsetstofile();
	void generateedgeoffsets();
	template <class T>
	void writeedgestofile(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS]);	
	void writeedgeoffsetbitstofile();
	void generateedgeoffsetbits();

	vertex_t getlocalvid(unsigned int bank, vertex_t globalvid);
	unsigned int getbank(vertex_t vertexid);
	template <class T>
	void clearedges(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS]);
	size_t hceildiv(size_t val1, size_t val2);
	void summary();
	void printworkloadstats();
	void printglobaltolocaltranslator();
	void checkedgeoffsets();
	void checkedges();
	void checkledges();
	
	void InsertBit(unsigned int * edgeoffsetbits, size_t index, unsigned int bit);
	
private:
	graph * graphobj;
	dataset_t dataset;
	
	unsigned int startbank;
	unsigned int numbanks;
	unsigned int numvertexbanks;
	unsigned int numedgebanks;
	unsigned int numverticespervertexbank;
	
	FILE * nvmeFd_edgeproperties_w[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	FILE * nvmeFd_edgeoffsets_w[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];	
	FILE * nvmeFd_edgeproperties_r[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	FILE * nvmeFd_edgeoffsets_r[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	
	edge_t * lvertexoutdegrees[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	edge_t bank_col_numvertices[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	edge_t bank_col_numedges[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	#ifdef STREAMEDGESSETUP
	std::vector<edgeprop1_t> edgepropertiesbuffer[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	#else 
	std::vector<edgeprop2_t> edgepropertiesbuffer[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];	
	#endif 
	xvertexoffset_t * edgeoffsets[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];	
	unsigned int * edgeoffsetbits[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS]; // smaller precision structures
	edge_t totalnumedgeswritten[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	
	int * globaltolocalvid[MAXNUMEDGEBANKS];
	vertex_t globaltolocalvid_weof[MAXNUMEDGEBANKS];
	
	utility * utilityobj;
};
#endif








