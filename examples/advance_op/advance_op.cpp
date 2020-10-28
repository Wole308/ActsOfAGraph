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
#include "../../src/edgeprocess/edge_process.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
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
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ edgesbuffer[i][flag][j][k] = new edge_type[KVDATA_BATCHSIZE+1]; }}}
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ vertexptrs[i][flag][j][k] = new edge_t[KVDRAMSZ+1]; }}} // KVDATA_BATCHSIZE // KVDRAMSZ
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
	delete [] edgesbuffer;
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
	
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	
	vertex_t basevoffset = 0;
	
	for(unsigned int groupid = 0; groupid < 1; groupid++){
		graphobj->openfilesforreading(groupid); //
		
		globalparams.groupbasevoffset = 0;
		globalparams.groupid = groupid;
		globalparams.graph_algorithmidx = PAGERANK;
		
		for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
			cout<< TIMINGRESULTSCOLOR <<">>> advance_op::run: graph iteration "<<graph_iterationidx<<" of advance started"<< RESET <<endl;
			
			globalparams.graph_iterationidx = graph_iterationidx;
			
			for(unsigned int col=0; col<graphobj->getnumedgebanks(); col += NUMSUPERCPUTHREADS){
			// for(unsigned int col=3; col<4; col += NUMSUPERCPUTHREADS){
				cout<<endl<< TIMINGRESULTSCOLOR << ">>> advance_op::start2: super iteration: [col: "<<col<<"][size: "<<graphobj->getnumedgebanks()<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<< RESET <<endl;
				WorkerThread(0, col, globalparams);
				cout<<">>> advance_op::start2 Finished: all threads joined..."<<endl;
				// break; // REMOVEME.
			}
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
void advance_op::WorkerThread(unsigned int superthreadidx, unsigned int col, hostglobalparams_t globalparams){
	size_t prevtotaledgesize = 0;
	unsigned int iteration_idx = 0;
	batchparams_t batchparams;
	totalsparams_t totalsparams;
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ batchparams.edgeoffset[i][j] = 0; }}
	utilityobj[superthreadidx]->setarray(batchparams.batchoffset, NUMCPUTHREADS, NUMSUBCPUTHREADS, BASEOFFSET_KVDRAMBUFFER);
	utilityobj[superthreadidx]->setarray(batchparams.batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	utilityobj[superthreadidx]->setarray(batchparams.runsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	totalsparams.totalnumedgesinfile[col] = lseek(graphobj->getnvmeFd_edges_r2()[col], 0, SEEK_END) / sizeof(edge_type);	
	totalsparams.numedgesretrieved[col] = 0;
	unsigned int iteration_size = utilityobj[superthreadidx]->hceildiv((lseek(graphobj->getnvmeFd_edges_r2()[col], 0, SEEK_END) / sizeof(edge_type)), KVDATA_BATCHSIZE);
	cout<<">>> WorkerThread:: total number of edges in file: "<<totalsparams.totalnumedgesinfile[col]<<endl;
	if(totalsparams.totalnumedgesinfile[col] == 0){ return; }
	
	while(true){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<<"PP&A:: [groupid:"<<globalparams.groupid<<"][col:"<<col<<"][size:"<<graphobj->getnumedgebanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:"<<NUMCPUTHREADS<<"]"<<endl;		
		#endif
		
		// load it
		loadgraphdata(col, vertexdatabuffer, (edge_t* (*)[NUMSUBCPUTHREADS])vertexptrs[superthreadidx][0], (edge_type* (*)[NUMSUBCPUTHREADS])edgesbuffer[superthreadidx][0], &batchparams, &totalsparams);
		loadsourcevertices(col, vertexdatabuffer, (edge_t* (*)[NUMSUBCPUTHREADS])vertexptrs[superthreadidx][0], (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], &batchparams);
		loaddestvertices(col, vertexdatabuffer, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], col * parametersobj[col]->GET_BATCH_RANGE(0), &batchparams);
		loadedges(col, (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], (edge_type* (*)[NUMSUBCPUTHREADS])edgesbuffer[superthreadidx][0], &batchparams);
		loadmessages(col, (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], &batchparams);
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(col, batchparams.edgesize[i][j]); }}
		
		// acts it
		helperfunctionsobj[superthreadidx]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[superthreadidx][0], 0);
		
		if(totalsparams.numedgesretrieved[col] >= totalsparams.totalnumedgesinfile[col]){ cout<<"WorkerThread:: finished processing col. breaking out... "<<endl; break; }
		iteration_idx += NUMCPUTHREADS * NUMSUBCPUTHREADS;
		if(iteration_idx > 512){ cout<<"WorkerThread:: something wrong. too many iterations? EXITING "<<endl; exit(EXIT_FAILURE); }
		// break; // REMOVEME.
		// exit(EXIT_SUCCESS); // REMOVEME.
	}
	// exit(EXIT_SUCCESS); // REMOVEME.
	return;
}

void advance_op::loadgraphdata(unsigned int col, value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams, totalsparams_t * totalsparams){
	unsigned int numedgesremaining = utilityobj[col]->hsub(totalsparams->totalnumedgesinfile[col], totalsparams->numedgesretrieved[col]);
	unsigned int tmp = totalsparams->numedgesretrieved[col];
	unsigned int eq;
	if(numedgesremaining > NUMCPUTHREADS * NUMSUBCPUTHREADS * KVDATA_BATCHSIZE){ eq = KVDATA_BATCHSIZE; } 
	else { eq = (numedgesremaining + (NUMSUBCPUTHREADS - 1)) / NUMSUBCPUTHREADS; }
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			batchparams->edgesize[i][j] = utilityobj[col]->hmin(eq, utilityobj[col]->hsub(totalsparams->totalnumedgesinfile[col], tmp));
			tmp += batchparams->edgesize[i][j];
		}
	}
	
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"loadgraphdata:: edgeoffset["<<i<<"]["<<j<<"]: "<<batchparams->edgeoffset[i][j]<<", edgesize["<<i<<"]["<<j<<"]: "<<batchparams->edgesize[i][j]<<endl; 
			#endif 
			loadgraphdata(col, i, j, vertexdatabuffer, vertexptrs, edgesbuffer, batchparams, totalsparams);
			totalsparams->numedgesretrieved[col] += batchparams->edgesize[i][j];
			
			if(j==NUMSUBCPUTHREADS-1){ batchparams->edgeoffset[i][0] = batchparams->edgeoffset[i][j] + batchparams->edgesize[i][j]; }
			else { batchparams->edgeoffset[i][j+1] += batchparams->edgeoffset[i][j] + batchparams->edgesize[i][j]; }
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<">>> loadgraphdata:: numedgesretrieved["<<col<<"]: "<<totalsparams->numedgesretrieved[col]<<endl;
	cout<<">>> loadgraphdata:: totalnumedgesinfile["<<col<<"]: "<<totalsparams->totalnumedgesinfile[col]<<endl;
	#endif
	return;
}
void advance_op::loadgraphdata(unsigned int col, unsigned int threadid, unsigned int subthreadid, value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams, totalsparams_t * totalsparams){						
	// load edges from file
	unsigned int numedgestoload = 0;
	unsigned int edgessz = batchparams->edgesize[threadid][subthreadid];
	if(batchparams->edgesize[threadid][subthreadid] > 0){
		if((batchparams->edgeoffset[threadid][subthreadid] + batchparams->edgesize[threadid][subthreadid]) >= totalsparams->totalnumedgesinfile[col]){ numedgestoload = batchparams->edgesize[threadid][subthreadid]; } 
		else { numedgestoload = batchparams->edgesize[threadid][subthreadid] + 1; }
	}
	if(numedgestoload > 0){ graphobj->loadedgesfromfile(col, batchparams->edgeoffset[threadid][subthreadid], edgesbuffer[threadid][subthreadid], 0, numedgestoload); }
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: edges offset: "<<batchparams->edgeoffset[threadid][subthreadid]<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: numedgestoload: "<<numedgestoload<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: first edge in edgesbuffer(*edgeoffset:"<<batchparams->edgeoffset[threadid][subthreadid]<<"): (srcvid: "<<edgesbuffer[threadid][subthreadid][0].srcvid<<", dstvid: "<<edgesbuffer[threadid][subthreadid][0].dstvid<<")"<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: last edge in edgesbuffer(*edgeoffset:"<<batchparams->edgeoffset[threadid][subthreadid]+numedgestoload+1<<"): (srcvid: "<<edgesbuffer[threadid][subthreadid][numedgestoload-1].srcvid<<", dstvid: "<<edgesbuffer[threadid][subthreadid][numedgestoload-1].dstvid<<")"<<endl;
	#endif
	
	// load vertex pointers
	vertex_t beginptr;
	vertex_t lastsrcvid;
	vertex_t endptr;
	vertex_t numvertexptrstoload;
	for(unsigned int k=1; k<INFINITI; k++){
		numedgestoload = numedgestoload / k; 
		edgessz = edgessz / k;
		
		beginptr = edgesbuffer[threadid][subthreadid][0].srcvid;
		lastsrcvid = edgesbuffer[threadid][subthreadid][numedgestoload-1].srcvid;
		endptr = lastsrcvid + 1; //"+1 is for edge-conditions"
		numvertexptrstoload = endptr - beginptr + 1;
		if(numvertexptrstoload <= KVDRAMSZ){ break; } // cout<<"loadgraphdata: found appropriate edges size ("<<numedgestoload<<"). breaking out..."<<endl; break; }
		else { cout<<"loadgraphdata: still searching for appropriate edges size ("<<k<<")("<<numvertexptrstoload<<")... "<<endl; }
		if(k > 64){ cout<<"loadgraphdata: something wrong. too many iterations? EXITING "<<endl; exit(EXIT_SUCCESS); }
	}
	graphobj->loadvertexptrsfromfile(col, beginptr, vertexptrs[threadid][subthreadid], 0, numvertexptrstoload);
	vertexptrs[threadid][subthreadid][0] = batchparams->edgeoffset[threadid][subthreadid]; // trim
	vertexptrs[threadid][subthreadid][numvertexptrstoload-1] = batchparams->edgeoffset[threadid][subthreadid] + numedgestoload - 1;
	
	// load variables
	batchparams->edgesize[threadid][subthreadid] = edgessz; // numedgestoload - 1;
	batchparams->srcvoffset[threadid][subthreadid] = edgesbuffer[threadid][subthreadid][0].srcvid;
	batchparams->srcvsize[threadid][subthreadid] = numvertexptrstoload;
	batchparams->destvoffset[threadid][subthreadid] = col * parametersobj[col]->GET_BATCH_RANGE(0);
	batchparams->beginvid[threadid][subthreadid] = edgesbuffer[threadid][subthreadid][0].srcvid;
	batchparams->beginkeyvalue[threadid][subthreadid].key = vertexptrs[threadid][subthreadid][0];
	batchparams->beginkeyvalue[threadid][subthreadid].value = vertexdatabuffer[batchparams->beginvid[threadid][subthreadid]];
	
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: beginptr: "<<beginptr<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: lastsrcvid: "<<lastsrcvid<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: endptr: "<<endptr<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: number of edges loaded : "<<batchparams->edgesize[threadid][subthreadid]<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: number of vertexptrs to load : "<<numvertexptrstoload<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: first data in vertexptrs(*edgeoffset:"<<beginptr<<"): vertexptrs["<<threadid<<"]["<<subthreadid<<"][0]: "<<vertexptrs[threadid][subthreadid][0]<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: second data in vertexptrs(*edgeoffset:"<<beginptr+1<<"): vertexptrs["<<threadid<<"]["<<subthreadid<<"][1]: "<<vertexptrs[threadid][subthreadid][1]<<endl;
	cout<<">>> loadgraphdata["<<threadid<<"]["<<subthreadid<<"]: last data in vertexptrs(*edgeoffset:"<<endptr<<"): vertexptrs["<<threadid<<"]["<<subthreadid<<"]["<<numvertexptrstoload-1<<"]: "<<vertexptrs[threadid][subthreadid][numvertexptrstoload-1]<<endl;
	cout<<">>> loadvariables["<<threadid<<"]["<<subthreadid<<"]: srcvoffset["<<threadid<<"]["<<subthreadid<<"]: "<<batchparams->srcvoffset[threadid][subthreadid]<<endl;
	cout<<">>> loadvariables["<<threadid<<"]["<<subthreadid<<"]: srcvsize["<<threadid<<"]["<<subthreadid<<"]: "<<batchparams->srcvsize[threadid][subthreadid]<<endl;
	cout<<">>> loadvariables["<<threadid<<"]["<<subthreadid<<"]: beginvid["<<threadid<<"]["<<subthreadid<<"]: "<<batchparams->beginvid[threadid][subthreadid]<<endl;
	cout<<">>> loadvariables["<<threadid<<"]["<<subthreadid<<"]: beginkeyvalue["<<threadid<<"]["<<subthreadid<<"].key: "<<batchparams->beginkeyvalue[threadid][subthreadid].key<<endl;
	cout<<">>> loadvariables["<<threadid<<"]["<<subthreadid<<"]: beginkeyvalue["<<threadid<<"]["<<subthreadid<<"].value: "<<batchparams->beginkeyvalue[threadid][subthreadid].value<<endl;
	#endif
	return;
}
void advance_op::loadsourcevertices(unsigned int col, value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int vid = 0; vid < batchparams->srcvsize[i][j]; vid++){
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + vid].key = vertexptrs[i][j][vid];
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + vid].value = vertexdatabuffer[batchparams->beginvid[i][j] + vid]; // 10000000 + vid; // vertexdatabuffer[vid]; // 10000000 + k;
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadsourcevertices["<<i<<"]["<<j<<"]: first data in kvbuffer->vertices[0]: key: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE].value<<""<<endl;
			cout<<">>> loadsourcevertices["<<i<<"]["<<j<<"]: last data in kvbuffer->vertices["<<batchparams->srcvsize[i][j] - 1<<"]: key: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + batchparams->srcvsize[i][j] - 1].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + batchparams->srcvsize[i][j] - 1].value<<""<<endl;			
			#endif 
		}
	}
	return;
}
void advance_op::loaddestvertices(unsigned int col, value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], size_t offset, batchparams_t * batchparams){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			memcpy(&kvbuffer[i][j][BASEOFFSET_VERTICESDATA], &vertexdatabuffer[offset], parametersobj[col]->GET_BATCH_RANGE(0) * sizeof(value_t));
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loaddestvertices["<<i<<"]["<<j<<"]: first data in dest vertices[0]: key: "<<kvbuffer[i][j][BASEOFFSET_VERTICESDATA].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_VERTICESDATA].value<<" (vertex data)"<<endl;
			cout<<">>> loaddestvertices["<<i<<"]["<<j<<"]: last data in dest vertices["<<parametersobj[col]->GET_BATCH_RANGE(0)/2 - 1<<"]: key: "<<kvbuffer[i][j][BASEOFFSET_VERTICESDATA + parametersobj[col]->GET_BATCH_RANGE(0)/2 - 1].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_VERTICESDATA + parametersobj[col]->GET_BATCH_RANGE(0)/2 - 1].value<<" (vertex data)"<<endl;
			#endif 
		}
	}
	return;
}
void advance_op::loadedges(unsigned int col, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int k=0; k<batchparams->edgesize[i][j]; k++){
				kvbuffer[i][j][BASEOFFSET_KVDRAM + k].key = edgesbuffer[i][j][k].dstvid;
				kvbuffer[i][j][BASEOFFSET_KVDRAM + k].value = edgesbuffer[i][j][k].srcvid;
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadedges["<<i<<"]["<<j<<"]: first data: kvbuffer["<<i<<"]["<<j<<"]["<<BASEOFFSET_KVDRAM<<"].key: "<<kvbuffer[i][j][BASEOFFSET_KVDRAM].key<<" (edge dstvid), kvbuffer["<<i<<"]["<<j<<"]["<<BASEOFFSET_KVDRAM<<"].value: "<<kvbuffer[i][j][BASEOFFSET_KVDRAM].value<<" (edges srcvid)"<<endl;
			#endif 
		}
	}
	return;
}
void advance_op::loadmessages(unsigned int col, uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams){			
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
			if(batchparams->srcvsize[i][j] >= KVDRAMSZ){ cout<<"advance_op::run::ERROR: fix this. srcvsize is greater than allowed. srcvsize["<<i<<"]["<<j<<"]: "<<batchparams->srcvsize[i][j]<<", KVDRAMSZ: "<<KVDRAMSZ<<endl; exit(EXIT_FAILURE); }
			helperfunctionsobj[0]->createmessages(
					kvbuffer[i][j],
					batchparams->srcvoffset[i][j], // unsigned int srcvoffset,
					batchparams->srcvsize[i][j], // unsigned int srcvsize,
					batchparams->destvoffset[i][j], // unsigned int destvoffset,
					batchparams->beginvid[i][j], // unsigned int beginvid,
					batchparams->beginkeyvalue[i][j].key, // unsigned int beginkey,
					batchparams->beginkeyvalue[i][j].value, // unsigned int beginvalue,
					TREE_DEPTH, // unsigned int treedepth,
					0, // unsigned int GraphIter,
					PAGERANK, // unsigned int GraphAlgo,
					batchparams->edgesize[i][j], // unsigned int runsize,
					parametersobj[col]->GET_BATCH_RANGE(0), // unsigned int batch_range,
					parametersobj[col]->GET_BATCH_RANGE_POW(0), // unsigned int batch_range_pow,
					APPLYVERTEXBUFFERSZ, // unsigned int applyvertexbuffersz,
					NUMLASTLEVELPARTITIONS); // unsigned int numlastlevelpartitions
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"loadmessages:: running Acts... size: "<<batchparams->edgesize[i][j]<<endl; 
			#endif 
		}
	}
	return;
}








