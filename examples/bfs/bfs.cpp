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
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ parametersobj[i] = new parameters(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ postprocessobj[i] = new postprocess(graphobj, statsobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ loadgraphobj[i] = new loadgraph(graphobj, statsobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ setupkernelobj[i] = new setupkernel(graphobj, statsobj); }

	#ifdef FPGA_IMPL
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}					
	#else
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}
	#endif
	#ifndef INMEMORYGP
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ edges[j][k] = new edge_type[MAXKVDATA_BATCHSIZE]; }}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ vertexptrs[j][k] = new edge_t[KVDATA_RANGE]; }} // FIXME. REMOVEME. size too large
	#endif 
	
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ setupkernelobj[i]->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer[i]); }
	#endif
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj[0]->loadSRstructures();
	#endif 
}
bfs::~bfs(){
	cout<<"bfs::~bfs:: finish destroying memory structures... "<<endl;
	// delete [] container->edgesbuffer;
	delete [] kvbuffer;
}
void bfs::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj[0]->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	setupkernelobj[0]->finishSR();
	#endif
}

runsummary_t bfs::run(){
	cout<<"bfs::run:: bfs algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	tempvertexdatabuffer = graphobj->generatetempverticesdata();
	graphobj->openfilesforreading(0);
	
	container_t mycontainer;
	vector<value_t> activevertices;
	activevertices.push_back(1); // 2
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	unsigned int GraphIter = 0;
	while(true){
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs::run: graph iteration "<<GraphIter<<" of bfs started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj[0]->printvalues(">>> run: printing active vertices for current iteration", activevertices, utilityobj[0]->hmin(activevertices.size(), 16));
		#endif
		
		WorkerThread(activevertices, &mycontainer, GraphIter); 
		
		activevertices.clear();
		postprocessobj[0]->applyvertices2(tempvertexdatabuffer, vertexdatabuffer, activevertices, BREADTHFIRSTSEARCH);
		
		if(activevertices.size() == 0 || GraphIter >= 30){ break; }
		GraphIter += 1;
	}
	cout<<endl;
	finish();
	utilityobj[0]->stopTIME("bfs::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}

void bfs::WorkerThread(vector<vertex_t> &activevertices, container_t * container, unsigned int GraphIter){
	for(unsigned int col=0; col<NUMSSDPARTITIONS; col++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<< TIMINGRESULTSCOLOR << ">>> bfs::WorkerThread: [col: "<<col<<"][size: "<<NUMSSDPARTITIONS<<"][step: 1]"<< RESET <<endl;
		#endif 
		
		vertex_t srcvidsoffset1 = 0;
		vertex_t srcvidsoffset2 = 0;
		vertex_t errcount = 0;
		
		while(true){
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<endl<< TIMINGRESULTSCOLOR << ">>> bfs::WorkerThread: [iteration: "<<errcount<<"][size: UNKNOWN][step: 1]"<< RESET <<endl;
			#endif 
		
			edge_t totalnumedges = loadgraphobj[0]->countedges(col, graphobj, activevertices, &srcvidsoffset1, EDGES_BATCHSIZE, container);
			unsigned int lbedgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS];
			for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ lbedgesizes[0][j] = totalnumedges / NUMSUBCPUTHREADS; }
			
			#ifdef INMEMORYGP
			loadgraphobj[0]->loadactivesubgraph(col, graphobj, activevertices, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, vertexdatabuffer, lbedgesizes, container);
			#else 
			loadgraphobj[0]->loadactivesubgraph(col, graphobj, activevertices, srcvidsoffset2, vertexptrs[0], edges[0], lbedgesizes, container);
			#endif
			loadgraphobj[0]->loadvertexdata(tempvertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
			loadgraphobj[0]->loadmessages(kvbuffer[0], container, GraphIter, BREADTHFIRSTSEARCH);
			for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(col, container->edgessize[i][j]); }}
			
			#ifdef INMEMORYGP
			setupkernelobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
			#else 
			setupkernelobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, vertexptrs, vertexdatabuffer, edges, 0);
			#endif 
			
			postprocessobj[0]->cummulateandcommitverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, tempvertexdatabuffer, col * KVDATA_RANGE_PERSSDPARTITION);
			
			if(srcvidsoffset1 >= activevertices.size()){ break; }
			if(errcount >= 10){ cout<<"bfs::WorkerThread::ERROR. looping too long, error count ("<<errcount<<") limit reached. EXITING..."<<endl; exit(EXIT_FAILURE); }
			errcount += 1;
		}
		// break; // REMOVEME.
		// exit(EXIT_SUCCESS); // REMOVEME.
	}
	return;
}






















