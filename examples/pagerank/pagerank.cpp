#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../src/stats/stats.h"
#include "../../acts/include/actscommon.h" //
#include "../../include/common.h"
#include "pagerank.h"
using namespace std;

pagerank::pagerank(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumedgebanks(), true, true, true);
	statsobj = new stats(graphobj);
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ parametersobj[i] = new parameters(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ edgeprocessobj[i] = new edge_process(graphobj, statsobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i] = new helperfunctions(graphobj, statsobj); }
	
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){
		#ifdef FPGA_IMPL
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvsourcedram[i][flag][j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvdestdram[i][flag][j][k] = (uint512_vec_dt *) aligned_alloc(4096, (BATCH_RANGE_KVS * sizeof(uint512_vec_dt))); }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvstats[i][flag][j][k] = (keyvalue_t *) aligned_alloc(4096, (KVSTATSDRAMSZ * sizeof(keyvalue_t))); }}}
		#else 
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvsourcedram[i][flag][j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvdestdram[i][flag][j][k] = new uint512_vec_dt[BATCH_RANGE_KVS]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvstats[i][flag][j][k] = new keyvalue_t[KVSTATSDRAMSZ]; }}}
		#endif
	}
	
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i]->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvsourcedram[i], (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvdestdram[i], kvstats[i]); }
	#endif
}
pagerank::~pagerank(){
	cout<<"pagerank::~pagerank:: finish destroying memory structures... "<<endl;
	#ifdef FPGA_IMPL
	helperfunctionsobj[0]->finishOCL();
	#endif 
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){		
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvsourcedram[i][flag][j][k]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvdestdram[i][flag][j][k]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ delete [] kvstats[i][flag][j][k]; }}}
	}
}

runsummary_t pagerank::run(){
	cout<<"pagerank::run:: pagerank algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	graphobj->generateverticesdata();
	graphobj->generatevertexproperties(); 
	
	graphobj->loadvertexpropertiesfromfile(); 
	graphobj->loadvertexdatafromfile();
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	
	vertex_t basevoffset = 0;
	
	for(unsigned int groupid = 0; groupid < 2; groupid++){
		graphobj->openfilesforreading(groupid); //
		
		globalparams.groupbasevoffset = 0;
		globalparams.groupid = groupid;
		globalparams.graph_algorithmidx = PAGERANK;
		
		for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
			cout<< TIMINGRESULTSCOLOR <<">>> pagerank::run: graph iteration "<<graph_iterationidx<<" of pagerank started"<< RESET <<endl;
			
			globalparams.graph_iterationidx = graph_iterationidx;
			
			for(unsigned int i_batch=0; i_batch<graphobj->getnumedgebanks(); i_batch += NUMSUPERCPUTHREADS){
				cout<<endl<< TIMINGRESULTSCOLOR << ">>> pagerank::start2: super iteration: [i_batch: "<<i_batch<<"][size: "<<graphobj->getnumedgebanks()<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<< RESET <<endl;
				for (int i = 0; i < NUMSUPERCPUTHREADS; i++) { WorkerThread(i, i_batch, globalparams); }
				cout<<">>> pagerank::start2 Finished: all threads joined..."<<endl;
				// break; // REMOVEME.
			}
		}
		
		graphobj->closefilesforreading();
		// break; // REMOVEME.
	}
	#ifdef FPGA_IMPL
	helperfunctionsobj[0]->finishOCL();
	#endif
	
	utilityobj[0]->stopTIME("pagerank::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	
	// return edgeprocessobj[0]->timingandsummary(NAp, totaltime_ms);
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void pagerank::WorkerThread(int superthreadidx, int threadidxoffset, hostglobalparams_t globalparams){ 
	unsigned int globaliteration_idx = 0;
	unsigned int voffset = (threadidxoffset + superthreadidx) * parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid);
	
	edgeprocessobj[superthreadidx]->resetstatevariables();
	
	#ifdef ACTSMODEL
	graphobj->loadvertexdatafromfile(0, ((threadidxoffset + superthreadidx) * parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid)), (keyvalue_t *)kvdestdram[superthreadidx][0][0][0], 0, parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid));
	helperfunctionsobj[superthreadidx]->replicateverticesdata((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvdestdram[superthreadidx][0], 0, parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid));
	#endif 
	#ifdef ACTSMODEL_LW
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ graphobj->loadvertexdatafromfile((((threadidxoffset + superthreadidx) * parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid)) + (i * BATCH_RANGE)), (value_t *)kvsourcedram[superthreadidx][0][0][i], BASEOFFSET_VERTICESDATA, BATCH_RANGE); }
	helperfunctionsobj[superthreadidx]->replicateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][0], BASEOFFSET_VERTICESDATA, BATCH_RANGE);
	#endif
	
	#ifdef FPGA_IMPL // REMOVEME. CHANGEMEBACK.
	helperfunctionsobj[superthreadidx]->writetokernel(0, (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][0], BASEOFFSET_VERTICESDATA, BASEOFFSET_VERTICESDATA, parametersobj[superthreadidx]->GET_BATCH_RANGE(globalparams.groupid)/2);
	#endif
	
	edge_t edgepropertyfilesize = lseek(graphobj->getnvmeFd_edges_r2()[threadidxoffset + superthreadidx], 0, SEEK_END) / sizeof(edgeprop1_t);			
	unsigned int iteration_size = utilityobj[superthreadidx]->hceildiv((lseek(graphobj->getnvmeFd_edges_r2()[threadidxoffset + superthreadidx], 0, SEEK_END) / sizeof(edgeprop1_t)), KVDATA_BATCHSIZE);
	
	unsigned int fdoffset[NUMCPUTHREADS];
	unsigned int loadsize[NUMCPUTHREADS];
	
	unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int runsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]; 
	utilityobj[superthreadidx]->setarray(batchoffset, NUMCPUTHREADS, NUMSUBCPUTHREADS, BASEOFFSET_KVDRAMBUFFER);
	utilityobj[superthreadidx]->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	utilityobj[superthreadidx]->setarray(runsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);

	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"... resetting messages... ["<<NUMCPUTHREADS<<" threads, "<<NUMSUBCPUTHREADS<<" subthreads]"<<endl;
	#endif
	for(int flag = 0; flag < NUMFLAGS; flag++){ for(int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ utilityobj[superthreadidx]->resetkeyvalues(&kvstats[superthreadidx][flag][i][j][BASEOFFSET_MESSAGESDRAM], MESSAGES_SIZE); }}}
	globaliteration_idx = 0;
	
	for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx += NUMCPUTHREADS){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<<"PP&A:: [groupid:"<<globalparams.groupid<<"][superthreadidx:"<<(threadidxoffset + superthreadidx)<<"][size:"<<graphobj->getnumedgebanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:"<<NUMCPUTHREADS<<"]"<<endl;		
		#endif
		
		int flag = helperfunctionsobj[superthreadidx]->getflag(globaliteration_idx);
		
		// Populate kvdrams
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ fdoffset[i] = (iteration_idx + i) * KVDATA_BATCHSIZE; }
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ loadsize[i] = utilityobj[superthreadidx]->hmin(KVDATA_BATCHSIZE, utilityobj[superthreadidx]->hsub((size_t)edgepropertyfilesize, (size_t)((size_t)(iteration_idx + i) * (size_t)KVDATA_BATCHSIZE))); }			
		utilityobj[superthreadidx]->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
		
		edgeprocessobj[superthreadidx]->generateupdates(globalparams.groupid, threadidxoffset + superthreadidx, fdoffset, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][flag], batchoffset, batchsize, loadsize, voffset);
		
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ runsize[i][j] += batchsize[i][j]; }}
		#ifdef ACTSMODEL
		helperfunctionsobj[superthreadidx]->updatemessagesbeforelaunch(globaliteration_idx, voffset, batchsize, runsize, kvstats[superthreadidx][flag], BASEOFFSET_MESSAGESDRAM, BASEOFFSET_STATSDRAM, globalparams);
		#endif
		#ifdef ACTSMODEL_LW
		helperfunctionsobj[superthreadidx]->updatemessagesbeforelaunch(globaliteration_idx, (iteration_idx + NUMCPUTHREADS >= iteration_size), voffset, batchsize, runsize, kvsourcedram[superthreadidx][flag], BASEOFFSET_MESSAGESDRAM_KVS, BASEOFFSET_STATSDRAM_KVS, globalparams);
		#endif
		
		// Launch the Kernel
		helperfunctionsobj[superthreadidx]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][flag], (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvdestdram[superthreadidx][flag], (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvstats[superthreadidx][flag], 0);
		
		#ifdef ACTSMODEL
		helperfunctionsobj[superthreadidx]->updatemessagesafterlaunch(globaliteration_idx, kvstats[superthreadidx][flag], BASEOFFSET_MESSAGESDRAM, BASEOFFSET_STATSDRAM);
		#endif
		#ifdef ACTSMODEL_LW
		helperfunctionsobj[superthreadidx]->updatemessagesafterlaunch(globaliteration_idx, (iteration_idx + NUMCPUTHREADS >= iteration_size), kvsourcedram[superthreadidx][flag], BASEOFFSET_MESSAGESDRAM_KVS, BASEOFFSET_STATSDRAM_KVS);
		#endif 
		globaliteration_idx += 1;
		// break; // REMOVEME.
		// exit(EXIT_SUCCESS);
	}
	// break; // REMOVEME.
	// exit(EXIT_SUCCESS);

	// writeback temp vertices data
	#ifdef FPGA_IMPL // REMOVEME. CHANGEMEBACK.
	helperfunctionsobj[superthreadidx]->readfromkernel(0, (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][0], BASEOFFSET_VERTICESDATA, BASEOFFSET_VERTICESDATA, parametersobj[superthreadidx]->GET_BATCH_RANGE(globalparams.groupid)/2);
	#endif
	#ifdef ACTSMODEL
	helperfunctionsobj[superthreadidx]->cummulateverticesdata((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvdestdram[superthreadidx][0], 0, parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid));
	helperfunctionsobj[superthreadidx]->applyvertices(((threadidxoffset + superthreadidx) * parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid)), (keyvalue_t *)kvdestdram[superthreadidx][0][0][0], 0, parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid), 0, globalparams.graph_iterationidx);
	#endif 
	#ifdef ACTSMODEL_LW
	helperfunctionsobj[superthreadidx]->cummulateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][0], BASEOFFSET_VERTICESDATA, BATCH_RANGE);
	helperfunctionsobj[superthreadidx]->applyvertices(((threadidxoffset + superthreadidx) * parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid)), (value_t* (*)[NUMSUBCPUTHREADS])kvsourcedram[superthreadidx][0], BASEOFFSET_VERTICESDATA, BATCH_RANGE, 0, globalparams);
	#endif
	graphobj->savevertexdatatofile(((threadidxoffset + superthreadidx) * parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid)), (keyvalue_t *)kvdestdram[superthreadidx][0][0][0], 0, parametersobj[superthreadidx]->GET_KVDATA_RANGE_PERSSDPARTITION(globalparams.groupid));
	return;
}



