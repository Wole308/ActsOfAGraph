#ifndef ACTGRAPH_PR_SW_H
#define ACTGRAPH_PR_SW_H
#include <mutex>
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#include "../kernels/kernelprocess.h"
#include "common.h"

class actgraph_pr_sw {
public:
	actgraph_pr_sw(graph * graphobj);		
	actgraph_pr_sw();
	~actgraph_pr_sw();
	
	void run();
	void start2();
	void reloadenv();
	void finish();
	void summary();
	float totalkerneltime();
	float totalpopulateKvDRAMtime();
	
	void WorkerThread2(int threadidx, int threadidxoffset);

	void generatekvs(int threadidx, unsigned int flag, unsigned int subthreadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t * edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]);		
	void workerthread_generatekvs(unsigned int ddr, unsigned int flag, int threadidx, unsigned int subthreadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]);				
	void printstructures(unsigned int threadidx, unsigned int flag);
	void checkresultfromkernel(unsigned int threadidx, unsigned int flag, unsigned int totalnumkeyvalues[NUMDRAMBANKS]);
	void loadverticesdatafromfile(int nvmeFd_verticesdata_r2, unsigned int offset, keyvalue_t * kvdram, vertex_t bufferoffset, vertex_t verticessize);
	void loadverticesdatafromfile(int nvmeFd_verticesdata_r2, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size);
	void writeverticesdatatofile(int nvmeFd_verticesdata_r2, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexpropertiesfromfile(int nvmeFd_vertexproperties_r2, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t verticessize);
	void writevertexpropertiestofile(int nvmeFd_vertexproperties_w2, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t verticessize);
	void replicateverticesdata(keyvalue_t * kvdramA,keyvalue_t * kvdramB,keyvalue_t * kvdramC,keyvalue_t * kvdramD, unsigned int offset, unsigned int size);
	void cummulateverticesdata(unsigned int threadidx, keyvalue_t * kvdramA,keyvalue_t * kvdramB,keyvalue_t * kvdramC,keyvalue_t * kvdramD, unsigned int offset, unsigned int size, utility * utilityobj);			
	void loadvertexisactiveinfosfromfile(int fd, unsigned int offset, unsigned int * buffer, vertex_t bufferoffset, vertex_t size);
	void applyvertices(unsigned int threadidx, int bankoffset, keyvalue_t * kvdram, vertex_t offset, vertex_t size);
	void setgraphiteration(unsigned int _graph_iterationidx);
	unsigned int getflag(unsigned int giteration_idx);
	
	#ifdef FPGA_IMPL
	void loadOCLstructures(std::string binaryFile);
	void launchkernel(unsigned int threadidx, unsigned int flag);
	void finishOCL();
	void reloadOCLenv(xcl_world * _world, cl_program * _program, cl_kernel * _kernel);
	void allocateOCLbuffers();
	#endif 
	
private:
	uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	uint512_vec_dt * kvdestdram[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	keyvalue_t * kvstats[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	
	unsigned int graph_iterationidx;	
	
	float totaltime_ms;
	float totaltime_topkernel_ms;
	float totaltime_populatekvdram_ms;
	
	vector<uint512_vec_dt> unused_vectorbuffer;
	std::thread panas_thread[NUMCPUTHREADS];
	std::thread genw_thread[NUMCPUTHREADS][NUMDRAMBANKS];
	value_t * verticesdatabuffer[MAXNUMVERTEXBANKS];
	vertexprop_t * vertexpropertiesbuffer[MAXNUMVERTEXBANKS];
	unsigned int * isactivevertexinfobuffer_source[MAXNUMVERTEXBANKS];
	unsigned int * isactivevertexinfobuffer_dest[MAXNUMVERTEXBANKS];
	
	kernelprocess * kernel_process[NUMCPUTHREADS];
	edge_process * edge_process_obj[NUMCPUTHREADS];
	utility * utilityobj[NUMCPUTHREADS];
	algorithm * algorithmobj[NUMCPUTHREADS];
	graph * graphobj;
	
	edge_t totalnumkvsread[NUMCPUTHREADS];
	
	#ifdef FPGA_IMPL
	cl_int err;	
	size_t global; 
	size_t local;
	
	size_t kvsource_size_bytes;
	size_t kvdest_size_bytes;
	size_t kvstats_size_bytes;
	
	cl_event kernel_events[2];
	cl_event read_event[8][2];
	
	xcl_world world;
	cl_program program;
	cl_kernel kernel;
	
	cl_mem buffer_kvsourcedram[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	cl_mem buffer_kvdestdram[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	cl_mem buffer_kvstatsdram[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	#endif
};
#endif







