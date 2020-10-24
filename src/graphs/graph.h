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
	// graph(unsigned int datasetid);
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
	SortReduceUtils::FileKvReader<uint32_t,uint32_t>* getreader_activevertexids(unsigned int i);
	
	vertexprop_t * getvertexpropertybuffer();
	value_t * getvertexdatabuffer();
	unsigned int * getvertexisactivebuffer();
	
	void loadvertexpropertiesfromfile();
	void workerthread_loadvertexpropertiesfromfile(int fd, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexdatafromfile();
	void workerthread_loadvertexdatafromfile(int fd, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexdatafromfile(vertex_t fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexdatafromfile(vertex_t fdoffset, value_t * buffer, vertex_t bufferoffset, vertex_t size);
	void savevertexdatatofile(vertex_t fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadedgesfromfile(int col, size_t fdoffset, edgeprop1_t * buffer, vertex_t bufferoffset, vertex_t datasize);
	void loadvertexpointersfromfile(int col, size_t fdoffset, prvertexoffset_t * buffer, vertex_t bufferoffset, vertex_t datasize);
	void loadedgesfromfile(int col, size_t fdoffset, edge_type * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexptrsfromfile(int col, size_t fdoffset, edge_t * buffer, vertex_t bufferoffset, vertex_t size);
	
	void generateverticesdata();
	void generatetempverticesdata();
	void generatevertexoutdegrees(vertex_t * vertexoutdegrees);
	void generatevertexproperties();
	vertex_t getnumactivevertices();
	void saveactiveverticestofile(vector<keyvalue_t> & activeverticesbuffer, unsigned int graph_iterationidx);
	void savevertexisactiveinfostofile(unsigned int fdoffset_unit, vertex_t bufferoffset, vertex_t datasize_uint, unsigned int graph_iterationidx);				
	void writerootvertextoactiveverticesfiles(keyy_t key, value_t value);
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
	vertexprop_t * vertexpropertybuffer;
	unsigned long * totalkeyvaluesread[MAXNUMSSDPARTITIONS];
	SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader_activevertexids_r2[NUMCPUTHREADS];
	unsigned int * vertexisactivebitbuffer;
	std::thread mythread;
};
#endif








