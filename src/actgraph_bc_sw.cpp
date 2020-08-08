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
	
	utilityobj = new utility();
	utilityobj->reset(&totalrunsummary);
}
actgraph_bc_sw::~actgraph_bc_sw() {} 

void actgraph_bc_sw::forwardrun(){
	actgraph_bfs_sw_obj->setedgeprogramstatus(true);
	// forward_graph_iterationidx = actgraph_bfs_sw_obj->run();
	runsummary_t runsummary = actgraph_bfs_sw_obj->run();
	forward_graph_iterationidx = runsummary.graph_iterationidx;
	utilityobj->append(&totalrunsummary, runsummary);
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
		runsummary_t runsummary = actgraph_bfs_sw_obj->start(forward_graph_iterationidx);
		utilityobj->append(&totalrunsummary, runsummary);
		
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
	overalltimingandsummary(NAp, totalrunsummary);
}
void actgraph_bc_sw::summary(){}
runsummary_t actgraph_bc_sw::overalltimingandsummary(unsigned int graph_iterationidx, runsummary_t totalrunsummary){
	cout<<"=== ACTGRAPH_BC::TOTAL TIMING AND SUMMARY RESULTS FOR ALL ITERATIONS === "<<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total number of kvReads for all threads: "<<totalrunsummary.totalsize<< RESET <<endl;
	
	cout << TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total time spent (SSD access): "<< totalrunsummary.totaltime_SSDtransfers_ms << " milliseconds" << RESET <<endl;
	cout << TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total time spent (SSD access): "<< totalrunsummary.totaltime_SSDtransfers_ms / 1000 << " seconds" << RESET <<endl;
	cout << TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total time spent (OCL data transfers): "<< totalrunsummary.totaltime_OCLtransfers_ms << " milliseconds" << RESET <<endl;
	cout << TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total time spent (OCL data transfers): "<< totalrunsummary.totaltime_OCLtransfers_ms / 1000 << " seconds" << RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total time spent (Overall processing): "<<totalrunsummary.totaltime_ms<< " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total time spent (Overall processing): "<<totalrunsummary.totaltime_ms / 1000<< " seconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total time spent (Overall processing excluding SSD & OCL data transfers): "<<totalrunsummary.totaltime_overallexcludingOCLandSSDtransfers_ms<< " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Total time spent (Overall processing excluding SSD & OCL data transfers): "<<totalrunsummary.totaltime_overallexcludingOCLandSSDtransfers_ms / 1000<< " seconds" << RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Throughput (Overall processing): "<<(unsigned long)totalrunsummary.totalsize / (long double)(totalrunsummary.totaltime_ms / 1000)<<" keyvalues per second"<< RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_bc_sw::summary Throughput (Overall processing excluding SSD & OCL data transfers): "<<(unsigned long)totalrunsummary.totalsize / (long double)(totalrunsummary.totaltime_overallexcludingOCLandSSDtransfers_ms / 1000)<<" keyvalues per second"<< RESET <<endl;
	cout<<endl;
	
	totalrunsummary.graph_iterationidx = graph_iterationidx;
	return totalrunsummary;
}

#ifdef FPGA_IMPL
void actgraph_bc_sw::loadOCLstructures(std::string binaryFile){
	actgraph_bfs_sw_obj->loadOCLstructures(binaryFile);
}
#endif 



