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
// #include "../../examples/helperfunctions/evalparams.h"
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
	loadgraphobj = new loadgraph(graphobj, statsobj); 
	setupkernelobj = new setupkernel(graphobj, statsobj); 
	// evalparamsobj = new evalparams();

	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }
	vdram = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDVDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	vdram = new uint512_vec_dt[PADDEDVDRAMSZ_KVS];
	#endif
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ edges[i] = new edge2_type[MAXKVDATA_BATCHSIZE]; }
	edgedatabuffer = new edge2_type[PADDEDEDGES_BATCHSIZE];
	
	#ifdef FPGA_IMPL
	setupkernelobj->loadOCLstructures(binaryFile, vdram, kvbuffer);
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
	long double totaltime_ms = 0;
	#ifdef PR_ALGORITHM
	cout<<"pagerank::run:: pagerank algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	// tempvertexdatabuffer = graphobj->generatetempverticesdata();
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0)); 
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0); 

	container_t container;
	vector<value_t> activevertices;

	// load workload
	loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)vdram, 0, KVDATA_RANGE);
	#ifdef DISPATCHTYPE_SYNC
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)kvbuffer[i], 0, KVDATA_RANGE); }
	#endif 
	
	loadgraphobj->loadedges_rowblockwise(0, graphobj, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, PAGERANK);
	
	loadgraphobj->loadoffsetmarkers((edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, &container);
	
	loadgraphobj->generatevmaskdata(activevertices, kvbuffer); //
	
	loadgraphobj->loadmessages(vdram, (uint512_vec_dt **)kvbuffer, &container, 1, PAGERANK);
	loadgraphobj->setcustomeval(vdram, (uint512_vec_dt **)kvbuffer, 0);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
	
	// run pagerank
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> pagerank::run: pagerank started."<< RESET <<endl;
	setupkernelobj->launchkernel((uint512_vec_dt *)vdram, (uint512_vec_dt **)kvbuffer, 0);
	utilityobj->stopTIME(">>> pagerank::finished:. Time Elapsed: ", begintime, NAp);
	totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closefilesforreading();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	#endif 
	return statsobj->timingandsummary(NAp, totaltime_ms);
}








