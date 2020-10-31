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
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
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
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ parametersobj[i] = new parameters(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i] = new helperfunctions2(graphobj, statsobj); }

	#ifdef FPGA_IMPL
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}					
	#else
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}
	#endif

	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i]->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer[i]); }
	#endif
	#ifdef GRAFBOOST_SETUP 
	helperfunctionsobj[0]->loadSRstructures();
	#endif 
}
bfs::~bfs(){
	cout<<"bfs::~bfs:: finish destroying memory structures... "<<endl;
	// delete [] container->edgesbuffer;
	delete [] kvbuffer;
}
void bfs::finish(){
	#ifdef FPGA_IMPL
	helperfunctionsobj[0]->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	helperfunctionsobj[0]->finishSR();
	#endif
}

runsummary_t bfs::run(){
	cout<<"bfs::run:: bfs algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	graphobj->generateverticesdata();
	graphobj->generatevertexproperties(); 
	graphobj->loadvertexpropertiesfromfile(); 
	graphobj->loadvertexdatafromfile();
	vertexpropertybuffer = graphobj->getvertexpropertybuffer();
	vertexdatabuffer = graphobj->getvertexdatabuffer();
	
	container_t mycontainer;
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ mycontainer.edgesbuffer[j][k] = new edge_type[KVDATA_BATCHSIZE+1]; }}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ mycontainer.vertexptrs[j][k] = new edge_t[KVDRAMSZ+1]; }}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ mycontainer.tempvertexptrs[j][k] = new edge_t[8]; }}
	vector<value_t> activevertices; 
	activevertices.push_back(2);
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int groupid = 0; groupid < 1; groupid++){
		graphobj->openfilesforreading(groupid);
		
		globalparams.groupbasevoffset = 0;
		globalparams.groupid = groupid;
		globalparams.graph_algorithmidx = PAGERANK;
		globalparams.graph_iterationidx = 0;
		
		while(true){
			cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs::run: graph iteration "<<globalparams.graph_iterationidx<<" of bfs started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj[0]->printvalues(">>> run: printing active vertices for current iteration", activevertices, utilityobj[0]->hmin(activevertices.size(), 16));
			#endif
			
			WorkerThread(0, globalparams, activevertices, &mycontainer);
			if(activevertices.size() == 0 || globalparams.graph_iterationidx >= 10){ break; }
			globalparams.graph_iterationidx += 1;
		}
		cout<<endl;
		
		graphobj->closefilesforreading();
	}
	#ifdef FPGA_IMPL
	helperfunctionsobj[0]->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	helperfunctionsobj[0]->finishSR();
	#endif
	utilityobj[0]->stopTIME("bfs::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void bfs::WorkerThread(unsigned int col, hostglobalparams_t globalparams, vector<vertex_t> &activevertices, container_t * container){
	size_t prevtotaledgesize = 0;
	unsigned int iteration_idx = 0;
	
	// prep 
	edge_t totalnumedges = helperfunctionsobj[0]->countedges(col, graphobj, activevertices, container);

	// load
	// loadgraphdata(col, graphobj, activevertices, container);
	
	
	unsigned int lbedgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ lbedgesizes[0][j] = totalnumedges / NUMSUBCPUTHREADS; }
	loadbalancedgraphdata(col, graphobj, activevertices, lbedgesizes, container);
	// loadgraphdata(col, graphobj, activevertices, container);
	
	
	helperfunctionsobj[0]->trim(container);
	helperfunctionsobj[0]->loadsourcevertices(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, activevertices, container);
	helperfunctionsobj[0]->loaddestvertices(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
	helperfunctionsobj[0]->loadedges((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, container);
	helperfunctionsobj[0]->loadmessages((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, container);
	
	// run acts
	helperfunctionsobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
	
	activevertices.clear();
	helperfunctionsobj[0]->cummulateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
	helperfunctionsobj[0]->applyvertices((col * KVDATA_RANGE_PERSSDPARTITION), activevertices, (value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
	return;
}

void bfs::loadbalancedgraphdata(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){
	unsigned int srcvidsoffset = 0;
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			unsigned int edgeoffset = 0;
			edge_t edgessz = 0;
			vertex_t srcvsz = 0;
			unsigned int hsrcvid = 0;
			for(unsigned int k=srcvidsoffset; k<srcvids.size(); k++){
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"loadgraphdata:: k: "<<k<<", edgeoffset: "<<edgeoffset<<", edgessz: "<<edgessz<<", srcvids["<<k<<"]: "<<srcvids[k]<<endl;
				#endif
				
				graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[i][j], 0, 2); 
				
				edge_t edgesoffset = container->tempvertexptrs[i][j][0];
				edge_t numedgestoload = container->tempvertexptrs[i][j][1] - container->tempvertexptrs[i][j][0];
				if(numedgestoload > 0){ graphobj->loadedgesfromfile(col, edgesoffset, container->edgesbuffer[i][j], edgeoffset, numedgestoload); }
				
				// re-assign vertex pointers
				container->vertexptrs[i][j][k+1] = container->vertexptrs[i][j][k] + numedgestoload;
				
				// re-assign edges' srcvids
				for(unsigned int n = 0; n < numedgestoload; n++){ container->edgesbuffer[i][j][edgeoffset + n].srcvid = hsrcvid; }
				
				edgeoffset += numedgestoload;
				edgessz += numedgestoload;
				srcvsz += 1;
				hsrcvid += 1;
				
				if(edgessz >= balancededgesizes[i][j]){ // FIXME. REMOVEME.
					cout<<"loadgraphdata::NOTE: edgessz ("<<edgessz<<") has reached max. srcvidsoffset: ("<<srcvidsoffset<<"). load in next subthread. breaking out..."<<endl; 
					break; }
				
				/* if(edgessz + 1000000 > KVDATA_BATCHSIZE){ // FIXME. REMOVEME.
					cout<<"loadgraphdata:ERROR. edgessz ("<<edgessz<<") has reached max. srcvidsoffset: ("<<srcvidsoffset<<"). load in next subthread. breaking out..."<<endl; 
					break; } */
			}
			
			srcvidsoffset += srcvsz;
			
			container->edgesize[i][j] = edgessz;
			container->edgeoffset[i][j] = NAp;//edgeoffset;
			container->firstvid[i][j] = container->edgesbuffer[i][j][0].srcvid;
			container->srcvoffset[i][j] = container->firstvid[i][j];
			container->srcvsize[i][j] = srcvsz + 1; // srcvids.size() + 1;
			container->destvoffset[i][j] = col * KVDATA_RANGE_PERSSDPARTITION;
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"loadgraphdata:: container->edgesize["<<i<<"]["<<j<<"]: "<<container->edgesize[i][j]<<endl;
			cout<<"loadgraphdata:: container->edgeoffset["<<i<<"]["<<j<<"]: "<<container->edgeoffset[i][j]<<endl;
			cout<<"loadgraphdata:: container->srcvoffset["<<i<<"]["<<j<<"]: "<<container->srcvoffset[i][j]<<endl;
			cout<<"loadgraphdata:: container->destvoffset["<<i<<"]["<<j<<"]: "<<container->destvoffset[i][j]<<endl;
			cout<<"loadgraphdata:: container->firstvid["<<i<<"]["<<j<<"]: "<<container->firstvid[i][j]<<endl;
			cout<<">>> loadgraphdata["<<i<<"]["<<j<<"]: number of vertexptrs to load : "<<srcvids.size()<<endl;
			cout<<">>> loadgraphdata["<<i<<"]["<<j<<"]: first data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"][0]: "<<container->vertexptrs[i][j][0]<<endl;
			cout<<">>> loadgraphdata["<<i<<"]["<<j<<"]: second data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"][1]: "<<container->vertexptrs[i][j][1]<<endl;
			cout<<">>> loadgraphdata["<<i<<"]["<<j<<"]: last data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"]["<<srcvids.size()-1<<"]: "<<container->vertexptrs[i][j][srcvids.size()-1]<<endl;
			utilityobj[0]->printvalues("bfs::loadgraphdata.container->vertexptrs[i][j]", container->vertexptrs[i][j], 16);
			utilityobj[0]->printedges("bfs::loadgraphdata.container->edgesbuffer[i][j]", container->edgesbuffer[i][j], 16); // edgessz
			#endif 
		}
	}
	return;
}

void bfs::loadgraphdata(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, container_t * container){
	unsigned int srcvidsoffset = 0;
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			unsigned int edgeoffset = 0;
			edge_t edgessz = 0;
			vertex_t srcvsz = 0;
			unsigned int hsrcvid = 0;
			for(unsigned int k=srcvidsoffset; k<srcvids.size(); k++){
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"loadgraphdata:: k: "<<k<<", edgeoffset: "<<edgeoffset<<", edgessz: "<<edgessz<<", srcvids["<<k<<"]: "<<srcvids[k]<<endl;
				#endif
				
				graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[i][j], 0, 2); 
				
				edge_t edgesoffset = container->tempvertexptrs[i][j][0];
				edge_t numedgestoload = container->tempvertexptrs[i][j][1] - container->tempvertexptrs[i][j][0];
				if(numedgestoload > 0){ graphobj->loadedgesfromfile(col, edgesoffset, container->edgesbuffer[i][j], edgeoffset, numedgestoload); }
				
				// re-assign vertex pointers
				container->vertexptrs[i][j][k+1] = container->vertexptrs[i][j][k] + numedgestoload;
				
				// re-assign edges' srcvids
				for(unsigned int n = 0; n < numedgestoload; n++){ container->edgesbuffer[i][j][edgeoffset + n].srcvid = hsrcvid; }
				
				edgeoffset += numedgestoload;
				edgessz += numedgestoload;
				srcvsz += 1;
				hsrcvid += 1;
				
				if(edgessz + 1000000 > KVDATA_BATCHSIZE){ // FIXME. REMOVEME.
					cout<<"loadgraphdata:ERROR. edgessz ("<<edgessz<<") has reached max. srcvidsoffset: ("<<srcvidsoffset<<"). load in next subthread. breaking out..."<<endl; 
					break; }
			}
			
			srcvidsoffset += srcvsz;
			
			container->edgesize[i][j] = edgessz;
			container->edgeoffset[i][j] = NAp;//edgeoffset;
			container->firstvid[i][j] = container->edgesbuffer[i][j][0].srcvid;
			container->srcvoffset[i][j] = container->firstvid[i][j];
			container->srcvsize[i][j] = srcvsz + 1; // srcvids.size() + 1;
			container->destvoffset[i][j] = col * KVDATA_RANGE_PERSSDPARTITION;
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"loadgraphdata:: container->edgesize["<<i<<"]["<<j<<"]: "<<container->edgesize[i][j]<<endl;
			cout<<"loadgraphdata:: container->edgeoffset["<<i<<"]["<<j<<"]: "<<container->edgeoffset[i][j]<<endl;
			cout<<"loadgraphdata:: container->srcvoffset["<<i<<"]["<<j<<"]: "<<container->srcvoffset[i][j]<<endl;
			cout<<"loadgraphdata:: container->destvoffset["<<i<<"]["<<j<<"]: "<<container->destvoffset[i][j]<<endl;
			cout<<"loadgraphdata:: container->firstvid["<<i<<"]["<<j<<"]: "<<container->firstvid[i][j]<<endl;
			cout<<">>> loadgraphdata["<<i<<"]["<<j<<"]: number of vertexptrs to load : "<<srcvids.size()<<endl;
			cout<<">>> loadgraphdata["<<i<<"]["<<j<<"]: first data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"][0]: "<<container->vertexptrs[i][j][0]<<endl;
			cout<<">>> loadgraphdata["<<i<<"]["<<j<<"]: second data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"][1]: "<<container->vertexptrs[i][j][1]<<endl;
			cout<<">>> loadgraphdata["<<i<<"]["<<j<<"]: last data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"]["<<srcvids.size()-1<<"]: "<<container->vertexptrs[i][j][srcvids.size()-1]<<endl;
			utilityobj[0]->printvalues("bfs::loadgraphdata.container->vertexptrs[i][j]", container->vertexptrs[i][j], 16);
			utilityobj[0]->printedges("bfs::loadgraphdata.container->edgesbuffer[i][j]", container->edgesbuffer[i][j], 16); // edgessz
			#endif 
		}
	}
	return;
}





















