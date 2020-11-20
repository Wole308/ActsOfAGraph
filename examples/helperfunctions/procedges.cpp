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
#include "../../src/stats/stats.h"
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../../acts/include/actscommon.h"
#include "../../src/utility/utility.h"
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
#include "procedges.h"
using namespace std;

procedges::procedges(stats * _statsobj){ 
	actsutilityobj = new actsutility(); 
	utilityobj = new utility(); 
	statsobj = _statsobj;
}
procedges::~procedges(){}

unsigned int procedges::processedgefunc(value_t Uprop, unsigned int edgeweight, unsigned int voutdegree, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t res = 0;
	if(GraphAlgo == PAGERANK){
		res = Uprop / voutdegree;
	} else if (GraphAlgo == BREADTHFIRSTSEARCH){
		res = NAp;
	} else if (GraphAlgo == SSSP){
		res = Uprop + edgeweight;
	} else { res = 0; }
	return res;
}
void procedges::processedges(edge_t * vertexptrs, value_t * verticesdata, keyvalue_t * edges, keyvalue_t * keyvalues, unsigned int srcvoffset, unsigned int srcvsize, unsigned int edgessize, unsigned int GraphIter, unsigned int GraphAlgo){							
	unsigned int firstvptr = vertexptrs[srcvoffset];
	unsigned int k = 0;
	
	for(unsigned int i=0; i<srcvsize; i++){
		unsigned int srcvid = srcvoffset + i;
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(i % 10000 == 0){ cout<<"procedges::processedges:: i: "<<i<<", srcvid: "<<srcvid<<", edges[0].srcvid: "<<edges[0].key<<", edges[0].dstvid: "<<edges[0].value<<endl; }
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("procedges::processedges.srcvid", srcvid, KVDATA_RANGE, NAp, NAp, NAp);
		#endif 
		unsigned int sourcedata = verticesdata[srcvid];
		vertex_t myfirstsrcvid = srcvoffset + i;
		vertex_t mylastsrcvid = myfirstsrcvid + 1;
		
		// cout<<"procedges::processedges:: myfirstsrcvid: "<<myfirstsrcvid<<endl;
		// cout<<"procedges::processedges:: mylastsrcvid: "<<mylastsrcvid<<endl;
		// cout<<"procedges::processedges:: mylastsrcvid: "<<mylastsrcvid<<endl;
		// cout<<"procedges::processedges:: vertexptrs["<<srcvoffset + i<<"]: "<<vertexptrs[srcvoffset + i]<<endl;
		// cout<<"procedges::processedges:: vertexptrs["<<srcvoffset + i + 1<<"]: "<<vertexptrs[srcvoffset + i + 1]<<endl;
		// cout<<"procedges::processedges:: firstvptr: "<<firstvptr<<endl;
		// cout<<"procedges::processedges:: srcvoffset: "<<srcvoffset<<endl;
		// cout<<"procedges::processedges:: i: "<<i<<endl;
		// cout<<"procedges::processedges:: srcvid: "<<srcvid<<endl;
		// cout<<"procedges::processedges:: sourcedata: "<<sourcedata<<endl;
		// cout<<"procedges::processedges:: srcvsize: "<<srcvsize<<endl;
		
		keyy_t beginvptr = vertexptrs[srcvoffset + i] - firstvptr;
		keyy_t endvptr = vertexptrs[srcvoffset + i + 1] - firstvptr;
		edge_t numedges = endvptr - beginvptr;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(endvptr < beginvptr){ cout<<"procedges::processedges::ERROR: endvptr < beginvptr. beginvptr: "<<beginvptr<<", endvptr: "<<endvptr<<", firstvptr: "<<firstvptr<<endl; exit(EXIT_FAILURE); }
		if(endvptr < edgessize){ actsutilityobj->checkptr(myfirstsrcvid, mylastsrcvid, beginvptr, endvptr, edges); }
		#endif
		
		for(unsigned int j=beginvptr; j<(beginvptr + numedges); j++){
			keyvalue_t edge = edges[j];
			
			keyvalue_t vertexupdate;
			vertexupdate.key = edge.value;
			vertexupdate.value = processedgefunc(sourcedata, 1, 1, GraphIter, GraphAlgo);
			
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("procedges::processedges", k, PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
			#endif 
			keyvalues[k] = vertexupdate; 
			k += 1;
			
			#ifdef _DEBUGMODE_STATSX
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); // speedup when commented out
			#endif
		}
	}
	return;
}

void procedges::start(uint512_vec_dt * kvdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t * vertexptrs, value_t * verticesdata, keyvalue_t * edges[NUMCPUTHREADS][NUMSUBCPUTHREADS]){ // pagerank
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef LOCKE
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<">>> procedges::start... running WorkerThread "<<j<<endl; 
			#endif
			WorkerThread(kvdram[i][j], vertexptrs, verticesdata, edges[i][j]);
		}
	}
	#else 
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<">>> procedges::start... running WorkerThread "<<j<<endl; 
			#endif
			mythread[j] = std::thread(&procedges::WorkerThread, this, kvdram[i][j], vertexptrs, verticesdata, edges[i][j]);
		}
	}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mythread[j].join(); }}
	#endif
	
	#ifdef _DEBUGMODE_TIMERS2
	utilityobj->stopBTIME("processedges:: total time elapsed: ", begintime, NAp);
	#endif
	#ifdef _DEBUGMODE_TIMERS3
	long double processedgestimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendprocessedgestimeelapsed(processedgestimeelapsed_ms);
	#endif
	return;
}
void procedges::start(uint512_vec_dt * kvdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], value_t * verticesdata[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * edges[NUMCPUTHREADS][NUMSUBCPUTHREADS]){ // bfs, sssp
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef LOCKE
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<">>> procedges::start2... running WorkerThread "<<j<<endl; 
			#endif
			WorkerThread(kvdram[i][j], vertexptrs[i][j], verticesdata[i][j], edges[i][j]);
		}
	}
	#else 
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<">>> procedges::start2... running WorkerThread "<<j<<endl; 
			#endif
			mythread[j] = std::thread(&procedges::WorkerThread, this, kvdram[i][j], vertexptrs[i][j], verticesdata[i][j], edges[i][j]);
		}
	}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mythread[j].join(); }}
	#endif
	
	#ifdef _DEBUGMODE_TIMERS2
	utilityobj->stopBTIME("processedges:: total time elapsed: ", begintime, NAp);
	#endif
	#ifdef _DEBUGMODE_TIMERS3
	long double processedgestimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendprocessedgestimeelapsed(processedgestimeelapsed_ms);
	#endif
	return;
}

void procedges::WorkerThread(uint512_vec_dt * kvdram, edge_t * vertexptrs, value_t * verticesdata, keyvalue_t * edges){
	unsigned int srcvoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key;
	unsigned int srcvsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key;
	unsigned int runsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	unsigned int runsize_kvs = runsize / VECTOR_SIZE;
	unsigned int edgessize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE].data[0].key;
	unsigned int GraphIter = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key;
	unsigned int GraphAlgo = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key;
	unsigned int kvuploadcount = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> procedges::WorkerThread... runsize: "<<runsize<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	processedges(vertexptrs, verticesdata, edges, (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS], srcvoffset, srcvsize, edgessize, GraphIter, GraphAlgo);														
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("procedges::WorkerThread.kvdram.first", (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS], 16);
	actsutilityobj->printkeyvalues("procedges::WorkerThread.kvdram.last", (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS+runsize_kvs-16], 16);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif
	// exit(EXIT_SUCCESS);
	return;
}






