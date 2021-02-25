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
#include "../../kernels/kernel.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "loadgraph.h"
using namespace std;

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

globalparams_t loadgraph::loadedges_rowblockwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadedges_rowblockwise:: loading edges (rowwise)... "<<endl;
	#endif 
	globalparams.BASEOFFSETKVS_EDGESDATA = BASEOFFSET_MESSAGESDATA_KVS + MESSAGESDRAMSZ; 
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE; 
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"[globalparams.BASEOFFSET_EDGESDATA: "<<globalparams.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_EDGESDATA: "<<globalparams.BASEOFFSETKVS_EDGESDATA<<"]"<<endl;
	#endif 
	
	vptr_type * tempvptrs[NUMSUBCPUTHREADS]; //  = new vptr_type[KVDATA_RANGE];
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
				edges[subthread][2*_BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = edgedatabuffer[index2].srcvid; // INVALIDDATA, 777
				counts[subthread] += 1;
				index1 += 1;
				continue;
			}
			
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("loadgraph::loadedges_rowblockwise 23. packingfactor*_BASEOFFSET_EDGESDATA + counts[subthread]", packingfactor*_BASEOFFSET_EDGESDATA + counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor, packingfactor*_BASEOFFSET_EDGESDATA, counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor);				
			#endif 
		
			edges[subthread][2*_BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = edgedatabuffer[index2].dstvid;
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
				edges[subthread][2*_BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = INVALIDDATA;
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
	globalparams.BASEOFFSETKVS_VERTEXPTR = globalparams.BASEOFFSETKVS_EDGESDATA + ((container->edgessize[0]/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	globalparams.SIZE_VERTEXPTRS = KVDATA_RANGE;
	globalparams.SIZE_EDGES = container->edgessize[0] + 1000; 
	unsigned int _BASEOFFSET_VERTEXPTR = globalparams.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE;
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"[globalparams.BASEOFFSET_VERTEXPTR: "<<globalparams.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_VERTEXPTR: "<<globalparams.BASEOFFSETKVS_VERTEXPTR<<"]"<<endl;
	#endif 
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int k=0; k<KVDATA_RANGE; k++){
			vptrs[i][2*_BASEOFFSET_VERTEXPTR + k] = tempvptrs[i][k]; 
		}
	}
	
	// >>> dummy writes
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		for(unsigned int k=counts[i]; k<counts[i] + (DRAMPADD/2); k++){
			edges[i][2*_BASEOFFSET_EDGESDATA + k].dstvid = UNUSEDDATA;
		}
	}
	
	// >>> dummy writes
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int k=vid; k<vid + (DRAMPADD/2); k++){
			vptrs[i][2*_BASEOFFSET_VERTEXPTR + k + 1].key = counts[i]; 
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
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadgraph::loadedges_rowblockwise.edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][2*_BASEOFFSET_EDGESDATA], 8); }
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges_rowblockwise.vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][2*_BASEOFFSET_VERTEXPTR], 8); }
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	unsigned int numvaliditems = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int k=0; k<counts[i]; k++){ 
			unsigned int value = edges[i][2*_BASEOFFSET_EDGESDATA + k].dstvid;
			if(value == INVALIDDATA || value == 777){ }
			else { numvaliditems += 1; }
		}
	}
	cout<<"loadgraph::loadedges_rowblockwise:: number of valid items: "<<numvaliditems<<endl;
	#endif
	
	unsigned int maxvptr = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		cout<<"container->edgessize["<<i<<"] (last vptr): "<<container->edgessize[i]<<endl;
		if(maxvptr < counts[i]){ maxvptr = counts[i]; }
	}
	cout<<"maxvptr: "<<maxvptr<<endl;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ tempvptrs[i] = NULL; }
	return globalparams;
}

globalparams_t loadgraph::loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer, vertex_t offset, vertex_t size, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadvertexdata:: loading vertex datas... "<<endl;
	#endif
	globalparams.BASEOFFSETKVS_VERTICESDATA = globalparams.BASEOFFSETKVS_VERTEXPTR + ((globalparams.SIZE_VERTEXPTRS/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	globalparams.SIZE_VERTICESDATA = KVDATA_RANGE;
	unsigned int _BASEOFFSET_VERTICESDATA = globalparams.BASEOFFSETKVS_VERTICESDATA * VECTOR_SIZE;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"[globalparams.BASEOFFSET_VERTICESDATA: "<<globalparams.BASEOFFSETKVS_VERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_VERTICESDATA: "<<globalparams.BASEOFFSETKVS_VERTICESDATA<<"]"<<endl;
	#endif 
	
	for(unsigned int i = 0; i < 1; i++){
		memcpy(&kvbuffer[_BASEOFFSET_VERTICESDATA], &vertexdatabuffer[offset], size * sizeof(value_t));
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadvertexdata", &kvbuffer[_BASEOFFSET_VERTICESDATA], 16);
		#endif
	}
	return globalparams;
}

void loadgraph::setrootvid(value_t * kvbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::setrootvid:: setting root vid(s)... "<<endl;
	#endif 
	unsigned int _BASEOFFSET_VERTICESDATA = globalparams.BASEOFFSETKVS_VERTICESDATA * VECTOR_SIZE; 
	
	for(unsigned int i = 0; i < activevertices.size(); i++){
		unsigned int vid = activevertices[i];
		kvbuffer[2*_BASEOFFSET_VERTICESDATA + vid] = 0;
	}
	return;
}

globalparams_t loadgraph::loadactvvertices(vector<vertex_t> &activevertices, keyy_t * kvbuffer, container_t * container, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadactvvertices:: loading active vertices... "<<endl;
	#endif
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.BASEOFFSETKVS_VERTICESDATA + ((globalparams.SIZE_VERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.SIZE_ACTIVEVERTICES = ACTIVEVERTICESSZ;
	unsigned int _BASEOFFSET_ACTIVEVERTICES = globalparams.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"[globalparams.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	#endif 
	return globalparams;
	
	// NOT IMPLEMENTED.
	for(unsigned int k=0; k<activevertices.size(); k++){
		kvbuffer[2*_BASEOFFSET_ACTIVEVERTICES + k] = activevertices[k];
	}
	container->actvvsize[0] = activevertices.size();
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printkeyvalues("loadgraph::loadactvvertices", (keyvalue_t *)&kvbuffer[2*_BASEOFFSET_ACTIVEVERTICES], utilityobj->hmin(16, activevertices.size()));
	#endif
	return globalparams;
}

void loadgraph::savevmasks(bool_type enable, uint512_vec_dt * kvbuffer, keyvalue_vec_bittype vmask[NUM_PARTITIONS][REDUCEBUFFERSZ], batch_type offset_kvs, buffer_type size_kvs){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::savevmasks:: saving vmask... "<<endl;
	#endif
	unsigned int bitsbuffer[REDUCEBUFFERSZ];
	keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	
	for (buffer_type i=0; i<REDUCEBUFFERSZ; i++){
		for(unsigned int k=0; k<NUM_PARTITIONS; k++){
			utilityobj->WRITETO_UINT(&bitsbuffer[i], 2*k, 1, vmask[k][i].key);
			utilityobj->WRITETO_UINT(&bitsbuffer[i], 2*k+1, 1, vmask[k][i].value);
		}
	}
	
	unsigned int index = 0;
	for (buffer_type i=0; i<VMASKBUFFERSZ_KVS; i++){
		for (vector_type v=0; v<VECTOR_SIZE; v++){
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("loadgraph::savevmasks_h 1", index, REDUCEBUFFERSZ, NAp, NAp, NAp);
			utilityobj->checkoutofbounds("loadgraph::savevmasks_h 1", i, VMASKBUFFERSZ_KVS, NAp, NAp, NAp);
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
	utilityobj->printkeyvalues("savevmasks.vmask", vmask, NUM_PARTITIONS, 4);
	utilityobj->printkeyvalues("savevmasks.tempbuffer", tempbuffer, 8, 4);
	utilityobj->printkeyvalues("savevmasks.kvbuffer[0]", (keyvalue_t *)&kvbuffer[offset_kvs], 4);
	#endif 
	// exit(EXIT_SUCCESS);
	return;
}
globalparams_t loadgraph::generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], globalparams_t globalparams){ 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::generatevmaskdata:: generating vmask... "<<endl;
	#endif
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.SIZE_VERTICESDATAMASK = VERTICESDATAMASKSZ;
	unsigned int _BASEOFFSET_VERTICESDATAMASK_KVS = globalparams.BASEOFFSETKVS_VERTICESDATAMASK;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"[globalparams.BASEOFFSET_VERTICESDATAMASK: "<<globalparams.BASEOFFSETKVS_VERTICESDATAMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_VERTICESDATAMASK: "<<globalparams.BASEOFFSETKVS_VERTICESDATAMASK<<"]"<<endl;
	#endif
	
	keyvalue_vec_bittype vmask[NUM_PARTITIONS][REDUCEBUFFERSZ];
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<VERTICESDATAMASKSZ_KVS; k++){ 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				#ifdef _DEBUGMODE_CHECKS3
				utilityobj->checkoutofbounds("loadgraph::generatevmaskdata 2._BASEOFFSET_VERTICESDATAMASK_KVS", _BASEOFFSET_VERTICESDATAMASK_KVS + k, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
				#endif
				kvbuffer[i][_BASEOFFSET_VERTICESDATAMASK_KVS + k].data[v].key = 0;
				kvbuffer[i][_BASEOFFSET_VERTICESDATAMASK_KVS + k].data[v].value = 0;
			}
		}
	}
	
	unsigned int vmaskoffset_kvs = 0;
	for(unsigned int offset=0; offset<BATCH_RANGE; offset+=REDUCESZ * NUM_PARTITIONS){
		for(unsigned int i=0; i<NUM_PARTITIONS; i++){
			unsigned int * V = (unsigned int *)vmask[i]; 
			for(unsigned int k=0; k<REDUCESZ; k++){
				unsigned int vid = offset + (i*REDUCESZ) + k;
				if(vid==1){ V[k] = 1; }
				else{ V[k] = 0; }
			}
		}
		
		for(unsigned int i=0; i<1; i++){ savevmasks(ON, kvbuffer[i], vmask, _BASEOFFSET_VERTICESDATAMASK_KVS + vmaskoffset_kvs, VMASKBUFFERSZ_KVS); }
		vmaskoffset_kvs += VMASKBUFFERSZ_KVS;
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printkeyvalues("generatevmaskdata.vmask", vmask, NUM_PARTITIONS, 4);
	utilityobj->printkeyvalues("generatevmaskdata.kvbuffer[0]", (keyvalue_t *)&kvbuffer[0][_BASEOFFSET_VERTICESDATAMASK_KVS], 4);
	cout<<"loadgraph::generatevmaskdata:: end. "<<endl;
	#endif
	return globalparams;
}

globalparams_t loadgraph::loadoffsetmarkers(edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], container_t * container, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::loadoffsetmarkers:: loading offset markers... "<<endl;
	#endif 
	globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.SIZE_VERTICESDATAMASK / VECTOR_SIZE);
	globalparams.SIZE_KVSTATSDRAM = KVSTATSDRAMSZ;
	
	globalparams.BASEOFFSETKVS_KVDRAM = globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ;
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.BASEOFFSETKVS_KVDRAM + KVDRAMSZ_KVS;

	unsigned int _BASEOFFSET_STATSDRAM = globalparams.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE; 
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"[globalparams.BASEOFFSET_STATSDRAM: "<<globalparams.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_STATSDRAM: "<<globalparams.BASEOFFSETKVS_STATSDRAM<<"]"<<endl;
	#endif 
	
	unsigned int totalnumpartitions = 0;
	for(unsigned int k=0; k<=TREE_DEPTH; k++){ totalnumpartitions += (unsigned int)pow(NUM_PARTITIONS, k); } 
	unsigned int maxdramsz = 0;
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<KVSTATSSZ; k++){
			statsptrVec[k].data[0].key = 0;
			statsptrVec[k].data[0].value = 0;
		}
	}
	
	keyvalue_t * tempstats = new keyvalue_t[KVSTATSDRAMSZ];
	for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){ tempstats[k].key = 0; tempstats[k].value = 0; }
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadgraph::loadoffsetmarkers:: edgessize["<<i<<"]: "<<container->edgessize[i]<<endl;
		#endif 
		edge_type * edgesptr = (edge_type *)&edges[i][2*_BASEOFFSET_EDGESDATA];
		keyvalue_t * statsptr = (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){ tempstats[k].key = 0; tempstats[k].value = 0; }
		
		for(unsigned int k=0; k<container->edgessize[i]; k++){
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(k % 100000 == 0){ cout<<"### loadgraph::loadoffsetmarkers:: k: "<<k<<endl; }
			#endif
			
			keyvalue_t keyvalue;
			keyvalue.key = edgesptr[k].dstvid;
			keyvalue.value = 0;
			
			if(keyvalue.key == INVALIDDATA){ keyvalue.key = 0; } // >>> for loadedges_rowblockwise
			
			for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
				
				unsigned int offset = 0;
				for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); } 
				
				unsigned int partitionCLOP = getglobalpartition(keyvalue, 0, BATCH_RANGE_POW, CLOP);
				
				#ifdef _DEBUGMODE_CHECKS
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.partitionCLOP.1", partitionCLOP, pow(NUM_PARTITIONS, TREE_DEPTH), keyvalue.key, container->edgessize[i], NAp);
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.partitionCLOP.2", offset + partitionCLOP, KVSTATSDRAMSZ, keyvalue.key, container->edgessize[i], NAp);
				#endif
				tempstats[offset + partitionCLOP].value += 1;
			}
		}
		
		for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
			unsigned int offset = 0;
			for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); }
			
			utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.offset", offset, KVSTATSDRAMSZ, NAp, NAp, NAp);
			utilityobj->getmarkerpositions((keyvalue_t *)&tempstats[offset], (unsigned int)pow(NUM_PARTITIONS, CLOP));
		}
		
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<KVSTATSSZ; k++){
			statsptrVec[k].data[0].key = tempstats[k].key;
			statsptrVec[k].data[0].value = 0;
			// statsptrVec[k].data[0].value = tempstats[k].value;
		}
		
		unsigned int totalnumpb4llop = 0;
		for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpb4llop += (unsigned int)pow(NUM_PARTITIONS, k); } 
		if(maxdramsz < (tempstats[totalnumpb4llop-1].key + tempstats[totalnumpb4llop-1].value)){ maxdramsz = tempstats[totalnumpb4llop-1].key + tempstats[totalnumpb4llop-1].value; }
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats [after]", tempstats, totalnumpb4llop);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing statsptr [after]", (keyvalue_t *)statsptrVec, totalnumpb4llop * VECTOR_SIZE, VECTOR_SIZE); // totalnumpartitions
		cout<<"tempstats["<<totalnumpb4llop-1<<"].key: "<<tempstats[totalnumpb4llop-1].key<<", tempstats["<<totalnumpb4llop-1<<"].value: "<<tempstats[totalnumpb4llop-1].value<<endl;
		#endif
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats [after]", tempstats, totalnumpartitions);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing statsptr [after]", (keyvalue_t *)statsptrVec, totalnumpartitions * VECTOR_SIZE, VECTOR_SIZE);
		#endif
	}
		
	globalparams.SIZE_KVDRAM = maxdramsz;
	globalparams.SIZE_KVDRAMWORKSPACE = maxdramsz;
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<1; i++){
		utilityobj->printkeyvalues("loadoffsetmarkers: printing edges[i][_BASEOFFSET_EDGESDATA]", (keyvalue_t *)&edges[i][baseoffset_edgedata], 16);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing edges[i][_BASEOFFSET_EDGESDATA][last]", (keyvalue_t *)&edges[i][baseoffset_edgedata+container->runsize[i]-32], 16);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][_BASEOFFSET_STATSDRAM]", (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM], (1+16) * VECTOR_SIZE, VECTOR_SIZE);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][_BASEOFFSET_STATSDRAM]", (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM], totalnumpartitions * VECTOR_SIZE, VECTOR_SIZE);
	}
	#endif
	return globalparams;
}

globalparams_t loadgraph::loadmessages(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo, globalparams_t globalparams){	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printcontainer(container); 
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"[globalparams.BASEOFFSET_MESSAGESDATA: "<<globalparams.BASEOFFSETKVS_MESSAGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_MESSAGESDATA: "<<globalparams.BASEOFFSETKVS_MESSAGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.BASEOFFSET_EDGESDATA: "<<globalparams.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_EDGESDATA: "<<globalparams.BASEOFFSETKVS_EDGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.BASEOFFSET_VERTEXPTR: "<<globalparams.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_VERTEXPTR: "<<globalparams.BASEOFFSETKVS_VERTEXPTR<<"]"<<endl;
	
	cout<<"[globalparams.BASEOFFSET_VERTICESDATA: "<<globalparams.BASEOFFSETKVS_VERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_VERTICESDATA: "<<globalparams.BASEOFFSETKVS_VERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	
	cout<<"[globalparams.BASEOFFSET_VERTICESDATAMASK: "<<globalparams.BASEOFFSETKVS_VERTICESDATAMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_VERTICESDATAMASK: "<<globalparams.BASEOFFSETKVS_VERTICESDATAMASK<<"]"<<endl;
	
	cout<<"[globalparams.BASEOFFSET_STATSDRAM: "<<globalparams.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_STATSDRAM: "<<globalparams.BASEOFFSETKVS_STATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.BASEOFFSET_KVDRAM: "<<globalparams.BASEOFFSETKVS_KVDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_KVDRAM: "<<globalparams.BASEOFFSETKVS_KVDRAM<<"]"<<endl;
	
	cout<<"[globalparams.BASEOFFSET_KVDRAMWORKSPACE: "<<globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE: "<<globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE<<"]"<<endl;

	cout<<"[globalparams.SIZE_MESSAGESDRAM: "<<globalparams.SIZE_MESSAGESDRAM<<"]"<<endl;
	cout<<"[globalparams.SIZE_EDGES: "<<globalparams.SIZE_EDGES<<"]"<<endl;
	cout<<"[globalparams.SIZE_VERTEXPTRS: "<<globalparams.SIZE_VERTEXPTRS<<"]"<<endl;
	cout<<"[globalparams.SIZE_VERTICESDATA: "<<globalparams.SIZE_VERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.SIZE_ACTIVEVERTICES: "<<globalparams.SIZE_ACTIVEVERTICES<<"]"<<endl;
	cout<<"[globalparams.SIZE_VERTICESDATAMASK: "<<globalparams.SIZE_VERTICESDATAMASK<<"]"<<endl;
	cout<<"[globalparams.SIZE_KVSTATSDRAM: "<<globalparams.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.SIZE_KVDRAM: "<<globalparams.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.SIZE_KVDRAMWORKSPACE: "<<globalparams.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid PADDEDKVSOURCEDRAMSZ (keyvalues): "<<((globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.SIZE_KVDRAMWORKSPACE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid PADDEDKVSOURCEDRAMSZ (bytes): "<<(((globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (max="<<(256 * 1024 * 1024)<<" bytes)"<< RESET <<std::endl;
	#endif 
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadgraph::loadmessages", (globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + KVDRAMWORKSPACESZ, PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	#endif
	
	unsigned int totalkvstatssize = 0;
	for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
		for(unsigned int k=0; k<CLOP; k++){ totalkvstatssize += (1 << (NUM_PARTITIONS_POW * k)); }
	}

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
			globalparams); // unsigned int runsize
			
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
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
			globalparams); // unsigned int runsize
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
	unsigned int kvstatssz = 0;
	for(unsigned int CLOP=0; CLOP<=TREE_DEPTH; CLOP++){ kvstatssz += (1 << (NUM_PARTITIONS_POW * CLOP)); }
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key = ON;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key = ON; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = ON;  
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = ON;  

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].data[0].key  = globalparams.BASEOFFSETKVS_MESSAGESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key = globalparams.BASEOFFSETKVS_EDGESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key = globalparams.BASEOFFSETKVS_VERTEXPTR;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key = globalparams.BASEOFFSETKVS_VERTICESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key = globalparams.BASEOFFSETKVS_ACTIVEVERTICES;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key = globalparams.BASEOFFSETKVS_VERTICESDATAMASK;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key = globalparams.BASEOFFSETKVS_STATSDRAM;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key = globalparams.BASEOFFSETKVS_KVDRAM;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].data[0].key = MESSAGESDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].data[0].key = edgessize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].data[0].key = srcvsize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATA].data[0].key = VERTICESDATASZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].data[0].key = actvvsize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].data[0].key = VERTICESDATAMASKSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].data[0].key = kvstatssz;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].data[0].key = KVDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key = KVSTATSDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_REDUCE].data[0].key = REDUCESZ; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_BATCHRANGE].data[0].key = BATCH_RANGE; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key = runsize; //

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_MESSAGESDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGES].data[0].key = NAp; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTEXPTRS].data[0].key = NAp; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATA].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_ACTIVEVERTICES].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATAMASK].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVSTATSDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAMWORKSPACE].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_REDUCE].data[0].key = APPLYVERTEXBUFFERSZ_POW; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_BATCHRANGE].data[0].key = BATCH_RANGE_POW; //
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].data[0].key = GraphAlgo;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].data[0].key = 1;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key = TREE_DEPTH;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key = TREE_DEPTH;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key = NUMLASTLEVELPARTITIONS; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key = srcvoffset;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key = srcvsize;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key = destvoffset;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURN_RETURNVALUES].data[0].key = MESSAGES_RETURN_RETURNVALUES;
	return globalparams;
}

void loadgraph::setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], unsigned int evalid){
	evalparamsobj->loadevalparams(evalid, vdram);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		evalparamsobj->loadevalparams(evalid, kvbuffer[i]);
	}
	return;
}






