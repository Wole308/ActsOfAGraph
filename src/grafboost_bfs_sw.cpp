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
#include "grafboost_bfs_sw.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "actgraph_pr_sw.h"
using namespace std;
#define YES

std::mutex mutex6_srkernel;
std::mutex mutex6_workthread;

#ifdef GRAFBOOST_SETUP
inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	uint32_t ret = a;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	return vid;
}
inline uint32_t finalize_program(uint32_t oldval, uint32_t val) {
	return val;
}
inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	//printf( "Comparing %lx %lx %s\n", old, newv, marked?"Y":"N" );
	if ( old == 0xffffffff ) return true;
	//printf( "Comparing %x %x %s\n", old, newv, marked?"Y":"N" );
	return false;
}
		
grafboost_bfs_sw::grafboost_bfs_sw(graph * _graphobj){
	graphobj = _graphobj;
	
	string tmp_dir = _graphobj->gettmp_dir();
	string idx_path = _graphobj->getidx_path();
	string mat_path = _graphobj->getmat_path();
	
	int max_vertexval_thread_count = NUMCPUTHREADS;
	// EdgeProcess<uint32_t,uint32_t>* edge_process = new EdgeProcess<uint32_t,uint32_t>(idx_path.c_str(), mat_path.c_str(), &edge_program);
	size_t vertex_count = _graphobj->get_num_vertices(); //  edge_process->GetVertexCount();
	
	SortReduceTypes::Config<uint64_t,uint32_t>* conf = new SortReduceTypes::Config<uint64_t,uint32_t>(tmp_dir.c_str(), "output.dat", (NUMCPUTHREADS + 2));
	conf->SetUpdateFunction(&vertex_update);
	sr = new SortReduce<uint64_t,uint32_t>(conf);
	
	float init_val2 = 1.0/(float)vertex_count;
	vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, *(uint32_t*)&init_val2, &is_active, &finalize_program, max_vertexval_thread_count);	
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			kvdram[i][ddr] = new uint512_vec_dt[DRAMSZ_KVS];
		}
	}
	
	totaltime_topkernel_ms = 0;
	totaltime_populatekvdram_ms = 0;
	
	numvertexbanks = graphobj->getnumvertexbanks();
	numedgebanks = graphobj->getnumedgebanks();
	numverticespervertexbank = KVDATA_RANGE / graphobj->getnumvertexbanks();
	
	size_t filelenght = lseek(graphobj->getnvmeFd_activevertexids_r2(), 0, SEEK_END) / sizeof(keyvalue_t);
	for(unsigned int i=0; i<NUMCPUTHREADS; i++) {
		size_t beginoffset =  i * (filelenght / NUMCPUTHREADS); 
		size_t endoffset = (size_t)beginoffset + (size_t)(filelenght / NUMCPUTHREADS);
		reader_activevertexids_r2[i] = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(graphobj->getnvmeFd_activevertexids_r2(), (beginoffset * sizeof(keyvalue_t)), (endoffset * sizeof(keyvalue_t))); }		
	actgraph_pr_sw_obj = new actgraph_pr_sw();
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edge_process_obj[i] = new edge_process(graphobj); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	srkernel_process = new srkernelprocess();
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totalnumkvsread[i] = 0; }
}
grafboost_bfs_sw::~grafboost_bfs_sw() {
	cout<<"grafboost_bfs_sw::~grafboost_bfs_sw::finish destroying memory structures... "<<endl;
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ delete [] kvdram[i][ddr]; }
	}
} 

unsigned int grafboost_bfs_sw::run(){
	for(graph_iterationidx=1; graph_iterationidx<64; graph_iterationidx++){
		cout<<"grafboost_bfs_sw::run: graph iteration "<<graph_iterationidx<<" of pagerank Started"<<endl;
		
		graphobj->openactiveverticesfilesforreading(graph_iterationidx);
		unsigned int numactivevertices = graphobj->getnumactivevertices();
		cout<<"=== grafboost_bfs_sw::run: number of active vertices for graph iteration "<<graph_iterationidx<<": "<<numactivevertices<<" ==="<<endl;
		if(numactivevertices == 0){ cout<<"grafboost_bfs_sw::run: no more active vertices to process. exiting..."<<endl; break; }
		reloadactvverticesfiles(); // important
		reloadsrhandlers(); // important
		
		start();
		summary();
		
		#if not (defined(LAUNCHKERNEL) || defined(LOADKVDRAMS))
		break;
		#endif		
		std::chrono::steady_clock::time_point begintime_kernel = std::chrono::steady_clock::now();
		sr->Finish();
		SortReduceTypes::Status status = sr->CheckStatus();
		while ( status.done_external == false ) {
			sleep(1);
			status = sr->CheckStatus();
			printf( "%s %s:%d-%d %s:%d-%d\n", status.done_input?"yes":"no", status.done_inmem?"yes":"no",status.internal_count, status.sorted_count, status.done_external?"yes":"no", status.external_count, status.file_count );		
			fflush(stdout);
		}
		
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		std::tuple<uint32_t,uint32_t,bool> res2 = sr->Next();
		vertex_values->Start();
		while ( std::get<2>(res2) ) {
			uint32_t key = std::get<0>(res2);
			uint32_t val = std::get<1>(res2);

			// printf( "\t\t++ SRR %i %i\n", key, val );
			while ( !vertex_values->Update(key,val) ) ;

			res2 = sr->Next();
		}
		vertex_values->Finish();
		
		size_t active_cnt = vertex_values->GetActiveCount();
		cout<<">>> Host::Sort-Reduce & Apply phase:: Active Vertices after iteration "<<graph_iterationidx<<": "<<active_cnt<<endl;
		vertex_values->NextIteration();		
		delete sr;
		float totaltime_kernel_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_kernel).count();
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL KERNEL TIME: "<<totaltime_kernel_ms + totalkerneltime()<<" milli seconds"<< RESET <<endl;
		cout<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL KERNEL TIME: "<<((totaltime_kernel_ms + totalkerneltime()) / 1000)<<" seconds"<< RESET <<endl;
		cout<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL POPULATE KVDRAM TIME: "<<totalpopulateKvDRAMtime()<<" milli seconds"<< RESET <<endl;
		cout<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL POPULATE KVDRAM TIME: "<<(totalpopulateKvDRAMtime() / 1000)<<" seconds"<< RESET <<endl<<endl;
		
		graphobj->closeactiveverticesfilesforreading();
		if ( active_cnt == 0 ) { cout<<"Host: No more active vertices to process. Exiting..."<<endl; break; }		
	}
	return graph_iterationidx;
}
void grafboost_bfs_sw::start() {
	cout<<">>> grafboost_bfs_sw::start: launching threads..."<<endl;
	for (int i = 0; i < NUMCPUTHREADS; i++) { panas_thread[i] = std::thread(&grafboost_bfs_sw::WorkerThread, this, i); }
	for ( int i = 0; i < NUMCPUTHREADS; i++ ) { panas_thread[i].join(); }
	cout<<">>> grafboost_bfs_sw:: Finished: all threads joined..."<<endl;
	finish();
}
void grafboost_bfs_sw::reloadactvverticesfiles(){
	size_t filelenght = lseek(graphobj->getnvmeFd_activevertexids_r2(), 0, SEEK_END) / sizeof(keyvalue_t);
	for(unsigned int i=0; i<NUMCPUTHREADS; i++) {
		size_t beginoffset =  i * (filelenght / NUMCPUTHREADS); 
		size_t endoffset = (size_t)beginoffset + (size_t)(filelenght / NUMCPUTHREADS);
		reader_activevertexids_r2[i] = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(graphobj->getnvmeFd_activevertexids_r2(), (beginoffset * sizeof(keyvalue_t)), (endoffset * sizeof(keyvalue_t))); }
	return;
}
void grafboost_bfs_sw::reloadsrhandlers(){
	string tmp_dir = graphobj->gettmp_dir();
	string idx_path = graphobj->getidx_path();
	string mat_path = graphobj->getmat_path();
	
	int max_vertexval_thread_count = NUMCPUTHREADS;
	size_t vertex_count = graphobj->get_num_vertices();
	
	SortReduceTypes::Config<uint64_t,uint32_t>* conf = new SortReduceTypes::Config<uint64_t,uint32_t>(tmp_dir.c_str(), "output.dat", (NUMCPUTHREADS + 2));
	conf->SetUpdateFunction(&vertex_update);
	sr = new SortReduce<uint64_t,uint32_t>(conf);
	
	float init_val2 = 1.0/(float)vertex_count;
	vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, *(uint32_t*)&init_val2, &is_active, &finalize_program, max_vertexval_thread_count);	
	return;
}
void grafboost_bfs_sw::finish(){}
void grafboost_bfs_sw::summary(){
	edge_t _totalnumkvsread = 0;
	for ( int i = 0; i < NUMCPUTHREADS; i++ ) {
		cout<<">>> grafboost_bfs_sw::summary Total number of kvReads for thread ["<<i<<"]: "<<totalnumkvsread[i]<<endl;
		_totalnumkvsread += totalnumkvsread[i];
	}
	cout<<">>> grafboost_bfs_sw::summary Total number of kvReads for all threads: "<<_totalnumkvsread<<endl;
	#ifdef LOCKE
	cout<<">>> grafboost_bfs_sw::summary Total kernel time: "<<totaltime_topkernel_ms<<" milli seconds"<<endl;
	cout<<">>> grafboost_bfs_sw::summary Total kernel time: "<<(totaltime_topkernel_ms / 1000)<<" seconds"<<endl;	
	cout<<">>> grafboost_bfs_sw::summary Total populate KvDRAM time: "<<totaltime_populatekvdram_ms<<" milli seconds"<<endl;
	cout<<">>> grafboost_bfs_sw::summary Total populate KvDRAM time: "<<(totaltime_populatekvdram_ms / 1000)<<" seconds"<<endl;	
	#endif 
	return;
}
float grafboost_bfs_sw::totalkerneltime(){ return totaltime_topkernel_ms; }
float grafboost_bfs_sw::totalpopulateKvDRAMtime(){ return totaltime_populatekvdram_ms; }

void grafboost_bfs_sw::WorkerThread(int threadidx){
	#ifdef LOCKE
	mutex6_workthread.lock();
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_grafboost_bfs_sw = std::chrono::steady_clock::now();
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"Host:: PP&A: [threadidx: "<<threadidx<<"][size: "<<NUMCPUTHREADS<<"][step: 1]"<<endl;
	#endif

	unsigned int iteration_size = 256; // FIXME
	unsigned int status = NOTFINISHED;
	for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx++){
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"PP&A:: [graph_iterationidx:"<<graph_iterationidx<<"], [threadidx:"<<threadidx<<"][size:"<<NUMCPUTHREADS<<"][step:1], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
		#endif 
		#ifdef _DEBUGMODE_TIMERS
		std::chrono::steady_clock::time_point begintime_iterationidx = std::chrono::steady_clock::now();
		#endif
		
		// Populate kvdrams
		#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
		std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
		#endif
		
		unsigned int kvsize[NUMDRAMBANKS];
		#ifdef BC_ALGORITHM
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			if(edgeprogramisactive == true){ kvsize[ddr] = edge_process_obj[threadidx]->generateupdates_random(reader_activevertexids_r2[threadidx], graphobj->getnvmeFd_edgeoffsets_r2()[0][0], graphobj->getnvmeFd_edgeproperties_r2()[0][0], kvdram[threadidx][ddr], 0, &status); }				
			else { kvsize[ddr] = edge_process_obj[threadidx]->generateupdates2_random(reader_activevertexids_r2[threadidx], (keyvalue_t *)kvdram[threadidx][ddr], 0, &status); }
			totalnumkvsread[threadidx] += kvsize[ddr];
		}
		#else
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			kvsize[ddr] = edge_process_obj[threadidx]->generateupdates_random(reader_activevertexids_r2[threadidx], graphobj->getnvmeFd_edgeoffsets_r2()[0][0], graphobj->getnvmeFd_edgeproperties_r2()[0][0], kvdram[threadidx][ddr], 0, &status);		
			totalnumkvsread[threadidx] += kvsize[ddr];
		}
		#endif
		
		#ifdef _DEBUGMODE_TIMERS
		utilityobj[threadidx]->stopTIME("PROCESS, SORT-REDUCE AND APPLY PHASE: Populate Kv DRAM Time Elapsed: ", begintime_populatekvdram, iteration_idx);
		#endif
		#ifdef LOCKE
		totaltime_populatekvdram_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_populatekvdram).count();
		#endif
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"Host::Process, Sort-reduce and Apply phase:: Print results before Kernel run "<<endl;
		printstructures(threadidx);
		#endif
		
		// Launch the Kernel
		#ifdef LAUNCHKERNEL
		#ifdef LOCKE
		std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
		#endif
		mutex6_srkernel.lock(); // lock
		srkernel_process->srtopkernel(
			sr
			,(keyvalue_t *)kvdram[threadidx][0]
			
			,kvsize[0]
);
		mutex6_srkernel.unlock(); // unlock
		#ifdef LOCKE
		totaltime_topkernel_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
		#endif
		#endif 
		
		// Print results after Kernel run
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"Host::Process, Sort-reduce and Apply phase:: Print results after Kernel run "<<endl;
		printstructures(threadidx);
		#endif
	
		#ifdef _DEBUGMODE_TIMERS
		actgraph_pr_sw_obj->stopTIME("PROCESS, SORT-REDUCE AND APPLY PHASE: Iteration Time Elapsed: ", begintime_iterationidx, iteration_idx);
		#endif
		if(status == FINISHED){ cout<<"actgraph_bfs_sw:: no more active vertices to process. exiting."<<endl; break; }
	}
	#ifdef _DEBUGMODE_TIMERS
	actgraph_pr_sw_obj->stopTIME("Total time elapsed (Entire Process, Sort-reduce & Apply process): ", begintime_grafboost_bfs_sw, NAp);
	#endif
	#ifdef LOCKE
	mutex6_workthread.unlock();
	#endif 
}

SortReduce<uint64_t,uint32_t>* grafboost_bfs_sw::get_sr(){
	return sr;
}
VertexValues<uint32_t,uint32_t>* grafboost_bfs_sw::get_vertexvalues(){
	return vertex_values;
}
void grafboost_bfs_sw::printstructures(unsigned int threadidx){
	for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printkvdrams(kvdram[threadidx][ddr]); }
	return;
}
void grafboost_bfs_sw::setedgeprogramstatus(bool flag){ edgeprogramisactive = flag; }

#endif 
