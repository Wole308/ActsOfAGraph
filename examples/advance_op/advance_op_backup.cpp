#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
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

#define BLOCKH 512 // 488

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
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ edgeprocessobj[i] = new edge_process(graphobj, statsobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i] = new helperfunctions2(graphobj, statsobj); }
	
	rank_curr = new value_t[graphobj->get_num_vertices()];
	rank_next = new value_t[graphobj->get_num_vertices()];
	degrees = new value_t[graphobj->get_num_vertices()];
	
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
}
advance_op::~advance_op(){
	cout<<"advance_op::~advance_op:: finish destroying memory structures... "<<endl;
	delete [] edgesbuffer;
	delete [] kvbuffer;
}
void advance_op::finish(){
	#if (defined(FPGA_IMPL) && defined(PR_ALGORITHM))
	helperfunctionsobj[0]->finishOCL();
	#endif
}

/** runsummary_t advance_op::run(){
	cout<<"advance_op::run:: advance_op algorithm started. "<<endl;
	graphobj->openfilesforreading(0);
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	graphobj->generateverticesdata();
	graphobj->generatevertexproperties(); 
	graphobj->loadvertexpropertiesfromfile(); 
	graphobj->loadvertexdatafromfile();
	vertexpropertybuffer = graphobj->getvertexpropertybuffer();
	vertexdatabuffer = graphobj->getvertexdatabuffer();

	// load it
	loadgraphdata(0, 0, (edge_t* (*)[NUMSUBCPUTHREADS])vertexptrs[0][0], (edge_type* (*)[NUMSUBCPUTHREADS])edgesbuffer[0][0], beginvid[0][0], vsize[0][0]);
	loadactsvertices(vertexdatabuffer, (edge_t* (*)[NUMSUBCPUTHREADS])vertexptrs[0][0], (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], beginvid[0][0], vsize[0][0]);
	loadactskeyvalues((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], (edge_type* (*)[NUMSUBCPUTHREADS])edgesbuffer[0][0]);
	loadactsmessages((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], (edge_type* (*)[NUMSUBCPUTHREADS])edgesbuffer[0][0], beginvid[0][0], vsize[0][0]);
	
	// acts it!
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	helperfunctionsobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], 0);
	utilityobj[0]->stopTIME("advance_op:: finished. Time Elapsed: ", begintime, NAp);
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return;
} */

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
			
			for(unsigned int i_batch=0; i_batch<graphobj->getnumedgebanks(); i_batch += NUMSUPERCPUTHREADS){
			// for(unsigned int i_batch=7; i_batch<graphobj->getnumedgebanks(); i_batch += NUMSUPERCPUTHREADS){
			// for(unsigned int i_batch=0; i_batch<1; i_batch += NUMSUPERCPUTHREADS){
				cout<<endl<< TIMINGRESULTSCOLOR << ">>> advance_op::start2: super iteration: [i_batch: "<<i_batch<<"][size: "<<graphobj->getnumedgebanks()<<"][step: "<<NUMSUPERCPUTHREADS<<"]"<< RESET <<endl;
				for (int i = 0; i < NUMSUPERCPUTHREADS; i++) { WorkerThread(i, i_batch, globalparams); }
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
	
	utilityobj[0]->stopTIME("advance::start2: finished start2. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void advance_op::WorkerThread(int superthreadidx, int threadidxoffset, hostglobalparams_t globalparams){
	unsigned int edgeoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int edgesize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int runsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]; 
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ edgeoffset[i][j] = 0; }}
	utilityobj[superthreadidx]->setarray(batchoffset, NUMCPUTHREADS, NUMSUBCPUTHREADS, BASEOFFSET_KVDRAMBUFFER);
	utilityobj[superthreadidx]->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	utilityobj[superthreadidx]->setarray(runsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	edge_t edgepropertyfilesize = lseek(graphobj->getnvmeFd_edges_r2()[threadidxoffset + superthreadidx], 0, SEEK_END) / sizeof(edge_type);			
	unsigned int iteration_size = utilityobj[superthreadidx]->hceildiv((lseek(graphobj->getnvmeFd_edges_r2()[threadidxoffset + superthreadidx], 0, SEEK_END) / sizeof(edge_type)), KVDATA_BATCHSIZE);
	
	for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx += NUMCPUTHREADS){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<endl<<"PP&A:: [groupid:"<<globalparams.groupid<<"][superthreadidx:"<<(threadidxoffset + superthreadidx)<<"][size:"<<graphobj->getnumedgebanks()<<"][step:"<<NUMSUPERCPUTHREADS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:"<<NUMCPUTHREADS<<"]"<<endl;		
		#endif
		
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ edgesize[i][j] = utilityobj[superthreadidx]->hmin(KVDATA_BATCHSIZE, utilityobj[superthreadidx]->hsub((size_t)edgepropertyfilesize, (size_t)((size_t)(iteration_idx + i*NUMSUBCPUTHREADS + j) * (size_t)KVDATA_BATCHSIZE))); }}
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ statsobj->appendkeyvaluecount(threadidxoffset + superthreadidx, edgesize[i][j]); }}
	
		// load it
		loadgraphdata(threadidxoffset + superthreadidx, edgeoffset, edgesize, (edge_t* (*)[NUMSUBCPUTHREADS])vertexptrs[0][0], (edge_type* (*)[NUMSUBCPUTHREADS])edgesbuffer[0][0], beginvid[0][0], voffset[0][0], vsize[0][0]);
		loadvariables((edge_type* (*)[NUMSUBCPUTHREADS])edgesbuffer[0][0], edgesize, beginvid[0][0], voffset[0][0], vsize[0][0]);
		loadactsvertices(vertexdatabuffer, (edge_t* (*)[NUMSUBCPUTHREADS])vertexptrs[0][0], (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], beginvid[0][0], vsize[0][0]);
		loadvariables2(edgeoffset, (edge_t* (*)[NUMSUBCPUTHREADS])vertexptrs[0][0], beginvid[0][0], beginkeyvalue[0][0]);
		loadactskeyvalues((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], (edge_type* (*)[NUMSUBCPUTHREADS])edgesbuffer[0][0], edgesize);
		loadactsmessages((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], edgesize, beginvid[0][0], beginkeyvalue[0][0], voffset[0][0], vsize[0][0]);

		// update
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ edgeoffset[i][j] += edgesize[i][j]; }}

		// acts it
		std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
		helperfunctionsobj[0]->launchkernel((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], 0);
		utilityobj[0]->stopTIME("advance_op:: finished. Time Elapsed: ", begintime, NAp);
		// break; // REMOVEME.
		exit(EXIT_SUCCESS);
	}
	// break; // REMOVEME.
	// exit(EXIT_SUCCESS);
	return;
}

void advance_op::loadgraphdata(unsigned int col, edge_t edgeoffset[NUMCPUTHREADS], edge_t edgesize[NUMCPUTHREADS], edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int vsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]){						
	// end-of-file edge condition
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			if(edgesize[i] < KVDATA_BATCHSIZE){ edgesize[i] -= 1; }
		}
	}
	
	// load edges from file
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			graphobj->loadedgesfromfile(col, edgeoffset[i], edgesbuffer[i][j], 0, edgesize[i]+1); 
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadgraphdata: first edge in edgesbuffer(*edgeoffset:"<<edgeoffset[i]<<"): (srcvid: "<<edgesbuffer[i][j][0].srcvid<<", dstvid: "<<edgesbuffer[i][j][0].dstvid<<")"<<endl;
			cout<<">>> loadgraphdata: last edge in edgesbuffer(*edgeoffset:"<<edgeoffset[i]+edgesize[i]+1<<"): (srcvid: "<<edgesbuffer[i][j][edgesize[i]].srcvid<<", dstvid: "<<edgesbuffer[i][j][edgesize[i]].dstvid<<")"<<endl;
			#endif 
		}
	}
	
	// load vertex pointers
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			vertex_t beginptr;
			vertex_t lastsrcvid;
			vertex_t endptr;
			vertex_t sizetoload;
			
			for(unsigned int k=1; k<INFINITI; k++){ // FIXME.
				edgesize[i] = edgesize[i] / k; 
				
				beginptr = edgesbuffer[i][j][0].srcvid;
				lastsrcvid = edgesbuffer[i][j][edgesize[i]].srcvid;
				endptr = lastsrcvid + 1; //"+1 is for edge-conditions"
				sizetoload = endptr - beginptr + 1;
				if(sizetoload <= KVDRAMSZ){ cout<<"loadgraphdata: found appropriate edges size ("<<edgesize[i]<<"). breaking out..."<<endl; break; }
				else { cout<<"loadgraphdata: still searching for appropriate edges size... "<<endl; }
				if(k > 64){ cout<<"loadgraphdata: something wrong. too many iterations? EXITING "<<endl; exit(EXIT_SUCCESS); }
			}
			
			utilityobj[0]->checkoutofbounds("advance_op::loadgraphdata 23", sizetoload, KVDRAMSZ, NAp, NAp, NAp);
			graphobj->loadvertexptrsfromfile(col, beginptr, vertexptrs[i][j], 0, sizetoload);
			vertexptrs[i][j][0] = edgeoffset[i]; // trim
			vertexptrs[i][j][sizetoload-1] = edgeoffset[i] + edgesize[i];
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadgraphdata: beginptr: "<<beginptr<<endl;
			cout<<">>> loadgraphdata: lastsrcvid: "<<lastsrcvid<<endl;
			cout<<">>> loadgraphdata: endptr: "<<endptr<<endl;
			cout<<">>> loadgraphdata: number of edges loaded : "<<edgesize[i]<<endl;
			cout<<">>> loadgraphdata: number of vertexptrs to load : "<<sizetoload<<endl;
			cout<<">>> loadgraphdata: first data in vertexptrs(*edgeoffset:"<<beginptr<<"): vertexptrs["<<i<<"]["<<j<<"][0]: "<<vertexptrs[i][j][0]<<endl;
			cout<<">>> loadgraphdata: second data in vertexptrs(*edgeoffset:"<<beginptr+1<<"): vertexptrs["<<i<<"]["<<j<<"][1]: "<<vertexptrs[i][j][1]<<endl;
			cout<<">>> loadgraphdata: last data in vertexptrs(*edgeoffset:"<<endptr<<"): vertexptrs["<<i<<"]["<<j<<"]["<<sizetoload-1<<"]: "<<vertexptrs[i][j][sizetoload-1]<<endl;
			#endif
		}
	}
	return;
}
void advance_op::loadvariables(edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int vsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]){						
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			voffset[i][j] = edgesbuffer[i][j][0].srcvid;
			vsize[i][j] = ((edgesbuffer[i][j][edgesize[i]].srcvid + 1) - edgesbuffer[i][j][0].srcvid) + 1;
			beginvid[i][j] = edgesbuffer[i][j][0].srcvid;
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadvariables: voffset["<<i<<"]["<<j<<"]: "<<voffset[i][j]<<endl;
			cout<<">>> loadvariables: vsize["<<i<<"]["<<j<<"]: "<<vsize[i][j]<<endl;
			cout<<">>> loadvariables: beginvid["<<i<<"]["<<j<<"]: "<<beginvid[i][j]<<endl;
			#endif 
		}
	}
	return;
}
void advance_op::loadactsvertices(value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int vsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int vid = 0; vid < vsize[i][j]; vid++){
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + vid].key = vertexptrs[i][j][vid];
				kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + vid].value = vertexdatabuffer[beginvid[i][j] + vid]; // 10000000 + vid; // vertexdatabuffer[vid]; // 10000000 + k;
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadactsvertices: first data in kvbuffer->vertices[0]: key: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE].value<<" (vertex data)"<<endl;
			cout<<">>> loadactsvertices: last data in kvbuffer->vertices["<<vsize[i][j] - 1<<"]: key: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + vsize[i][j] - 1].key<<" (vertexptr), value: "<<kvbuffer[i][j][BASEOFFSET_KVDRAMWORKSPACE + vsize[i][j] - 1].value<<" (vertex data)"<<endl;
			#endif 
		}
	}
	return;
}
void advance_op::loadvariables2(edge_t edgeoffset[NUMCPUTHREADS], edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t beginkeyvalue[NUMCPUTHREADS][NUMSUBCPUTHREADS]){						
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			beginkeyvalue[i][j].key = vertexptrs[i][j][0];
			beginkeyvalue[i][j].value = vertexdatabuffer[beginvid[i][j]];
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadvariables2: beginkeyvalue["<<i<<"]["<<j<<"].key: "<<beginkeyvalue[i][j].key<<endl;
			cout<<">>> loadvariables2: beginkeyvalue["<<i<<"]["<<j<<"].value: "<<beginkeyvalue[i][j].value<<endl;
			#endif 
		}
	}
	return;
}
void advance_op::loadactskeyvalues(keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS]){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			for(unsigned int k=0; k<edgesize[i]; k++){
				kvbuffer[i][j][BASEOFFSET_KVDRAM + k].key = edgesbuffer[i][j][k].dstvid;
				kvbuffer[i][j][BASEOFFSET_KVDRAM + k].value = edgesbuffer[i][j][k].srcvid;
			}
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<">>> loadactskeyvalues: first data in kvbuffer->keyvalues: kvbuffer["<<i<<"]["<<j<<"]["<<BASEOFFSET_KVDRAM<<"].key: "<<kvbuffer[i][j][BASEOFFSET_KVDRAM].key<<" (edge dstvid), kvbuffer["<<i<<"]["<<j<<"]["<<BASEOFFSET_KVDRAM<<"].value: "<<kvbuffer[i][j][BASEOFFSET_KVDRAM].value<<" (edges srcvid)"<<endl;
			#endif 
		}
	}
	return;
}
void advance_op::loadactsmessages(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t beginkeyvalue[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int vsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
			if(vsize[i][j] >= KVDRAMSZ){ cout<<"advance_op::run::ERROR: fix this. vsize is greater than allowed. vsize["<<i<<"]["<<j<<"]: "<<vsize[i][j]<<", KVDRAMSZ: "<<KVDRAMSZ<<endl; exit(EXIT_FAILURE); }
			helperfunctionsobj[0]->createmessages(
					kvbuffer[i][j],
					voffset[i][j], // 0, // FIXME.REMOVEME. //0, // unsigned int voffset,
					vsize[i][j], // BATCH_RANGE, // NAp, // unsigned int vsize,
					beginvid[i][j], // unsigned int beginvid,
					beginkeyvalue[i][j].key, // unsigned int beginkey,
					beginkeyvalue[i][j].value, // unsigned int beginvalue,
					TREE_DEPTH, // unsigned int treedepth,
					0, // unsigned int GraphIter,
					PAGERANK, // unsigned int GraphAlgo,
					edgesize[i], // unsigned int runsize,
					BATCH_RANGE, // unsigned int batch_range,
					BATCH_RANGE_POW, // unsigned int batch_range_pow,
					APPLYVERTEXBUFFERSZ, // unsigned int applyvertexbuffersz,
					NUMLASTLEVELPARTITIONS); // unsigned int numlastlevelpartitions
		}
	}
	return;
}










