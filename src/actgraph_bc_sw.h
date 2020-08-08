#ifndef ACTGRAPH_BC_SW_H
#define ACTGRAPH_BC_SW_H
#include <mutex>
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h"
#include "../kernels/srkernelprocess.h"
#include "actgraph_bfs_sw.h"
#include "actgraph_pr_sw.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../heuristics/heuristics.h"
#include "../utility/utility.h"
#include "common.h"

class actgraph_bc_sw {
public:
	actgraph_bc_sw(graph * _graphobj, heuristics * _heuristicsobj, actgraph_pr_sw * _actgraph_pr_sw_obj, std::string _binaryFile);
	~actgraph_bc_sw();
	
	void forwardrun();
	void reverserun();
	void finish();
	void summary();
	runsummary_t overalltimingandsummary(unsigned int graph_iterationidx, runsummary_t totalrunsummary);
	
	#ifdef FPGA_IMPL
	void loadOCLstructures(std::string binaryFile);
	#endif 
	
private:	
	actgraph_pr_sw * actgraph_pr_sw_obj;
	actgraph_bfs_sw * actgraph_bfs_sw_obj;
	graph * graphobj;
	heuristics * heuristicsobj;
	unsigned int forward_graph_iterationidx;
	runsummary_t totalrunsummary; 
	utility * utilityobj;
	
	std::string binaryFile;
};
#endif








