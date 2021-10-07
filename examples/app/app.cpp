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
#include "../../examples/helperfunctions/loadgraph_sw.h"
#include "../../examples/helperfunctions/setupkernel.h"
#include "../../src/graphs/createundirectedgraph.h" // 
#include "../../kernels/swkernel.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "app.h"
using namespace std;

app::app(unsigned int algorithmid, unsigned int datasetid, std::string _binaryFile1, std::string _binaryFile2){
	algorithm * thisalgorithmobj = new algorithm();
	graphobj = new graph(thisalgorithmobj, datasetid, 1, true, true, true);
	statsobj = new stats(graphobj);
	algorithmobj = new algorithm();
	utilityobj = new utility();
	loadgraphobj = new loadgraph(graphobj, statsobj);
	loadgraphswobj = new loadgraph_sw(graphobj, statsobj);
	setupkernelobj = new setupkernel(graphobj, thisalgorithmobj, statsobj); 
	swkernelobj = new swkernel(graphobj, thisalgorithmobj, statsobj);

	#ifndef SW_IMPL 
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }				
	vdram = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	vdram = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	#endif
	#endif 
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ edges[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }	// 2 because edge_type consist 4 bytes not 8 bytes			
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ edges[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	#endif
	#endif 
	
	edgedatabuffer = new edge2_type[graphobj->getedgessize(0)];

	binaryFile[0] = _binaryFile1;
	binaryFile[1] = _binaryFile2;
	
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj->loadSRstructures();
	#endif
}
app::~app(){
	cout<<"app::~app:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void app::finish(){}

runsummary_t app::run(){
	#ifdef SW_IMPL 
	run_sw();
	#else 
	run_hw();
	#endif
}

runsummary_t app::run_hw(){
	cout<<"app::run_hw:: app algorithm started. "<<endl;
	long double totaltime_ms = 0;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	long double edgesprocessed_totals[128];
	long double timeelapsed_totals[128][8];
	
	// set root vid
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	unsigned int NumGraphIters = 1;
	#else 
	unsigned int NumGraphIters = 8; // 32; // 3,12,32
	#endif 
	container_t container;
	vector<value_t> actvvs;
	globalparams_TWOt globalparams;

	actvvs.push_back(1);

	// load workload information
	globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsK.SIZE_MESSAGESDRAM = 0;
	globalparams.globalparamsK.SIZE_EDGES = 0;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsK.SIZE_VERTICESDATAMASK = 0;
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = 0;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsE.SIZE_MESSAGESDRAM = 0;
	globalparams.globalparamsE.SIZE_EDGES = 0;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsE.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsE.SIZE_VERTICESDATAMASK = 0;
	globalparams.globalparamsE.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsE.SIZE_KVDRAM = 0;
	globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	#endif 
	
	// edges
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams = loadgraphobj->loadedges_rowblockwise(0, graphobj, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)edges, &container, globalparams);
	#else 
	globalparams = loadgraphobj->loadedges_rowblockwise(0, graphobj, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, globalparams);
	#endif 
	
	cout<<"app::loadvertexdata:: loading source vertex datas... "<<endl;
	globalparams.globalparamsK = loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)vdram, 666, 666, 666, globalparams.globalparamsK, 0, SOURCE);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams.globalparamsK = loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)kvbuffer[i], 666, 666, 666, globalparams.globalparamsK, 0, SOURCE); }
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams.globalparamsE = loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)edges[i], 666, 666, 666, globalparams.globalparamsE, 1, SOURCE); }
	#endif 
	
	cout<<"app::loadvertexdata:: loading dest vertex datas... "<<endl;
	loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)vdram, 666, 666, 666, globalparams.globalparamsK, 0, DEST);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams.globalparamsK = loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)kvbuffer[i], 666, 666, 666, globalparams.globalparamsK, 0, DEST); }
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams.globalparamsE = loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)edges[i], 666, 666, 666, globalparams.globalparamsE, 1, DEST); }
	#endif 
	
	cout<<"app::setrootvid:: setting root vid(s)... "<<endl;
	loadgraphobj->setrootvid((value_t *)vdram, actvvs, globalparams);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->setrootvid((value_t *)kvbuffer[i], actvvs, globalparams); }

	// active vertices & masks
	cout<<"app::loadactvvertices:: loading active vertices... "<<endl;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams = loadgraphobj->loadactvvertices(actvvs, (keyy_t *)&kvbuffer[i], &container, globalparams); }
	cout<<"app::generatevmaskdata:: generating vmask... "<<endl;
	globalparams = loadgraphobj->generatevmaskdata(actvvs, kvbuffer, globalparams);
	// exit(EXIT_SUCCESS); //////////////////////
	
	// stats info 
	cout<<"app::loadoffsetmarkers:: loading offset markers... "<<endl;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams = loadgraphobj->loadoffsetmarkers((vptr_type **)kvbuffer, (edge_type **)edges, (keyvalue_t **)kvbuffer, &container, globalparams); 
	#else 
	globalparams = loadgraphobj->loadoffsetmarkers((vptr_type **)kvbuffer, (edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, &container, globalparams); 
	#endif 
	
	// messages
	cout<<"app::loadmessages:: loading messages... "<<endl;
	globalparams = loadgraphobj->loadmessages(vdram, edges, kvbuffer, &container, NumGraphIters, 
		#ifdef PR_ALGORITHM 
		PAGERANK,
		#endif 
		#ifdef BFS_ALGORITHM  
		BFS,
		#endif 
		#ifdef SSSP_ALGORITHM 
		SSSP,
		#endif
		globalparams);
	
	// others
	cout<<"app::appendkeyvaluecount:: appending value count... "<<endl;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
	
	cout<<"app::experiements: resetting kvdram & kvdram workspaces..."<<endl;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM], globalparams.globalparamsK.SIZE_KVDRAM);
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE], globalparams.globalparamsK.SIZE_KVDRAM);
	}
	
	unsigned int numValidIters = 0;
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	numValidIters = 1;
	unsigned int total_edges_processed = graphobj->get_num_edges();
	for(unsigned int k=0; k<128; k++){ edgesprocessed_totals[k] = total_edges_processed; }
	#else
	unsigned int total_edges_processed = utilityobj->runsssp_sw(actvvs, vertexptrbuffer, edgedatabuffer, NumGraphIters, edgesprocessed_totals, &numValidIters);
	#endif
	// exit(EXIT_SUCCESS); // --------------
	
	// run_hw
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> app::run_hw: app started. ("<<actvvs.size()<<" active vertices)"<< RESET <<endl;
	long double total_time_elapsed = setupkernelobj->runapp(binaryFile, (uint512_vec_dt *)vdram, (uint512_vec_dt **)edges, (uint512_vec_dt **)kvbuffer, timeelapsed_totals, numValidIters);
	
	// output
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<NumGraphIters; i++){
		for(unsigned int analysis_i=0; analysis_i<3; analysis_i++){
			cout <<">>> app::run_hw: throughput["<<i<<"]["<<analysis_i<<"]("<<edgesprocessed_totals[i]/1000000<<"M edges,"<<timeelapsed_totals[i][analysis_i]<<" ms): "<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) * (1000))<<" edges/sec ("<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) / (1000))<<" million edges/sec)" <<endl;								
		}
	}
	#endif
	cout<<endl<< TIMINGRESULTSCOLOR << ">>> app::run_hw: THROUGHPUT SUMMARY DETAILS: "<< RESET <<endl;
	for(unsigned int i=0; i<NumGraphIters; i++){
		unsigned int total_edges = 0;
		long double total_time = 0;
		
		unsigned int analysis_begincount;
		unsigned int analysis_icount = 3;
		#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 0;
		#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 0;
		#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 2;
		#else 
		analysis_begincount = 0;
		#endif
		
		for(unsigned int analysis_i=analysis_begincount; analysis_i<analysis_icount; analysis_i++){ total_time += timeelapsed_totals[i][analysis_i]; }
		
		if(edgesprocessed_totals[i] > 0){
			for(unsigned int analysis_i=analysis_begincount; analysis_i<analysis_icount; analysis_i++){ cout<<""<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) / (1000))<<", "; }
			cout<<"[Iter "<<i<<" throughput: "<<((edgesprocessed_totals[i] / total_time) / (1000))<<"]";
			cout<<"[Iter "<<i<<" edges processed: "<<edgesprocessed_totals[i]<<"]";
			cout<<endl;
		}
	}
	cout<<endl<<">>> app::run_hw: total_edges_processed: "<<total_edges_processed<<" edges ("<<total_edges_processed/1000000<<" million edges)"<<endl;
	cout<<">>> app::run_hw: total_time_elapsed: "<<total_time_elapsed<<" ms ("<<total_time_elapsed/1000<<" s)"<<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput: "<<((total_edges_processed / total_time_elapsed) * (1000))<<" edges/sec ("<<((total_edges_processed / total_time_elapsed) / (1000))<<" million edges/sec)"<< RESET <<endl;			
	
	utilityobj->runsssp_sw(actvvs, vertexptrbuffer, edgedatabuffer, NumGraphIters, edgesprocessed_totals, &numValidIters);
	verifyresults_hw(kvbuffer[0], globalparams);
	
	finish();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}

void app::verifyresults_hw(uint512_vec_dt * kvbuffer, globalparams_TWOt globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"app::verifyresults_hw: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64];
	for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	
	uint512_vec_dt buff[REDUCEBUFFERSZ];
	for(unsigned int offset_kvs=0; offset_kvs<VERTICESDATASZ_KVS; offset_kvs+=REDUCEBUFFERSZ){
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			buff[i] = kvbuffer[globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + i];
		}
		
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				unsigned int pos = i*VECTOR_SIZE + v;
				unsigned int vid1 = offset_kvs*REDUCESZ*VECTOR_SIZE + v*REDUCESZ + i*2;
				unsigned int vid2 = vid1 + 1;
				unsigned int vdata1 = buff[i].data[v].key;
				unsigned int vdata2 = buff[i].data[v].value;
				
				if(vdata1 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"app:verifyresults_hw: vid1: "<<vid1<<",vdata1: "<<vdata1<<endl;
					#endif 
					vdatas[vdata1] += 1; 
				}
				if(vdata2 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"app:verifyresults_hw: vid2: "<<vid2<<",vdata2: "<<vdata2<<endl;
					#endif
					vdatas[vdata2] += 1; 
				}
			}
		}
	}
	utilityobj->printvalues("app::verifyresults_hw.vdatas: verifying results after kernel run", vdatas, 16);
	return;
}
















