#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../src/stats/stats.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../include/common.h"
#include "pagerank.h"
using namespace std;

pagerank::pagerank(unsigned int algorithmid, unsigned int datasetid){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumvertexbanks(), heuristicsobj->getdefaultnumedgebanks());
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ edgeprocessobj[i] = new edge_process(graphobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i] = new helperfunctions(graphobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ dataaccessobj[i] = new dataaccess(); }
	
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){
		#ifdef FPGA_IMPL
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvsourcedram[i][flag][j][k] = (uint512_vec_dt *) aligned_alloc(4096, (KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt))); }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvdestdram[i][flag][j][k] = (uint512_vec_dt *) aligned_alloc(4096, (BATCH_RANGE_KVS * sizeof(uint512_vec_dt))); }}} // KVDATA_RANGE_PERSSDPARTITION_KVS
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvstats[i][flag][j][k] = (keyvalue_t *) aligned_alloc(4096, (KVSTATSDRAMSZ * sizeof(keyvalue_t))); }}}
		#else 
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvsourcedram[i][flag][j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvdestdram[i][flag][j][k] = new uint512_vec_dt[BATCH_RANGE_KVS]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvstats[i][flag][j][k] = new keyvalue_t[KVSTATSDRAMSZ]; }}}
		#endif
	}
}
pagerank::~pagerank(){
	cout<<"pagerank::~pagerank:: finish destroying memory structures... "<<endl;
	#ifdef FPGA_IMPL
	finishOCL();
	#endif 
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){		
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvsourcedram[i][flag][j][k]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvdestdram[i][flag][j][k]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvstats[i][flag][j][k]; }}}
	}
}

runsummary_t pagerank::run() {
	graphobj->openfilesforreading();
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	graphobj->generateverticesdata();
	graphobj->generatevertexproperties();
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	graphobj->loadvertexpropertiesfromfile();
	graphobj->loadvertexdatafromfile();
	
	for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
		cout<< TIMINGRESULTSCOLOR <<">>> pagerank::run: graph iteration "<<graph_iterationidx<<" of pagerank started"<< RESET <<endl;
		
		for(unsigned int i_batch=0; i_batch<graphobj->getnumvertexbanks(); i_batch += NUMSUPERCPUTHREADS){
			cout<<">>> pagerank::start2: super iteration: [i_batch: "<<i_batch<<"][size: "<<graphobj->getnumvertexbanks()<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<<endl;
			for (int i = 0; i < NUMSUPERCPUTHREADS; i++) { WorkerThread(i, i_batch, graph_iterationidx); }
			cout<<">>> pagerank::start2 Finished: all threads joined..."<<endl;
			break;
		}
	}
	
	utilityobj[0]->stopTIME("pagerank::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closefilesforreading();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	
	return edgeprocessobj[0]->timingandsummary(NAp, totaltime_ms);
}
void pagerank::WorkerThread(int threadidx, int threadidxoffset, unsigned int graph_iterationidx){
	unsigned int globaliteration_idx = 0;
	unsigned int voffset = 0; // (threadidxoffset + threadidx) * KVDATA_RANGE_PERSSDPARTITION;
	edgeprocessobj[threadidx]->resetstatevariables();
	
	graphobj->loadvertexdatafromfile(threadidxoffset + threadidx, 0, (keyvalue_t *)kvdestdram[threadidx][0][0][0], 0, KVDATA_RANGE_PERSSDPARTITION);
	helperfunctionsobj[threadidx]->replicateverticesdata((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvdestdram[threadidx][0], 0, KVDATA_RANGE_PERSSDPARTITION);
	#ifdef FPGA_IMPL
	writeVstokernel(threadidx);
	#endif
	
	for(unsigned int lbankoffset = 0; lbankoffset < graphobj->getnumedgebanks(); lbankoffset += 1){
		edge_t edgepropertyfilesize = lseek(graphobj->getnvmeFd_edgeproperties_r2()[lbankoffset][threadidxoffset + threadidx], 0, SEEK_END) / sizeof(edgeprop1_t);			
		unsigned int iteration_size = utilityobj[threadidx]->hceildiv((lseek(graphobj->getnvmeFd_edgeproperties_r2()[lbankoffset][threadidxoffset + threadidx], 0, SEEK_END) / sizeof(edgeprop1_t)), KVDATA_BATCHSIZE);
		
		unsigned int fdoffset[NUMCPUTHREADS];
		unsigned int loadsize[NUMCPUTHREADS];
		
		unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
		unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
		unsigned int runsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]; 
		utilityobj[threadidx]->setarray(batchoffset, NUMCPUTHREADS, NUMSUBCPUTHREADS, BASEOFFSET_KVDRAMBUFFER);
		utilityobj[threadidx]->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
		utilityobj[threadidx]->setarray(runsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	
		for(int flag = 0; flag < NUMFLAGS; flag++){ for(int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ utilityobj[threadidx]->resetkeyvalues("pagerank:: resetting messages", &kvstats[threadidx][flag][i][j][BASEOFFSET_MESSAGESDRAM], MESSAGES_SIZE); }}}
		
		for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx += NUMCPUTHREADS){
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"PP&A:: [threadidx:"<<(threadidxoffset + threadidx)<<"][size:"<<graphobj->getnumvertexbanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [lbankoffset:"<<lbankoffset<<"][size:"<<graphobj->getnumedgebanks()<<"][step:1], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:NUMCPUTHREADS]"<<endl;		
			#endif
			
			int flag = helperfunctionsobj[threadidx]->getflag(globaliteration_idx);
			
			// Populate kvdrams
			for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ fdoffset[i] = (iteration_idx + i) * KVDATA_BATCHSIZE; }
			for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ loadsize[i] = utilityobj[threadidx]->hmin(KVDATA_BATCHSIZE, utilityobj[threadidx]->hsub((size_t)edgepropertyfilesize, (size_t)((size_t)(iteration_idx + i) * (size_t)KVDATA_BATCHSIZE))); }			
			utilityobj[threadidx]->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
			
			std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
			edgeprocessobj[threadidx]->generateupdates(lbankoffset, threadidxoffset + threadidx, fdoffset, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvsourcedram[threadidx][flag], batchoffset, batchsize, loadsize);
			utilityobj[threadidx]->stopTIME("pagerank::WorkerThread: finished start2. Time Elapsed: ", begintime, NAp);
			
			for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ runsize[i][j] += batchsize[i][j]; }}
			helperfunctionsobj[threadidx]->updatemessagesbeforelaunch(graph_iterationidx, voffset, batchsize, runsize, kvstats[threadidx][flag]);
			
			// Launch the Kernel
			helperfunctionsobj[threadidx]->launchkernel(threadidx, (uint512_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram[threadidx][flag], (uint512_dt* (*)[NUMSUBCPUTHREADS])kvdestdram[threadidx][flag], (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvstats[threadidx][flag]);
			
			helperfunctionsobj[threadidx]->updatemessagesafterlaunch(threadidx, globaliteration_idx, kvstats[threadidx][flag]);
			globaliteration_idx += 1;
		}
	}

	// writeback temp vertices data
	#ifdef FPGA_IMPL
	readVsfromkernel(threadidx);
	#endif
	helperfunctionsobj[threadidx]->cummulateverticesdata((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvdestdram[threadidx][0], 0, KVDATA_RANGE_PERSSDPARTITION);
	helperfunctionsobj[threadidx]->applyvertices(threadidx, (keyvalue_t *)kvdestdram[threadidx][0][0][0], 0, KVDATA_RANGE_PERSSDPARTITION, 0, graph_iterationidx);
	graphobj->savevertexdatatofile(threadidxoffset + threadidx, 0, (keyvalue_t *)kvdestdram[threadidx][0][0][0], 0, KVDATA_RANGE_PERSSDPARTITION);
	return;
}



