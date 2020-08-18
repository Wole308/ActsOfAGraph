#ifndef GRAFBOOST_PR_SW_H
#define GRAFBOOST_PR_SW_H
#include <mutex>
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../kernels/srkernelprocess.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "actgraph_pr_sw.h"
#include "common.h"

class grafboost_pr_sw {
public:
	grafboost_pr_sw(graph * _graphobj);		
	~grafboost_pr_sw();
	
	void run();
	void start();
	void finish();
	void summary();
	float totalkerneltime();
	float totalpopulateKvDRAMtime();
	
	void WorkerThread(int threadidx, edge_t ledgeoffset, edge_t ledgeize);
	
	size_t hsub(size_t val1, size_t val2);
	size_t hceildiv(size_t val1, size_t val2);
	int hmin(size_t val1, size_t val2);
	void stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);

	void printkvdrams(uint512_vec_dt * kvdram);
	void printstructures(unsigned int threadidx);
	unsigned int loadedgesfromfile(int nvmeFd_edges_r, size_t fileoffset, edgeprop2_t * edgesbuffer, edge_t edgesoffset, edge_t edgessize);

private:
	uint512_vec_dt * kvdramA[NUMCPUTHREADS];
	keyvalue_t * kvstatsA[NUMCPUTHREADS];
	uint512_vec_dt * kvdramB[NUMCPUTHREADS];
	keyvalue_t * kvstatsB[NUMCPUTHREADS];
	uint512_vec_dt * kvdramC[NUMCPUTHREADS];
	keyvalue_t * kvstatsC[NUMCPUTHREADS];
	uint512_vec_dt * kvdramD[NUMCPUTHREADS];
	keyvalue_t * kvstatsD[NUMCPUTHREADS];
	
	int nvmeFd_vertexoffsets_r2[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	int nvmeFd_vertexproperties_r2[MAXNUMEDGEBANKS];
	int nvmeFd_edgeproperties_r2[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	
	vector<uint512_vec_dt> unused_vectorbuffer;
	edge_t totalnumkvsread[NUMCPUTHREADS];
	std::thread panas_thread[NUMCPUTHREADS];
	
	actgraph_pr_sw * actgraph_pr_sw_obj;
	edge_process * edge_process_obj[NUMCPUTHREADS];
	srkernelprocess * srkernel_process;
	utility * utilityobj[NUMCPUTHREADS];
	graph * graphobj;
	SortReduce<uint64_t,uint32_t>* sr;
	VertexValues<uint32_t,uint32_t>* vertex_values;
	
	float totaltime_topkernel_ms;
	float totaltime_populatekvdram_ms;

	vertexprop_t * vertexpropertiesbuffer[MAXNUMVERTEXBANKS];
	edgeprop2_t * edgesbuffer[NUMCPUTHREADS][NUMDRAMBANKS];
	prvertexoffset_t * vertexoffsetsbuffer[NUMCPUTHREADS][NUMDRAMBANKS];
	
	unsigned int numvertexbanks;
	unsigned int numedgebanks;
	unsigned int numverticespervertexbank;
	unsigned int graph_iterationidx;
};
#endif








