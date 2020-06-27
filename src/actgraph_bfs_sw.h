#ifndef ACTGRAPH_BFS_SW_H
#define ACTGRAPH_BFS_SW_H
#include <mutex>
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../kernels/kernelprocess.h"
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
	
	unsigned int run();
	void start();
	void reloadactvverticesfiles();
	void reloadenv();
	void finish();
	void summary();
	float totalkerneltime();
	float totalpopulateKvDRAMtime();
	
	void WorkerThread1(int threadidx);
	void WorkerThread2(int threadidx, int bankoffset);
	
	void generatekvs(int threadidx, unsigned int bankoffset, unsigned int iteration_idx, unsigned int * kvcount);
	void workerthread_generatekvs(unsigned int ddr, int threadidx, unsigned int bankoffset, unsigned int iteration_idx, unsigned int * kvcount);
	void partitionupdates(keyvalue_t * kvdram, vector<keyvalue_t> (&vertexupdates)[NUMSSDPARTITIONS], unsigned int vbegin, unsigned int keyvaluesize, unsigned int rangeperpartition);
	void appendupdatestobuffer(vector<keyvalue_t> (&sourcebuffer)[NUMSSDPARTITIONS], vector<keyvalue_t> (&destinationbuffer)[NUMSSDPARTITIONS], FILE * destinationfile[NUMSSDPARTITIONS], bool writetodram);
	void loadupdatesfrombuffer(vector<keyvalue_t> & sourcebuffer, size_t sourceoffset, keyvalue_t * kvdram, unsigned int kvoffset, unsigned int kvsize, bool loadfromdram);
	void printstructures(unsigned int threadidx);
	void applyvertices(unsigned int threadidx, int bankoffset, keyvalue_t * kvdram, vertex_t offset, vertex_t size);
	void setedgeprogramstatus(bool flag);
	void clearvectorbuffers();
	
	#ifdef FPGA_IMPL
	void loadOCLstructures(std::string binaryFile);
	void launchkernel(unsigned int flag);
	void finishOCL();
	unsigned int getflag(unsigned int giteration_idx);
	#endif 
	
private:
	uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMDRAMBANKS];
	uint512_vec_dt * kvdestdram[NUMCPUTHREADS][NUMDRAMBANKS];
	keyvalue_t * kvstats[NUMCPUTHREADS][NUMDRAMBANKS];
	
	float totaltime_topkernel1_ms;
	float totaltime_topkernel2_ms;
	float totaltime_populatekvdram1_ms;
	float totaltime_populatekvdram2_ms;
	
	unsigned int graph_iterationidx;
	std::thread panas_thread[NUMCPUTHREADS];
	std::thread genw_thread[NUMCPUTHREADS];
	vector<keyvalue_t> buffer_vertexupdates_w[NUMSSDPARTITIONS];
	vector<keyvalue_t> intermediatevertexupdates[NUMCPUTHREADS][NUMSSDPARTITIONS];
	value_t * verticesdatabuffer[MAXNUMVERTEXBANKS];
	vertexprop_t * vertexpropertiesbuffer[MAXNUMVERTEXBANKS];
	unsigned int * isactivevertexinfobuffer[MAXNUMVERTEXBANKS];
	SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader_activevertexids_r2[NUMCPUTHREADS]; 
	
	actgraph_pr_sw * actgraph_pr_sw_obj;
	edge_process * edge_process_obj[NUMCPUTHREADS];
	kernelprocess * kernel_process[NUMCPUTHREADS];
	utility * utilityobj[NUMCPUTHREADS];
	graph * graphobj;
	heuristics * heuristicsobj;
	algorithm * algorithmobj;
	
	vector<uint512_vec_dt> unused_vectorbuffer;
	edge_t totalnumkvsread1[NUMCPUTHREADS];
	edge_t totalnumkvsread2[NUMCPUTHREADS];
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
	
	cl_mem buffer_kvsourcedram[2][NUMDRAMBANKS];
	cl_mem buffer_kvdestdram[2][NUMDRAMBANKS];
	cl_mem buffer_kvstatsdram[2][NUMDRAMBANKS];
	#endif
};
#endif








