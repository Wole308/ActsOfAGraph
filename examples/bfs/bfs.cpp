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
#include "bfs.h"
using namespace std;

bfs::bfs(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumedgebanks(), true, true, true);
	statsobj = new stats(graphobj);
	algorithmobj = new algorithm();
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
	edgedatabuffer = new edge2_type[PADDEDEDGES_BATCHSIZE];
	
	#ifdef FPGA_IMPL
	setupkernelobj->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer);
	#endif
}
bfs::~bfs(){
	cout<<"bfs::~bfs:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void bfs::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj->finishOCL();
	#endif
}

runsummary_t bfs::run(){
	cout<<"bfs::run:: bfs algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	tempvertexdatabuffer = graphobj->generatetempverticesdata();
	graphobj->openfilesforreading(0);
	
	container_t container;
	vector<value_t> activevertices;
	activevertices.push_back(1);//  (1); // 2
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	unsigned int GraphIter = 0;
	while(true){
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs::run: graph iteration "<<GraphIter<<" of bfs started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
		
		loadgraphobj->loadvertexdata(tempvertexdatabuffer, (keyvalue_t **)kvbuffer, 0, KVDATA_RANGE_PERSSDPARTITION);
		// loadgraphobj->loadvertexptrs(0, vertexptrbuffer, vertexdatabuffer, (vptr_type **)kvbuffer, &container);
		// loadgraphobj->loadedges_columnwise(0, 0, vertexptrbuffer, edgedatabuffer, (edge_type **)kvbuffer, BASEOFFSET_EDGESDATA, &container, PAGERANK);
		loadgraphobj->loadedges_rowwise(0, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, PAGERANK);
		loadgraphobj->loadactvvertices(activevertices, (keyvalue_t **)kvbuffer, &container);
		loadgraphobj->loadmessages(kvbuffer, &container, GraphIter, BREADTHFIRSTSEARCH);
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
		
		setupkernelobj->launchkernel((uint512_vec_dt **)kvbuffer, 0);
		exit(EXIT_SUCCESS);
		
		#ifdef FPGA_IMPL
		setupkernelobj->readfromkernel(0, (uint512_vec_dt **)kvbuffer, BASEOFFSET_VERTICESDATA, BASEOFFSET_VERTICESDATA, (BATCH_RANGE / 2));
		#endif
		postprocessobj->cummulateandcommitverticesdata((value_t **)kvbuffer, tempvertexdatabuffer, 0 * KVDATA_RANGE_PERSSDPARTITION);
		activevertices.clear();
		postprocessobj->applyvertices2(tempvertexdatabuffer, vertexdatabuffer, activevertices, BREADTHFIRSTSEARCH);
		
		// if(activevertices.size() == 0 || GraphIter >= 60){ break; }
		if(activevertices.size() == 0 || GraphIter >= 0){ break; }
		GraphIter += 1;
	}
	cout<<endl;
	finish();
	utilityobj->stopTIME("bfs::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}




















