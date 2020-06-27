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
#include "../kernels/enigma.h"
#include "sortreduce.h"
#include "types.h"
#include "../kernels/srkernelprocess.h"
#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"
#include "EdgeProcess.h"
#include "VertexValues.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../heuristics/heuristics.h"
#include "../utility/utility.h"
#include "grafboost_bfs_sw.h"
#include "grafboost_bc_sw.h"
using namespace std;
	
#ifdef GRAFBOOST_SETUP
grafboost_bc_sw::grafboost_bc_sw(graph * _graphobj){			
	graphobj = _graphobj;
	grafboost_bfs_sw_obj = new grafboost_bfs_sw(_graphobj);
	forward_graph_iterationidx = 0;
}
grafboost_bc_sw::~grafboost_bc_sw() {} 

void grafboost_bc_sw::forwardrun(){
	grafboost_bfs_sw_obj->setedgeprogramstatus(true);
	forward_graph_iterationidx = grafboost_bfs_sw_obj->run();
	return;
}
void grafboost_bc_sw::reverserun(){
	
	grafboost_bfs_sw_obj->setedgeprogramstatus(false);
	for(int reverse_graph_iteration = forward_graph_iterationidx-1; reverse_graph_iteration>0; reverse_graph_iteration--){ // FIXME >=
		cout<<"grafboost_bc_sw::reverserun: reverse graph iteration "<<reverse_graph_iteration<<" of betweenness-centrality Started"<<endl;
		
		graphobj->openactiveverticesfilesforreading(reverse_graph_iteration);
		unsigned int numactivevertices = graphobj->getnumactivevertices();
		cout<<"=== grafboost_bc_sw::run: number of active vertices for graph iteration "<<reverse_graph_iteration<<": "<<numactivevertices<<" ==="<<endl;
		if(numactivevertices == 0){ cout<<"grafboost_bc_sw::run: no more active vertices to process. exiting..."<<endl; break; }
		grafboost_bfs_sw_obj->reloadactvverticesfiles(); // important
		grafboost_bfs_sw_obj->reloadsrhandlers(); // important
		
		grafboost_bfs_sw_obj->start();
		summary();
		
		#if not (defined(LAUNCHKERNEL) || defined(LOADKVDRAMS))
		break;
		#endif		
		std::chrono::steady_clock::time_point begintime_kernel = std::chrono::steady_clock::now();
		grafboost_bfs_sw_obj->get_sr()->Finish();
		SortReduceTypes::Status status = grafboost_bfs_sw_obj->get_sr()->CheckStatus();
		while ( status.done_external == false ) {
			sleep(1);
			status = grafboost_bfs_sw_obj->get_sr()->CheckStatus();
			printf( "%s %s:%d-%d %s:%d-%d\n", status.done_input?"yes":"no", status.done_inmem?"yes":"no",status.internal_count, status.sorted_count, status.done_external?"yes":"no", status.external_count, status.file_count );		
			fflush(stdout);
		}
		
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		std::tuple<uint32_t,uint32_t,bool> res2 = grafboost_bfs_sw_obj->get_sr()->Next();
		grafboost_bfs_sw_obj->get_vertexvalues()->Start();
		while ( std::get<2>(res2) ) {
			uint32_t key = std::get<0>(res2);
			uint32_t val = std::get<1>(res2);

			// printf( "\t\t++ SRR %i %i\n", key, val );
			while ( !grafboost_bfs_sw_obj->get_vertexvalues()->Update(key,val) ) ;

			res2 = grafboost_bfs_sw_obj->get_sr()->Next();
		}
		grafboost_bfs_sw_obj->get_vertexvalues()->Finish();
		
		size_t active_cnt = grafboost_bfs_sw_obj->get_vertexvalues()->GetActiveCount();
		cout<<">>> Host::Sort-Reduce & Apply phase:: Active Vertices after iteration "<<reverse_graph_iteration<<": "<<active_cnt<<endl;
		grafboost_bfs_sw_obj->get_vertexvalues()->NextIteration();		
		delete grafboost_bfs_sw_obj->get_sr();
		float totaltime_kernel_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_kernel).count();
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL KERNEL TIME: "<<totaltime_kernel_ms + totalkerneltime()<<" milli seconds"<< RESET <<endl;
		cout<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL KERNEL TIME: "<<((totaltime_kernel_ms + totalkerneltime()) / 1000)<<" seconds"<< RESET <<endl;
		cout<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL POPULATE KVDRAM TIME: "<<totalpopulateKvDRAMtime()<<" milli seconds"<< RESET <<endl;
		cout<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL POPULATE KVDRAM TIME: "<<(totalpopulateKvDRAMtime() / 1000)<<" seconds"<< RESET <<endl<<endl;
		
		if ( active_cnt == 0 ) { cout<<"Host: No more active vertices to process. Exiting..."<<endl; break; }
	}
	return;
}
void grafboost_bc_sw::finish(){}
void grafboost_bc_sw::summary(){}
float grafboost_bc_sw::totalkerneltime(){ return NAp; }
float grafboost_bc_sw::totalpopulateKvDRAMtime(){ return NAp; }

#endif 


