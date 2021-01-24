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
	postprocessobj = new postprocess(graphobj, statsobj); 
	loadgraphobj = new loadgraph(graphobj, statsobj); 
	// compactgraphobj = new compactgraph(graphobj, statsobj);
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
	setupkernelobj->loadOCLstructures(binaryFile, kvbuffer);
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
	cout<<"sssp::run:: sssp algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	tempvertexdatabuffer = graphobj->generatetempverticesdata();
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	// set root vid
	unsigned int NumGraphIters = 6; // 4,6
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
	vector<value_t> activevertices2;
	for(unsigned int i=0; i<activevertices.size(); i++){ activevertices2.push_back(activevertices[i]); }
	
	// load workload
	loadgraphobj->loadvertexdata(tempvertexdatabuffer, (keyvalue_t *)kvbuffer[0], 0, KVDATA_RANGE);
	#ifndef COMPACTEDGES // FIXME. NOTNEAT.
	loadgraphobj->loadedges_rowwise(0, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, &container, SSSP);
	loadgraphobj->loadoffsetmarkers((edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, &container);
	loadgraphobj->loadactvvertices(activevertices, (keyy_t *)kvbuffer[0], &container);
	#endif
	loadgraphobj->loadmessages(vdram, kvbuffer, &container, NumGraphIters, BREADTHFIRSTSEARCH);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
	
	// run sssp
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> sssp::run: sssp started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;

	setupkernelobj->launchkernel((uint512_vec_dt *)vdram, (uint512_vec_dt **)kvbuffer, 0);

	utilityobj->stopTIME(">>> sssp::finished:. Time Elapsed: ", begintime, NAp);
	long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	
	// verify 
	verify(activevertices);
	// utilityobj->runbfs_sw(activevertices2, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	utilityobj->runsssp_sw(activevertices2, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	verifyvertexdata((keyvalue_t **)kvbuffer);
	verifyactvvsdata((keyvalue_t **)kvbuffer);
	verifykernelreturnvalues(kvbuffer);
	
	finish();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	return statsobj->timingandsummary(NAp, totaltime_ms);
}

void sssp::verify(vector<vertex_t> &activevertices){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"sssp::verify. verifying..."<<endl;
	#endif 
	#if defined(PROCESSACTIVEVERTICESTEST) //&& defined(ENABLE_PERFECTACCURACY)
	unsigned int edges1_count = 0;
	unsigned int edgesdstv1_sum = 0;
	unsigned int edges2_count = 0;
	unsigned int edgesdstv2_sum = 0;
	unsigned int edges3_count = 0;
	unsigned int edgesdstv3_sum = 0;
	unsigned int edges4_count = 0;
	unsigned int edgesdstv4_sum = 0;
	unsigned int edges5_count = 0;
	unsigned int edgesdstv5_sum = 0;
	unsigned int actvvs_count = 0;
	unsigned int actvvs_verbosecount = 0;
	keyy_t keys[COMPACTPARAM_ITEMSIZE_TOTALDATA];
	unsigned int CLOP = kvbuffer[0][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key - 1;
	if(CLOP == TREE_DEPTH+1){ CLOP = TREE_DEPTH; } // exclude reduce phase
	
	// 1st check (scanning edges in file...)
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	utilityobj->collectedgestats(activevertices, vertexptrbuffer, edgedatabuffer, &edges1_count, &edgesdstv1_sum);
	
	// 2nd check (stats collected during acts.procactvvs stage)
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		edges2_count += kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[0].key; 
		edgesdstv2_sum += kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[1].key; 
	}
	
	// 3rd check (stats collected after acts.procactvvs stage)
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		unsigned int sz = kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[2].key;
		if(sz > KVDRAMSZ){ cout<<"ERROR: something wrong (sz("<<sz<<") > KVDRAMSZ("<<KVDRAMSZ<<")). exiting... "<<endl; exit(EXIT_FAILURE); }
		for(unsigned int j=0; j<sz; j++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				keyvalue_t keyvalue = kvbuffer[i][BASEOFFSET_KVDRAM_KVS + j].data[v];
				
				if(keyvalue.key != INVALIDDATA && keyvalue.value != INVALIDDATA){
					edges3_count += 1;
					edgesdstv3_sum += keyvalue.key;
				}
			}
		}
	}
	if(CLOP != 1){ edges3_count = NAp; edgesdstv3_sum = NAp; }
	
	// 4th check (checking edges in acts.LLOP...)
	verifykvLOP((keyvalue_t **)kvbuffer, kvbuffer, CLOP, &edges4_count, &edgesdstv4_sum);
	
	// 5th check (stats collected during acts.reduce phase)
	for(unsigned int i=0; i<1; i++){ 
		edges5_count += kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[3].key; 
		edgesdstv5_sum += kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[4].key; 
	}

	// 6th check 
	unsigned int actvvsdstv1_sum = 0;
	unsigned int cctv = 0;
	for(unsigned int i=0; i<1; i++){ // NUMSUBCPUTHREADS
		unsigned int sz = kvbuffer[i][PADDEDKVSOURCEDRAMSZ_KVS-1].data[5].key;
		// cout<<"~~~~~~~~~~~~~~~~~~~~~~~ sz: "<<sz<<endl;
		if(i==0){ actvvs_verbosecount = sz; }
		keyy_t * KK = (keyy_t *)&kvbuffer[i][BASEOFFSET_ACTIVEVERTICES_KVS];
		unsigned int localactvvs_count = 0;
		unsigned int localactvvsdstv1_sum = 0;
		for(unsigned int k=0; k<sz; k++){
			// cout<<"*************** sssp:verify: actvvid: "<<KK[k]<<endl;
			if(KK[k] != INVALIDDATA){
				if(i==0){
					#ifdef _DEBUGMODE_HOSTPRINTS3
					if(cctv < 16){ cout<<"sssp:verify: actvvid: "<<KK[k]<<endl; }
					#endif 
					actvvs_count += 1;
					actvvsdstv1_sum += KK[k];
				}
				localactvvs_count += 1;
				localactvvsdstv1_sum += KK[k]; 
				cctv += 1;
			}
		}
		cout<<"sssp::verifyactvvs: num actvvs found in kvbuffer["<<i<<"]: localactvvs_count: "<<localactvvs_count<<", localactvvsdstv1_sum: "<<localactvvsdstv1_sum<<endl;
	}
	
	// 7th view
	for(unsigned int i=0; i<0; i++){ // NUMSUBCPUTHREADS
		cout<<endl<<"[7th view]: subthread: "<<i<<endl;
		keyy_t * KK = (keyy_t *)&kvbuffer[0][BASEOFFSET_ACTIVEVERTICES_KVS];
		for(unsigned int k=0; k<4; k++){
			if(KK[k] != INVALIDDATA){
				#ifdef _DEBUGMODE_HOSTPRINTS3
				cout<<"sssp:verify: actvvid: "<<KK[k]<<endl;
				#endif
			}
		}
	}
	
	cout<<"+++++++++++++++++++++++++++++ sssp:verify (offchip, edges in file               ) edges1_count: "<<edges1_count<<", edgesdstv1_sum: "<<edgesdstv1_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ sssp:verify (onchip, during acts.procactvvs stage ) edges2_count: "<<edges2_count<<", edgesdstv2_sum: "<<edgesdstv2_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ sssp:verify (inkvdram, after acts.procactvvs stage) edges3_count: "<<edges3_count<<", edgesdstv3_sum: "<<edgesdstv3_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ sssp:verify (inkvdram, after CLOP="<<CLOP<<" stage         ) edges4_count: "<<edges4_count<<", edgesdstv4_sum: "<<edgesdstv4_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ sssp:verify (inkvdram, after acts.reduce stage    ) edges5_count: "<<edges5_count<<", edgesdstv5_sum: "<<edgesdstv5_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ sssp:verify (onchip, active vertices for next it  ) actvvs_count: "<<actvvs_count<<" (actvvs_verbosecount:"<<actvvs_verbosecount<<", actvvsdstv1_sum:"<<actvvsdstv1_sum<<")"<<endl;
	
	if(kvbuffer[0][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key > 1){ edges1_count = edges2_count; }
	
	#if defined(_DEBUGMODE_HOSTCHECKS2) && not defined(HW)
	if(CLOP == 1){
		if(edges1_count != edges2_count || edges1_count != edges3_count || edges1_count != edges4_count){ cout<<"sssp::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }
		if((edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv3_sum || edgesdstv1_sum != edgesdstv4_sum) && false){ cout<<"sssp::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }							
	} else if(CLOP == TREE_DEPTH){
		if(edges1_count != edges2_count || edges1_count != edges4_count || edges1_count != edges5_count){ cout<<"sssp::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }
		if((edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv4_sum || edgesdstv1_sum != edgesdstv5_sum) && false){ cout<<"sssp::verify: INEQUALITY ERROR: edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv3_sum || edgesdstv1_sum != edgesdstv4_sum. ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }							
	} else {
		if(edges1_count != edges2_count || edges1_count != edges4_count){ cout<<"sssp::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }
		if((edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv4_sum) && false){ cout<<"sssp::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }							
	}
	cout<<"sssp::verify: verify successful."<<endl;
	#endif 
	#endif
	return;
}
void sssp::verifykvLOP(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], unsigned int CLOP, unsigned int * edges4_count, unsigned int * edgesdstv4_sum){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"sssp::verifykvLOP. verifying kvbuffer..."<<endl;
	#endif 
	keyy_t keys[COMPACTPARAM_ITEMSIZE_TOTALDATA];
	
	unsigned int rangeperpartition = 1 << (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * CLOP));
	unsigned int numberofpartitions = 1 << (NUM_PARTITIONS_POW * CLOP);
	unsigned int statsoffset = 0; for(unsigned int k=0; k<CLOP; k++){ statsoffset += (unsigned int)pow(NUM_PARTITIONS, k); } 
	unsigned int baseoffset_stats_kvs = BASEOFFSET_STATSDRAM_KVS + statsoffset;
	unsigned int baseoffset_kvdram = BASEOFFSET_KVDRAMWORKSPACE;
	if(CLOP % 2 == 0){ baseoffset_kvdram = BASEOFFSET_KVDRAM; }
	else { baseoffset_kvdram = BASEOFFSET_KVDRAMWORKSPACE; }
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"sssp::verifykvLOP:: numberofpartitions: "<<(1 << (NUM_PARTITIONS_POW * CLOP))<<", rangeperpartition: "<<rangeperpartition<<", baseoffset_stats_kvs: "<<baseoffset_stats_kvs<<", statsoffset: "<<statsoffset<<", baseoffset_kvdram: "<<baseoffset_kvdram<<endl;
	if(false){ utilityobj->printkeyvalues("sssp::verifykvLOP. stats", (keyvalue_t *)&stats[0][baseoffset_stats_kvs + 0], (1 + NUM_PARTITIONS)*VECTOR_SIZE, VECTOR_SIZE); }
	#endif
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"sssp::verifykvLOP:: verifying thread: "<<i<<" of "<<NUMSUBCPUTHREADS<<" threads..."<<endl;
		#endif 
		for(unsigned int p=0; p<numberofpartitions; p++){
			unsigned int numerrorkeys = 0;
			
			unsigned int upperlimit = 0;
			unsigned int lowerindex = upperlimit + (p * rangeperpartition);
			unsigned int upperindex = upperlimit + ((p+1) * rangeperpartition);
			
			unsigned int begin = stats[i][baseoffset_stats_kvs + p].data[0].key;
			unsigned int size = stats[i][baseoffset_stats_kvs + p].data[0].value;
			unsigned int end = begin + size;
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"sssp::verifykvLOP:: begin: "<<begin<<", size: "<<size<<", end: "<<end<<". ["<<lowerindex<<"->"<<upperindex<<"]"<<endl;
			#endif 
			
			for(unsigned int k=begin; k<begin + size; k++){
				keyvalue_t keyvalue = kvbuffer[i][baseoffset_kvdram + k];
				if(kvbuffer[i][baseoffset_kvdram + k].key != INVALIDDATA && kvbuffer[i][baseoffset_kvdram + k].value != INVALIDDATA){
					keyy_t thiskey = kvbuffer[i][baseoffset_kvdram + k].key;
					if(thiskey < lowerindex || thiskey >= upperindex){
						#ifdef ENABLE_PERFECTACCURACY
						if(numerrorkeys < 8){
							cout<<"sssp::verifykvLOP::ERROR KEYVALUE. i: "<<i<<", p: "<<p<<", index: "<<k-begin<<", thiskey: "<<thiskey<<", kvbuffer["<<i<<"]["<<baseoffset_kvdram + k<<"].value: "<<kvbuffer[i][baseoffset_kvdram + k].value<<", ["<<lowerindex<<"->"<<upperindex<<"]"<<endl; 					
							exit(EXIT_FAILURE);
						}
						cout<<"sssp::verifykvLOP::ERROR KEYVALUE thiskey: "<<thiskey<<", kvbuffer["<<i<<"]["<<baseoffset_kvdram + k<<"].value: "<<kvbuffer[i][baseoffset_kvdram + k].value<<endl; 
						exit(EXIT_FAILURE);
						#endif 
						
						numerrorkeys += 1;
					}
					
					*edges4_count += 1;
					*edgesdstv4_sum += thiskey;
				}
			}
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"sssp::verifykvLOP:: "<<numerrorkeys<<" errors seen for partition "<<p<<". ["<<lowerindex<<" -> "<<upperindex<<"]("<<begin<<" -> "<<end<<")("<<(end-begin)<<" values)"<<endl<<endl;
			#endif
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<""<<endl;
		#endif
	}
	return;
}
void sssp::verifyvertexdata(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"sssp::verifyvertexdata: verifying vertex data... "<<endl;
	#endif
	vector<value_t> activevertices;

	unsigned int baseoffset = BASEOFFSET_VERTICESDATA;
	unsigned int totalnumactvvs = 0;
	
	for(unsigned int i=0; i<1; i++){
		totalnumactvvs = 0;
		for(unsigned int vid=0; vid<KVDATA_RANGE/2; vid++){
			keyvalue_t keyvalue = kvbuffer[i][BASEOFFSET_VERTICESDATA + vid];
			if(vid % 1000000 == 0){ cout<<"sssp::verifyvertexdata: vid: "<<vid<<endl; }
		}
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"sssp::verifyvertexdata: number of active vertices for next iteration (subthread "<<i<<"): "<<totalnumactvvs<<endl;
		#endif
	}
	return;
}
void sssp::verifyactvvsdata(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"sssp::verifyactvvsdata: verifying actvvs data... "<<endl;
	#endif
	
	unsigned int actvvs_count = 0;
	unsigned int actvvsdstv1_sum = 0;
	unsigned int cctv = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		unsigned int sz = 16; // just-for-test
		keyy_t * KK = (keyy_t *)&kvbuffer[i][BASEOFFSET_ACTIVEVERTICES];
		unsigned int localactvvs_count = 0;
		unsigned int localactvvsdstv1_sum = 0;
		for(unsigned int k=0; k<sz; k++){
			if(KK[k] != INVALIDDATA){
				if(i==0){
					#ifdef _DEBUGMODE_HOSTPRINTS3
					if(cctv < 16){ cout<<"sssp:verifyactvvsdata: actvvid: "<<KK[k]<<endl; }
					#endif 
					actvvs_count += 1;
					actvvsdstv1_sum += KK[k];
				}
				localactvvs_count += 1;
				localactvvsdstv1_sum += KK[k]; 
				cctv += 1;
			}
		}
		cout<<"sssp::verifyactvvsdata: some actvvs found in kvbuffer["<<i<<"]: localactvvs_count: "<<localactvvs_count<<", localactvvsdstv1_sum: "<<localactvvsdstv1_sum<<endl;
	}
	return;
}
void sssp::verifykernelreturnvalues(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"sssp::verifykernelreturnvalues: results returned from kernel... "<<endl;
	#endif
	
	for(unsigned int i=0; i<16; i++){
		keyvalue_t keyvalue = kvbuffer[0][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_RETURNVALUES + i].data[0];	
		cout<<"sssp::verifykernelreturnvalues:: active vertices from GraphIter "<<i<<": "<<keyvalue.key<<endl;
	}
	return;
}


























