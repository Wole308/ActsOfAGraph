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
#include "../../examples/helperfunctions/postprocess.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "pagerank_pim.h"
using namespace std;

pagerank_pim::pagerank_pim(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
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
	for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[0][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }			
	#else
	for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[0][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	#endif
	
	#ifdef FPGA_IMPL
	setupkernelobj->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer);
	#endif
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj->loadSRstructures();
	#endif 
}
pagerank_pim::~pagerank_pim(){
	cout<<"pagerank_pim::~pagerank_pim:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void pagerank_pim::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	setupkernelobj->finishSR();
	#endif
}

runsummary_t pagerank_pim::run(){
	cout<<"pagerank_pim::run:: pagerank_pim algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	
	// load
	container_t container;
	loadgraphobj->loadvertexptrs(0, vertexptrbuffer, (keyvalue_t **)kvbuffer[0], &container);
	loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, 0, KVDATA_RANGE_PERSSDPARTITION);
	loadgraphobj->loadedgedata(0, vertexptrbuffer, edgedatabuffer, (keyvalue_t **)kvbuffer[0], &container, PAGERANK);
	loadgraphobj->loadmessages((uint512_vec_dt **)kvbuffer[0], &container);
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printcontainer(&container); 
	#endif
	
	// run pagerank_pim
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
		cout<< TIMINGRESULTSCOLOR <<">>> pagerank_pim::run: graph iteration "<<graph_iterationidx<<" of pagerank_pim started"<< RESET <<endl;
		
		setupkernelobj->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
	}
	finish();
	utilityobj->stopTIME("pagerank_pim::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closefilesforreading();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}





