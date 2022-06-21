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
	unsigned int num_LLPs = universalparams.NUMREDUCEPARTITIONS * universalparams.NUM_PARTITIONS; // 1 << (OPT_NUM_PARTITIONS_POW * universalparams.TREE_DEPTH);
	unsigned int vsize_LLP = 1 << (universalparams.BATCH_RANGE_POW - (OPT_NUM_PARTITIONS_POW * universalparams.TREE_DEPTH));
	unsigned int num_LLPset = (num_LLPs + (universalparams.NUM_PARTITIONS - 1)) / universalparams.NUM_PARTITIONS;
	
	unsigned int _NUM_PEs = NUM_PEs;
	unsigned int _num_vPs = num_vPs; // CRIICAL FIXME.
	unsigned int _num_LLPs = num_LLPs;
	unsigned int _num_LLPset = num_LLPset;
	
	vector<edge2_type> edges_in_channel[MAXNUM_PEs];
	vector<edge2_type> edgesin_srcvp[MAXNUM_VPs];
	vector<edge2_type> edgesin_srcvp_lldstvp[num_LLPs];
	vector<edge2_type> edgesin_srcvp_lldstvp_srcv2p[num_LLPs][VECTOR_SIZE];
	unsigned int edge_count_in_vpartition[MAXNUM_PEs][MAXNUM_VPs];
	
	for(unsigned int i=0; i<_NUM_PEs; i++){ edges_final[i].clear(); }
	for(unsigned int i=0; i<_NUM_PEs; i++){ for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ edges_map[i][v_p][ll_p].offset = 0; edges_map[i][v_p][ll_p].size = 0; }}}
	for(unsigned int i=0; i<_NUM_PEs; i++){ for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ edge_count_in_vpartition[i][v_p] = 0; }}
		
	cout<<"loadedges:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", vsize_LLP: "<<vsize_LLP<<", num_vPs: "<<num_vPs<<", vsize_vP: "<<vsize_vP<<", INVALIDDATA: "<<INVALIDDATA<<"] "<<endl;
	
	// partiition into HBM channels 
	cout<<"STAGE 0: partiition into HBM channels "<<endl;
	for(unsigned int vid=0; vid<utilityobj->hmin(universalparams.NUM_VERTICES, universalparams.KVDATA_RANGE)-1; vid++){
		#ifdef _DEBUGMODE_HOSTCHECKS3
		if(vid % 10000000 == 0){ cout<<"### loadedges::start:: vid: "<<vid<<" (of "<<universalparams.NUM_VERTICES<<" vertices), vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
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
	
	bool debug = false; // true, false;
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
			if(debug){ cout<<"STAGE 2: within a v-partition, partition into last-level-partitions (LLPs)"<<endl; }
			// [[0,16,...][1,17...][2,18,...]...[15,31,...]]...[[0,16,...][1,17,...][2,18,...]...[15,31,...]]. distributed by modulo(%) function
			for(unsigned int t=0; t<edgesin_srcvp[v_p].size(); t++){
				edge2_type edge = edgesin_srcvp[v_p][t];
				unsigned int ll_p = (edge.dstvid / vsize_LLP);
				unsigned int newll_p = ((ll_p / universalparams.NUM_PARTITIONS) * universalparams.NUM_PARTITIONS) + (edge.dstvid % universalparams.NUM_PARTITIONS);
				#ifdef _DEBUGMODE_HOSTCHECKS3
				utilityobj->checkoutofbounds("loadedges::ERROR 23::", newll_p, num_LLPs, edge.dstvid, edge.dstvid, vsize_LLP);
				#endif 
				
				edgesin_srcvp_lldstvp[newll_p].push_back(edge);
			}
			if(false){ cout<<"loadedges[STAGE 2 check]:: {srcvid, dstvid}"<<endl; }
			
			// witihin a LLP, re-arrange by hash of srcvids 
			if(debug){ cout<<"STAGE 3: witihin a LLP, re-arrange by srcvids "<<endl; }
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){
				for(unsigned int t=0; t<edgesin_srcvp_lldstvp[ll_p].size(); t++){
					edge2_type edge = edgesin_srcvp_lldstvp[ll_p][t];
					#ifdef _DEBUGMODE_HOSTCHECKS3
					unsigned int u_l = ((ll_p+1) + (OPT_NUM_PARTITIONS-1)) / OPT_NUM_PARTITIONS;
					if(edge.dstvid != INVALIDDATA && ((edge.srcvid - (v_p * vsize_vP) >= vsize_vP) || (edge.dstvid >= u_l * (vsize_LLP * VDATA_PACKINGSIZE)))){ cout<<"loadedges:: ERROR 234. edge_vec data is out of bounds. edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", v_p: "<<v_p<<", ll_p: "<<ll_p<<", vsize_vP: "<<vsize_vP<<", vsize_LLP: "<<vsize_LLP<<", u_l: "<<u_l<<". EXITING..."<<endl; exit(EXIT_FAILURE); }
					#endif 
					edgesin_srcvp_lldstvp_srcv2p[ll_p][edge.srcvid % VECTOR_SIZE].push_back(edge);	
				}
			}
			// exit(EXIT_SUCCESS);
			
			// witihin a LLP, re-arrange by srcvids, also place edges in dram
			if(debug){ cout<<"STAGE 4: preparing edges and loading into dram..."<<endl; }
			edge2_type dummy_edge; dummy_edge.srcvid = INVALIDDATA; dummy_edge.dstvid = INVALIDDATA; 
			unsigned int depths[VECTOR_SIZE];
			for(unsigned int llp_set=0; llp_set<_num_LLPset; llp_set++){ // ll_p set 
				unsigned int offset_llpset = llp_set * universalparams.NUM_PARTITIONS;
				for(unsigned int llp_subset=0; llp_subset<universalparams.NUM_PARTITIONS / VECTOR_SIZE; llp_subset++){ // ll_p set 
					unsigned int offset_llpsubset = llp_subset * VECTOR_SIZE;
					for(unsigned int llp_id=0; llp_id<VECTOR_SIZE; llp_id++){ // ll_p
						if(false){ cout<<"loadedges:: [llp_set: "<<llp_set<<", llp_subset: "<<llp_subset<<", llp_id: "<<llp_id<<"]"<<endl; }
					
						// get layout
						getXYLayoutV(llp_id, depths);
						unsigned int max_kvs = 0; 
						for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(edgesin_srcvp_lldstvp_srcv2p[offset_llpset + offset_llpsubset + v][depths[v]].size() > max_kvs){ max_kvs = edgesin_srcvp_lldstvp_srcv2p[offset_llpsubset + v][depths[v]].size(); }
							else if(edgesin_srcvp_lldstvp_srcv2p[offset_llpset + offset_llpsubset + v][depths[v]].size() == 0){ if(debug_detail){ cout<<"!!!!!!!!!!!!!!!!! loadedges:: EMPTY PARTITION SEEN @ p: "<<offset_llpset + offset_llpsubset + v<<", v: "<<v<<", llp_set: "<<llp_set<<", llp_subset: "<<llp_subset<<", llp_id: "<<llp_id<<", max_kvs: "<<max_kvs<<endl; }}}
					
						for(buffer_type t=0; t<max_kvs; t++){
							edge2_vec_dt edge_vec;
							for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(t < edgesin_srcvp_lldstvp_srcv2p[offset_llpset + offset_llpsubset + v][depths[v]].size()){ edge_vec.data[v] = edgesin_srcvp_lldstvp_srcv2p[offset_llpset + offset_llpsubset + v][depths[v]][t]; } else { edge_vec.data[v] = dummy_edge; }}
							
							// re-arrange
							edge2_vec_dt edge_vec2 = rearrangeLayoutV(offset_llpset + offset_llpsubset + llp_id, edge_vec);
							
							#ifdef _DEBUGMODE_HOSTCHECKS3
							unsigned int u_l = llp_set + 1;
							for(unsigned int v=0; v<VECTOR_SIZE; v++){ 
								unsigned int first_limit = edge_vec2.data[v].srcvid - (v_p * vsize_vP);
								unsigned int second_limit = u_l * (vsize_LLP * VDATA_PACKINGSIZE);
								if(edge_vec2.data[v].dstvid != INVALIDDATA && ((edge_vec2.data[v].srcvid - (v_p * vsize_vP) >= vsize_vP) || (edge_vec2.data[v].dstvid >= u_l * (vsize_LLP * VDATA_PACKINGSIZE)))){ 
									cout<<"loadedges:: ERROR 237. edge_vec2 data is out of bounds. t: "<<t<<", edge_vec2.data["<<v<<"].srcvid: "<<edge_vec2.data[v].srcvid<<", edge_vec2.data["<<v<<"].dstvid: ";
									cout<<edge_vec2.data[v].dstvid<<", first_limit: "<<first_limit<<", second_limit: "<<second_limit<<", v_p: "<<v_p<<", vsize_vP: "<<vsize_vP<<", vsize_LLP: ";
									cout<<vsize_LLP<<", llp_set: "<<llp_set<<", offset_llpset: "<<offset_llpset<<", offset_llpsubset: "<<offset_llpsubset<<", u_l: "<<u_l<<". EXITING..."<<endl; exit(EXIT_FAILURE); 
								}
							}
							#endif 
							
							edge2_vec_dt edge_vec3 = edge_vec2;
							for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(edge_vec2.data[v].dstvid != INVALIDDATA){ edge_vec3.data[v].srcvid = (edge_vec2.data[v].srcvid - v) / universalparams.NUM_PARTITIONS; }}
							 
							for(unsigned int v=0; v<VECTOR_SIZE; v++){
								utilityobj->checkoutofbounds("loadedges::ERROR 56::", globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index, universalparams.MAXHBMCAPACITY_KVS2 * VECTOR_SIZE, globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, NAp);	
								edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v].key = edge_vec3.data[v].dstvid; 
								edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v].value = edge_vec3.data[v].srcvid;
							}
							index+=1;
							edges_final[i].push_back(edge_vec2); 
							
							edges_map[i][v_p][llp_set].size += VECTOR_SIZE; 
							edge_count_in_vpartition[i][v_p] += VECTOR_SIZE;
						}
					}
				}
			}
			
			if(false){ cout<<"loadedges:: edges_final[0].size(): "<<edges_final[0].size()<<", edge_count_in_vpartition["<<i<<"]["<<v_p<<"]: "<<edge_count_in_vpartition[i][v_p]<<endl; }
		}
		if(true){ cout<<"+++ loadedges:: edges_final["<<i<<"].size(): "<<edges_final[i].size() * VECTOR_SIZE<<endl; }
		// exit(EXIT_SUCCESS);
		
		#ifdef _DEBUGMODE_KERNELPRINTS3
		for(unsigned int t=0; t<4; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"[^"<<(edges_final[i][t].data[v].srcvid / vsize_vP)<<", $"<<(edges_final[i][t].data[v].dstvid / vsize_LLP)<<"], "; } cout<<endl; }
		for(unsigned int t=0; t<4; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"[&"<<(edges_final[i][t].data[v].srcvid % VECTOR_SIZE)<<", $"<<(edges_final[i][t].data[v].dstvid % universalparams.NUM_PARTITIONS)<<"], "; } cout<<endl; }
		for(unsigned int t=0; t<4; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"[#"<<edges_final[i][t].data[v].srcvid<<", $"<<edges_final[i][t].data[v].dstvid<<"], "; } cout<<endl; }
		for(unsigned int t=0; t<0; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"[#"<<edges_final[i][t].data[v].srcvid<<"("<<(edges_final[i][t].data[v].srcvid % VECTOR_SIZE)<<")("<<(edges_final[i][t].data[v].srcvid / vsize_vP)<<"), $"<<edges_final[i][t].data[v].dstvid<<"("<<(edges_final[i][t].data[v].dstvid % universalparams.NUM_PARTITIONS)<<")("<<(edges_final[i][t].data[v].dstvid / vsize_LLP)<<")], "; } cout<<endl; }
		cout<<"=== Edge content in dram "<<i<<": "<<edges_final[i].size() * VECTOR_SIZE<<endl; 
		#endif 
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
		if(i==0){ cout<<"loadedges:: vptrs["<<i<<"][~ + 0].key: "<<vptrs[i][vptr_baseoffset + 0].key<<", count[0]: "<<0<<endl; }
		for(unsigned int v_p=1; v_p<_num_vPs; v_p++){
			vptrs[i][vptr_baseoffset + v_p].key = vptrs[i][vptr_baseoffset + v_p - 1].key + edge_count_in_vpartition[i][v_p - 1]; // NEWCHANGE.
			if(i==0 && v_p<8){ cout<<"loadedges:: vptrs["<<i<<"][~ + "<<v_p<<"].key: "<<vptrs[i][vptr_baseoffset + v_p].key<<", vptrs["<<i<<"][~ + "<<v_p<<"].key_kvs: "<<vptrs[i][vptr_baseoffset + v_p].key / VECTOR2_SIZE<<", num edges["<<i<<"]["<<v_p<<"]: "<<edge_count_in_vpartition[i][v_p-1]<<endl; }
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
	
	cout<<"STAGE 4: calculating edge-map offsets..."<<endl;
	for(unsigned int i=0; i<_NUM_PEs; i++){
		#ifdef TESTKERNEL 
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		if(debug_detail){ for(unsigned int t=0; t<16; t++){ for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"{"<<edges_final[i][t].data[v].srcvid<<", "<<edges_final[i][t].data[v].dstvid<<"}, "; } cout<<endl;}}
		unsigned int offset = 0;
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){
			if(debug_detail){ cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<"]"<<endl; }
			for(unsigned int ll_p=0; ll_p<_num_LLPset; ll_p++){ // MAXNUM_LLPs
				edges_map[i][v_p][ll_p].offset = offset;
				offset += edges_map[i][v_p][ll_p].size;
				if(i==0 && v_p==0 && ll_p < 4){ 
				// if(i==0 && ll_p < 4){ 
				// if(ll_p < 4){ 
				cout<<"Edge statistics in dram "<<i<<": edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].offset: "<<edges_map[i][v_p][ll_p].offset<<", edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].size: "<<edges_map[i][v_p][ll_p].size<<endl; }
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
	cout<<"projected total edge count for all drams: "<<edges_final[0].size() * VECTOR_SIZE * NUM_PEs<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<" ["<<(((edges_final[0].size() * VECTOR_SIZE * NUM_PEs) - universalparams.NUM_EDGES) * 100) / universalparams.NUM_EDGES<<"% increase]"<<endl;
	#endif
	
	// clear unused vectors and return
	for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ edgesin_srcvp[v_p].clear(); } // clear 
	for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ edgesin_srcvp_lldstvp[ll_p].clear(); } // clear
	for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ edgesin_srcvp_lldstvp_srcv2p[ll_p][p].clear(); }} 
	// exit(EXIT_SUCCESS);
	return globalparams;
}






