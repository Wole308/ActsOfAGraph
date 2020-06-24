#ifndef GRAFBOOST_BFS_SW_H
#define GRAFBOOST_BFS_SW_H
#include <mutex>
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../kernels/srkernelprocess.h"
#include "actgraph_pr_sw.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "common.h"

class grafboost_bfs_sw {
public:
	grafboost_bfs_sw(graph * _graphobj);	
	void relaunch(graph * _graphobj);
	~grafboost_bfs_sw();
	
	unsigned int run();
	void start();
	void reloadactvverticesfiles();
	void reloadsrhandlers();
	void finish();
	void summary();
	float totalkerneltime();
	float totalpopulateKvDRAMtime();
	
	void WorkerThread(int threadidx);
	
	SortReduce<uint64_t,uint32_t>* get_sr();
	VertexValues<uint32_t,uint32_t>* get_vertexvalues();
	void printstructures(unsigned int threadidx);
	void setedgeprogramstatus(bool flag);
	
private:	
	uint512_vec_dt * kvdram[NUMCPUTHREADS][NUMDRAMBANKS];
	
	float totaltime_topkernel_ms;
	float totaltime_populatekvdram_ms;
	
	unsigned int graph_iterationidx;
	std::thread panas_thread[NUMCPUTHREADS];
	SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader_activevertexids_r2[MAXNUMVERTEXBANKS]; 
	
	actgraph_pr_sw * actgraph_pr_sw_obj;
	edge_process * edge_process_obj[NUMCPUTHREADS];
	srkernelprocess * srkernel_process;
	utility * utilityobj[NUMCPUTHREADS];
	graph * graphobj;
	SortReduce<uint64_t,uint32_t>* sr;
	VertexValues<uint32_t,uint32_t>* vertex_values;
	
	vector<uint512_vec_dt> unused_vectorbuffer;
	edge_t totalnumkvsread[NUMCPUTHREADS];
	bool edgeprogramisactive;
	
	unsigned int numvertexbanks;
	unsigned int numedgebanks;
	unsigned int numverticespervertexbank;
};
#endif








