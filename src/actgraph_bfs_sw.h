#ifndef ACTGRAPH_BFS_SW_H
#define ACTGRAPH_BFS_SW_H
#include <mutex>
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../kernels/acts.h"
#include "../kernels/srkernelprocess.h"
#include "actgraph_pr_sw.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "actgraph_pr_sw.h"
#include "../algorithm/algorithm.h"
#include "../heuristics/heuristics.h"
#include "../utility/utility.h"
#include "common.h"

class actgraph_bfs_sw {
public:
	actgraph_bfs_sw(graph * _graphobj, heuristics * _heuristicsobj, actgraph_pr_sw * _actgraph_pr_sw_obj, std::string _binaryFile);		
	~actgraph_bfs_sw();
	
	runsummary_t run();
	runsummary_t start(unsigned int graph_iterationidx);
	void reloadactvverticesfiles();
	void reloadenv();
	void finish();
	runsummary_t timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms);
	runsummary_t overalltimingandsummary(unsigned int graph_iterationidx, runsummary_t totalsizetime);
	unsigned long gettotalsize1();
	unsigned long gettotalsize2();
	unsigned long gettotalsize();
	long double totalkerneltime();
	long double totalpopulateKvDRAMtime();
	
	void WorkerThread1(int threadidx);
	void WorkerThread2(int threadidx, int bankoffset);
	
	void generatekvs(int threadidx, unsigned int flag, unsigned int bankoffset, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]);
	void settleupdates(int threadidx, unsigned int flag, unsigned int iteration_idx, unsigned int iteration_size);
	void workerthread_generatekvs(unsigned int ddr, unsigned int flag, int threadidx, unsigned int bankoffset, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]);			
	void partitionupdates(keyvalue_t * kvdram, vector<keyvalue_t> (&vertexupdates)[NUMSSDPARTITIONS], unsigned int vbegin, unsigned int keyvaluesize, unsigned int rangeperpartition);
	void appendupdatestobuffer(vector<keyvalue_t> (&sourcebuffer)[NUMSSDPARTITIONS], vector<keyvalue_t> (&destinationbuffer)[NUMSSDPARTITIONS], FILE * destinationfile[NUMSSDPARTITIONS], bool writetodram);
	void loadupdatesfrombuffer(vector<keyvalue_t> & sourcebuffer, size_t sourceoffset, keyvalue_t * kvdram, unsigned int kvoffset, unsigned int kvsize, bool loadfromdram);
	void printstructures(int threadidx, unsigned int flag);
	
	void cummulateverticesdata(int threadidx, unsigned int offset, unsigned int size);
	void workerthread_cummulateverticesdata(int ithreadidx, int threadidx, unsigned int offset, unsigned int size);
	void applyvertices(int threadidx, int bankoffset, keyvalue_t * kvdram, vertex_t offset, vertex_t size);
	
	void setedgeprogramstatus(bool flag);
	void clearvectorbuffers();
	unsigned int getflag(unsigned int giteration_idx);
	void settime_OCLdatatransfers_ms(int threadidx, long double value);
	long double gettime_OCLdatatransfers_ms(int threadidx);
	
	#ifdef FPGA_IMPL
	void loadOCLstructures(std::string binaryFile);
	void writeVstokernel(int threadidx);
	void readVsfromkernel(int threadidx);
	void finishOCL();
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
	
	long double totaltime_topkernel1_ms;
	long double totaltime_topkernel2_ms;
	long double totaltime_populatekvdram1_ms;
	long double totaltime_populatekvdram2_ms;
	
	unsigned int graph_iterationidx;
	std::thread panas_thread[NUMCPUTHREADS];
	std::thread genw_thread[NUMCPUTHREADS][NUMDRAMBANKS];
	std::thread runacts_thread[NUMCPUTHREADS][NUMDRAMBANKS];
	vector<keyvalue_t> buffer_vertexupdates_w[NUMSSDPARTITIONS];
	vector<keyvalue_t> intermediatevertexupdates[NUMCPUTHREADS][NUMSSDPARTITIONS];
	value_t * verticesdatabuffer[MAXNUMVERTEXBANKS];
	vertexprop_t * vertexpropertiesbuffer[MAXNUMVERTEXBANKS];
	unsigned int * isactivevertexinfobuffer[MAXNUMVERTEXBANKS];
	SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader_activevertexids_r2[NUMCPUTHREADS]; 
	
	actgraph_pr_sw * actgraph_pr_sw_obj;
	edge_process * edge_process_obj[NUMCPUTHREADS];
	#ifdef SW 
	acts * kernelobjs[NUMCPUTHREADS][NUMDRAMBANKS];
	#endif 
	utility * utilityobj[NUMCPUTHREADS];
	graph * graphobj;
	heuristics * heuristicsobj;
	algorithm * algorithmobj;
	
	vector<uint512_vec_dt> unused_vectorbuffer;
	edge_t totalnumkvsread1[NUMCPUTHREADS];
	edge_t totalnumkvsread2[NUMCPUTHREADS];
	long double totaltime_OCLdatatransfers[NUMCPUTHREADS];
	bool edgeprogramisactive; // BC parameter
	std::string binaryFile;
	
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








