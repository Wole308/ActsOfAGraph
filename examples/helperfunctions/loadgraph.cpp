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
#include "loadgraph.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask, active partition mask}
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

unsigned int loadgraph::GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int loadgraph::READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
void loadgraph::WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
	/** WRITETO_UINT(&data, 28, 4, 1) => 010000000000000000000000000000
	WRITETO_UINT(&data, 28, 4, 2) => 100000000000000000000000000000
	WRITETO_UINT(&data, 28, 4, 3) => 110000000000000000000000000000 */
	
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~GETMASK_UINT((index), (size)));
	unsigned int C = ((tempdata) & (B));
	unsigned int D = (C) | A;
	*data = D;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"WRITETO_ULONG. index: "<<index<<", size: "<<size<<", value: "<<value<<endl;
	cout<<"WRITETO_ULONG. tempdata"<<endl; actsutilityobj->ULONGTOBINARY(tempdata);
	cout<<"WRITETO_ULONG. A"<<endl; actsutilityobj->ULONGTOBINARY(A);
	cout<<"WRITETO_ULONG. B (~mask)"<<endl; actsutilityobj->ULONGTOBINARY(B);
	cout<<"WRITETO_ULONG. C"<<endl; actsutilityobj->ULONGTOBINARY(C);
	cout<<"WRITETO_ULONG. D (result)"<<endl; actsutilityobj->ULONGTOBINARY(D);
	#endif
	return; 
}

globalparams_t loadgraph::loadvertexdata(unsigned int Algo, value_t * vertexdatabuffer, keyvalue_t * kvbuffer, vertex_t vbufferoffset, vertex_t size, globalparams_t globalparams, unsigned int edgesorkv, unsigned int srcordest){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::loadvertexdata:: loading vertex datas... vbufferoffset: "<<vbufferoffset<<", size: "<<size<<endl;
	#endif
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadvertexdata.BASEOFFSETKVS_DESTVERTICESDATA", globalparams.BASEOFFSETKVS_DESTVERTICESDATA, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);				
	#endif
	
	value_t * KV = (value_t *)&kvbuffer[vbufferoffset];
	for(unsigned int k=0; k<size; k++){
		KV[k] = algorithmobj->vertex_initdata(Algo, k); 
	} 
	return globalparams;
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
	utilityobj->checkoutofbounds("loadactvvertices.BASEOFFSETKVS_ACTIVEVERTICES", globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	#endif 
	return globalparams;
}

unsigned int loadgraph::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth){
	unsigned int partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadgraph::getglobalpartition", partition, (1 << (NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}
globalparams_TWOt loadgraph::loadoffsetmarkers(edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadoffsetmarkers:: loading offset markers... "<<endl;
	#endif
	
	// CRITICAL NEWCHANGE. since kvdram and edges dram are now merged into one
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK;
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = ACTIVE_KVSTATSDRAMSZ *
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		NUM_EDGE_BANKS;
		#else 
		1;	
		#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK + globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK;
	globalparams.globalparamsE.SIZE_KVSTATSDRAM = ACTIVE_KVSTATSDRAMSZ; // * NUM_EDGE_BANKS; // NEWCHANGE.
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
	globalparams.globalparamsE.SIZE_EDGESSTATSDRAM = EDGESSTATSDRAMSZ;
	#endif 
	
	// calculate best-fit value for NUM_EDGECHUNKS_IN_A_BUFFER
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	// NB: Formular: 3V + 2E/N + 2E/N = (256MB/4)
	#ifdef CONFIG_UNIFYSRCV
	unsigned int num_vertices_per_channel = 0;
	unsigned int Vspace = globalparams.globalparamsK.SIZE_DESTVERTICESDATA + globalparams.globalparamsK.SIZE_VERTEXPTRS;
	#else 
	unsigned int num_vertices_per_channel = KVDATA_RANGE;
	unsigned int Vspace = 3 * num_vertices_per_channel;
	#endif 
	unsigned int num_edges_per_channel = globalparams.globalparamsE.SIZE_EDGES;
	unsigned int A = 4 * num_edges_per_channel; // capacity (uint32_t) for kv datas
	unsigned int B = (KVDRAMWORKCAPACITY_BYTES / 4) - Vspace;
	if((KVDRAMWORKCAPACITY_BYTES / 4) < Vspace){ cout<<"loadoffsetmarkers::ERROR: (KVDRAMWORKCAPACITY_BYTES / 4)("<<(KVDRAMWORKCAPACITY_BYTES / 4)<<") < (3 * num_vertices_per_channel)("<<(3 * num_vertices_per_channel)<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	unsigned int num_edgechunks_in_a_buffer = (A + (B-1)) / B; // 3V + 2E/N + 2E/N = (256MB/4)
	cout<<"loadoffsetmarkers:: A: "<<A<<", B: "<<B<<", num_edgechunks_in_a_buffer: "<<num_edgechunks_in_a_buffer<<", globalparams.globalparamsE.SIZE_EDGES: "<<globalparams.globalparamsE.SIZE_EDGES<<endl;
	#else 
	unsigned int num_edgechunks_in_a_buffer = 1;	
	#endif
	cout<<"loadoffsetmarkers:: globalparams.globalparamsK.SIZE_VERTEXPTRS: "<<globalparams.globalparamsK.SIZE_VERTEXPTRS<<endl;
	cout<<"loadoffsetmarkers:: globalparams.globalparamsK.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsK.SIZE_DESTVERTICESDATA<<endl;
	// exit(EXIT_SUCCESS);
	
	globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = num_edgechunks_in_a_buffer;
	globalparams.globalparamsE.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;

	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#else 
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#endif
	unsigned int _KVSTATSDRAMSZ = globalparams.globalparamsK.SIZE_KVSTATSDRAM;
	
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
	unsigned int _BASEOFFSET_EDGESSTATSDRAM2 = (BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM) * VECTOR_SIZE; 
	unsigned int _BASEOFFSET_VERTEXPTR = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE;
	unsigned int _NUM_EDGECHUNKS_IN_A_BUFFER = globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_STATSDRAM", globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif
	
	unsigned int totalnumpartitions = 0;
	for(unsigned int k=0; k<=TREE_DEPTH; k++){ totalnumpartitions += (unsigned int)pow(NUM_PARTITIONS, k); } 
	unsigned int maxdramsz = 0;

	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<_KVSTATSDRAMSZ; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				statsptrVec[k].data[v].key = 0;
				statsptrVec[k].data[v].value = 0;
			}
		}
	}
	
	keyvalue_t **tempstats = new keyvalue_t*[_NUM_EDGECHUNKS_IN_A_BUFFER];
	for(unsigned int i = 0; i < _NUM_EDGECHUNKS_IN_A_BUFFER; ++i){ tempstats[i] = new keyvalue_t[_KVSTATSDRAMSZ]; }
	for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ for(unsigned int k=0; k<_KVSTATSDRAMSZ; k++){ tempstats[u][k].key = 0; tempstats[u][k].value = 0; }}
	
	unsigned int src1 = 0;
	unsigned int src2 = 0;
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(i==0 || i==NUMCOMPUTEUNITS_SLR2 || i==NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1){ } else { continue; }
		#endif 
		
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadgraph::loadoffsetmarkers:: edgessize["<<i<<"]: "<<container->edgessize[i]<<endl;
		#endif 
		if(i >= NUM_PEs){ continue; }
		
		keyvalue_t * statsptr = (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ for(unsigned int k=0; k<_KVSTATSDRAMSZ; k++){ tempstats[u][k].key = 0; tempstats[u][k].value = 0; }}
		
		src1 = 0;
		src2 = 0;
		
		unsigned int srC=0;
		unsigned int SRC_CHKPT[(_NUM_EDGECHUNKS_IN_A_BUFFER+1)];
		unsigned int PARTITION_CHKPT[(_NUM_EDGECHUNKS_IN_A_BUFFER+1)];
		unsigned int num_edges_in_a_chunk = container->edgessize[i] / _NUM_EDGECHUNKS_IN_A_BUFFER;
		for(unsigned int k=0; k<_NUM_EDGECHUNKS_IN_A_BUFFER; k++){
			unsigned int index = k * (container->edgessize[i] / _NUM_EDGECHUNKS_IN_A_BUFFER);
			// for(unsigned int t=0; t<16; t++){ if(edges_temp[i][index+t].status != EDGESTATUS_BITMAP){ srC = edges_temp[i][index+t].srcvid; break; }}
			bool isfound=false; for(unsigned int t=0; t<64; t++){ if(edges_temp[i][index+t].status == EDGESTATUS_VALIDEDGE){ srC = edges_temp[i][index+t].srcvid; isfound=true; break; }}
			if(isfound==false){ cout<<"loadoffsetmarkers: ERROR 65. isfound==false. EXITING..."<<endl; exit(EXIT_FAILURE); }
			srC = (srC / PROCESSPARTITIONSZ) * PROCESSPARTITIONSZ; // round down
			SRC_CHKPT[k] = srC;
			PARTITION_CHKPT[k] = srC / PROCESSPARTITIONSZ;
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"loadoffsetmarkers: k: "<<k<<", index: "<<index<<", srC: "<<srC<<", SRC_CHKPT["<<k<<"]: "<<SRC_CHKPT[k]<<", PARTITION_CHKPT["<<k<<"]: "<<PARTITION_CHKPT[k]<<", num_edges_in_a_chunk: "<<num_edges_in_a_chunk<<", _NUM_EDGECHUNKS_IN_A_BUFFER: "<<_NUM_EDGECHUNKS_IN_A_BUFFER<<endl; 					
			#endif
		}
		SRC_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = KVDATA_RANGE-1;
		PARTITION_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = KVDATA_RANGE / PROCESSPARTITIONSZ;
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadoffsetmarkers: index: LAST. srC: "<<srC<<", SRC_CHKPT["<<_NUM_EDGECHUNKS_IN_A_BUFFER<<"]: "<<SRC_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER]<<", PARTITION_CHKPT["<<_NUM_EDGECHUNKS_IN_A_BUFFER<<"]: "<<PARTITION_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER]<<", num_edges_in_a_chunk: "<<num_edges_in_a_chunk<<", _NUM_EDGECHUNKS_IN_A_BUFFER: "<<_NUM_EDGECHUNKS_IN_A_BUFFER<<endl; 					
		#endif
		
		for(unsigned int k=0; k<container->edgessize[i]; k++){
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(k % 1000000 == 0){ cout<<"### loadgraph::loadoffsetmarkers:: k: "<<k<<endl; }
			#endif
			
			if(edges_temp[i][k].status != EDGESTATUS_VALIDEDGE){ continue; }
			
			keyvalue_t keyvalue; 
			keyvalue.key = edges_temp[i][k].dstvid;
			keyvalue.value = 0;
			
			src2 = edges_temp[i][k].srcvid;
			#ifdef _DEBUGMODE_CHECKS
			if(k % VECTOR2_SIZE == 0){ if(k%1000000 == 0){ cout<<"loadgraph::loadoffsetmarkers: src1: "<<src1<<", src2: "<<src2<<", k: "<<k<<", i: "<<i<<endl; }}
			#endif 
			
			for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
				unsigned int offset = 0;
				for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); } 
				
				unsigned int partitionCLOP = getglobalpartition(keyvalue, 0, BATCH_RANGE_POW, CLOP);
				
				#ifdef _DEBUGMODE_CHECKS3
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.partitionCLOP.1", partitionCLOP, pow(NUM_PARTITIONS, TREE_DEPTH), edges_temp[i][k].dstvid, edges_temp[i][k].status, container->edgessize[i]);
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.partitionCLOP.2", offset + partitionCLOP, _KVSTATSDRAMSZ, keyvalue.key, container->edgessize[i], NAp);
				#endif
				
				#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
				unsigned int p=0;
				for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ if(src2 >= SRC_CHKPT[u] && src2 < SRC_CHKPT[u+1]){ p = u; } }
				tempstats[p][offset + partitionCLOP].value += 1;
				#else 
				tempstats[0][offset + partitionCLOP].value += 1;	
				#endif 
			}
		}
		cout<<"### loadgraph::loadoffsetmarkers:: Finished collecting stats for PE: "<<i<<endl;
		
		for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
			unsigned int offset = 0;
			for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); }
			
			utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.offset", offset, _KVSTATSDRAMSZ, NAp, NAp, NAp);
			for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ 
				utilityobj->getmarkerpositions((keyvalue_t *)&tempstats[u][offset], (unsigned int)pow(NUM_PARTITIONS, CLOP)); 
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ 
			cout<<"loadoffsetmarkers: printing tempstats for u: "<<u<<endl; 
			utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats[u][----]", tempstats[u], 17); 
		}
		#endif 
		
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){
			for(unsigned int k=0; k<_KVSTATSDRAMSZ; k++){
				statsptrVec[k].data[u].key = tempstats[u][k].key;
				statsptrVec[k].data[u].value = 0;
				// statsptrVec[k].data[u].value = tempstats[u][k].value; // CRITICAL REMOVEME.
			}
		}
		
		uint512_vec_dt * edgesstatsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_EDGESSTATSDRAM];
		uint512_vec_dt * edgesstatsptrVec2 = (uint512_vec_dt *)&stats[i][_BASEOFFSET_EDGESSTATSDRAM2];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER+1; u++){
			edgesstatsptrVec[u].data[0].key = PARTITION_CHKPT[u];
			edgesstatsptrVec[u].data[0].value = 0;
			edgesstatsptrVec2[u].data[0].key = PARTITION_CHKPT[u];
			edgesstatsptrVec2[u].data[0].value = 0;
		}
		
		unsigned int totalnumpb4llop = 0;
		for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpb4llop += (unsigned int)pow(NUM_PARTITIONS, k); } 
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){
			if(maxdramsz < (tempstats[u][totalnumpb4llop-1].key + tempstats[u][totalnumpb4llop-1].value)){ maxdramsz = tempstats[u][totalnumpb4llop-1].key + tempstats[u][totalnumpb4llop-1].value; }
		}
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadoffsetmarkers:: maxdramsz: "<<maxdramsz<<", totalnumpb4llop: "<<totalnumpb4llop<<", TREE_DEPTH: "<<TREE_DEPTH<<endl;
		#endif 
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		if(i==0){
			for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){
				utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats [after]", tempstats[u], totalnumpb4llop);
				// utilityobj->printkeyvalues("loadoffsetmarkers: printing statsptrVec [after]", (keyvalue_t *)statsptrVec, totalnumpb4llop * VECTOR_SIZE, VECTOR_SIZE);
				// cout<<"tempstats["<<u<<"]["<<totalnumpb4llop-1<<"].key: "<<tempstats[u][totalnumpb4llop-1].key<<", tempstats["<<u<<"]["<<totalnumpb4llop-1<<"].value: "<<tempstats[u][totalnumpb4llop-1].value<<endl;
			}
		}
		#endif
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(i==0){ for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ cout<<"loadoffsetmarkers: u:"<<u<<endl; utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats[u] [after]", tempstats[u], 17); } }
		#endif
		// exit(EXIT_SUCCESS); ////
	}
	// exit(EXIT_SUCCESS); ////
	
	globalparams.globalparamsK.SIZE_KVDRAM = maxdramsz + 64; // CHECKME. FIXME.
	globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = globalparams.globalparamsK.SIZE_KVDRAM; // maxdramsz;
	globalparams.globalparamsK.SIZE_OTHERINFOS = 0; // 64 * VECTOR_SIZE;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.SIZE_KVDRAM = 0; 
	globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	globalparams.globalparamsE.SIZE_OTHERINFOS = 0;
	#endif  
	
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + globalparams.globalparamsK.SIZE_EDGESSTATSDRAM; 
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM + (globalparams.globalparamsK.SIZE_KVDRAM / VECTOR_SIZE); 
	globalparams.globalparamsK.BASEOFFSETKVS_OTHERINFOS = globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM + globalparams.globalparamsE.SIZE_EDGESSTATSDRAM; 
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM + (globalparams.globalparamsE.SIZE_KVDRAM / VECTOR_SIZE);
	globalparams.globalparamsE.BASEOFFSETKVS_OTHERINFOS = globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE); 
	#endif
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_KVDRAM", globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_KVDRAMWORKSPACE", globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	for(unsigned int i=0; i<1; i++){
		cout<<">>> loadoffsetmarkers: printing stats["<<i<<"]..."<<endl;
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
			}
		}
	}
	return;
}

globalparams_TWOt loadgraph::generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * vdram, globalparams_TWOt globalparams){ 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::generatevmaskdata:: generating vmask... "<<endl;
	#endif
	
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsK.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_VERTICESDATAMASK = 0;
	
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsK.SIZE_VERTICESDATAMASK / VECTOR2_SIZE);
	globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK = (NUMPROCESSEDGESPARTITIONS * 2); // '*2' is padding value. AUTOMATEME.
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsE.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_VERTICESDATAMASK = 0;
	
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsE.SIZE_VERTICESDATAMASK / VECTOR2_SIZE);
	globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK = 0;
	#endif 
	
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsV.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsV.SIZE_VERTICESDATAMASK = 0;
	
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsV.SIZE_VERTICESDATAMASK / VECTOR2_SIZE);
	globalparams.globalparamsV.SIZE_VERTICESPARTITIONMASK = (NUMPROCESSEDGESPARTITIONS * 2);
	
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
	
	vdram[_BASEOFFSETV_VERTICESPARTITIONMASK_KVS].data[0].key = 0x00000001;
	return globalparams;
}

void loadgraph::savevdata(uint512_vec_dt * kvdram, unsigned int buffer[VDATA_PACKINGSIZE][REDUCEPARTITIONSZ_KVS2], batch_type vbaseoffset_kvs, batch_type voffset_kvs){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::savemasks:: saving vdata: voffset_kvs: "<<voffset_kvs<<endl;
	#endif 
	for(unsigned int i=0; i<REDUCEPARTITIONSZ_KVS2; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram[vbaseoffset_kvs + voffset_kvs + i].data[v].key = buffer[2*v][i];
			kvdram[vbaseoffset_kvs + voffset_kvs + i].data[v].value = buffer[2*v+1][i];
		}
	}
	return;
}
void loadgraph::setrootvid(unsigned int Algo, uint512_vec_dt * vbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::setrootvid:: setting root vid(s)... "<<endl;
	#endif 
	
	unsigned int data = 0;
	
	#ifndef CONFIG_SKELETAL_ALGORITHM
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS 
	// {1st 16 is masks}{2nd 16 is vdatas}
	// utilityobj->WRITETO_UINT((unsigned int *)&data, OFFSETOF_VDATA, SIZEOF_VDATA, 0);	
	// utilityobj->WRITETO_UINT((unsigned int *)&data, OFFSETOF_VMASK, SIZEOF_VMASK, 1);
	utilityobj->WRITETO_UINT((unsigned int *)&data, BEGINOFFSETOF_VMASK, SIZEOF_VMASK, 1);
	#else 
	utilityobj->WRITETO_UINT((unsigned int *)&data, OFFSETOF_VDATA, SIZEOF_VDATA, 0);
	utilityobj->WRITETO_UINT((unsigned int *)&data, OFFSETOF_VMASK, SIZEOF_VMASK, 1);
	#endif 
	vbuffer[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + (1 * NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2)].data[0].key = data;
	cout<<"loadgraph::setrootvid: data: "<<data<<", vbuffer["<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + (1 * NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2)<<"].data[0].key: "<<vbuffer[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + (1 * NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2)].data[0].key<<endl;			
	#endif 
	return;
}

globalparams_TWOt loadgraph::loadmessages(uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo, globalparams_TWOt globalparams){	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printcontainer(container); 
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"[globalparams.globalparamsM.BASEOFFSET_MESSAGESDATA: "<<globalparams.globalparamsM.BASEOFFSETKVS_MESSAGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_MESSAGESDATA: "<<globalparams.globalparamsM.BASEOFFSETKVS_MESSAGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_EDGESDATA: "<<globalparams.globalparamsM.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_EDGESDATA: "<<globalparams.globalparamsM.BASEOFFSETKVS_EDGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_VERTEXPTR: "<<globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR: "<<globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_SRCVERTICESDATA: "<<globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA: "<<globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_DESTVERTICESDATA: "<<globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA: "<<globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_VERTICESDATAMASK: "<<globalparams.globalparamsM.BASEOFFSETKVS_VERTICESDATAMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_VERTICESDATAMASK: "<<globalparams.globalparamsM.BASEOFFSETKVS_VERTICESDATAMASK<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_VERTICESPARTITIONMASK : "<<globalparams.globalparamsM.BASEOFFSETKVS_VERTICESPARTITIONMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_VERTICESPARTITIONMASK: "<<globalparams.globalparamsM.BASEOFFSETKVS_VERTICESPARTITIONMASK<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_STATSDRAM: "<<globalparams.globalparamsM.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_STATSDRAM: "<<globalparams.globalparamsM.BASEOFFSETKVS_STATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_EDGESSTATSDRAM: "<<globalparams.globalparamsM.BASEOFFSETKVS_EDGESSTATSDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparams.globalparamsM.BASEOFFSETKVS_EDGESSTATSDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_KVDRAM: "<<globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM: "<<globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSET_KVDRAMWORKSPACE: "<<globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE: "<<globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE<<"]"<<endl;

	cout<<"[globalparams.globalparamsM.SIZE_MESSAGESDRAM: "<<globalparams.globalparamsM.SIZE_MESSAGESDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_EDGES: "<<globalparams.globalparamsM.SIZE_EDGES<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_VERTEXPTRS: "<<globalparams.globalparamsM.SIZE_VERTEXPTRS<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_SRCVERTICESDATA: "<<globalparams.globalparamsM.SIZE_SRCVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsM.SIZE_DESTVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_ACTIVEVERTICES: "<<globalparams.globalparamsM.SIZE_ACTIVEVERTICES<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_VERTICESDATAMASK: "<<globalparams.globalparamsM.SIZE_VERTICESDATAMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_VERTICESPARTITIONMASK: "<<globalparams.globalparamsM.SIZE_VERTICESPARTITIONMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_KVSTATSDRAM: "<<globalparams.globalparamsM.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_EDGESSTATSDRAM: "<<globalparams.globalparamsM.SIZE_EDGESSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_KVDRAM: "<<globalparams.globalparamsM.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE: "<<globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_OTHERINFOS: "<<globalparams.globalparamsM.SIZE_OTHERINFOS<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsM.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: PADDEDKVSOURCEDRAMSZ (keyvalues): "<<PADDEDKVSOURCEDRAMSZ<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: PADDEDKVSOURCEDRAMSZ (bytes): "<<(PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t))<<" bytes"<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid workload space(not including temp space) (keyvalues): "<<(globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid workload space(not including temp space) (keyvalues): "<<(((unsigned long)globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE) * sizeof(keyvalue_t))<<" bytes. "<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid PADDEDKVSOURCEDRAMSZ (keyvalues): "<<((globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid PADDEDKVSOURCEDRAMSZ (bytes): "<<((((unsigned long)globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (HBM max="<<(unsigned int)(KVSOURCEDRAMSZ * 8)<<" bytes)"<< RESET <<std::endl;					
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

	if(((((unsigned long)globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + (unsigned long)globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t)) > (TOTALDRAMCAPACITY_KV * 8)){ cout<<"ERROR: dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
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
	
	// cout<<"[globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsK.MESSAGES_NUM_REDUCEPARTITIONS<<"]"<<endl;
	
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
	utilityobj->checkoutofbounds("loadgraph::loadmessages(A)", (globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE) + globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE, TOTALDRAMCAPACITY_V, NAp, NAp, NAp);
	utilityobj->checkoutofbounds("loadgraph::loadmessages(A)", (globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE) + globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE, PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	utilityobj->checkoutofbounds("loadgraph::loadmessages(B)", (globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR_SIZE) + globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE, PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	#endif
	
	// unsigned int _NUMREDUCEPARTITIONS = (((globalparams.globalparamsV.SIZE_SRCVERTICESDATA / NUM_PEs) + (REDUCEPARTITIONSZ-1)) / REDUCEPARTITIONSZ);
	// unsigned int _NUMPROCESSEDGESPARTITIONS = (((globalparams.globalparamsV.SIZE_SRCVERTICESDATA) + (PROCESSPARTITIONSZ-1)) / PROCESSPARTITIONSZ);
	
	unsigned int _NUMREDUCEPARTITIONS = NUMREDUCEPARTITIONS; // NOTE: check loadgraph::setrootvid to ensure matching
	// unsigned int _NUMPROCESSEDGESPARTITIONS = NUMPROCESSEDGESPARTITIONS;
	unsigned int _NUMPROCESSEDGESPARTITIONS = (((globalparams.globalparamsV.SIZE_SRCVERTICESDATA) + (PROCESSPARTITIONSZ-1)) / PROCESSPARTITIONSZ);
	if(_NUMREDUCEPARTITIONS == 0){ cout<<"loadgraph::loadmessages: ERROR: _NUMREDUCEPARTITIONS("<<_NUMREDUCEPARTITIONS<<") == 0. EXITING..."<<endl; exit(EXIT_FAILURE); }

	createmessages(
			0,
			vdram, // uint512_vec_dt * kvstats,
			container->srcvoffset[0], // srcvoffset,
			container->srcvsize[0], // srcvsize,
			container->edgessize[0], // edgessize,
			container->destvoffset[0], // destvoffset,
			container->actvvsize[0], // actvvsize,
			GraphIter, // GraphIter,
			GraphAlgo, // GraphAlgo,
			container->runsize[0], // runsize
			_NUMPROCESSEDGESPARTITIONS,
			_NUMREDUCEPARTITIONS,
			globalparams.globalparamsV // NEWCHANGE.
			); 
	// exit(EXIT_SUCCESS); //
	
	#ifdef CONFIG_ACTS_HYBRIDLOGIC
	createmessages(
			0,
			mdram, // uint512_vec_dt * kvstats,
			0, // srcvoffset,
			0, // srcvsize,
			globalparams.globalparamsM.SIZE_EDGES, // edgessize,
			0, // destvoffset,
			1, // actvvsize,
			GraphIter, // GraphIter,
			GraphAlgo, // GraphAlgo,
			0, // // runsize
			0, // number of process partitions
			0, // number of reduce partitions
			globalparams.globalparamsM // NEWCHANGE.
			); // runsize
	#endif 
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		createmessages(
			i,
			edges[i], // uint512_vec_dt * kvstats,
			0, // srcvoffset,
			0, // srcvsize,
			container->edgessize[i], // edgessize,
			0, // destvoffset,
			0, // actvvsize,
			GraphIter, // GraphIter,
			GraphAlgo, // GraphAlgo,
			0, // run size,
			_NUMPROCESSEDGESPARTITIONS, // number of process partitions
			_NUMREDUCEPARTITIONS, // number of reduce partitions
			globalparams.globalparamsE); // runsize
		createmessages(
			i,
			kvbuffer[i], // uint512_vec_dt * kvstats,
			container->srcvoffset[i], // srcvoffset,
			container->srcvsize[i], // srcvsize,
			0, // container->edgessize[i], // edgessize,
			container->destvoffset[i], // destvoffset,
			container->actvvsize[i], // actvvsize,
			GraphIter, // GraphIter,
			GraphAlgo, // GraphAlgo,
			container->runsize[i], // runsize
			_NUMPROCESSEDGESPARTITIONS, // number of process partitions
			_NUMREDUCEPARTITIONS, // number of reduce partitions
			globalparams.globalparamsK); // runsize
		#else 
		createmessages(
			i,
			kvbuffer[i], // uint512_vec_dt * kvstats,
			container->srcvoffset[i], // srcvoffset,
			container->srcvsize[i], // srcvsize,
			container->edgessize[i], // edgessize,
			container->destvoffset[i], // destvoffset,
			container->actvvsize[i], // actvvsize,
			GraphIter, // GraphIter,
			GraphAlgo, // GraphAlgo,
			container->runsize[i], // runsize
			_NUMPROCESSEDGESPARTITIONS, // number of process partitions
			_NUMREDUCEPARTITIONS, // number of reduce partitions
			globalparams.globalparamsK); // runsize
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
	cout<<"loadmessages:: _NUMREDUCEPARTITIONS: "<<kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_NUM_REDUCEPARTITIONS].data[0].key<<endl; 
	cout<<"loadmessages:: _NUMPROCESSEDGESPARTITIONS: "<<kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_NUM_PROCESSEDGESPARTITIONS].data[0].key<<endl; 
	cout<<"loadmessages:: _ACTSPARAMS_NUMLOPS: "<<kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key<<endl; 
	cout<<"loadmessages:: _ACTSPARAMS_TREEDEPTH: "<<kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key<<endl; 
	#endif
	return globalparams;
}
globalparams_t loadgraph::createmessages(
			unsigned int id,
			uint512_vec_dt * kvbuffer,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int actvvsize,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			unsigned int _NUMPROCESSEDGESPARTITIONS,
			unsigned int _NUMREDUCEPARTITIONS,
			globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"--- loadgraph::createmessages: ACTIVE_KVSTATSDRAMSZ: "<<ACTIVE_KVSTATSDRAMSZ<<", globalparams.SIZE_KVSTATSDRAM: "<<globalparams.SIZE_KVSTATSDRAM<<endl;
	#endif 
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key = ON;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key = ON;//ON,OFF // CRITICAL REMOVEME.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = ON;//ON,OFF  // CRITICAL REMOVEME. 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = ON;//ON,OFF // CRITICAL REMOVEME.  
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_GPMODE].data[0].key = 
		#ifdef CONFIG_ALGORITHMTYPE_RANDOMACTIVEVERTICES
		// PARAM_ACTSONLYMODE;
		PARAM_HYBRIDGPMODE;
		// PARAM_TRADGPONLYMODE
		#else 
		PARAM_ACTSONLYMODE;
		#endif 
		;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES].data[0].key = 
		#ifdef CONFIG_INSERTSTATSMETADATAINEDGES
		1;
		#else 
		0;
		#endif 
		;
		
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_EVALUATION_ACTS_MEMORYLAYOUT].data[0].key = 
		#ifdef CONFIG_ACTS_MEMORYLAYOUT
		ON;
		#else 
		OFF;
		#endif 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_EVALUATION_ACTS_PARTITIONINGLOGIC].data[0].key = 
		#ifdef CONFIG_ACTS_PARTITIONINGLOGIC
		ON;
		#else 
		OFF;	
		#endif 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC].data[0].key = 
		#ifdef CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
		ON;
		#else 
		OFF;	
		#endif 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_EVALUATION_ACTS_HYBRIDLOGIC].data[0].key = 
		#ifdef CONFIG_ACTS_HYBRIDLOGIC
		ON;
		#else 
		OFF;	
		#endif 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_EVALUATION_ACTS_UNUSED1].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_EVALUATION_ACTS_UNUSED2].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_EVALUATION_ACTS_UNUSED3].data[0].key = NAp;
	
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
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSMESSAGE_OTHERINFOS].data[0].key = globalparams.BASEOFFSETKVS_OTHERINFOS;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].data[0].key = MESSAGESDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].data[0].key = globalparams.SIZE_EDGES; // edgessize; // NEWCHANGE***
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].data[0].key = globalparams.SIZE_VERTEXPTRS; // srcvsize; // NEWCHANGE***
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_SRCVERTICESDATA].data[0].key = globalparams.SIZE_SRCVERTICESDATA; // SRCVERTICESDATASZ; // NEWCHANGE.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_DESTVERTICESDATA].data[0].key = globalparams.SIZE_DESTVERTICESDATA; // DESTVERTICESDATASZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].data[0].key = actvvsize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].data[0].key = globalparams.SIZE_VERTICESDATAMASK;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESPARTITIONMASK].data[0].key = globalparams.SIZE_VERTICESPARTITIONMASK; // CRITICAL FIXME.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].data[0].key = globalparams.SIZE_KVSTATSDRAM; // kvstatssz; // NEWCHANGE
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGESSTATSDRAM].data[0].key = EDGESSTATSDRAMSZ; //64; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].data[0].key = globalparams.SIZE_KVDRAM; // KVDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key = globalparams.SIZE_KVDRAMWORKSPACE; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_OTHERINFOS].data[0].key = globalparams.SIZE_OTHERINFOS; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZEKVS_PROCESSEDGESPARTITION].data[0].key = PROCESSPARTITIONSZ_KVS2; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_REDUCE].data[0].key = REDUCESZ; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZEKVS_REDUCEPARTITION].data[0].key = REDUCEPARTITIONSZ_KVS2; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZEKVS_VMASKBUFFER].data[0].key = VMASKBUFFERSZ_KVS; //
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
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_NUM_REDUCEPARTITIONS].data[0].key = _NUMREDUCEPARTITIONS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_NUM_PROCESSEDGESPARTITIONS].data[0].key = _NUMPROCESSEDGESPARTITIONS;
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].data[0].key = GraphAlgo;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].data[0].key = 1;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key = 
		#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
		TREE_DEPTH-1;
		#else 
		TREE_DEPTH;
		#endif
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key = 
		#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
		TREE_DEPTH-1;
		#else 
		TREE_DEPTH;
		#endif
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key = NUMLASTLEVELPARTITIONS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key = srcvoffset;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key = srcvsize;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key = destvoffset;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER].data[0].key = globalparams.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_INSTID].data[0].key = id; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUM_EDGE_BANKS].data[0].key = //  = NUM_EDGE_BANKS; 
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		NUM_EDGE_BANKS;
		#else
		1;
		#endif 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM].data[0].key = 
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		1;
		#else
		0;
		#endif 

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_DUMMYCHKPOINT].data[0].key = 70707070; 
	for(unsigned int m=0; m<MESSAGES_RETURNVALUES_SIZE; m++){ kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + m].data[0].key = 0; } 
	// for(unsigned int m=0; m<MESSAGES_MAILBOX_SIZE; m++){ kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + m].data[0].key = ON; } // settled at utility.cpp->runsssp_sw
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

globalparams_t loadgraph::finishglobaparamsV(globalparams_t globalparams){
	// order of base addresses
	// messages area {messages}
	// edges area {edges, vertex ptrs} 
	// vertices area {src vertices data, dest vertices data}
	// actvvs area {active vertices, active vertices mask}
	// stats area {stats, edge stats}
	// workspace area {kvdram, kvdram workspace}
	
	// globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + (globalparams.SIZE_VERTICESPARTITIONMASK / VECTOR_SIZE);
	globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + globalparams.SIZE_VERTICESPARTITIONMASK;
	globalparams.SIZE_KVSTATSDRAM = NUMREDUCEPARTITIONS * NUM_PEs * 2; // '2' is padding 
	globalparams.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.BASEOFFSETKVS_STATSDRAM + globalparams.SIZE_KVSTATSDRAM;
	globalparams.SIZE_EDGESSTATSDRAM = 0; 
	globalparams.SIZE_KVDRAM = 0; 
	globalparams.SIZE_KVDRAMWORKSPACE = globalparams.SIZE_KVDRAM;
	globalparams.SIZE_OTHERINFOS = 0;
	globalparams.BASEOFFSETKVS_KVDRAM = globalparams.BASEOFFSETKVS_EDGESSTATSDRAM + EDGESSTATSDRAMSZ;
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.BASEOFFSETKVS_KVDRAM + (globalparams.SIZE_KVDRAM / VECTOR_SIZE); 
	globalparams.BASEOFFSETKVS_OTHERINFOS = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE);
	return globalparams;
}

globalparams_t loadgraph::finishglobaparamsM(globalparams_t globalparams){
	// order of base addresses
	// messages area {messages}
	// edges area {edges, vertex ptrs} 
	// vertices area {src vertices data, dest vertices data}
	// actvvs area {active vertices, active vertices mask}
	// stats area {stats, edge stats}
	// workspace area {kvdram, kvdram workspace}

	// globalparams.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	// globalparams.SIZE_ACTIVEVERTICES = 0; 
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.BASEOFFSETKVS_ACTIVEVERTICES + (globalparams.SIZE_ACTIVEVERTICES / VECTOR2_SIZE);
	globalparams.SIZE_VERTICESDATAMASK = 0; 
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.SIZE_VERTICESDATAMASK / VECTOR_SIZE);
	globalparams.SIZE_VERTICESPARTITIONMASK = 0; 
	// globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + (globalparams.SIZE_VERTICESPARTITIONMASK / VECTOR_SIZE);
	globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + globalparams.SIZE_VERTICESPARTITIONMASK;
	globalparams.SIZE_KVSTATSDRAM = 0; 
	globalparams.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.BASEOFFSETKVS_STATSDRAM + globalparams.SIZE_KVSTATSDRAM;
	globalparams.SIZE_EDGESSTATSDRAM = 0; 
	globalparams.SIZE_KVDRAM = 0; 
	globalparams.SIZE_KVDRAMWORKSPACE = globalparams.SIZE_KVDRAM;
	globalparams.SIZE_OTHERINFOS = 0;
	globalparams.BASEOFFSETKVS_KVDRAM = globalparams.BASEOFFSETKVS_EDGESSTATSDRAM + EDGESSTATSDRAMSZ;
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.BASEOFFSETKVS_KVDRAM + (globalparams.SIZE_KVDRAM / VECTOR_SIZE); 
	globalparams.BASEOFFSETKVS_OTHERINFOS = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE);
	return globalparams;
}