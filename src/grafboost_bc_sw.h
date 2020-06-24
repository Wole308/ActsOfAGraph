#ifndef GRAFBOOST_BC_SW_H
#define GRAFBOOST_BC_SW_H
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../kernels/srkernelprocess.h"
#include "grafboost_bfs_sw.h"
#include "actgraph_pr_sw.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../heuristics/heuristics.h"
#include "../utility/utility.h"
#include "common.h"

class grafboost_bc_sw {
public:
	grafboost_bc_sw(graph * _graphobj);		
	~grafboost_bc_sw();
	
	void forwardrun();
	void reverserun();
	void start();
	void finish();
	void summary();
	float totalkerneltime();
	float totalpopulateKvDRAMtime();

private:
	grafboost_bfs_sw * grafboost_bfs_sw_obj;
	graph * graphobj;
	heuristics * heuristicsobj;
	unsigned int forward_graph_iterationidx;
	unsigned int reverse_graph_iteration;
};
#endif








