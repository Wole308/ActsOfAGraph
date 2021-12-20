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
// #include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "loadgraph.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define TWOO 2

loadgraph::loadgraph(graph * _graphobj, stats * _statsobj){
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
	evalparamsobj = new evalparams();
}
loadgraph::loadgraph(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadgraph::~loadgraph(){} 

unsigned int loadgraph::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth){
	unsigned int partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS
	utilityobj->checkoutofbounds("loadgraph::getglobalpartition", partition, (1 << (NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}

#ifdef XXX
globalparams_TWOt loadgraph::loadedges_rowblockwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){			
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadedges_rowblockwise:: loading edges (rowwise)... "<<endl;
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
	utilityobj->checkoutofbounds("loadedges_rowblockwise.BASEOFFSETKVS_EDGESDATA", globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
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
		if(vid % 100000 == 0){ cout<<"### loadgraph::loadedges_rowblockwise:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadgraph::loadedges_rowblockwise:: vptr_begin: "<<vptr_begin<<endl;
		utilityobj->printvalues("loadedges_rowblockwise.counts", (value_t *)counts, NUMSUBCPUTHREADS);
		cout<<"loadgraph::loadedges_rowblockwise:: vptr_end: "<<vptr_end<<endl;
		cout<<"loadgraph::loadedges_rowblockwise:: edges_size: "<<edges_size<<endl;
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
			utilityobj->checkoutofbounds("loadgraph::loadedges_rowblockwise 23. packingfactor*_BASEOFFSET_EDGESDATA + counts[subthread]", packingfactor*_BASEOFFSET_EDGESDATA + counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor, packingfactor*_BASEOFFSET_EDGESDATA, counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor);				
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
				if(counts[i] % 16 != 0){ cout<<"loadedges_rowblockwise:: ERROR. counts["<<i<<"] % 16 != 0. counts["<<i<<"]: "<<counts[i]<<". exiting..."<<endl; exit(EXIT_FAILURE); }
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
	utilityobj->checkoutofbounds("loadedges_rowblockwise.globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR", globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);	
	utilityobj->checkoutofbounds("loadedges_rowblockwise.globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR", globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);		
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
	cout<<"loadedges_rowblockwise:: loading Acts... sizes: ["; 
	cout<<"["; for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ totalsize += container->edgessize[i]; cout<<container->edgessize[i]; if(i<NUMSUBCPUTHREADS-1){ cout<<", "; }} cout<<"]";
	cout<<"]"<<endl;
	cout<<"loadedges_rowblockwise:: total sizes: "<<totalsize<<endl; 
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::loadedges_rowblockwise:: index1: "<<index1<<", index2: "<<index2<<endl;
	utilityobj->printvalues("loadgraph::loadedges_rowblockwise.counts", (value_t *)counts, NUMSUBCPUTHREADS);
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadgraph::loadedges_rowblockwise.edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][TWOO*_BASEOFFSET_EDGESDATA], 8); }
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges_rowblockwise.vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][2*_BASEOFFSET_VERTEXPTR], 8); }
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
	cout<<"loadgraph::loadedges_rowblockwise:: number of valid items: "<<numvaliditems<<endl;
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
#endif 

globalparams_t loadgraph::loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer, vertex_t kvbufferoffset, vertex_t size, globalparams_t globalparams, unsigned int edgesorkv, unsigned int srcordest){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadvertexdata:: loading vertex datas... "<<endl;
	#endif
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadvertexdata.BASEOFFSETKVS_DESTVERTICESDATA", globalparams.BASEOFFSETKVS_DESTVERTICESDATA, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif
	
	memcpy(&kvbuffer[kvbufferoffset], &vertexdatabuffer[0], size * sizeof(value_t));
	return globalparams;
} 

void loadgraph::setrootvid(value_t * kvbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::setrootvid:: setting root vid(s)... "<<endl;
	#endif 
	
	unsigned int baseoffset1 = globalparams.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE; 
	unsigned int baseoffset2 = globalparams.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE; 
	
	for(unsigned int i = 0; i < activevertices.size(); i++){
		unsigned int vid = activevertices[i];
		kvbuffer[2*baseoffset1 + vid] = 0;
		kvbuffer[2*baseoffset2 + vid] = 0;
	}
	return;
}

globalparams_TWOt loadgraph::loadactvvertices(vector<vertex_t> &activevertices, keyy_t * kvbuffer, container_t * container, globalparams_TWOt globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadactvvertices:: loading active vertices... "<<endl;
	#endif
	
	globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.globalparamsK.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_ACTIVEVERTICES = ACTIVEVERTICESSZ;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.globalparamsE.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_ACTIVEVERTICES = 0;
	#endif 
	globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.globalparamsV.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsV.SIZE_ACTIVEVERTICES = ACTIVEVERTICESSZ;
	
	unsigned int _BASEOFFSET_ACTIVEVERTICES = globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadactvvertices.BASEOFFSETKVS_ACTIVEVERTICES", globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	#endif 
	return globalparams;
}

void loadgraph::savevmasks(bool_type enable, uint512_vec_dt * kvbuffer, keyvalue_vec_bittype vmask[VMASK_PACKINGSIZE][REDUCEBUFFERSZ], batch_type offset_kvs, buffer_type size_kvs){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::savevmasks:: saving vmask... "<<endl;
	#endif
	unsigned int bitsbuffer[REDUCEBUFFERSZ];
	keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	
	for (buffer_type i=0; i<REDUCEBUFFERSZ; i++){
		for(unsigned int k=0; k<VMASK_PACKINGSIZE; k++){
			utilityobj->WRITETO_UINT(&bitsbuffer[i], 2*k, 1, vmask[k][i].key);
			utilityobj->WRITETO_UINT(&bitsbuffer[i], 2*k+1, 1, vmask[k][i].value);
		}
	}
	
	unsigned int index = 0;
	for (buffer_type i=0; i<VMASKBUFFERSZ_KVS; i++){
		for (vector_type v=0; v<VECTOR_SIZE; v++){
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("loadgraph::savevmasks_h 1.1", index, REDUCEBUFFERSZ, NAp, NAp, NAp);
			utilityobj->checkoutofbounds("loadgraph::savevmasks_h 1.2", i, VMASKBUFFERSZ_KVS, NAp, NAp, NAp);
			#endif
			tempbuffer[v][i].key = bitsbuffer[index]; 
			tempbuffer[v][i].value = bitsbuffer[index+1]; 
			index += 2;
		}
	}
	
	for (buffer_type i=0; i<size_kvs; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvbuffer[offset_kvs + i].data[v] = tempbuffer[v][i];
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printvalues("savevmasks.bitsbuffer", bitsbuffer, 4);
	utilityobj->printkeyvalues("savevmasks.vmask", vmask, VMASK_PACKINGSIZE, 4);
	utilityobj->printkeyvalues("savevmasks.tempbuffer", tempbuffer, 8, 4);
	utilityobj->printkeyvalues("savevmasks.kvbuffer[0]", (keyvalue_t *)&kvbuffer[offset_kvs], 4);
	#endif
	return;
}
globalparams_TWOt loadgraph::generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * vdram, globalparams_TWOt globalparams){ 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::generatevmaskdata:: generating vmask... "<<endl;
	#endif
	
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsK.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_VERTICESDATAMASK = VERTICESDATAMASKSZ;
	
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsK.SIZE_VERTICESDATAMASK / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK = 512 * VECTOR_SIZE;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsE.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_VERTICESDATAMASK = 0;
	
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsE.SIZE_VERTICESDATAMASK / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK = 0;
	#endif 
	
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsV.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsV.SIZE_VERTICESDATAMASK = VERTICESDATAMASKSZ;
	
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsV.SIZE_VERTICESDATAMASK / VECTOR_SIZE);
	globalparams.globalparamsV.SIZE_VERTICESPARTITIONMASK = 512 * VECTOR_SIZE;
	
	unsigned int _BASEOFFSET_VERTICESDATAMASK_KVS = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK;
	unsigned int _BASEOFFSET_VERTICESPARTITIONMASK_KVS = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK;
	unsigned int _BASEOFFSETV_VERTICESDATAMASK_KVS = globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK;
	unsigned int _BASEOFFSETV_VERTICESPARTITIONMASK_KVS = globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK; // NEWCHANGE.
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("generatevmaskdata.BASEOFFSETKVS_VERTICESDATAMASK", globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	utilityobj->checkoutofbounds("generatevmaskdata.BASEOFFSETKVS_VERTICESPARTITIONMASK", globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSET_VERTICESDATAMASK: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK<<"]"<<endl;
	#endif
	
	keyvalue_vec_bittype vmask[VMASK_PACKINGSIZE][REDUCEBUFFERSZ];
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<VERTICESDATAMASKSZ_KVS; k++){ 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				#ifdef _DEBUGMODE_CHECKS3
				utilityobj->checkoutofbounds("loadgraph::generatevmaskdata 2._BASEOFFSET_VERTICESDATAMASK_KVS", _BASEOFFSET_VERTICESDATAMASK_KVS + k, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
				#endif
				kvbuffer[i][_BASEOFFSET_VERTICESDATAMASK_KVS + k].data[v].key = 0;
				kvbuffer[i][_BASEOFFSET_VERTICESDATAMASK_KVS + k].data[v].value = 0;
				
				vdram[_BASEOFFSETV_VERTICESDATAMASK_KVS + k].data[v].key = 0;
				vdram[_BASEOFFSETV_VERTICESDATAMASK_KVS + k].data[v].value = 0; // NEWCHANGE.
			}
		}
	}
	
	unsigned int vmaskoffset_kvs = 0;
	for(unsigned int offset=0; offset<BATCH_RANGE; offset+=REDUCESZ * VMASK_PACKINGSIZE){
		for(unsigned int i=0; i<VMASK_PACKINGSIZE; i++){
			unsigned int * V = (unsigned int *)vmask[i]; 
			for(unsigned int k=0; k<REDUCESZ; k++){
				unsigned int vid = offset + (i*REDUCESZ) + k;
				if(vid==activevertices[0]){ V[k] = 1; }
				else{ V[k] = 0; }
			}
		}
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ savevmasks(ON, kvbuffer[i], vmask, _BASEOFFSET_VERTICESDATAMASK_KVS + vmaskoffset_kvs, VMASKBUFFERSZ_KVS); }
		savevmasks(ON, vdram, vmask, _BASEOFFSETV_VERTICESDATAMASK_KVS + vmaskoffset_kvs, VMASKBUFFERSZ_KVS); // NEWCHANGE.
		vmaskoffset_kvs += VMASKBUFFERSZ_KVS;
	}
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ 
			#ifdef _DEBUGMODE_CHECKS3
			utilityobj->checkoutofbounds("loadgraph::generatevmaskdata 2._BASEOFFSET_VERTICESPARTITIONMASK_KVS", _BASEOFFSET_VERTICESPARTITIONMASK_KVS + k, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
			#endif
			kvbuffer[i][_BASEOFFSET_VERTICESPARTITIONMASK_KVS + k].data[0].key = 0;
			vdram[_BASEOFFSETV_VERTICESPARTITIONMASK_KVS + k].data[0].key = 0; // NEWCHANGE.
		}
		kvbuffer[i][_BASEOFFSET_VERTICESPARTITIONMASK_KVS].data[0].key = 1; // CRITICAL AUTOMATEME.
		vdram[_BASEOFFSETV_VERTICESPARTITIONMASK_KVS].data[0].key = 1; // NEWCHANGE
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printkeyvalues("generatevmaskdata.vmask", vmask, VMASK_PACKINGSIZE, 4);
	utilityobj->printkeyvalues("generatevmaskdata.kvbuffer[0]", (keyvalue_t *)&kvbuffer[0][_BASEOFFSET_VERTICESDATAMASK_KVS], 4);
	cout<<"loadgraph::generatevmaskdata:: end. "<<endl;
	#endif
	
	return globalparams;
}

globalparams_TWOt loadgraph::loadoffsetmarkers(vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadoffsetmarkers:: loading offset markers... "<<endl;
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
		cout<<"loadgraph::loadoffsetmarkers:: edgessize["<<i<<"]: "<<container->edgessize[i]<<endl;
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
			cout<<"loadgraph::loadoffsetmarkers: k: "<<k<<", index: "<<index<<", srC: "<<srC<<", SRC_CHKPT["<<k<<"]: "<<SRC_CHKPT[k]<<", PARTITION_CHKPT["<<k<<"]: "<<PARTITION_CHKPT[k]<<" "<<endl; 
			#endif 
		}
		SRC_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = KVDATA_RANGE-1;
		PARTITION_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = (1 << (NUM_PARTITIONS_POW * (TREE_DEPTH-1))); // 256
		
		for(unsigned int k=0; k<container->edgessize[i]; k++){
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(k % 100000 == 0){ cout<<"### loadgraph::loadoffsetmarkers:: k: "<<k<<endl; }
			#endif
			
			keyvalue_t keyvalue;
			keyvalue.key = edgesptr[k].dstvid;
			keyvalue.value = 0;
			
			if(k % VECTOR2_SIZE == 0){ src2 = edgesptr[k].dstvid; } 
			#ifdef _DEBUGMODE_CHECKS
			if(k % VECTOR2_SIZE == 0){ if(k%1000000 == 0){ cout<<"loadgraph::loadoffsetmarkers: src1: "<<src1<<", src2: "<<src2<<", k: "<<k<<", i: "<<i<<endl; }}
			if(src2 < src1){ cout<<"loadgraph::loadoffsetmarkers: ERROR SOMEWHERE. src1: "<<src1<<", src2: "<<src2<<", k: "<<k<<", i: "<<i<<endl; exit(EXIT_FAILURE); }
			src1 = src2;
			#endif 
			
			if(keyvalue.key == INVALIDDATA){ keyvalue.key = 0; } // >>> for loadedges_rowblockwise
			
			for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
				unsigned int offset = 0;
				for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); } 
				
				unsigned int partitionCLOP = getglobalpartition(keyvalue, 0, BATCH_RANGE_POW, CLOP);
				
				#ifdef _DEBUGMODE_CHECKS
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.partitionCLOP.1", partitionCLOP, pow(NUM_PARTITIONS, TREE_DEPTH), keyvalue.key, container->edgessize[i], NAp);
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.partitionCLOP.2", offset + partitionCLOP, _KVSTATSDRAMSZ, keyvalue.key, container->edgessize[i], NAp);
				#endif
				
				#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
				unsigned int p=0;
				for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ if(src2 >= SRC_CHKPT[u] && src2 < SRC_CHKPT[u+1]){ p = u; } }
				#ifdef _DEBUGMODE_CHECKS3
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.p.1", p, _NUM_EDGECHUNKS_IN_A_BUFFER, keyvalue.key, vptrs[i][TWOO*_BASEOFFSET_VERTEXPTR + keyvalue.key].key, NAp);
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
			
			utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.offset", offset, KVSTATSDRAMSZ, NAp, NAp, NAp);
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
void loadgraph::accumstats(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], globalparams_TWOt globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::accumstats:: accumulating stats to base... "<<endl;
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

globalparams_TWOt loadgraph::loadmessages(uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo, globalparams_TWOt globalparams){	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printcontainer(container); 
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"[globalparams.globalparamsV.BASEOFFSET_MESSAGESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_EDGESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_VERTEXPTR: "<<globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR: "<<globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_SRCVERTICESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_DESTVERTICESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_VERTICESDATAMASK: "<<globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK: "<<globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_VERTICESPARTITIONMASK : "<<globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK: "<<globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_STATSDRAM: "<<globalparams.globalparamsV.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_STATSDRAM: "<<globalparams.globalparamsV.BASEOFFSETKVS_STATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_EDGESSTATSDRAM: "<<globalparams.globalparamsV.BASEOFFSETKVS_EDGESSTATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparams.globalparamsV.BASEOFFSETKVS_EDGESSTATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_KVDRAM: "<<globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM: "<<globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSET_KVDRAMWORKSPACE: "<<globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE: "<<globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE<<"]"<<endl;

	cout<<"[globalparams.globalparamsV.SIZE_MESSAGESDRAM: "<<globalparams.globalparamsV.SIZE_MESSAGESDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_EDGES: "<<globalparams.globalparamsV.SIZE_EDGES<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_VERTEXPTRS: "<<globalparams.globalparamsV.SIZE_VERTEXPTRS<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_SRCVERTICESDATA: "<<globalparams.globalparamsV.SIZE_SRCVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsV.SIZE_DESTVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_ACTIVEVERTICES: "<<globalparams.globalparamsV.SIZE_ACTIVEVERTICES<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_VERTICESDATAMASK: "<<globalparams.globalparamsV.SIZE_VERTICESDATAMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_VERTICESPARTITIONMASK: "<<globalparams.globalparamsV.SIZE_VERTICESPARTITIONMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_KVSTATSDRAM: "<<globalparams.globalparamsV.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_EDGESSTATSDRAM: "<<globalparams.globalparamsV.SIZE_EDGESSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_KVDRAM: "<<globalparams.globalparamsV.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE: "<<globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_OTHERINFOS: "<<globalparams.globalparamsV.SIZE_OTHERINFOS<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsV.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: PADDEDKVSOURCEDRAMSZ (keyvalues): "<<PADDEDKVSOURCEDRAMSZ<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: PADDEDKVSOURCEDRAMSZ (bytes): "<<(PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t))<<" bytes"<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid workload space(not including temp space) (keyvalues): "<<(globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid workload space(not including temp space) (keyvalues): "<<(((unsigned long)globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE) * sizeof(keyvalue_t))<<" bytes. "<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid PADDEDKVSOURCEDRAMSZ (keyvalues): "<<((globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid PADDEDKVSOURCEDRAMSZ (bytes): "<<((((unsigned long)globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (HBM max="<<(unsigned int)(KVSOURCEDRAMSZ * 8)<<" bytes)"<< RESET <<std::endl;					

	if(((((unsigned long)globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + (unsigned long)globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t)) > (KVSOURCEDRAMSZ * 8)){ cout<<"ERROR: dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"[globalparams.globalparamsK.BASEOFFSET_MESSAGESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_EDGESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_VERTEXPTR: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_SRCVERTICESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_DESTVERTICESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_VERTICESDATAMASK: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_VERTICESPARTITIONMASK : "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_STATSDRAM: "<<globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM: "<<globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_EDGESSTATSDRAM: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_KVDRAM: "<<globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM: "<<globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_KVDRAMWORKSPACE: "<<globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE: "<<globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSET_OTHERINFOS: "<<globalparams.globalparamsK.BASEOFFSETKVS_OTHERINFOS * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_OTHERINFOS: "<<globalparams.globalparamsK.BASEOFFSETKVS_OTHERINFOS<<"]"<<endl;

	cout<<"[globalparams.globalparamsK.SIZE_MESSAGESDRAM: "<<globalparams.globalparamsK.SIZE_MESSAGESDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_EDGES: "<<globalparams.globalparamsK.SIZE_EDGES<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_VERTEXPTRS: "<<globalparams.globalparamsK.SIZE_VERTEXPTRS<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_SRCVERTICESDATA: "<<globalparams.globalparamsK.SIZE_SRCVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsK.SIZE_DESTVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_ACTIVEVERTICES: "<<globalparams.globalparamsK.SIZE_ACTIVEVERTICES<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_VERTICESDATAMASK: "<<globalparams.globalparamsK.SIZE_VERTICESDATAMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK: "<<globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_KVSTATSDRAM: "<<globalparams.globalparamsK.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_EDGESSTATSDRAM: "<<globalparams.globalparamsK.SIZE_EDGESSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_KVDRAM: "<<globalparams.globalparamsK.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE: "<<globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: PADDEDKVSOURCEDRAMSZ (keyvalues): "<<PADDEDKVSOURCEDRAMSZ<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: PADDEDKVSOURCEDRAMSZ (bytes): "<<(PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t))<<" bytes"<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid workload space(not including temp space) (keyvalues): "<<(globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid workload space(not including temp space) (keyvalues): "<<(((unsigned long)globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE) * sizeof(keyvalue_t))<<" bytes. "<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid PADDEDKVSOURCEDRAMSZ (keyvalues): "<<((globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid PADDEDKVSOURCEDRAMSZ (bytes): "<<((((unsigned long)globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (HBM max="<<(unsigned int)(KVSOURCEDRAMSZ * 8)<<" bytes)"<< RESET <<std::endl;					

	if(((((unsigned long)globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + (unsigned long)globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t)) > (KVSOURCEDRAMSZ * 8)){ cout<<"ERROR: dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif
	
	#if defined(_DEBUGMODE_HOSTPRINTS3) & defined(EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM)
	cout<<"[globalparams.globalparamsE.BASEOFFSET_MESSAGESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_EDGESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_VERTEXPTR: "<<globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR: "<<globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_SRCVERTICESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_DESTVERTICESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_VERTICESDATAMASK: "<<globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK: "<<globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_VERTICESPARTITIONMASK : "<<globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK: "<<globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_STATSDRAM: "<<globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM: "<<globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_EDGESSTATSDRAM: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_KVDRAM: "<<globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM: "<<globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_KVDRAMWORKSPACE: "<<globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE: "<<globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSET_OTHERINFOS: "<<globalparams.globalparamsE.BASEOFFSETKVS_OTHERINFOS * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_OTHERINFOS: "<<globalparams.globalparamsE.BASEOFFSETKVS_OTHERINFOS<<"]"<<endl;

	cout<<"[globalparams.globalparamsE.SIZE_MESSAGESDRAM: "<<globalparams.globalparamsE.SIZE_MESSAGESDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_EDGES: "<<globalparams.globalparamsE.SIZE_EDGES<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_VERTEXPTRS: "<<globalparams.globalparamsE.SIZE_VERTEXPTRS<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_SRCVERTICESDATA: "<<globalparams.globalparamsE.SIZE_SRCVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsE.SIZE_DESTVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_ACTIVEVERTICES: "<<globalparams.globalparamsE.SIZE_ACTIVEVERTICES<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_VERTICESDATAMASK: "<<globalparams.globalparamsE.SIZE_VERTICESDATAMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK: "<<globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_KVSTATSDRAM: "<<globalparams.globalparamsE.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_EDGESSTATSDRAM: "<<globalparams.globalparamsE.SIZE_EDGESSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_KVDRAM: "<<globalparams.globalparamsE.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE: "<<globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsE.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: PADDEDKVSOURCEDRAMSZ (keyvalues): "<<PADDEDKVSOURCEDRAMSZ<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: PADDEDKVSOURCEDRAMSZ (bytes): "<<(PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t))<<" bytes"<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid workload space(not including temp space) (keyvalues): "<<(globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid workload space(not including temp space) (keyvalues): "<<(((unsigned long)globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE) * sizeof(keyvalue_t))<<" bytes. "<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid PADDEDKVSOURCEDRAMSZ (keyvalues): "<<((globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid PADDEDKVSOURCEDRAMSZ (bytes): "<<((((unsigned long)globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (HBM max="<<(unsigned int)(KVSOURCEDRAMSZ * 8)<<" bytes)"<< RESET <<std::endl;					

	if(((((unsigned long)globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + (unsigned long)globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t)) > (KVSOURCEDRAMSZ * 8)){ cout<<"ERROR: dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadgraph::loadmessages(A)", (globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE) + globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE, PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	utilityobj->checkoutofbounds("loadgraph::loadmessages(A)", (globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE) + globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE, PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	utilityobj->checkoutofbounds("loadgraph::loadmessages(B)", (globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE) + globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE, PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	#endif

	createmessages(
			vdram, // uint512_vec_dt * kvstats,
			container->srcvoffset[0], // unsigned int srcvoffset,
			container->srcvsize[0], // unsigned int srcvsize,
			container->edgessize[0], // unsigned int edgessize,
			container->destvoffset[0], // unsigned int destvoffset,
			container->actvvsize[0], // unsigned int actvvsize,
			GraphIter, // unsigned int GraphIter,
			GraphAlgo, // unsigned int GraphAlgo,
			container->runsize[0],
			// globalparams.globalparamsK
			globalparams.globalparamsV // NEWCHANGE.
			); // unsigned int runsize
	// cout<<"loadgraph (123): GraphIter: "<<GraphIter<<", vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key: "<<vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key<<endl;
	// exit(EXIT_SUCCESS); //
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		createmessages(
			edges[i], // uint512_vec_dt * kvstats,
			0, // unsigned int srcvoffset,
			0, // unsigned int srcvsize,
			container->edgessize[i], // unsigned int edgessize,
			0, // unsigned int destvoffset,
			0, // unsigned int actvvsize,
			GraphIter, // unsigned int GraphIter,
			GraphAlgo, // unsigned int GraphAlgo,
			0,
			globalparams.globalparamsE); // unsigned int runsize
		createmessages(
			kvbuffer[i], // uint512_vec_dt * kvstats,
			container->srcvoffset[i], // unsigned int srcvoffset,
			container->srcvsize[i], // unsigned int srcvsize,
			0, // container->edgessize[i], // unsigned int edgessize,
			container->destvoffset[i], // unsigned int destvoffset,
			container->actvvsize[i], // unsigned int actvvsize,
			GraphIter, // unsigned int GraphIter,
			GraphAlgo, // unsigned int GraphAlgo,
			container->runsize[i],
			globalparams.globalparamsK); // unsigned int runsize
		#else 
		createmessages(
			kvbuffer[i], // uint512_vec_dt * kvstats,
			container->srcvoffset[i], // unsigned int srcvoffset,
			container->srcvsize[i], // unsigned int srcvsize,
			container->edgessize[i], // unsigned int edgessize,
			container->destvoffset[i], // unsigned int destvoffset,
			container->actvvsize[i], // unsigned int actvvsize,
			GraphIter, // unsigned int GraphIter,
			GraphAlgo, // unsigned int GraphAlgo,
			container->runsize[i],
			globalparams.globalparamsK); // unsigned int runsize
		#endif 
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"loadmessages:: running Acts... size: "<<container->edgessize[i]<<endl; 
		#endif 
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	unsigned int totalsize = 0;
	cout<<"loadmessages:: loading Acts... sizes: ["; 
	cout<<"["; for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ totalsize += container->runsize[i]; cout<<container->runsize[i]; if(i<NUMSUBCPUTHREADS-1){ cout<<", "; }} cout<<"]";
	cout<<"]"<<endl;
	cout<<"loadmessages:: total sizes: "<<totalsize<<endl; 
	#endif
	return globalparams;
}
globalparams_t loadgraph::createmessages(
			uint512_vec_dt * kvbuffer,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int actvvsize,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			globalparams_t globalparams){

	cout<<"--- loadgraph::createmessages: ACTIVE_KVSTATSDRAMSZ: "<<ACTIVE_KVSTATSDRAMSZ<<", globalparams.SIZE_KVSTATSDRAM: "<<globalparams.SIZE_KVSTATSDRAM<<endl;
	 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key = ON;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key = ON;//ON,OFF // CRITICAL REMOVEME.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = ON;//ON,OFF  // CRITICAL REMOVEME. 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = OFF;//ON,OFF // CRITICAL REMOVEME.  
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASK].data[0].key = ON;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASKP].data[0].key = ON;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].data[0].key  = globalparams.BASEOFFSETKVS_MESSAGESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key = globalparams.BASEOFFSETKVS_EDGESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key = globalparams.BASEOFFSETKVS_VERTEXPTR;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA].data[0].key = globalparams.BASEOFFSETKVS_SRCVERTICESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key = globalparams.BASEOFFSETKVS_DESTVERTICESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key = globalparams.BASEOFFSETKVS_ACTIVEVERTICES;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key = globalparams.BASEOFFSETKVS_VERTICESDATAMASK;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key = globalparams.BASEOFFSETKVS_STATSDRAM;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESSTATSDRAM].data[0].key = globalparams.BASEOFFSETKVS_EDGESSTATSDRAM;//
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key = globalparams.BASEOFFSETKVS_KVDRAM;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_OTHERINFOS].data[0].key = globalparams.BASEOFFSETKVS_OTHERINFOS;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].data[0].key = MESSAGESDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].data[0].key = edgessize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].data[0].key = srcvsize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_SRCVERTICESDATA].data[0].key = VERTICESDATASZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_DESTVERTICESDATA].data[0].key = VERTICESDATASZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].data[0].key = actvvsize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].data[0].key = globalparams.SIZE_VERTICESDATAMASK; // VERTICESDATAMASKSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESPARTITIONMASK].data[0].key = globalparams.SIZE_VERTICESPARTITIONMASK;// VERTICESDATAMASKSZ; // CRITICAL FIXME.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].data[0].key = globalparams.SIZE_KVSTATSDRAM; // kvstatssz; // NEWCHANGE
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGESSTATSDRAM].data[0].key = EDGESSTATSDRAMSZ; //64; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].data[0].key = globalparams.SIZE_KVDRAM; // KVDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key = globalparams.SIZE_KVDRAMWORKSPACE; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_OTHERINFOS].data[0].key = globalparams.SIZE_OTHERINFOS; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_REDUCE].data[0].key = REDUCESZ; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_BATCHRANGE].data[0].key = BATCH_RANGE; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key = runsize; //

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_MESSAGESDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGES].data[0].key = NAp; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTEXPTRS].data[0].key = NAp; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_SRCVERTICESDATA].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_DESTVERTICESDATA].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_ACTIVEVERTICES].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATAMASK].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVSTATSDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGESSTATSDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAMWORKSPACE].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_REDUCE].data[0].key = REDUCESZ_POW; // APPLYVERTEXBUFFERSZ_POW; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_BATCHRANGE].data[0].key = BATCH_RANGE_POW; //
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].data[0].key = GraphAlgo;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].data[0].key = 1;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key = TREE_DEPTH;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key = TREE_DEPTH;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key = NUMLASTLEVELPARTITIONS; // NUMLASTLEVELPARTITIONS; // NEWCHANGE.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key = srcvoffset;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key = srcvsize;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key = destvoffset;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER].data[0].key = globalparams.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;//NUM_EDGECHUNKS_IN_A_BUFFER; // CRITICAL REMOVEME.

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURN_RETURNVALUES].data[0].key = MESSAGES_RETURN_RETURNVALUES;
	
	// cout<<"createmessages (123): GraphIter: "<<GraphIter<<", kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key: "<<kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key<<endl;
	// exit(EXIT_SUCCESS); //
	return globalparams;
}

void loadgraph::setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], unsigned int evalid){
	evalparamsobj->loadevalparams(evalid, vdram);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		evalparamsobj->loadevalparams(evalid, kvbuffer[i]);
	}
	return;
}

globalparams_t loadgraph::finishglobaparams(globalparams_t globalparams){
	// globalparams.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	// globalparams.SIZE_ACTIVEVERTICES = 0; 
	// globalparams.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	// globalparams.SIZE_VERTICESDATAMASK = 0; 
	// globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.SIZE_VERTICESDATAMASK / VECTOR_SIZE);
	// globalparams.SIZE_VERTICESPARTITIONMASK = 0; 
	globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + (globalparams.SIZE_VERTICESPARTITIONMASK / VECTOR_SIZE);
	globalparams.SIZE_KVSTATSDRAM = 0; 
	globalparams.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ;
	globalparams.SIZE_EDGESSTATSDRAM = 0; 
	globalparams.SIZE_KVDRAM = 0; 
	globalparams.SIZE_KVDRAMWORKSPACE = globalparams.SIZE_KVDRAM;
	globalparams.SIZE_OTHERINFOS = 0;
	globalparams.BASEOFFSETKVS_KVDRAM = globalparams.BASEOFFSETKVS_EDGESSTATSDRAM + EDGESSTATSDRAMSZ;
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.BASEOFFSETKVS_KVDRAM + (globalparams.SIZE_KVDRAM / VECTOR_SIZE); 
	globalparams.BASEOFFSETKVS_OTHERINFOS = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE);
	return globalparams;
}