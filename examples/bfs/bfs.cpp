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
#include "bfs.h"
using namespace std;
#define PROCESSACTIVEVERTICESTEST

bfs::bfs(unsigned int algorithmid, unsigned int datasetid, std::string _binaryFile1, std::string _binaryFile2){
	algorithm * thisalgorithmobj = new algorithm();
	graphobj = new graph(thisalgorithmobj, datasetid, 1, true, true, true);
	statsobj = new stats(graphobj);
	algorithmobj = new algorithm();
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
	
	binaryFile[0] = _binaryFile1;
	binaryFile[1] = _binaryFile2;
	
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj->loadSRstructures();
	#endif 
}
bfs::~bfs(){
	cout<<"bfs::~bfs:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void bfs::finish(){}

runsummary_t bfs::run(){
	long double totaltime_ms = 0;
	cout<<"bfs::run:: bfs algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	// set root vid
	unsigned int NumGraphIters = 1; // 3,12
	container_t container;
	vector<value_t> activevertices;
	globalparams_t globalparams;

	activevertices.push_back(1);
	// activevertices.push_back(2);
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

	// load workload information
	globalparams.BASEOFFSETKVS_MESSAGESDATA = 0;
	
	// edges & vptrs
	globalparams = loadgraphobj->loadedges_rowblockwise(0, graphobj, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, BFS, globalparams);
	
	// vertex data
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams = loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)kvbuffer[i], 0, KVDATA_RANGE, globalparams); }
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->setrootvid((value_t *)kvbuffer[i], activevertices, globalparams); }
	
	// active vertices & masks
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams = loadgraphobj->loadactvvertices(activevertices, (keyy_t *)&kvbuffer[i], &container, globalparams); }
	globalparams = loadgraphobj->generatevmaskdata(activevertices, kvbuffer, globalparams);
	
	// workspace info 
	globalparams = loadgraphobj->loadoffsetmarkers((edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, &container, globalparams); 
	
	// messages
	globalparams = loadgraphobj->loadmessages(vdram, kvbuffer, &container, NumGraphIters, BFS, globalparams);
	
	// others
	loadgraphobj->setcustomeval(vdram, (uint512_vec_dt **)kvbuffer, 0);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
	
	// experiements
	experiements(0, NumGraphIters, 1, NumGraphIters, &container, activevertices, globalparams); // full run
	// experiements(0, 0, NumGraphIters, NumGraphIters, &container, activevertices, globalparams); // N full runs
	// experiements(2, 0, NumGraphIters, NumGraphIters, &container, activevertices, globalparams); // N full runs, isolating partition & reduce phases

	finish();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void bfs::experiements(unsigned int evalid, unsigned int start, unsigned int size, unsigned int NumGraphIters, container_t * container, vector<value_t> & activevertices, globalparams_t globalparams){
	unsigned int swnum_its = utilityobj->runsssp_sw(activevertices, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	for(unsigned int num_its=start; num_its<start+size; num_its++){
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs::run: bfs evaluation "<<num_its<<" started. (NumGraphIters: "<<NumGraphIters<<", num active vertices: "<<activevertices.size()<<")"<< RESET <<endl;

		cout<<"bfs::experiements: resetting kvdram & kvdram workspaces..."<<endl;
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.BASEOFFSETKVS_KVDRAM], KVDRAMSZ);
			utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE], KVDRAMWORKSPACESZ);
		}

		globalparams = loadgraphobj->loadmessages(vdram, kvbuffer, container, num_its, BFS, globalparams);
		loadgraphobj->setcustomeval(vdram, (uint512_vec_dt **)kvbuffer, evalid);
		
		std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs::run: bfs started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
		setupkernelobj->runapp(binaryFile, (uint512_vec_dt *)vdram, (uint512_vec_dt **)kvbuffer);
		utilityobj->stopTIME(">>> bfs::finished:. Time Elapsed: ", begintime, NAp);
		long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
		
		utilityobj->runsssp_sw(activevertices, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	
		statsobj->timingandsummary(num_its, totaltime_ms);
		if(num_its > swnum_its){ break; } 
	}
	
	verifyresults(kvbuffer[0], globalparams);
	return;
}

void bfs::verifyresults(uint512_vec_dt * kvdram, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"bfs::verifyactvvsdata: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64];
	for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	
	uint512_vec_dt buff[REDUCEBUFFERSZ];
	for(unsigned int offset_kvs=0; offset_kvs<VERTICESDATASZ_KVS; offset_kvs+=REDUCEBUFFERSZ){
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			// buff[i] = kvdram[BASEOFFSET_VERTICESDATA_KVS + offset_kvs + i];
			buff[i] = kvdram[globalparams.BASEOFFSETKVS_VERTICESDATA + offset_kvs + i];
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
					cout<<"bfs:verifyresults: vid1: "<<vid1<<",vdata1: "<<vdata1<<endl;
					#endif 
					vdatas[vdata1] += 1; 
				}
				if(vdata2 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"bfs:verifyresults: vid2: "<<vid2<<",vdata2: "<<vdata2<<endl;
					#endif
					vdatas[vdata2] += 1; 
				}
			}
		}
	}
	utilityobj->printvalues("bfs::verifyresults.vdatas: verifying results after kernel run", vdatas, 16);
	return;
}
















