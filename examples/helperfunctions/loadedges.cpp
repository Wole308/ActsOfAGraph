#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/evalparams.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "loadedges.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define TWOO 2

loadedges::loadedges(graph * _graphobj, stats * _statsobj){
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
	evalparamsobj = new evalparams();
}
loadedges::loadedges(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadedges::~loadedges(){} 

globalparams_TWOt loadedges::loadedgess(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){			
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadedges::loadedges:: loading edges (rowwise)... "<<endl;
	#endif 
	
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsK.SIZE_MESSAGESDRAM;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsE.SIZE_MESSAGESDRAM;
	#endif  
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#else 
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#endif 
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadedges.BASEOFFSETKVS_EDGESDATA", globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSET_EDGESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSET_EDGESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA<<"]"<<endl;
	#endif 
	
	vptr_type * tempvptrs[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ tempvptrs[i] = new vptr_type[KVDATA_RANGE]; }
	unsigned int counts[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ counts[i] = 0; }
	unsigned int index1 = 0;
	unsigned int index2 = 0;
	
	unsigned int packingfactor = 2;
	
	unsigned int vid=0;
	for(vid=0; vid<KVDATA_RANGE-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		if(vid % 100000 == 0){ cout<<"### loadedges::loadedges:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadedges::loadedges:: vptr_begin: "<<vptr_begin<<endl;
		utilityobj->printvalues("loadedges.counts", (value_t *)counts, NUMSUBCPUTHREADS);
		cout<<"loadedges::loadedges:: vptr_end: "<<vptr_end<<endl;
		cout<<"loadedges::loadedges:: edges_size: "<<edges_size<<endl;
		#endif
		
		unsigned int subthread = 0;
		unsigned int k=0;
		while(k<edges_size){
			subthread = (index1 / VECTOR2_SIZE) % NUMSUBCPUTHREADS;
			
			// setting header metadata
			if(counts[subthread] % VECTOR2_SIZE == 0){
				edges[subthread][TWOO*_BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = edgedatabuffer[index2].srcvid; // INVALIDDATA, 777
				counts[subthread] += 1;
				index1 += 1;
				continue;
			}
			
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("loadedges::loadedges 23. packingfactor*_BASEOFFSET_EDGESDATA + counts[subthread]", packingfactor*_BASEOFFSET_EDGESDATA + counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor, packingfactor*_BASEOFFSET_EDGESDATA, counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor);				
			#endif 
		
			edges[subthread][TWOO*_BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = edgedatabuffer[index2].dstvid;
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"k: "<<k<<", subthread: "<<subthread<<", counts["<<subthread<<"]: "<<counts[subthread]<<", index2: "<<index2<<", edgedatabuffer["<<index2<<"].dstvid: "<<edgedatabuffer[index2].dstvid<<endl;
			#endif
			
			counts[subthread] += 1;
			index1 += 1;
			index2 += 1;
			k += 1;
		}
		
		// fill in the blanks
		if(counts[subthread] % VECTOR2_SIZE != 0 && edges_size > 0){
			unsigned int sz = VECTOR2_SIZE - (counts[subthread] % VECTOR2_SIZE);
			for(unsigned int v=0; v<sz; v++){
				edges[subthread][TWOO*_BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = INVALIDDATA;
				counts[subthread] += 1;
				index1 += 1;
			}
		}
		
		// load temp vertex ptrs 
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
			if(vid < KVDATA_RANGE-1){
				tempvptrs[i][vid + 1].key = counts[i]; 
				
				#ifdef _DEBUGMODE_CHECKS2
				if(counts[i] % 16 != 0){ cout<<"loadedges:: ERROR. counts["<<i<<"] % 16 != 0. counts["<<i<<"]: "<<counts[i]<<". exiting..."<<endl; exit(EXIT_FAILURE); }
				#endif 
			}
		}
	}
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(graphobj->get_num_vertices());
		container->edgessize[i] = counts[i]; 
		container->runsize[i] = counts[i]; // FIXME. 1
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	// >>> create vertex pointers 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0; 
	globalparams.globalparamsK.SIZE_EDGES = 0; 
	
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + ((container->edgessize[0]/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = KVDATA_RANGE / VPTR_SHRINK_RATIO; 
	globalparams.globalparamsE.SIZE_EDGES = container->edgessize[0] + 1000; 
	#else 
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA + ((container->edgessize[0]/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = KVDATA_RANGE / VPTR_SHRINK_RATIO;
	globalparams.globalparamsK.SIZE_EDGES = container->edgessize[0] + 1000; 
	#endif
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadedges.globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR", globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);	
	utilityobj->checkoutofbounds("loadedges.globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR", globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);		
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSET_VERTEXPTR: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSET_VERTEXPTR: "<<globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR: "<<globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR<<"]"<<endl;
	#endif 
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams_t globalparamsVPTRS = globalparams.globalparamsE;
	#else 
	globalparams_t globalparamsVPTRS = globalparams.globalparamsK;
	#endif 
	
	unsigned int _index = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
	_index = 0;
		for(unsigned int k=0; k<KVDATA_RANGE; k+=VPTR_SHRINK_RATIO){
			vptrs[i][(TWOO * globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + _index] = tempvptrs[i][k];
			_index += 1;
		}
		for(unsigned int k=0; k<(DRAMPADD/2); k++){ // dummy pads
			vptrs[i][(TWOO * globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + _index].key = counts[i];
			_index += 1;
		}
	}
	#ifdef NOT_USED
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int k=0; k<KVDATA_RANGE; k++){
			vptrs[i][(TWOO * globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + k] = tempvptrs[i][k];
		}
	}
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ // dummy pads
		for(unsigned int k=vid; k<vid + (DRAMPADD/2); k++){
			vptrs[i][(TWOO * globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + k + 1].key = counts[i];
		}
	}
	#endif 
	
	// >>> dummy writes
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		for(unsigned int k=counts[i]; k<counts[i] + (DRAMPADD/2); k++){
			edges[i][TWOO*_BASEOFFSET_EDGESDATA + k].dstvid = UNUSEDDATA;
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	unsigned int totalsize = 0;
	cout<<"loadedges:: loading Acts... sizes: ["; 
	cout<<"["; for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ totalsize += container->edgessize[i]; cout<<container->edgessize[i]; if(i<NUMSUBCPUTHREADS-1){ cout<<", "; }} cout<<"]";
	cout<<"]"<<endl;
	cout<<"loadedges:: total sizes: "<<totalsize<<endl; 
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadedges::loadedges:: index1: "<<index1<<", index2: "<<index2<<endl;
	utilityobj->printvalues("loadedges::loadedges.counts", (value_t *)counts, NUMSUBCPUTHREADS);
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges::loadedges.edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][TWOO*_BASEOFFSET_EDGESDATA], 8); }
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges.vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][2*_BASEOFFSET_VERTEXPTR], 8); }
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	unsigned int numvaliditems = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int k=0; k<counts[i]; k++){ 
			unsigned int value = edges[i][TWOO*_BASEOFFSET_EDGESDATA + k].dstvid;
			if(value == INVALIDDATA || value == 777){ }
			else { numvaliditems += 1; }
		}
	}
	cout<<"loadedges::loadedges:: number of valid items: "<<numvaliditems<<endl;
	#endif
	
	unsigned int maxvptr = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"container->edgessize["<<i<<"] (last vptr): "<<container->edgessize[i]<<endl;
		#endif 
		if(maxvptr < counts[i]){ maxvptr = counts[i]; }
	}
	cout<<"maxvptr: "<<maxvptr<<endl;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ tempvptrs[i] = NULL; }
	
	return globalparams;
}

