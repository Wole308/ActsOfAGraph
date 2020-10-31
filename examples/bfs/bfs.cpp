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
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
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
	container_t mycontainer;
	container = &mycontainer;
	
	#ifdef FPGA_IMPL
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}					
	#else
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}
	#endif
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ container->edgesbuffer[j][k] = new edge_type[KVDATA_BATCHSIZE+1]; }}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ container->vertexptrs[j][k] = new edge_t[KVDRAMSZ+1]; }}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ container->tempvertexptrs[j][k] = new edge_t[2]; }}
	
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
	cout<<"bfs::run:: advance algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	graphobj->generateverticesdata();
	graphobj->generatevertexproperties(); 
	graphobj->loadvertexpropertiesfromfile(); 
	graphobj->loadvertexdatafromfile();
	vertexpropertybuffer = graphobj->getvertexpropertybuffer();
	vertexdatabuffer = graphobj->getvertexdatabuffer();
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	
	vertex_t basevoffset = 0;
	
	for(unsigned int groupid = 0; groupid < 1; groupid++){
		graphobj->openfilesforreading(groupid); //
		
		globalparams.groupbasevoffset = 0;
		globalparams.groupid = groupid;
		globalparams.graph_algorithmidx = PAGERANK;
		
		for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
			cout<< TIMINGRESULTSCOLOR <<">>> bfs::run: graph iteration "<<graph_iterationidx<<" of advance started"<< RESET <<endl;
			globalparams.graph_iterationidx = graph_iterationidx;
			WorkerThread(0, globalparams);
		}
		
		graphobj->closefilesforreading();
		// break; // REMOVEME.
	}
	#ifdef FPGA_IMPL
	helperfunctionsobj[0]->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	helperfunctionsobj[0]->finishSR();
	#endif
	
	utilityobj[0]->stopTIME("advance::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void bfs::WorkerThread(unsigned int superthreadidx, hostglobalparams_t globalparams){
	size_t prevtotaledgesize = 0;
	unsigned int iteration_idx = 0;
	
	vector<vertex_t> srcvids;
	// for(unsigned int k=0; k<8; k++){ srcvids.push_back(k*512); }
	srcvids.push_back(2);
	
	// load
	loadgraphdata(graphobj, srcvids, container);
	trim(container);
	loadsourcevertices(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, srcvids, container);
	loaddestvertices(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
	loadedges((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, container);
	loadmessages((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, container);
	
	// run acts
	helperfunctionsobj[superthreadidx]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
	// exit(EXIT_SUCCESS); // REMOVEME.
	
	helperfunctionsobj[superthreadidx]->cummulateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
	helperfunctionsobj[superthreadidx]->applyvertices((superthreadidx * KVDATA_RANGE_PERSSDPARTITION), activeverticesbuffer, (value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, 0);
	exit(EXIT_SUCCESS); // REMOVEME.
	return;
}

void bfs::loadgraphdata(graph * graphobj, vector<vertex_t> srcvids, container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			unsigned int edgeoffset = 0;
			edge_t edgessz = 0;
			unsigned int hsrcvid = 0;
			for(unsigned int k=0; k<srcvids.size(); k++){
				#ifdef _DEBUGMODE_HOSTPRINTS2
				cout<<"loadgraphdata:: k: "<<k<<", edgeoffset: "<<edgeoffset<<", edgessz: "<<edgessz<<", srcvids["<<k<<"]: "<<srcvids[k]<<endl;
				#endif
				
				graphobj->loadvertexptrsfromfile(j, srcvids[k], container->tempvertexptrs[i][j], 0, 2); 
				
				edge_t edgesoffset = container->tempvertexptrs[i][j][0];
				edge_t numedgestoload = container->tempvertexptrs[i][j][1] - container->tempvertexptrs[i][j][0];
				if(numedgestoload > 0){ graphobj->loadedgesfromfile(j, edgesoffset, &(container->edgesbuffer[i][j][edgeoffset]), 0, numedgestoload); }
				
				// re-assign vertex pointers
				container->vertexptrs[i][j][k+1] = container->vertexptrs[i][j][k] + numedgestoload; // re-assign vertex pointers
				
				// re-assign edges' srcvids
				for(unsigned int n = 0; n < numedgestoload; n++){ container->edgesbuffer[i][j][edgeoffset + n].srcvid = hsrcvid; }
				
				edgeoffset += numedgestoload;
				edgessz += numedgestoload;
				hsrcvid += 1;
			}
			container->edgesize[i][j] = edgessz;
			container->edgeoffset[i][j] = NAp;//edgeoffset;
			container->firstvid[i][j] = container->edgesbuffer[i][j][0].srcvid;
			container->srcvoffset[i][j] = container->firstvid[i][j];
			container->srcvsize[i][j] = srcvids.size() + 1;
			container->destvoffset[i][j] = j * parametersobj[0]->GET_BATCH_RANGE(0);
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
void bfs::trim(container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			container->vertexptrs[i][j][0] = 0;
			container->vertexptrs[i][j][container->srcvsize[i][j]-1] = container->edgesize[i][j];// - 1; // NEWCHANGE.
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> trim["<<i<<"]["<<j<<"]: first data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"][0]: "<<container->vertexptrs[i][j][0]<<endl;
			cout<<">>> trim["<<i<<"]["<<j<<"]: last data in vertexptrs: vertexptrs["<<i<<"]["<<j<<"]["<<container->srcvsize[i][j]-1<<"]: "<<container->vertexptrs[i][j][container->srcvsize[i][j]-1]<<endl;
			#endif 
		}
	}
	return;
}
void bfs::loadsourcevertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vector<vertex_t> srcvids, container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int k = 0; k < container->srcvsize[i][j]; k++){
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + k].key = container->vertexptrs[i][j][k];
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + k].value = vertexdatabuffer[srcvids[k]];
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj[0]->printkeyvalues("bfs::loadsourcevertices", &kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE], 16);
			cout<<">>> loadsourcevertices["<<i<<"]["<<j<<"]: last data in kvbuffer->vertices: key: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + container->srcvsize[i][j] - 1].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + container->srcvsize[i][j] - 1].value<<endl;			
			#endif 
		}
	}
	return;
}
void bfs::loaddestvertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			memcpy(&kvbuffer[i][j][BASEOFFSET_VERTICESDATA], &vertexdatabuffer[j*parametersobj[0]->GET_BATCH_RANGE(0)], parametersobj[0]->GET_BATCH_RANGE(0) * sizeof(value_t));
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loaddestvertices["<<i<<"]["<<j<<"]: first data in dest vertices[0]: key: "<<kvbuffer[i][j][BASEOFFSET_VERTICESDATA].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_VERTICESDATA].value<<" (vertex data)"<<endl;
			cout<<">>> loaddestvertices["<<i<<"]["<<j<<"]: last data in dest vertices["<<parametersobj[0]->GET_BATCH_RANGE(0)/2 - 1<<"]: key: "<<kvbuffer[i][j][BASEOFFSET_VERTICESDATA + parametersobj[0]->GET_BATCH_RANGE(0)/2 - 1].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_VERTICESDATA + parametersobj[0]->GET_BATCH_RANGE(0)/2 - 1].value<<" (vertex data)"<<endl;
			utilityobj[0]->printkeyvalues("bfs::loaddestvertices", &kvbuffer[i][j][BASEOFFSET_VERTICESDATA], 16);
			#endif 
		}
	}
	return;
}
void bfs::loadedges(keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int k=0; k<container->edgesize[i][j]; k++){
				kvbuffer[i][j][BASEOFFSET_KVDRAM + k].key = container->edgesbuffer[i][j][k].dstvid;
				kvbuffer[i][j][BASEOFFSET_KVDRAM + k].value = container->edgesbuffer[i][j][k].srcvid;
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadedges["<<i<<"]["<<j<<"]: first data: kvbuffer["<<i<<"]["<<j<<"]["<<BASEOFFSET_KVDRAM<<"].key: "<<kvbuffer[i][j][BASEOFFSET_KVDRAM].key<<" (edge dstvid), kvbuffer["<<i<<"]["<<j<<"]["<<BASEOFFSET_KVDRAM<<"].value: "<<kvbuffer[i][j][BASEOFFSET_KVDRAM].value<<" (edges srcvid)"<<endl;
			utilityobj[0]->printkeyvalues("bfs::loadedges", &kvbuffer[i][j][BASEOFFSET_KVDRAM], 16);
			#endif 
		}
	}
	return;
}
void bfs::loadmessages(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){			
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
			if(container->srcvsize[i][j] >= KVDRAMSZ){ cout<<"bfs::run::ERROR: fix this. srcvsize is greater than allowed. srcvsize["<<i<<"]["<<j<<"]: "<<container->srcvsize[i][j]<<", KVDRAMSZ: "<<KVDRAMSZ<<endl; exit(EXIT_FAILURE); }
			helperfunctionsobj[0]->createmessages(
					kvbuffer[i][j], // uint512_vec_dt * kvstats,
					container->srcvoffset[i][j], // unsigned int srcvoffset,
					container->srcvsize[i][j], // unsigned int srcvsize,
					container->destvoffset[i][j], // unsigned int destvoffset,
					container->firstvid[i][j], // unsigned int firstvid,
					container->vertexptrs[i][j][0], // unsigned int firstkey,
					NAp, // unsigned int firstvalue,
					TREE_DEPTH, // unsigned int treedepth,
					0, // unsigned int GraphIter,
					PAGERANK, // unsigned int GraphAlgo,
					container->edgesize[i][j], // unsigned int runsize,
					parametersobj[0]->GET_BATCH_RANGE(0), // unsigned int batch_range,
					parametersobj[0]->GET_BATCH_RANGE_POW(0), // unsigned int batch_range_pow,
					APPLYVERTEXBUFFERSZ, // unsigned int applyvertexbuffersz,
					NUMLASTLEVELPARTITIONS); // unsigned int numlastlevelpartitions
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"loadmessages:: running Acts... size: "<<container->edgesize[i][j]<<endl; 
			#endif 
		}
	}
	return;
}
























