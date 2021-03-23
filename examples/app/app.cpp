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
#include "app.h"
using namespace std;

app::app(unsigned int algorithmid, unsigned int datasetid, std::string _binaryFile1, std::string _binaryFile2){
	algorithm * thisalgorithmobj = new algorithm();
	graphobj = new graph(thisalgorithmobj, datasetid, 1, true, true, true);
	statsobj = new stats(graphobj);
	algorithmobj = new algorithm();
	utilityobj = new utility();
	loadgraphobj = new loadgraph(graphobj, statsobj);
	setupkernelobj = new setupkernel(graphobj, statsobj); 

	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }				
	vdram = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	vdram = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	#endif

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
app::~app(){
	cout<<"app::~app:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void app::finish(){}

runsummary_t app::run(){
	cout<<"app::run:: app algorithm started. "<<endl;
	long double totaltime_ms = 0;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	// set root vid
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	unsigned int NumGraphIters = 1;
	#else 
	unsigned int NumGraphIters = 2; // 3,12,32
	#endif 
	container_t container;
	vector<value_t> activevertices;
	globalparams_t globalparams;

	activevertices.push_back(1);

	// load workload information
	globalparams.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.BASEOFFSETKVS_VERTICESDATA = 0;
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.SIZE_MESSAGESDRAM = 0;
	globalparams.SIZE_EDGES = 0;
	globalparams.SIZE_VERTEXPTRS = 0;
	globalparams.SIZE_VERTICESDATA = 0;
	globalparams.SIZE_ACTIVEVERTICES = 0;
	globalparams.SIZE_VERTICESDATAMASK = 0;
	globalparams.SIZE_KVSTATSDRAM = 0;
	globalparams.SIZE_KVDRAM = 0;
	globalparams.SIZE_KVDRAMWORKSPACE = 0;
	
	globalparams = loadgraphobj->loadedges_rowblockwise(0, graphobj, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, globalparams);
	
	// vertex data
	cout<<"app::loadvertexdata:: loading vertex datas... "<<endl;
	loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)vdram, 0, KVDATA_RANGE, globalparams);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams = loadgraphobj->loadvertexdata(vertexdatabuffer, (keyvalue_t *)kvbuffer[i], 0, KVDATA_RANGE, globalparams); }
	cout<<"app::setrootvid:: setting root vid(s)... "<<endl;
	loadgraphobj->setrootvid((value_t *)vdram, activevertices, globalparams);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->setrootvid((value_t *)kvbuffer[i], activevertices, globalparams); }

	// active vertices & masks
	cout<<"app::loadactvvertices:: loading active vertices... "<<endl;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams = loadgraphobj->loadactvvertices(activevertices, (keyy_t *)&kvbuffer[i], &container, globalparams); }
	cout<<"app::generatevmaskdata:: generating vmask... "<<endl;
	globalparams = loadgraphobj->generatevmaskdata(activevertices, kvbuffer, globalparams);
	
	// workspace info 
	cout<<"app::loadoffsetmarkers:: loading offset markers... "<<endl;
	globalparams = loadgraphobj->loadoffsetmarkers((edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, &container, globalparams); 
	
	// messages
	cout<<"app::loadmessages:: loading messages... "<<endl;
	globalparams = loadgraphobj->loadmessages(vdram, kvbuffer, &container, NumGraphIters, 
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
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.BASEOFFSETKVS_KVDRAM], globalparams.SIZE_KVDRAM);
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE], globalparams.SIZE_KVDRAM);
	}
	
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	unsigned int total_edges_processed = graphobj->get_num_edges();
	#else 
	unsigned int total_edges_processed = utilityobj->runsssp_sw(activevertices, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	#endif 

	// run
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> app::run: app started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
	long double total_time_elapsed = setupkernelobj->runapp(binaryFile, (uint512_vec_dt *)vdram, (uint512_vec_dt **)kvbuffer);
	
	cout<<">>> app::run: total_edges_processed: "<<total_edges_processed<<" edges ("<<total_edges_processed/1000000<<" million edges)"<<endl;
	cout<<">>> app::run: total_time_elapsed: "<<total_time_elapsed<<" ms ("<<total_time_elapsed/1000<<" s)"<<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> app::run: throughput: "<<((total_edges_processed / total_time_elapsed) * (1000))<<" edges/sec ("<<((total_edges_processed / total_time_elapsed) / (1000))<<" million edges/sec)"<< RESET <<endl;
	
	utilityobj->runsssp_sw(activevertices, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	verifyresults(kvbuffer[0], globalparams);
	
	finish();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}

void app::verifyresults(uint512_vec_dt * kvdram, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"app::verifyactvvsdata: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64];
	for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	
	uint512_vec_dt buff[REDUCEBUFFERSZ];
	for(unsigned int offset_kvs=0; offset_kvs<VERTICESDATASZ_KVS; offset_kvs+=REDUCEBUFFERSZ){
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
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
					cout<<"app:verifyresults: vid1: "<<vid1<<",vdata1: "<<vdata1<<endl;
					#endif 
					vdatas[vdata1] += 1; 
				}
				if(vdata2 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"app:verifyresults: vid2: "<<vid2<<",vdata2: "<<vdata2<<endl;
					#endif
					vdatas[vdata2] += 1; 
				}
			}
		}
	}
	utilityobj->printvalues("app::verifyresults.vdatas: verifying results after kernel run", vdatas, 16);
	return;
}
















