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
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
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
#include "sssp.h"
using namespace std;

sssp::sssp(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumedgebanks(), true, true, true);
	statsobj = new stats(graphobj);
	algorithmobj = new algorithm();
	parametersobj = new parameters(); 
	utilityobj = new utility(); 
	postprocessobj = new postprocess(graphobj, statsobj); 
	loadgraphobj = new loadgraph(graphobj, statsobj); 
	// compactgraphobj = new compactgraph(graphobj, statsobj);
	setupkernelobj = new setupkernel(graphobj, statsobj); 

	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }				
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	#endif

	if(graphobj->getdataset().graphdirectiontype == UNDIRECTEDGRAPH){
		edgedatabuffer = new edge2_type[2 * graphobj->get_num_edges()];
		// packededgedatabuffer = new uuint64_dt[2 * graphobj->get_num_edges()];
	} else {
		edgedatabuffer = new edge2_type[graphobj->get_num_edges()];
		// packededgedatabuffer = new uuint64_dt[graphobj->get_num_edges()];
	}
	// packedvertexptrbuffer = new edge_t[KVDATA_RANGE];
	
	#ifdef FPGA_IMPL
	setupkernelobj->loadOCLstructures(binaryFile, kvbuffer);
	#endif
}
sssp::~sssp(){
	cout<<"sssp::~sssp:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void sssp::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj->finishOCL();
	#endif
}

runsummary_t sssp::run(){
	cout<<"sssp::run:: sssp algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	tempvertexdatabuffer = graphobj->generatetempverticesdata();
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	// set root vid
	unsigned int NumGraphIters = 1; // 6
	container_t container;
	vector<value_t> activevertices;

	activevertices.push_back(1);
	// for(unsigned int i=0; i<2; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<100; i++){ activevertices.push_back(i); } //
	// for(unsigned int i=0; i<500; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<2048; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<4096; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<10000; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<100000; i++){ activevertices.push_back(i); } //
	// for(unsigned int i=0; i<1000000; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<2000000; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<4000000; i++){ activevertices.push_back(i); }
	vector<value_t> activevertices2;
	for(unsigned int i=0; i<activevertices.size(); i++){ activevertices2.push_back(activevertices[i]); }
	
	// load workload
	loadgraphobj->loadvertexdata(tempvertexdatabuffer, (keyvalue_t **)kvbuffer, 0, KVDATA_RANGE);
	#ifndef COMPACTEDGES // FIXME. NOTNEAT.
	loadgraphobj->loadedges_rowwise(0, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, SSSP);
	loadgraphobj->loadoffsetmarkers((edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, &container);
	loadgraphobj->loadactvvertices(activevertices, (vptr_type **)kvbuffer, (keyy_t **)kvbuffer, &container);
	#endif
	loadgraphobj->loadmessages(kvbuffer, &container, NumGraphIters, BREADTHFIRSTSEARCH);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
	
	// run sssp
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> sssp::run: sssp started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;

	setupkernelobj->launchkernel((uint512_vec_dt **)kvbuffer, 0);

	utilityobj->stopTIME(">>> sssp::finished:. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	// verify 
	// verify(activevertices);
	// utilityobj->runbfs_sw(activevertices2, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	// verifyvertexdata((keyvalue_t **)kvbuffer);
	// verifyactvvsdata((keyvalue_t **)kvbuffer);
	// verifykernelreturnvalues(kvbuffer);
	// apply((keyvalue_t **)kvbuffer, activevertices);
	
	finish();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}


























