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
#include "../include/examplescommon.h"
#include "advance_op.h"
using namespace std;

/** auto advance_op = [rank_curr, rank_next] __host__ __device__(
					const VertexT &src, VertexT &dest,
					const SizeT &edge_id, const VertexT &input_item,
					const SizeT &input_pos, SizeT &output_pos) -> bool {
	ValueT add_value = rank_curr[src];
	if (isfinite(add_value)) {
		atomicAdd(rank_next + dest, add_value);
	}
	return true;
}; */

advance_op::advance_op(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumedgebanks(), true, true, true);
	statsobj = new stats(graphobj);
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ parametersobj[i] = new parameters(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i] = new helperfunctions2(graphobj, statsobj); }

	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){
		#ifdef FPGA_IMPL
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[i][flag][j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}}					
		#else
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[i][flag][j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}}
		#endif
		// for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ edgesbuffer[i][flag][j][k] = new edge_type[KVDATA_BATCHSIZE+1]; }}}
		// for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ vertexptrs[i][flag][j][k] = new edge_t[KVDRAMSZ+1]; }}} // KVDATA_BATCHSIZE // KVDRAMSZ
	}
	
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i]->loadOCLstructures(binaryFile, (uint512_vec_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer[i]); }
	#endif
	#ifdef GRAFBOOST_SETUP 
	helperfunctionsobj[0]->loadSRstructures();
	#endif 
}
advance_op::~advance_op(){
	cout<<"advance_op::~advance_op:: finish destroying memory structures... "<<endl;
	// delete [] edgesbuffer;
	delete [] kvbuffer;
}
void advance_op::finish(){
	#ifdef FPGA_IMPL
	helperfunctionsobj[0]->finishOCL();
	#endif
	#ifdef GRAFBOOST_SETUP
	helperfunctionsobj[0]->finishSR();
	#endif
}

runsummary_t advance_op::run(){
	cout<<"advance_op::run:: advance algorithm started. "<<endl;
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
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	for(unsigned int groupid = 0; groupid < 1; groupid++){
		graphobj->openfilesforreading(groupid); //
		
		globalparams.groupbasevoffset = 0;
		globalparams.groupid = groupid;
		globalparams.graph_algorithmidx = PAGERANK;
		
		for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
			cout<< TIMINGRESULTSCOLOR <<">>> advance_op::run: graph iteration "<<graph_iterationidx<<" of advance started"<< RESET <<endl;
			
			globalparams.graph_iterationidx = graph_iterationidx;
			
			for(unsigned int col=0; col<graphobj->getnumedgebanks(); col += NUMSUPERCPUTHREADS){
				cout<<endl<< TIMINGRESULTSCOLOR << ">>> advance_op::start2: super iteration: [col: "<<col<<"][size: "<<graphobj->getnumedgebanks()<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<< RESET <<endl;
				WorkerThread(0, col, globalparams, activevertices, &mycontainer);
				cout<<">>> advance_op::start2 Finished: all threads joined..."<<endl;
				// break; // REMOVEME.
			}
		}
		
		graphobj->closefilesforreading();
		// break; // REMOVEME.
	}
	finish();
	utilityobj[0]->stopTIME("advance::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void advance_op::WorkerThread(unsigned int superthreadidx, unsigned int col, hostglobalparams_t globalparams, vector<vertex_t> &nextactivevertices, container_t * container){
	size_t prevtotaledgesize = 0;
	unsigned int iteration_idx = 0;
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ container->edgeoffset[i][j] = 0; }}
	container->totalnumedgesinfile[col] = lseek(graphobj->getnvmeFd_edges_r2()[col], 0, SEEK_END) / sizeof(edge_type);	
	container->numedgesretrieved[col] = 0;
	unsigned int iteration_size = utilityobj[superthreadidx]->hceildiv((lseek(graphobj->getnvmeFd_edges_r2()[col], 0, SEEK_END) / sizeof(edge_type)), KVDATA_BATCHSIZE);
	cout<<">>> WorkerThread:: total number of edges in file: "<<container->totalnumedgesinfile[col]<<endl;
	if(container->totalnumedgesinfile[col] == 0){ return; }
	
	while(true){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<<"PP&A:: [groupid:"<<globalparams.groupid<<"][col:"<<col<<"][size:"<<graphobj->getnumedgebanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:"<<NUMCPUTHREADS<<"]"<<endl;		
		#endif
		
		// load
		loadgraphdata(col, graphobj, vertexdatabuffer, container);
		helperfunctionsobj[0]->trim(container);
		helperfunctionsobj[0]->loadsourcevertices(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], container);
		helperfunctionsobj[0]->loaddestvertices(vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
		helperfunctionsobj[0]->loadedges((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer, container);
		helperfunctionsobj[0]->loadmessages((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer, container, PAGERANK);
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(col, container->edgesize[i][j]); }}
		
		// run acts
		helperfunctionsobj[superthreadidx]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], 0);
		
		if(container->numedgesretrieved[col] >= container->totalnumedgesinfile[col]){ cout<<"WorkerThread:: finished processing col. breaking out... "<<endl; break; }
		iteration_idx += NUMCPUTHREADS * NUMSUBCPUTHREADS;
		if(iteration_idx > 512){ cout<<"WorkerThread:: something wrong. too many iterations? EXITING "<<endl; exit(EXIT_FAILURE); }
		
		helperfunctionsobj[0]->cummulateverticesdata((value_t* (*)[NUMSUBCPUTHREADS])kvbuffer);
		helperfunctionsobj[0]->applyvertices(nextactivevertices, (value_t* (*)[NUMSUBCPUTHREADS])kvbuffer, col * KVDATA_RANGE_PERSSDPARTITION, KVDATA_RANGE_PERSSDPARTITION);
		// break; // REMOVEME.
		// exit(EXIT_SUCCESS); // REMOVEME.
	}
	// exit(EXIT_SUCCESS); // REMOVEME.
	return;
}

void advance_op::loadgraphdata(unsigned int col, graph * graphobj, value_t * vertexdatabuffer, container_t * container){
	unsigned int numedgesremaining = utilityobj[col]->hsub(container->totalnumedgesinfile[col], container->numedgesretrieved[col]);
	unsigned int tmp = container->numedgesretrieved[col];
	unsigned int eq;
	if(numedgesremaining > NUMCPUTHREADS * NUMSUBCPUTHREADS * KVDATA_BATCHSIZE){ eq = KVDATA_BATCHSIZE; } 
	else { eq = (numedgesremaining + (NUMSUBCPUTHREADS - 1)) / NUMSUBCPUTHREADS; }
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			container->edgesize[i][j] = utilityobj[col]->hmin(eq, utilityobj[col]->hsub(container->totalnumedgesinfile[col], tmp));
			tmp += container->edgesize[i][j];
		}
	}
	
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"loadgraphdata:: edgeoffset["<<i<<"]["<<j<<"]: "<<container->edgeoffset[i][j]<<", edgesize["<<i<<"]["<<j<<"]: "<<container->edgesize[i][j]<<endl; 
			#endif 
			loadgraphdata(col, i, j, graphobj, vertexdatabuffer, container);
			container->numedgesretrieved[col] += container->edgesize[i][j];
			
			if(j==NUMSUBCPUTHREADS-1){ container->edgeoffset[i][0] = container->edgeoffset[i][j] + container->edgesize[i][j]; }
			else { container->edgeoffset[i][j+1] += container->edgeoffset[i][j] + container->edgesize[i][j]; }
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<">>> loadgraphdata:: numedgesretrieved["<<col<<"]: "<<container->numedgesretrieved[col]<<endl;
	cout<<">>> loadgraphdata:: totalnumedgesinfile["<<col<<"]: "<<container->totalnumedgesinfile[col]<<endl;
	#endif
	return;
}
void advance_op::loadgraphdata(unsigned int col, unsigned int threadid, unsigned int subthreadid, graph * graphobj, value_t * vertexdatabuffer, container_t * container){						
	// load edges from file
	unsigned int numedgestoload = 0;
	unsigned int edgessz = container->edgesize[threadid][subthreadid];
	if(container->edgesize[threadid][subthreadid] > 0){
		if((container->edgeoffset[threadid][subthreadid] + container->edgesize[threadid][subthreadid]) >= container->totalnumedgesinfile[col]){ numedgestoload = container->edgesize[threadid][subthreadid]; } 
		else { numedgestoload = container->edgesize[threadid][subthreadid] + 1; }
	}
	if(numedgestoload > 0){ graphobj->loadedgesfromfile(col, container->edgeoffset[threadid][subthreadid], container->edgesbuffer[threadid][subthreadid], 0, numedgestoload); }
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: edges offset: "<<container->edgeoffset[threadid][subthreadid]<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: numedgestoload: "<<numedgestoload<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: first edge in edgesbuffer(*edgeoffset:"<<container->edgeoffset[threadid][subthreadid]<<"): (srcvid: "<<container->edgesbuffer[threadid][subthreadid][0].srcvid<<", dstvid: "<<container->edgesbuffer[threadid][subthreadid][0].dstvid<<")"<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: last edge in edgesbuffer(*edgeoffset:"<<container->edgeoffset[threadid][subthreadid]+numedgestoload+1<<"): (srcvid: "<<container->edgesbuffer[threadid][subthreadid][numedgestoload-1].srcvid<<", dstvid: "<<container->edgesbuffer[threadid][subthreadid][numedgestoload-1].dstvid<<")"<<endl;						
	#endif
	
	// load vertex pointers
	vertex_t beginptr;
	vertex_t lastsrcvid;
	vertex_t endptr;
	vertex_t numvertexptrstoload;
	for(unsigned int k=1; k<INFINITI; k++){
		numedgestoload = numedgestoload / k;
		edgessz = edgessz / k;
		
		beginptr = container->edgesbuffer[threadid][subthreadid][0].srcvid;
		lastsrcvid = container->edgesbuffer[threadid][subthreadid][numedgestoload-1].srcvid;
		endptr = lastsrcvid + 1; //"+1 is for edge-conditions"
		numvertexptrstoload = endptr - beginptr + 1;
		if(numvertexptrstoload <= KVDRAMSZ){ break; } 
		else { cout<<"loadgraphdata: still searching for appropriate edges size ("<<k<<")("<<numvertexptrstoload<<")... "<<endl; }
		if(k > 64){ cout<<"loadgraphdata: something wrong. too many iterations? EXITING "<<endl; exit(EXIT_SUCCESS); }
	}
	graphobj->loadvertexptrsfromfile(col, beginptr, container->vertexptrs[threadid][subthreadid], 0, numvertexptrstoload);
	
	// load variables
	container->edgesize[threadid][subthreadid] = edgessz; // numedgestoload - 1;
	container->firstvid[threadid][subthreadid] = container->edgesbuffer[threadid][subthreadid][0].srcvid;
	container->srcvoffset[threadid][subthreadid] = container->edgesbuffer[threadid][subthreadid][0].srcvid;
	container->srcvsize[threadid][subthreadid] = numvertexptrstoload;
	container->destvoffset[threadid][subthreadid] = col * KVDATA_RANGE_PERSSDPARTITION;
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: beginptr: "<<beginptr<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: lastsrcvid: "<<lastsrcvid<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: endptr: "<<endptr<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: number of edges loaded : "<<container->edgesize[threadid][subthreadid]<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: number of vertexptrs to load : "<<numvertexptrstoload<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: first data in vertexptrs(*edgeoffset:"<<beginptr<<"): vertexptrs["<<threadid<<"]["<<subthreadid<<"][0]: "<<container->vertexptrs[threadid][subthreadid][0]<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: second data in vertexptrs(*edgeoffset:"<<beginptr+1<<"): vertexptrs["<<threadid<<"]["<<subthreadid<<"][1]: "<<container->vertexptrs[threadid][subthreadid][1]<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: last data in vertexptrs(*edgeoffset:"<<endptr<<"): vertexptrs["<<threadid<<"]["<<subthreadid<<"]["<<numvertexptrstoload-1<<"]: "<<container->vertexptrs[threadid][subthreadid][numvertexptrstoload-1]<<endl;
	cout<<">>> loadvariables["<<threadid<<"]["<<subthreadid<<"]: srcvoffset["<<threadid<<"]["<<subthreadid<<"]: "<<container->srcvoffset[threadid][subthreadid]<<endl;
	cout<<">>> loadvariables["<<threadid<<"]["<<subthreadid<<"]: srcvsize["<<threadid<<"]["<<subthreadid<<"]: "<<container->srcvsize[threadid][subthreadid]<<endl;
	cout<<">>> loadvariables["<<threadid<<"]["<<subthreadid<<"]: firstvid["<<threadid<<"]["<<subthreadid<<"]: "<<container->firstvid[threadid][subthreadid]<<endl;
	#endif
	return;
}






