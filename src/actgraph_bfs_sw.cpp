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
#include "actgraph_bfs_sw.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "actgraph_pr_sw.h"
#include "../algorithm/algorithm.h"
#include "../heuristics/heuristics.h"
#include "../utility/utility.h"
#include "actgraph_pr_sw.h"
using namespace std;
#define YES

std::mutex mutex2_workthread;
std::mutex mutex2_buffervertexupdates;
std::mutex mutex2_topkernel1;
std::mutex mutex2_topkernel2;
std::mutex mutex2_populatekvdrams1;
std::mutex mutex2_populatekvdrams2;
#ifdef FPGA_IMPL
#define OCL_CHECK(call)							\
	do {								\
		cl_int err = call;					\
		if (err != CL_SUCCESS) {				\
			cout << "ERR: Error calling " #call		\
				", error code is: " << err << endl;	\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0);
#endif 
	
actgraph_bfs_sw::actgraph_bfs_sw(graph * _graphobj, heuristics * _heuristicsobj, actgraph_pr_sw * _actgraph_pr_sw_obj, std::string _binaryFile){
	graphobj = _graphobj;
	heuristicsobj = _heuristicsobj;
	actgraph_pr_sw_obj = _actgraph_pr_sw_obj;
	binaryFile = _binaryFile;
	
	#ifdef SW
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvsourcedram[i][ddr] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdestdram[i][ddr] = new uint512_vec_dt[KVDATA_RANGE_PERSSDPARTITION_KVS]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][ddr] = new keyvalue_t[KVSTATSDRAMSZ]; }
	}
	#endif
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvsourcedram[i][ddr] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdestdram[i][ddr] = (uint512_vec_dt *) aligned_alloc(4096, (KVDATA_RANGE_PERSSDPARTITION_KVS * sizeof(uint512_vec_dt))); }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][ddr] = (keyvalue_t *) aligned_alloc(4096, (KVSTATSDRAMSZ * sizeof(keyvalue_t))); }
	}
	#endif

	totaltime_topkernel1_ms = 0;
	totaltime_topkernel2_ms = 0;
	totaltime_populatekvdram1_ms = 0;
	totaltime_populatekvdram2_ms = 0;

	size_t filelenght = lseek(_graphobj->getnvmeFd_activevertexids_r2(), 0, SEEK_END) / sizeof(keyvalue_t);
	for(unsigned int i=0; i<NUMCPUTHREADS; i++) {
		size_t beginoffset =  (size_t)i * (size_t)(filelenght / NUMCPUTHREADS); 
		size_t endoffset = (size_t)beginoffset + (size_t)(filelenght / NUMCPUTHREADS);
		
		if(i==0){ reader_activevertexids_r2[i] = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(graphobj->getnvmeFd_activevertexids_r2(), (beginoffset * sizeof(keyvalue_t)), ((endoffset + (filelenght % NUMCPUTHREADS)) * sizeof(keyvalue_t))); }				
		else { reader_activevertexids_r2[i] = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(graphobj->getnvmeFd_activevertexids_r2(), (beginoffset * sizeof(keyvalue_t)), (endoffset * sizeof(keyvalue_t)));  }
	}
	
	for(unsigned int i=0; i<graphobj->getnumvertexbanks(); i++){ vertexpropertiesbuffer[i] = new vertexprop_t[(KVDATA_RANGE / graphobj->getnumvertexbanks())]; }
	for(unsigned int i=0; i<graphobj->getnumvertexbanks(); i++){ verticesdatabuffer[i] = new value_t[(KVDATA_RANGE / graphobj->getnumvertexbanks())]; }
	unsigned int isactivevertexinfo = (KVDATA_RANGE / graphobj->getnumvertexbanks()) / NUMBITSINUNSIGNEDINT;
	for(unsigned int i=0; i<graphobj->getnumvertexbanks(); i++){ 
		isactivevertexinfobuffer[i] = new unsigned int[isactivevertexinfo]; 
		for(unsigned int k=0; k<isactivevertexinfo; k++){ isactivevertexinfobuffer[i][k] = 0; }}
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edge_process_obj[i] = new edge_process(_graphobj); }
	#ifdef SW
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ kernel_process[i] = new kernelprocess(); }
	#endif
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	algorithmobj = new algorithm();
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totalnumkvsread1[i] = 0; }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totalnumkvsread2[i] = 0; }
}
actgraph_bfs_sw::~actgraph_bfs_sw() {
	cout<<"actgraph_bfs_sw::~actgraph_bfs_sw::finish destroying memory structures... "<<endl;
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvsourcedram[i][ddr]; }
	}
	for(unsigned int i=0; i<graphobj->getnumvertexbanks(); i++){ buffer_vertexupdates_w[i].clear(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<graphobj->getnumvertexbanks(); j++){ intermediatevertexupdates[i][j].clear(); }}
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ buffer_vertexupdates_w[i].clear(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSSDPARTITIONS; j++) { intermediatevertexupdates[i][j].clear(); }}
	for(unsigned int i=0; i<MAXNUMVERTEXBANKS; i++){ delete [] vertexpropertiesbuffer[i]; }	
	#ifdef FPGA_IMPL
	finishOCL();
	#endif 
} 

unsigned int actgraph_bfs_sw::run(){
	graphobj->writerootvertextoactiveverticesfiles(12, 0);
	
	// for(graph_iterationidx=0; graph_iterationidx<64; graph_iterationidx++){
	// for(graph_iterationidx=0; graph_iterationidx<5; graph_iterationidx++){
	for(graph_iterationidx=8; graph_iterationidx<9; graph_iterationidx++){
		cout<<"actgraph_bfs_sw::run: graph iteration "<<graph_iterationidx<<" of pagerank Started"<<endl;
		
		graphobj->openactiveverticesfilesforreading(graph_iterationidx);
		graphobj->openactiveverticesfilesforwriting((graph_iterationidx + 1));
		unsigned int numactivevertices = graphobj->getnumactivevertices();
		cout<<"=== actgraph_bfs_sw::run: number of active vertices for graph iteration "<<graph_iterationidx<<": "<<numactivevertices<<" ==="<<endl;
		if(numactivevertices == 0){ cout<<"actgraph_bfs_sw::run: no more active vertices to process. exiting..."<<endl; break; }
		reloadactvverticesfiles(); // important
		
		heuristicstype_t edges_access_type = heuristicsobj->gettype(0, graph_iterationidx, numactivevertices);		
		if(edges_access_type.changed == true){
			cout<<"actgraph_bfs_sw:: type changed, switching data structures for graph iteration "<<graph_iterationidx<<endl;
			graphobj->closefilesforreading();
			graphobj->setbanks(edges_access_type.numvertexbanks, edges_access_type.numedgebanks);
			graphobj->openfilesforreading();
		} else { cout<<"actgraph_bfs_sw:: type not changed for graph iteration "<<graph_iterationidx<<endl; }
		
		if(edges_access_type.type == STREAMING_ACCESS_TYPE){
			cout<<"=== STREAMING_ACCESS_TYPE ==="<<endl;
			actgraph_pr_sw_obj->setgraphiteration(graph_iterationidx);
			actgraph_pr_sw_obj->start2();
		} if(edges_access_type.type == PARTIAL_ACCESS_TYPE){
			cout<<"=== PARTIAL_ACCESS_TYPE ==="<<endl;
			start();
		}
		
		graphobj->closeactiveverticesfilesforreading();
		graphobj->closeactiveverticesfilesforwriting();
		for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ buffer_vertexupdates_w[i].clear(); }
		for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSSDPARTITIONS; j++) { intermediatevertexupdates[i][j].clear(); }}
		summary();
	}
	// finish();
	return graph_iterationidx;
}
void actgraph_bfs_sw::start() {
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	cout<<">>> actgraph_bfs_sw::start: generating & partitioning vertex updates ..."<<endl;	
	cout<<">>> actgraph_bfs_sw::start: launching threads..."<<endl;
	for (int i = 0; i < NUMCPUTHREADS; i++) { panas_thread[i] = std::thread(&actgraph_bfs_sw::WorkerThread1, this, i); }
	for (int i = 0; i < NUMCPUTHREADS; i++ ) { panas_thread[i].join(); }
	cout<<">>> actgraph_bfs_sw:: Finished: all threads joined..."<<endl;
	
	cout<<">>> actgraph_bfs_sw::start: partitioning and applying vertex updates ..."<<endl;
	for(unsigned int i_batch=0; i_batch<NUMSSDPARTITIONS; i_batch += NUMCPUTHREADS){
		cout<<">>> actgraph_bfs_sw::start: super iteration: [i_batch: "<<i_batch<<"][size: "<<NUMSSDPARTITIONS<<"][step: "<<NUMCPUTHREADS<<"]"<<endl;
		cout<<">>> actgraph_bfs_sw::start: launching threads..."<<endl;
		for (int i = 0; i < NUMCPUTHREADS; i++) { actgraph_pr_sw_obj->loadvertexpropertiesfromfile(graphobj->getnvmeFd_vertexproperties_r2()[(i_batch + i)], 0, vertexpropertiesbuffer[i], 0, utilityobj[i]->GETKVDATA_RANGE_FORSSDPARTITION_((i_batch + i))); }
		
		#ifdef SW
		for (int i = 0; i < NUMCPUTHREADS; i++) { panas_thread[i] = std::thread(&actgraph_bfs_sw::WorkerThread2, this, i, i_batch); }
		for ( int i = 0; i < NUMCPUTHREADS; i++ ) { panas_thread[i].join(); }
		/// for (int i = 0; i < NUMCPUTHREADS; i++) { actgraph_pr_sw_obj->writevertexpropertiestofile(graphobj->getnvmeFd_vertexproperties_w2()[(i_batch + i)], 0, vertexpropertiesbuffer[i], 0, utilityobj[i]->GETKVDATA_RANGE_FORSSDPARTITION_((i_batch + i))); }
		#else 
		for (int i = 0; i < NUMCPUTHREADS; i++) { WorkerThread2(i, i_batch); }
		#endif 
		
		#ifndef PR_ALGORITHM
		for (int i = 0; i < NUMDRAMBANKS; i++) { graphobj->savevertexisactiveinfostofile(graphobj->getnvmeFd_vertexisactive_w2(), utilityobj[0]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((i_batch + i)), isactivevertexinfobuffer[i], 0, utilityobj[0]->GETKVDATA_RANGE_FORSSDPARTITION_((i_batch + i)), (graph_iterationidx + 1)); }			
		#endif
		cout<<">>> actgraph_bfs_sw:: Finished: all threads joined..."<<endl;
	}
	utilityobj[0]->stopTIME("actgraph_bfs_sw::start: finished start. Time Elapsed: ", begintime, NAp);
}
void actgraph_bfs_sw::reloadactvverticesfiles(){
	size_t filelenght = lseek(graphobj->getnvmeFd_activevertexids_r2(), 0, SEEK_END) / sizeof(keyvalue_t);
	for(unsigned int i=0; i<NUMCPUTHREADS; i++) {
		size_t beginoffset =  (size_t)i * (size_t)(filelenght / NUMCPUTHREADS); 
		size_t endoffset = (size_t)beginoffset + (size_t)(filelenght / NUMCPUTHREADS);

		if(i==0){ reader_activevertexids_r2[i] = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(graphobj->getnvmeFd_activevertexids_r2(), (beginoffset * sizeof(keyvalue_t)), ((endoffset + (filelenght % NUMCPUTHREADS)) * sizeof(keyvalue_t))); }				
		else { reader_activevertexids_r2[i] = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(graphobj->getnvmeFd_activevertexids_r2(), (beginoffset * sizeof(keyvalue_t)), (endoffset * sizeof(keyvalue_t)));  }
	}
	return;
}
void actgraph_bfs_sw::reloadenv(){ return; }
void actgraph_bfs_sw::finish(){
	#ifdef FPGA_IMPL
	finishOCL();
	#endif
}
void actgraph_bfs_sw::summary(){
	edge_t _totalnumkvsread1 = 0;
	edge_t _totalnumkvsread2 = 0;
	for ( int i = 0; i < NUMCPUTHREADS; i++ ) {
		cout<<">>> actgraph_bfs_sw::summary Total number of kvReads for thread ["<<i<<"]: "<<totalnumkvsread1[i]<<endl;
		_totalnumkvsread1 += totalnumkvsread1[i];
	}
	for ( int i = 0; i < NUMCPUTHREADS; i++ ) {
		cout<<">>> actgraph_bfs_sw::summary Total number of kvReads for thread ["<<i<<"]: "<<totalnumkvsread2[i]<<endl;
		_totalnumkvsread2 += totalnumkvsread2[i];
	}
	cout<<">>> actgraph_bfs_sw::summary Total number of kvReads for all threads (1): "<<_totalnumkvsread1<<endl;
	cout<<">>> actgraph_bfs_sw::summary Total number of kvReads for all threads (2): "<<_totalnumkvsread2<<endl;
	#ifdef LOCKE
	cout<<">>> actgraph_bfs_sw::summary Total kernel 1 time: "<<totaltime_topkernel1_ms<<" milli seconds"<<endl;
	cout<<">>> actgraph_bfs_sw::summary Total kernel 1 time: "<<(totaltime_topkernel1_ms / 1000)<<" seconds"<<endl;	
	cout<<">>> actgraph_bfs_sw::summary Total kernel 2 time: "<<totaltime_topkernel2_ms<<" milli seconds"<<endl;
	cout<<">>> actgraph_bfs_sw::summary Total kernel 2 time: "<<(totaltime_topkernel2_ms / 1000)<<" seconds"<<endl;	
	cout<<">>> actgraph_bfs_sw::summary Total populate KvDRAM 1 time: "<<totaltime_populatekvdram1_ms<<" milli seconds"<<endl;
	cout<<">>> actgraph_bfs_sw::summary Total populate KvDRAM 1 time: "<<(totaltime_populatekvdram1_ms / 1000)<<" seconds"<<endl;	
	cout<<">>> actgraph_bfs_sw::summary Total populate KvDRAM 2 time: "<<totaltime_populatekvdram2_ms<<" milli seconds"<<endl;
	cout<<">>> actgraph_bfs_sw::summary Total populate KvDRAM 2 time: "<<(totaltime_populatekvdram2_ms / 1000)<<" seconds"<<endl;	
	#endif 
	return;
}
float actgraph_bfs_sw::totalkerneltime(){ return (totaltime_topkernel1_ms + totaltime_topkernel2_ms); }
float actgraph_bfs_sw::totalpopulateKvDRAMtime(){ return (totaltime_populatekvdram1_ms + 0); }

void actgraph_bfs_sw::WorkerThread1(int threadidx){
	#ifdef LOCKE
	mutex2_workthread.lock();
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_actgraph_bfs2_sw = std::chrono::steady_clock::now();
	#endif

	unsigned int iteration_size = 256;
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
			if(edgeprogramisactive == true){ kvsize[ddr] = edge_process_obj[threadidx]->generateupdates_random(reader_activevertexids_r2[threadidx], graphobj->getnvmeFd_edgeoffsets_r2()[0][0], graphobj->getnvmeFd_edgeproperties_r2()[0][0], kvsourcedram[threadidx][ddr], 0, &status); }
			else { kvsize[ddr] = edge_process_obj[threadidx]->generateupdates2_random(reader_activevertexids_r2[threadidx], (keyvalue_t *)kvsourcedram[threadidx][ddr], 0, &status); }
			totalnumkvsread1[threadidx] += kvsize[ddr];
		}
		#else
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			kvsize[ddr] = edge_process_obj[threadidx]->generateupdates_random(reader_activevertexids_r2[threadidx], graphobj->getnvmeFd_edgeoffsets_r2()[0][0], graphobj->getnvmeFd_edgeproperties_r2()[0][0], kvsourcedram[threadidx][ddr], 0, &status);		
			totalnumkvsread1[threadidx] += kvsize[ddr];
		}
		#endif 
		#ifdef LOCKE
		#ifdef _DEBUGMODE_TIMERS
		utilityobj[threadidx]->stopTIME("ACTGRAPH_BFS_SW::PARTITION PHASE: Populate Kv DRAM Time Elapsed: ", begintime_populatekvdram, iteration_idx);
		#endif
		totaltime_populatekvdram1_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_populatekvdram).count();
		#endif
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"ACTGRAPH_BFS_SW::PARTITION PHASE:: Print results before Kernel run "<<endl;
		printstructures(threadidx);
		#endif

		// Launch the Kernel
		#ifdef LAUNCHKERNEL
		#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
		std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
		#endif
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ this->partitionupdates((keyvalue_t *) kvsourcedram[threadidx][ddr], intermediatevertexupdates[threadidx], 0, kvsize[ddr], (KVDATA_RANGE / NUMSSDPARTITIONS)); }
		#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
		totaltime_topkernel1_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
		#endif
		#endif 
		
		// Buffer updates
		mutex2_buffervertexupdates.lock(); // lock
		this->appendupdatestobuffer(intermediatevertexupdates[threadidx], buffer_vertexupdates_w, graphobj->getnvmeFd_vertexupdates_w(), true);
		mutex2_buffervertexupdates.unlock(); // unlock
		for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ intermediatevertexupdates[threadidx][i].clear(); }

		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"ACTGRAPH_BFS_SW::PARTITION PHASE:: Print results after Kernel run "<<endl;
		printstructures(threadidx);
		#endif
	
		#ifdef _DEBUGMODE_TIMERS
		utilityobj[threadidx]->stopTIME("ACTGRAPH_BFS_SW::PARTITION PHASE: Iteration Time Elapsed: ", begintime_iterationidx, iteration_idx);
		#endif
		if(status == FINISHED){ cout<<"ACTGRAPH_BFS_SW::PARTITION PHASE:: no more active vertices to process. exiting."<<endl; break; }
	}
	
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("ACTGRAPH_BFS_SW::PARTITION PHASE::Total time elapsed: ", begintime_actgraph_bfs2_sw, NAp);
	#endif
	#ifdef LOCKE
	mutex2_workthread.unlock();
	#endif 
}
void actgraph_bfs_sw::WorkerThread2(int threadidx, int bankoffset){
	#ifdef LOCKE
	mutex2_workthread.lock();
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_actgraph_bfs2_sw = std::chrono::steady_clock::now();
	#endif
	#ifdef FPGA_IMPL
	unsigned int giteration_idx = 0;
	#endif 
	
	// load temp vertices data // FIXME. ensure non-sharing
	actgraph_pr_sw_obj->loadverticesdatafromfile(graphobj->getnvmeFd_verticesdata_r2()[(bankoffset + threadidx)], 0, (keyvalue_t *)kvdestdram[threadidx][0], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidx)));
	actgraph_pr_sw_obj->replicateverticesdata((keyvalue_t *)kvdestdram[threadidx][0],(keyvalue_t *)kvdestdram[threadidx][1],(keyvalue_t *)kvdestdram[threadidx][2],(keyvalue_t *)kvdestdram[threadidx][3], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidx)));
	
	unsigned int kvcount[NUMDRAMBANKS];
	unsigned int iteration_size = utilityobj[threadidx]->hceildiv(buffer_vertexupdates_w[(bankoffset + threadidx)].size(), (NUMDRAMBANKS * KVDATA_BATCHSIZE));
	for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx++){
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"PP&A:: [graph_iterationidx:"<<graph_iterationidx<<"], [threadidx:"<<threadidx<<"][size:"<<NUMCPUTHREADS<<"][step:1], [bankoffset:"<<bankoffset<<"][size:"<<NUMSSDPARTITIONS<<"][step:"<<NUMCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
		#endif 
		#ifdef _DEBUGMODE_TIMERS
		std::chrono::steady_clock::time_point begintime_iterationidx = std::chrono::steady_clock::now();
		#endif
		
		#ifdef FPGA_IMPL
		int flag = getflag(giteration_idx);
		#endif
		
		// Populate kvdrams
		generatekvs(threadidx, bankoffset, iteration_idx, iteration_size, kvcount);
		
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_RUNKERNELCOMMANDID)].key = ON;	
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_COMMANDID)].key = NAp;		
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_PROCESSCOMMANDID)].key = OFF;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_PARTITIONCOMMANDID)].key = ON;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_APPLYUPDATESCOMMANDID)].key = ON;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_KVDATASIZEID)].key = KVDATA_BATCHSIZE;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_FINALNUMPARTITIONSID)].key = NAp;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_TREEDEPTHID)].key = NAp;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_ITERATIONID)].key = iteration_idx;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_SSDPARTITIONID)].key = (bankoffset + threadidx); 
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_VOFFSET)].key = utilityobj[threadidx]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(threadidx); // threadidx * 
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_VSIZE)].key = NAp;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_TREEDEPTH)].key = utilityobj[threadidx]->GETTREEDEPTH_((bankoffset + threadidx)); 
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_FINALNUMPARTITIONS)].key = pow(NUM_PARTITIONS, utilityobj[threadidx]->GETTREEDEPTH_((bankoffset + threadidx))); 
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID)].key = KVDATA_BATCHSIZE_KVS;
			kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_GRAPHITERATIONID)].key = graph_iterationidx;
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"ACTGRAPH_BFS_SW::PARTITION AND APPLY PHASE:: Print results before Kernel run "<<endl;
		printstructures(threadidx);
		#endif
		#ifdef _DEBUGMODE_CHECKSX
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printtempverticesdata(kvsourcedram[threadidx][ddr]); }
		#endif
		
		// Launch the Kernel
		#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
		std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
		#endif
		#ifdef SW 
		kernel_process[threadidx]->topkernel(
			(uint512_dt *)kvsourcedram[threadidx][0]
,			(uint512_dt *)kvsourcedram[threadidx][1]
,			(uint512_dt *)kvsourcedram[threadidx][2]
,			(uint512_dt *)kvsourcedram[threadidx][3]
			,(uint512_dt *)kvdestdram[threadidx][0]
			,(uint512_dt *)kvdestdram[threadidx][1]
			,(uint512_dt *)kvdestdram[threadidx][2]
			,(uint512_dt *)kvdestdram[threadidx][3]
			,(keyvalue_t *)kvstats[threadidx][0]
			,(keyvalue_t *)kvstats[threadidx][1]
			,(keyvalue_t *)kvstats[threadidx][2]
			,(keyvalue_t *)kvstats[threadidx][3]
);
		#else
		launchkernel(flag);
		#endif 
		#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
		totaltime_topkernel2_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
		#endif
		
		// Print results after Kernel run
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"ACTGRAPH_BFS_SW::PARTITION AND APPLY PHASE:: Print results after Kernel run "<<endl;
		printstructures(threadidx);
		#endif
		#ifdef _DEBUGMODE_CHECKSX
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printtempverticesdata(kvsourcedram[threadidx][ddr]); }
		#endif		
		#ifdef _DEBUGMODE_CHECKS3
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->recordstats(kvstats[threadidx][ddr]); }
		#endif
		#ifdef FPGA_IMPL
		giteration_idx += 1;
		#endif
		#ifdef _DEBUGMODE_TIMERS
		utilityobj[threadidx]->stopTIME("ACTGRAPH_BFS_SW::PARTITION AND APPLY PHASE: Iteration Time Elapsed: ", begintime_iterationidx, iteration_idx);
		#endif
	}
	
	// writeback temp vertices data
	actgraph_pr_sw_obj->cummulateverticesdata(threadidx, (keyvalue_t *)kvdestdram[threadidx][0],(keyvalue_t *)kvdestdram[threadidx][1],(keyvalue_t *)kvdestdram[threadidx][2],(keyvalue_t *)kvdestdram[threadidx][3], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidx)), utilityobj[threadidx]);		
	applyvertices(threadidx, bankoffset, (keyvalue_t *)kvdestdram[threadidx][0], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidx)));
	
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("ACTGRAPH_BFS_SW::PARTITION AND APPLY PHASE::Total time elapsed: ", begintime_actgraph_bfs2_sw, NAp);
	#endif
	#ifdef LOCKE
	mutex2_workthread.unlock();
	#endif 
}

#ifdef SW
void actgraph_bfs_sw::generatekvs(int threadidx, unsigned int bankoffset, unsigned int iteration_idx, unsigned int iteration_size, unsigned int * kvcount){
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
	#endif
	
	for (int i = 0; i < NUMDRAMBANKS; i++){ kvcount[i] = 0; }
	for (int i = 0; i < NUMDRAMBANKS; i++){ workerthread_generatekvs(i, threadidx, bankoffset, iteration_idx, iteration_size, &kvcount[i]); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ totalnumkvsread2[threadidx] += kvcount[i]; }
	#ifdef _DEBUGMODE_CHECKS3
	for (int i = 0; i < NUMDRAMBANKS; i++){ globaldebugger_totalkeyvaluesstransferredtokernel += kvcount[i]; }
	#endif
	
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("PROCESS, PARTITION AND APPLY PHASE: Populate Kv DRAM Time Elapsed: ", begintime_populatekvdram, iteration_idx);
	#endif
	#ifdef LOCKE
	totaltime_populatekvdram2_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_populatekvdram).count();
	#endif
	return;
}
#endif 
#ifdef FPGA_IMPL 
void actgraph_bfs_sw::generatekvs(int threadidx, unsigned int bankoffset, unsigned int iteration_idx, unsigned int iteration_size, unsigned int * kvcount){
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
	#endif
	
	for (int i = 0; i < NUMDRAMBANKS; i++){ kvcount[i] = 0; }
	for (int i = 0; i < NUMDRAMBANKS; i++){ genw_thread[i] = std::thread(&actgraph_bfs_sw::workerthread_generatekvs, this, i, threadidx, bankoffset, iteration_idx, iteration_size, &kvcount[i]); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ genw_thread[i].join(); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ totalnumkvsread2[threadidx] += kvcount[i]; }
	#ifdef _DEBUGMODE_CHECKS3
	for (int i = 0; i < NUMDRAMBANKS; i++){ globaldebugger_totalkeyvaluesstransferredtokernel += kvcount[i]; }
	#endif
	
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("PROCESS, PARTITION AND APPLY PHASE: Populate Kv DRAM Time Elapsed: ", begintime_populatekvdram, iteration_idx);
	#endif
	#ifdef LOCKE
	totaltime_populatekvdram2_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_populatekvdram).count();
	#endif
	return;
}
#endif 
void actgraph_bfs_sw::workerthread_generatekvs(unsigned int ddr, int threadidx, unsigned int bankoffset, unsigned int iteration_idx, unsigned int iteration_size, unsigned int * kvcount){
	if((utilityobj[threadidx]->resetActs(iteration_idx) == 1) || (iteration_idx == 0)){ edge_process_obj[threadidx]->resetstats(kvstats[threadidx][ddr]); }

	*kvcount = utilityobj[threadidx]->hmin(KVDATA_BATCHSIZE, utilityobj[threadidx]->hsub((size_t)buffer_vertexupdates_w[(bankoffset + threadidx)].size(), (size_t)((size_t)((size_t)iteration_idx * (size_t)NUMDRAMBANKS * (size_t)KVDATA_BATCHSIZE) + (ddr * KVDATA_BATCHSIZE))));			
	size_t kvoffset = (size_t)((size_t)iteration_idx * (size_t)NUMDRAMBANKS * (size_t)KVDATA_BATCHSIZE) + (size_t)(ddr * KVDATA_BATCHSIZE);	
	this->loadupdatesfrombuffer(buffer_vertexupdates_w[(bankoffset + threadidx)], kvoffset, (keyvalue_t *) kvsourcedram[threadidx][ddr], 0, *kvcount, true);
	
	edge_process_obj[threadidx]->collectstats(kvstats[threadidx][ddr], *kvcount);
	
	if((utilityobj[threadidx]->runActs(iteration_idx) == 1) || (iteration_idx == (iteration_size-1))){
		kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_RUNKERNELCOMMANDID)].key = ON;
	} else { kvstats[threadidx][ddr][utilityobj[threadidx]->getmessagesAddr(MESSAGES_RUNKERNELCOMMANDID)].key = OFF; }
}
void actgraph_bfs_sw::partitionupdates(keyvalue_t * kvdram, vector<keyvalue_t> (&vertexupdates)[NUMSSDPARTITIONS], unsigned int vbegin, unsigned int keyvaluesize, unsigned int rangeperpartition){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_bfs_sw::partitionupdates:: vbegin: "<<vbegin<<", keyvaluesize: "<<keyvaluesize<<", rangeperpartition: "<<rangeperpartition<<std::endl;
	#endif 
	for(unsigned int i=0; i<keyvaluesize; i++){
		keyvalue_t keyvalue = kvdram[i];
		keyy_t key = kvdram[i].key;
		value_t value = kvdram[i].value;
		unsigned int partition = (key - vbegin) / rangeperpartition;
		if(partition >= NUMSSDPARTITIONS){ cout<<"actgraph_bfs_sw:: invalid key. key: "<<key<<", value: "<<value<<endl; exit(EXIT_FAILURE); }
		
		vertexupdates[partition].push_back(keyvalue);
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ cout<<"vertexupdates["<<i<<"].size(): "<<vertexupdates[i].size()<<endl; }
	#endif 
	return;
}
void actgraph_bfs_sw::appendupdatestobuffer(vector<keyvalue_t> (&sourcebuffer)[NUMSSDPARTITIONS], vector<keyvalue_t> (&destinationbuffer)[NUMSSDPARTITIONS], FILE * destinationfile[NUMSSDPARTITIONS], bool writetodram){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_bfs_sw::appendupdatestobuffer. buffering vertex updates to file..." << std::endl;
	#endif	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ cout<<"actgraph_bfs_sw::appendupdatestobuffer:: sourcebuffer["<<i<<"].size(): "<<sourcebuffer[i].size()<<endl; }
	#endif
	
	unsigned int totalcount = 0;
	size_t destinationoffset = 0;
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){
		if(writetodram == true){ destinationoffset = (destinationbuffer[i].size()-1); if(destinationoffset < 0){ destinationoffset = 0; }}
		if(writetodram == true){ for(unsigned int k=0; k<sourcebuffer[i].size(); k++){ destinationbuffer[i].push_back(sourcebuffer[i][k]); }} // destinationbuffer[i][(destinationoffset + k)] = sourcebuffer[i][k]; }} // FIXME. change to memcpy
		else { if(sourcebuffer[i].size() > 0){ if(fwrite (sourcebuffer[i].data(), (size_t)(sourcebuffer[i].size() * sizeof(keyvalue_t)), 1, destinationfile[i]) != 1){ cout<<"actgraph_bfs_sw::appendupdatestobuffer::ERROR seen 56. sourcebuffer["<<i<<"].size(): "<<sourcebuffer[i].size()<<endl; exit(EXIT_FAILURE); }}}
		totalcount += sourcebuffer[i].size();
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "actgraph_bfs_sw::appendupdatestobuffer. "<<totalcount<<" key-value pairs saved to "<<NUMSSDPARTITIONS<<" partitions in file / buffer."<<endl;
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS
	size_t totalsz = 0;
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ totalsz += destinationbuffer[i].size(); std::cout << "actgraph_bfs_sw::appendupdatestobuffer: destinationbuffer["<<i<<"].size: "<<destinationbuffer[i].size()<<endl; }
	std::cout << "actgraph_bfs_sw::appendupdatestobuffer: totalsz: "<<totalsz<<endl;
	#endif
	// exit(EXIT_SUCCESS);
	return;
}
void actgraph_bfs_sw::loadupdatesfrombuffer(vector<keyvalue_t> & sourcebuffer, size_t sourceoffset, keyvalue_t * kvdram, unsigned int kvoffset, unsigned int kvsize, bool loadfromdram){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl<<"actgraph_bfs_sw::loadupdatesfrombuffer. buffering vertex updates to file..." << std::endl;
	#endif 
	unsigned int totalcount = 0;
	if(loadfromdram == true){ for(unsigned int k=0; k<kvsize; k++){ kvdram[(kvoffset + k)] = sourcebuffer[(sourceoffset + k)]; }} // FIXME. change to memcpy
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "actgraph_bfs_sw::loadupdatesfrombuffer. "<<kvsize<<" key-value pairs loaded from buffer."<<endl;
	#endif 
	return;
}
void actgraph_bfs_sw::printstructures(unsigned int threadidx){
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printkvdrams(kvsourcedram[threadidx][ddr]); }	
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printstats(16, kvstats[threadidx][ddr]); }
	return;
}
void actgraph_bfs_sw::applyvertices(unsigned int threadidx, int bankoffset, keyvalue_t * kvdram, vertex_t offset, vertex_t size){
	cout<<"actgraph_bfs_sw::applyvertices:: apply vertices: threadidx: "<<threadidx<<endl;
	actgraph_pr_sw_obj->loadverticesdatafromfile(graphobj->getnvmeFd_verticesdata_r2()[(bankoffset + threadidx)], 0, verticesdatabuffer[threadidx], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((bankoffset + threadidx))); 
	
	vector<keyvalue_t> activeverticesbuffer;
	for(unsigned int i=0; i<size; i++){
		keyvalue_t kvtempdata = kvdram[offset + i];
		value_t vdata = verticesdatabuffer[threadidx][i];
		value_t temp = algorithmobj->apply(kvtempdata.value, vdata);
		verticesdatabuffer[threadidx][i] = temp;
	
		if(temp != vdata){
			keyvalue_t kv;
			#ifdef BC_ALGORITHM // REMOVEME. DIRECTIVE
			kv.key = kvtempdata.key;
			#else 
			kv.key = i;
			#endif 
			kv.value = temp;
			#ifdef _DEBUGMODE_HOSTPRINTSX
			cout<<"actgraph_bfs_sw::applyvertices:: active vertex found: kv.key: "<<kv.key<<", kv.value: "<<kv.value<<endl;
			#endif
			activeverticesbuffer.push_back(kv);
			utilityobj[threadidx]-> InsertBit(isactivevertexinfobuffer[threadidx], i, 1);
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTSX
	cout<<"actgraph_bfs_sw::applyvertices:: activeverticesbuffer.size(): "<<activeverticesbuffer.size()<<endl;
	for(unsigned int t=0; t<activeverticesbuffer.size(); t++){ cout<<"actgraph_bfs_sw::applyvertices:: activeverticesbuffer["<<t<<"].key: "<<activeverticesbuffer[t].key<<", activeverticesbuffer["<<t<<"].value: "<<activeverticesbuffer[t].value<<endl; }
	#endif
	
	actgraph_pr_sw_obj->writeverticesdatatofile(graphobj->getnvmeFd_verticesdata_w2()[(bankoffset + threadidx)], 0, verticesdatabuffer[threadidx], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidx)));
	graphobj->saveactiveverticestofile(graphobj->getnvmeFd_activevertexids_w(), activeverticesbuffer, (graph_iterationidx + 1));
	activeverticesbuffer.clear();
	return;
}
void actgraph_bfs_sw::setedgeprogramstatus(bool flag){ edgeprogramisactive = flag; }
void actgraph_bfs_sw::clearvectorbuffers(){
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ buffer_vertexupdates_w[i].clear(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSSDPARTITIONS; j++) { intermediatevertexupdates[i][j].clear(); }}
}

#ifdef FPGA_IMPL
void actgraph_bfs_sw::loadOCLstructures(std::string binaryFile){
	cl_int err;
	global = 1; local = 1;
	
	kvsource_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt);
	kvdest_size_bytes = KVDATA_RANGE_PERSSDPARTITION_KVS * sizeof(uint512_vec_dt);
	kvstats_size_bytes = KVSTATSDRAMSZ * sizeof(keyvalue_t);
	
	// FPGA World, program and kernel objects
	world = { 0 };
	world = xcl_world_single();
	clReleaseCommandQueue(world.command_queue);
	world.command_queue =
		clCreateCommandQueue(world.context, world.device_id,
						   CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
					
	// Create Program and Kernel
	program = xcl_import_binary_file(world, binaryFile.c_str()); // -44 means INVALID PROGRAM
	kernel = xcl_get_kernel(program, "topkernel");
	
	// Allocate Buffer in Global Memory
	for(int flag=0; flag<2; flag++){
		std::cout << "Allocate Buffer in Global Memory" << std::endl;
		
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
			buffer_kvsourcedram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvsource_size_bytes, kvsourcedram[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvsourcedram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
			buffer_kvdestdram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvdest_size_bytes, kvdestdram[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvdestdram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
			buffer_kvstatsdram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, kvstats[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvstatsdram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
	}
	
	#ifdef FPGA_IMPL
	actgraph_pr_sw_obj->reloadOCLenv(&world, &program, &kernel);
	actgraph_pr_sw_obj->allocateOCLbuffers();
	#endif 
	return;
}
void actgraph_bfs_sw::launchkernel(unsigned int flag){
	#ifdef LAUNCHKERNEL
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
	#endif
	// Set the Kernel Arguments. Using setArg(), i.e. setting kernel arguments, explicitly before enqueueMigrateMemObjects(), 
	// i.e. copying host memory to device memory,  allowing runtime to associate buffer with correct DDR banks automatically. 
	std::cout << "Set the Kernel Arguments" << std::endl;
	int nargs=0;
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvsourcedram[flag][ddr]);
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvdestdram[flag][ddr]);
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstatsdram[flag][ddr]);
	}
	
	// Copy input data to device global memory
	std::cout << "Copy input data to device global memory" << std::endl;
	array<cl_event, 12> write_events;
	
	// Copy data from Host to Device
	// std::chrono::steady_clock::time_point begintime_enqueuewrite = std::chrono::steady_clock::now();
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvsourcedram[flag][ddr], CL_FALSE, 0, kvsource_size_bytes, kvsourcedram[flag][ddr], 0, NULL, &write_events[ddr] ));
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdestdram[flag][ddr], CL_FALSE, 0, kvdest_size_bytes, kvdestdram[flag][ddr], 0, NULL, &write_events[4 + ddr] ));
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstatsdram[flag][ddr], CL_FALSE, 0, kvstats_size_bytes, kvstats[flag][ddr], 0, NULL, &write_events[8 + ddr] ));
	}
	
	// Launch the Kernel			
	printf("Enqueueing NDRange kernel.\n");
	std::cout << "Launch the Kernel" << std::endl;
	std::vector<cl_event> waitList; 
	for(unsigned int i=0; i<4 * NUM_KAPI; i++){ waitList.push_back(write_events[i]); }
	OCL_CHECK(clEnqueueNDRangeKernel(world.command_queue, kernel, 1, nullptr,
									&global, &local, 4 * NUM_KAPI, waitList.data(),
									&kernel_events[flag]));
	
	// Copy Result from Device Global Memory to Host Local Memory
	std::cout << "Getting Results (Device to Host)..." << std::endl;
	printf("Enqueueing Migrate Mem Object (Device to Host) calls\n");
	utilityobj[0]->set_callback(kernel_events[flag], "ooo_queue");
	#ifdef LOCKE
	clWaitForEvents(1, &kernel_events[flag]); // REMOVEME
	#endif 
	
	// Copy input data from device global memory back to host
	// This operation only needs to wait for the kernel call. This call will
	// potentially overlap the next kernel call as well as the next read
	// operations
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[0]->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Top kernel Time Elapsed: ", begintime_topkernel, NAp);
	#endif 
	#ifdef LOCKE
	totaltime_topkernel2_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
	#endif
	#endif 
}
void actgraph_bfs_sw::finishOCL(){
	for(unsigned int flag=0; flag<2; flag++){ clWaitForEvents(1, &kernel_events[flag]); }
	
	cout<<"actgraph_bfs_sw::finish: releasing and destroying all OCL structures... "<<endl;
	#ifdef LAUNCHKERNEL
	clFlush(world.command_queue);
	clFinish(world.command_queue);
	for(int flag=0; flag<2; flag++){
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvsourcedram[flag][ddr])); }
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvdestdram[flag][ddr])); }
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvstatsdram[flag][ddr])); }
		clWaitForEvents(1, &kernel_events[flag]);
	}
	
	cout<<"clReleaseCommandQueue"<<endl;
	clReleaseCommandQueue(world.command_queue);
	cout<<"clReleaseContext"<<endl;
	clReleaseContext(world.context);
	cout<<"clReleaseDevice"<<endl;
	clReleaseDevice(world.device_id); 
	cout<<"clReleaseKernel"<<endl;
	clReleaseKernel(kernel);
	cout<<"clReleaseProgram"<<endl;
	clReleaseProgram(program);
	/** cout<<"free(world.platform_id)"<<endl;
	free(world.platform_id);
	cout<<"free(world.device_id)"<<endl;
	free(world.device_id); */
	cout<<"actgraph_bfs_sw::finish: released and destroyed all OCL structures"<<endl;
	#endif 
}
unsigned int actgraph_bfs_sw::getflag(unsigned int giteration_idx){
	// wait for events to finish
	int flag = giteration_idx % 2;
	#ifdef LAUNCHKERNEL
	if (giteration_idx >= 2) {
		cout<<"actgraph_pr_fpga::WorkerThread: waiting for events..."<<endl;
		clWaitForEvents(1, &kernel_events[flag]);
	}
	#endif 
	return flag;
}
#endif




