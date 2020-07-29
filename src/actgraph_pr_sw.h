#ifndef ACTGRAPH_PR_SW_H
#define ACTGRAPH_PR_SW_H
#include <mutex>
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#include "../kernels/acts.h"
#include "../kernels/kernelprocess.h"
#include "common.h"

class actgraph_pr_sw {
public:
	actgraph_pr_sw(graph * graphobj);		
	actgraph_pr_sw();
	~actgraph_pr_sw();
	
	void run();
	runsummary_t start2(unsigned int graph_iterationidx);
	void reloadenv();
	void finish();
	runsummary_t timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms);
	unsigned long gettotalsize();
	long double totalkerneltime();
	long double totalpopulateKvDRAMtime();
	
	void WorkerThread2(int threadidx, int threadidxoffset);

	void generatekvs(int threadidx, unsigned int flag, unsigned int threadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t * edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]);		
	void workerthread_generatekvs(unsigned int ddr, unsigned int flag, int threadidx, unsigned int threadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]);				
	void settleupdates(int threadidx, unsigned int flag, unsigned int iteration_idx, unsigned int iteration_size);
	void printstructures(int threadidx, unsigned int flag);
	void checkresultfromkernel(int threadidx, unsigned int flag, unsigned int totalnumkeyvalues[NUMDRAMBANKS]);
	void loadverticesdatafromfile(int threadidx, int nvmeFd_verticesdata_r2, unsigned int offset, keyvalue_t * kvdram, vertex_t bufferoffset, vertex_t verticessize);
	void loadverticesdatafromfile(int threadidx, int nvmeFd_verticesdata_r2, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size);
	void writeverticesdatatofile(int threadidx, int nvmeFd_verticesdata_r2, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size);
	void loadvertexpropertiesfromfile(int threadidx, int nvmeFd_vertexproperties_r2, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t verticessize);
	void writevertexpropertiestofile(int threadidx, int nvmeFd_vertexproperties_w2, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t verticessize);
	void loadvertexisactiveinfosfromfile(int threadidx, int fd, unsigned int offset, unsigned int * buffer, vertex_t bufferoffset, vertex_t size);
	
	void replicateverticesdata(keyvalue_t * kvdramA,keyvalue_t * kvdramB,keyvalue_t * kvdramC,keyvalue_t * kvdramD, unsigned int offset, unsigned int size);
	void cummulateverticesdata(int threadidx, unsigned int offset, unsigned int size);	
	void workerthread_cummulateverticesdata(int ithreadidx, int threadidx, unsigned int offset, unsigned int size);
	void applyvertices(int threadidx, int bankoffset, vertex_t bufferoffset, vertex_t size);
	void workerthread_applyvertices(int ithreadidx, int threadidx, int bankoffset, vertex_t fileoffset, vertex_t bufferoffset, vertex_t size);
	void setgraphiteration(unsigned int _graph_iterationidx);
	unsigned int getflag(unsigned int giteration_idx);
	void settime_OCLdatatransfers_ms(int threadidx, long double value);
	long double gettime_OCLdatatransfers_ms(int threadidx);
	void lockmutex(std::mutex * mutex);
	void unlockmutex(std::mutex * mutex);
	
	#ifdef FPGA_IMPL
	void loadOCLstructures(std::string binaryFile);
	void writeVstokernel(int threadidx);
	void readVsfromkernel(int threadidx);
	void finishOCL();
	void reloadOCLenv(xcl_world * _world, cl_program * _program, cl_kernel * _kernel);
	void allocateOCLbuffers();
	#endif 
	
	#ifdef SW
	void topkernel(unsigned int ddr, int threadidx, uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats);
	void launchkernel(int threadidx, uint512_dt ** kvsourcedram, uint512_dt ** kvdestdram, keyvalue_t ** kvstats);
	#endif 
	#ifdef FPGA_IMPL
	void launchkernel(int threadidx, unsigned int flag);
	#endif 
	
private:
	uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	uint512_vec_dt * kvdestdram[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	keyvalue_t * kvstats[NUMCPUTHREADS][NUMFLAGS][NUMDRAMBANKS];
	
	unsigned int graph_iterationidx;	
	
	long double totaltime_ms;
	long double totaltime_topkernel_ms;
	long double totaltime_populatekvdram_ms;
	
	vector<uint512_vec_dt> unused_vectorbuffer;
	std::thread panas_thread[NUMCPUTHREADS];
	std::thread genw_thread[NUMCPUTHREADS][NUMDRAMBANKS];
	value_t * verticesdatabuffer[MAXNUMVERTEXBANKS];
	vertexprop_t * vertexpropertiesbuffer[MAXNUMVERTEXBANKS];
	unsigned int * isactivevertexinfobuffer_source[MAXNUMVERTEXBANKS];
	unsigned int * isactivevertexinfobuffer_dest[MAXNUMVERTEXBANKS];
	
	acts * kernelobjs[NUMCPUTHREADS][NUMDRAMBANKS];
	edge_process * edge_process_obj[NUMCPUTHREADS];
	utility * utilityobj[NUMCPUTHREADS];
	algorithm * algorithmobj[NUMCPUTHREADS];
	graph * graphobj;
	
	edge_t totalnumkvsread[NUMCPUTHREADS];
	long double totaltime_OCLdatatransfers[NUMCPUTHREADS];
	
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







