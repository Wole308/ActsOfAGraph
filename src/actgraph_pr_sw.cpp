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
#include "EdgeProcess.h" // Grafboost header
#include "VertexValues.h" // Grafboost header
#include "sortreduce.h" // Grafboost header
#include "filekvreader.h" // Grafboost header
#include "../kernels/kernelprocess.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#include "actgraph_pr_sw.h"
using namespace std;
#define YES

std::mutex mutex1_workthread;
std::mutex mutex_loadvprop;
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

actgraph_pr_sw::actgraph_pr_sw(graph * _graphobj){
	graphobj = _graphobj;
	
	#ifdef SW
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdram[i][ddr] = new uint512_vec_dt[DRAMSZ_KVS]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][ddr] = new metadata_t[KVSTATS_SIZE]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ messages[i][ddr] = new int[MESSAGES_SIZE]; }
	}
	#endif
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdram[i][ddr] = (uint512_vec_dt *) aligned_alloc(4096, (DRAMSZ_KVS * sizeof(uint512_vec_dt))); }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][ddr] = (metadata_t *) aligned_alloc(4096, (KVSTATS_SIZE * sizeof(metadata_t))); }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ messages[i][ddr] = (int *) aligned_alloc(4096, (MESSAGES_SIZE * sizeof(int))); }
	}
	#endif
	
	totaltime_topkernel_ms = 0;
	totaltime_populatekvdram_ms = 0;
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ algorithmobj[i] = new algorithm(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edge_process_obj[i] = new edge_process(_graphobj); }
	#ifdef SW 
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ kernel_process[i] = new kernelprocess(); }	
	#endif 
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totalnumkvsread[i] = 0; }

	for(unsigned int i=0; i<graphobj->getnumvertexbanks(); i++){ vertexpropertiesbuffer[i] = new vertexprop_t[(KVDATA_RANGE / graphobj->getnumvertexbanks())]; }
	for(unsigned int i=0; i<graphobj->getnumvertexbanks(); i++){ verticesdatabuffer[i] = new value_t[(KVDATA_RANGE / graphobj->getnumvertexbanks())]; }
	#ifndef PR_ALGORITHM
	unsigned int isactivevertexinfo = (KVDATA_RANGE / graphobj->getnumvertexbanks()) / NUMBITSINUNSIGNEDINT;
	for(unsigned int i=0; i<graphobj->getnumvertexbanks(); i++){ isactivevertexinfobuffer_source[i] = new unsigned int[isactivevertexinfo]; }
	for(unsigned int i=0; i<graphobj->getnumvertexbanks(); i++){ 
		isactivevertexinfobuffer_dest[i] = new unsigned int[isactivevertexinfo]; 
		for(unsigned int k=0; k<isactivevertexinfo; k++){ isactivevertexinfobuffer_dest[i][k] = 0; }}
	#endif
}
actgraph_pr_sw::actgraph_pr_sw(){}
actgraph_pr_sw::~actgraph_pr_sw(){
	cout<<"actgraph_pr_sw::~actgraph_pr_sw:: finish destroying memory structures... "<<endl;
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){		
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvdram[i][ddr]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvstats[i][ddr]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] messages[i][ddr]; }
	}
	#ifdef FPGA_IMPL
	finishOCL();
	#endif 
}

void actgraph_pr_sw::run(){
	for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
		cout<<"actgraph_pr_sw::run: graph iteration "<<graph_iterationidx<<" of pagerank Started"<<endl;
		// start();
		start2();
		summary();
	}
	return;
}
void actgraph_pr_sw::start() {
	for(unsigned int i_batch=0; i_batch<graphobj->getnumvertexbanks(); i_batch += NUMDRAMBANKS){
		cout<<">>> actgraph_pr_sw::start: super iteration: [i_batch: "<<i_batch<<"][size: "<<graphobj->getnumvertexbanks()<<"][step: "<<NUMDRAMBANKS<<"]"<<endl;
		cout<<">>> actgraph_pr_sw::start: loading vertex properties..."<<endl;
		for (int i = 0; i < NUMDRAMBANKS; i++) { loadvertexpropertiesfromfile(graphobj->getnvmeFd_vertexproperties_r2()[(i_batch + i)], 0, vertexpropertiesbuffer[i], 0, utilityobj[i]->GETKVDATA_RANGE_FORSSDPARTITION_((i_batch + i))); }
		#ifndef PR_ALGORITHM
		for (int i = 0; i < NUMDRAMBANKS; i++) { loadvertexisactiveinfosfromfile(graphobj->getnvmeFd_vertexisactive_r2(), utilityobj[0]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((i_batch + i)), isactivevertexinfobuffer_source[i], 0, utilityobj[0]->GETKVDATA_RANGE_FORSSDPARTITION_((i_batch + i))); }
		#endif
		
		#ifdef SW
		for (int i = 0; i < NUMCPUTHREADS; i++) { panas_thread[i] = std::thread(&actgraph_pr_sw::WorkerThread, this, i, i_batch); }
		for (int i = 0; i < NUMCPUTHREADS; i++ ) { panas_thread[i].join(); }
		#else 
		for (int i = 0; i < NUMCPUTHREADS; i++) { WorkerThread(i, i_batch); }
		#endif
		
		#ifndef PR_ALGORITHM
		for (int i = 0; i < NUMDRAMBANKS; i++) { graphobj->savevertexisactiveinfostofile(graphobj->getnvmeFd_vertexisactive_w2(), utilityobj[0]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((i_batch + i)), isactivevertexinfobuffer_dest[i], 0, utilityobj[0]->GETKVDATA_RANGE_FORSSDPARTITION_((i_batch + i)), (graph_iterationidx + 1)); }
		#endif
		cout<<">>> actgraph_pr_sw:: Finished: all threads joined..."<<endl;
	}
	finish();
}
void actgraph_pr_sw::start2() {
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int i_batch=0; i_batch<graphobj->getnumvertexbanks(); i_batch += NUMCPUTHREADS){
		cout<<">>> actgraph_pr_sw::start2: super iteration: [i_batch: "<<i_batch<<"][size: "<<graphobj->getnumvertexbanks()<<"][step: "<<NUMCPUTHREADS<<"]"<<endl;
		cout<<">>> actgraph_pr_sw::start2: loading vertex properties..."<<endl;
	
		#ifdef SW
		for (int i = 0; i < NUMCPUTHREADS; i++) { panas_thread[i] = std::thread(&actgraph_pr_sw::WorkerThread2, this, i, i_batch); }
		for (int i = 0; i < NUMCPUTHREADS; i++ ) { panas_thread[i].join(); }
		#else 
		for (int i = 0; i < NUMCPUTHREADS; i++) { WorkerThread2(i, i_batch); }
		#endif
		
		cout<<">>> actgraph_pr_sw::start2 Finished: all threads joined..."<<endl;
	}
	finish();
	utilityobj[0]->stopTIME("actgraph_pr_sw::start2: finished start2. Time Elapsed: ", begintime, NAp);
}
void actgraph_pr_sw::reloadenv(){ return; }
void actgraph_pr_sw::finish(){
	#if (defined(FPGA_IMPL) && defined(PR_ALGORITHM))
	finishOCL();
	#endif
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL KERNEL TIME: "<<totalkerneltime()<<" milli seconds"<< RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL KERNEL TIME: "<<((totalkerneltime()) / 1000)<<" seconds"<< RESET <<endl;
}
void actgraph_pr_sw::summary(){
	edge_t _totalnumkvsread = 0;
	for ( int i = 0; i < NUMCPUTHREADS; i++ ) {
		cout<<">>> actgraph_pr_sw::summary Total number of kvReads for thread ["<<i<<"]: "<<totalnumkvsread[i]<<endl;
		_totalnumkvsread += totalnumkvsread[i];
	}
	cout<<">>> actgraph_pr_sw::summary Total number of kvReads for all threads: "<<_totalnumkvsread<<endl;
	#ifdef LOCKE
	cout<<">>> actgraph_pr_sw::summary Total kernel time: "<<totaltime_topkernel_ms<<" milli seconds"<<endl;
	cout<<">>> actgraph_pr_sw::summary Total kernel time: "<<(totaltime_topkernel_ms / 1000)<<" seconds"<<endl;
	cout<<">>> actgraph_pr_sw::summary Total populate KvDRAM time: "<<totaltime_populatekvdram_ms<<" milli seconds"<<endl;
	cout<<">>> actgraph_pr_sw::summary Total populate KvDRAM time: "<<(totaltime_populatekvdram_ms / 1000)<<" seconds"<<endl;
	#endif 
	return;
}
float actgraph_pr_sw::totalkerneltime(){ return totaltime_topkernel_ms; }
float actgraph_pr_sw::totalpopulateKvDRAMtime(){ return totaltime_populatekvdram_ms; }

void actgraph_pr_sw::WorkerThread(int threadidx, int gbankoffset){
	#ifdef LOCKE
	mutex1_workthread.lock();
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_partitionandapply = std::chrono::steady_clock::now();
	#endif

	unsigned int numbanksassignedtoeachthread = graphobj->getnumedgebanks() / NUMCPUTHREADS;
	unsigned int subthreadidxoffset = threadidx * (graphobj->getnumedgebanks() / NUMCPUTHREADS);	
	for(unsigned int subthreadidx = 0; subthreadidx < numbanksassignedtoeachthread; subthreadidx++){
		
		// load temp vertices data
		loadverticesdatafromfile(graphobj->getnvmeFd_verticesdata_r2()[(subthreadidxoffset + subthreadidx)], 0, (keyvalue_t *)kvdram[threadidx][0], (2 * PADDEDKVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((gbankoffset + subthreadidx)));
		replicateverticesdata((keyvalue_t *)kvdram[threadidx][0],(keyvalue_t *)kvdram[threadidx][1],(keyvalue_t *)kvdram[threadidx][2],(keyvalue_t *)kvdram[threadidx][3], (2 * PADDEDKVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((gbankoffset + subthreadidx)));
		
		// each dram bank handles the same column in a different bank
		for(unsigned int lbankoffset = 0; lbankoffset < NUMCPUTHREADS; lbankoffset += NUMDRAMBANKS){ // bank set
			
			edge_t edgepropertyfilesize[NUMDRAMBANKS];
			unsigned int _iteration_size[NUMDRAMBANKS];	
			#ifdef STREAMEDGESSETUP
			size_t sizeofedgeprop = sizeof(edgeprop1_t);
			#else 
			size_t sizeofedgeprop = sizeof(edgeprop2_t);
			#endif 
			edgepropertyfilesize[0] = lseek(graphobj->getnvmeFd_edgeproperties_r2()[(gbankoffset + lbankoffset + 0)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeofedgeprop;			
			_iteration_size[0] = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[(gbankoffset + lbankoffset + 0)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeofedgeprop), KVDATA_BATCHSIZE);
			edgepropertyfilesize[1] = lseek(graphobj->getnvmeFd_edgeproperties_r2()[(gbankoffset + lbankoffset + 1)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeofedgeprop;			
			_iteration_size[1] = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[(gbankoffset + lbankoffset + 1)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeofedgeprop), KVDATA_BATCHSIZE);
			edgepropertyfilesize[2] = lseek(graphobj->getnvmeFd_edgeproperties_r2()[(gbankoffset + lbankoffset + 2)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeofedgeprop;			
			_iteration_size[2] = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[(gbankoffset + lbankoffset + 2)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeofedgeprop), KVDATA_BATCHSIZE);
			edgepropertyfilesize[3] = lseek(graphobj->getnvmeFd_edgeproperties_r2()[(gbankoffset + lbankoffset + 3)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeofedgeprop;			
			_iteration_size[3] = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[(gbankoffset + lbankoffset + 3)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeofedgeprop), KVDATA_BATCHSIZE);
			unsigned int iteration_size = 0;
			for(unsigned int i=0; i<NUMDRAMBANKS; i++){ iteration_size = utilityobj[threadidx]->hmax(iteration_size, _iteration_size[i]); }			
			#ifdef _DEBUGMODE_HOSTPRINTS
			for(unsigned int i=0; i<NUMDRAMBANKS; i++){  cout<<"actgraph_pr_sw::WorkerThread: edgepropertyfilesize["<<i<<"]: "<<edgepropertyfilesize[i]<<", numverticespervertexbank: "<<utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_(0)<<", threadidx: "<<threadidx<<", subthreadidx: "<<(subthreadidxoffset + subthreadidx)<<", DRAMSZ_KVS: "<<DRAMSZ_KVS<<endl; }			
			#endif
			
			unsigned int runningvertexid[NUMDRAMBANKS];
			unsigned int kvcount[NUMDRAMBANKS];
			for(unsigned int i=0; i<NUMDRAMBANKS; i++){ runningvertexid[i] = utilityobj[threadidx]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((gbankoffset + lbankoffset + i)); }
			for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx++) {
				#ifdef _DEBUGMODE_HOSTPRINTS2
				cout<<"PP&A:: [threadidx:"<<threadidx<<"][size:"<<NUMCPUTHREADS<<"][step:1], [subthreadidx:"<<(subthreadidxoffset + subthreadidx)<<"][size:"<<(graphobj->getnumedgebanks() / NUMCPUTHREADS)<<"][step:1], [bank offset:"<<(gbankoffset + lbankoffset)<<"][size:"<<graphobj->getnumedgebanks()<<"][step:"<<NUMDRAMBANKS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
				#endif 
				#ifdef _DEBUGMODE_TIMERS
				std::chrono::steady_clock::time_point begintime_iterationidx = std::chrono::steady_clock::now();
				#endif
				
				// Populate kvdrams 
				generatekvs(threadidx, subthreadidxoffset, subthreadidx, gbankoffset, lbankoffset, edgepropertyfilesize, runningvertexid, iteration_idx, kvcount);
				
				for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
					messages[threadidx][ddr][MESSAGES_COMMANDID] = NAp;		
					messages[threadidx][ddr][MESSAGES_PROCESSCOMMANDID] = OFF;
					messages[threadidx][ddr][MESSAGES_PARTITIONCOMMANDID] = ON;
					messages[threadidx][ddr][MESSAGES_APPLYUPDATESCOMMANDID] = ON; // ON*, OFF
					messages[threadidx][ddr][MESSAGES_KVDATASIZEID] = KVDATA_BATCHSIZE;
					messages[threadidx][ddr][MESSAGES_FINALNUMPARTITIONSID] = NAp;
					messages[threadidx][ddr][MESSAGES_TREEDEPTHID] = NAp;
					messages[threadidx][ddr][MESSAGES_ITERATIONID] = iteration_idx;
					messages[threadidx][ddr][MESSAGES_SSDPARTITIONID] = (subthreadidxoffset + subthreadidx);
					messages[threadidx][ddr][DRAM_VOFFSET] = 0;
					messages[threadidx][ddr][DRAM_VSIZE] = NAp;
					messages[threadidx][ddr][DRAM_TREEDEPTH] = utilityobj[threadidx]->GETTREEDEPTH_((subthreadidxoffset + subthreadidx));
					messages[threadidx][ddr][DRAM_FINALNUMPARTITIONS] = pow(NUM_PARTITIONS, utilityobj[threadidx]->GETTREEDEPTH_((subthreadidxoffset + subthreadidx)));
					messages[threadidx][ddr][MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID] = PADDEDKVDATA_BATCHSIZE_KVS;
					messages[threadidx][ddr][GRAPH_ITERATIONID] = graph_iterationidx;
				}
				
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"Host::Process, Partition and Apply phase:: Print results before Kernel run "<<endl;
				printstructures(threadidx);
				#endif
				// continue;
				// exit(EXIT_SUCCESS);

				// Launch the Kernel
				#ifdef LAUNCHKERNEL
				#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
				std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
				#endif
				#ifdef SW
				kernel_process[threadidx]->topkernel(
					(uint512_dt *)kvdram[threadidx][0]
					,(metadata_t *)kvstats[threadidx][0]
					,(int *)messages[threadidx][0]
);
				#else 
					unsigned int flag = 1; // FIXME.
				launchkernel(flag);
				#endif 
				#ifdef LOCKE
				totaltime_topkernel_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
				#endif
				#endif
				
				// Print results after Kernel run
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"Host::Process, Partition and Apply phase:: Print results after Kernel run "<<endl;
				printstructures(threadidx);
				#endif
				#ifdef _DEBUGMODE_CHECKSX
				utilityobj[threadidx]->printtempverticesdata(kvdram[threadidx][0]);
				utilityobj[threadidx]->printtempverticesdata(kvdram[threadidx][1]);
				utilityobj[threadidx]->printtempverticesdata(kvdram[threadidx][2]);
				utilityobj[threadidx]->printtempverticesdata(kvdram[threadidx][3]);
				#endif
		
				#ifdef _DEBUGMODE_CHECKS2X
				checkresultfromkernel(threadidx, kvcount[NUMDRAMBANKS]);
				#endif
				#ifdef _DEBUGMODE_CHECKS3
				utilityobj[threadidx]->recordstats(kvstats[threadidx][0]);
				utilityobj[threadidx]->recordstats(kvstats[threadidx][1]);
				utilityobj[threadidx]->recordstats(kvstats[threadidx][2]);
				utilityobj[threadidx]->recordstats(kvstats[threadidx][3]);
				#endif
			
				#ifdef _DEBUGMODE_TIMERS
				utilityobj[threadidx]->stopTIME("PROCESS, PARTITION AND APPLY PHASE: Iteration Time Elapsed: ", begintime_iterationidx, iteration_idx);
				#endif
			}
		}
		
		// writeback temp vertices data
		cummulateverticesdata(threadidx, (keyvalue_t *)kvdram[threadidx][0],(keyvalue_t *)kvdram[threadidx][1],(keyvalue_t *)kvdram[threadidx][2],(keyvalue_t *)kvdram[threadidx][3], (2 * PADDEDKVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((subthreadidxoffset + subthreadidx)), utilityobj[threadidx]);
		applyvertices(threadidx, gbankoffset, (keyvalue_t *)kvdram[threadidx][0], (2 * PADDEDKVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((subthreadidxoffset + subthreadidx))); // FIXME. should be within lbankoffset loop???
	}
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("Total time elapsed (Entire Process, Partition & Apply process): ", begintime_partitionandapply, NAp);
	#endif
	#ifdef LOCKE
	mutex1_workthread.unlock();
	#endif 
}
void actgraph_pr_sw::WorkerThread2(int threadidx, int threadidxoffset){
	#ifdef LOCKE
	mutex1_workthread.lock();
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_partitionandapply = std::chrono::steady_clock::now();
	#endif
	#ifdef FPGA_IMPL
	unsigned int giteration_idx = 0;
	#endif 
	
	// load temp vertices data
	loadverticesdatafromfile(graphobj->getnvmeFd_verticesdata_r2()[(threadidxoffset + threadidx)], 0, (keyvalue_t *)kvdram[threadidx][0], (2 * PADDEDKVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)));
	replicateverticesdata((keyvalue_t *)kvdram[threadidx][0],(keyvalue_t *)kvdram[threadidx][1],(keyvalue_t *)kvdram[threadidx][2],(keyvalue_t *)kvdram[threadidx][3], (2 * PADDEDKVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)));
	
	for(unsigned int lbankoffset = 0; lbankoffset < graphobj->getnumedgebanks(); lbankoffset += NUMDRAMBANKS){
		
		edge_t edgepropertyfilesize[NUMDRAMBANKS];
		unsigned int _iteration_size[NUMDRAMBANKS];	
		#ifdef STREAMEDGESSETUP
		size_t sizeofedgeprop = sizeof(edgeprop1_t);
		#else 
		size_t sizeofedgeprop = sizeof(edgeprop2_t);
		#endif 
		
		/** mutex_loadvprop.lock(); // lock
		if(threadidx == 0){
			for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ loadvertexpropertiesfromfile(graphobj->getnvmeFd_vertexproperties_r2()[lbankoffset], 0, vertexpropertiesbuffer[ddr], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx))); }
			#ifndef PR_ALGORITHM
			for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ loadvertexisactiveinfosfromfile(graphobj->getnvmeFd_vertexisactive_r2(), utilityobj[0]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((lbankoffset + ddr)), isactivevertexinfobuffer_source[ddr], 0, utilityobj[0]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx))); }
			#endif
		}			
		mutex_loadvprop.unlock(); // unlock */
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ loadvertexpropertiesfromfile(graphobj->getnvmeFd_vertexproperties_r2()[lbankoffset], 0, vertexpropertiesbuffer[ddr], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx))); }
		#ifndef PR_ALGORITHM
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ loadvertexisactiveinfosfromfile(graphobj->getnvmeFd_vertexisactive_r2(), utilityobj[0]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((threadidxoffset + threadidx)), isactivevertexinfobuffer_source[ddr], 0, utilityobj[0]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx))); }
		#endif
		
		edgepropertyfilesize[0] = lseek(graphobj->getnvmeFd_edgeproperties_r2()[(lbankoffset + 0)][(threadidxoffset + threadidx)], 0, SEEK_END) / sizeofedgeprop;			
		_iteration_size[0] = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[(lbankoffset + 0)][(threadidxoffset + threadidx)], 0, SEEK_END) / sizeofedgeprop), KVDATA_BATCHSIZE);
		edgepropertyfilesize[1] = lseek(graphobj->getnvmeFd_edgeproperties_r2()[(lbankoffset + 1)][(threadidxoffset + threadidx)], 0, SEEK_END) / sizeofedgeprop;			
		_iteration_size[1] = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[(lbankoffset + 1)][(threadidxoffset + threadidx)], 0, SEEK_END) / sizeofedgeprop), KVDATA_BATCHSIZE);
		edgepropertyfilesize[2] = lseek(graphobj->getnvmeFd_edgeproperties_r2()[(lbankoffset + 2)][(threadidxoffset + threadidx)], 0, SEEK_END) / sizeofedgeprop;			
		_iteration_size[2] = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[(lbankoffset + 2)][(threadidxoffset + threadidx)], 0, SEEK_END) / sizeofedgeprop), KVDATA_BATCHSIZE);
		edgepropertyfilesize[3] = lseek(graphobj->getnvmeFd_edgeproperties_r2()[(lbankoffset + 3)][(threadidxoffset + threadidx)], 0, SEEK_END) / sizeofedgeprop;			
		_iteration_size[3] = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[(lbankoffset + 3)][(threadidxoffset + threadidx)], 0, SEEK_END) / sizeofedgeprop), KVDATA_BATCHSIZE);
		unsigned int iteration_size = 0;
		for(unsigned int i=0; i<NUMDRAMBANKS; i++){ iteration_size = utilityobj[threadidx]->hmax(iteration_size, _iteration_size[i]); }			
		#ifdef _DEBUGMODE_HOSTPRINTS
		for(unsigned int i=0; i<NUMDRAMBANKS; i++){ cout<<"actgraph_pr_sw::WorkerThread: edgepropertyfilesize["<<i<<"]: "<<edgepropertyfilesize[i]<<", numverticespervertexbank: "<<utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_(0)<<", threadidx: "<<(threadidxoffset + threadidx)<<", DRAMSZ_KVS: "<<DRAMSZ_KVS<<endl; }			
		#endif
		
		unsigned int runningvertexid[NUMDRAMBANKS];
		unsigned int kvcount[NUMDRAMBANKS];
		for(unsigned int i=0; i<NUMDRAMBANKS; i++){ runningvertexid[i] = utilityobj[threadidx]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((threadidxoffset + threadidx)); }
		for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx++) {
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"PP&A:: [threadidx:"<<threadidx<<"][size:"<<NUMCPUTHREADS<<"][step:1], [threadidx:"<<(threadidxoffset + threadidx)<<"][size:"<<(graphobj->getnumedgebanks() / NUMCPUTHREADS)<<"][step:1], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
			#endif 
			#ifdef _DEBUGMODE_TIMERS
			std::chrono::steady_clock::time_point begintime_iterationidx = std::chrono::steady_clock::now();
			#endif
			
			#ifdef FPGA_IMPL
			int flag = getflag(giteration_idx);
			#endif 
			
			// Populate kvdrams
			generatekvs(threadidx, threadidxoffset, threadidx, 0, lbankoffset, edgepropertyfilesize, runningvertexid, iteration_idx, kvcount);
			
			for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
				messages[threadidx][ddr][MESSAGES_COMMANDID] = NAp;		
				messages[threadidx][ddr][MESSAGES_PROCESSCOMMANDID] = OFF;
				messages[threadidx][ddr][MESSAGES_PARTITIONCOMMANDID] = ON;
				messages[threadidx][ddr][MESSAGES_APPLYUPDATESCOMMANDID] = ON; // ON*, OFF
				messages[threadidx][ddr][MESSAGES_KVDATASIZEID] = KVDATA_BATCHSIZE;
				messages[threadidx][ddr][MESSAGES_FINALNUMPARTITIONSID] = NAp;
				messages[threadidx][ddr][MESSAGES_TREEDEPTHID] = NAp;
				messages[threadidx][ddr][MESSAGES_ITERATIONID] = iteration_idx;
				messages[threadidx][ddr][MESSAGES_SSDPARTITIONID] = (threadidxoffset + threadidx);
				messages[threadidx][ddr][DRAM_VOFFSET] = 0;
				messages[threadidx][ddr][DRAM_VSIZE] = NAp;
				messages[threadidx][ddr][DRAM_TREEDEPTH] = utilityobj[threadidx]->GETTREEDEPTH_((threadidxoffset + threadidx));
				messages[threadidx][ddr][DRAM_FINALNUMPARTITIONS] = pow(NUM_PARTITIONS, utilityobj[threadidx]->GETTREEDEPTH_((threadidxoffset + threadidx)));
				messages[threadidx][ddr][MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID] = PADDEDKVDATA_BATCHSIZE_KVS;
				messages[threadidx][ddr][GRAPH_ITERATIONID] = graph_iterationidx;
			}
			
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"Host::Process, Partition and Apply phase:: Print results before Kernel run "<<endl;
			printstructures(threadidx);
			#endif
			// continue;
			// exit(EXIT_SUCCESS);

			// Launch the Kernel
			#ifdef LAUNCHKERNEL
			#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
			std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
			#endif
			#ifdef SW
			kernel_process[threadidx]->topkernel(
				(uint512_dt *)kvdram[threadidx][0]
				,(metadata_t *)kvstats[threadidx][0]
				,(int *)messages[threadidx][0]
);
			#else
			launchkernel(flag);
			#endif 
			#ifdef LOCKE
			totaltime_topkernel_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
			cout<<"----------------------- totaltime_topkernel_ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count()<<"ms. "<<endl;
			#endif
			#endif
			
			// Print results after Kernel run
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"Host::Process, Partition and Apply phase:: Print results after Kernel run "<<endl;
			printstructures(threadidx);
			#endif
			#ifdef _DEBUGMODE_CHECKSX
			utilityobj[threadidx]->printtempverticesdata(kvdram[threadidx][0]);
			utilityobj[threadidx]->printtempverticesdata(kvdram[threadidx][1]);
			utilityobj[threadidx]->printtempverticesdata(kvdram[threadidx][2]);
			utilityobj[threadidx]->printtempverticesdata(kvdram[threadidx][3]);
			#endif
	
			#ifdef _DEBUGMODE_CHECKS2X
			checkresultfromkernel(threadidx, kvcount[NUMDRAMBANKS]);
			#endif
			#ifdef _DEBUGMODE_CHECKS3
			utilityobj[threadidx]->recordstats(kvstats[threadidx][0]);
			utilityobj[threadidx]->recordstats(kvstats[threadidx][1]);
			utilityobj[threadidx]->recordstats(kvstats[threadidx][2]);
			utilityobj[threadidx]->recordstats(kvstats[threadidx][3]);
			#endif
			#ifdef FPGA_IMPL
			giteration_idx += 1;
			#endif 
			
			#ifdef _DEBUGMODE_TIMERS
			utilityobj[threadidx]->stopTIME("PROCESS, PARTITION AND APPLY PHASE: Iteration Time Elapsed: ", begintime_iterationidx, iteration_idx);
			#endif
		}
	}

	// writeback temp vertices data
	#ifndef PR_ALGORITHM
	for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ graphobj->savevertexisactiveinfostofile(graphobj->getnvmeFd_vertexisactive_w2(), utilityobj[0]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((threadidxoffset + threadidx)), isactivevertexinfobuffer_dest[threadidx], 0, utilityobj[0]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)), (graph_iterationidx + 1)); }
	#endif
	cummulateverticesdata(threadidx, (keyvalue_t *)kvdram[threadidx][0],(keyvalue_t *)kvdram[threadidx][1],(keyvalue_t *)kvdram[threadidx][2],(keyvalue_t *)kvdram[threadidx][3], (2 * PADDEDKVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)), utilityobj[threadidx]);
	applyvertices(threadidx, threadidxoffset, (keyvalue_t *)kvdram[threadidx][0], (2 * PADDEDKVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)));
		
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("Total time elapsed (Entire Process, Partition & Apply process): ", begintime_partitionandapply, NAp);
	#endif
	#ifdef LOCKE
	mutex1_workthread.unlock();
	#endif 
}

#ifdef SW
void actgraph_pr_sw::generatekvs(int threadidx, unsigned int subthreadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t * edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int * kvcount){
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
	#endif
	
	for (int i = 0; i < NUMDRAMBANKS; i++){ kvcount[i] = 0; }
	for (int i = 0; i < NUMDRAMBANKS; i++){ workerthread_generatekvs(i, threadidx, subthreadidxoffset, subthreadidx, gbankoffset, lbankoffset, edgepropertyfilesize[i], runningvertexid, iteration_idx, &kvcount[i]); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ totalnumkvsread[threadidx] += kvcount[i]; }
	#ifdef _DEBUGMODE_CHECKS3
	for (int i = 0; i < NUMDRAMBANKS; i++){ globaldebugger_totalkeyvaluesstransferredtokernel += kvcount[i]; }
	#endif
	
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("PROCESS, PARTITION AND APPLY PHASE: Populate Kv DRAM Time Elapsed: ", begintime_populatekvdram, iteration_idx);
	#endif
	#ifdef LOCKE
	totaltime_populatekvdram_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_populatekvdram).count();
	#endif
	return;
}
#endif 
#ifdef FPGA_IMPL 
void actgraph_pr_sw::generatekvs(int threadidx, unsigned int subthreadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t * edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int * kvcount){
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
	#endif
	
	for (int i = 0; i < NUMDRAMBANKS; i++){ kvcount[i] = 0; }
	for (int i = 0; i < NUMDRAMBANKS; i++){ genw_thread[i] = std::thread(&actgraph_pr_sw::workerthread_generatekvs, this, i, threadidx, subthreadidxoffset, subthreadidx, gbankoffset, lbankoffset, edgepropertyfilesize[i], runningvertexid, iteration_idx, &kvcount[i]); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ genw_thread[i].join(); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ totalnumkvsread[threadidx] += kvcount[i]; }
	#ifdef _DEBUGMODE_CHECKS3
	for (int i = 0; i < NUMDRAMBANKS; i++){ globaldebugger_totalkeyvaluesstransferredtokernel += kvcount[i]; }
	#endif
	
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("PROCESS, PARTITION AND APPLY PHASE: Populate Kv DRAM Time Elapsed: ", begintime_populatekvdram, iteration_idx);
	#endif
	#ifdef LOCKE
	totaltime_populatekvdram_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_populatekvdram).count();
	#endif
	// exit(EXIT_SUCCESS);
	return;
}
#endif 
void actgraph_pr_sw::workerthread_generatekvs(unsigned int ddr, int threadidx, unsigned int subthreadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int * kvcount){
	vertex_t edgepropsz = utilityobj[threadidx]->hmin(KVDATA_BATCHSIZE, utilityobj[threadidx]->hsub((size_t)edgepropertyfilesize, (size_t)((size_t)((size_t)iteration_idx * (size_t)KVDATA_BATCHSIZE))));
	*kvcount = edge_process_obj[threadidx]->generateupdates_stream((iteration_idx * KVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((subthreadidxoffset + threadidx)), &runningvertexid[ddr], 
			edgepropsz, graphobj->getnvmeFd_edgeproperties_r2()[gbankoffset + lbankoffset + ddr][(subthreadidxoffset + subthreadidx)], graphobj->getnvmeFd_edgeoffsets_r2()[gbankoffset + lbankoffset + ddr][(subthreadidxoffset + subthreadidx)], vertexpropertiesbuffer[ddr], isactivevertexinfobuffer_source[ddr], kvdram[threadidx][ddr], kvstats[threadidx][ddr], (subthreadidxoffset + threadidx));
	return;			
}
void actgraph_pr_sw::printstructures(unsigned int threadidx){
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printkvdrams(kvdram[threadidx][ddr]); }	
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printstats(16, kvstats[threadidx][ddr]); }	
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printkvdramsII(kvdram[threadidx][ddr], kvstats[threadidx][ddr]); }
	return;
}
void actgraph_pr_sw::checkresultfromkernel(unsigned int threadidx, unsigned int totalnumkeyvalues[NUMDRAMBANKS]){
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->checkkvdrams((keyvalue_t *)kvdram[threadidx][ddr], kvstats[threadidx][ddr]); }
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->checkkvstats(kvstats[threadidx][ddr], totalnumkeyvalues[ddr]); }
	return;
}
void actgraph_pr_sw::loadverticesdatafromfile(int fd, unsigned int offset, keyvalue_t * kvdram, vertex_t bufferoffset, vertex_t size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "actgraph_pr_sw::loadverticesdatafromfile. loading vertices from file... offset: "<<offset<<", size: "<<size<<endl;
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile: offset: "<<offset<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile: size: "<<size<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile: offset: "<<offset<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile: (bufferoffset + size): "<<(bufferoffset + size)<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile: DRAMSZ: "<<(DRAMSZ_KVS * VECTOR_SIZE)<<endl;
	#endif 

	// if(size > 0){ if(pread(fd, &kvdram[bufferoffset], (size_t)(size * sizeof(value_t)), offset) < 0){ cout<<"actgraph_pr_sw::loadverticesdatafromfile::ERROR 35"<<endl; exit(EXIT_FAILURE); }}

	value_t * vertexdata = new value_t[size]; // FIXME.
	keyvalue_t * kvdramPtr = (keyvalue_t *)kvdram;
	if(size > 0){ if(pread(fd, vertexdata, (size_t)(size * sizeof(value_t)), (size_t)(offset * sizeof(value_t))) < 0){ cout<<"actgraph_pr_sw::loadverticesdatafromfile::ERROR 35"<<endl; exit(EXIT_FAILURE); }}
	for(unsigned int i=0; i<size; i++){ kvdramPtr[(bufferoffset + i)].value = vertexdata[i]; }
	delete [] vertexdata;

	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile. kvdram["<<(bufferoffset + 0)<<"].key: "<<kvdram[(bufferoffset + 0)].key<<", kvdram["<<(bufferoffset + 0)<<"].value: "<<kvdram[(bufferoffset + 0)].value<<endl;			
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile. kvdram["<<(bufferoffset + 1)<<"].key: "<<kvdram[(bufferoffset + 1)].key<<", kvdram["<<(bufferoffset + 1)<<"].value: "<<kvdram[(bufferoffset + 1)].value<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile. kvdram["<<(bufferoffset + 2)<<"].key: "<<kvdram[(bufferoffset + 2)].key<<", kvdram["<<(bufferoffset + 2)<<"].value: "<<kvdram[(bufferoffset + 2)].value<<endl;
	std::cout << endl;
	#endif
	return;
}
void actgraph_pr_sw::loadverticesdatafromfile(int fd, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "actgraph_pr_sw::loadverticesdatafromfile. loading vertices from file... offset: "<<offset<<", size: "<<size<<endl;
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile<buffer>: offset: "<<offset<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile<buffer>: size: "<<size<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile<buffer>: bufferoffset: "<<bufferoffset<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile<buffer>: (bufferoffset + size): "<<(bufferoffset + size)<<endl;
	#endif 
	
	if(size > 0){ if(pread(fd, &buffer[bufferoffset], (size_t)(size * sizeof(value_t)), (size_t)(offset * sizeof(value_t))) < 0){ cout<<"actgraph_pr_sw::loadverticesdatafromfile<buffer>::ERROR 35"<<endl; exit(EXIT_FAILURE); }}

	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile<buffer>: buffer[0]: "<<buffer[0]<<endl;			
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile<buffer>: buffer[1]: "<<buffer[1]<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile<buffer>: buffer[2]: "<<buffer[2]<<endl;
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile<buffer>: buffer["<<(size-1)<<"]: "<<buffer[(size-1)]<<endl;
	std::cout << endl;
	#endif 
	// exit(EXIT_SUCCESS);
	return;
}
void actgraph_pr_sw::writeverticesdatatofile(int fd, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "actgraph_pr_sw::writeverticesdatatofile. loading vertices from file... offset: "<<offset<<", size: "<<size<<endl;
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::writeverticesdatatofile<buffer>: offset: "<<offset<<endl;
	std::cout <<"actgraph_pr_sw::writeverticesdatatofile<buffer>: size: "<<size<<endl;
	std::cout <<"actgraph_pr_sw::writeverticesdatatofile<buffer>: bufferoffset: "<<bufferoffset<<endl;
	std::cout <<"actgraph_pr_sw::writeverticesdatatofile<buffer>: (bufferoffset + size): "<<(bufferoffset + size)<<endl;
	#endif 
	
	if(size > 0){ if(pwrite(fd, &buffer[bufferoffset], (size_t)(size * sizeof(value_t)), (size_t)(offset * sizeof(value_t))) < 0){ cout<<"actgraph_pr_sw::writeverticesdatatofile<buffer>::ERROR 35"<<endl; exit(EXIT_FAILURE); }}

	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::writeverticesdatatofile<buffer>: buffer[0]: "<<buffer[0]<<endl;			
	std::cout <<"actgraph_pr_sw::writeverticesdatatofile<buffer>: buffer[1]: "<<buffer[1]<<endl;
	std::cout <<"actgraph_pr_sw::writeverticesdatatofile<buffer>: buffer[2]: "<<buffer[2]<<endl;
	std::cout <<"actgraph_pr_sw::writeverticesdatatofile<buffer>: buffer["<<(size-1)<<"]: "<<buffer[(size-1)]<<endl;
	std::cout << endl;
	#endif 
	// exit(EXIT_SUCCESS);
	return;
}
void actgraph_pr_sw::loadvertexpropertiesfromfile(int fd, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "actgraph_pr_sw::loadvertexpropertiesfromfile. loading vertices from file... vertexprop offset: "<<bufferoffset<<", size: "<<size<<", offset: "<<offset<<endl;
	#endif 
	
	if(size > 0){ if(pread(fd, &buffer[bufferoffset], (size_t)(size * sizeof(vertexprop_t)), (size_t)(offset * sizeof(vertexprop_t))) < 0){ cout<<"actgraph_pr_sw::loadvertexpropertiesfromfile::ERROR 35"<<endl; exit(EXIT_FAILURE); }}

	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::loadvertexpropertiesfromfile. buffer[0].outdegree: "<<buffer[0].outdegree<<endl;
	std::cout <<"actgraph_pr_sw::loadvertexpropertiesfromfile. buffer[1].outdegree: "<<buffer[1].outdegree<<endl;
	if(size > 0){ std::cout <<"actgraph_pr_sw::loadvertexpropertiesfromfile. buffer["<<(size-1)<<"].outdegree: "<<buffer[size-1].outdegree<<endl; }
	#endif
	// exit(EXIT_SUCCESS);
	return;
}
void actgraph_pr_sw::writevertexpropertiestofile(int fd, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "actgraph_pr_sw::writevertexpropertiestofile. loading vertices from file... vertexprop offset: "<<bufferoffset<<", size: "<<size<<", offset: "<<offset<<endl;
	#endif 
	
	if(size > 0){ if(pwrite(fd, &buffer[bufferoffset], (size_t)(size * sizeof(vertexprop_t)), (size_t)(offset * sizeof(vertexprop_t))) < 0){ cout<<"actgraph_pr_sw::writevertexpropertiestofile::ERROR 35"<<endl; exit(EXIT_FAILURE); }}

	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::writevertexpropertiestofile. buffer[0].outdegree: "<<buffer[0].outdegree<<endl;
	std::cout <<"actgraph_pr_sw::writevertexpropertiestofile. buffer[1].outdegree: "<<buffer[1].outdegree<<endl;
	if(size > 0){ std::cout <<"actgraph_pr_sw::writevertexpropertiestofile. buffer["<<(size-1)<<"].outdegree: "<<buffer[size-1].outdegree<<endl; } 
	#endif
	// exit(EXIT_SUCCESS);
	return;
}
void actgraph_pr_sw::replicateverticesdata(keyvalue_t * kvdramA,keyvalue_t * kvdramB,keyvalue_t * kvdramC,keyvalue_t * kvdramD, unsigned int offset, unsigned int size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "actgraph_pr_sw::replicateverticesdata. replicating verticesdata across DRAMs"<<endl;
	#endif 
	memcpy(&kvdramA[offset], &kvdramA[offset], (size * sizeof(keyvalue_t)));
	memcpy(&kvdramB[offset], &kvdramA[offset], (size * sizeof(keyvalue_t)));
	memcpy(&kvdramC[offset], &kvdramA[offset], (size * sizeof(keyvalue_t)));
	memcpy(&kvdramD[offset], &kvdramA[offset], (size * sizeof(keyvalue_t)));
	return;
}
void actgraph_pr_sw::cummulateverticesdata(unsigned int threadidx, keyvalue_t * kvdramA,keyvalue_t * kvdramB,keyvalue_t * kvdramC,keyvalue_t * kvdramD, unsigned int offset, unsigned int size, utility * utilityobj){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "actgraph_pr_sw::cummulateverticesdata. commulatiing verticesdata across DRAMs"<<endl;
	#endif 
	keyy_t key;
	value_t value;
	for(unsigned int i=offset; i<(offset + size); i++){
		// cout<<"BEFORE: cummulateverticesdata: kvdramA["<<i<<"].key: "<<kvdramA[i].key<<endl;
		// cout<<"BEFORE: cummulateverticesdata: kvdramA["<<i<<"].value: "<<kvdramA[i].value<<endl;
		kvdramA[i].value = utilityobj->hmin(utilityobj->hmin(kvdramA[i].value, kvdramB[i].value), utilityobj->hmin(kvdramC[i].value, kvdramD[i].value));
		// cout<<"AFTER: cummulateverticesdata: kvdramA["<<i<<"].key: "<<kvdramA[i].key<<endl;
		// cout<<"AFTER: cummulateverticesdata: kvdramA["<<i<<"].value: "<<kvdramA[i].value<<endl;
		// if(i>(offset + 10)){ exit(EXIT_SUCCESS); }
		// exit(EXIT_SUCCESS);
	}
	return;
}
void actgraph_pr_sw::loadvertexisactiveinfosfromfile(int fd, unsigned int offset, unsigned int * buffer, vertex_t bufferoffset, vertex_t size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "actgraph_pr_sw::loadvertexisactiveinfosfromfile. loading vertices from file... vertexprop offset: "<<bufferoffset<<", size: "<<size<<", offset: "<<offset<<endl;
	#endif 
	
	if(size > 0){ if(pread(fd, &buffer[bufferoffset], (size_t)((size / NUMBITSINUNSIGNEDINT) * sizeof(unsigned int)), (size_t)((offset / NUMBITSINUNSIGNEDINT) * sizeof(unsigned int))) < 0){ cout<<"actgraph_pr_sw::loadvertexisactiveinfosfromfile::ERROR 35"<<endl; exit(EXIT_FAILURE); }}

	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"actgraph_pr_sw::loadvertexisactiveinfosfromfile. buffer[0]: "<<buffer[0]<<endl;
	std::cout <<"actgraph_pr_sw::loadvertexisactiveinfosfromfile. buffer[1]: "<<buffer[1]<<endl;
	if(size > 0){ std::cout <<"actgraph_pr_sw::loadvertexisactiveinfosfromfile. buffer["<<(size-1)<<"]: "<<buffer[size-1]<<endl; }
	#endif
	return;
}
void actgraph_pr_sw::applyvertices(unsigned int threadidx, int bankoffset, keyvalue_t * kvdram, vertex_t offset, vertex_t size){
	loadverticesdatafromfile(graphobj->getnvmeFd_verticesdata_r2()[(bankoffset + threadidx)], 0, verticesdatabuffer[threadidx], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((bankoffset + threadidx))); 
	
	#ifndef PR_ALGORITHM
	vector<keyvalue_t> activeverticesbuffer;
	#endif 
	for(unsigned int i=0; i<size; i++){
		keyvalue_t kvtempdata = kvdram[offset + i];
		value_t vdata = verticesdatabuffer[threadidx][i];
		value_t temp = algorithmobj[0]->apply(kvtempdata.value, vdata);
		verticesdatabuffer[threadidx][i] = temp;
		
		#ifndef PR_ALGORITHM
		if(temp != vdata){
			keyvalue_t kv;
			kv.key = i;
			kv.value = temp;
			#ifdef _DEBUGMODE_HOSTPRINTSX
			cout<<"actgraph_pr_sw::applyvertices:: active vertex found: kv.key: "<<kv.key<<", kv.value: "<<kv.value<<endl;
			#endif 
			activeverticesbuffer.push_back(kv);
			utilityobj[threadidx]->InsertBit(isactivevertexinfobuffer_dest[threadidx], i, 1);
		}
		#endif 
	}
	#ifdef _DEBUGMODE_HOSTPRINTSX
	cout<<"actgraph_pr_sw::applyvertices:: activeverticesbuffer.size(): "<<activeverticesbuffer.size()<<endl;
	for(unsigned int t=0; t<activeverticesbuffer.size(); t++){ cout<<"actgraph_pr_sw::applyvertices:: activeverticesbuffer["<<t<<"].key: "<<activeverticesbuffer[t].key<<", activeverticesbuffer["<<t<<"].value: "<<activeverticesbuffer[t].value<<endl; }
	#endif
	
	writeverticesdatatofile(graphobj->getnvmeFd_verticesdata_w2()[(bankoffset + threadidx)], 0, verticesdatabuffer[threadidx], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidx)));
	#ifndef PR_ALGORITHM
	graphobj->saveactiveverticestofile(graphobj->getnvmeFd_activevertexids_w(), activeverticesbuffer, (graph_iterationidx + 1));
	activeverticesbuffer.clear();
	#endif 
	return;
}
void actgraph_pr_sw::setgraphiteration(unsigned int _graph_iterationidx){ graph_iterationidx = _graph_iterationidx; }

#ifdef FPGA_IMPL
void actgraph_pr_sw::loadOCLstructures(std::string binaryFile){
	cl_int err;
	global = 1; local = 1;
	
	kvdramsz_kvs = DRAMSZ_KVS; 
	kv_size_bytes = kvdramsz_kvs * sizeof(uint512_vec_dt);
	kvstats_size_bytes = KVSTATS_SIZE * sizeof(metadata_t);
	messages_size_bytes = MESSAGES_SIZE * sizeof(int);
	
	kvV1_size_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV2_size_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV3_size_bytes = MAXNUMVERTICESPERBANK_KVS * sizeof(uint512_vec_dt);
	
	kvV1_offset_bytes = 0 * sizeof(uint512_vec_dt);
	kvV2_offset_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV3_offset_bytes = _x2PADDEDKVDATA_SIZE_KVS * sizeof(uint512_vec_dt);
	
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
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_kvdram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kv_size_bytes, kvdram[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvdram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_kvstats[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, kvstats[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvstats%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_messages[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, messages_size_bytes, messages[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_messages%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
	}
	return;
}
void actgraph_pr_sw::launchkernel(unsigned int flag){
	#ifdef LAUNCHKERNEL
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
	#endif
	// Set the Kernel Arguments. Using setArg(), i.e. setting kernel arguments, explicitly before enqueueMigrateMemObjects(), 
	// i.e. copying host memory to device memory,  allowing runtime to associate buffer with correct DDR banks automatically. 
	std::cout << "Set the Kernel Arguments" << std::endl;
	int nargs=0;
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvdram[flag][ddr]);
	}
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstats[flag][ddr]);
	}
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_messages[flag][ddr]);
	}
	
	// Copy input data to device global memory
	std::cout << "Copy input data to device global memory" << std::endl;
	array<cl_event, 12> write_events;
	
	// Copy data from Host to Device
	// std::chrono::steady_clock::time_point begintime_enqueuewrite = std::chrono::steady_clock::now();
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdram[flag][ddr], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, kvdram[flag][ddr], 0, NULL, &write_events[ddr] ));
	}
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstats[flag][ddr], CL_FALSE, 0, kvstats_size_bytes, kvstats[flag][ddr], 0, NULL, &write_events[4 + ddr] ));
	}
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_messages[flag][ddr], CL_FALSE, 0, messages_size_bytes, messages[flag][ddr], 0, NULL, &write_events[8 + ddr] ));
	}
	
	// Launch the Kernel			
	printf("Enqueueing NDRange kernel.\n");
	std::cout << "Launch the Kernel" << std::endl;
	std::vector<cl_event> waitList; 
	for(unsigned int i=0; i<12; i++){ waitList.push_back(write_events[i]); }
	OCL_CHECK(clEnqueueNDRangeKernel(world.command_queue, kernel, 1, nullptr,
									&global, &local, 12, waitList.data(),
									&kernel_events[flag]));
	
	// Copy Result from Device Global Memory to Host Local Memory
	std::cout << "Getting Results (Device to Host)..." << std::endl;
	printf("Enqueueing Migrate Mem Object (Device to Host) calls\n");
	utilityobj[0]->set_callback(kernel_events[flag], "ooo_queue");
	#ifdef LOCKE
	clWaitForEvents(1, &kernel_events[flag]); // REMOVEME
	#endif 
	// exit(EXIT_SUCCESS);
	
	// Copy input data from device global memory back to host
	// This operation only needs to wait for the kernel call. This call will
	// potentially overlap the next kernel call as well as the next read
	// operations
	#ifdef _DEBUGMODE_TIMERS
	actgraph_pr_sw_obj->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Top kernel Time Elapsed: ", begintime_topkernel, iteration_idx);
	#endif 
	#ifdef LOCKE
	totaltime_topkernel_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
	#endif
	#endif 
}
void actgraph_pr_sw::finishOCL(){
	cout<<"actgraph_pr_sw::finish destroying memory structures... "<<endl;
	for(unsigned int flag=0; flag<2; flag++){ clWaitForEvents(1, &kernel_events[flag]); }
	
	cout<<"actgraph_pr_sw::finish: releasing and destroying all OCL structures... "<<endl;
	#ifdef LAUNCHKERNEL
	clFlush(world.command_queue);
	clFinish(world.command_queue);
	for(int flag=0; flag<2; flag++){
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvdram[flag][ddr])); }
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvstats[flag][ddr])); }
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_messages[flag][ddr])); }
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
	cout<<"actgraph_pr_sw::finish: released and destroyed all OCL structures"<<endl;
	
	// OCL_CHECK(clReleaseKernel(kernel));
	// OCL_CHECK(clReleaseProgram(program));
	// OCL_CHECK(clReleaseCommandQueue(world.command_queue));
	// OCL_CHECK(clReleaseContext(world.context));
	// xcl_release_world(world);
	#endif 
}
unsigned int actgraph_pr_sw::getflag(unsigned int giteration_idx){
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
void actgraph_pr_sw::reloadOCLenv(xcl_world * _world, cl_program * _program, cl_kernel * _kernel){
	world = *_world;
	program = *_program;
	kernel = *_kernel;
	return; 
}
void actgraph_pr_sw::allocateOCLbuffers(){
	cl_int err;
	global = 1; local = 1;
	
	kvdramsz_kvs = DRAMSZ_KVS; 
	kv_size_bytes = kvdramsz_kvs * sizeof(uint512_vec_dt);
	kvstats_size_bytes = KVSTATS_SIZE * sizeof(metadata_t);
	messages_size_bytes = MESSAGES_SIZE * sizeof(int);
	
	kvV1_size_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV2_size_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV3_size_bytes = MAXNUMVERTICESPERBANK_KVS * sizeof(uint512_vec_dt);
	
	kvV1_offset_bytes = 0 * sizeof(uint512_vec_dt);
	kvV2_offset_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV3_offset_bytes = _x2PADDEDKVDATA_SIZE_KVS * sizeof(uint512_vec_dt);
	
	// Allocate Buffer in Global Memory
	for(int flag=0; flag<2; flag++){
		std::cout << "Allocate Buffer in Global Memory" << std::endl;
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_kvdram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kv_size_bytes, kvdram[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvdram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_kvstats[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, kvstats[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvstats%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_messages[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, messages_size_bytes, messages[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_messages%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
	}
}
#endif 


