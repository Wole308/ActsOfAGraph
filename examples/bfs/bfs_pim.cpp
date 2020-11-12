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
#include <algorithm>
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
#include "bfs_pim.h"
using namespace std;

bfs_pim::bfs_pim(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
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
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}					
	#else
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}
	#endif

	#ifdef FPGA_IMPL
	setupkernelobj->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer[i]);
	#endif
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj->loadSRstructures();
	#endif 
}
bfs_pim::~bfs_pim(){
	cout<<"bfs_pim::~bfs_pim:: finish destroying memory structures... "<<endl;
	// delete [] container->edgesbuffer;
	delete [] kvbuffer;
}
void bfs_pim::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	setupkernelobj->finishSR();
	#endif
}

runsummary_t bfs_pim::run(){
	cout<<"bfs_pim::run:: bfs_pim algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	
	unsigned int GraphIter=0;
	// container_t mycontainer;
	// for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ mycontainer.edgesbuffer[j][k] = new edge_type[KVDATA_BATCHSIZE+1]; }}
	// for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ mycontainer.vertexptrs[j][k] = new edge_t[KVDRAMSZ+1]; }}
	// for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ mycontainer.tempvertexptrs[j][k] = new edge_t[8]; }}
	vector<value_t> activevertices;
	// vector<value_t> activeverticesgroup[NUMSUBCPUTHREADS];
	// vector<value_t> activevertices2;
	activevertices.push_back(2); // 2
	
	// load
	container_t container;
	loadgraphobj->loadvertexptrs(0, vertexptrbuffer, vertexdatabuffer, (keyvalue_t **)kvbuffer[0], &container);
	loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, 0, KVDATA_RANGE_PERSSDPARTITION);
	loadgraphobj->loadedges(0, vertexptrbuffer, edgedatabuffer, (edge_type **)kvbuffer[0], BASEOFFSET_EDGESDATA, &container, BREADTHFIRSTSEARCH);
	// loadgraphobj[0]->loadedges(col, vertexptrbuffer, edgedatabuffer, (keyvalue_t **)kvbuffer[superthreadidx][0][0], BASEOFFSET_EDGESDATA, container, PAGERANK);
		
	loadgraphobj->loadmessages((uint512_vec_dt **)kvbuffer[0], &container, 0, BREADTHFIRSTSEARCH);
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printcontainer(&container); 
	#endif
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	// while(true){
	for(GraphIter=0; GraphIter<2; GraphIter++){
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs_pim::run: graph iteration "<<GraphIter<<" of bfs_pim started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printvalues(">>> run: printing active vertices for current iteration", activevertices, utilityobj->hmin(activevertices.size(), 16));
		#endif
		
		loadgraphobj->loadactvvertices(activevertices, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, &container);
		loadgraphobj->loadmessages((uint512_vec_dt **)kvbuffer[0], &container, GraphIter, BREADTHFIRSTSEARCH);
		
		setupkernelobj->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
		
		activevertices.clear();
		postprocessobj->cummulateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
		postprocessobj->applyvertices(activevertices, (value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, 0, KVDATA_RANGE);
		
		exit(EXIT_SUCCESS); // REMOVEME.
	}
	cout<<endl;
	finish();
	utilityobj->stopTIME("bfs_pim::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}



/* void bfs_pim::WorkerThread(vector<vertex_t> &currentactivevertices, vector<vertex_t> &nextactivevertices, container_t * container, hostglobalparams_t globalparams){
	size_t prevtotaledgesize = 0;
	unsigned int iteration_idx = 0;
	
	for(unsigned int col=0; col<NUMSSDPARTITIONS; col++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<< TIMINGRESULTSCOLOR << ">>> bfs_pim::WorkerThread: [col: "<<col<<"][size: "<<NUMSSDPARTITIONS<<"][step: 1]"<< RESET <<endl;
		#endif 
		
		edge_t totalnumedges = loadgraphobj[0]->countedges(col, graphobj, currentactivevertices, container);
		unsigned int lbedgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS];
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ lbedgesizes[0][j] = totalnumedges / NUMSUBCPUTHREADS; }
		loadgraphobj[0]->loadgraphdata(col, graphobj, currentactivevertices, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, vertexdatabuffer, lbedgesizes, container);
		loadgraphobj[0]->loadvertexdata(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
		loadgraphobj[0]->loadmessages(kvbuffer[0], container);
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj[0]->printcontainer(container); 
		#endif
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(col, container->edgessize[i][j]); }}
		
		// run acts
		setupkernelobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
	
		postprocessobj[0]->cummulateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
		postprocessobj[0]->applyvertices(nextactivevertices, (value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
		
		break; // REMOVEME.
		// exit(EXIT_SUCCESS); // REMOVEME.
	}
	// exit(EXIT_SUCCESS); // REMOVEME.
	return;
} */






















