#ifndef GRAPH_H
#define GRAPH_H
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../../src/heuristics/heuristics.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/utility/utility.h"
#include "../../include/host_common.h"
#include "../../include/common.h"

class graph {
public:
	graph(algorithm * _algorithmobj, unsigned int datasetid, unsigned int _numedgebanks, bool _initfiles, bool _initgraphstructures, bool _initstatstructures);						
	~graph();
	
	void initializefiles();
	void initgraphstructures();
	void initstatstructures();
	
	size_t get_num_vertices();
	size_t get_num_edges();
	void setnumvertices(unsigned int num_vertices);
	
	size_t getnumedgebanks();
	void setnumedgebanks(unsigned int _numedgebanks);
	
	void setbanks(unsigned int _numedgebanks);

	void configureactivevertexreaders();

	void openfilesforreading(unsigned int groupid);
	void openfilesforwriting(unsigned int groupid);
	
	void closefilesforreading();
	void closefilesforwriting();
	
	void opentemporaryfilesforreading();
	void opentemporaryfilesforwriting();
	
	void closetemporaryfilesforreading();
	void closetemporaryfilesforwriting();
	
	void openactiveverticesfilesforreading(unsigned int graph_iterationidx);
	void openactiveverticesfilesforwriting(unsigned int graph_iterationidx);
	
	void closeactiveverticesfilesforreading();
	void closeactiveverticesfilesforwriting();
	
	string getdatasetdir();
	string getpath_vertexdata();
	string getpath_tempvertexdata();
	string getpath_vertexproperties();
	string getpath_edges(unsigned int groupid, unsigned int j);
	string getpath_vertexptrs(unsigned int groupid, unsigned int j);
	string getpath_activevertexids();
	string getpath_activevertices(unsigned int graph_iterationidx);
	string getpath_activeverticesW(unsigned int graph_iterationidx);
	string getpath_vertexisactive(unsigned int graph_iterationidx);
	string getpath_vertexupdates();
	string gettmp_dir();
	string getidx_path();
	string getmat_path();	
	string gettmppath_parentvid(); // files for createsmartgraph.cpp
	string gettmppath_childrenstats();
	
	int getnvmeFd_verticesdata_r2();
	int getnvmeFd_verticesdata_w2();
	int getnvmeFd_tempverticesdata_r2(); 
	int getnvmeFd_tempverticesdata_w2();
	int getnvmeFd_vertexoutdegrees_r2();
	int getnvmeFd_vertexproperties_r2();
	int getnvmeFd_vertexproperties_w2();	
	int * getnvmeFd_edges_r2();
	int * getnvmeFd_edges_w2();
	FILE ** getnvmeFd_edges_r();
	FILE ** getnvmeFd_edges_w();
	int * getnvmeFd_vertexptrs_r2();
	int * getnvmeFd_vertexptrs_w2();
	FILE ** getnvmeFd_vertexptrs_r();
	FILE ** getnvmeFd_vertexptrs_w();
	int getnvmeFd_activevertexids_r2();
	int getnvmeFd_activevertexids_w2();
	FILE * getnvmeFd_activevertexids_w();
	int getnvmeFd_vertexisactive_r2();
	int getnvmeFd_vertexisactive_w2();
	
	vertexprop_t * getvertexpropertybuffer();
	value_t * getvertexdatabuffer();
	value_t * gettempvertexdatabuffer();
	edge_t * getvertexptrbuffer(); 
	unsigned int * getvertexisactivebuffer();
	
	edge_t * loadvertexptrsfromfile(int col); 
	void loadvertexptrsfromfile(int col, size_t fdoffset, edge_t * buffer, vertex_t bufferoffset, vertex_t size);
	value_t * generateverticesdata();
	value_t * generatetempverticesdata();
	void loadedgesfromfile(int col, size_t fdoffset, edge_type * buffer, vertex_t bufferoffset, vertex_t size);
	edge_t getedgessize(int col); 
	
	// void generatetempverticesdata();
	void generatevertexoutdegrees(vertex_t * vertexoutdegrees);
	void generatevertexproperties();
	
	void loadalldatasets();
	void setdataset(unsigned int id);
	dataset_t getdataset();
	void printdataset();
	unsigned long gettotalkeyvaluesread(unsigned int col);
	void appendkeyvaluesread(unsigned int col, unsigned int value);
	
private:
	algorithm * algorithmobj;
	utility * utilityobj;

	dataset_t _datasets[128];
	dataset_t thisdataset;
	unsigned int numedgebanks;
	unsigned int numverticespervertexbank;
	
	int nvmeFd_verticesdata_r2;
	int nvmeFd_verticesdata_w2;
	int nvmeFd_tempverticesdata_r2; 
	int nvmeFd_tempverticesdata_w2;
	int nvmeFd_vertexoutdegrees_r2; 
	int nvmeFd_vertexproperties_r2;
	int nvmeFd_vertexproperties_w2;	
	int * nvmeFd_edges_r2;
	int * nvmeFd_edges_w2;
	FILE ** nvmeFd_edges_r;
	FILE ** nvmeFd_edges_w;	
	int * nvmeFd_vertexptrs_r2; 
	int * nvmeFd_vertexptrs_w2;
	FILE ** nvmeFd_vertexptrs_r;
	FILE ** nvmeFd_vertexptrs_w;
	int nvmeFd_activevertexids_r2; 
	int nvmeFd_activevertexids_w2; 
	FILE * nvmeFd_activevertexids_w;	
	int nvmeFd_vertexisactive_r2; 
	int nvmeFd_vertexisactive_w2;	
	FILE ** nvmeFd_vertexupdates_w;
	
	value_t * vertexdatabuffer;
	value_t * tempvertexdatabuffer;
	vertexprop_t * vertexpropertybuffer;
	edge_t * vertexptrbuffer; 
	unsigned long * totalkeyvaluesread[MAXNUMSSDPARTITIONS];
	unsigned int * vertexisactivebitbuffer;
};
#endif








