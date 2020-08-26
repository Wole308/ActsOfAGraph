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

	void configureactivevertexreaders();

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
	
	int * getnvmeFd_verticesdata_r2();
	int * getnvmeFd_verticesdata_w2();
	int * getnvmeFd_tempverticesdata_r2(); 
	int * getnvmeFd_tempverticesdata_w2();
	int * getnvmeFd_vertexoutdegrees_r2();
	int * getnvmeFd_vertexproperties_r2();
	int * getnvmeFd_vertexproperties_w2();	
	int ** getnvmeFd_edgeproperties_r2();
	int ** getnvmeFd_edgeproperties_w2();
	int ** getnvmeFd_edgeoffsets_r2();
	int ** getnvmeFd_edgeoffsets_w2();
	int getnvmeFd_activevertexids_r2();
	int getnvmeFd_activevertexids_w2();
	FILE * getnvmeFd_activevertexids_w();
	int getnvmeFd_vertexisactive_r2();
	int getnvmeFd_vertexisactive_w2();
	SortReduceUtils::FileKvReader<uint32_t,uint32_t>* getreader_activevertexids(unsigned int i);
	
	vertexprop_t * getvertexpropertybuffer(unsigned int id);
	value_t * getvertexdatabuffer(unsigned int id);
	unsigned int * getvertexisactivebuffer(unsigned int id);
	
	void loadvertexpropertiesfromfile();
	void workerthread_loadvertexpropertiesfromfile(int ithreadidx, int fd, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexdatafromfile();
	void workerthread_loadvertexdatafromfile(int ithreadidx, int fd, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexdatafromfile(int bank, vertex_t fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexdatafromfile(int bank, vertex_t fdoffset, value_t * buffer, vertex_t bufferoffset, vertex_t size);
	void savevertexdatatofile(int bank, vertex_t fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadedgepropertyfromfile(int bank, int col, size_t fdoffset, edgeprop1_t * buffer, vertex_t bufferoffset, vertex_t datasize);
	void loadvertexpointersfromfile(int bank, int col, size_t fdoffset, prvertexoffset_t * buffer, vertex_t bufferoffset, vertex_t datasize);
	
	void generateverticesdata();
	void generatetempverticesdata();
	void generatevertexoutdegrees(vertex_t * vertexoutdegrees, unsigned int bank);
	void generatevertexproperties();
	vertex_t getnumactivevertices();
	void saveactiveverticestofile(vector<keyvalue_t> & activeverticesbuffer, unsigned int graph_iterationidx);
	void savevertexisactiveinfostofile(unsigned int fdoffset_unit, vertex_t bufferoffset, vertex_t datasize_uint, unsigned int graph_iterationidx);				
	void writerootvertextoactiveverticesfiles(keyy_t key, value_t value);
	void loadalldatasets();
	void setdataset(unsigned int id);
	dataset_t getdataset();
	unsigned long gettotalkeyvaluesread(unsigned int bank, unsigned int col);
	void appendkeyvaluesread(unsigned int bank, unsigned int col, unsigned int value);
	
private:
	algorithm * algorithmobj;
	utility * utilityobj;

	dataset_t _datasets[32];
	dataset_t thisdataset;
	unsigned int numedgebanks;
	unsigned int numvertexbanks;
	unsigned int numverticespervertexbank;
	
	int * nvmeFd_verticesdata_r2;
	int * nvmeFd_verticesdata_w2;
	int * nvmeFd_tempverticesdata_r2; 
	int * nvmeFd_tempverticesdata_w2;
	int * nvmeFd_vertexoutdegrees_r2; 
	int * nvmeFd_vertexproperties_r2;
	int * nvmeFd_vertexproperties_w2;	
	int ** nvmeFd_edgeproperties_r2; 
	int ** nvmeFd_edgeproperties_w2;
	int ** nvmeFd_edgeoffsets_r2; 
	int ** nvmeFd_edgeoffsets_w2;
	int nvmeFd_activevertexids_r2; 
	int nvmeFd_activevertexids_w2; 
	FILE * nvmeFd_activevertexids_w;	
	int nvmeFd_vertexisactive_r2; 
	int nvmeFd_vertexisactive_w2;	
	FILE ** nvmeFd_vertexupdates_w;
	
	value_t * vertexdatabuffer[MAXNUMVERTEXBANKS];
	vertexprop_t * vertexpropertybuffer[MAXNUMVERTEXBANKS];
	unsigned long * totalkeyvaluesread[MAXNUMSSDPARTITIONS][MAXNUMSSDPARTITIONS];
	SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader_activevertexids_r2[NUMCPUTHREADS];
	unsigned int * vertexisactivebitbuffer[MAXNUMVERTEXBANKS];
	std::thread mythread[MAXNUMVERTEXBANKS];
};
#endif








