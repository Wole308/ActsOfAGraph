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
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
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
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvsourcedram[i][0][ddr] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdestdram[i][0][ddr] = new uint512_vec_dt[KVDATA_RANGE_PERSSDPARTITION_KVS]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][0][ddr] = new keyvalue_t[KVSTATSDRAMSZ]; }
	}
	#endif
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvsourcedram[i][flag][ddr] = (uint512_vec_dt *) aligned_alloc(4096, (KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt))); }}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdestdram[i][flag][ddr] = (uint512_vec_dt *) aligned_alloc(4096, (KVDATA_RANGE_PERSSDPARTITION_KVS * sizeof(uint512_vec_dt))); }}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][flag][ddr] = (keyvalue_t *) aligned_alloc(4096, (KVSTATSDRAMSZ * sizeof(keyvalue_t))); }}
	}
	#endif
	
	totaltime_topkernel_ms = 0;
	totaltime_populatekvdram_ms = 0;
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ algorithmobj[i] = new algorithm(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edge_process_obj[i] = new edge_process(_graphobj); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edge_process_obj[i]->settime_ssdaccesses_ms(0); }
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
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ settime_OCLdatatransfers_ms(i, 0); }
}
actgraph_pr_sw::actgraph_pr_sw(){}
actgraph_pr_sw::~actgraph_pr_sw(){
	cout<<"actgraph_pr_sw::~actgraph_pr_sw:: finish destroying memory structures... "<<endl;
	#ifdef FPGA_IMPL
	finishOCL();
	#endif 
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){		
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvsourcedram[i][flag][ddr]; }}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvdestdram[i][flag][ddr]; }}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvstats[i][flag][ddr]; }}
	}
}

void actgraph_pr_sw::run(){
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
		cout<<"actgraph_pr_sw::run: graph iteration "<<graph_iterationidx<<" of pagerank Started"<<endl;
		start2(graph_iterationidx);
	}
	utilityobj[0]->stopTIME("actgraph_pr_sw::start2: finished start2. Time Elapsed: ", begintime, NAp);
	float totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	timingandsummary(graph_iterationidx, totaltime_ms);
	return;
}
/** start2: Temp vertices data is fixed while Vertex properties are loaded on demand till all banks are processed */
runsummary_t actgraph_pr_sw::start2(unsigned int graph_iterationidx) {
	float totaltime_ms = 0;

	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totalnumkvsread[i] = 0; }
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int i_batch=0; i_batch<graphobj->getnumvertexbanks(); i_batch += NUMCPUTHREADS){
		cout<<">>> actgraph_pr_sw::start2: super iteration: [i_batch: "<<i_batch<<"][size: "<<graphobj->getnumvertexbanks()<<"][step: "<<NUMCPUTHREADS<<"]"<<endl;
		cout<<">>> actgraph_pr_sw::start2: loading vertex properties..."<<endl;
	
		#ifdef SW
		for (int i = 0; i < NUMCPUTHREADS; i++) { WorkerThread2(i, i_batch); }
		#else 
		for (int i = 0; i < NUMCPUTHREADS; i++) { WorkerThread2(i, i_batch); }
		#endif
		
		cout<<">>> actgraph_pr_sw::start2 Finished: all threads joined..."<<endl;
	}
	utilityobj[0]->stopTIME("actgraph_pr_sw::start2: finished start2. Time Elapsed: ", begintime, NAp);
	totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	finish();
	return timingandsummary(graph_iterationidx, totaltime_ms);
}
void actgraph_pr_sw::reloadenv(){ return; }
void actgraph_pr_sw::finish(){
	#if (defined(FPGA_IMPL) && defined(PR_ALGORITHM))
	finishOCL();
	#endif
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL KERNEL TIME: "<<totalkerneltime()<<" milli seconds"<< RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> TIMING SUMMARY: TOTAL KERNEL TIME: "<<((totalkerneltime()) / 1000)<<" seconds"<< RESET <<endl;
}
runsummary_t actgraph_pr_sw::timingandsummary(unsigned int graph_iterationidx, float totaltime_ms){
	cout<<"=== ACTGRAPH_PR::TIMING AND SUMMARY RESULTS FOR ITERATION: "<<graph_iterationidx<<" === "<<endl;
	float totaltime_overallexcludingOCLandSSDtransfers_ms = totaltime_ms;
	float totaltime_SSDtransfers_ms = 0;
	float totaltime_OCLtransfers_ms = 0;
	
	for (int i = 0; i < NUMCPUTHREADS; i++){ cout<<">>> actgraph_pr_sw::summary Total number of kvReads for thread ["<<i<<"]: "<<totalnumkvsread[i]<<endl; }
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_pr_sw::summary Total number of kvReads for all threads: "<<gettotalsize()<< RESET <<endl;
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totaltime_SSDtransfers_ms += edge_process_obj[i]->gettime_SSDaccesses_ms(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totaltime_OCLtransfers_ms += gettime_OCLdatatransfers_ms(i); }
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_pr_sw::summary Total time spent (SSD access): "<< totaltime_SSDtransfers_ms << " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_pr_sw::summary Total time spent (OCL data transfers): "<< totaltime_OCLtransfers_ms << " milliseconds" << RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_pr_sw::summary Total time spent (Overall processing): "<<totaltime_ms<< " milliseconds" << RESET <<endl;
	totaltime_overallexcludingOCLandSSDtransfers_ms -= (totaltime_SSDtransfers_ms + totaltime_OCLtransfers_ms);
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_pr_sw::summary Total time spent (Overall processing excluding SSD & OCL data transfers): "<<totaltime_overallexcludingOCLandSSDtransfers_ms<< " milliseconds" << RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_pr_sw::summary Throughput (Overall processing): "<<(float)gettotalsize() / (float)(totaltime_ms / 1000)<<" keyvalues per second"<< RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> actgraph_pr_sw::summary Throughput (Overall processing excluding SSD & OCL data transfers): "<<(float)gettotalsize() / (float)(totaltime_overallexcludingOCLandSSDtransfers_ms / 1000)<<" keyvalues per second"<< RESET <<endl;
	cout<<endl;
	
	runsummary_t runsummary; 
	runsummary.totalsize = gettotalsize(); 
	runsummary.totaltime_SSDtransfers_ms = totaltime_SSDtransfers_ms;
	runsummary.totaltime_OCLtransfers_ms = totaltime_OCLtransfers_ms;
	runsummary.totaltime_ms = totaltime_ms;
	runsummary.totaltime_overallexcludingOCLandSSDtransfers_ms = totaltime_overallexcludingOCLandSSDtransfers_ms;
	return runsummary;
}
unsigned int actgraph_pr_sw::gettotalsize(){
	unsigned int totalsize = 0;
	for (int i = 0; i < NUMCPUTHREADS; i++){ totalsize += totalnumkvsread[i]; }
	return totalsize;
}
float actgraph_pr_sw::totalkerneltime(){ return totaltime_topkernel_ms; }
float actgraph_pr_sw::totalpopulateKvDRAMtime(){ return totaltime_populatekvdram_ms; }

void actgraph_pr_sw::WorkerThread2(int threadidx, int threadidxoffset){
	#ifdef LOCKE
	mutex1_workthread.lock();
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_partitionandapply = std::chrono::steady_clock::now();
	#endif
	unsigned int giteration_idx = 0;
	
	// load temp vertices data
	loadverticesdatafromfile(graphobj->getnvmeFd_verticesdata_r2()[(threadidxoffset + threadidx)], 0, (keyvalue_t *)kvdestdram[threadidx][0][0], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)));
	replicateverticesdata((keyvalue_t *)kvdestdram[threadidx][0][0],(keyvalue_t *)kvdestdram[threadidx][0][1],(keyvalue_t *)kvdestdram[threadidx][0][2],(keyvalue_t *)kvdestdram[threadidx][0][3], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)));
	#ifdef FPGA_IMPL
	writeVstokernel(threadidx);
	#endif 
	
	for(unsigned int lbankoffset = 0; lbankoffset < graphobj->getnumedgebanks(); lbankoffset += NUMDRAMBANKS){
		edge_t edgepropertyfilesize[NUMDRAMBANKS];
		unsigned int _iteration_size[NUMDRAMBANKS];	
		#ifdef STREAMEDGESSETUP
		size_t sizeofedgeprop = sizeof(edgeprop1_t);
		#else 
		size_t sizeofedgeprop = sizeof(edgeprop2_t);
		#endif 
		
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
		for(unsigned int i=0; i<NUMDRAMBANKS; i++){ cout<<"actgraph_pr_sw::WorkerThread: edgepropertyfilesize["<<i<<"]: "<<edgepropertyfilesize[i]<<", numverticespervertexbank: "<<utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_(0)<<", threadidx: "<<(threadidxoffset + threadidx)<<endl; }			
		#endif
		
		unsigned int runningvertexid[NUMDRAMBANKS];
		unsigned int kvcount[NUMDRAMBANKS]; for (int i = 0; i < NUMDRAMBANKS; i++){ kvcount[i] = 0; }
		unsigned int keyvaluecount[NUMDRAMBANKS]; for (int i = 0; i < NUMDRAMBANKS; i++){ keyvaluecount[i] = 0; }
		for(unsigned int i=0; i<NUMDRAMBANKS; i++){ runningvertexid[i] = utilityobj[threadidx]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((threadidxoffset + threadidx)); }
		for(int flag = 0; flag < NUMFLAGS; flag++){ for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->resetkeyvalues("actgraph_pr_sw:: resetting messages", &kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM], MESSAGES_SIZE); }} 
		
		for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx++){
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"PP&A:: [threadidx:"<<threadidx<<"][size:"<<NUMCPUTHREADS<<"][step:1], [threadidx:"<<(threadidxoffset + threadidx)<<"][size:"<<(graphobj->getnumedgebanks() / NUMCPUTHREADS)<<"][step:1], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
			#endif 
			#ifdef _DEBUGMODE_TIMERS
			std::chrono::steady_clock::time_point begintime_iterationidx = std::chrono::steady_clock::now();
			#endif
			
			int flag = getflag(giteration_idx);
			
			// Populate kvdrams
			generatekvs(threadidx, flag, threadidxoffset, threadidx, 0, lbankoffset, edgepropertyfilesize, runningvertexid, iteration_idx, iteration_size, kvcount, keyvaluecount);
			
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"Host::Process, Partition and Apply phase:: Print results before Kernel run "<<endl;
			printstructures(threadidx, flag);
			#endif
			
			// Launch the Kernel
			#ifdef LAUNCHKERNEL
			#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
			std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
			#endif
			#ifdef SW
			kernel_process[threadidx]->topkernelMW((uint512_dt **)kvsourcedram[threadidx][flag], (uint512_dt **)kvdestdram[threadidx][flag], (keyvalue_t **)kvstats[threadidx][flag]);
			#else
			launchkernel(threadidx, flag);
			#endif 
			#ifdef LOCKE
			totaltime_topkernel_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
			#endif
			#endif
			
			// settle updates
			settleupdates(threadidx, flag, iteration_idx, iteration_size);
			
			// Print results after Kernel run
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"Host::Process, Partition and Apply phase:: Print results after Kernel run "<<endl;
			printstructures(threadidx, flag);
			#endif
			#ifdef _DEBUGMODE_CHECKSX
			utilityobj[threadidx]->printtempverticesdata(kvsourcedram[threadidx][flag][0]);
			utilityobj[threadidx]->printtempverticesdata(kvsourcedram[threadidx][flag][1]);
			utilityobj[threadidx]->printtempverticesdata(kvsourcedram[threadidx][flag][2]);
			utilityobj[threadidx]->printtempverticesdata(kvsourcedram[threadidx][flag][3]);
			#endif
	
			#ifdef _DEBUGMODE_CHECKSX
			utilityobj[threadidx]->recordstats(kvstats[threadidx][flag][0]);
			utilityobj[threadidx]->recordstats(kvstats[threadidx][flag][1]);
			utilityobj[threadidx]->recordstats(kvstats[threadidx][flag][2]);
			utilityobj[threadidx]->recordstats(kvstats[threadidx][flag][3]);
			#endif
			giteration_idx += 1;
			
			#ifdef _DEBUGMODE_TIMERS
			utilityobj[threadidx]->stopTIME("PROCESS, PARTITION AND APPLY PHASE: Iteration Time Elapsed: ", begintime_iterationidx, iteration_idx);
			#endif
		}
	}

	// writeback temp vertices data
	#ifndef PR_ALGORITHM
	for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ graphobj->savevertexisactiveinfostofile(graphobj->getnvmeFd_vertexisactive_w2(), utilityobj[0]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((threadidxoffset + threadidx)), isactivevertexinfobuffer_dest[threadidx], 0, utilityobj[0]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)), (graph_iterationidx + 1)); }
	#endif
	#ifdef FPGA_IMPL
	readVsfromkernel(threadidx);
	#endif 
	cummulateverticesdata(threadidx, (keyvalue_t *)kvdestdram[threadidx][0][0],(keyvalue_t *)kvdestdram[threadidx][0][1],(keyvalue_t *)kvdestdram[threadidx][0][2],(keyvalue_t *)kvdestdram[threadidx][0][3], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)), utilityobj[threadidx]);
	applyvertices(threadidx, threadidxoffset, (keyvalue_t *)kvdestdram[threadidx][0][0], 0, utilityobj[threadidx]->GETKVDATA_RANGE_FORSSDPARTITION_((threadidxoffset + threadidx)));
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[threadidx]->stopTIME("Total time elapsed (Entire Process, Partition & Apply process): ", begintime_partitionandapply, NAp);
	#endif
	#ifdef LOCKE
	mutex1_workthread.unlock();
	#endif 
}

#ifdef SW
void actgraph_pr_sw::generatekvs(int threadidx, unsigned int flag, unsigned int threadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t * edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]){
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
	#endif
	
	for (int i = 0; i < NUMDRAMBANKS; i++){ kvcount[i] = 0; }
	// for (int i = 0; i < NUMDRAMBANKS; i++){ workerthread_generatekvs(i, flag, threadidx, threadidxoffset, subthreadidx, gbankoffset, lbankoffset, edgepropertyfilesize[i], runningvertexid, iteration_idx, iteration_size, kvcount, keyvaluecount); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ genw_thread[threadidx][i] = std::thread(&actgraph_pr_sw::workerthread_generatekvs, this, i, flag, threadidx, threadidxoffset, subthreadidx, gbankoffset, lbankoffset, edgepropertyfilesize[i], runningvertexid, iteration_idx, iteration_size, kvcount, keyvaluecount); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ genw_thread[threadidx][i].join(); }
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
void actgraph_pr_sw::generatekvs(int threadidx, unsigned int flag, unsigned int threadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t * edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]){
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
	#endif
	
	for (int i = 0; i < NUMDRAMBANKS; i++){ kvcount[i] = 0; }
	for (int i = 0; i < NUMDRAMBANKS; i++){ genw_thread[threadidx][i] = std::thread(&actgraph_pr_sw::workerthread_generatekvs, this, i, flag, threadidx, threadidxoffset, subthreadidx, gbankoffset, lbankoffset, edgepropertyfilesize[i], runningvertexid, iteration_idx, iteration_size, kvcount, keyvaluecount); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ genw_thread[threadidx][i].join(); }
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
void actgraph_pr_sw::workerthread_generatekvs(unsigned int ddr, unsigned int flag, int threadidx, unsigned int threadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t edgepropertyfilesize, unsigned int * runningvertexid, unsigned int iteration_idx, unsigned int iteration_size, unsigned int kvcount[NUMDRAMBANKS], unsigned int keyvaluecount[NUMDRAMBANKS]){
	vertex_t edgepropsz = utilityobj[threadidx]->hmin(KVDATA_BATCHSIZE, utilityobj[threadidx]->hsub((size_t)edgepropertyfilesize, (size_t)((size_t)((size_t)iteration_idx * (size_t)KVDATA_BATCHSIZE))));
	unsigned int returnkvcount = edge_process_obj[threadidx]->generateupdates_stream((iteration_idx * KVDATA_BATCHSIZE), utilityobj[threadidx]->GETKVDATA_RANGEOFFSET_FORSSDPARTITION_((threadidxoffset + threadidx)), &runningvertexid[ddr], 
			edgepropsz, graphobj->getnvmeFd_edgeproperties_r2()[gbankoffset + lbankoffset + ddr][(threadidxoffset + subthreadidx)], graphobj->getnvmeFd_edgeoffsets_r2()[gbankoffset + lbankoffset + ddr][(threadidxoffset + subthreadidx)], vertexpropertiesbuffer[ddr], isactivevertexinfobuffer_source[ddr], kvsourcedram[threadidx][flag][ddr], kvstats[threadidx][flag][ddr], (threadidxoffset + threadidx));				
	
	kvcount[ddr] = returnkvcount;
	keyvaluecount[ddr] += returnkvcount;
	
	if((utilityobj[threadidx]->runActs(iteration_idx) == 1) || (iteration_idx == (iteration_size-1))){
		#ifdef FPGA_IMPL 
		#ifdef _DEBUGMODE_CHECKS
		if(keyvaluecount[ddr] >= KVDRAMSZ){ cout<<"ERROR: out of bounds 34. *keyvaluecount: "<<keyvaluecount[ddr]<<endl; }
		#endif 
		#endif 
		kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNKERNELCOMMANDID].key = ON;
		kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_BATCHSIZE].key = returnkvcount;
		kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNSIZE].key = keyvaluecount[ddr]; 
		kvstats[threadidx][flag][ddr][BASEOFFSET_STATSDRAM + 0].value = keyvaluecount[ddr]; 
		cout<<"...running Acts... size: "<<keyvaluecount[ddr]<<endl; 
		keyvaluecount[ddr] = 0;
	} else { 
		kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNKERNELCOMMANDID].key = OFF; 
		kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_BATCHSIZE].key = returnkvcount; 
		kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNSIZE].key = 0;
		kvstats[threadidx][flag][ddr][BASEOFFSET_STATSDRAM + 0].value = 0; 
		cout<<"...loading KvDRAM... size: "<<returnkvcount<<endl; 
	}
	
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_COMMANDID].key = NAp;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_PROCESSCOMMANDID].key = OFF;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_PARTITIONCOMMANDID].key = ON;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_APPLYUPDATESCOMMANDID].key = ON;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_KVDATASIZEID].key = KVDATA_BATCHSIZE;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_FINALNUMPARTITIONSID].key = NAp;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_TREEDEPTHID].key = NAp;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_ITERATIONID].key = iteration_idx;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_SSDPARTITIONID].key = (threadidxoffset + threadidx);
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_VOFFSET].key = 0;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_VSIZE].key = NAp;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_TREEDEPTH].key = TREE_DEPTH;
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_FINALNUMPARTITIONS].key = pow(NUM_PARTITIONS, TREE_DEPTH);
	kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_GRAPHITERATIONID].key = graph_iterationidx;
	return;
}
void actgraph_pr_sw::settleupdates(int threadidx, unsigned int flag, unsigned int iteration_idx, unsigned int iteration_size){
	if((utilityobj[threadidx]->runActs(iteration_idx) == 1) || (iteration_idx == (iteration_size-1))){
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_NEXTBATCHOFFSET].key = 0; 
		}
		// exit(EXIT_SUCCESS);
	} else {
		for(int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			utilityobj[threadidx]->appenddata(&kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_NEXTBATCHOFFSET].key, kvstats[threadidx][flag][ddr][BASEOFFSET_MESSAGESDRAM + MESSAGES_BATCHSIZE].key); 
		}
	}
	return;			
}

void actgraph_pr_sw::printstructures(unsigned int threadidx, unsigned int flag){
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printkvdrams(kvsourcedram[threadidx][flag][ddr]); }	
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printstats(16, kvstats[threadidx][flag][ddr]); }	
	// for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->printkvdramsII(kvsourcedram[threadidx][flag][ddr], kvstats[threadidx][flag][ddr]); }
	return;
}
void actgraph_pr_sw::checkresultfromkernel(unsigned int threadidx, unsigned int flag, unsigned int totalnumkeyvalues[NUMDRAMBANKS]){
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->checkkvdrams((keyvalue_t *)kvsourcedram[threadidx][flag][ddr], kvstats[threadidx][flag][ddr]); }
	for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){ utilityobj[threadidx]->checkkvstats(kvstats[threadidx][flag][ddr], totalnumkeyvalues[ddr]); }
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
	std::cout <<"actgraph_pr_sw::loadverticesdatafromfile: DRAMSZ: "<<(KVSOURCEDRAMSZ_KVS * VECTOR_SIZE)<<endl;
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
		kvdramA[i].value = utilityobj->hmin(utilityobj->hmin(kvdramA[i].value, kvdramB[i].value), utilityobj->hmin(kvdramC[i].value, kvdramD[i].value));
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
unsigned int actgraph_pr_sw::getflag(unsigned int giteration_idx){
	#ifdef FPGA_IMPL
	// wait for events to finish
	int flag = giteration_idx % 2;
	#ifdef LAUNCHKERNEL
	if (giteration_idx >= 2) {
		cout<<"actgraph_pr_fpga::WorkerThread: waiting for events..."<<endl;
		clWaitForEvents(1, &kernel_events[flag]);
	}
	#endif
	#else 
	int flag = 0;
	#endif 
	return flag;
}
void actgraph_pr_sw::settime_OCLdatatransfers_ms(unsigned int threadidx, float value){
	totaltime_OCLdatatransfers[threadidx] = value;
}
float actgraph_pr_sw::gettime_OCLdatatransfers_ms(unsigned int threadidx){
	return totaltime_OCLdatatransfers[threadidx];
}

#ifdef FPGA_IMPL
void actgraph_pr_sw::loadOCLstructures(std::string binaryFile){
	cl_int err;
	global = 1; local = 1;
	
	kvsource_size_bytes = KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
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
	for (int threadidx = 0; threadidx < NUMCPUTHREADS; threadidx++) {
		for(int flag=0; flag<2; flag++){
			std::cout << "Allocate Buffer in Global Memory" << std::endl;
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
				buffer_kvsourcedram[threadidx][flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvsource_size_bytes, kvsourcedram[threadidx][flag][ddr], &err);
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvsourcedram[%i][%i][%i] buffer arg. %i \n", threadidx, flag, ddr, err);
					exit(EXIT_FAILURE);
				}
			}
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
				buffer_kvdestdram[threadidx][flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvdest_size_bytes, kvdestdram[threadidx][flag][ddr], &err);
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvdestdram[%i][%i][%i] buffer arg. %i \n", threadidx, flag, ddr, err);
					exit(EXIT_FAILURE);
				}
			}
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
				buffer_kvstatsdram[threadidx][flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, kvstats[threadidx][flag][ddr], &err);
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvstatsdram[%i][%i][%i] buffer arg. %i \n", threadidx, flag, ddr, err);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	return;
}
void actgraph_pr_sw::writeVstokernel(unsigned int threadidx){
	#ifdef LAUNCHKERNEL
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_writeVstokernel = std::chrono::steady_clock::now();
	#endif
	
	std::cout << "Copy input data to kernel global memory" << std::endl;
	#ifdef CONFIG_FACTOROUTOCLDATATRANSFERS
	std::chrono::steady_clock::time_point begintime_OCLdatatransfers = std::chrono::steady_clock::now();
	#endif
	array<cl_event, 1> write_events;
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdestdram[threadidx][0][ddr], CL_TRUE, 0, kvdest_size_bytes, kvdestdram[threadidx][0][ddr], 0, NULL, &write_events[ddr] ));
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ clWaitForEvents(1, &write_events[ddr]); }
	#ifdef CONFIG_FACTOROUTOCLDATATRANSFERS
	totaltime_OCLdatatransfers[threadidx] += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_OCLdatatransfers).count();
	#endif
	
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[0]->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Write Vertices to Kernel Time Elapsed: ", begintime_writeVstokernel, NAp);
	#endif 
	#endif 
}
void actgraph_pr_sw::launchkernel(unsigned int threadidx, unsigned int flag){
	#ifdef LAUNCHKERNEL
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
	#endif
	
	// Set the Kernel Arguments. Using setArg(), i.e. setting kernel arguments, explicitly before enqueueMigrateMemObjects(), 
	// i.e. copying host memory to device memory,  allowing runtime to associate buffer with correct DDR banks automatically. 
	std::cout << "Set the Kernel Arguments" << std::endl;
	int nargs=0;
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), (void *) &buffer_kvsourcedram[threadidx][flag][ddr]);
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), (void *) &buffer_kvdestdram[threadidx][flag][ddr]);
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), (void *) &buffer_kvstatsdram[threadidx][flag][ddr]);
	}
	
	// Copy input data to device global memory
	std::cout << "Copy input data to device global memory" << std::endl;
	array<cl_event, 3> write_events;
	
	// Copy data from Host to Device
	#ifdef CONFIG_FACTOROUTOCLDATATRANSFERS
	cl_bool CLBOOL = CL_TRUE;
	#else 
	cl_bool CLBOOL = CL_FALSE;
	#endif
	#ifdef CONFIG_FACTOROUTOCLDATATRANSFERS
	std::chrono::steady_clock::time_point begintime_OCLdatatransfers = std::chrono::steady_clock::now();
	#endif 
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvsourcedram[threadidx][flag][ddr], CLBOOL, 0, kvsource_size_bytes, kvsourcedram[threadidx][flag][ddr], 0, NULL, &write_events[ddr] ));
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdestdram[threadidx][flag][ddr], CLBOOL, 0, 64, kvdestdram[threadidx][flag][ddr], 0, NULL, &write_events[1 + ddr] ));
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstatsdram[threadidx][flag][ddr], CLBOOL, 0, kvstats_size_bytes, kvstats[threadidx][flag][ddr], 0, NULL, &write_events[1 + 1 + ddr] ));
	}
	#ifdef CONFIG_FACTOROUTOCLDATATRANSFERS
	totaltime_OCLdatatransfers[threadidx] += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_OCLdatatransfers).count();
	#endif
	
	// Launch the Kernel			
	printf("Enqueueing NDRange kernel.\n");
	std::cout << "Launch the Kernel" << std::endl;
	std::vector<cl_event> waitList; 
	for(unsigned int i=0; i<1 * NUM_KAPI; i++){ waitList.push_back(write_events[i]); }
	OCL_CHECK(clEnqueueNDRangeKernel(world.command_queue, kernel, 1, nullptr,
									&global, &local, 1 * NUM_KAPI, waitList.data(),
									&kernel_events[flag]));
	
	// Copy Result from Device Global Memory to Host Local Memory
	std::cout << "Getting Results (Device to Host)..." << std::endl;
	printf("Enqueueing Migrate Mem Object (Device to Host) calls\n");
	utilityobj[0]->set_callback(kernel_events[flag], "ooo_queue");
	#ifdef LOCKE
	clWaitForEvents(1, &kernel_events[flag]);
	#endif
	
	// Copy input data from device global memory back to host
	// This operation only needs to wait for the kernel call. This call will
	// potentially overlap the next kernel call as well as the next read
	// operations
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[0]->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Top kernel Time Elapsed: ", begintime_topkernel, NAp);
	#endif 
	#ifdef LOCKE
	totaltime_topkernel_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
	#endif
	#endif 
}
void actgraph_pr_sw::readVsfromkernel(unsigned int threadidx){
	#ifdef LAUNCHKERNEL
	#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
	std::chrono::steady_clock::time_point begintime_kernelwriteback = std::chrono::steady_clock::now();
	#endif
	
	std::cout << "Copy input data to host global memory" << std::endl;
	#ifdef CONFIG_FACTOROUTOCLDATATRANSFERS
	std::chrono::steady_clock::time_point begintime_OCLdatatransfers = std::chrono::steady_clock::now();
	#endif 
	array<cl_event, 1> write_events;
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdestdram[threadidx][0][ddr], CL_TRUE, 0, kvdest_size_bytes, kvdestdram[threadidx][0][ddr], 0, NULL, &write_events[ddr] ));
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ clWaitForEvents(1, &write_events[ddr]); }
	#ifdef CONFIG_FACTOROUTOCLDATATRANSFERS
	totaltime_OCLdatatransfers[threadidx] += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_OCLdatatransfers).count();
	#endif
	
	#ifdef _DEBUGMODE_TIMERS
	utilityobj[0]->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Read Vertices from kernel Time Elapsed: ", begintime_kernelwriteback, NAp);
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
	for (int threadidx = 0; threadidx < NUMCPUTHREADS; threadidx++) {
		for(int flag=0; flag<2; flag++){
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvsourcedram[threadidx][flag][ddr])); }
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvdestdram[threadidx][flag][ddr])); }
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvstatsdram[threadidx][flag][ddr])); }
		}
		for(int flag=0; flag<2; flag++){ clWaitForEvents(1, &kernel_events[flag]); }
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
	cout<<"actgraph_pr_sw::finish: released and destroyed all OCL structures"<<endl;
	#endif 
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
	
	kvsource_size_bytes = KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvdest_size_bytes = KVDATA_RANGE_PERSSDPARTITION_KVS * sizeof(uint512_vec_dt);
	kvstats_size_bytes = KVSTATSDRAMSZ * sizeof(keyvalue_t);

	// Allocate Buffer in Global Memory
	for (int threadidx = 0; threadidx < NUMCPUTHREADS; threadidx++) {
		for(int flag=0; flag<2; flag++){
			std::cout << "[actgraph_pr_sw] Allocate Buffer in Global Memory" << std::endl;
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
				buffer_kvsourcedram[threadidx][flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvsource_size_bytes, kvsourcedram[threadidx][flag][ddr], &err);
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvsourcedram%i[%i] buffer arg. %i \n", ddr, flag, err);
					exit(EXIT_FAILURE);
				}
			}
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
				buffer_kvdestdram[threadidx][flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvdest_size_bytes, kvdestdram[threadidx][flag][ddr], &err);
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvdestdram%i[%i] buffer arg. %i \n", ddr, flag, err);
					exit(EXIT_FAILURE);
				}
			}
			for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
				buffer_kvstatsdram[threadidx][flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, kvstats[threadidx][flag][ddr], &err);
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvstatsdram%i[%i] buffer arg. %i \n", ddr, flag, err);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}
#endif 


