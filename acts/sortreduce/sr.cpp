#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include "EdgeProcess.h"
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h"
#include "sortreduce.h"
#include "types.h"
#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"
#include "EdgeProcess.h"
#include "VertexValues.h"
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "../../src/utility/utility.h"
#include "../../acts/actsutility/actsutility.h"
#include "sr.h"
using namespace std;

sr::sr(){ actsutilityobj = new actsutility(); }
sr::~sr(){}

unsigned int sr::procedgesfunc(unsigned int Uprop, unsigned int edgeweight){
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = Uprop;
	#elif defined(BFS_ALGORITHM)
	ret = Uprop + edgeweight;
	#elif defined(BC_ALGORITHM)
	ret = Uprop + edgeweight;
	#endif
	return ret;
}
void sr::processedges(keyvalue_t * vertexptrs, value_t * vertexdatas, keyvalue_t * edges, keyvalue_t * keyvalues, unsigned int srcvoffset, unsigned int srcvsize, unsigned int edgessize){							
	unsigned int firstvptr = vertexptrs[srcvoffset].key;
	unsigned int k = 0;
	
	// for(unsigned int i=srcvoffset; i<(srcvoffset + srcvsize); i++){
	for(unsigned int i=0; i<srcvsize; i++){
		unsigned int srcvid = srcvoffset + i; // actvvs[i];
		
		// cout<<"--------------- sr::run:: srcvid: "<<srcvid<<endl;
		// continue;
		
		unsigned int sourcedata = vertexdatas[srcvid];
		vertex_t myfirstsrcvid = srcvoffset + i;
		vertex_t mylastsrcvid = myfirstsrcvid + 1;
		
		// keyy_t beginvptr = vertexptrs[srcvid].key - firstvptr;
		// keyy_t endvptr = vertexptrs[srcvid + 1].key - firstvptr;
		keyy_t beginvptr = vertexptrs[srcvoffset + i].key - firstvptr;
		keyy_t endvptr = vertexptrs[srcvoffset + i + 1].key - firstvptr;
		edge_t numedges = endvptr - beginvptr;
		// cout<<"--- beginvptr: "<<beginvptr<<endl;
		// cout<<"--- endvptr: "<<endvptr<<endl;
		// cout<<"--- numedges: "<<numedges<<endl;
		// exit(EXIT_SUCCESS);
		
		#ifdef _DEBUGMODE_CHECKS2
		if(endvptr < beginvptr){ cout<<"sr::processedges::ERROR: localendvptr < beginvptr. beginvptr: "<<beginvptr<<", endvptr: "<<endvptr<<endl; exit(EXIT_FAILURE); }
		if(endvptr < edgessize){ actsutilityobj->checkptr(myfirstsrcvid, mylastsrcvid, beginvptr, endvptr, edges); }
		#endif
		
		for(unsigned int j=beginvptr; j<(beginvptr + numedges); j++){
			keyvalue_t edge = edges[j];
			
			keyvalue_t vertexupdate;
			vertexupdate.key = edge.value;
			vertexupdate.value = procedgesfunc(sourcedata, 1);
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
			#endif 
			
			keyvalues[k] = vertexupdate; 
			k += 1;
		}
	}
	return;
}

#ifdef GRAFBOOST_SETUP
void sr::srtopkernel(SortReduce<uint64_t,uint32_t>* _sr, uint512_dt * kvdram){
	unsigned int srcvoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key;
	unsigned int srcvsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key;
	unsigned int runsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	unsigned int runsize_kvs = runsize / VECTOR_SIZE;
	unsigned int edgessize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE].data[0].key;
	unsigned int kvuploadcount = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> SortReduce Launched... runsize: "<<runsize<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	processedges((keyvalue_t *)&kvdram[BASEOFFSET_VERTEXPTR_KVS], (value_t *)&kvdram[BASEOFFSET_VERTICESDATA_KVS], (keyvalue_t *)&kvdram[BASEOFFSET_EDGESDATA_KVS], (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS], srcvoffset, srcvsize, edgessize);														
	actsutilityobj->printkeyvalues("sr::srtopkernel.kvdram.first", (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS], 16);
	actsutilityobj->printkeyvalues("sr::srtopkernel.kvdram.last", (keyvalue_t *)&kvdram[BASEOFFSET_KVDRAM_KVS+runsize_kvs-16], 16);
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	actsutilityobj->printglobalvars();
	#endif 
	// return;
	// exit(EXIT_SUCCESS);
	
	for (unsigned int i = 0; i < runsize_kvs; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			uint64_t key = (uint64_t)(kvdram[BASEOFFSET_KVDRAM_KVS + i].data[v].key);
			uint64_t value = (uint64_t)(kvdram[BASEOFFSET_KVDRAM_KVS + i].data[v].value);
			if(i % 1000000 == 0){ cout<<"i: "<<i<<", key: "<<key<<", value: "<<value<<endl; }
			while ( !_sr->Update(key, value) ) { }
			kvuploadcount += 1;
		}
	}
	cout<<"srtopkernel: number of key-value pairs uploaded for sorting: "<<kvuploadcount<<endl;
	return;
}
#endif 





