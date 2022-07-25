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

loadgraph::loadgraph(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	evalparamsobj = new evalparams();
	myuniversalparams = _universalparams;
}
loadgraph::~loadgraph(){} 

unsigned int loadgraph::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth, universalparams_t universalparams){
	unsigned int partition = ((keyvalue.key - upperlimit) >> (universalparams.BATCH_RANGE_POW - (universalparams.NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadgraph::getglobalpartition", partition, (1 << (universalparams.NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}

globalparams_t loadgraph::loadvertexdata(std::string algo, keyvalue_t * kvbuffer, vertex_t vbufferoffset, vertex_t size, globalparams_t globalparams, unsigned int edgesorkv, unsigned int srcordest, universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	if(size>0){ cout<<"loadgraph::loadvertexdata:: loading vertex datas... vbufferoffset: "<<vbufferoffset<<", size: "<<size<<endl; }
	#endif
	#ifdef _DEBUGMODE_CHECKS3
	if(size>0){ utilityobj->checkoutofbounds("loadvertexdata.BASEOFFSETKVS_DESTVERTICESDATA", globalparams.BASEOFFSETKVS_DESTVERTICESDATA, universalparams.MAXHBMCAPACITY_KVS2, NAp, NAp, NAp);	}
	#endif
	
	algorithm * algorithmobj = new algorithm();
	value_t * KV = (value_t *)&kvbuffer[vbufferoffset];
	for(unsigned int k=0; k<size; k++){
		KV[k] = algorithmobj->vertex_initdata(algo); 
	} 
	return globalparams;
}

globalparams_TWOt loadgraph::loadactvvertices(uint512_vec_dt * vdram, vector<vertex_t> &activevertices, globalparams_TWOt globalparams, universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadactvvertices:: loading active vertices... "<<endl;
	#endif
	
	globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.globalparamsK.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_ACTIVEVERTICES = universalparams.NUMPROCESSEDGESPARTITIONS * ALLIGNED_MAXNUM_EDGEBLOCKS_PER_VPARTITION; // 0; 
	
	globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.globalparamsE.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_ACTIVEVERTICES = 0;
	
	globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.globalparamsV.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsV.SIZE_ACTIVEVERTICES = universalparams.NUMPROCESSEDGESPARTITIONS * ALLIGNED_MAXNUM_EDGEBLOCKS_PER_VPARTITION; //    (MAXNUM_EDGEBLOCKS_PER_VPARTITION * VECTOR2_SIZE);
	
	unsigned int baseoffset_activevertices = globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadactvvertices.BASEOFFSETKVS_ACTIVEVERTICES", globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES, universalparams.MAXHBMCAPACITY_KVS2, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSET_ACTIVEVERTICES: "<<globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES: "<<globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES<<"]"<<endl;
	#endif 
	
	// void utility::writedata(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data)
	// for(unsigned int t=0; t<4; t++){
		// for(unsigned int v=0; v<8; v++){
			// vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].key = 777; 
			// vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].value = 777;
		// }
	// }
	
	utilityobj->writedata(vdram, globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES, 0, 1);
	utilityobj->writedata(vdram, globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES, 1, 0);
	return globalparams;
}

globalparams_TWOt loadgraph::loadoffsetmarkers(keyvalue_t * stats[MAXNUM_PEs], vector<edge2_vec_dt> (&edges_final)[MAXNUM_PEs], container_t * container, globalparams_TWOt globalparams, universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadoffsetmarkers:: loading offset markers... "<<endl;
	#endif
	
	// CRITICAL NEWCHANGE. since kvdram and edges dram are now merged into one
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK;
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = ((1 << (universalparams.NUM_PARTITIONS_POW * universalparams.TREE_DEPTH)) * 2) * 1; // FIXME?
	
	globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK + globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK;
	globalparams.globalparamsE.SIZE_KVSTATSDRAM = ((1 << (universalparams.NUM_PARTITIONS_POW * universalparams.TREE_DEPTH)) * 2); 
	
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM + globalparams.globalparamsK.SIZE_KVSTATSDRAM; 
	globalparams.globalparamsK.SIZE_EDGESSTATSDRAM = 0;
	
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM + globalparams.globalparamsE.SIZE_KVSTATSDRAM;
	globalparams.globalparamsE.SIZE_EDGESSTATSDRAM = EDGESSTATSDRAMSZ;
	
	// calculate best-fit value for NUM_EDGECHUNKS_IN_A_BUFFER
	unsigned int num_edgechunks_in_a_buffer = 0;
	// NB: Formular: 3V + 2E/N + 2E/N = (256MB/4)
	unsigned int num_vertices_per_channel = 0;
	unsigned int VCapacity_u32 = globalparams.globalparamsK.SIZE_DESTVERTICESDATA + globalparams.globalparamsK.SIZE_VERTEXPTRS; // capacity (uint32_t) for v data
	unsigned int KvCapacity_u32; if(universalparams.TREE_DEPTH >= 3){ KvCapacity_u32 = 2 * (HBM_BITWIDTHx32 / EDGEDATA_PACKINGSIZE) * globalparams.globalparamsE.SIZE_EDGES; } else { KvCapacity_u32 = (HBM_BITWIDTHx32 / EDGEDATA_PACKINGSIZE) * globalparams.globalparamsE.SIZE_EDGES; } // capacity (uint32_t) for kv data
	unsigned int AvailKvCapacity_u32 = universalparams.VOLUMESIZEU32_WORKSPACEDATA - VCapacity_u32;
	if(universalparams.VOLUMESIZEU32_WORKSPACEDATA < VCapacity_u32){ cout<<"loadoffsetmarkers::ERROR: (universalparams.VOLUMESIZEU32_WORKSPACEDATA)("<<(universalparams.VOLUMESIZEU32_WORKSPACEDATA)<<") < (3 * num_vertices_per_channel)("<<(3 * num_vertices_per_channel)<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	num_edgechunks_in_a_buffer = (KvCapacity_u32 + (AvailKvCapacity_u32-1)) / AvailKvCapacity_u32; // 3V + 2E/N + 2E/N = (256MB/4)
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadoffsetmarkers:: KvCapacity_u32: "<<KvCapacity_u32<<", AvailKvCapacity_u32: "<<AvailKvCapacity_u32<<", num_edgechunks_in_a_buffer: "<<num_edgechunks_in_a_buffer<<", globalparams.globalparamsE.SIZE_EDGES: "<<globalparams.globalparamsE.SIZE_EDGES<<endl;
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"loadoffsetmarkers:: number of edgechunks in a buffer: "<<num_edgechunks_in_a_buffer<<endl;
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadoffsetmarkers:: globalparams.globalparamsE.SIZE_EDGES: "<<globalparams.globalparamsE.SIZE_EDGES<<endl;
	cout<<"loadoffsetmarkers:: globalparams.globalparamsK.SIZE_VERTEXPTRS: "<<globalparams.globalparamsK.SIZE_VERTEXPTRS<<endl;
	cout<<"loadoffsetmarkers:: globalparams.globalparamsK.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsK.SIZE_DESTVERTICESDATA<<endl;
	cout<<"loadoffsetmarkers:: universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA: "<<universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA<<endl;
	cout<<"loadoffsetmarkers:: universalparams.VOLUMESIZEU32_WORKSPACEDATA: "<<universalparams.VOLUMESIZEU32_WORKSPACEDATA<<endl;
	cout<<"loadoffsetmarkers:: universalparams.VOLUMEOFFSETKVS_WORKDATA: "<<universalparams.VOLUMEOFFSETKVS_WORKDATA<<endl;
	cout<<"loadoffsetmarkers:: universalparams.VOLUMESIZEU32_WORKDATA: "<<universalparams.VOLUMESIZEU32_WORKDATA<<endl;
	cout<<"loadoffsetmarkers:: VCapacity_u32: "<<VCapacity_u32<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = num_edgechunks_in_a_buffer;
	globalparams.globalparamsE.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;
	
	unsigned int baseoffset_edgesdata = 0;
		baseoffset_edgesdata = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	unsigned int kvstatsdramsz = globalparams.globalparamsK.SIZE_KVSTATSDRAM;
	
	globalparams_t globalparamsVPTRS;
	globalparams_t globalparamsSTATS;
		globalparamsVPTRS = globalparams.globalparamsE;
		globalparamsSTATS = globalparams.globalparamsE;
	globalparams_t globalparamsEDGESSTATS;
	globalparamsEDGESSTATS = globalparams.globalparamsE; 	
	
	unsigned int baseoffset_statsdram = globalparamsSTATS.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE; // NEWCHANGE.
	unsigned int baseoffset_edgesstatsdram = globalparamsSTATS.BASEOFFSETKVS_EDGESSTATSDRAM * VECTOR_SIZE; 
	unsigned int baseoffset_edgesstatsdram2 = (BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM) * VECTOR_SIZE; 
	unsigned int baseoffset_vertexptr = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE;
	
	unsigned int num_vPs = universalparams.NUMPROCESSEDGESPARTITIONS;
	unsigned int vsize_vP = universalparams.PROCESSPARTITIONSZ;
	unsigned int num_LLPs = universalparams.NUMREDUCEPARTITIONS * universalparams.NUM_PARTITIONS;
	unsigned int vsize_LLP = universalparams.REDUCEPARTITIONSZ_KVS2;
	unsigned int num_LLPset = (num_LLPs + (universalparams.NUM_PARTITIONS - 1)) / universalparams.NUM_PARTITIONS;
	unsigned int vsize_LLPset = vsize_LLP * universalparams.NUM_PARTITIONS;
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<">>> loadgraph:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", vsize_LLP: "<<vsize_LLP<<", vsize_LLPset: "<<vsize_LLPset<<", num_vPs: "<<num_vPs<<", vsize_vP: "<<vsize_vP<<", INVALIDDATA: "<<INVALIDDATA<<"] "<<endl;
	#endif 
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_STATSDRAM", globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM, universalparams.PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif
	
	unsigned int totalnumpartitions = 0;
	for(unsigned int k=0; k<=universalparams.TREE_DEPTH; k++){ totalnumpartitions += (unsigned int)pow(universalparams.NUM_PARTITIONS, k); } 
	unsigned int maxdramsz = 0;
	
	for(unsigned int i = 0; i < NUM_PEs; i++){
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][baseoffset_statsdram];
		for(unsigned int k=0; k<kvstatsdramsz; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ statsptrVec[k].data[v].key = 0; statsptrVec[k].data[v].value = 0; }
		}
	}
	
	keyvalue_t **tempstats = new keyvalue_t*[num_edgechunks_in_a_buffer];
	for(unsigned int i = 0; i < num_edgechunks_in_a_buffer; ++i){ tempstats[i] = new keyvalue_t[kvstatsdramsz]; }
	for(unsigned int u=0; u<num_edgechunks_in_a_buffer; u++){ for(unsigned int k=0; k<kvstatsdramsz; k++){ tempstats[u][k].key = 0; tempstats[u][k].value = 0; }}
	
	unsigned int src1 = 0;
	unsigned int src2 = 0;
	
	for(unsigned int i = 0; i < NUM_PEs; i++){
		#ifdef TESTKERNEL // ignore unused loads when testing
		// if(i==0 || i==NUMCOMPUTEUNITS_SLR2 || i==NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1){ } else { continue; }
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<< endl << TIMINGRESULTSCOLOR <<"loadgraph::loadoffsetmarkers:: loading stats for PE "<<i<<" [edgessize["<<i<<"]: "<<container->edgessize[i]<<"]"<< RESET << endl;
		#endif 
		if(i >= NUM_PEs){ continue; }
		
		keyvalue_t * statsptr = (keyvalue_t *)&stats[i][baseoffset_statsdram];
		for(unsigned int u=0; u<num_edgechunks_in_a_buffer; u++){ for(unsigned int k=0; k<kvstatsdramsz; k++){ tempstats[u][k].key = 0; tempstats[u][k].value = 0; }}
		
		unsigned int src1 = 0;
		unsigned int src2 = 0;
		
		unsigned int srC=0;
		unsigned int SRC_CHKPT[8];
		unsigned int PARTITION_CHKPT[8];
		unsigned int num_edges_in_a_chunk = container->edgessize[i] / num_edgechunks_in_a_buffer;
		for(unsigned int k=0; k<num_edgechunks_in_a_buffer; k++){
			unsigned int index = k * (container->edgessize[i] / num_edgechunks_in_a_buffer);
			unsigned int index_kvs = index / EDGEDATA_PACKINGSIZE; // 64
			bool isfound=false; for(unsigned int t=0; t<1024; t++){ if(edges_final[i][index_kvs+t].data[0].srcvid != INVALIDDATA){ srC = edges_final[i][index_kvs+t].data[0].srcvid; isfound=true; break; }}
			if(isfound==false){ cout<<"loadoffsetmarkers: ERROR 65. isfound==false. EXITING..."<<endl; 
				for(unsigned int t=0; t<1024; t++){ cout<<"edges_final["<<i<<"]["<<index_kvs+t<<"].data[0].srcvid: "<<edges_final[i][index_kvs+t].data[0].srcvid<<", edges_final["<<i<<"]["<<index_kvs+t<<"].data[0].srcvid: "<<edges_final[i][index_kvs+t].data[0].srcvid<<endl; }
				exit(EXIT_FAILURE); }
			srC = (srC / universalparams.PROCESSPARTITIONSZ) * universalparams.PROCESSPARTITIONSZ; // round down
			SRC_CHKPT[k] = srC;
			PARTITION_CHKPT[k] = srC / universalparams.PROCESSPARTITIONSZ;
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"loadoffsetmarkers: k: "<<k<<", index: "<<index<<", srC: "<<srC<<", SRC_CHKPT["<<k<<"]: "<<SRC_CHKPT[k]<<", PARTITION_CHKPT["<<k<<"]: "<<PARTITION_CHKPT[k]<<", num_edges_in_a_chunk: "<<num_edges_in_a_chunk<<", num_edgechunks_in_a_buffer: "<<num_edgechunks_in_a_buffer<<endl; 					
			#endif
		}
		SRC_CHKPT[num_edgechunks_in_a_buffer] = universalparams.KVDATA_RANGE-1;
		PARTITION_CHKPT[num_edgechunks_in_a_buffer] = universalparams.KVDATA_RANGE / universalparams.PROCESSPARTITIONSZ;
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadoffsetmarkers: index: LAST. srC: "<<srC<<", SRC_CHKPT["<<num_edgechunks_in_a_buffer<<"]: "<<SRC_CHKPT[num_edgechunks_in_a_buffer]<<", PARTITION_CHKPT["<<num_edgechunks_in_a_buffer<<"]: "<<PARTITION_CHKPT[num_edgechunks_in_a_buffer]<<", num_edges_in_a_chunk: "<<num_edges_in_a_chunk<<", num_edgechunks_in_a_buffer: "<<num_edgechunks_in_a_buffer<<endl; 					
		#endif
		// exit(EXIT_SUCCESS);
		
		for(unsigned int p=0; p<num_edgechunks_in_a_buffer; p++){ tempstats[p][0].value = num_edges_in_a_chunk; } // container->edgessize[i]; } // NEWCHANGE.

		for(unsigned int k=0; k<edges_final[i].size(); k++){ // container->edgessize[i]
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(k % 1000000 == 0){ cout<<"### loadgraph::loadoffsetmarkers:: k: "<<k<<endl; }
			#endif
			
			edge2_type edge = edges_final[i][k].data[0];
	
			unsigned int ll_p = edge.dstvid / vsize_LLPset;
			#ifdef _DEBUGMODE_HOSTCHECKS3
			utilityobj->checkoutofbounds("loadgraph::ERROR 23::", ll_p, num_LLPs, edge.dstvid, edge.dstvid, vsize_LLP);
			#endif 
			
			unsigned int p=0;
			for(unsigned int u=0; u<num_edgechunks_in_a_buffer; u++){ if(edge.srcvid >= SRC_CHKPT[u] && edge.srcvid < SRC_CHKPT[u+1]){ p = u; } }
			tempstats[p][1 + ll_p].value += EDGEDATA_PACKINGSIZE;
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"### loadgraph::loadoffsetmarkers:: Finished collecting stats &&& for PE: "<<i<<endl;
		#endif 
	
		for(unsigned int u=0; u<num_edgechunks_in_a_buffer; u++){ 
			utilityobj->getmarkerpositions((keyvalue_t *)&tempstats[u][1], universalparams.NUMREDUCEPARTITIONS, UPDATEDATA_PACKINGSIZE); 
		}
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(i%NUMCOMPUTEUNITS_SLR0==0 || i%NUMCOMPUTEUNITS_SLR1==0 || i%NUMCOMPUTEUNITS_SLR2==0){
			for(unsigned int u=0; u<num_edgechunks_in_a_buffer; u++){ 
				cout<<"loadoffsetmarkers: printing tempstats for u: "<<u<<endl; 
				utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats[u][----]", tempstats[u], 1 + universalparams.NUMREDUCEPARTITIONS + 4, 1); 
			}
		}
		#endif
		// exit(EXIT_SUCCESS);
		
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][baseoffset_statsdram];
		for(unsigned int u=0; u<num_edgechunks_in_a_buffer; u++){ 
			for(unsigned int k=0; k<kvstatsdramsz; k++){ 
				statsptrVec[k].data[u].key = tempstats[u][k].key; statsptrVec[k].data[u].value = 0; // tempstats[u][k].value;
			}
		} 
		
		uint512_vec_dt * edgesstatsptrVec = (uint512_vec_dt *)&stats[i][baseoffset_edgesstatsdram];
		uint512_vec_dt * edgesstatsptrVec2 = (uint512_vec_dt *)&stats[i][baseoffset_edgesstatsdram2];
		for(unsigned int u=0; u<num_edgechunks_in_a_buffer+1; u++){
			edgesstatsptrVec[u].data[0].key = PARTITION_CHKPT[u]; edgesstatsptrVec[u].data[0].value = 0;
			edgesstatsptrVec2[u].data[0].key = PARTITION_CHKPT[u]; edgesstatsptrVec2[u].data[0].value = 0;
		}

		for(unsigned int u=0; u<num_edgechunks_in_a_buffer; u++){
			if(maxdramsz < (tempstats[u][1+universalparams.NUMREDUCEPARTITIONS-1].key + tempstats[u][1+universalparams.NUMREDUCEPARTITIONS-1].value)){ maxdramsz = tempstats[u][1+universalparams.NUMREDUCEPARTITIONS-1].key + tempstats[u][1+universalparams.NUMREDUCEPARTITIONS-1].value; }
		}
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadoffsetmarkers:: maxdramsz: "<<maxdramsz<<", universalparams.TREE_DEPTH: "<<universalparams.TREE_DEPTH<<endl;
		#endif
		
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(i==0){ for(unsigned int u=0; u<num_edgechunks_in_a_buffer; u++){ cout<<"loadoffsetmarkers: u:"<<u<<endl; utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats[u] [after]", tempstats[u], 17, 1); } } // 17, 273
		#endif
	}
	// exit(EXIT_SUCCESS); 
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadoffsetmarkers :::::::::::::::::::::::::::::::::::: maxdramsz: "<<maxdramsz<<", universalparams.TREE_DEPTH: "<<universalparams.TREE_DEPTH<<" ::::::::::::::::::::::::::::::::::::"<<endl;
	#endif
	
	globalparams.globalparamsK.SIZE_KVDRAM = maxdramsz + 64; 
	if(universalparams.TREE_DEPTH >= 3){ globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = globalparams.globalparamsK.SIZE_KVDRAM; } else { globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = 0; } // TO-FIT NEWCHANGE.
	globalparams.globalparamsK.SIZE_OTHERINFOS = 0; 
	globalparams.globalparamsE.SIZE_KVDRAM = 0; 
	globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	globalparams.globalparamsE.SIZE_OTHERINFOS = 0;
	
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + globalparams.globalparamsK.SIZE_EDGESSTATSDRAM; 
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM + (globalparams.globalparamsK.SIZE_KVDRAM / UPDATEDATA_PACKINGSIZE); 
	globalparams.globalparamsK.BASEOFFSETKVS_OTHERINFOS = globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE / UPDATEDATA_PACKINGSIZE); 
	globalparams.globalparamsK.BASEOFFSETKVS_ENDOFFILE = globalparams.globalparamsK.BASEOFFSETKVS_OTHERINFOS + 256;

	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM + globalparams.globalparamsE.SIZE_EDGESSTATSDRAM; 
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM + (globalparams.globalparamsE.SIZE_KVDRAM / UPDATEDATA_PACKINGSIZE);
	globalparams.globalparamsE.BASEOFFSETKVS_OTHERINFOS = globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE / UPDATEDATA_PACKINGSIZE); 
	globalparams.globalparamsE.BASEOFFSETKVS_ENDOFFILE = globalparams.globalparamsE.BASEOFFSETKVS_OTHERINFOS + 256;
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_KVDRAM", globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM, universalparams.PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_KVDRAMWORKSPACE", globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE, universalparams.PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	for(unsigned int i=0; i<1; i++){
		cout<<">>> loadoffsetmarkers: printing stats["<<i<<"]..."<<endl;
		utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][baseoffset_statsdram]", (keyvalue_t *)&stats[i][baseoffset_statsdram], (1+universalparams.NUMREDUCEPARTITIONS) * VECTOR_SIZE, VECTOR_SIZE);
		// utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][baseoffset_statsdram]", (keyvalue_t *)&stats[i][baseoffset_statsdram], totalnumpartitions * VECTOR_SIZE, VECTOR_SIZE);
	}
	#endif
	// exit(EXIT_SUCCESS); //
	return globalparams;
}
void loadgraph::accumstats(uint512_vec_dt * kvbuffer[MAXNUM_PEs], uint512_vec_dt * stats[MAXNUM_PEs], globalparams_TWOt globalparams, universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::accumstats:: accumulating stats to base... "<<endl;
	#endif

	for(unsigned int i=0; i<NUM_PEs; i+=universalparams.NUM_EDGE_BANKS){
		uint512_vec_dt * basestatsptrVec = (uint512_vec_dt *)&kvbuffer[i / universalparams.NUM_EDGE_BANKS][globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM];
		for(unsigned int j=0; j<universalparams.NUM_EDGE_BANKS; j++){
			uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i + j][globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM];
			for(unsigned int k=0; k<globalparams.globalparamsK.SIZE_KVSTATSDRAM; k++){ // globalparams.globalparamsK.SIZE_KVSTATSDRAM // ACTIVE_KVSTATSSZ
				basestatsptrVec[j*globalparams.globalparamsK.SIZE_KVSTATSDRAM + k] = statsptrVec[k];
			}
		}
	}
	return;
}

globalparams_TWOt loadgraph::loadmaps(vector<vertex_t> &activevertices, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUM_PEs], map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs], keyvalue_t * edgeblock_map[MAXNUM_PEs][MAXNUM_VPs], globalparams_TWOt globalparams, universalparams_t universalparams){ 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadmaps:: generating vmask... "<<endl;
	#endif
	
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP = globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES + (globalparams.globalparamsK.SIZE_ACTIVEVERTICES / VECTOR2_SIZE);
	globalparams.globalparamsK.SIZE_EDGESMAP = 0;
	
	globalparams.globalparamsK.BASEOFFSETKVS_EDGEBLOCKMAP = globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP + (globalparams.globalparamsK.SIZE_EDGESMAP / VECTOR2_SIZE);
	globalparams.globalparamsK.SIZE_EDGEBLOCKMAP = 0; 
	
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP + (globalparams.globalparamsK.SIZE_EDGESMAP / VECTOR2_SIZE);
	globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK = ((universalparams.NUMPROCESSEDGESPARTITIONS * 2) * VECTOR2_SIZE); // '*2' is padding value. AUTOMATEME.
	
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP = globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES + (globalparams.globalparamsE.SIZE_ACTIVEVERTICES / VECTOR2_SIZE);
	globalparams.globalparamsE.SIZE_EDGESMAP = utilityobj->allignlower_FACTOR((universalparams.NUMPROCESSEDGESPARTITIONS * (1 << (OPT_NUM_PARTITIONS_POW * universalparams.TREE_DEPTH))), 16); // num_vPs * num_LLPs
	
	globalparams.globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP = globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP + (globalparams.globalparamsE.SIZE_EDGESMAP / VECTOR2_SIZE);
	globalparams.globalparamsE.SIZE_EDGEBLOCKMAP = universalparams.NUMPROCESSEDGESPARTITIONS * ALLIGNED_MAXNUM_EDGEBLOCKS_PER_VPARTITION; // utilityobj->allignlower_FACTOR((universalparams.NUMPROCESSEDGESPARTITIONS * (1 << (OPT_NUM_PARTITIONS_POW * universalparams.TREE_DEPTH))), 16); // num_vPs * num_LLPs
	
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP + (globalparams.globalparamsE.SIZE_EDGEBLOCKMAP / VECTOR2_SIZE);
	globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK = 0;
	
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP = globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + (globalparams.globalparamsV.SIZE_ACTIVEVERTICES / VECTOR2_SIZE);
	globalparams.globalparamsV.SIZE_EDGESMAP = 0;
	
	globalparams.globalparamsV.BASEOFFSETKVS_EDGEBLOCKMAP = globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP + (globalparams.globalparamsV.SIZE_EDGESMAP / VECTOR2_SIZE);
	globalparams.globalparamsV.SIZE_EDGEBLOCKMAP = 0; 
	
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP + (globalparams.globalparamsV.SIZE_EDGESMAP / VECTOR2_SIZE);
	globalparams.globalparamsV.SIZE_VERTICESPARTITIONMASK = ((universalparams.NUMPROCESSEDGESPARTITIONS * 2) * VECTOR2_SIZE);
	
	unsigned int _BASEOFFSET_VERTICESDATAMASK_KVS = globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP;
	unsigned int _BASEOFFSET_VERTICESPARTITIONMASK_KVS = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK;
	unsigned int _BASEOFFSETV_VERTICESDATAMASK_KVS = globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP;
	unsigned int _BASEOFFSETV_VERTICESPARTITIONMASK_KVS = globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK; 
	
	// load edges map
	unsigned int num_vPs = universalparams.NUMPROCESSEDGESPARTITIONS;
	unsigned int num_LLPs = universalparams.NUMREDUCEPARTITIONS * universalparams.NUM_PARTITIONS; 
	// unsigned int vsize_LLP; if(universalparams.ALGORITHM == BFS || universalparams.ALGORITHM == SSSP){ vsize_LLP = 262144; } else { vsize_LLP = universalparams.REDUCEPARTITIONSZ_KVS2; }
	unsigned int num_LLPset = (num_LLPs + (universalparams.NUM_PARTITIONS - 1)) / universalparams.NUM_PARTITIONS;
	
	// load edge map 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		#ifdef TESTKERNEL 
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadgraph::loadmaps:: [PE: "<<i<<"]"<<endl;
		#endif 
		unsigned int index = 0;
		value_t * edges_u32 = (unsigned int *)&edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP];
		for(unsigned int v_p=0; v_p<num_vPs; v_p++){
			for(unsigned int ll_p=0; ll_p<num_LLPset; ll_p++){
				edges_u32[index] = edges_map[i][v_p][ll_p].offset; // / EDGEDATA_PACKINGSIZE;
				// if(false){ cout<<"loadgraph::loadmaps:: edges_u32["<<index<<"]: "<<edges_u32[index]<<", edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].offset: "<<edges_map[i][v_p][ll_p].offset<<endl; }
				if(i==0 && v_p==0 && ll_p<64){ cout<<"loadgraph::load edge maps:: edges_u32["<<index<<"]: "<<edges_u32[index]<<endl; }
				index += 1;
			}
		}
		for(unsigned int padd=index; padd<index+16; padd++){ edges_u32[padd] = edges_u32[index]; }// padding 
	}
	
	// load edge block map 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		#ifdef TESTKERNEL 
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadgraph::loadmaps:: [PE: "<<i<<"]"<<endl;
		#endif 
		unsigned int index = 0; // triple_t * edgeblock_map[MAXNUM_PEs][MAXNUM_VPs]
		keyvalue_t * mask_u32 = (keyvalue_t *)&edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP];
	
		for(unsigned int v_p=0; v_p<num_vPs; v_p++){
			// for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){
			for(unsigned int t=0; t<ALLIGNED_MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){
				mask_u32[index] = edgeblock_map[i][v_p][t];
				#ifdef _DEBUGMODE_HOSTPRINTS3
				// if(i==0 && v_p==0 && t<64){ cout<<"loadgraph::loadmaps:: edgeblock_map["<<i<<"]["<<v_p<<"]["<<t<<"].key: "<<edgeblock_map[i][v_p][t].key<<", edgeblock_map["<<i<<"]["<<v_p<<"]["<<t<<"].value: "<<edgeblock_map[i][v_p][t].value<<endl; }
				if(i==0 && v_p<3 && t<64){ cout<<"loadgraph::load edgeblock maps:: mask_u32["<<index<<"].key: "<<mask_u32[index].key<<", mask_u32["<<index<<"].value: "<<mask_u32[index].value<<endl; }
				#endif 
				index += 1;
			}
		}
		
	}
	
	// load vertex partition mask 
	vdram[_BASEOFFSETV_VERTICESPARTITIONMASK_KVS].data[0].key = 0x00000001;
	
	// checks 
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadmaps.BASEOFFSETKVS_EDGESMAP", globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP, universalparams.PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	utilityobj->checkoutofbounds("loadmaps.BASEOFFSETKVS_VERTICESPARTITIONMASK", globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK, universalparams.PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP<<"]"<<endl;
	#endif
	// exit(EXIT_SUCCESS);
	return globalparams;
}

void loadgraph::savevdata(uint512_vec_dt * kvdram, unsigned int buffer[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::savemasks:: saving vdata: voffset_kvs: "<<voffset_kvs<<endl;
	#endif 
	for(unsigned int i=0; i<universalparams.REDUCEPARTITIONSZ_KVS2; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram[vbaseoffset_kvs + voffset_kvs + i].data[v].key = buffer[2*v][i];
			kvdram[vbaseoffset_kvs + voffset_kvs + i].data[v].value = buffer[2*v+1][i];
		}
	}
	return;
}
void loadgraph::setrootvid(uint512_vec_dt * vbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams, universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::setrootvid:: setting root vid(s)... "<<endl;
	#endif 
	
	unsigned int data = 0;
	utilityobj->WRITETO_UINT((unsigned int *)&data, OFFSETOF_VDATA, SIZEOF_VDATA, 0);
	utilityobj->WRITETO_UINT((unsigned int *)&data, OFFSETOF_VMASK, SIZEOF_VMASK, 1);
	
	uint512_ivec_dt * ivbuffer = (uint512_ivec_dt *)&vbuffer[globalparams.BASEOFFSETKVS_SRCVERTICESDATA];	
	
	#ifdef CONFIG_EDGEHASHSCHEME_SETVIDS
	unsigned int H = (universalparams.ROOTVID % (EDGEDATA_PACKINGSIZE * NUM_PEs)) / EDGEDATA_PACKINGSIZE;
	unsigned int ldstvid = ((universalparams.ROOTVID / (EDGEDATA_PACKINGSIZE * NUM_PEs)) * EDGEDATA_PACKINGSIZE) + (universalparams.ROOTVID % EDGEDATA_PACKINGSIZE); 
	unsigned int depth = (H * universalparams.NUMREDUCEPARTITIONS * universalparams.REDUCEPARTITIONSZ_KVS2) / VDATA_SHRINK_RATIO;
	ivbuffer[depth + (ldstvid / VECTOR2_SIZE)].data[ldstvid % VECTOR2_SIZE] = data;
	// for(unsigned int v=2; v<14; v++){ ivbuffer[depth + (ldstvid / VECTOR2_SIZE)].data[v] = data; }
	#endif 
	
	#ifdef CONFIG_EDGEHASHSCHEME_SINGLEVID
	unsigned int depth = (1 * universalparams.NUMREDUCEPARTITIONS * universalparams.REDUCEPARTITIONSZ_KVS2) / VDATA_SHRINK_RATIO;
	ivbuffer[depth].data[0] = data;
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::setrootvid: data: "<<data<<", depth: "<<depth<<", ldstvid / VECTOR2_SIZE: "<<ldstvid / VECTOR2_SIZE<<", ldstvid % VECTOR2_SIZE: "<<ldstvid % VECTOR2_SIZE<<", vbuffer["<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + depth + (ldstvid / VECTOR2_SIZE)<<"].data[0].key: "<<vbuffer[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + depth + (ldstvid / VECTOR2_SIZE)].data[0].key<<endl;			
	#endif 
	return;
}

globalparams_TWOt loadgraph::loadmessages(uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvbuffer[MAXNUM_PEs], container_t * container, globalparams_TWOt globalparams, universalparams_t universalparams){	
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
	
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsM.BASEOFFSETKVS_EDGESMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsM.BASEOFFSETKVS_EDGESMAP<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_EDGEBLOCKMAP: "<<globalparams.globalparamsM.BASEOFFSETKVS_EDGEBLOCKMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.BASEOFFSETKVS_EDGEBLOCKMAP: "<<globalparams.globalparamsM.BASEOFFSETKVS_EDGEBLOCKMAP<<"]"<<endl;
	
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
	cout<<"[globalparams.globalparamsM.SIZE_EDGESMAP: "<<globalparams.globalparamsM.SIZE_EDGESMAP<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_VERTICESPARTITIONMASK: "<<globalparams.globalparamsM.SIZE_VERTICESPARTITIONMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_KVSTATSDRAM: "<<globalparams.globalparamsM.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_EDGESSTATSDRAM: "<<globalparams.globalparamsM.SIZE_EDGESSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_KVDRAM: "<<globalparams.globalparamsM.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE: "<<globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.SIZE_OTHERINFOS: "<<globalparams.globalparamsM.SIZE_OTHERINFOS<<"]"<<endl;
	cout<<"[globalparams.globalparamsM.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsM.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: universalparams.PADDEDKVSOURCEDRAMSZ (keyvalues): "<<universalparams.PADDEDKVSOURCEDRAMSZ<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: universalparams.PADDEDKVSOURCEDRAMSZ (bytes): "<<(universalparams.PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t))<<" bytes"<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid workload space(not including temp space) (keyvalues): "<<(globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid workload space(not including temp space) (keyvalues): "<<(((unsigned long)globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM*VECTOR_SIZE) * sizeof(keyvalue_t))<<" bytes. "<< RESET <<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[sizes]:: valid universalparams.PADDEDKVSOURCEDRAMSZ (keyvalues): "<<((globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE)<<" keyvalues"<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid universalparams.PADDEDKVSOURCEDRAMSZ (bytes): "<<((((unsigned long)globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (HBM max="<<(unsigned int)(universalparams.KVSOURCEDRAMSZ * 8)<<" bytes)"<< RESET <<std::endl;					
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"[globalparams.globalparamsV.BASEOFFSET_MESSAGESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA: "<<globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.VOLUMEOFFSETKVS_WORKDATA: "<<globalparams.globalparamsV.VOLUMEOFFSETKVS_WORKDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.VOLUMEOFFSETKVS_WORKSPACEDATA: "<<globalparams.globalparamsV.VOLUMEOFFSETKVS_WORKSPACEDATA<<"]"<<endl;
	
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
	
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_EDGEBLOCKMAP: "<<globalparams.globalparamsV.BASEOFFSETKVS_EDGEBLOCKMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.BASEOFFSETKVS_EDGEBLOCKMAP: "<<globalparams.globalparamsV.BASEOFFSETKVS_EDGEBLOCKMAP<<"]"<<endl;
	
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
	cout<<"[globalparams.globalparamsV.VOLUMESIZEU32_WORKDATA: "<<globalparams.globalparamsV.VOLUMESIZEU32_WORKDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.VOLUMESIZEU32_WORKSPACEDATA: "<<globalparams.globalparamsV.VOLUMESIZEU32_WORKSPACEDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_EDGES: "<<globalparams.globalparamsV.SIZE_EDGES<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_VERTEXPTRS: "<<globalparams.globalparamsV.SIZE_VERTEXPTRS<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_SRCVERTICESDATA: "<<globalparams.globalparamsV.SIZE_SRCVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsV.SIZE_DESTVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_ACTIVEVERTICES: "<<globalparams.globalparamsV.SIZE_ACTIVEVERTICES<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_EDGESMAP: "<<globalparams.globalparamsV.SIZE_EDGESMAP<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_VERTICESPARTITIONMASK: "<<globalparams.globalparamsV.SIZE_VERTICESPARTITIONMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_KVSTATSDRAM: "<<globalparams.globalparamsV.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_EDGESSTATSDRAM: "<<globalparams.globalparamsV.SIZE_EDGESSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_KVDRAM: "<<globalparams.globalparamsV.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE: "<<globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.SIZE_OTHERINFOS: "<<globalparams.globalparamsV.SIZE_OTHERINFOS<<"]"<<endl;
	cout<<"[globalparams.globalparamsV.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsV.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid universalparams.PADDEDKVSOURCEDRAMSZ: "<<((((unsigned long)globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (HBM max="<<(unsigned int)((1 << 28))<<" bytes)"<< RESET <<std::endl;					
	// if(((((unsigned long)globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + (unsigned long)globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t)) > ((1 << 28))){ cout<<"ERROR: dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"[globalparams.globalparamsK.BASEOFFSET_MESSAGESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA: "<<globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.VOLUMEOFFSETKVS_WORKDATA: "<<globalparams.globalparamsK.VOLUMEOFFSETKVS_WORKDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.VOLUMEOFFSETKVS_WORKSPACEDATA: "<<globalparams.globalparamsK.VOLUMEOFFSETKVS_WORKSPACEDATA<<"]"<<endl;
	
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
	
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGEBLOCKMAP: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGEBLOCKMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_EDGEBLOCKMAP: "<<globalparams.globalparamsK.BASEOFFSETKVS_EDGEBLOCKMAP<<"]"<<endl;
	
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
	cout<<"[globalparams.globalparamsK.VOLUMESIZEU32_WORKDATA: "<<globalparams.globalparamsK.VOLUMESIZEU32_WORKDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.VOLUMESIZEU32_WORKSPACEDATA: "<<globalparams.globalparamsK.VOLUMESIZEU32_WORKSPACEDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_EDGES: "<<globalparams.globalparamsK.SIZE_EDGES<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_VERTEXPTRS: "<<globalparams.globalparamsK.SIZE_VERTEXPTRS<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_SRCVERTICESDATA: "<<globalparams.globalparamsK.SIZE_SRCVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsK.SIZE_DESTVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_ACTIVEVERTICES: "<<globalparams.globalparamsK.SIZE_ACTIVEVERTICES<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_EDGESMAP: "<<globalparams.globalparamsK.SIZE_EDGESMAP<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK: "<<globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_KVSTATSDRAM: "<<globalparams.globalparamsK.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_EDGESSTATSDRAM: "<<globalparams.globalparamsK.SIZE_EDGESSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_KVDRAM: "<<globalparams.globalparamsK.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE: "<<globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid universalparams.PADDEDKVSOURCEDRAMSZ: "<<((((unsigned long)globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (HBM max="<<(unsigned int)(universalparams.VOLUMESIZEU32_WORKSPACEDATA * sizeof(unsigned int))<<" bytes)"<< RESET <<std::endl;					
	if(((((unsigned long)globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + (unsigned long)globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t)) > (universalparams.VOLUMESIZEU32_WORKSPACEDATA * sizeof(unsigned int))){ cout<<"ERROR: dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
	if(globalparams.globalparamsK.SIZE_KVDRAM < 1000){ cout<<"ERROR: globalparams.globalparamsK.SIZE_KVDRAM("<<globalparams.globalparamsK.SIZE_KVDRAM<<") < 1000. EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"[globalparams.globalparamsE.BASEOFFSET_MESSAGESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA: "<<globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.VOLUMEOFFSETKVS_WORKDATA: "<<globalparams.globalparamsE.VOLUMEOFFSETKVS_WORKDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.VOLUMEOFFSETKVS_WORKSPACEDATA: "<<globalparams.globalparamsE.VOLUMEOFFSETKVS_WORKSPACEDATA<<"]"<<endl;
	
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
	
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP<<"]"<<endl;
	
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP: "<<globalparams.globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP<<"]"<<endl;
	
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
	cout<<"[globalparams.globalparamsE.VOLUMESIZEU32_WORKDATA: "<<globalparams.globalparamsE.VOLUMESIZEU32_WORKDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.VOLUMESIZEU32_WORKSPACEDATA: "<<globalparams.globalparamsE.VOLUMESIZEU32_WORKSPACEDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_EDGES: "<<globalparams.globalparamsE.SIZE_EDGES<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_VERTEXPTRS: "<<globalparams.globalparamsE.SIZE_VERTEXPTRS<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_SRCVERTICESDATA: "<<globalparams.globalparamsE.SIZE_SRCVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_DESTVERTICESDATA: "<<globalparams.globalparamsE.SIZE_DESTVERTICESDATA<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_ACTIVEVERTICES: "<<globalparams.globalparamsE.SIZE_ACTIVEVERTICES<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_EDGESMAP: "<<globalparams.globalparamsE.SIZE_EDGESMAP<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK: "<<globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_KVSTATSDRAM: "<<globalparams.globalparamsE.SIZE_KVSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_EDGESSTATSDRAM: "<<globalparams.globalparamsE.SIZE_EDGESSTATSDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_KVDRAM: "<<globalparams.globalparamsE.SIZE_KVDRAM<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE: "<<globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE<<"]"<<endl;
	cout<<"[globalparams.globalparamsE.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparams.globalparamsE.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<"]"<<endl;
	
	std::cout<< TIMINGRESULTSCOLOR << ">> host[bytes]:: valid universalparams.PADDEDKVSOURCEDRAMSZ: "<<((((unsigned long)globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t))<<" bytes. (HBM max="<<(unsigned int)(universalparams.VOLUMESIZEU32_WORKDATA * sizeof(unsigned int))<<" bytes)"<< RESET <<std::endl;					
	if(((((unsigned long)globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) + (unsigned long)globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE) * sizeof(keyvalue_t)) > (universalparams.VOLUMESIZEU32_WORKDATA * sizeof(unsigned int))){ cout<<"ERROR: dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadgraph::loadmessages(A)", (globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR2_SIZE) + globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE, universalparams.MAXHBMCAPACITY_KVS2 * VECTOR2_SIZE, NAp, NAp, NAp);
	utilityobj->checkoutofbounds("loadgraph::loadmessages(B)", (globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR2_SIZE) + globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE, universalparams.PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	utilityobj->checkoutofbounds("loadgraph::loadmessages(C)", (globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE * VECTOR2_SIZE) + globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE, universalparams.PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	#endif
	
	if(universalparams.NUMREDUCEPARTITIONS == 0){ cout<<"loadgraph::loadmessages: ERROR: NUMREDUCEPARTITIONS("<<universalparams.NUMREDUCEPARTITIONS<<") == 0. EXITING..."<<endl; exit(EXIT_FAILURE); }

	createmessages(
			0,
			vdram, // uint512_vec_dt * kvstats,
			container->srcvoffset[0], // srcvoffset,
			container->srcvsize[0], // srcvsize,
			container->edgessize[0], // edgessize,
			container->destvoffset[0], // destvoffset,
			container->actvvsize[0], // actvvsize,
			container->runsize[0], // runsize
			universalparams.NUMPROCESSEDGESPARTITIONS, // number of process partitions
			universalparams.NUMREDUCEPARTITIONS, // number of reduce partitions
			globalparams.globalparamsV, // NEWCHANGE.
			universalparams
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
			0, // // runsize
			0, // number of process partitions
			0, // number of reduce partitions
			globalparams.globalparamsM, // NEWCHANGE.
			universalparams
			); // runsize
	#endif 
	
	for(unsigned int i = 0; i < NUM_PEs; i++){
		createmessages(
			i,
			edges[i], // uint512_vec_dt * kvstats,
			0, // srcvoffset,
			0, // srcvsize,
			container->edgessize[i], // edgessize,
			0, // destvoffset,
			0, // actvvsize,
			0, // run size,
			universalparams.NUMPROCESSEDGESPARTITIONS, // number of process partitions
			universalparams.NUMREDUCEPARTITIONS, // number of reduce partitions
			globalparams.globalparamsE,
			universalparams); // runsize
		createmessages(
			i,
			kvbuffer[i], // uint512_vec_dt * kvstats,
			container->srcvoffset[i], // srcvoffset,
			container->srcvsize[i], // srcvsize,
			0, // container->edgessize[i], // edgessize,
			container->destvoffset[i], // destvoffset,
			container->actvvsize[i], // actvvsize,
			container->runsize[i], // runsize
			universalparams.NUMPROCESSEDGESPARTITIONS, // number of process partitions
			universalparams.NUMREDUCEPARTITIONS, // number of reduce partitions
			globalparams.globalparamsK,
			universalparams); // runsize
		
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"loadmessages:: running Acts... size: "<<container->edgessize[i]<<endl; 
		#endif 
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	unsigned int totalsize = 0;
	cout<<"loadmessages:: loading Acts... sizes: ["; 
	cout<<"["; for(unsigned int i = 0; i < NUM_PEs; i++){ totalsize += container->runsize[i]; cout<<container->runsize[i]; if(i<NUM_PEs-1){ cout<<", "; }} cout<<"]";
	cout<<"]"<<endl;
	cout<<"loadmessages:: total sizes: "<<totalsize<<endl; 
	cout<<"loadmessages:: _NUMREDUCEPARTITIONS: "<<kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_NUM_REDUCEPARTITIONS].data[0].key<<endl; 
	cout<<"loadmessages:: universalparams.NUMPROCESSEDGESPARTITIONS: "<<kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_NUM_PROCESSEDGESPARTITIONS].data[0].key<<endl; 
	cout<<"loadmessages:: _ACTSPARAMS_NUMLOPS: "<<kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key<<endl; 
	cout<<"loadmessages:: _ACTSPARAMS_TREEDEPTH: "<<kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
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
			unsigned int runsize,
			unsigned int numprocesspartitions, 
			unsigned int numreducepartitions, 
			globalparams_t globalparams,
			universalparams_t universalparams){
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key = ON;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key = ON;//ON,OFF // CRITICAL REMOVEME.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = ON;//ON,OFF  // CRITICAL REMOVEME
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = ON;//ON,OFF 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_MERGECOMMAND].data[0].key = ON;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_EXCHANGECOMMAND].data[0].key = ON; // ON,OFF
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_GPMODE].data[0].key = 
		#ifdef CONFIG_ALGORITHMTYPE_RANDOMACTIVEVERTICES
		PARAM_ACTSONLYMODE;
		// PARAM_HYBRIDGPMODE;
		// PARAM_TRADGPONLYMODE
		#else 
		PARAM_ACTSONLYMODE;
		#endif 
		;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES].data[0].key = 0;;
		
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
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_EVALUATION_ACTS_VERYBARE].data[0].key =
		#ifdef CONFIG_ACTS_VERYBARE
		ON;
		#else 
		OFF;	
		#endif	
		
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD].data[0].key = CONFIG_HYBRIDGPMODE_HYBRIDVTHRESHOLD;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION].data[0].key = CONFIG_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION; // 0;
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].data[0].key  = globalparams.BASEOFFSETKVS_MESSAGESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_VOLUMEOFFSETKVS_WORKDATA].data[0].key = universalparams.VOLUMEOFFSETKVS_WORKDATA; // globalparams.VOLUMESIZEU32_WORKDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_VOLUMEOFFSETKVS_WORKSPACEDATA].data[0].key = universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA; // globalparams.VOLUMESIZEU32_WORKSPACEDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key = globalparams.BASEOFFSETKVS_EDGESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key = globalparams.BASEOFFSETKVS_VERTEXPTR;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA].data[0].key = globalparams.BASEOFFSETKVS_SRCVERTICESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key = globalparams.BASEOFFSETKVS_DESTVERTICESDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key = globalparams.BASEOFFSETKVS_ACTIVEVERTICES;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESMAP].data[0].key = globalparams.BASEOFFSETKVS_EDGESMAP;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGEBLOCKMAP].data[0].key = globalparams.BASEOFFSETKVS_EDGEBLOCKMAP;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key = globalparams.BASEOFFSETKVS_STATSDRAM;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESSTATSDRAM].data[0].key = globalparams.BASEOFFSETKVS_EDGESSTATSDRAM;//
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key = globalparams.BASEOFFSETKVS_KVDRAM;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSMESSAGE_OTHERINFOS].data[0].key = globalparams.BASEOFFSETKVS_OTHERINFOS;
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BUFFERBASEOFFSETKVS_VDATA1].data[0].key  = 0; // NEWCHANGE.
	if(universalparams.ALGORITHM == CF){ kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BUFFERBASEOFFSETKVS_VDATA2].data[0].key = MAX_BLOCKRAM_VDESTDATA_SIZE / 2; } // universalparams.RED_SRAMSZ; } 
		else { kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BUFFERBASEOFFSETKVS_VDATA2].data[0].key  = 0; }
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BUFFERBASEOFFSETKVS_VDATA3].data[0].key  = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BUFFERBASEOFFSETKVS_VDATA4].data[0].key  = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BUFFERBASEOFFSETKVS_VDATA5].data[0].key  = NAp;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].data[0].key = MESSAGESDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_VOLUMESIZEU32_WORKDATA].data[0].key = universalparams.VOLUMESIZEU32_WORKDATA; // globalparams.VOLUMESIZEU32_WORKDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_VOLUMESIZEU32_WORKSPACEDATA].data[0].key  = universalparams.VOLUMESIZEU32_WORKSPACEDATA; // globalparams.VOLUMESIZEU32_WORKDATA;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].data[0].key = globalparams.SIZE_EDGES; // edgessize; // NEWCHANGE***
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].data[0].key = globalparams.SIZE_VERTEXPTRS; // srcvsize; // NEWCHANGE***
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_SRCVERTICESDATA].data[0].key = globalparams.SIZE_SRCVERTICESDATA; // SRCVERTICESDATASZ; // NEWCHANGE.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_DESTVERTICESDATA].data[0].key = globalparams.SIZE_DESTVERTICESDATA; // DESTVERTICESDATASZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].data[0].key = actvvsize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGESMAP].data[0].key = globalparams.SIZE_EDGESMAP;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGEBLOCKMAP].data[0].key = globalparams.SIZE_EDGEBLOCKMAP;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESPARTITIONMASK].data[0].key = globalparams.SIZE_VERTICESPARTITIONMASK; // CRITICAL FIXME.
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].data[0].key = globalparams.SIZE_KVSTATSDRAM; // kvstatssz; // NEWCHANGE
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGESSTATSDRAM].data[0].key = EDGESSTATSDRAMSZ; //64; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].data[0].key = globalparams.SIZE_KVDRAM; // KVDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key = globalparams.SIZE_KVDRAMWORKSPACE; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_OTHERINFOS].data[0].key = globalparams.SIZE_OTHERINFOS; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZEKVS_PROCESSEDGESPARTITION].data[0].key = universalparams.PROCESSPARTITIONSZ_KVS2; //  / 2; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_REDUCE].data[0].key = NAp; // OBSOLETE //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZEKVS_REDUCEPARTITION].data[0].key = universalparams.REDUCEPARTITIONSZ_KVS2; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZEKVS_VMASKBUFFER].data[0].key = NAp; // OBSOLETE 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_BATCHRANGE].data[0].key = universalparams.BATCH_RANGE; //
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
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_REDUCE].data[0].key = universalparams.REDUCESZ_POW; // APPLYVERTEXBUFFERSZ_POW; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_BATCHRANGE].data[0].key = universalparams.BATCH_RANGE_POW; // _BATCH_RANGE_POW; //
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_NUM_REDUCEPARTITIONS].data[0].key = numreducepartitions; // _NUMREDUCEPARTITIONS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_NUM_PROCESSEDGESPARTITIONS].data[0].key = numprocesspartitions; //universalparams.NUMPROCESSEDGESPARTITIONS;
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].data[0].key = universalparams.ALGORITHM;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].data[0].key = 1;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key = universalparams.TREE_DEPTH; // _FINAL_TREE_DEPTH;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key = universalparams.TREE_DEPTH; // _FINAL_TREE_DEPTH;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key = universalparams.NUMLASTLEVELPARTITIONS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key = srcvoffset;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key = srcvsize;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key = destvoffset;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER].data[0].key = globalparams.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; // CRITICAL REMOVEME!
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_INSTID].data[0].key = id; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUM_EDGE_BANKS].data[0].key = universalparams.NUM_EDGE_BANKS; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM].data[0].key = universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM;	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUM_PARTITIONS].data[0].key = universalparams.NUM_PARTITIONS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP1].data[0].key = universalparams.NUM_PARTITIONS; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP2].data[0].key = universalparams.NUM_PARTITIONS; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP3].data[0].key = universalparams.NUM_PARTITIONS; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_POW_PARTITIONS].data[0].key = universalparams.NUM_PARTITIONS_POW; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP1].data[0].key = universalparams.NUM_PARTITIONS_POW; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP2].data[0].key = universalparams.NUM_PARTITIONS_POW; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP3].data[0].key = universalparams.NUM_PARTITIONS_POW; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_WORKBUFFER_SIZE].data[0].key = universalparams.WORKBUFFER_SIZE; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS].data[0].key = universalparams.MAXHBMCAPACITY_KVS2; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_VPTR_SHRINK_RATIO].data[0].key = universalparams.VPTR_SHRINK_RATIO; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_READOFFLINESTATS].data[0].key = 0;//1; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_UNUSED4].data[0].key = NAp; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR2].data[0].key = NUMCOMPUTEUNITS_SLR2; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR1].data[0].key = NUMCOMPUTEUNITS_SLR1; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR0].data[0].key = NUMCOMPUTEUNITS_SLR0; 

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_DUMMYCHKPOINT].data[0].key = 70707070; 
	for(unsigned int m=0; m<MESSAGES_RETURNVALUES_SIZE; m++){ kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + m].data[0].key = 0; } 
	// exit(EXIT_SUCCESS); //
	return globalparams;
}

void loadgraph::setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[MAXNUM_PEs], unsigned int evalid){
	evalparamsobj->loadevalparams(evalid, vdram);
	for(unsigned int i = 0; i < NUM_PEs; i++){ 
		evalparamsobj->loadevalparams(evalid, kvbuffer[i]);
	}
	return;
}

globalparams_t loadgraph::finishglobaparamsV(globalparams_t globalparams, universalparams_t universalparams){
	// order of base addresses
	// messages area {messages}
	// edges area {edges, vertex ptrs} 
	// vertices area {src vertices data, dest vertices data}
	// actvvs area {active vertices, active vertices mask}
	// stats area {stats, edge stats}
	// workspace area {kvdram, kvdram workspace}
	
	// globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + (globalparams.SIZE_VERTICESPARTITIONMASK / VECTOR_SIZE);
	globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + globalparams.SIZE_VERTICESPARTITIONMASK;
	globalparams.SIZE_KVSTATSDRAM = universalparams.NUMREDUCEPARTITIONS * NUM_PEs * 2; // '2' is padding 
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

globalparams_t loadgraph::finishglobaparamsM(globalparams_t globalparams, universalparams_t universalparams){
	// order of base addresses
	// messages area {messages}
	// edges area {edges, vertex ptrs} 
	// vertices area {src vertices data, dest vertices data}
	// actvvs area {active vertices, active vertices mask}
	// stats area {stats, edge stats}
	// workspace area {kvdram, kvdram workspace}

	// globalparams.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	// globalparams.SIZE_ACTIVEVERTICES = 0; 
	globalparams.BASEOFFSETKVS_EDGESMAP = globalparams.BASEOFFSETKVS_ACTIVEVERTICES + (globalparams.SIZE_ACTIVEVERTICES / VECTOR2_SIZE);
	globalparams.SIZE_EDGESMAP = 0; 
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.BASEOFFSETKVS_EDGESMAP + (globalparams.SIZE_EDGESMAP / VECTOR_SIZE);
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