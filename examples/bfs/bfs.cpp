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
#include "../../examples/helperfunctions/compactgraph.h"
#include "../../examples/helperfunctions/setupkernel.h"
#include "../../examples/helperfunctions/postprocess.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "bfs.h"
using namespace std;
#define PROCESSACTIVEVERTICESTEST

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
	compactgraphobj = new compactgraph(graphobj, statsobj);
	setupkernelobj = new setupkernel(graphobj, statsobj); 

	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }				
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	#endif
	edgedatabuffer = new edge2_type[PADDEDEDGES_BATCHSIZE];
	
	packedvertexptrbuffer = new edge_t[KVDATA_RANGE];
	// packededgedatabuffer = new edge2_type[PADDEDEDGES_BATCHSIZE];
	packededgedatabuffer = new uuint64_dt[PADDEDEDGES_BATCHSIZE];
	
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
	// activevertices.push_back(1);
	// for(unsigned int i=0; i<10; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<10000; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<1000000; i++){ activevertices.push_back(i); } //
	for(unsigned int i=0; i<2000000; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<4000000; i++){ activevertices.push_back(i); }
	
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	loadgraphobj->loadvertexdata(tempvertexdatabuffer, (keyvalue_t **)kvbuffer, 0, KVDATA_RANGE_PERSSDPARTITION);
	#ifdef COMPACTEDGES
	compactgraphobj->compact(vertexptrbuffer, edgedatabuffer, packedvertexptrbuffer, packededgedatabuffer);
	utilityobj->printvalues("bfs::run:: packedvertexptrbuffer", packedvertexptrbuffer, 16);
	loadgraphobj->loadedges_rowwise(0, packedvertexptrbuffer, packededgedatabuffer, (vptr_type **)kvbuffer, (uuint64_dt **)kvbuffer, &container, PAGERANK);
	loadgraphobj->loadoffsetmarkers((uuint64_dt **)kvbuffer, (keyvalue_t **)kvbuffer, &container);
	#else
	loadgraphobj->loadedges_rowwise(0, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, PAGERANK);
	loadgraphobj->loadoffsetmarkers((edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, &container);
	#endif
	// exit(EXIT_SUCCESS); // REMOVEME.
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int GraphIter=0; GraphIter<1; GraphIter++){
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs::run: graph iteration "<<GraphIter<<" of bfs started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
		
		loadgraphobj->loadactvvertices(activevertices, (keyvalue_t **)kvbuffer, &container);
		loadgraphobj->loadmessages(kvbuffer, &container, GraphIter, BREADTHFIRSTSEARCH);
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
		
		setupkernelobj->launchkernel((uint512_vec_dt **)kvbuffer, 0);
		verify(activevertices);
		exit(EXIT_SUCCESS);
		
		#ifdef FPGA_IMPL
		setupkernelobj->readfromkernel(0, (uint512_vec_dt **)kvbuffer, BASEOFFSET_VERTICESDATA, BASEOFFSET_VERTICESDATA, (BATCH_RANGE / 2));
		#endif
		postprocessobj->cummulateandcommitverticesdata((value_t **)kvbuffer, tempvertexdatabuffer, 0 * KVDATA_RANGE_PERSSDPARTITION);
		activevertices.clear();
		postprocessobj->applyvertices2(tempvertexdatabuffer, vertexdatabuffer, activevertices, BREADTHFIRSTSEARCH);
		
		if(activevertices.size() == 0){ break; }
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
void bfs::verify(vector<vertex_t> &activevertices){
	cout<<"bfs::verify. verifying..."<<endl;
	#if defined(PROCESSACTIVEVERTICESTEST) && defined(ENABLE_PERFECTACCURACY)
	unsigned int edges1_count = 0;
	unsigned int edgesdstv1_sum = 0;
	unsigned int edges2_count = 0;
	unsigned int edgesdstv2_sum = 0;
	unsigned int edges3_count = 0;
	unsigned int edgesdstv3_sum = 0;
	keyy_t keys[COMPACTPARAM_ITEMSIZE_TOTALDATA];
	
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	utilityobj->printedgestats(activevertices, vertexptrbuffer, edgedatabuffer, &edges1_count, &edgesdstv1_sum);
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ edges2_count += kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[0].key; edgesdstv2_sum += kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[1].key; }				
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		unsigned int sz = kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[2].key;
		if(sz > KVDRAMSZ){ cout<<"ERROR: something wrong (sz("<<sz<<") > KVDRAMSZ("<<KVDRAMSZ<<")). exiting... "<<endl; exit(EXIT_FAILURE); }
		for(unsigned int j=0; j<sz; j++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				keyvalue_t keyvalue = kvbuffer[i][BASEOFFSET_KVDRAM_KVS + j].data[v];
				
				if(keyvalue.key != INVALIDDATA && keyvalue.key != INVALIDDATA){
					unsigned int numitems = utilityobj->PARSE(keyvalue, keys);
					edges3_count += numitems; 
					for(unsigned int t=0; t<numitems; t++){ edgesdstv3_sum += keys[t]; }
				}
			}
		}
	}
	
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (offchip) edges1_count: "<<edges1_count<<", edgesdstv1_sum: "<<edgesdstv1_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (onchip)  edges2_count: "<<edges2_count<<", edgesdstv2_sum: "<<edgesdstv2_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (inkvdram) edges3_count: "<<edges3_count<<", edgesdstv2_sum: "<<edgesdstv3_sum<<endl;
	
	if(edges1_count != edges2_count || edges1_count != edges3_count){ cout<<"bfs::verify: ERROR: edges1_count != edges2_count || edges1_count != edges3_count. ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }
	if((edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv2_sum) && false){ cout<<"bfs::verify: ERROR: edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv2_sum. ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }							
	cout<<"bfs::verify: verify successful."<<endl;
	#endif
	return;
}




















