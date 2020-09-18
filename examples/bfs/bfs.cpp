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
#include "../../acts/include/actscommon.h" //
#include "../../include/common.h"
#include "bfs.h"
using namespace std;
std::mutex mutex_bufferupdates;

bfs::bfs(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
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
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){ kvintermediatedram[i][j] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	}
	
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i]->loadOCLstructures(binaryFile, (uint512_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvsourcedram[i], (uint512_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvdestdram[i], kvstats[i]); }
	#endif 
}
bfs::~bfs(){
	cout<<"bfs::~bfs:: finish destroying memory structures... "<<endl;
	#ifdef FPGA_IMPL
	helperfunctionsobj[0]->finishOCL();
	#endif 
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){		
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvsourcedram[i][flag][j][k]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvdestdram[i][flag][j][k]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvstats[i][flag][j][k]; }}}
	}
}

runsummary_t bfs::run() {
	graphobj->openfilesforreading();
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	graphobj->generateverticesdata();
	graphobj->generatevertexproperties();
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	graphobj->loadvertexpropertiesfromfile();
	graphobj->loadvertexdatafromfile();
	graphobj->writerootvertextoactiveverticesfiles(12, 0);
	
	// for(unsigned int graph_iterationidx=0; graph_iterationidx<64; graph_iterationidx++){
	for(unsigned int graph_iterationidx=0; graph_iterationidx<24; graph_iterationidx++){
		cout<< TIMINGRESULTSCOLOR <<">>> bfs::run: graph iteration "<<graph_iterationidx<<" of breadth-first-search started"<< RESET <<endl;
		
		graphobj->openactiveverticesfilesforreading(graph_iterationidx);
		graphobj->openactiveverticesfilesforwriting((graph_iterationidx + 1));
		unsigned int numactivevertices = graphobj->getnumactivevertices();
		cout<<"=== bfs::run: number of active vertices for graph iteration "<<graph_iterationidx<<": "<<numactivevertices<<" ==="<<endl;
		if(numactivevertices == 0){ cout<<"actgraph_bfs_sw::run: no more active vertices to process. exiting..."<<endl; break; }
		graphobj->configureactivevertexreaders(); // important
		
		for(unsigned int i = 0; i < NUMSUPERCPUTHREADS; i++) { WorkerThread1(i, graph_iterationidx); }
		for(unsigned int i = 0; i < NUMSSDPARTITIONS; i++) { cout<<"=== bfs::run: number of active vertices in ssdpartition "<<i<<": "<<VUbuffer[i].size()<<" ==="<<endl; }
		
		for(unsigned int i_batch=0; i_batch<NUMSSDPARTITIONS; i_batch += NUMSUPERCPUTHREADS){
			cout<<">>> actgraph_bfs_sw::start: super iteration: [i_batch: "<<i_batch<<"][size: "<<NUMSSDPARTITIONS<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<<endl;
			for (int i = 0; i < NUMSUPERCPUTHREADS; i++) { WorkerThread2(i, i_batch, graph_iterationidx); }
			cout<<">>> bfs::run Finished: all threads joined..."<<endl;
		}

		graphobj->closeactiveverticesfilesforreading();
		graphobj->closeactiveverticesfilesforwriting();
		for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ VUbuffer[i].clear(); }
		for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSSDPARTITIONS; j++) { intermediateVUbuffer[i][j].clear(); }}
	}
	
	utilityobj[0]->stopTIME("bfs::run: finished run. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closefilesforreading();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	
	return edgeprocessobj[0]->timingandsummary(NAp, totaltime_ms);
}
void bfs::WorkerThread1(int threadidx, unsigned int graph_iterationidx){
	kvresults_t results[NUMCPUTHREADS];
	unsigned int iteration_size = 256;
	
	for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx += 1){
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"PP&A:: [threadidx:"<<threadidx<<"][size:"<<NUMSSDPARTITIONS<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
		#endif
		
		// Populate kvdrams
		edgeprocessobj[threadidx]->generateupdates(threadidx, 0, 0, (keyvalue_t **)kvintermediatedram[threadidx], BASEOFFSET_KVDRAMBUFFER, results);
		
		// Partition updates
		for(int i = 0; i < NUMCPUTHREADS; i++){ partitionupdates(threadidx, (keyvalue_t *)kvintermediatedram[threadidx][i], 0, results[i].datasize); }
		
		// Buffer updates
		mutex_bufferupdates.lock();
		appendupdatestobuffer(intermediateVUbuffer[threadidx], VUbuffer);
		mutex_bufferupdates.unlock();
		
		unsigned int numfinished = 0;
		for(unsigned int i=0; i<NUMCPUTHREADS; i++){ if(results[i].message == FINISHED){ numfinished += 1; }}
		if(numfinished == NUMCPUTHREADS){ cout<<"bfs::PARTITION PHASE:: no more active vertices to process. exiting."<<endl; break; }
	}
	return;
}
void bfs::WorkerThread2(int superthreadidx, int threadidxoffset, unsigned int graph_iterationidx){
	unsigned int globaliteration_idx = 0;
	unsigned int voffset = (threadidxoffset + superthreadidx) * KVDATA_RANGE_PERSSDPARTITION;
	
	#ifdef ACTSMODEL
	graphobj->loadvertexdatafromfile(threadidxoffset + superthreadidx, voffset, (keyvalue_t *)kvdestdram[superthreadidx][0][0][0], 0, KVDATA_RANGE_PERSSDPARTITION);
	helperfunctionsobj[superthreadidx]->replicateverticesdata((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvdestdram[superthreadidx][0], 0, KVDATA_RANGE_PERSSDPARTITION);
	#endif 
	// FIXME. do for ACTSMODEL_LW
	#ifdef FPGA_IMPL
	helperfunctionsobj[superthreadidx]->writeVstokernel(0);
	#endif
	
	unsigned int fdoffset[NUMCPUTHREADS];
	unsigned int loadsize[NUMCPUTHREADS];
	
	unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int runsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]; 
	utilityobj[superthreadidx]->setarray(batchoffset, NUMCPUTHREADS, NUMSUBCPUTHREADS, BASEOFFSET_KVDRAMBUFFER);
	utilityobj[superthreadidx]->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	utilityobj[superthreadidx]->setarray(runsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	
	for(int flag = 0; flag < NUMFLAGS; flag++){ for(int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ utilityobj[superthreadidx]->resetkeyvalues("bfs:: resetting messages", &kvstats[superthreadidx][flag][i][j][BASEOFFSET_MESSAGESDRAM], MESSAGES_SIZE); }}}
	unsigned int iteration_size = utilityobj[superthreadidx]->hceildiv(VUbuffer[threadidxoffset + superthreadidx].size(), (NUMCPUTHREADS * KVDATA_BATCHSIZE));

	for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx += NUMCPUTHREADS){
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"PP&A:: [superthreadidx:"<<(threadidxoffset + superthreadidx)<<"][size:"<<graphobj->getnumvertexbanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
		#endif
		
		int flag = helperfunctionsobj[superthreadidx]->getflag(globaliteration_idx);
		
		// Populate kvdrams
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ fdoffset[i] = (iteration_idx + i) * KVDATA_BATCHSIZE; }
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ loadsize[i] = utilityobj[superthreadidx]->hmin(KVDATA_BATCHSIZE, utilityobj[superthreadidx]->hsub((size_t)VUbuffer[threadidxoffset + superthreadidx].size(), (size_t)((size_t)(iteration_idx + i) * (size_t)KVDATA_BATCHSIZE))); }			
		utilityobj[superthreadidx]->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
		retrieveupdates(superthreadidx, threadidxoffset + superthreadidx, fdoffset, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][flag], batchoffset, batchsize, loadsize, voffset);
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ runsize[i][j] += batchsize[i][j]; }}
		#ifdef ACTSMODEL
		helperfunctionsobj[superthreadidx]->updatemessagesbeforelaunch(globaliteration_idx, graph_iterationidx, BREADTHFIRSTSEARCH, voffset, batchsize, runsize, kvstats[superthreadidx][flag], BASEOFFSET_MESSAGESDRAM, BASEOFFSET_STATSDRAM);
		#endif 
		// FIXME. do for ACTSMODEL_LW
		
		// launch kernel
		helperfunctionsobj[superthreadidx]->launchkernel((uint512_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][flag], (uint512_dt* (*)[NUMSUBCPUTHREADS])kvdestdram[superthreadidx][flag], (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvstats[superthreadidx][flag], flag);
		
		#ifdef ACTSMODEL
		helperfunctionsobj[superthreadidx]->updatemessagesafterlaunch(globaliteration_idx, kvstats[superthreadidx][flag], BASEOFFSET_MESSAGESDRAM, BASEOFFSET_STATSDRAM);
		#endif 
		// FIXME. do for ACTSMODEL_LW
		globaliteration_idx += 1;
	}

	#ifdef FPGA_IMPL
	helperfunctionsobj[superthreadidx]->readVsfromkernel(0);
	#endif
	#ifdef ACTSMODEL
	helperfunctionsobj[superthreadidx]->cummulateverticesdata((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvdestdram[superthreadidx][0], 0, KVDATA_RANGE_PERSSDPARTITION);
	helperfunctionsobj[superthreadidx]->applyvertices(0, ((threadidxoffset + superthreadidx) * KVDATA_RANGE_PERSSDPARTITION), (keyvalue_t *)kvdestdram[superthreadidx][0][0][0], 0, KVDATA_RANGE_PERSSDPARTITION, voffset, graph_iterationidx); // FIXME. CHECKME.
	#endif 
	// FIXME. do for ACTSMODEL_LW
	graphobj->savevertexdatatofile(threadidxoffset + superthreadidx, 0, (keyvalue_t *)kvdestdram[superthreadidx][0][0][0], 0, KVDATA_RANGE_PERSSDPARTITION); // NOT USED
	return;
}

void bfs::partitionupdates(unsigned int threadidx, keyvalue_t * buffer, unsigned int vbegin, unsigned int datasize){
	unsigned int rangeperpartition = KVDATA_RANGE / NUMSSDPARTITIONS;
	for(unsigned int i=0; i<datasize; i++){
		keyvalue_t keyvalue = buffer[i];
		keyy_t key = buffer[i].key;
		value_t value = buffer[i].value;
		unsigned int partition = (key - vbegin) / rangeperpartition;
		if(partition >= NUMSSDPARTITIONS){ cout<<"bfs:: invalid key. key: "<<key<<", value: "<<value<<endl; exit(EXIT_FAILURE); }
		intermediateVUbuffer[threadidx][partition].push_back(keyvalue);
	}
	return;
}
void bfs::appendupdatestobuffer(vector<keyvalue_t> (&sourcebuffer)[NUMSSDPARTITIONS], vector<keyvalue_t> (&destinationbuffer)[NUMSSDPARTITIONS]){
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){
		for(unsigned int k=0; k<sourcebuffer[i].size(); k++){ destinationbuffer[i].push_back(sourcebuffer[i][k]); }			
	}
	return;
}

void bfs::retrieveupdates(unsigned int superthreadidx, unsigned int bank, unsigned int fdoffset[NUMCPUTHREADS], keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int datasize[NUMCPUTHREADS], unsigned int voffset){					
	#ifdef LOCKE
	for (int i = 0; i < NUMCPUTHREADS; i++){ workerthread_retrieveupdates(i, superthreadidx, bank, fdoffset[i], batch[i], batchoffset[i], batchsize[i], datasize[i], voffset); }
	#else 
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[superthreadidx][i] = std::thread(&bfs::workerthread_retrieveupdates, this, i, superthreadidx, bank, fdoffset[i], batch[i], batchoffset[i], batchsize[i], datasize[i], voffset); }
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[superthreadidx][i].join(); }
	#endif
	return;
}
void bfs::workerthread_retrieveupdates(int ithreadidx, unsigned int superthreadidx, unsigned int bank, unsigned int fdoffset, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], unsigned int datasize, unsigned int voffset){
	unsigned int partition;
	for(unsigned int k=0; k<datasize; k++){ 
		partition = edgeprocessobj[superthreadidx]->insertkeyvaluetobuffer(batch, batchoffset, batchsize, VUbuffer[bank][fdoffset + k], voffset);
	}
	return;
}








