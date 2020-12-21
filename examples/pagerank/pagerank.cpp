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
	parametersobj = new parameters(); 
	utilityobj = new utility(); 
	postprocessobj = new postprocess(graphobj, statsobj); 
	loadgraphobj = new loadgraph(graphobj, statsobj); 
	setupkernelobj = new setupkernel(graphobj, statsobj); 

	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }		
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	#endif
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ edges[i] = new edge2_type[MAXKVDATA_BATCHSIZE]; }
	edgedatabuffer = new edge2_type[PADDEDEDGES_BATCHSIZE];
	
	#ifdef FPGA_IMPL
	setupkernelobj->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer);
	#endif
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj->loadSRstructures();
	#endif 
}
pagerank::~pagerank(){
	cout<<"pagerank::~pagerank:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void pagerank::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj->finishOCL();
	#endif
}

runsummary_t pagerank::run(){
	cout<<"pagerank::run:: pagerank algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	tempvertexdatabuffer = graphobj->generatetempverticesdata();
	graphobj->openfilesforreading(0);
	
	container_t container;
	vector<value_t> activevertices;
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int GraphIter=0; GraphIter<1; GraphIter++){
		cout<< TIMINGRESULTSCOLOR <<">>> pagerank::run: graph iteration "<<GraphIter<<" of pagerank started"<< RESET <<endl;
		
		#ifdef GRAFBOOST_SETUP
		setupkernelobj->startSRteration();
		#endif
		
		for(unsigned int col=0; col<graphobj->getnumedgebanks(); col += NUMSUPERCPUTHREADS){ //2 graphobj->getnumedgebanks()
			cout<<endl<< TIMINGRESULTSCOLOR << ">>> pagerank::start2: super iteration: [col: "<<col<<"][size: "<<graphobj->getnumedgebanks()<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<< RESET <<endl;
			WorkerThread(0, col, activevertices, &container, GraphIter);
			cout<<">>> pagerank::start2 Finished: all threads joined..."<<endl;
			break; // REMOVEME.
		}
		
		activevertices.clear();
		#ifdef ACTGRAPH_SETUP
		postprocessobj->applyvertices2(tempvertexdatabuffer, vertexdatabuffer, activevertices, PAGERANK);
		#endif 
		#ifdef GRAFBOOST_SETUP
		setupkernelobj->finishSRteration(GraphIter, activevertices); // NEWCHANGE.
		#endif
	}
	finish();
	utilityobj->stopTIME("pagerank::run: finished run. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closefilesforreading();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}

void pagerank::WorkerThread(unsigned int superthreadidx, unsigned int col, vector<vertex_t> &nextactivevertices, container_t * container, unsigned int GraphIter){
	vertex_t srcvoffset = 0;
	unsigned int iteration_size = utilityobj->hceildiv(graphobj->getedgessize(col), EDGES_BATCHSIZE);
	cout<<">>> WorkerThread:: total number of edges in file["<<col<<"]: "<<graphobj->getedgessize(col)<<endl;
	if(graphobj->getedgessize(col) == 0){ cout<<">>> WorkerThread:: no edges. skipping..."<<endl; return; }
	
	loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t **)kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
	#ifdef FPGA_IMPL
	setupkernelobj->writetokernel(0, (uint512_vec_dt **)kvbuffer, BASEOFFSET_VERTICESDATA, BASEOFFSET_VERTICESDATA, (BATCH_RANGE / 2));
	#endif
	
	for(unsigned int iteration_idx=0; iteration_idx<iteration_size; iteration_idx++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<<"PP&A:: [col:"<<col<<"][size:"<<graphobj->getnumedgebanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
		#endif
		
		#ifdef INMEMORYGP
		srcvoffset = loadgraphobj->loadedges_columnwise(col, srcvoffset, vertexptrbuffer, edgedatabuffer, (edge_type **)kvbuffer, BASEOFFSET_EDGESDATA, container, PAGERANK);
		loadgraphobj->loadvertexptrs(col, vertexptrbuffer, vertexdatabuffer, (vptr_type **)kvbuffer, container);
		#else 
		srcvoffset = loadgraphobj->loadedges_columnwise(col, srcvoffset, vertexptrbuffer, edgedatabuffer, (edge_type **)edges, 0, container, PAGERANK);
		#endif
		#ifdef COLLECTSTATSOFFLINE
		loadgraphobj->loadkvstats((keyvalue_t **)kvbuffer, container);
		// loadgraphobj->loadoffsetmarkers((keyvalue_t **)kvbuffer, container);
		#endif 
		loadgraphobj->loadmessages((uint512_vec_dt **)kvbuffer, container, GraphIter, PAGERANK);
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(col, container->edgessize[i]); }
		
		#ifdef INMEMORYGP
		setupkernelobj->launchkernel((uint512_vec_dt **)kvbuffer, 0);
		#else 
		setupkernelobj->launchkernel((uint512_vec_dt **)kvbuffer, graphobj->loadvertexptrsfromfile(col), vertexdatabuffer, (edge_type **)edges, 0);
		#endif 
	
		// break; // REMOVEME.
		// exit(EXIT_SUCCESS); // REMOVEME.
	}
	
	#ifdef FPGA_IMPL
	setupkernelobj->readfromkernel(0, (uint512_vec_dt **)kvbuffer, BASEOFFSET_VERTICESDATA, BASEOFFSET_VERTICESDATA, (BATCH_RANGE / 2));
	#endif
	postprocessobj->cummulateandcommitverticesdata((value_t **)kvbuffer, tempvertexdatabuffer, col * KVDATA_RANGE_PERSSDPARTITION);
	return;
}








