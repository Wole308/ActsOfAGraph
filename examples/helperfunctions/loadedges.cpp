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

unsigned int loadedges::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth){
	unsigned int partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS
	utilityobj->checkoutofbounds("loadedges::getglobalpartition", partition, (1 << (NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}
globalparams_TWOt loadedges::loadoffsetmarkers(vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadedges::loadoffsetmarkers:: loading offset markers... "<<endl;
	#endif
	
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + (globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = ACTIVE_KVSTATSDRAMSZ *
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		NUM_EDGE_BANKS;
		#else 
		1;	
		#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK + (globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_KVSTATSDRAM = ACTIVE_KVSTATSDRAMSZ;// * NUM_EDGE_BANKS; // NEWCHANGE.
	#endif 
	
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM + globalparams.globalparamsK.SIZE_KVSTATSDRAM; 
	globalparams.globalparamsK.SIZE_EDGESSTATSDRAM = 
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		0;
		#else 
		EDGESSTATSDRAMSZ;	
		#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM + globalparams.globalparamsE.SIZE_KVSTATSDRAM;
	globalparams.globalparamsE.SIZE_EDGESSTATSDRAM = EDGESSTATSDRAMSZ; // 0; // NEWCHANGE.
	#endif 
	
	// calculate best-fit value for NUM_EDGECHUNKS_IN_A_BUFFER
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int num_edges_per_channel = globalparams.globalparamsE.SIZE_EDGES; // E/NUMSUBCPUTHREADS;
	unsigned int num_vertices_per_channel = KVDATA_RANGE;
	unsigned int A = 4 * num_edges_per_channel;
	unsigned int B = (256000000/4) - (3 * num_vertices_per_channel);
	unsigned int num_edgechunks_in_a_buffer = (A + (B-1)) / B; // 3V + 2E/N + 2E/N = (256MB/4)
	cout<<"---------------------------------------------------------------------- A: "<<A<<", B: "<<B<<", num_edgechunks_in_a_buffer: "<<num_edgechunks_in_a_buffer<<endl;
	#else 
	unsigned int num_edgechunks_in_a_buffer = 1;	
	#endif
	
	globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = num_edgechunks_in_a_buffer; // NUM_EDGECHUNKS_IN_A_BUFFER;
	globalparams.globalparamsE.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;

	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#else 
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#endif
	unsigned int _KVSTATSDRAMSZ = globalparams.globalparamsK.SIZE_KVSTATSDRAM; // CHANGEPOINT.
	unsigned int _KVSTATSSZ = _KVSTATSDRAMSZ / VECTOR_SIZE;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams_t globalparamsVPTRS = globalparams.globalparamsE;
	globalparams_t globalparamsSTATS = globalparams.globalparamsE;
	#else 
	globalparams_t globalparamsVPTRS = globalparams.globalparamsK;
	globalparams_t globalparamsSTATS = globalparams.globalparamsK;
	#endif
	globalparams_t globalparamsEDGESSTATS =  
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		globalparams.globalparamsE;
		#else 
		globalparams.globalparamsK;
		#endif 
	
	unsigned int _BASEOFFSET_STATSDRAM = globalparamsSTATS.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE; // NEWCHANGE.
	unsigned int _BASEOFFSET_EDGESSTATSDRAM = globalparamsSTATS.BASEOFFSETKVS_EDGESSTATSDRAM * VECTOR_SIZE; 
	unsigned int _BASEOFFSET_VERTEXPTR = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE;
	unsigned int _NUM_EDGECHUNKS_IN_A_BUFFER = globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_STATSDRAM", globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSET_STATSDRAM: "<<globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM: "<<globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM<<"]"<<endl;
	#endif 
	
	unsigned int totalnumpartitions = 0;
	for(unsigned int k=0; k<=TREE_DEPTH; k++){ totalnumpartitions += (unsigned int)pow(NUM_PARTITIONS, k); } 
	unsigned int maxdramsz = 0;

	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<_KVSTATSSZ; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				statsptrVec[k].data[v].key = 0;
				statsptrVec[k].data[v].value = 0;
			}
		}
	}
	
	keyvalue_t **tempstats = new keyvalue_t*[_NUM_EDGECHUNKS_IN_A_BUFFER];
	for(unsigned int i = 0; i < _NUM_EDGECHUNKS_IN_A_BUFFER; ++i){ tempstats[i] = new keyvalue_t[KVSTATSDRAMSZ]; }
	for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){ tempstats[u][k].key = 0; tempstats[u][k].value = 0; }}
	
	unsigned int src1 = 0;
	unsigned int src2 = 0;
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadedges::loadoffsetmarkers:: edgessize["<<i<<"]: "<<container->edgessize[i]<<endl;
		#endif 
		edge_type * edgesptr = (edge_type *)&edges[i][TWOO*_BASEOFFSET_EDGESDATA];
		keyvalue_t * statsptr = (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){ tempstats[u][k].key = 0; tempstats[u][k].value = 0; }}
		
		src1 = 0;
		src2 = 0;
		
		unsigned int SRC_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER+1];
		unsigned int PARTITION_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER+1];
		for(unsigned int k=0; k<_NUM_EDGECHUNKS_IN_A_BUFFER; k++){
			unsigned int index = k * (container->edgessize[i] / _NUM_EDGECHUNKS_IN_A_BUFFER);
			unsigned int srC = edgesptr[(index/VECTOR2_SIZE) * VECTOR2_SIZE].dstvid; // round down
			srC = (srC / (REDUCESZ * VDATA_PACKINGSIZE)) * (REDUCESZ * VDATA_PACKINGSIZE); // round down
			SRC_CHKPT[k] = srC;
			PARTITION_CHKPT[k] = srC / (REDUCESZ * VDATA_PACKINGSIZE);
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"loadedges::loadoffsetmarkers: k: "<<k<<", index: "<<index<<", srC: "<<srC<<", SRC_CHKPT["<<k<<"]: "<<SRC_CHKPT[k]<<", PARTITION_CHKPT["<<k<<"]: "<<PARTITION_CHKPT[k]<<" "<<endl; 
			#endif 
		}
		SRC_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = KVDATA_RANGE-1;
		PARTITION_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = (1 << (NUM_PARTITIONS_POW * (TREE_DEPTH-1))); // 256
		
		for(unsigned int k=0; k<container->edgessize[i]; k++){
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(k % 100000 == 0){ cout<<"### loadedges::loadoffsetmarkers:: k: "<<k<<endl; }
			#endif
			
			keyvalue_t keyvalue;
			keyvalue.key = edgesptr[k].dstvid;
			keyvalue.value = 0;
			
			if(k % VECTOR2_SIZE == 0){ src2 = edgesptr[k].dstvid; } 
			#ifdef _DEBUGMODE_CHECKS
			if(k % VECTOR2_SIZE == 0){ if(k%1000000 == 0){ cout<<"loadedges::loadoffsetmarkers: src1: "<<src1<<", src2: "<<src2<<", k: "<<k<<", i: "<<i<<endl; }}
			if(src2 < src1){ cout<<"loadedges::loadoffsetmarkers: ERROR SOMEWHERE. src1: "<<src1<<", src2: "<<src2<<", k: "<<k<<", i: "<<i<<endl; exit(EXIT_FAILURE); }
			src1 = src2;
			#endif 
			
			if(keyvalue.key == INVALIDDATA){ keyvalue.key = 0; } // >>> for loadedges_rowblockwise
			
			for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
				unsigned int offset = 0;
				for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); } 
				
				unsigned int partitionCLOP = getglobalpartition(keyvalue, 0, BATCH_RANGE_POW, CLOP);
				
				#ifdef _DEBUGMODE_CHECKS
				utilityobj->checkoutofbounds("loadedges::loadoffsetmarkers.partitionCLOP.1", partitionCLOP, pow(NUM_PARTITIONS, TREE_DEPTH), keyvalue.key, container->edgessize[i], NAp);
				utilityobj->checkoutofbounds("loadedges::loadoffsetmarkers.partitionCLOP.2", offset + partitionCLOP, _KVSTATSDRAMSZ, keyvalue.key, container->edgessize[i], NAp);
				#endif
				
				#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
				unsigned int p=0;
				for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ if(src2 >= SRC_CHKPT[u] && src2 < SRC_CHKPT[u+1]){ p = u; } }
				#ifdef _DEBUGMODE_CHECKS3
				utilityobj->checkoutofbounds("loadedges::loadoffsetmarkers.p.1", p, _NUM_EDGECHUNKS_IN_A_BUFFER, keyvalue.key, vptrs[i][TWOO*_BASEOFFSET_VERTEXPTR + keyvalue.key].key, NAp);
				#endif
				tempstats[p][offset + partitionCLOP].value += 1;
				#else 
				tempstats[0][offset + partitionCLOP].value += 1;	
				#endif 
			}
		}
		
		for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
			unsigned int offset = 0;
			for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); }
			
			utilityobj->checkoutofbounds("loadedges::loadoffsetmarkers.offset", offset, KVSTATSDRAMSZ, NAp, NAp, NAp);
			for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ utilityobj->getmarkerpositions((keyvalue_t *)&tempstats[u][offset], (unsigned int)pow(NUM_PARTITIONS, CLOP)); }
		}
		
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){
			for(unsigned int k=0; k<_KVSTATSSZ; k++){
				statsptrVec[k].data[u].key = tempstats[u][k].key;
				statsptrVec[k].data[u].value = 0; // CRITICAL REMOVEME.
				// statsptrVec[k].data[u].value = tempstats[u][k].value; // CRITICAL REMOVEME.
			}
		}
		
		uint512_vec_dt * edgesstatsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_EDGESSTATSDRAM];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER+1; u++){
			edgesstatsptrVec[u].data[0].key = PARTITION_CHKPT[u];
			edgesstatsptrVec[u].data[0].value = 0;
		}
		
		unsigned int totalnumpb4llop = 0;
		#ifdef ENABLERECURSIVEPARTITIONING
		for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpb4llop += (unsigned int)pow(NUM_PARTITIONS, k); } 
		#else 
		for(unsigned int k=0; k<TREE_DEPTH+1; k++){ totalnumpb4llop += (unsigned int)pow(NUM_PARTITIONS, k); } 	// NOTE: because 
		#endif
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){
			if(maxdramsz < (tempstats[u][totalnumpb4llop-1].key + tempstats[u][totalnumpb4llop-1].value)){ maxdramsz = tempstats[u][totalnumpb4llop-1].key + tempstats[u][totalnumpb4llop-1].value; }
		}
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadoffsetmarkers:: maxdramsz: "<<maxdramsz<<endl;
		#endif 
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats [after]", tempstats, totalnumpb4llop);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing statsptr [after]", (keyvalue_t *)statsptrVec, totalnumpb4llop * VECTOR_SIZE, VECTOR_SIZE); // totalnumpartitions
		cout<<"tempstats["<<totalnumpb4llop-1<<"].key: "<<tempstats[totalnumpb4llop-1].key<<", tempstats["<<totalnumpb4llop-1<<"].value: "<<tempstats[totalnumpb4llop-1].value<<endl;
		#endif
		#ifdef _DEBUGMODE_HOSTPRINTS
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ cout<<"loadoffsetmarkers: u:"<<u<<endl; utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats[u] [after]", tempstats[u], 17); } //280
		#endif
	}
	
	globalparams.globalparamsK.SIZE_KVDRAM = maxdramsz + 64; // CHECKME. FIXME.
	globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = globalparams.globalparamsK.SIZE_KVDRAM; // maxdramsz;
	globalparams.globalparamsK.SIZE_OTHERINFOS = 0; // 64 * VECTOR_SIZE;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.SIZE_KVDRAM = 0; 
	globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	globalparams.globalparamsE.SIZE_OTHERINFOS = 0;
	#endif  
	
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + globalparams.globalparamsK.SIZE_EDGESSTATSDRAM; //EDGESSTATSDRAMSZ; // NEWCHANGE.
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM + (globalparams.globalparamsK.SIZE_KVDRAM / VECTOR_SIZE); 
	globalparams.globalparamsK.BASEOFFSETKVS_OTHERINFOS = globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM + globalparams.globalparamsE.SIZE_EDGESSTATSDRAM; // NEWCHANGE.
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM + (globalparams.globalparamsE.SIZE_KVDRAM / VECTOR_SIZE);
	globalparams.globalparamsE.BASEOFFSETKVS_OTHERINFOS = globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE); 
	#endif
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_KVDRAM", globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_KVDRAMWORKSPACE", globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	for(unsigned int i=0; i<1; i++){
		// utilityobj->printkeyvalues("loadoffsetmarkers: printing edges[i][_BASEOFFSET_EDGESDATA]", (keyvalue_t *)&edges[i][baseoffset_edgedata], 16);
		// utilityobj->printkeyvalues("loadoffsetmarkers: printing edges[i][_BASEOFFSET_EDGESDATA][last]", (keyvalue_t *)&edges[i][baseoffset_edgedata+container->runsize[i]-32], 16);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][_BASEOFFSET_STATSDRAM]", (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM], (1+16) * VECTOR_SIZE, VECTOR_SIZE);
		// utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][_BASEOFFSET_STATSDRAM]", (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM], totalnumpartitions * VECTOR_SIZE, VECTOR_SIZE);
	}
	#endif
	// exit(EXIT_SUCCESS); //
	return globalparams;
}
void loadedges::accumstats(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], globalparams_TWOt globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadedges::accumstats:: accumulating stats to base... "<<endl;
	#endif

	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i+=NUM_EDGE_BANKS){
		uint512_vec_dt * basestatsptrVec = (uint512_vec_dt *)&kvbuffer[i/NUM_EDGE_BANKS][globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM];
		for(unsigned int j=0; j<NUM_EDGE_BANKS; j++){
			uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i + j][globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM];
			for(unsigned int k=0; k<ACTIVE_KVSTATSSZ; k++){
				basestatsptrVec[j*ACTIVE_KVSTATSSZ + k] = statsptrVec[k];
				// basestatsptrVec[j*ACTIVE_KVSTATSSZ + k] = statsptrVec[k];
			}
		}
	}
	return;
}
