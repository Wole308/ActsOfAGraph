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
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../examples/helperfunctions/setupkernel.h"
#include "../../examples/helperfunctions/postprocess.h"
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
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ postprocessobj[i] = new postprocess(graphobj, statsobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ loadgraphobj[i] = new loadgraph(graphobj, statsobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ setupkernelobj[i] = new setupkernel(graphobj, statsobj); }

	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){
		#ifdef FPGA_IMPL
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[i][flag][j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}}					
		#else
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[i][flag][j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}}
		#endif
	}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ edges[j][k] = new edge_type[MAXKVDATA_BATCHSIZE]; }}
	#ifndef INMEMORYGP
	edgedatabuffer = new edge_type[PADDEDEDGES_BATCHSIZE];
	#endif 
	
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ postprocessobj[i]->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer[i]); }
	#endif
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj[0]->loadSRstructures();
	#endif 
}
pagerank::~pagerank(){
	cout<<"pagerank::~pagerank:: finish destroying memory structures... "<<endl;
	// delete [] edgesbuffer;
	delete [] kvbuffer;
}
void pagerank::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj[0]->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	setupkernelobj[0]->finishSR();
	#endif
}

runsummary_t pagerank::run(){
	cout<<"pagerank::run:: pagerank algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	
	container_t container;
	vector<value_t> activevertices;
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int GraphIter=0; GraphIter<1; GraphIter++){
		cout<< TIMINGRESULTSCOLOR <<">>> pagerank::run: graph iteration "<<GraphIter<<" of pagerank started"<< RESET <<endl;
		
		for(unsigned int col=0; col<graphobj->getnumedgebanks(); col += NUMSUPERCPUTHREADS){
			cout<<endl<< TIMINGRESULTSCOLOR << ">>> pagerank::start2: super iteration: [col: "<<col<<"][size: "<<graphobj->getnumedgebanks()<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<< RESET <<endl;
			WorkerThread(0, col, activevertices, &container, GraphIter);
			cout<<">>> pagerank::start2 Finished: all threads joined..."<<endl;
			break; // REMOVEME.
		}
	}
	finish();
	utilityobj[0]->stopTIME("pagerank::run: finished run. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closefilesforreading();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
runsummary_t pagerank::runpim(){
	cout<<"pagerank::runpim:: pagerank_pim algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	
	// load
	container_t container;
	size_t fedgeoffset = 0;
	loadgraphobj[0]->loadvertexptrs(0, vertexptrbuffer, vertexdatabuffer, (keyvalue_t **)kvbuffer[0][0][0], &container);
	loadgraphobj[0]->loadvertexdata(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], 0, KVDATA_RANGE_PERSSDPARTITION);
	loadgraphobj[0]->loadedges(0, vertexptrbuffer, edgedatabuffer, (edge_type **)kvbuffer[0][0][0], BASEOFFSET_EDGESDATA, &container, PAGERANK);
	loadgraphobj[0]->loadmessages((uint512_vec_dt **)kvbuffer[0], &container, 0, PAGERANK);
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(0, container.edgessize[i][j]); }}
	
	// run pagerank_pim
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
		cout<< TIMINGRESULTSCOLOR <<">>> pagerank_pim::run: graph iteration "<<graph_iterationidx<<" of pagerank_pim started"<< RESET <<endl;
		
		setupkernelobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
	}
	finish();
	utilityobj[0]->stopTIME("pagerank_pim::runpim: finished runpim. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closefilesforreading();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}

void pagerank::WorkerThread(unsigned int superthreadidx, unsigned int col, vector<vertex_t> &nextactivevertices, container_t * container, unsigned int GraphIter){
	vertex_t srcvoffset = 0;
	unsigned int iteration_size = utilityobj[superthreadidx]->hceildiv(graphobj->getedgessize(col), EDGES_BATCHSIZE);
	cout<<">>> WorkerThread:: total number of edges in file["<<col<<"]: "<<graphobj->getedgessize(col)<<endl;
	
	for(unsigned int iteration_idx=0; iteration_idx<iteration_size; iteration_idx++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<<"PP&A:: [col:"<<col<<"][size:"<<graphobj->getnumedgebanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
		#endif
		
		#ifdef PROCEDGESINCPU
		srcvoffset = loadgraphobj[superthreadidx]->loadedges(col, srcvoffset, vertexptrbuffer, edgedatabuffer, edges[0], 0, container, PAGERANK);
		#else 
		srcvoffset = loadgraphobj[superthreadidx]->loadedges(col, srcvoffset, vertexptrbuffer, edgedatabuffer, (edge_type **)kvbuffer[superthreadidx][0][0], BASEOFFSET_EDGESDATA, container, PAGERANK);
		loadgraphobj[superthreadidx]->loadvertexptrs(col, vertexptrbuffer, vertexdatabuffer, (keyvalue_t **)kvbuffer[superthreadidx][0][0], container);
		#endif
		loadgraphobj[superthreadidx]->loadvertexdata(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
		loadgraphobj[superthreadidx]->loadmessages((uint512_vec_dt **)kvbuffer[superthreadidx][0][0], container, GraphIter, PAGERANK);
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(col, container->edgessize[i][j]); }}
		
		#ifdef PROCEDGESINCPU
		setupkernelobj[superthreadidx]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], graphobj->loadvertexptrsfromfile(col), vertexdatabuffer, edges, 0);
		#else 
		setupkernelobj[superthreadidx]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], 0);
		#endif 
		
		postprocessobj[superthreadidx]->cummulateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
		postprocessobj[superthreadidx]->applyvertices(nextactivevertices, (value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION, PAGERANK);

		// break; // REMOVEME.
		// exit(EXIT_SUCCESS); // REMOVEME.
	}
	return;
}








