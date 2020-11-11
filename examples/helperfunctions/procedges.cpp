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
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../../acts/include/actscommon.h"
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif 
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
#include "procedges.h"
using namespace std;

procedges::procedges(){ actsutilityobj = new actsutility(); }
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
		unsigned int srcvid = srcvoffset + i; // actvvs[i];
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("procedges::processedges.srcvid", srcvid, KVDATA_RANGE, NAp, NAp, NAp);
		#endif 
		unsigned int sourcedata = verticesdata[srcvid]; // vertexptrs[srcvid].value; // vertexdatas[srcvid];
		vertex_t myfirstsrcvid = srcvoffset + i;
		vertex_t mylastsrcvid = myfirstsrcvid + 1;
		
		keyy_t beginvptr = vertexptrs[srcvoffset + i] - firstvptr;
		keyy_t endvptr = vertexptrs[srcvoffset + i + 1] - firstvptr;
		edge_t numedges = endvptr - beginvptr;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(endvptr < beginvptr){ cout<<"procedges::processedges::ERROR: localendvptr < beginvptr. beginvptr: "<<beginvptr<<", endvptr: "<<endvptr<<endl; exit(EXIT_FAILURE); }
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
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
			#endif
		}
	}
	return;
}

void procedges::start(uint512_dt * kvdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t * vertexptrs, value_t * verticesdata, keyvalue_t * edges[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			#ifdef _DEBUGMODE_KERNELPRINTS2
			cout<<">>> procedges::start... running WorkerThread "<<j<<endl; 
			#endif
			WorkerThread(kvdram[i][j], vertexptrs, verticesdata, edges[i][j]);
		}
	}
	return;
}
void procedges::WorkerThread(uint512_dt * kvdram, edge_t * vertexptrs, value_t * verticesdata, keyvalue_t * edges){
	unsigned int srcvoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key;
	unsigned int srcvsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key;
	unsigned int runsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	unsigned int runsize_kvs = runsize / VECTOR_SIZE;
	unsigned int edgessize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE].data[0].key;
	unsigned int GraphIter = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key;
	unsigned int GraphAlgo = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key;
	unsigned int kvuploadcount = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> procedges::WorkerThread... runsize: "<<runsize<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	processedges(vertexptrs, verticesdata, edges, (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS], srcvoffset, srcvsize, edgessize, GraphIter, GraphAlgo);														
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("procedges::srtopkernel.kvdram.first", (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS], 16);
	actsutilityobj->printkeyvalues("procedges::srtopkernel.kvdram.last", (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS+runsize_kvs-16], 16);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif
	return;
}







