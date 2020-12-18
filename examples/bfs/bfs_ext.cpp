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
#include "bfs_ext.h"
using namespace std;

bfs_ext::bfs_ext(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
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
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ vertexptrs[j][k] = new edge_t[MAXKVDATA_BATCHSIZE]; }} // FIXME. (KVDATA_RANGE) REMOVEME. size too large // REMOVEME. use MAXKVDATA_BATCHSIZE
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ verticesdata[j][k] = new value_t[MAXKVDATA_BATCHSIZE]; }} // FIXME. (KVDATA_RANGE) REMOVEME. size too large
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ edges[j][k] = new edge_type[MAXKVDATA_BATCHSIZE]; }}
	#endif 
	
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ setupkernelobj[i]->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer[i]); }
	#endif
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj[0]->loadSRstructures();
	#endif 
}
bfs_ext::~bfs_ext(){
	cout<<"bfs_ext::~bfs_ext:: finish destroying memory structures... "<<endl;
	// delete [] container->edgesbuffer;
	delete [] kvbuffer;
}
void bfs_ext::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj[0]->finishOCL();
	#endif
}

runsummary_t bfs_ext::run(){
	cout<<"bfs_ext::run:: bfs_ext algorithm started. "<<endl;
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
	unsigned int active_cnt = activevertices.size();
	while(true){
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs_ext::run: graph iteration "<<GraphIter<<" of bfs_ext started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj[0]->printvalues(">>> run: printing active vertices for current iteration", activevertices, utilityobj[0]->hmin(activevertices.size(), 16));
		#endif
		
		#ifdef GRAFBOOST_SETUP
		setupkernelobj[0]->startSRteration();
		#endif
		
		WorkerThread(activevertices, &mycontainer, GraphIter); 
		
		activevertices.clear();
		#ifdef ACTGRAPH_SETUP
		postprocessobj[0]->applyvertices2(tempvertexdatabuffer, vertexdatabuffer, activevertices, BREADTHFIRSTSEARCH);
		#endif
		#ifdef GRAFBOOST_SETUP
		active_cnt = setupkernelobj[0]->finishSRteration(GraphIter, activevertices);
		#endif
		active_cnt = activevertices.size();
		
		if(activevertices.size() == 0 || GraphIter >= 60){ break; }
		// if(activevertices.size() == 0 || GraphIter >= 3){ break; }
		GraphIter += 1;
	}
	cout<<endl;
	finish();
	utilityobj[0]->stopTIME("bfs_ext::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}

void bfs_ext::WorkerThread(vector<vertex_t> &activevertices, container_t * container, unsigned int GraphIter){
	vertex_t srcvoffset = 0;
	for(unsigned int col=0; col<NUMSSDPARTITIONS; col++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<< TIMINGRESULTSCOLOR << ">>> bfs_ext::WorkerThread: [col: "<<col<<"][size: "<<NUMSSDPARTITIONS<<"][step: 1]"<< RESET <<endl;
		#endif 
		
		vertex_t srcvidsoffset1 = 0;
		vertex_t srcvidsoffset2 = 0;
		vertex_t errcount = 0;
		
		loadgraphobj[0]->loadvertexdata(tempvertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
		#ifdef FPGA_IMPL
		setupkernelobj[0]->writetokernel(0, (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, BASEOFFSET_VERTICESDATA, BASEOFFSET_VERTICESDATA, (BATCH_RANGE / 2));
		#endif
		
		while(true){
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<endl<< TIMINGRESULTSCOLOR << ">>> bfs_ext::WorkerThread: [iteration: "<<errcount<<"][size: UNKNOWN][step: 1]"<< RESET <<endl;
			#endif 
		
			edge_t totalnumedges = loadgraphobj[0]->countedges(col, graphobj, activevertices, &srcvidsoffset1, EDGES_BATCHSIZE, container);
			unsigned int lbedgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS];
			for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ lbedgesizes[0][j] = totalnumedges / NUMSUBCPUTHREADS; }
			
			#ifdef INMEMORYGP
			loadgraphobj[0]->loadactivesubgraph(col, graphobj, activevertices, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, vertexdatabuffer, lbedgesizes, container);
			#else 
			loadgraphobj[0]->loadactivesubgraph(col, graphobj, activevertices, srcvidsoffset2, vertexdatabuffer, vertexptrs[0], verticesdata[0], (edge2_type **)edges[0], lbedgesizes, container);
			#endif
			loadgraphobj[0]->loadmessages(kvbuffer[0], container, GraphIter, BREADTHFIRSTSEARCH);
			for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(col, container->edgessize[i][j]); }}
			
			#ifdef INMEMORYGP
			setupkernelobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
			#else 
			setupkernelobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, vertexptrs, verticesdata, (edge_type* (*)[NUMSUBCPUTHREADS])edges, 0);
			#endif 
			// exit(EXIT_SUCCESS);
			
			if(srcvidsoffset1 >= activevertices.size()){ break; }
			if(errcount >= 10){ cout<<"bfs_ext::WorkerThread::ERROR. looping too long, error count ("<<errcount<<") limit reached. EXITING..."<<endl; exit(EXIT_FAILURE); }
			errcount += 1;
		}
		
		#ifdef FPGA_IMPL
		setupkernelobj[0]->readfromkernel(0, (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, BASEOFFSET_VERTICESDATA, BASEOFFSET_VERTICESDATA, (BATCH_RANGE / 2));
		#endif
		#ifdef ACTGRAPH_SETUP
		postprocessobj[0]->cummulateandcommitverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, tempvertexdatabuffer, col * KVDATA_RANGE_PERSSDPARTITION);
		#endif 
		
		// break; // REMOVEME.
		// exit(EXIT_SUCCESS); // REMOVEME.
	}
	return;
}






















