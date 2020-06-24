#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "host_common.h"
#include "common.h"
#include "../debugger/host_debugger.h"
#include "EdgeProcess.h"
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../kernels/kernelprocess.h"
#include "sortreduce.h"
#include "types.h"
#include "../kernels/srkernelprocess.h"
#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"
#include "EdgeProcess.h"
#include "VertexValues.h"
#include "actgraph_bc_sw.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../heuristics/heuristics.h"
#include "../utility/utility.h"
#include "actgraph_bfs_sw.h"
#include "actgraph_bc_sw.h"
using namespace std;
	
actgraph_bc_sw::actgraph_bc_sw(graph * _graphobj, heuristics * _heuristicsobj, actgraph_pr_sw * _actgraph_pr_sw_obj, std::string _binaryFile){
	graphobj = _graphobj;
	heuristicsobj = _heuristicsobj;
	actgraph_pr_sw_obj = _actgraph_pr_sw_obj;
	binaryFile = _binaryFile;
	actgraph_bfs_sw_obj = new actgraph_bfs_sw(_graphobj, _heuristicsobj, _actgraph_pr_sw_obj, binaryFile);
	forward_graph_iterationidx = 0;
}
actgraph_bc_sw::~actgraph_bc_sw() {} 

void actgraph_bc_sw::forwardrun(){
	actgraph_bfs_sw_obj->setedgeprogramstatus(true);
	forward_graph_iterationidx = actgraph_bfs_sw_obj->run();
	return;
}
void actgraph_bc_sw::reverserun(){
	
	actgraph_bfs_sw_obj->setedgeprogramstatus(false);
	for(int reverse_graph_iteration = forward_graph_iterationidx-1; reverse_graph_iteration>=1; reverse_graph_iteration--){
		cout<<"actgraph_bc_sw::run: reverse graph iteration "<<reverse_graph_iteration<<" of pagerank Started"<<endl;
		
		graphobj->openactiveverticesfilesforreading(reverse_graph_iteration);
		graphobj->openactiveverticesfilesforwriting((reverse_graph_iteration - 1));
		unsigned int numactivevertices = graphobj->getnumactivevertices();
		cout<<"=== actgraph_bc_sw::run: number of active vertices for reverse graph iteration "<<reverse_graph_iteration<<": "<<numactivevertices<<" ==="<<endl;
		if(numactivevertices == 0){ cout<<"actgraph_bc_sw::run: no more active vertices to process. exiting..."<<endl; break; }
		actgraph_bfs_sw_obj->reloadactvverticesfiles(); // important
		
		cout<<"=== PARTIAL_ACCESS_TYPE ==="<<endl;
		actgraph_bfs_sw_obj->start();
		
		graphobj->closeactiveverticesfilesforreading();
		graphobj->closeactiveverticesfilesforwriting();
		actgraph_bfs_sw_obj->clearvectorbuffers();
		summary();
	}
	// actgraph_bfs_sw_obj->finish();
	return;
}
void actgraph_bc_sw::finish(){
	actgraph_bfs_sw_obj->finish();
}
void actgraph_bc_sw::summary(){}
float actgraph_bc_sw::totalkerneltime(){ return NAp; }
float actgraph_bc_sw::totalpopulateKvDRAMtime(){ return NAp; }

#ifdef FPGA_IMPL
void actgraph_bc_sw::loadOCLstructures(std::string binaryFile){
	actgraph_bfs_sw_obj->loadOCLstructures(binaryFile);
}
#endif 



