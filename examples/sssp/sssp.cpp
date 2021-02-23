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
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "sssp.h"
using namespace std;
#define PROCESSACTIVEVERTICESTEST

sssp::sssp(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumedgebanks(), true, true, true);
	statsobj = new stats(graphobj);
	algorithmobj = new algorithm();
	parametersobj = new parameters(); 
	utilityobj = new utility();
	loadgraphobj = new loadgraph(graphobj, statsobj);
	setupkernelobj = new setupkernel(graphobj, statsobj); 

	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }				
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	#endif
	vdram = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];

	if(graphobj->getdataset().graphdirectiontype == UNDIRECTEDGRAPH){
		edgedatabuffer = new edge2_type[2 * graphobj->get_num_edges()];
	} else {
		edgedatabuffer = new edge2_type[graphobj->get_num_edges()];
	}
	
	#ifdef FPGA_IMPL
	setupkernelobj->loadOCLstructures(binaryFile, vdram, kvbuffer);
	#endif
}
sssp::~sssp(){
	cout<<"sssp::~sssp:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void sssp::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj->finishOCL();
	#endif
}

runsummary_t sssp::run(){
	long double totaltime_ms = 0;
	// #ifdef SSSP_ALGORITHM
	cout<<"sssp::run:: sssp algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	// set root vid
	unsigned int NumGraphIters = 3; // 4,6,12
	container_t container;
	vector<value_t> activevertices;

	activevertices.push_back(1);
	// for(unsigned int i=0; i<2; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<100; i++){ activevertices.push_back(i); } //
	// for(unsigned int i=0; i<500; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<2048; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<4096; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<10000; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<100000; i++){ activevertices.push_back(i); } //
	// for(unsigned int i=0; i<1000000; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<2000000; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<4000000; i++){ activevertices.push_back(i); }
	
	// load workload
	loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)vdram, 0, KVDATA_RANGE);
	#ifdef DISPATCHTYPE_SYNC
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)kvbuffer[i], 0, KVDATA_RANGE); }
	#endif
	
	loadgraphobj->loadedges_rowblockwise(0, graphobj, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, SSSP);
	
	loadgraphobj->loadoffsetmarkers((edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, &container); 
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->setrootvid((value_t *)kvbuffer[i], activevertices); }
	loadgraphobj->loadactvvertices(activevertices, (keyy_t *)vdram, &container);
	loadgraphobj->generatevmaskdata(activevertices, kvbuffer);
	
	loadgraphobj->loadmessages(vdram, kvbuffer, &container, NumGraphIters, SSSP);
	loadgraphobj->setcustomeval(vdram, (uint512_vec_dt **)kvbuffer, 0);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }

	// experiements
	experiements(0, NumGraphIters, 1, NumGraphIters, &container, activevertices); // full run
	// experiements(0, 0, NumGraphIters, NumGraphIters, &container, activevertices); // N full runs
	// experiements(2, 0, NumGraphIters, NumGraphIters, &container, activevertices); // N full runs, isolating partition & reduce phases
	
	finish();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	// #endif 
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void sssp::experiements(unsigned int evalid, unsigned int start, unsigned int size, unsigned int NumGraphIters, container_t * container, vector<value_t> & activevertices){
	unsigned int swnum_its = utilityobj->runsssp_sw(activevertices, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	for(unsigned int num_its=start; num_its<start+size; num_its++){
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> sssp::run: sssp evaluation "<<num_its<<" started. (NumGraphIters: "<<NumGraphIters<<", num active vertices: "<<activevertices.size()<<")"<< RESET <<endl;

		cout<<"sssp::experiements: resetting kvdram & kvdram workspaces..."<<endl;
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][BASEOFFSET_KVDRAM_KVS], KVDRAMSZ);
			utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][BASEOFFSET_KVDRAMWORKSPACE_KVS], KVDRAMWORKSPACESZ);
		}

		loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)vdram, 0, KVDATA_RANGE); 
		loadgraphobj->loadactvvertices(activevertices, (keyy_t *)vdram, container); 
		
		loadgraphobj->loadmessages(vdram, kvbuffer, container, num_its, SSSP);
		loadgraphobj->setcustomeval(vdram, (uint512_vec_dt **)kvbuffer, evalid);
		
		std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> sssp::run: sssp started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
		setupkernelobj->launchkernel((uint512_vec_dt *)vdram, (uint512_vec_dt **)kvbuffer, 0);
		utilityobj->stopTIME(">>> sssp::finished:. Time Elapsed: ", begintime, NAp);
		long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
		
		utilityobj->runsssp_sw(activevertices, vertexptrbuffer, edgedatabuffer, NumGraphIters); // runsssp_sw runbfs_sw
	
		statsobj->timingandsummary(num_its, totaltime_ms);
		if(num_its > swnum_its){ break; } 
	}
	verifyresults(kvbuffer[0]);
	return;
}

void sssp::verifyresults(uint512_vec_dt * kvdram){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"sssp::verifyactvvsdata: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64];
	for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	
	uint512_vec_dt buff[REDUCEBUFFERSZ];
	for(unsigned int offset_kvs=0; offset_kvs<VERTICESDATASZ_KVS; offset_kvs+=REDUCEBUFFERSZ){
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			buff[i] = kvdram[BASEOFFSET_VERTICESDATA_KVS + offset_kvs + i];
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
					cout<<"sssp:verifyresults: vid1: "<<vid1<<",vdata1: "<<vdata1<<endl;
					#endif 
					vdatas[vdata1] += 1; 
				}
				if(vdata2 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"sssp:verifyresults: vid2: "<<vid2<<",vdata2: "<<vdata2<<endl;
					#endif
					vdatas[vdata2] += 1; 
				}
			}
		}
	}
	utilityobj->printvalues("sssp::verifyresults.vdatas: verifying results after kernel run", vdatas, 16);
	return;
}
























