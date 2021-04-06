#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <mutex>
#include <thread>

// #include <chrono>
// #include <stdlib.h>
// #include <ctime>
// #include <map>
// #include <stdio.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <vector>
// #include <mutex>

#include "../../src/graphs/graph.h"
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "../../acts/actsutility/actsutility.h"
#include "acts_sw.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {vertices data}
// actvvs area {active vertices, active vertices mask}
// workspace area {stats, kvdram, kvdram workspace}

std::mutex vlock;

acts_sw::acts_sw(){ actsutilityobj = new actsutility(); }
acts_sw::~acts_sw(){}

void printstats(vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS]){
	for(unsigned int t = 0; t < TOTALNUMPARTITIONS; t++){
		if(t==0 && t<(1) && kvdram[t].size()>0){
			cout<<"topkernelproc:: LOP 1: partition: 1"<<": (t:"<<t<<") ["<<kvdram[t].size()<<"]"<<endl; 
		} else if(t>=(1) && t<(1+16) && kvdram[t].size()>0){
			cout<<"topkernelproc:: LOP 2: partition: "<<t-(1)<<": (t:"<<t<<") ["<<kvdram[t].size()<<"]"<<endl; 
		} else if(t>=(1+16) && t<(1+16+256) && kvdram[t].size()>0){
			cout<<"topkernelproc:: LOP 3: partition: "<<t-(1+16)<<": (t:"<<t<<") ["<<kvdram[t].size()<<"]"<<endl; 
		}
	}
	return;
}

inline unsigned int amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

unsigned int get_num_source_partitions(unsigned int currentLOP){
	if(currentLOP == 0){ currentLOP = 1; }
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

unsigned int getskipsize(unsigned int currentLOP, bool_type sourceORdest){
	if(currentLOP == 0){ currentLOP = 1; }
	
	unsigned int result = NUMLASTLEVELPARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){
		result = result / NUM_PARTITIONS;
	}
	return result;
}

sweepparams_t getsweepparams(unsigned int currentLOP, unsigned int source_partition){
	sweepparams_t sweepparams;
	unsigned int sourceskipsize = getskipsize(currentLOP, SOURCE);
	
	unsigned int div = BATCH_RANGE;
	for(unsigned int i=0; i<TREE_DEPTH; i++){
		div = div / NUM_PARTITIONS;
	}
	
	sweepparams.currentLOP = currentLOP;
	sweepparams.upperlimit = source_partition * sourceskipsize * div; // POW_BATCHRANGE
	sweepparams.source_partition = source_partition;
	return sweepparams;
}

inline unsigned int processfunc(unsigned int udata, unsigned int edgew, unsigned int GraphAlgo){
	unsigned int res = 0;
	#ifdef PR_ALGORITHM
	res = udata;
	#endif
	#ifdef BFS_ALGORITHM
	// res = NAp;  // BFS
	res = udata + edgew;
	#endif 
	#ifdef SSSP_ALGORITHM
	res = udata + edgew;
	#endif
	return res;
}

inline value_t reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef PR_ALGORITHM
	temp = vtemp + res;
	#endif 
	#ifdef BFS_ALGORITHM
	temp = min(vtemp, res);
	#endif 
	#ifdef SSSP_ALGORITHM
	temp = min(vtemp, res);
	#endif
	return temp;
}

unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit, unsigned int batch_range_pow){
	unsigned int partition;

	if(keyvalue.value == INVALIDDATA){ partition = keyvalue.key; } 
	else { partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))); }
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){
		cout<<"acts_sw::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", BATCH_RANGE_POW: "<<BATCH_RANGE_POW<<endl; 
		exit(EXIT_FAILURE); 
	}
	#endif
	return partition;
}

void acts_sw::processit(graph * graphobj, edge_type * edges, edge_t * vptrs, value_t * vprops, value_t * tempvprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int currentLOP, unsigned int GraphAlgo, unsigned int GraphIter, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker){				
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"processit called."<<endl;
	#endif
	unsigned int _sourcestatsmarker = *sourcestatsmarker;
	unsigned int _deststatsmarker = *deststatsmarker;

	sweepparams_t sweepparams = getsweepparams(currentLOP, 0);
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	unsigned int runsize = graphobj->get_num_vertices();
	#else 
	unsigned int runsize = actvvs.size();
	#endif 
	
	for(unsigned int i=0; i<runsize; i++){
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	if(i%500000==0){ cout<<i<<" vertices processed (of "<<runsize<<")."<<endl; }
	#endif 
		#ifdef ALLVERTEXISACTIVE_ALGORITHM
		unsigned int vid = i;
		#else 
		unsigned int vid = actvvs[i];
		#endif 
		edge_t vptr_begin = vptrs[vid];
		edge_t vptr_end = vptrs[vid+1];
		
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ cout<<"topkernelproc: ERROR SEEN."<<endl; exit(EXIT_FAILURE); continue; }
		
		for(unsigned int k=0; k<edges_size; k++){
			unsigned int dstvid = edges[vptr_begin + k].dstvid;
			
			#ifdef PR_ALGORITHM
			value_t res = vprops[vid];
			#endif
			#ifdef BFS_ALGORITHM
			value_t res = vprops[vid] + 1;
			#endif 
			#ifdef SSSP_ALGORITHM
			value_t res = vprops[vid] + 1;
			#endif
			
			keyvalue_t keyvalue;
			keyvalue.key = dstvid;
			keyvalue.value = res;
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"processit: udata: "<<res<<", srcvid: "<<vid<<", dstvid: "<<dstvid<<endl;
			#endif 
			
			#if TREE_DEPTH==1
			#ifdef VLOCK
			vlock.lock();
			#endif 
			
			value_t vprop = tempvprops[dstvid];
			#ifdef PR_ALGORITHM
			value_t vtemp = vprop + res;
			#endif 
			#ifdef BFS_ALGORITHM
			value_t vtemp = min(vprop, res);
			#endif 
			#ifdef SSSP_ALGORITHM
			value_t vtemp = min(vprop, res);
			#endif
			tempvprops[dstvid] = vtemp;
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"processit_reduceit: destvid: "<<keyvalue.key<<", vtemp: "<<vtemp<<endl;
			#endif 
			
			#ifdef VLOCK
			vlock.unlock();
			#endif 
			#endif 
			
			#if TREE_DEPTH>1
			unsigned int partition = getpartition(keyvalue, currentLOP, sweepparams.upperlimit, BATCH_RANGE_POW);
			kvdram[_deststatsmarker + partition].push_back(keyvalue);
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("acts_sw.processit. ERROR 7.", _deststatsmarker + partition, TOTALNUMPARTITIONS, NAp, NAp, NAp); 
			#endif
			#endif
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(1);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
			#endif
		}
	}
	
	*sourcestatsmarker = _sourcestatsmarker + 1;
	*deststatsmarker = _deststatsmarker + NUM_PARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

void acts_sw::partitionit_reduceit(graph * graphobj, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], value_t * vprops, value_t * tempvprops, vector<vertex_t> &actvvs_nextit, unsigned int currentLOP, unsigned int GraphAlgo, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker){
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"partitionit_reduceit called."<<endl;
	#endif
	unsigned int _sourcestatsmarker = *sourcestatsmarker;
	unsigned int _deststatsmarker = *deststatsmarker;

	for(currentLOP=2; currentLOP<=TREE_DEPTH; currentLOP++){ // currentLOP <= TREE_DEPTH-1
		unsigned int num_source_partitions = get_num_source_partitions(currentLOP);
		
		for(unsigned int source_partition=0; source_partition<num_source_partitions; source_partition+=1){
			sweepparams_t sweepparams = getsweepparams(currentLOP, source_partition);
	
			for(unsigned int t=0; t<kvdram[_sourcestatsmarker].size(); t++){
				keyvalue_t keyvalue = kvdram[_sourcestatsmarker][t];
				
				if(currentLOP == TREE_DEPTH){
					unsigned int dstvid = keyvalue.key;
					value_t res = keyvalue.value;
					
					#ifdef VLOCK
					vlock.lock();
					#endif 
			
					value_t vprop = tempvprops[dstvid];
					#ifdef PR_ALGORITHM
					value_t vtemp = vprop + res;
					#endif 
					#ifdef BFS_ALGORITHM
					value_t vtemp = min(vprop, res);
					#endif 
					#ifdef SSSP_ALGORITHM
					value_t vtemp = min(vprop, res);
					#endif
					tempvprops[dstvid] = vtemp;
					
					#ifdef VLOCK
					vlock.unlock();
					#endif
			
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					cout<<"processit_reduceit: destvid: "<<keyvalue.key<<", vtemp: "<<vtemp<<endl;
					#endif
				} else {
					unsigned int partition = getpartition(keyvalue, currentLOP, sweepparams.upperlimit, BATCH_RANGE_POW);
					kvdram[_deststatsmarker + partition].push_back(keyvalue);
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("acts_sw.partitionit_reduceit. ERROR 7.", _deststatsmarker + partition, TOTALNUMPARTITIONS, NAp, NAp, NAp); 
					#endif 
				}
			}
			
			_sourcestatsmarker += 1;
			_deststatsmarker += NUM_PARTITIONS;
		}
	}
	
	*sourcestatsmarker = _sourcestatsmarker;
	*deststatsmarker = _deststatsmarker;
	return;
}

void acts_sw::topkernelproc(graph * graphobj, edge_type * edges, edge_t * vptrs, value_t * vprops, value_t * tempvprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int GraphIter){
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 1;
	
	for(unsigned int t=0; t<TOTALNUMPARTITIONS; t++){ kvdram[t].clear(); } // reset
	
	for(unsigned int currentLOP=1; currentLOP<1+TREE_DEPTH; currentLOP++){
		if(currentLOP == 1){ processit(graphobj, edges, vptrs, vprops, tempvprops, actvvs, actvvs_nextit, kvdram, currentLOP, GraphAlgo, GraphIter, &sourcestatsmarker, &deststatsmarker); }
		else { partitionit_reduceit(graphobj, kvdram, vprops, tempvprops, actvvs_nextit, currentLOP, GraphAlgo, &sourcestatsmarker, &deststatsmarker); }
	
		#ifdef _DEBUGMODE_HOSTPRINTS
		printstats(kvdram);
		#endif
	}
	return;
}

#ifdef SINGLEVBUFFER
void acts_sw::start(graph * graphobj, edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], value_t * vprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters){				
	value_t * tempvprops = new value_t[KVDATA_RANGE];
	memcpy(tempvprops, vprops, KVDATA_RANGE * sizeof(value_t));
	
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> acts_sw::start (single vbuffer): Iteration: "<<GraphIter<<" ("<<actvvs.size()<<" active vertices)"<<endl;
		
		#ifdef MULTITHREADEDIMPL
		for(int i = 0; i < NUMSUBCPUTHREADS; i++){ threadobj[i] = std::thread(&acts_sw::topkernelproc, this, graphobj, edges[i], vptrs[i], vprops, tempvprops, std::ref(actvvs), std::ref(actvvs_nextit), std::ref(kvdram[i]), GraphAlgo, GraphIter); }
		for(int i = 0; i < NUMSUBCPUTHREADS; i++){ threadobj[i].join(); }
		#else 
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			#ifdef ALLVERTEXISACTIVE_ALGORITHM
			cout<<">>> acts_sw::start: instance :"<<i<<endl;
			#endif 
			topkernelproc(edges[i], vptrs[i], vprops, tempvprops, actvvs, actvvs_nextit, kvdram[i], GraphAlgo, GraphIter);
		}
		#endif 
		
		topkernelsync(graphobj, vprops, tempvprops, actvvs_nextit);
		
		actvvs.clear();
		actvvs.assign(actvvs_nextit.begin(), actvvs_nextit.end());
		actvvs_nextit.clear();
		
		if(actvvs.size() == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
	}
	return;
}
void acts_sw::topkernelsync(graph * graphobj, value_t * vprops, value_t * tempvprops, vector<vertex_t> &actvvs_nextit){
	for(unsigned int i=0; i<KVDATA_RANGE; i++){
		value_t vprop = vprops[i];
		value_t vtemp = tempvprops[i];
		
		vprops[i] = vtemp;
		
		if(vtemp != vprop){ actvvs_nextit.push_back(i); } 
	}
	return;
}
#endif 

#ifndef SINGLEVBUFFER
void acts_sw::start(graph * graphobj, edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], value_t * vprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters){				
	value_t * tempvprops[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ tempvprops[i] = new value_t[KVDATA_RANGE]; }
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ memcpy(tempvprops[i], vprops, KVDATA_RANGE * sizeof(value_t)); }
	
	std::chrono::steady_clock::time_point begintime_start = std::chrono::steady_clock::now();
	
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> acts_sw::start (multiple vbuffers): Iteration: "<<GraphIter<<" ("<<actvvs.size()<<" active vertices)"<<endl;
		
		std::chrono::steady_clock::time_point begintime_proc = std::chrono::steady_clock::now();
		#ifdef MULTITHREADEDIMPL
		for(int i = 0; i < NUMSUBCPUTHREADS; i++){ threadobj[i] = std::thread(&acts_sw::topkernelproc, this, graphobj, edges[i], vptrs[i], vprops, tempvprops[i], std::ref(actvvs), std::ref(actvvs_nextit), std::ref(kvdram[i]), GraphAlgo, GraphIter); }
		for(int i = 0; i < NUMSUBCPUTHREADS; i++){ threadobj[i].join(); }
		#else 
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			#ifdef ALLVERTEXISACTIVE_ALGORITHM
			cout<<">>> acts_sw::start: instance :"<<i<<endl;
			#endif 
			topkernelproc(edges[i], vptrs[i], vprops, tempvprops[i], actvvs, actvvs_nextit, kvdram[i], GraphAlgo, GraphIter);
		}
		#endif 
		long double total_time_elapsed_proc = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_proc).count();
		cout<<">>> acts_sw::start: total_time_elapsed (proc): "<<total_time_elapsed_proc<<" ms ("<<total_time_elapsed_proc/1000<<" s)"<<endl;
		
		std::chrono::steady_clock::time_point begintime_sync = std::chrono::steady_clock::now();
		topkernelsync(graphobj, vprops, tempvprops, actvvs_nextit);
		long double total_time_elapsed_sync = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_sync).count();
		cout<<">>> acts_sw::start: total_time_elapsed (sync): "<<total_time_elapsed_sync<<" ms ("<<total_time_elapsed_sync/1000<<" s)"<<endl;
		
		// actvvs.clear();
		// actvvs.assign(actvvs_nextit.begin(), actvvs_nextit.end());
		// actvvs_nextit.clear();
		
		if(actvvs.size() == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
	}
	
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_start).count();
	unsigned int total_edges_processed = 212698418;
	cout<<">>> acts_sw::start: total_edges_processed: "<<total_edges_processed<<" edges ("<<total_edges_processed/1000000<<" million edges)"<<endl;
	cout<<">>> acts_sw::start: total_time_elapsed: "<<total_time_elapsed<<" ms ("<<total_time_elapsed/1000<<" s)"<<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> acts_sw::start: throughput: "<<((total_edges_processed / total_time_elapsed) * (1000))<<" edges/sec ("<<((total_edges_processed / total_time_elapsed) / (1000))<<" million edges/sec)"<< RESET <<endl;
	
	return;
}
void acts_sw::topkernelsync(graph * graphobj, value_t * vprops, value_t * tempvprops[NUMSUBCPUTHREADS], vector<vertex_t> &actvvs_nextit){
	unsigned int _DIVSZ = graphobj->get_num_vertices() / NUMTHREADS_SYNC;
	#ifdef MULTITHREADEDIMPL
	for(int i = 0; i < NUMTHREADS_SYNC; i++){ threadobj[i] = std::thread(&acts_sw::topkernelsync_thread, this, i*_DIVSZ, _DIVSZ, graphobj, vprops, tempvprops, std::ref(actvvs_nextit)); }
	for(int i = 0; i < NUMTHREADS_SYNC; i++){ threadobj[i].join(); }
	#else 
	for(unsigned int i=0; i<NUMTHREADS_SYNC; i++){
		#ifdef ALLVERTEXISACTIVE_ALGORITHM
		cout<<">>> acts_sw::topkernelsync: instance :"<<i<<endl;
		#endif 
		topkernelsync_thread(i*_DIVSZ, _DIVSZ, graphobj, vprops, tempvprops, actvvs_nextit);
	}
	#endif 
}
void acts_sw::topkernelsync_thread(unsigned int offset, unsigned int size, graph * graphobj, value_t * vprops, value_t * tempvprops[NUMSUBCPUTHREADS], vector<vertex_t> &actvvs_nextit){
	cout<<"topkernelsync_thread: offset: "<<offset<<", size: "<<size<<endl;
	for(unsigned int t=offset; t<offset+size; t++){
		value_t vprop = vprops[t];
		
		value_t accumvtemp = 0;
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ // NUMSUBCPUTHREADS
			value_t vtemp = tempvprops[j][t];

			#ifdef PR_ALGORITHM
			accumvtemp = accumvtemp + vtemp;
			#endif 
			#ifdef BFS_ALGORITHM
			accumvtemp = min(accumvtemp, vtemp);
			#endif 
			#ifdef SSSP_ALGORITHM
			accumvtemp = min(accumvtemp, vtemp);
			#endif
		}
		
		vprops[t] = accumvtemp;
		
		/* if(accumvtemp != vprop){ 
			vlock.lock(); // FIXME.
			actvvs_nextit.push_back(t); 
			vlock.unlock();
		} */ 
	}
	return;
}
/* void acts_sw::topkernelsync_thread(unsigned int offset, unsigned int size, graph * graphobj, value_t * vprops, value_t * tempvprops[NUMSUBCPUTHREADS], vector<vertex_t> &actvvs_nextit){
	cout<<"topkernelsync_thread: offset: "<<offset<<", size: "<<size<<endl;
	for(unsigned int t=0; t<KVDATA_RANGE; t++){
		value_t vprop = vprops[t];
		
		value_t accumvtemp = 0;
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){
			value_t vtemp = tempvprops[j][t];

			#ifdef PR_ALGORITHM
			accumvtemp = accumvtemp + vtemp;
			#endif 
			#ifdef BFS_ALGORITHM
			accumvtemp = amin(accumvtemp, vtemp);
			#endif 
			#ifdef SSSP_ALGORITHM
			accumvtemp = amin(accumvtemp, vtemp);
			#endif
		}
		vprops[t] = accumvtemp;
		
		if(accumvtemp != vprop){ 
			vlock.lock(); // FIXME.
			actvvs_nextit.push_back(t); 
			vlock.unlock();
		} 
	}
	return;
} */
#endif 






