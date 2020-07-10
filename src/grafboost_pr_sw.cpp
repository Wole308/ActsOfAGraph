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
#include "grafboost_pr_sw.h"
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
#include "../utility/utility.h"
#include "actgraph_pr_sw.h"
using namespace std;
#define YES
#define NUM_THREADS

std::mutex mutex5_srkernel;
std::mutex mutex5_workthread;
std::mutex mutex5_populatekvdrams;

#ifdef GRAFBOOST_SETUP
inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	float af = *(float*)&a;
	float bf = *(float*)&b;
	float tf = af+bf;
	
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	float af = *(float*)&value;
	float tf = af/(float)fanout;

	uint32_t ret = *(uint32_t*)&tf;

	return ret;
}
size_t g_vertex_count = 1;
inline uint32_t finalize_program(uint32_t oldval, uint32_t val) {
	float damp = 0.15/(float)g_vertex_count;
	float af = *(float*)&val;
	float tf = damp + af*0.85;
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	if ( !marked ) return false;

	float af = *(float*)&old;
	float bf = *(float*)&newv;

	if ( af > bf ) {
		if (af-bf < 0.0001 ) return false;
		else return true;
	} else {
		if (bf-af < 0.0001 ) return false;
		else return true;
	}
}
	
grafboost_pr_sw::grafboost_pr_sw(graph * _graphobj){	
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
		kvdramA[i] = new uint512_vec_dt[KVSOURCEDRAMSZ_KVS];
		kvdramB[i] = new uint512_vec_dt[KVSOURCEDRAMSZ_KVS];
		kvdramC[i] = new uint512_vec_dt[KVSOURCEDRAMSZ_KVS];
		kvdramD[i] = new uint512_vec_dt[KVSOURCEDRAMSZ_KVS];
		kvstatsA[i] = new metadata_t[1]; // NAp
		kvstatsB[i] = new metadata_t[1]; // NAp
		kvstatsC[i] = new metadata_t[1]; // NAp
		kvstatsD[i] = new metadata_t[1]; // NAp
	}
	
	numvertexbanks = _graphobj->getnumvertexbanks();
	numedgebanks = _graphobj->getnumedgebanks();
	numverticespervertexbank = KVDATA_RANGE / _graphobj->getnumvertexbanks();
	
	for(unsigned int i=0; i<numedgebanks; i++) { for(unsigned int j=0; j<numedgebanks; j++) { nvmeFd_vertexoffsets_r2[i][j] = _graphobj->getnvmeFd_edgeoffsets_r2()[i][j]; }}
	for(unsigned int i=0; i<numedgebanks; i++) { nvmeFd_vertexproperties_r2[i] = _graphobj->getnvmeFd_vertexproperties_r2()[i]; }
	for(unsigned int i=0; i<numedgebanks; i++) { for(unsigned int j=0; j<numedgebanks; j++) { nvmeFd_edgeproperties_r2[i][j] = _graphobj->getnvmeFd_edgeproperties_r2()[i][j]; }}
	
	srkernel_process = new srkernelprocess();
	actgraph_pr_sw_obj = new actgraph_pr_sw();
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edge_process_obj[i] = new edge_process(_graphobj); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	
	totaltime_topkernel_ms = 0;
	totaltime_populatekvdram_ms = 0;
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totalnumkvsread[i] = 0; }
	for(unsigned int i=0; i<numedgebanks; i++){ vertexpropertiesbuffer[i] = new vertexprop_t[numverticespervertexbank]; }
}
grafboost_pr_sw::~grafboost_pr_sw() {} 

void grafboost_pr_sw::run(){
	for(graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
		cout<<"grafboost_pr_sw::run: graph iteration "<<graph_iterationidx<<" of pagerank Started"<<endl;
		start();
		summary();
		
		#if not (defined(LAUNCHKERNEL) || defined(LOADKVDRAMS))
		break;
		#endif 
		if(NUMCPUTHREADS > MAXNUMVERTEXBANKS){ std::cout<<"hostprocess:: WARNING: maxed out on available parallelism. NUMCPUTHREADS is greater than MAXNUMVERTEXBANKS. returning..."<<std::endl; exit(EXIT_SUCCESS); }	
		
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
		
		if ( active_cnt == 0 ) { cout<<"Host: No more active vertices to process. Exiting..."<<endl; break; }
	}
	return;
}
void grafboost_pr_sw::start() {
	edge_t edgepropertyfilesize = lseek(nvmeFd_edgeproperties_r2[0][0], 0, SEEK_END) / sizeof(edgeprop2_t);
	cout<<">>> grafboost_pr_sw::start: launching threads..."<<endl;
	for (int i = 0; i < NUMCPUTHREADS; i++) { panas_thread[i] = std::thread(&grafboost_pr_sw::WorkerThread, this, i, (i * (edgepropertyfilesize / NUMCPUTHREADS)), (edgepropertyfilesize / NUMCPUTHREADS)); }
	for ( int i = 0; i < NUMCPUTHREADS; i++ ) { panas_thread[i].join(); }
	cout<<">>> grafboost_pr_sw:: Finished: all threads joined..."<<endl;
	finish();
}
void grafboost_pr_sw::finish(){
	cout<<"grafboost_pr_sw::finish destroying memory structures... "<<endl;
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		delete[] kvdramA[i];
		delete[] kvdramB[i];
		delete[] kvdramC[i];
		delete[] kvdramD[i];
		delete[] kvstatsA[i];
		delete[] kvstatsB[i];
		delete[] kvstatsC[i];
		delete[] kvstatsD[i];
	}
}
void grafboost_pr_sw::summary(){
	edge_t _totalnumkvsread = 0;
	for ( int i = 0; i < NUMCPUTHREADS; i++ ) {
		cout<<">>> grafboost_pr_sw::summary Total number of kvReads for thread ["<<i<<"]: "<<totalnumkvsread[i]<<endl;
		_totalnumkvsread += totalnumkvsread[i];
	}
	cout<<">>> grafboost_pr_sw::summary Total number of kvReads for all threads: "<<_totalnumkvsread<<endl;
	#ifdef LOCKE
	cout<<">>> grafboost_pr_sw::summary Total kernel time: "<<totaltime_topkernel_ms<<" milli seconds"<<endl;
	cout<<">>> grafboost_pr_sw::summary Total kernel time: "<<(totaltime_topkernel_ms / 1000)<<" seconds"<<endl;	
	cout<<">>> grafboost_pr_sw::summary Total populate KvDRAM time: "<<totaltime_populatekvdram_ms<<" milli seconds"<<endl;
	cout<<">>> grafboost_pr_sw::summary Total populate KvDRAM time: "<<(totaltime_populatekvdram_ms / 1000)<<" seconds"<<endl;	
	#endif 
	return;
}
float grafboost_pr_sw::totalkerneltime(){ return totaltime_topkernel_ms; }
float grafboost_pr_sw::totalpopulateKvDRAMtime(){ return totaltime_populatekvdram_ms; }

void grafboost_pr_sw::WorkerThread(int threadidx, edge_t ledgeoffset, edge_t ledgeize){
	#ifdef LOCKE
	mutex5_workthread.lock();
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_partitionandapply = std::chrono::steady_clock::now();
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"grafboost_pr_sw::WorkerThread: threadidx: "<<threadidx<<endl;
	#endif
	
	unsigned int iteration_size = utilityobj[threadidx]->hceildiv(ledgeize, (edge_t)(KVDATA_BATCHSIZE * NUMDRAMBANKS));
	for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx++) {
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"PP&A:: [threadidx:"<<threadidx<<"][size:"<<NUMCPUTHREADS<<"][step:1], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
		#endif 
		#ifdef _DEBUGMODE_TIMERS
		std::chrono::steady_clock::time_point begintime_iterationidx = std::chrono::steady_clock::now();
		#endif
		
		// Populate kvdrams 
		#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
		mutex5_populatekvdrams.lock();
		std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
		#endif
		size_t loffsetA = (iteration_idx * NUMDRAMBANKS * KVDATA_BATCHSIZE) + (0 * KVDATA_BATCHSIZE);
		vertex_t edgeszA = hmin(KVDATA_BATCHSIZE, hsub((edge_t)ledgeize, loffsetA));
		edge_process_obj[threadidx]->generateupdates_contiguous((ledgeize + loffsetA), edgeszA, nvmeFd_edgeproperties_r2[0][0], nvmeFd_vertexoffsets_r2[0][0], vertexpropertiesbuffer[0], kvdramA[threadidx], kvstatsA[threadidx], NAp);				
		totalnumkvsread[threadidx] += edgeszA;
		size_t loffsetB = (iteration_idx * NUMDRAMBANKS * KVDATA_BATCHSIZE) + (1 * KVDATA_BATCHSIZE);
		vertex_t edgeszB = hmin(KVDATA_BATCHSIZE, hsub((edge_t)ledgeize, loffsetB));
		edge_process_obj[threadidx]->generateupdates_contiguous((ledgeize + loffsetB), edgeszB, nvmeFd_edgeproperties_r2[0][0], nvmeFd_vertexoffsets_r2[0][0], vertexpropertiesbuffer[0], kvdramB[threadidx], kvstatsB[threadidx], NAp);				
		totalnumkvsread[threadidx] += edgeszB;
		size_t loffsetC = (iteration_idx * NUMDRAMBANKS * KVDATA_BATCHSIZE) + (2 * KVDATA_BATCHSIZE);
		vertex_t edgeszC = hmin(KVDATA_BATCHSIZE, hsub((edge_t)ledgeize, loffsetC));
		edge_process_obj[threadidx]->generateupdates_contiguous((ledgeize + loffsetC), edgeszC, nvmeFd_edgeproperties_r2[0][0], nvmeFd_vertexoffsets_r2[0][0], vertexpropertiesbuffer[0], kvdramC[threadidx], kvstatsC[threadidx], NAp);				
		totalnumkvsread[threadidx] += edgeszC;
		size_t loffsetD = (iteration_idx * NUMDRAMBANKS * KVDATA_BATCHSIZE) + (3 * KVDATA_BATCHSIZE);
		vertex_t edgeszD = hmin(KVDATA_BATCHSIZE, hsub((edge_t)ledgeize, loffsetD));
		edge_process_obj[threadidx]->generateupdates_contiguous((ledgeize + loffsetD), edgeszD, nvmeFd_edgeproperties_r2[0][0], nvmeFd_vertexoffsets_r2[0][0], vertexpropertiesbuffer[0], kvdramD[threadidx], kvstatsD[threadidx], NAp);				
		totalnumkvsread[threadidx] += edgeszD;
		#ifdef _DEBUGMODE_TIMERS
		stopTIME("PROCESS, SORT-REDUCE AND APPLY PHASE: Populate Kv DRAM Time Elapsed: ", begintime_populatekvdram, iteration_idx);
		#endif
		#ifdef LOCKE
		totaltime_populatekvdram_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_populatekvdram).count();
		mutex5_populatekvdrams.unlock();
		#endif
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"Host::Process, Sort-reduce and Apply phase:: Print results before Kernel run "<<endl;
		printstructures(threadidx);
		#endif
		
		// Launch the Kernel
		#ifdef LAUNCHKERNEL
		#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
		std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
		#endif
		mutex5_srkernel.lock(); // lock
		srkernel_process->srtopkernel(
			sr
			,(keyvalue_t *)kvdramA[threadidx]
			
			,edgeszA
);
		mutex5_srkernel.unlock(); // unlock
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
		stopTIME("PROCESS, SORT-REDUCE AND APPLY PHASE: Iteration Time Elapsed: ", begintime_iterationidx, iteration_idx);
		#endif
	}
	#ifdef _DEBUGMODE_TIMERS
	stopTIME("Total time elapsed (Entire Process, Sort-reduce & Apply process): ", begintime_partitionandapply, NAp);
	#endif
	#ifdef LOCKE
	mutex5_workthread.unlock();
	#endif 
}

size_t grafboost_pr_sw::hsub(size_t val1, size_t val2){
	if(val1 < val2){ return 0; }
	else { return val1 - val2; }
}
size_t grafboost_pr_sw::hceildiv(size_t val1, size_t val2){
	return (val1 + (val2 - 1)) / val2;
}
int grafboost_pr_sw::hmin(size_t val1, size_t val2){
	if(val1 < 0){ val1 = 0; }
	if(val2 < 0){ val2 = 0; }
	if(val1 < val2){ return val1; }
	else { return val2; }
}
void grafboost_pr_sw::stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx){
	// #ifdef _DEBUGMODE_HOSTPRINTS
	std::chrono::steady_clock::time_point endtime = std::chrono::steady_clock::now();		
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::nanoseconds> (endtime - begintime).count() << "[nanao seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;			
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::microseconds> (endtime - begintime).count() << "[micro seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds> (endtime - begintime).count() << "[milli seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::seconds> (endtime - begintime).count() << "[seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << std::endl;
	// #endif
	return;
}

void grafboost_pr_sw::printkvdrams(uint512_vec_dt * kvdram){
	// Print results after Kernel run
	for(unsigned int i = 0 ; i<(16/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<i<<"].data["<<j<<"].key: "<<kvdram[i].data[j].key<<std::endl;
		}
    }
	for(unsigned int i = 0 ; i<(16/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<(PADDEDKVDATA_BATCHSIZE_KVS + i)<<"].data["<<j<<"].key: "<<kvdram[(PADDEDKVDATA_BATCHSIZE_KVS + i)].data[j].key<<std::endl;
		}
    }
	std::cout<<std::endl;
	return;
}
void grafboost_pr_sw::printstructures(unsigned int threadidx){
	printkvdrams(kvdramA[threadidx]);
	printkvdrams(kvdramB[threadidx]);
	printkvdrams(kvdramC[threadidx]);
	printkvdrams(kvdramD[threadidx]);
	return;
}

unsigned int grafboost_pr_sw::loadedgesfromfile(int nvmeFd_edges_r, size_t fileoffset, edgeprop2_t * edgesbuffer, edge_t edgesoffset, edge_t edgessize){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "load edges from file. "<< std::endl;
	std::cout << "Host::loadedgesfromfile. loading edges from file... edgesoffset: "<<edgesoffset<<endl;
	#endif
	
	if(edgessize > 0){ if(pread(nvmeFd_edges_r, &edgesbuffer[edgesoffset], (size_t)(edgessize * sizeof(edgeprop2_t)), fileoffset) < 0){ cout<<"loadedgesfromfile::ERROR 37"<<endl; exit(EXIT_FAILURE); }}	
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<endl<< "Host::loadedgesfromfile. number of edges loaded from file: "<<edgessize<<endl;
	std::cout <<"Host::loadedgesfromfile. edgesbuffer["<<0<<"].srcvid: "<<edgesbuffer[0].srcvid<<", edgesbuffer["<<0<<"].dstvid: "<<edgesbuffer[0].dstvid<<endl;
	std::cout <<"Host::loadedgesfromfile. edgesbuffer["<<1<<"].srcvid: "<<edgesbuffer[1].srcvid<<", edgesbuffer["<<1<<"].dstvid: "<<edgesbuffer[1].dstvid<<endl;
	if(edgessize > 0){ std::cout <<"Host::loadedgesfromfile. edgesbuffer["<<(edgessize-1)<<"].srcvid: "<<edgesbuffer[(edgessize-1)].srcvid<<", edgesbuffer["<<(edgessize-1)<<"].dstvid: "<<edgesbuffer[(edgessize-1)].dstvid<<endl; }
	std::cout << endl;
	#endif 
	return edgessize;
}

#endif 



