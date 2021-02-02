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
#include "../../examples/helperfunctions/compactgraph.h"
#include "../../examples/helperfunctions/setupkernel.h"
#include "../../examples/helperfunctions/postprocess.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "bfs.h"
using namespace std;
#define PROCESSACTIVEVERTICESTEST

bfs::bfs(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumedgebanks(), true, true, true);
	statsobj = new stats(graphobj);
	algorithmobj = new algorithm();
	parametersobj = new parameters(); 
	utilityobj = new utility(); 
	postprocessobj = new postprocess(graphobj, statsobj); 
	loadgraphobj = new loadgraph(graphobj, statsobj); 
	compactgraphobj = new compactgraph(graphobj, statsobj);
	setupkernelobj = new setupkernel(graphobj, statsobj); 

	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }				
	vdram = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDVDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	vdram = new uint512_vec_dt[PADDEDVDRAMSZ_KVS];
	#endif

	if(graphobj->getdataset().graphdirectiontype == UNDIRECTEDGRAPH){
		edgedatabuffer = new edge2_type[2 * graphobj->get_num_edges()];
		packededgedatabuffer = new uuint64_dt[2 * graphobj->get_num_edges()];
	} else {
		edgedatabuffer = new edge2_type[graphobj->get_num_edges()];
		packededgedatabuffer = new uuint64_dt[graphobj->get_num_edges()];
	}
	packedvertexptrbuffer = new edge_t[KVDATA_RANGE];
	
	#ifdef FPGA_IMPL
	setupkernelobj->loadOCLstructures(binaryFile, vdram, kvbuffer);
	#endif
}
bfs::~bfs(){
	cout<<"bfs::~bfs:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void bfs::finish(){
	#ifdef FPGA_IMPL
	setupkernelobj->finishOCL();
	#endif
}

runsummary_t bfs::run(){
	long double totaltime_ms = 0;
	#ifdef BFS_ALGORITHM
	cout<<"bfs::run:: bfs algorithm started. "<<endl;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata();
	tempvertexdatabuffer = graphobj->generatetempverticesdata();
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	// set root vid
	unsigned int NumGraphIters = 1; // 12
	unsigned int evalid = 0; // 2;
	container_t container;
	vector<value_t> activevertices;

	// activevertices.push_back(1);
	// activevertices.push_back(2);
	// for(unsigned int i=0; i<2; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<100; i++){ activevertices.push_back(i); } //
	// for(unsigned int i=0; i<500; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<2048; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<4096; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<10000; i++){ activevertices.push_back(i); }
	for(unsigned int i=0; i<100000; i++){ activevertices.push_back(i); } //
	// for(unsigned int i=0; i<1000000; i++){ activevertices.push_back(i); } 
	// for(unsigned int i=0; i<2000000; i++){ activevertices.push_back(i); }
	// for(unsigned int i=0; i<4000000; i++){ activevertices.push_back(i); }
	
	// load workload
	loadgraphobj->loadvertexdata(tempvertexdatabuffer, (keyvalue_t *)vdram, 0, KVDATA_RANGE); 
	compactgraphobj->compact(vertexptrbuffer, edgedatabuffer, packedvertexptrbuffer, packededgedatabuffer); 
	loadgraphobj->loadedges_rowwise(0, packedvertexptrbuffer, packededgedatabuffer, (vptr_type **)kvbuffer, (uuint64_dt **)kvbuffer, &container, BREADTHFIRSTSEARCH); 			
	loadgraphobj->loadoffsetmarkers((uuint64_dt **)kvbuffer, (keyvalue_t **)kvbuffer, &container);
	loadgraphobj->loadactvvertices(activevertices, (keyy_t *)vdram, &container); 
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
	
	// experiements
	experiements(0, NumGraphIters, 1, NumGraphIters, &container, activevertices); // full run
	// experiements(0, 0, NumGraphIters, NumGraphIters, &container, activevertices); // N full runs
	// experiements(2, 0, NumGraphIters, NumGraphIters, &container, activevertices); // N full runs, isolating partition & reduce phases
	
	finish();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	#endif 
	return statsobj->timingandsummary(NAp, totaltime_ms);
}
void bfs::experiements(unsigned int evalid, unsigned int start, unsigned int size, unsigned int NumGraphIters, container_t * container, vector<value_t> & activevertices){
	unsigned int swnum_its = utilityobj->runbfs_sw(activevertices, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	for(unsigned int num_its=start; num_its<start+size; num_its++){
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs::run: bfs evaluation "<<num_its<<" started. (NumGraphIters: "<<NumGraphIters<<", num active vertices: "<<activevertices.size()<<")"<< RESET <<endl;

		cout<<"bfs::experiements: resetting kvdram & kvdram workspaces..."<<endl;
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][BASEOFFSET_KVDRAM_KVS], KVDRAMSZ);
			utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][BASEOFFSET_KVDRAMWORKSPACE_KVS], KVDRAMWORKSPACESZ);
		}

		loadgraphobj->loadvertexdata(tempvertexdatabuffer, (keyvalue_t *)vdram, 0, KVDATA_RANGE); 
		loadgraphobj->loadactvvertices(activevertices, (keyy_t *)vdram, container); 
		
		loadgraphobj->loadmessages(vdram, kvbuffer, container, num_its, BREADTHFIRSTSEARCH);
		loadgraphobj->setcustomeval(vdram, (uint512_vec_dt **)kvbuffer, evalid);
		
		std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
		cout<<endl<< TIMINGRESULTSCOLOR <<">>> bfs::run: bfs started. ("<<activevertices.size()<<" active vertices)"<< RESET <<endl;
		setupkernelobj->launchkernel((uint512_vec_dt *)vdram, (uint512_vec_dt **)kvbuffer, 0);
		utilityobj->stopTIME(">>> bfs::finished:. Time Elapsed: ", begintime, NAp);
		long double totaltime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
		
		if(evalid == 0){ verify(activevertices, num_its); }
		utilityobj->runbfs_sw(activevertices, vertexptrbuffer, edgedatabuffer, NumGraphIters);
	
		statsobj->timingandsummary(num_its, totaltime_ms);
		if(num_its > swnum_its){ break; } 
	}
	
	verifyvertexdata((keyvalue_t *)vdram);
	verifyactvvsdata((keyvalue_t *)vdram);
	verifykernelreturnvalues(vdram);
	return;
}

void bfs::verify(vector<vertex_t> &activevertices, unsigned int NumGraphIters){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"bfs::verify. verifying..."<<endl;
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
					unsigned int numitems = utilityobj->PARSE(keyvalue, keys);
					edges3_count += numitems; 
					for(unsigned int t=0; t<numitems; t++){ edgesdstv3_sum += keys[t]; }
				}
			}
		}
	}
	if(CLOP != 1){ edges3_count = NAp; edgesdstv3_sum = NAp; }
	
	// 4th check (checking edges in acts.LLOP...)
	verifykvLOP((keyvalue_t **)kvbuffer, kvbuffer, CLOP, &edges4_count, &edgesdstv4_sum);
	
	// 5th check
	edges5_count = vdram[PADDEDVDRAMSZ_KVS-1].data[3].key; 
	edgesdstv5_sum = vdram[PADDEDVDRAMSZ_KVS-1].data[4].key; 

	// 6th check 
	unsigned int actvvsdstv1_sum = 0;
	unsigned int cctv = 0;
	unsigned int sz = vdram[PADDEDVDRAMSZ_KVS-1].data[5].key;
	actvvs_verbosecount = sz;
	keyy_t * KK = (keyy_t *)&vdram[BASEOFFSET_ACTIVEVERTICES_KVS];
	unsigned int localactvvs_count = 0;
	unsigned int localactvvsdstv1_sum = 0;
	for(unsigned int k=0; k<sz; k++){
		// if(KK[k] != INVALIDDATA){
		if(KK[k] != INVALIDDATA && KK[k] != UNUSEDDATA){ // CRITICAL REMOVEME. NEWCHANGE
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(cctv < 16){ 
			cout<<"bfs:verify: actvvid: "<<KK[k]<<endl; 
			}
			#endif 
			actvvs_count += 1;
			actvvsdstv1_sum += KK[k];
				
			localactvvs_count += 1;
			localactvvsdstv1_sum += KK[k]; 
			cctv += 1;
		}
	}
	cout<<"bfs::verifyactvvs: num actvvs found in vdram: localactvvs_count: "<<localactvvs_count<<", localactvvsdstv1_sum: "<<localactvvsdstv1_sum<<endl;
	
	// 7th view
	cout<<endl<<"[7th view]: active vertices in vdram: "<<endl;
	keyy_t * TT = (keyy_t *)&vdram[BASEOFFSET_ACTIVEVERTICES_KVS];
	for(unsigned int k=0; k<4; k++){
		if(TT[k] != INVALIDDATA){
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"bfs:verify: actvvid: "<<TT[k]<<endl;
			#endif
		}
	}
	
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (offchip, edges in file               ) edges1_count: "<<edges1_count<<", edgesdstv1_sum: "<<edgesdstv1_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (onchip, during acts.procactvvs stage ) edges2_count: "<<edges2_count<<", edgesdstv2_sum: "<<edgesdstv2_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (inkvdram, after acts.procactvvs stage) edges3_count: "<<edges3_count<<", edgesdstv3_sum: "<<edgesdstv3_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (inkvdram, after CLOP="<<CLOP<<" stage         ) edges4_count: "<<edges4_count<<", edgesdstv4_sum: "<<edgesdstv4_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (inkvdram, after acts.reduce stage    ) edges5_count: "<<edges5_count<<", edgesdstv5_sum: "<<edgesdstv5_sum<<endl;
	cout<<"+++++++++++++++++++++++++++++ bfs:verify (onchip, active vertices for next it  ) actvvs_count: "<<actvvs_count<<" (actvvs_verbosecount:"<<actvvs_verbosecount<<", actvvsdstv1_sum:"<<actvvsdstv1_sum<<")"<<endl;
	
	if(kvbuffer[0][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key > 1){ edges1_count = edges2_count; }
	
	#if defined(_DEBUGMODE_HOSTCHECKS2) && not defined(HW)
	if(NumGraphIters > 0){
		if(CLOP == 1){
			if(edges1_count != edges2_count || edges1_count != edges3_count || edges1_count != edges4_count){ cout<<"bfs::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }
			if((edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv3_sum || edgesdstv1_sum != edgesdstv4_sum) && false){ cout<<"bfs::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }							
		} else if(CLOP == TREE_DEPTH){
			if(edges1_count != edges2_count || edges1_count != edges4_count || edges1_count != edges5_count){ cout<<"bfs::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }
			if((edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv4_sum || edgesdstv1_sum != edgesdstv5_sum) && false){ cout<<"bfs::verify: INEQUALITY ERROR: edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv3_sum || edgesdstv1_sum != edgesdstv4_sum. ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }							
		} else {
			if(edges1_count != edges2_count || edges1_count != edges4_count){ cout<<"bfs::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }
			if((edgesdstv1_sum != edgesdstv2_sum || edgesdstv1_sum != edgesdstv4_sum) && false){ cout<<"bfs::verify: INEQUALITY ERROR: ARE ALL ACTS INSTANCES RUNNING? exiting..."<<endl; exit(EXIT_FAILURE); }							
		}
	}
	cout<<"bfs::verify: verify successful."<<endl;
	#endif 
	#endif
	return;
}
void bfs::verifykvLOP(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], unsigned int CLOP, unsigned int * edges4_count, unsigned int * edgesdstv4_sum){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"bfs::verify. verifying kvbuffer..."<<endl;
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
	cout<<"bfs::verifykvLOP:: numberofpartitions: "<<(1 << (NUM_PARTITIONS_POW * CLOP))<<", rangeperpartition: "<<rangeperpartition<<", baseoffset_stats_kvs: "<<baseoffset_stats_kvs<<", statsoffset: "<<statsoffset<<", baseoffset_kvdram: "<<baseoffset_kvdram<<endl;
	if(false){ utilityobj->printkeyvalues("bfs::verifykvLOP. stats", (keyvalue_t *)&stats[0][baseoffset_stats_kvs + 0], (1 + NUM_PARTITIONS)*VECTOR_SIZE, VECTOR_SIZE); }
	#endif
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"bfs::verifykvLOP:: verifying thread: "<<i<<" of "<<NUMSUBCPUTHREADS<<" threads..."<<endl;
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
			cout<<"bfs::verifykvLOP:: begin: "<<begin<<", size: "<<size<<", end: "<<end<<". ["<<lowerindex<<"->"<<upperindex<<"]"<<endl;
			#endif 
			
			for(unsigned int k=begin; k<begin + size; k++){
				keyvalue_t keyvalue = kvbuffer[i][baseoffset_kvdram + k];
				if(kvbuffer[i][baseoffset_kvdram + k].key != INVALIDDATA && kvbuffer[i][baseoffset_kvdram + k].value != INVALIDDATA){
					keyy_t thiskey = utilityobj->GETKEY(kvbuffer[i][baseoffset_kvdram + k]);
					if(thiskey < lowerindex || thiskey >= upperindex){
						#ifdef ENABLE_PERFECTACCURACY
						if(numerrorkeys < 8){
							cout<<"bfs::verifykvLOP::ERROR KEYVALUE. i: "<<i<<", p: "<<p<<", index: "<<k-begin<<", thiskey: "<<thiskey<<", kvbuffer["<<i<<"]["<<baseoffset_kvdram + k<<"].value: "<<kvbuffer[i][baseoffset_kvdram + k].value<<", ["<<lowerindex<<"->"<<upperindex<<"]"<<endl; 					
							utilityobj->PARSE(keyvalue, keys);
							exit(EXIT_FAILURE);
						}
						cout<<"bfs::verifykvLOP::ERROR KEYVALUE thiskey: "<<thiskey<<", kvbuffer["<<i<<"]["<<baseoffset_kvdram + k<<"].value: "<<kvbuffer[i][baseoffset_kvdram + k].value<<endl; 
						exit(EXIT_FAILURE);
						#endif 
						
						numerrorkeys += 1;
					}
					
					unsigned int numitems = utilityobj->PARSE(kvbuffer[i][baseoffset_kvdram + k], keys);
					*edges4_count += numitems; 
					for(unsigned int t=0; t<numitems; t++){ *edgesdstv4_sum += keys[t]; }
				}
			}
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"bfs::verifykvLOP:: "<<numerrorkeys<<" errors seen for partition "<<p<<". ["<<lowerindex<<" -> "<<upperindex<<"]("<<begin<<" -> "<<end<<")("<<(end-begin)<<" values)"<<endl<<endl;
			#endif
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<""<<endl;
		#endif
	}
	return;
}
void bfs::verifyvertexdata(keyvalue_t * vdram){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"bfs::verifyvertexdata: verifying vertex data... "<<endl;
	#endif
	vector<value_t> activevertices;

	unsigned int baseoffset = BASEOFFSET_VERTICESDATA;
	unsigned int totalnumactvvs = 0;
	
	for(unsigned int i=0; i<1; i++){
		totalnumactvvs = 0;
		for(unsigned int k=0; k<KVDATA_RANGE/32; k++){
			keyvalue_t keyvalue = vdram[BASEOFFSET_VERTICESDATA + k];
			
			for(unsigned int t = 0; t < 64; t+=2){
				unsigned int vid = k*32 + t/2;
				unsigned int index = t;
				unsigned int bitsize = 2;
				
				if(vid % 1000000 == 0){ cout<<"bfs::verifyvertexdata: vid: "<<vid<<endl; }
				
				unsigned int vdataj = utilityobj->READFROM_ULONG(keyvalue, index, bitsize);
				if(vdataj == VISITED_IN_PAST_ITERATION){
					totalnumactvvs += 1; 
					#ifdef _DEBUGMODE_HOSTPRINTS3
					if(totalnumactvvs < 16){ cout<<"bfs::verifyvertexdata: VISITED_IN_CURRENT_ITERATION seen @ k:"<<k<<", t:"<<t<<", vid:"<<vid<<endl; }
					#endif
				}
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"bfs::verifyvertexdata: number of active vertices for next iteration (subthread "<<i<<"): "<<totalnumactvvs<<endl;
		#endif
	}
	return;
}
void bfs::verifyactvvsdata(keyvalue_t * vdram){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"bfs::verifyactvvsdata: verifying actvvs data... "<<endl;
	#endif
	
	unsigned int actvvs_count = 0;
	unsigned int actvvsdstv1_sum = 0;
	unsigned int cctv = 0;
	unsigned int sz = 8; // just-for-test
	keyy_t * KK = (keyy_t *)&vdram[BASEOFFSET_ACTIVEVERTICES];
	unsigned int localactvvs_count = 0;
	unsigned int localactvvsdstv1_sum = 0;
	
	for(unsigned int k=0; k<sz; k++){
		if(KK[k] != INVALIDDATA){
			#ifdef _DEBUGMODE_HOSTPRINTS3
			if(cctv < 8){ cout<<"bfs:verifyactvvsdata: actvvid: "<<KK[k]<<endl; }
			#endif 
			actvvs_count += 1;
			actvvsdstv1_sum += KK[k];

			localactvvs_count += 1;
			localactvvsdstv1_sum += KK[k]; 
			cctv += 1;
		}
	}
	
	cout<<"bfs::verifyactvvsdata: some actvvs found in vdram: localactvvs_count: "<<localactvvs_count<<", localactvvsdstv1_sum: "<<localactvvsdstv1_sum<<endl;
	return;
}
void bfs::verifykernelreturnvalues(uint512_vec_dt * vdram){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"bfs::verifykernelreturnvalues: results returned from kernel... "<<endl;
	#endif
	
	for(unsigned int i=0; i<16; i++){
		keyvalue_t keyvalue = vdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_RETURNVALUES + i].data[0];	
		cout<<"bfs::verifykernelreturnvalues:: active vertices from GraphIter "<<i<<": "<<keyvalue.key<<endl;
	}
	return;
}
void bfs::apply(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], vector<value_t> &activevertices){
	activevertices.clear();
	unsigned int baseoffset = BASEOFFSET_VERTICESDATA;
	unsigned long vdata = UNVISITED;
	unsigned int totalnumactvvs_unmerged = 0;
	unsigned int edges1_count = 0;
	unsigned int edgesdstv1_sum = 0;
	
	for(unsigned int k=0; k<KVDATA_RANGE/32; k++){
		for(unsigned int t = 0; t < 64; t+=2){
			vdata = UNVISITED;
			unsigned int vid = k*32 + t/2;
			unsigned int index = 64 - t - 2;
			unsigned int bitsize = 2;
			
			if(vid % 1000000 == 0){ cout<<"bfs::apply: vid: "<<vid<<endl; }
			
			// unmerged
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				unsigned int vdataj = utilityobj->READFROM_ULONG(kvbuffer[j][baseoffset + k], index, bitsize);
				if(vdataj == VISITED_IN_CURRENT_ITERATION){ vdata = VISITED_IN_CURRENT_ITERATION; totalnumactvvs_unmerged += 1; }
			}
			
			// merged
			keyvalue_t mergedkv; mergedkv.key = 0; mergedkv.value = 0;
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				mergedkv.key = mergedkv.key | kvbuffer[j][baseoffset + k].key;
				mergedkv.value = mergedkv.value | kvbuffer[j][baseoffset + k].value;
			}
			unsigned int vdataj = utilityobj->READFROM_ULONG(mergedkv, index, bitsize);
			if(vdataj == VISITED_IN_CURRENT_ITERATION){ vdata = VISITED_IN_CURRENT_ITERATION; activevertices.push_back(vid); edgesdstv1_sum += vid; }
			
			// synchronize
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				utilityobj->WRITETO_ULONG(&kvbuffer[j][baseoffset + k], index, bitsize, vdata);
			}
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(vdata == VISITED_IN_CURRENT_ITERATION){ cout<<"bfs::apply: VISITED_IN_CURRENT_ITERATION seen @ (k:"<<k<<", t:"<<t<<", vid:"<<vid<<"): vdata: "<<vdata<<endl; }
			#endif
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"+++++++++++++++++++++++++++++ bfs:apply (nextit acvvs) edges1_count: NAp, edgesdstv1_sum: "<<edgesdstv1_sum<<endl;
	cout<<"bfs::apply: number of active vertices for next iteration (unmerged): "<<totalnumactvvs_unmerged<<endl;
	cout<<"bfs::apply: number of active vertices for next iteration: "<<activevertices.size()<<endl;
	// for(unsigned int i=0; i<activevertices.size(); i++){ cout<<"bfs::apply: activevertices["<<i<<"]: "<<activevertices[i]<<endl; }
	for(unsigned int i=0; i<utilityobj->hmin(activevertices.size(), 16); i++){ cout<<"bfs::apply: activevertices["<<i<<"]: "<<activevertices[i]<<endl; }
	#endif
	return;
}


















