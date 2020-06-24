#ifndef GRAPH_H
#define GRAPH_H
#include "../algorithm/algorithm.h"
#include "../src/common.h"
#include "../src/host_common.h"

class graph {
public:
	graph(unsigned int datasetid, vertex_t batchsz);
	graph(algorithm * algorithmobj, unsigned int datasetid, unsigned int numvertexbanks, unsigned int numedgebanks);
	~graph();
	
	size_t get_num_vertices();
	size_t get_num_edges();
	size_t getnumedgebanks();
	void setnumedgebanks(unsigned int _numedgebanks);
	size_t getnumvertexbanks();
	void setnumvertexbanks(unsigned int _numvertexbanks);
	void setbanks(unsigned int _numvertexbanks, unsigned int _numedgebanks);
	
	void openfilesforreading();
	void opentemporaryfilesforreading();
	void opentemporaryfilesforwriting();
	void closefilesforreading();
	void closetemporaryfilesforreading();
	void closetemporaryfilesforwriting();
	void openactiveverticesfilesforreading(unsigned int graph_iterationidx);
	void openactiveverticesfilesforwriting(unsigned int graph_iterationidx);
	void closeactiveverticesfilesforreading();
	void closeactiveverticesfilesforwriting();
	void openedgesfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks); // NB: CAUTION WITH THIS
	void openedgeoffsetsfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks);
	void openedgeoffsetbitsfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks);
	void openedgesfileforreading(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks);
	void openedgeoffsetsfileforreading(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks);
	
	void closeedgesfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks);
	void closeedgesfileforreading(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks);
	void closeedgeoffsetsfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks);
	void closeedgeoffsetsfileforreading(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks);
	
	string getpath_vertexdata(unsigned int i);
	string getpath_tempvertexdata(unsigned int i);
	string getpath_vertexproperties(unsigned int i);
	string getpath_edgesproperties(unsigned int i, unsigned int j);
	string getpath_edgeoffsets(unsigned int i, unsigned int j);
	string getpath_activevertexids(unsigned int i);
	string getpath_activevertices(unsigned int graph_iterationidx);
	string getpath_activeverticesW(unsigned int graph_iterationidx);
	string getpath_vertexisactive(unsigned int graph_iterationidx);
	string getpath_vertexupdates(unsigned int i);
	string gettmp_dir();
	string getidx_path();
	string getmat_path();	
	string gettmppath_parentvid(); // files for createsmartgraph.cpp
	string gettmppath_childrenstats();
	
	int * getnvmeFd_verticesdata_r2(); // vertex data
	int * getnvmeFd_verticesdata_w2();
	int * getnvmeFd_tempverticesdata_r2(); // temporary vertex data
	int * getnvmeFd_tempverticesdata_w2();
	int * getnvmeFd_vertexoutdegrees_r2();
	int * getnvmeFd_vertexproperties_r2(); // vertex properties
	int * getnvmeFd_vertexproperties_w2();	
	int ** getnvmeFd_edgeproperties_r2(); // edge properties
	int ** getnvmeFd_edgeproperties_w2();
	FILE *** getnvmeFd_edgeproperties_r();
	FILE *** getnvmeFd_edgeproperties_w();
	int ** getnvmeFd_edgeoffsets_r2(); // edge offsets
	int ** getnvmeFd_edgeoffsets_w2();
	FILE *** getnvmeFd_edgeoffsets_r();
	FILE *** getnvmeFd_edgeoffsets_w();
	int getnvmeFd_activevertexids_r2(); // active vertices
	int getnvmeFd_activevertexids_w2();
	FILE * getnvmeFd_activevertexids_w();
	int getnvmeFd_vertexisactive_r2(); // vertex-is-active 
	int getnvmeFd_vertexisactive_w2();
	FILE ** getnvmeFd_vertexupdates_w(); // temporary vertex updates
	int gettmpnvmeFd_parents_w2(); // files for createsmartgraph.cpp
	int gettmpnvmeFd_parents_r2();
	int gettmpnvmeFd_children_w2();
	int gettmpnvmeFd_children_r2();
	
	void generateverticesdata();
	void generatetempverticesdata();
	void generatevertexoutdegrees(vertex_t * vertexoutdegrees, unsigned int bank);
	void generatevertexproperties();
	vertex_t getnumactivevertices();
	void saveactiveverticestofile(unsigned int graph_iterationidx, int fd);
	void saveactiveverticestofile(unsigned int graph_iterationidx, keyvalue_t keyvalue, bool save=true);
	void saveactiveverticestofile(FILE * nvmeFd_activevertexids_w, vector<keyvalue_t> & activeverticesbuffer, unsigned int graph_iterationidx);
	void savevertexisactiveinfostofile(int fd, unsigned int offset, unsigned int * buffer, vertex_t bufferoffset, vertex_t size, unsigned int graph_iterationidx);
	void writerootvertextoactiveverticesfiles(keyy_t key, value_t value);
	
	void loadalldatasets();
	void setdataset(unsigned int id);
	dataset_t getdataset();

	void getedges(vertex_t vid, vector<edgeprop2_t> & edges);
	void getedges(vertex_t vid, unsigned int bank, unsigned int col, vector<edgeprop2_t> & edges);
	void addedges(vertex_t vid, vector<edgeprop2_t> & edges);
	void addedges(vertex_t vid, unsigned int bank, unsigned int col, vector<edgeprop2_t> & edges);
	void initializecommunity();
	int getparent(int childvid);
	void setparent(int childvid, int parentvid);
	void removeparent(int childvid);
	unsigned int getnumchildren(unsigned int parentvid);
	unsigned int getnum1stdescendants();
	unsigned int getnumfriends();
	void printparents(unsigned int start, unsigned int size);
	void printchildrencount(unsigned int start, unsigned int size);
	
private:
	dataset_t _datasets[32];
	dataset_t thisdataset;
	
	unsigned int numedgebanks;
	unsigned int numvertexbanks;
	unsigned int numverticespervertexbank;
	
	int * nvmeFd_verticesdata_r2; // vertex data
	int * nvmeFd_verticesdata_w2;
	int * nvmeFd_tempverticesdata_r2; // temporary vertex data
	int * nvmeFd_tempverticesdata_w2;
	int * nvmeFd_vertexoutdegrees_r2; 
	int * nvmeFd_vertexproperties_r2; // vertex properties
	int * nvmeFd_vertexproperties_w2;	
	int ** nvmeFd_edgeproperties_r2; // edge properties
	int ** nvmeFd_edgeproperties_w2;
	FILE *** nvmeFd_edgeproperties_r;
	FILE *** nvmeFd_edgeproperties_w;	
	int ** nvmeFd_edgeoffsets_r2; // edge offsets
	int ** nvmeFd_edgeoffsets_w2;
	FILE *** nvmeFd_edgeoffsets_r;
	FILE *** nvmeFd_edgeoffsets_w;
	int nvmeFd_activevertexids_r2; // active vertices
	int nvmeFd_activevertexids_w2; 
	FILE * nvmeFd_activevertexids_w;	
	int nvmeFd_vertexisactive_r2; // vertex-is-active 
	int nvmeFd_vertexisactive_w2;	
	FILE ** nvmeFd_vertexupdates_w;
	int tmpnvmeFd_parents_w2; // files for createsmartgraph.cpp
	int tmpnvmeFd_parents_r2;
	int tmpnvmeFd_children_w2;
	int tmpnvmeFd_children_r2;
	
	int * parents;
	int * childrencount;
	
	algorithm * algorithmobj;
};
#endif








