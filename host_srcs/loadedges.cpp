#include "loadedges.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define LOADSLICEDEDGES_CORRECT	
#define CALCULATELOCALDSTVIDS
#define INSERTBITMAP
#define LOADEDGES
#define CALCULATEOFFSETS

#define LOADEDGES_ENSUREPERFECTACCURACY

#define TWOO 2

// #define MYVDATA_SHRINK_FACTOR VDATA_SHRINK_RATIO
#define MYVDATA_SHRINK_FACTOR 1

bool debugb = false;//false;
bool debug2b = false; //true;//false;
bool debug3b = false;

loadedges::loadedges(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	evalparamsobj = new evalparams();
	universalparams = _universalparams;
}
loadedges::~loadedges(){} 

unsigned int loadedges::GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int loadedges::READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
void loadedges::WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
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

unsigned int loadedges::allignlower(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = val / V_SIZE;
	// cout<<"fac: "<<fac<<", val: "<<val<<", V_SIZE: "<<V_SIZE<<", (fac * V_SIZE): "<<(fac * V_SIZE)<<endl;
	return (fac * V_SIZE);
}
unsigned int loadedges::allignhigher(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = (val + (V_SIZE-1)) / V_SIZE;
	return (fac * V_SIZE);
}

unsigned int loadedges::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int loadedges::getlocalvid(unsigned int vid){
	unsigned int s = gethash(vid);
	return (vid - s) / NUM_PEs; 
}

unsigned int loadedges::getpartition(unsigned int data, vertex_t upperlimit){
	// unsigned int partition = (data - upperlimit) / ((universalparams.WORKBUFFER_SIZE*VECTOR2_SIZE) / universalparams.NUM_PARTITIONS);
	unsigned int partition = (data - upperlimit) % VECTOR2_SIZE;
	// cout<<"loadedges::getpartition::. partition out of bounds partition: "<<partition<<", data: "<<data<<", upperlimit: "<<upperlimit<<", _ACTS_READEDGEGRANULARITY: "<<_ACTS_READEDGEGRANULARITY<<". "<<endl; 
	if(partition >= universalparams.NUM_PARTITIONS){ 
		cout<<"loadedges::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", data: "<<data<<", upperlimit: "<<upperlimit<<", _ACTS_READEDGEGRANULARITY: "<<universalparams.WORKBUFFER_SIZE * VECTOR2_SIZE<<". EXITING... "<<endl; 
		exit(EXIT_FAILURE); 
	}
	return partition;
}
void loadedges::calculateoffsets(keyvalue_t * buffer, unsigned int size){
	buffer[0].key = 0;
	for(buffer_type i=1; i<size; i++){ 
		// buffer[i].key = allignlower(buffer[i-1].key + buffer[i-1].value, VECTOR2_SIZE); 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value; 
	}
	return;
}
void loadedges::getXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE]){
	unsigned int s_ = s % VECTOR_SIZE;
	
	if(s_==0){ 
		depths[0] = 0; 
		depths[1] = 1; 
		depths[2] = 2; 
		depths[3] = 3; 
		depths[4] = 4; 
		depths[5] = 5; 
		depths[6] = 6; 
		depths[7] = 7; 
	} else if(s_==1){ 
		depths[0] = 7; 
		depths[1] = 0; 
		depths[2] = 1; 
		depths[3] = 2; 
		depths[4] = 3; 
		depths[5] = 4; 
		depths[6] = 5; 
		depths[7] = 6; 
	} else if(s_==2){ 
		depths[0] = 6; 
		depths[1] = 7; 
		depths[2] = 0; 
		depths[3] = 1; 
		depths[4] = 2; 
		depths[5] = 3; 
		depths[6] = 4; 
		depths[7] = 5; 
	} else if(s_==3){ 
		depths[0] = 5; 
		depths[1] = 6; 
		depths[2] = 7; 
		depths[3] = 0; 
		depths[4] = 1; 
		depths[5] = 2; 
		depths[6] = 3; 
		depths[7] = 4; 
	} else if(s_==4){ 
		depths[0] = 4; 
		depths[1] = 5; 
		depths[2] = 6; 
		depths[3] = 7; 
		depths[4] = 0; 
		depths[5] = 1; 
		depths[6] = 2; 
		depths[7] = 3; 
	} else if(s_==5){ 
		depths[0] = 3; 
		depths[1] = 4; 
		depths[2] = 5; 
		depths[3] = 6; 
		depths[4] = 7; 
		depths[5] = 0; 
		depths[6] = 1; 
		depths[7] = 2; 
	} else if(s_==6){ 
		depths[0] = 2; 
		depths[1] = 3; 
		depths[2] = 4; 
		depths[3] = 5; 
		depths[4] = 6; 
		depths[5] = 7; 
		depths[6] = 0; 
		depths[7] = 1; 
	} else { 
		depths[0] = 1; 
		depths[1] = 2; 
		depths[2] = 3; 
		depths[3] = 4; 
		depths[4] = 5; 
		depths[5] = 6; 
		depths[6] = 7; 
		depths[7] = 0; 
	}
	return;
}
edge2_vec_dt loadedges::rearrangeLayoutV(unsigned int s, edge2_vec_dt edge_vec){
	edge2_vec_dt edge_vec2;
	unsigned int s_ = s;// % VECTOR_SIZE;
	if(s_==0){ 
		edge_vec2.data[0] = edge_vec.data[0]; 
		edge_vec2.data[1] = edge_vec.data[1]; 
		edge_vec2.data[2] = edge_vec.data[2]; 
		edge_vec2.data[3] = edge_vec.data[3]; 
		edge_vec2.data[4] = edge_vec.data[4]; 
		edge_vec2.data[5] = edge_vec.data[5]; 
		edge_vec2.data[6] = edge_vec.data[6]; 
		edge_vec2.data[7] = edge_vec.data[7]; 
	} else if(s_==1){ 
		edge_vec2.data[7] = edge_vec.data[0]; 
		edge_vec2.data[0] = edge_vec.data[1]; 
		edge_vec2.data[1] = edge_vec.data[2]; 
		edge_vec2.data[2] = edge_vec.data[3]; 
		edge_vec2.data[3] = edge_vec.data[4]; 
		edge_vec2.data[4] = edge_vec.data[5]; 
		edge_vec2.data[5] = edge_vec.data[6]; 
		edge_vec2.data[6] = edge_vec.data[7]; 
	} else if(s_==2){ 
		edge_vec2.data[6] = edge_vec.data[0]; 
		edge_vec2.data[7] = edge_vec.data[1]; 
		edge_vec2.data[0] = edge_vec.data[2]; 
		edge_vec2.data[1] = edge_vec.data[3]; 
		edge_vec2.data[2] = edge_vec.data[4]; 
		edge_vec2.data[3] = edge_vec.data[5]; 
		edge_vec2.data[4] = edge_vec.data[6]; 
		edge_vec2.data[5] = edge_vec.data[7]; 
	} else if(s_==3){ 
		edge_vec2.data[5] = edge_vec.data[0]; 
		edge_vec2.data[6] = edge_vec.data[1]; 
		edge_vec2.data[7] = edge_vec.data[2]; 
		edge_vec2.data[0] = edge_vec.data[3]; 
		edge_vec2.data[1] = edge_vec.data[4]; 
		edge_vec2.data[2] = edge_vec.data[5]; 
		edge_vec2.data[3] = edge_vec.data[6]; 
		edge_vec2.data[4] = edge_vec.data[7]; 
	} else if(s_==4){ 
		edge_vec2.data[4] = edge_vec.data[0]; 
		edge_vec2.data[5] = edge_vec.data[1]; 
		edge_vec2.data[6] = edge_vec.data[2]; 
		edge_vec2.data[7] = edge_vec.data[3]; 
		edge_vec2.data[0] = edge_vec.data[4]; 
		edge_vec2.data[1] = edge_vec.data[5]; 
		edge_vec2.data[2] = edge_vec.data[6]; 
		edge_vec2.data[3] = edge_vec.data[7]; 
	} else if(s_==5){ 
		edge_vec2.data[3] = edge_vec.data[0]; 
		edge_vec2.data[4] = edge_vec.data[1]; 
		edge_vec2.data[5] = edge_vec.data[2]; 
		edge_vec2.data[6] = edge_vec.data[3]; 
		edge_vec2.data[7] = edge_vec.data[4]; 
		edge_vec2.data[0] = edge_vec.data[5]; 
		edge_vec2.data[1] = edge_vec.data[6]; 
		edge_vec2.data[2] = edge_vec.data[7]; 
	} else if(s_==6){ 
		edge_vec2.data[2] = edge_vec.data[0]; 
		edge_vec2.data[3] = edge_vec.data[1]; 
		edge_vec2.data[4] = edge_vec.data[2]; 
		edge_vec2.data[5] = edge_vec.data[3]; 
		edge_vec2.data[6] = edge_vec.data[4]; 
		edge_vec2.data[7] = edge_vec.data[5]; 
		edge_vec2.data[0] = edge_vec.data[6]; 
		edge_vec2.data[1] = edge_vec.data[7]; 
	} else { 
		edge_vec2.data[1] = edge_vec.data[0]; 
		edge_vec2.data[2] = edge_vec.data[1]; 
		edge_vec2.data[3] = edge_vec.data[2]; 
		edge_vec2.data[4] = edge_vec.data[3]; 
		edge_vec2.data[5] = edge_vec.data[4]; 
		edge_vec2.data[6] = edge_vec.data[5]; 
		edge_vec2.data[7] = edge_vec.data[6]; 
		edge_vec2.data[0] = edge_vec.data[7]; 
	}
	
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"$$$["<<edge_vec2.data[v].srcvid<<", "<<edge_vec2.data[v].dstvid<<"("<<(edge_vec2.data[v].dstvid % VECTOR_SIZE)<<")], "; } cout<<endl; 
					
	return edge_vec2;
}

globalparams_TWOt loadedges::start(unsigned int col, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, vptr_type * vptrs[MAXNUM_PEs], uint512_vec_dt * edges[MAXNUM_PEs], vector<edge2_vec_dt> (&edges_final)[MAXNUM_PEs], map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs], container_t * container, globalparams_TWOt globalparams){						
	cout<<"loadedges::start:: loading edges... "<<endl;
	
	unsigned int kvdata_range__div__vptr_shrink_ratio = (universalparams.KVDATA_RANGE + (universalparams.VPTR_SHRINK_RATIO-1)) / universalparams.VPTR_SHRINK_RATIO;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsK.SIZE_MESSAGESDRAM;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsE.SIZE_MESSAGESDRAM; 
	
	unsigned int num_vPs = universalparams.NUMPROCESSEDGESPARTITIONS;
	unsigned int vsize_vP = universalparams.PROCESSPARTITIONSZ;
	unsigned int num_LLPs = 1 << (OPT_NUM_PARTITIONS_POW * universalparams.TREE_DEPTH);
	unsigned int vsize_LLP = 1 << (universalparams.BATCH_RANGE_POW - (OPT_NUM_PARTITIONS_POW * universalparams.TREE_DEPTH));
	
	unsigned int _NUM_PEs = NUM_PEs;
	unsigned int _num_vPs = num_vPs; // CRIICAL FIXME.
	unsigned int _num_LLPs = num_LLPs;
	
	vector<edge2_type> edges_in_channel[MAXNUM_PEs];
	vector<edge2_type> edgesin_srcvp[MAXNUM_VPs];
	vector<edge2_type> edgesin_srcvp_lldstvp[num_LLPs];
	// vector<edge2_type> edgesin_srcvp_lldstvp_srcv2p[num_LLPs][universalparams.NUM_PARTITIONS];
	vector<edge2_type> edgesin_srcvp_lldstvp_srcv2p[num_LLPs][VECTOR_SIZE];
	unsigned int edge_count_in_vpartition[MAXNUM_PEs][MAXNUM_VPs];
	
	for(unsigned int i=0; i<_NUM_PEs; i++){ edges_final[i].clear(); }
	for(unsigned int i=0; i<_NUM_PEs; i++){ for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ edges_map[i][v_p][ll_p].offset = 0; edges_map[i][v_p][ll_p].size = 0; }}}
	for(unsigned int i=0; i<_NUM_PEs; i++){ for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ edge_count_in_vpartition[i][v_p] = 0; }}
		
	cout<<"loadedges:: [num_LLPs: "<<num_LLPs<<", vsize_LLP: "<<vsize_LLP<<", num_vPs: "<<num_vPs<<", vsize_vP: "<<vsize_vP<<", INVALIDDATA: "<<INVALIDDATA<<"] "<<endl;
	
	// partiition into HBM channels 
	cout<<"STAGE 0: partiition into HBM channels "<<endl;
	for(unsigned int vid=0; vid<utilityobj->hmin(universalparams.NUM_VERTICES, universalparams.KVDATA_RANGE)-1; vid++){
		#ifdef _DEBUGMODE_HOSTCHECKS3
		if(vid % 1000000 == 0){ cout<<"### loadedges::start:: vid: "<<vid<<" (of "<<universalparams.NUM_VERTICES<<" vertices), vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(19)::", vid, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
		utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(19b)::", vid+1, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; cout<<">>> loadedges:: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). EXITING... (vid: "<<vid<<", edges_size: "<<edges_size<<")"<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int i=0; i<edges_size; i++){
			edge2_type edge = edgedatabuffer[vptr_begin + i];
			#ifdef _DEBUGMODE_HOSTCHECKS3
			if(edge.srcvid >= universalparams.KVDATA_RANGE || edge.dstvid >= universalparams.KVDATA_RANGE){ continue; } // edge.dstvid = edge.dstvid % universalparams.KVDATA_RANGE; } // CRIICAL FIXME.
			#endif 
			
			unsigned int H = gethash(edge.dstvid);
			edge.dstvid = getlocalvid(edge.dstvid); // hash it.
			edges_in_channel[H].push_back(edge);
		}
	}
	
	bool debug = true; // true; // false;
	bool debug_detail = false;
	cout<<"STAGE 1: preparing edges..."<<endl;
	for(unsigned int i=0; i<_NUM_PEs; i++){
		cout<<"loadedges:: [PE: "<<i<<"]"<<endl;
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		unsigned int index = 0;
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ edgesin_srcvp[v_p].clear(); } // clear 
		
		// within a HBM channel, partition into v-partitions 
		if(debug){ cout<<"STAGE 1: within a HBM channel, partition into v-partitions "<<endl; }
		for(unsigned int t=0; t<edges_in_channel[i].size(); t++){
			edge2_type edge = edges_in_channel[i][t];
			unsigned int vP = (edge.srcvid / vsize_vP);
			#ifdef _DEBUGMODE_HOSTCHECKS3
			utilityobj->checkoutofbounds("loadedges::ERROR 22::", vP, num_vPs, edge.srcvid, edge.srcvid, vsize_vP);
			#endif 
		
			edgesin_srcvp[vP].push_back(edge);
		}
		
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ edgesin_srcvp_lldstvp[ll_p].clear(); } // clear
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ edgesin_srcvp_lldstvp_srcv2p[ll_p][p].clear(); }} // clear
		
			// within a v-partition, partition into last-level-partitions (LLPs)
			/* //////////////////////////////////////////////////////////////////
			if(debug){ cout<<"STAGE 2: within a v-partition, partition into last-level-partitions (LLPs)"<<endl; }
			for(unsigned int t=0; t<edgesin_srcvp[v_p].size(); t++){
				edge2_type edge = edgesin_srcvp[v_p][t];
				unsigned int ll_p = (edge.dstvid / vsize_LLP);
				#ifdef _DEBUGMODE_HOSTCHECKS3
				utilityobj->checkoutofbounds("loadedges::ERROR 23::", ll_p, num_LLPs, edge.dstvid, edge.dstvid, vsize_LLP);
				#endif 
			
				edgesin_srcvp_lldstvp[ll_p].push_back(edge);
			}
			////////////////////////////////////////////////////////////////// */
			// edgesin_srcvp_lldstvp
			if(debug){ cout<<"STAGE 2: within a v-partition, partition into last-level-partitions (LLPs)"<<endl; }
			// [[0,16,...][1,17...][2,18,...]...[15,31,...]]...[[0,16,...][1,17,...][2,18,...]...[15,31,...]]
			for(unsigned int t=0; t<edgesin_srcvp[v_p].size(); t++){
				edge2_type edge = edgesin_srcvp[v_p][t];
				// edge.dstvid = 294913; // CRIICAL REMOVEME.
				// unsigned int ll_p = (edge.dstvid / 16384); // CRIICAL REMOVEME.
				
				unsigned int ll_p = (edge.dstvid / vsize_LLP);
				unsigned int newll_p = ((ll_p / universalparams.NUM_PARTITIONS) * universalparams.NUM_PARTITIONS) + (edge.dstvid % universalparams.NUM_PARTITIONS);
				#ifdef _DEBUGMODE_HOSTCHECKS3
				utilityobj->checkoutofbounds("loadedges::ERROR 23::", newll_p, num_LLPs, edge.dstvid, edge.dstvid, vsize_LLP);
				#endif 
				
				// cout<<"--- edge.dstvid: "<<edge.dstvid<<", vsize_LLP: "<<vsize_LLP<<", ll_p: "<<ll_p<<", newll_p: "<<newll_p<<", universalparams.NUM_PARTITIONS: "<<universalparams.NUM_PARTITIONS<<endl;
				edgesin_srcvp_lldstvp[newll_p].push_back(edge);
				// exit(EXIT_SUCCESS); ////
			}
			if(true){ cout<<"loadedges[STAGE 2 check]:: {srcvid, dstvid}"<<endl;
			for(unsigned int ll_p=0; ll_p<num_LLPs; ll_p++){ cout<<"["<<edgesin_srcvp_lldstvp[ll_p][7].srcvid<<", "<<edgesin_srcvp_lldstvp[ll_p][7].dstvid<<"("<<(edgesin_srcvp_lldstvp[ll_p][7].dstvid % universalparams.NUM_PARTITIONS)<<")], "; } cout<<endl; }
			// exit(EXIT_SUCCESS);
			//////////////////////////////////////////////////////////////////
			
			// witihin a LLP, re-arrange by hash of srcvids 
			/* if(debug){ cout<<"STAGE 3: witihin a LLP, re-arrange by srcvids "<<endl; }
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){
				for(unsigned int t=0; t<edgesin_srcvp_lldstvp[ll_p].size(); t++){
					edge2_type edge = edgesin_srcvp_lldstvp[ll_p][t];
					edgesin_srcvp_lldstvp_srcv2p[ll_p][edge.srcvid % universalparams.NUM_PARTITIONS].push_back(edge);	
				}
			} */
			if(debug){ cout<<"STAGE 3: witihin a LLP, re-arrange by srcvids "<<endl; }
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){
				for(unsigned int t=0; t<edgesin_srcvp_lldstvp[ll_p].size(); t++){
					edge2_type edge = edgesin_srcvp_lldstvp[ll_p][t];
					edgesin_srcvp_lldstvp_srcv2p[ll_p][edge.srcvid % VECTOR_SIZE].push_back(edge);	
				}
			}
			
			// witihin a LLP, re-arrange by srcvids 
			// finally, place edges in dram
			// vector<edge2_type> edgesin_srcvp_lldstvp_srcv2p[num_LLPs][VECTOR_SIZE];
			if(debug){ cout<<"STAGE 4: preparing edges and loading into dram..."<<endl; }
			edge2_type dummy_edge; dummy_edge.srcvid = INVALIDDATA; dummy_edge.dstvid = INVALIDDATA; 
			unsigned int depths[VECTOR_SIZE];
			for(unsigned int llp_set=0; llp_set<universalparams.NUM_PARTITIONS / VECTOR_SIZE; llp_set++){ // ll_p set 
				unsigned int llp_offset = llp_set * VECTOR_SIZE;
				for(unsigned int llp_id=0; llp_id<VECTOR_SIZE; llp_id++){ // ll_p
					// get layout
					getXYLayoutV(llp_id, depths);
					unsigned int max_kvs = 0; for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(edgesin_srcvp_lldstvp_srcv2p[llp_offset + v][depths[v]].size() > max_kvs){ max_kvs = edgesin_srcvp_lldstvp_srcv2p[llp_offset + v][depths[v]].size(); }}
					// for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"loadedges:: max_kvs: "<<max_kvs<<" depths["<<v<<"]: "<<depths[v]<<endl; }
					
					for(buffer_type t=0; t<max_kvs; t++){
						
						edge2_vec_dt edge_vec;
						for(unsigned int v=0; v<VECTOR_SIZE; v++){
							if(t < edgesin_srcvp_lldstvp_srcv2p[llp_offset + v][depths[v]].size()){ edge_vec.data[v] = edgesin_srcvp_lldstvp_srcv2p[llp_offset + v][depths[v]][t]; } else { edge_vec.data[v] = dummy_edge; }
						}
						
						// for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"^["<<edge_vec.data[v].srcvid<<", "<<edge_vec.data[v].dstvid<<"("<<(edge_vec.data[v].dstvid % VECTOR_SIZE)<<")], "; } cout<<endl; 
						
						// re-arrange
						edge2_vec_dt edge_vec2 = rearrangeLayoutV(llp_offset + llp_id, edge_vec);
						
						// cout<<"loadedges:: ~~~~~~~~~~~~~~~~ llp_id: "<<llp_id<<endl;
						// if(i==0 && llp_id==0 && t< 16){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"#["<<edge_vec2.data[v].srcvid<<", "<<edge_vec2.data[v].dstvid<<"("<<(edge_vec2.data[v].dstvid % VECTOR_SIZE)<<")], "; } cout<<endl; }
						// exit(EXIT_SUCCESS);
						
						edge2_vec_dt edge_vec3 = edge_vec2;
						for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(edge_vec2.data[v].dstvid != INVALIDDATA){ edge_vec3.data[v].srcvid = (edge_vec2.data[v].srcvid - v) / universalparams.NUM_PARTITIONS; }}
						 
						for(unsigned int v=0; v<VECTOR_SIZE; v++){
							utilityobj->checkoutofbounds("loadedges::ERROR 56::", globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index, universalparams.MAXHBMCAPACITY_KVS2 * VECTOR_SIZE, globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, NAp);	
							edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v].key = edge_vec3.data[v].dstvid; 
							edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v].value = edge_vec3.data[v].srcvid;
						}
						index+=1;
						edges_final[i].push_back(edge_vec2); 
						
						edges_map[i][v_p][0].size += 2 * VECTOR_SIZE;
						edge_count_in_vpartition[i][v_p] += 2 * VECTOR_SIZE;
					}
				}
			}
			// exit(EXIT_SUCCESS);
			
			if(debug_detail){ cout<<"loadedges:: edges_final[0].size(): "<<edges_final[0].size()<<", edge_count_in_vpartition["<<i<<"]["<<v_p<<"]: "<<edge_count_in_vpartition[i][v_p]<<endl; }
		}
		
		///////////////////////////////////////////////////////
		for(unsigned int t=0; t<16; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"[^"<<(edges_final[i][t].data[v].srcvid / vsize_vP)<<", $"<<(edges_final[i][t].data[v].dstvid / vsize_LLP)<<"], "; } cout<<endl; }
		for(unsigned int t=0; t<16; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"[&"<<(edges_final[i][t].data[v].srcvid % VECTOR_SIZE)<<", $"<<(edges_final[i][t].data[v].dstvid % universalparams.NUM_PARTITIONS)<<"], "; } cout<<endl; }
		for(unsigned int t=0; t<16; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"[#"<<edges_final[i][t].data[v].srcvid<<", $"<<edges_final[i][t].data[v].dstvid<<"], "; } cout<<endl; }
		// for(unsigned int t=0; t<16; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"[#"<<edges_final[i][t].data[v].srcvid<<"("<<(edges_final[i][t].data[v].srcvid % VECTOR_SIZE)<<")("<<(edges_final[i][t].data[v].srcvid / vsize_vP)<<"), $"<<edges_final[i][t].data[v].dstvid<<"("<<(edges_final[i][t].data[v].dstvid % universalparams.NUM_PARTITIONS)<<")("<<(edges_final[i][t].data[v].dstvid / vsize_LLP)<<")], "; } cout<<endl; }
		cout<<"=== Edge content in dram "<<i<<": "<<edges_final[i].size() * VECTOR_SIZE<<endl; 
		////////////////////////////////////////////////////
		exit(EXIT_SUCCESS);
	}
	// exit(EXIT_SUCCESS);
	
	// load vptrs into dram.
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0; 
	globalparams.globalparamsK.SIZE_EDGES = 0; 
	
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (universalparams.AVERAGENUM_WORKEDGES_PER_CHANNEL / VECTOR_SIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = (kvdata_range__div__vptr_shrink_ratio) + universalparams.DRAMPADD_VPTRS; 
	globalparams.globalparamsE.SIZE_EDGES = (edges_final[0].size() * VECTOR_SIZE) + 1000; // average_num_edges_per_channel + 1000; // '1000' is padding 
	globalparams_t globalparamsVPTRS; globalparamsVPTRS = globalparams.globalparamsE; 
			
	cout<<"STAGE 3: calculating vptrs and loading into dram..."<<endl;
	unsigned int vptr_baseoffset = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR2_SIZE;
	for(unsigned int i=0; i<_NUM_PEs; i++){
		#ifdef TESTKERNEL 
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		vptrs[i][vptr_baseoffset + 0].key = 0;
		cout<<"loadedges:: vptrs["<<i<<"][vptr_baseoffset + 0].key: "<<vptrs[i][vptr_baseoffset + 0].key<<", count[0]: "<<0<<endl;
		for(unsigned int v_p=1; v_p<_num_vPs; v_p++){
			vptrs[i][vptr_baseoffset + v_p].key = vptrs[i][vptr_baseoffset + v_p - 1].key + edge_count_in_vpartition[i][v_p - 1]; // NEWCHANGE.
			if(i==0){ cout<<"loadedges:: vptrs["<<i<<"][vptr_baseoffset + "<<v_p<<"].key: "<<vptrs[i][vptr_baseoffset + v_p].key<<", vptrs["<<i<<"][vptr_baseoffset + "<<v_p<<"].key_kvs: "<<vptrs[i][vptr_baseoffset + v_p].key / VECTOR_SIZE<<", edge_count_in_vpartition["<<i<<"]["<<v_p<<"]: "<<edge_count_in_vpartition[i][v_p]<<endl; }
		}
		for(unsigned int v_p=_num_vPs; v_p<_num_vPs + universalparams.DRAMPADD_VPTRS; v_p++){ // dummy filling...
			vptrs[i][vptr_baseoffset + v_p].key = vptrs[i][vptr_baseoffset + _num_vPs - 1].key; 
		}
		
		// verifying...
		for(unsigned int index=vptrs[i][0].key; index<vptrs[i][1].key; index++){
			keyvalue_t kv = edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (index / VECTOR_SIZE)].data[index % VECTOR_SIZE];
			utilityobj->checkoutofbounds("loadedges::ERROR 5611::", kv.key, universalparams.PROCESSPARTITIONSZ, kv.key, kv.value, index);	
		}
	}
	
	/* #ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int v_p=0; v_p<4; v_p++){
		cout<<"STAGE X: v_p: "<<v_p<<endl;
		for(unsigned int t=0; t<4; t++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				cout<<"~~~ loadedges:: edges[0]["<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA<<" + "<<(vptrs[0][vptr_baseoffset + v_p].key / VECTOR_SIZE) + t<<"].data["<<v<<"].key: "<<edges[0][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (vptrs[0][vptr_baseoffset + v_p].key / VECTOR_SIZE) + t].data[v].key<<", edges[0]["<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA<<" + "<<(vptrs[0][vptr_baseoffset + v_p].key / VECTOR_SIZE) + t<<"].data["<<v<<"].value: "<<edges[0][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (vptrs[0][vptr_baseoffset + v_p].key / VECTOR_SIZE) + t].data[v].value<<endl;								 	
			}
		}			
	}
	#endif  */
	
	/* // load container
	cout<<"STAGE 4: loading container..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(universalparams.NUM_VERTICES);
		container->edgessize[i] = edges_final[0].size() * VECTOR_SIZE;
		container->runsize[i] = edges_final[0].size() * VECTOR_SIZE;
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	} */
	
	/* // checking...
	cout<<"STAGE 5: checking final stats..."<<endl;
	unsigned int total_edge_count = 0;
	for(unsigned int i=0; i<_NUM_PEs; i++){ 
		#ifdef TESTKERNEL 
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ 
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){
				total_edge_count += edges_map[i][v_p][ll_p].size; 
				if(i==0 && v_p==0){ cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<", ll_p: "<<ll_p<<"]: edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].size: "<<edges_map[i][v_p][ll_p].size<<endl; }
			}
		}
	} */
	
	/* // map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs]
	unsigned int offset = 0;
	for(unsigned int i=0; i<_NUM_PEs; i++){ 
		#ifdef TESTKERNEL 
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){
			cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<"]"<<endl;
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ // MAXNUM_LLPs
				edges_map[i][v_p][ll_p].offset = offset;
				cout<<"Edge statistics in dram "<<i<<": edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].offset: "<<edges_map[i][v_p][ll_p].offset<<", edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].size: "<<edges_map[i][v_p][ll_p].size<<endl;
				offset += edges_map[i][v_p][ll_p].size;
			}
		}
	} */
	
	cout<<"STAGE 4: calculating edge-map offsets..."<<endl;
	unsigned int offset = 0;
	for(unsigned int i=0; i<_NUM_PEs; i++){
		#ifdef TESTKERNEL 
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		for(unsigned int t=0; t<16; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"{"<<edges_final[i][t].data[v].srcvid<<", "<<edges_final[i][t].data[v].dstvid<<"}, "; } cout<<endl;}
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){
			if(debug_detail){ cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<"]"<<endl; }
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ // MAXNUM_LLPs
				edges_map[i][v_p][ll_p].offset = offset;
				if(debug_detail){ cout<<"Edge statistics in dram "<<i<<": edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].offset: "<<edges_map[i][v_p][ll_p].offset<<", edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].size: "<<edges_map[i][v_p][ll_p].size<<endl; }
				offset += edges_map[i][v_p][ll_p].size;
			}
		}
	}
	
	// load container
	cout<<"STAGE 5: loading container..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(universalparams.NUM_VERTICES);
		container->edgessize[i] = edges_final[0].size() * VECTOR_SIZE;
		container->runsize[i] = edges_final[0].size() * VECTOR_SIZE;
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	
	unsigned int total_edge_count2 = 0; for(unsigned int i=0; i<_NUM_PEs; i++){ cout<<"Edge content in dram "<<i<<": "<<edges_final[i].size() * VECTOR_SIZE<<endl;  }
	cout<<"total edge count: "<<total_edge_count2<<endl;
	#ifdef TESTKERNEL
	cout<<"projected total edge count for all drams: "<<edges_final[0].size() * VECTOR_SIZE * NUM_PEs<<endl;
	#endif 
	exit(EXIT_SUCCESS);
	return globalparams;
}






