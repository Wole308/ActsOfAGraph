#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <bits/stdc++.h> 
#include <iostream> 
#include <sys/stat.h> 
#include <sys/types.h>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "pagerank.h"
using namespace std;

pagerank::pagerank(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumedgebanks(), true, true, true);
	statsobj = new stats(graphobj);
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ parametersobj[i] = new parameters(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i] = new helperfunctions2(graphobj, statsobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ loadgraphobj[i] = new loadgraph(graphobj, statsobj); }

	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){
		#ifdef FPGA_IMPL
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[i][flag][j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}}					
		#else
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[i][flag][j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}}
		#endif
	}
	
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i]->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer[i]); }
	#endif
	#ifdef GRAFBOOST_SETUP 
	helperfunctionsobj[0]->loadSRstructures();
	#endif 
}
pagerank::~pagerank(){
	cout<<"pagerank::~pagerank:: finish destroying memory structures... "<<endl;
	// delete [] edgesbuffer;
	delete [] kvbuffer;
}
void pagerank::finish(){
	#ifdef FPGA_IMPL
	helperfunctionsobj[0]->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	helperfunctionsobj[0]->finishSR();
	#endif
}

runsummary_t pagerank::run(){
	cout<<"pagerank::run:: pagerank algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	
	container_t mycontainer;
	vector<value_t> activevertices;
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int groupid = 0; groupid < 1; groupid++){
		graphobj->openfilesforreading(groupid); //
		
		globalparams.groupbasevoffset = 0;
		globalparams.groupid = groupid;
		globalparams.graph_algorithmidx = PAGERANK;
		
		for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
			cout<< TIMINGRESULTSCOLOR <<">>> pagerank::run: graph iteration "<<graph_iterationidx<<" of pagerank started"<< RESET <<endl;
			
			globalparams.graph_iterationidx = graph_iterationidx;
			for(unsigned int col=0; col<graphobj->getnumedgebanks(); col += NUMSUPERCPUTHREADS){
				cout<<endl<< TIMINGRESULTSCOLOR << ">>> pagerank::start2: super iteration: [col: "<<col<<"][size: "<<graphobj->getnumedgebanks()<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<< RESET <<endl;
				WorkerThread(0, col, globalparams, activevertices, &mycontainer);
				cout<<">>> pagerank::start2 Finished: all threads joined..."<<endl;
				break; // REMOVEME.
			}
		}
	}
	finish();
	utilityobj[0]->stopTIME("pagerank::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void pagerank::WorkerThread(unsigned int superthreadidx, unsigned int col, hostglobalparams_t globalparams, vector<vertex_t> &nextactivevertices, container_t * container){
	unsigned int iteration_idx = 0;
	unsigned int iteration_size = utilityobj[superthreadidx]->hceildiv((lseek(graphobj->getnvmeFd_edges_r2()[col], 0, SEEK_END) / sizeof(edge_type)), KVDATA_BATCHSIZE);
	cout<<">>> WorkerThread:: total number of edges in file: "<<(lseek(graphobj->getnvmeFd_edges_r2()[col], 0, SEEK_END) / sizeof(edge_type))<<endl;
	
	while(true){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<<"PP&A:: [groupid:"<<globalparams.groupid<<"][col:"<<col<<"][size:"<<graphobj->getnumedgebanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:"<<NUMCPUTHREADS<<"]"<<endl;		
		#endif
		
		// load
		loadgraphobj[0]->loadvertexptrs(col, vertexptrbuffer, (keyvalue_t **)kvbuffer[superthreadidx][0][0], container);
		loadgraphobj[0]->loadvertexdata(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
		loadgraphobj[0]->loadedgedata(col, vertexptrbuffer, edgedatabuffer, (keyvalue_t **)kvbuffer[superthreadidx][0][0], container);
		loadgraphobj[0]->loadmessages((uint512_vec_dt **)kvbuffer[superthreadidx][0][0], container);
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(col, container->edgessize[i][j]); }}
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj[0]->printcontainer(container); 
		#endif
	
		// run pagerank
		helperfunctionsobj[superthreadidx]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], 0);
		
		helperfunctionsobj[0]->cummulateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
		helperfunctionsobj[0]->applyvertices(nextactivevertices, (value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
		
		break; // REMOVEME.
		// exit(EXIT_SUCCESS); // REMOVEME.
	}
	// exit(EXIT_SUCCESS); // REMOVEME.
	return;
}








