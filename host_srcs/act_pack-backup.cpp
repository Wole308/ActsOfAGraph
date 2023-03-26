#include "act_pack.h"
using namespace std;

act_pack::act_pack(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
}
act_pack::~act_pack(){} 

unsigned int get_H2(unsigned int vid){
	return (vid % (FOLD_SIZE * EDGE_PACK_SIZE * NUM_PEs)) / (FOLD_SIZE * EDGE_PACK_SIZE);
	// return vid % NUM_PEs;
	// return utilityobj->get_H(vid);
}
/* unsigned int get_local2(unsigned int vid){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int lvid = (y * (FOLD_SIZE * EDGE_PACK_SIZE)) + x;
	return lvid;
} */
unsigned int get_local2(unsigned int vid){
	return (vid % NUM_PEs) / NUM_PEs;
}
/* unsigned int get_global2(unsigned int lvid, unsigned int H){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y2 = lvid / (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int x2 = lvid % (FOLD_SIZE * EDGE_PACK_SIZE);		
	unsigned int vid = (y2 * W) + (H * (FOLD_SIZE * EDGE_PACK_SIZE)) + x2;
	return vid;
	// return (lvid * NUM_PEs) + H;
	// return utilityobj->get_global(lvid, H);
} */

unsigned int get_local_to_upartitionn(unsigned int lvid){
	return lvid % MAX_UPARTITION_SIZE;
}

void getXYLayoutV(unsigned int s, unsigned int depths[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		depths[v] = (EDGE_PACK_SIZE + v - s) % EDGE_PACK_SIZE; 
	}
}
edge3_vec_dt rearrangeLayoutV(unsigned int s, edge3_vec_dt edge_vec){
	edge3_vec_dt edge_vec3;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		edge_vec3.data[(EDGE_PACK_SIZE + v - s) % EDGE_PACK_SIZE] = edge_vec.data[v]; 
	}
	return edge_vec3;
}
edge3_vec_dt rearrangeLayoutVB(unsigned int s, edge3_vec_dt edge_vec){
	edge3_vec_dt edge_vec3;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		edge_vec3.data[(EDGE_PACK_SIZE + v + s) % EDGE_PACK_SIZE] = edge_vec.data[v]; 
	}
	return edge_vec3;
}

void act_pack::pack(vector<edge_t> &vertexptrbuffer, vector<edge3_type> &edgedatabuffer, 
		vector<edge3_vec_dt> (&act_pack_edges)[NUM_PEs], map_t * act_pack_map[NUM_PEs][MAX_NUM_UPARTITIONS], map_t * act_pack_map2[NUM_PEs][MAX_NUM_UPARTITIONS],
		vector<edge3_vec_dt> (&act_pack_edgeupdates)[NUM_PEs], map_t * act_pack_edgeudates_map[NUM_PEs][MAX_NUM_UPARTITIONS]
		){			
	unsigned int num_vPs = universalparams.NUM_UPARTITIONS;
	unsigned int vsize_vP = MAX_UPARTITION_SIZE;
	unsigned int num_LLPs = universalparams.NUM_APPLYPARTITIONS * universalparams.NUM_PARTITIONS; // EDGE_PACK_SIZE; // 
	unsigned int vsize_LLP = MAX_APPLYPARTITION_SIZE / EDGE_PACK_SIZE; 
	unsigned int num_LLPset = (num_LLPs + (universalparams.NUM_PARTITIONS - 1)) / universalparams.NUM_PARTITIONS;
	cout<<"=== act-pack: num_vPs: "<<num_vPs<<" ==="<<endl;
	cout<<"=== act-pack: vsize_vP: "<<vsize_vP<<" ==="<<endl;
	cout<<"=== act-pack: num_LLPs: "<<num_LLPs<<" ==="<<endl;
	cout<<"=== act-pack: vsize_LLP: "<<vsize_LLP<<" ==="<<endl;
	cout<<"=== act-pack: num_LLPset: "<<num_LLPset<<" ==="<<endl;
	
	cout<<"=== act-pack: EDGE_PACK_SIZE: "<<EDGE_PACK_SIZE<<" ==="<<endl;
	cout<<"=== act-pack: HBM_CHANNEL_PACK_SIZE: "<<HBM_CHANNEL_PACK_SIZE<<" ==="<<endl;
	cout<<"=== act-pack: HBM_AXI_PACK_SIZE: "<<HBM_AXI_PACK_SIZE<<" ==="<<endl;
	cout<<"=== act-pack: HBM_AXI_PACK_BITSIZE: "<<HBM_AXI_PACK_BITSIZE<<" ==="<<endl;
	cout<<"=== act-pack: HBM_CHANNEL_BYTESIZE: "<<HBM_CHANNEL_BYTESIZE<<" ==="<<endl;
	cout<<"=== act-pack: MAX_APPLYPARTITION_VECSIZE: "<<MAX_APPLYPARTITION_VECSIZE<<" ==="<<endl;
	cout<<"=== act-pack: MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<" ==="<<endl;
	cout<<"=== act-pack: MAX_UPARTITION_VECSIZE: "<<MAX_UPARTITION_VECSIZE<<" ==="<<endl;
	cout<<"=== act-pack: MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<" ==="<<endl;
	cout<<"=== act-pack: HBM_CHANNEL_BYTESIZE: "<<HBM_CHANNEL_BYTESIZE<<" ==="<<endl;
	cout<<"=== act-pack: HBM_CHANNEL_SIZE: "<<HBM_CHANNEL_SIZE<<" ==="<<endl;
	cout<<"=== act-pack: MAX_NUM_LLPSETS: "<<MAX_NUM_LLPSETS<<" ==="<<endl;
	cout<<"=== act-pack: UPDATES_BUFFER_PACK_SIZE: "<<UPDATES_BUFFER_PACK_SIZE<<" ==="<<endl;
	
	bool debug = false; // true;// false;

	vector<edge3_type> edges_in_channel[NUM_PEs];
	vector<edge3_type> edgesin_srcvp[MAX_NUM_UPARTITIONS];
	vector<edge3_type> edgesin_srcvp_lldstvp[num_LLPs];
	vector<edge3_type> edgesin_srcvp_lldstvp_srcv2p[num_LLPs][MAX_NUM_PARTITIONS]; 
	
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v_p=0; v_p<MAX_NUM_UPARTITIONS; v_p++){ for(unsigned int ll_p=0; ll_p<MAX_NUM_LLPSETS; ll_p++){ act_pack_map[i][v_p][ll_p].offset = 0; act_pack_map[i][v_p][ll_p].size = 0; }}}

	unsigned int edge_index = 0;
	for(unsigned int vid=0; vid<universalparams.NUM_VERTICES-1; vid++){
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; }
		
		for(unsigned int i=0; i<edges_size; i++){
			edge3_type this_edge = edgedatabuffer[vptr_begin + i];
			edge3_type edge; edge.srcvid = this_edge.srcvid; edge.dstvid = this_edge.dstvid; edge.valid = 1;
			if(edge.srcvid >= universalparams.NUM_VERTICES || edge.dstvid >= universalparams.NUM_VERTICES){ continue; } 
			
			unsigned int H = get_H2(edge.dstvid);
			// edge.dstvid = get_local2(edge.dstvid);
			utilityobj->checkoutofbounds("loadedges::ERROR 223::", H, NUM_PEs, edge.srcvid, edge.dstvid, vsize_vP);
			
			edges_in_channel[H].push_back(edge);
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS4
	for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"dist edges:: PE: "<<i<<": edges_in_channel["<<i<<"].size(): "<<edges_in_channel[i].size()<<""<<endl; }
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"loading edges [STAGE 2]: preparing edges..."<<endl;
	#endif 
	for(unsigned int i=0; i<NUM_PEs; i++){ // NUM_PEs // NUM_VALID_HBM_CHANNELS
		if(utilityobj->channel_is_active(i) == false){ continue; }
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"act_pack:: [PE: "<<i<<"]"<<endl;
		#endif 
		for(unsigned int v_p=0; v_p<num_vPs; v_p++){ edgesin_srcvp[v_p].clear(); } // clear 
		
		// within a HBM channel, partition into v-partitions 
		if(debug){ cout<<"STAGE 1: within a HBM channel, partition into v-partitions "<<endl; }
		for(unsigned int t=0; t<edges_in_channel[i].size(); t++){
			edge3_type edge = edges_in_channel[i][t];
			
			if(edge.srcvid >= 60000000){ edge.srcvid = 60000000; } // FIXME.
			if(edge.dstvid >= 60000000){ edge.dstvid = 60000000; } // FIXME.
			
			unsigned int vP = (edge.srcvid / vsize_vP);
			if(vP >= num_vPs){ vP = num_vPs-1; } // FIXME.
	
			#ifdef _DEBUGMODE_HOSTCHECKS3
			utilityobj->checkoutofbounds("act_pack::ERROR 22::", vP, num_vPs, edge.srcvid, edge.srcvid, vsize_vP);
			#endif 
			edgesin_srcvp[vP].push_back(edge);
		}
		
		for(unsigned int v_p=0; v_p<num_vPs; v_p++){ // num_vPs
			for(unsigned int ll_p=0; ll_p<num_LLPs; ll_p++){ edgesin_srcvp_lldstvp[ll_p].clear(); } // clear
			for(unsigned int ll_p=0; ll_p<num_LLPs; ll_p++){ for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ edgesin_srcvp_lldstvp_srcv2p[ll_p][p].clear(); }} // clear
		
			// within a v-partition, partition into last-level-partitions (LLPs) --- according to reduce concurrency degree (i.e. 8 for kv=u64, 16 for kv=u32)
			if(debug){ cout<<"STAGE 2: [i: "<<i<<", v-partition "<<v_p<<"] => partition into last-level-partitions (LLPs)"<<endl; } 
			for(unsigned int t=0; t<edgesin_srcvp[v_p].size(); t++){
				edge3_type edge = edgesin_srcvp[v_p][t];
				unsigned int local_dstvid = get_local2(edge.dstvid);
				unsigned int ll_p = local_dstvid / vsize_LLP;
				unsigned int newll_p = ((ll_p / EDGE_PACK_SIZE) * EDGE_PACK_SIZE) + (local_dstvid % EDGE_PACK_SIZE);
				#ifdef _DEBUGMODE_HOSTCHECKS3
				utilityobj->checkoutofbounds("act_pack::ERROR 23::", newll_p, num_LLPs, local_dstvid, vsize_LLP, ll_p);
				// if(local_dstvid % EDGE_PACK_SIZE != newll_p){ cout<<"act-pack:   ll_p: "<<ll_p<<", local_dstvid("<<local_dstvid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<local_dstvid % EDGE_PACK_SIZE<<") != newll_p("<<newll_p<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
				#endif 
				
				edgesin_srcvp_lldstvp[newll_p].push_back(edge);
			}
			if(false){ cout<<"act_pack[STAGE 2 check]:: {srcvid, dstvid}"<<endl; }
			
			// witihin a LLP, re-arrange by srcvids 
			if(debug){ cout<<"STAGE 3: witihin a LLP, re-arrange by srcvids "<<endl; }
			for(unsigned int ll_p=0; ll_p<num_LLPs; ll_p++){
				for(unsigned int t=0; t<edgesin_srcvp_lldstvp[ll_p].size(); t++){
					edge3_type edge = edgesin_srcvp_lldstvp[ll_p][t];
					#ifdef _DEBUGMODE_HOSTCHECKS3
					if(edge.dstvid != INVALIDDATA && ((edge.srcvid - (v_p * vsize_vP) >= vsize_vP))){ cout<<"act_pack:: ERROR 235. edge_vec data is out of bounds. edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", v_p: "<<v_p<<", ll_p: "<<ll_p<<", vsize_vP: "<<vsize_vP<<", vsize_LLP: "<<vsize_LLP<<". EXITING..."<<endl; exit(EXIT_FAILURE); }
					#endif 
					edgesin_srcvp_lldstvp_srcv2p[ll_p][edge.srcvid % EDGE_PACK_SIZE].push_back(edge);
				}
			}
			
			// witihin a LLP, re-arrange by srcvids
			if(debug){ cout<<"STAGE 4: preparing edges and loading into dram..."<<endl; }
			unsigned int depths[EDGE_PACK_SIZE];
			for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){ // ll_p set 
				unsigned int offset_llpset = llp_set * EDGE_PACK_SIZE; 
				
				// unsigned int finest_granularity_size = 0;
				for(unsigned int llp_id=0; llp_id<EDGE_PACK_SIZE; llp_id++){		
					if(false){ cout<<"act_pack:: [llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", ~: "<<(llp_set * EDGE_PACK_SIZE * vsize_LLP) + (llp_id * vsize_LLP)<<"]"<<endl; }
				
					// get layout
					getXYLayoutV(llp_id, depths);
				
					// get maximum
					unsigned int max_kvs = 0; 
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ // debug_detail
						if(edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]].size() > max_kvs){ max_kvs = edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]].size(); if(false){ cout<<"^^^^^^^^^^^^^ act_pack:: NON EMPTY PARTITION SEEN @ p: "<<offset_llpset + v<<", v: "<<v<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", max_kvs: "<<max_kvs<<endl; }}
						else if(edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]].size() == 0){ if(false){ cout<<"!!!!!!!!!!!!!!!!! act_pack:: EMPTY PARTITION SEEN @ p: "<<offset_llpset + v<<", v: "<<v<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", max_kvs: "<<max_kvs<<endl; }}
					}
				
					unsigned int finest_granularity_size = 0;
					for(unsigned int t=0; t<max_kvs; t++){
						edge3_vec_dt edge_vec;
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
							utilityobj->checkoutofbounds("loadedges::ERROR 5934a. ", offset_llpset + v, num_LLPs, NAp, NAp, NAp);	
							utilityobj->checkoutofbounds("loadedges::ERROR 5934b. ", depths[v], MAX_NUM_PARTITIONS, NAp, NAp, NAp);
							if(t < edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]].size()){ 
								edge_vec.data[v] = edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]][t]; 
								edge_vec.data[v].valid = 1; 
							} 
							else {	
								edge_vec.data[v].srcvid = INVALIDDATA;
								edge_vec.data[v].dstvid = INVALIDDATA; 
								edge_vec.data[v].valid = 0;
							} 
						}
						
						#ifdef _DEBUGMODE_HOSTCHECKS3
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							if(edge_vec.data[v].dstvid != INVALIDDATA){ 
								unsigned int local_dstvid = get_local2(edge_vec.data[v].dstvid);
								if(local_dstvid % EDGE_PACK_SIZE != v){ 
									cout<<"act_pack:: ERROR 452. t: "<<t<<", edge_vec.data["<<v<<"].dstvid("<<local_dstvid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; 
									cout<<"act_pack:: ERROR 452b. t: "<<t<<", local_dstvid("<<local_dstvid<<") - (llp_set("<<llp_set<<") * vsize_LLP("<<vsize_LLP<<") * EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<llp_set * vsize_LLP * EDGE_PACK_SIZE<<")  < vsize_LLP * EDGE_PACK_SIZE(="<<vsize_LLP * EDGE_PACK_SIZE<<"). EXITING..."<<endl; 
									for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"edge_vec.data["<<v1<<"].srcvid: "<<edge_vec.data[v1].srcvid<<"("<<edge_vec.data[v1].srcvid % EDGE_PACK_SIZE<<"), edge_vec.data["<<v1<<"].dstvid: "<<edge_vec.data[v1].dstvid<<"("<<edge_vec.data[v1].dstvid % EDGE_PACK_SIZE<<"), depths["<<v1<<"]: "<<depths[v1]<<", edge_vec.data["<<v1<<"].valid: "<<edge_vec.data[v1].valid<<endl; } 
									exit(EXIT_FAILURE); 
								}
							}
						}
						#endif 
						
						// re-arrange
						edge3_vec_dt edge_vec3 = rearrangeLayoutV(llp_id, edge_vec);
						
						#ifdef _DEBUGMODE_HOSTCHECKS3
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							if(edge_vec3.data[v].dstvid != INVALIDDATA){ 
								if(edge_vec3.data[v].srcvid % EDGE_PACK_SIZE != v || edge_vec3.data[v].srcvid - (v_p * vsize_vP) >= vsize_vP){ 
									cout<<"act_pack:: ERROR 453. t: "<<t<<", edge_vec3.data["<<v<<"].srcvid("<<edge_vec3.data[v].srcvid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<edge_vec3.data[v].srcvid % EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; 
									cout<<"act_pack:: ERROR 453b. t: "<<t<<", edge_vec3.data["<<v<<"].srcvid("<<edge_vec3.data[v].srcvid<<") - ((v_p("<<v_p<<") * vsize_vP("<<vsize_vP<<")(="<<v_p * vsize_vP<<")) >= vsize_vP(="<<vsize_vP<<"). EXITING..."<<endl; 
									for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"edge_vec3.data["<<v1<<"].srcvid: "<<edge_vec3.data[v1].srcvid<<"("<<edge_vec3.data[v1].srcvid % EDGE_PACK_SIZE<<"), edge_vec3.data["<<v1<<"].dstvid: "<<edge_vec3.data[v1].dstvid<<"("<<edge_vec3.data[v1].dstvid % EDGE_PACK_SIZE<<"), depths["<<v1<<"]: "<<depths[v1]<<", edge_vec.data["<<v1<<"].valid: "<<edge_vec.data[v1].valid<<endl; }
									for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"edge_vec.data["<<v1<<"].srcvid: "<<edge_vec.data[v1].srcvid<<"("<<edge_vec.data[v1].srcvid % EDGE_PACK_SIZE<<"), edge_vec.data["<<v1<<"].dstvid: "<<edge_vec.data[v1].dstvid<<"("<<edge_vec.data[v1].dstvid % EDGE_PACK_SIZE<<"), depths["<<v1<<"]: "<<depths[v1]<<", edge_vec.data["<<v1<<"].valid: "<<edge_vec.data[v1].valid<<endl; } 
									exit(EXIT_FAILURE); 
								}
							}
						}
						#endif
						
						#ifdef _DEBUGMODE_CHECKS3
						edge3_vec_dt edge_vec_test = edge_vec3;
						edge3_vec_dt edge_vec2_local_test;
						unsigned int sample_key = INVALIDDATA; unsigned int sample_u = 0; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(edge_vec_test.data[v].dstvid != INVALIDDATA){ sample_key = get_local2(edge_vec_test.data[v].dstvid) % EDGE_PACK_SIZE; sample_u = v; }} ///////// CRITICAL FIXME.
						unsigned int rotateby = 0; if(sample_key != INVALIDDATA){ rotateby = sample_key - sample_u; }
						bool rotate_forward = true; if(sample_key != INVALIDDATA){ if(sample_key > sample_u){ rotateby = sample_key - sample_u; rotate_forward = false; } else { rotateby = sample_u - sample_key; rotate_forward = true; }} else { rotateby = 0; }
						if(rotate_forward == false){ edge_vec2_local_test = rearrangeLayoutVB(rotateby, edge_vec_test); } else{ edge_vec2_local_test = rearrangeLayoutV(rotateby, edge_vec_test); }
						for(int v = 0; v < EDGE_PACK_SIZE; v++){
							unsigned int local_dstvid = get_local2(edge_vec2_local_test.data[v].dstvid);
							if(edge_vec2_local_test.data[v].dstvid != INVALIDDATA){ 
								if(local_dstvid % EDGE_PACK_SIZE != v){ 
									cout<<"act_pack. ERROR 340. edge_vec2_local_test.data["<<v<<"].dstvid("<<local_dstvid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<local_dstvid % EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; 
									for(int v1 = 0; v1 < EDGE_PACK_SIZE; v1++){ cout<<"+++ i; "<<i<<", acts:: edge_vec2_local_test.data["<<v1<<"].dstvid: "<<edge_vec2_local_test.data[v1].dstvid<<", local_dstvid: "<<get_local2(edge_vec2_local_test.data[v1].dstvid)<<"("<<get_local2(edge_vec2_local_test.data[v1].dstvid) % EDGE_PACK_SIZE<<"), edge_vec2_local_test.data["<<v1<<"].srcvid: "<<edge_vec2_local_test.data[v1].srcvid<<"("<<edge_vec2_local_test.data[v1].srcvid % EDGE_PACK_SIZE<<")"<<endl; }				
									exit(EXIT_FAILURE); 
								}
							}
						}
						#endif

						// load to act_pack buffer
						act_pack_edges[i].push_back(edge_vec3);
						act_pack_map[i][v_p][llp_set].size += 1;
						act_pack_map2[i][v_p][(llp_set * EDGE_PACK_SIZE) + llp_id].size += 1;
						// load to edge updates buffer (act-pack format)
						// act_pack_edgeupdates[i].push_back(edge_vec3); // 
						act_pack_edgeudates_map[i][v_p][(llp_set * EDGE_PACK_SIZE) + llp_id].size += 1;
						
						finest_granularity_size += 1;
					}
					// cout<<"--------------------- v_p: "<<v_p<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<": "<<finest_granularity_size<<" ----------------------"<<endl;
				} // iteration end: llp_id:EDGE_PACK_SIZE
				// cout<<"--------------------- v_p: "<<v_p<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<": "<<finest_granularity_size<<" ----------------------"<<endl;
			} // iteration end: llp_set:num_LLPset
		} // iteration end: v_p:num_vPs 
		cout<<"act-pack:: PE: "<<i<<": act_pack_edges["<<i<<"].size(): "<<act_pack_edges[i].size() * EDGE_PACK_SIZE<<""<<endl;
	} // iteration end: i(NUM_PEs) end here
	// exit(EXIT_SUCCESS); //////////////////////////

	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"loading edges [STAGE 2]: preparing offsets..."<<endl;
	#endif
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int index = 0;
		for(unsigned int v_p=0; v_p<num_vPs; v_p++){
			for(unsigned int ll_p=0; ll_p<num_LLPset; ll_p++){ 
				act_pack_map[i][v_p][ll_p].offset = index;
				index += act_pack_map[i][v_p][ll_p].size;
			}
		}
	}
	
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int index = 0;
		for(unsigned int v_p=0; v_p<num_vPs; v_p++){
			for(unsigned int ll_p=0; ll_p<num_LLPset * EDGE_PACK_SIZE; ll_p++){ 
				act_pack_map2[i][v_p][ll_p].offset = index;
				index += act_pack_map2[i][v_p][ll_p].size;
			}
		}
	}
	
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int index = 0;
		for(unsigned int v_p=0; v_p<num_vPs; v_p++){
			for(unsigned int ll_p=0; ll_p<num_LLPset * EDGE_PACK_SIZE; ll_p++){ 
				act_pack_edgeudates_map[i][v_p][ll_p].offset = index;
				index += act_pack_edgeudates_map[i][v_p][ll_p].size;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS//4
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		for(unsigned int vblock_p=0; vblock_p<1; vblock_p++){ // _num_vPs
			for(unsigned int ll_p=0; ll_p<4; ll_p++){ // MAXNUM_LLPs, _num_LLPset
				cout<<"Edge Map in dram "<<i<<": act_pack_map["<<i<<"]["<<vblock_p<<"]["<<ll_p<<"].offset: "<<act_pack_map[i][vblock_p][ll_p].offset<<", act_pack_map["<<i<<"]["<<vblock_p<<"]["<<ll_p<<"].size: "<<act_pack_map[i][vblock_p][ll_p].size<<endl; 
			}
		}
	}
	#endif 
	
	// clear unused vectors and return
	for(unsigned int i=0; i<NUM_PEs; i++){ edges_in_channel[i].clear(); } 
	for(unsigned int v_p=0; v_p<num_vPs; v_p++){ edgesin_srcvp[v_p].clear(); } 
	for(unsigned int ll_p=0; ll_p<num_LLPs; ll_p++){ edgesin_srcvp_lldstvp[ll_p].clear(); } 
	for(unsigned int ll_p=0; ll_p<num_LLPs; ll_p++){ for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ edgesin_srcvp_lldstvp_srcv2p[ll_p][p].clear(); }} 
	exit(EXIT_SUCCESS);
	return;
}

void act_pack::load_edgeupdates(vector<edge_t> &vertexptrbuffer, vector<edge3_type> &edgedatabuffer, vector<edge3_type> (&final_edge_updates)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS]){			
	cout<<"=== load_edgeupdates: EDGE_PACK_SIZE: "<<EDGE_PACK_SIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: HBM_CHANNEL_PACK_SIZE: "<<HBM_CHANNEL_PACK_SIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: HBM_AXI_PACK_SIZE: "<<HBM_AXI_PACK_SIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: HBM_AXI_PACK_BITSIZE: "<<HBM_AXI_PACK_BITSIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: HBM_CHANNEL_BYTESIZE: "<<HBM_CHANNEL_BYTESIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: MAX_APPLYPARTITION_VECSIZE: "<<MAX_APPLYPARTITION_VECSIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: MAX_UPARTITION_VECSIZE: "<<MAX_UPARTITION_VECSIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: HBM_CHANNEL_BYTESIZE: "<<HBM_CHANNEL_BYTESIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: HBM_CHANNEL_SIZE: "<<HBM_CHANNEL_SIZE<<" ==="<<endl;
	cout<<"=== load_edgeupdates: MAX_NUM_LLPSETS: "<<MAX_NUM_LLPSETS<<" ==="<<endl;
	cout<<"=== load_edgeupdates: UPDATES_BUFFER_PACK_SIZE: "<<UPDATES_BUFFER_PACK_SIZE<<" ==="<<endl;
	
	bool debug = false; // true;// false;

	vector<edge3_type> edges_in_channel[NUM_PEs];
	vector<edge3_type> edgesin_srcvp[MAX_NUM_UPARTITIONS];
	
	unsigned int edge_index = 0;
	for(unsigned int vid=0; vid<universalparams.NUM_VERTICES-1; vid++){
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; }
		
		for(unsigned int i=0; i<edges_size; i++){
			edge3_type this_edge = edgedatabuffer[vptr_begin + i];
			edge3_type edge; edge.srcvid = this_edge.srcvid; edge.dstvid = this_edge.dstvid; edge.valid = 1;
			if(edge.srcvid >= universalparams.NUM_VERTICES || edge.dstvid >= universalparams.NUM_VERTICES){ continue; } 
			
			unsigned int H = get_H2(edge.dstvid);
			utilityobj->checkoutofbounds("loadedges::ERROR 223::", H, NUM_PEs, edge.srcvid, edge.dstvid, MAX_UPARTITION_SIZE);
			
			edges_in_channel[H].push_back(edge);
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS4
	for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"dist edges:: PE: "<<i<<": edges_in_channel["<<i<<"].size(): "<<edges_in_channel[i].size()<<""<<endl; }
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"load_edgeupdates: loading edges [STAGE 2]: preparing edges..."<<endl;
	#endif 
	for(unsigned int i=0; i<NUM_PEs; i++){ // NUM_PEs
		if(utilityobj->channel_is_active(i) == false){ continue; }
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"act_pack:: [PE: "<<i<<"]"<<endl;
		#endif 
		for(unsigned int v_p=0; v_p<universalparams.NUM_UPARTITIONS; v_p++){ edgesin_srcvp[v_p].clear(); } // clear 
		
		// within a HBM channel, partition into v-partitions 
		if(debug){ cout<<"STAGE 1: within a HBM channel, partition into v-partitions "<<endl; }
		for(unsigned int t=0; t<edges_in_channel[i].size(); t++){
			edge3_type edge = edges_in_channel[i][t];
			
			if(edge.srcvid >= 60000000){ edge.srcvid = 60000000; } 
			if(edge.dstvid >= 60000000){ edge.dstvid = 60000000; } 
			
			unsigned int vP = (edge.srcvid / MAX_UPARTITION_SIZE);
			if(vP >= universalparams.NUM_UPARTITIONS){ vP = universalparams.NUM_UPARTITIONS-1; } 
	
			#ifdef _DEBUGMODE_HOSTCHECKS3
			utilityobj->checkoutofbounds("act_pack::ERROR 22::", vP, universalparams.NUM_UPARTITIONS, edge.srcvid, edge.srcvid, MAX_UPARTITION_SIZE);
			#endif 
			edgesin_srcvp[vP].push_back(edge);
		}
		
		/* for(unsigned int v_p=0; v_p<universalparams.NUM_UPARTITIONS; v_p++){
			// within a v-partition, partition into last-level-partition sets (LLP sets) 
			if(debug){ cout<<"load_edgeupdates: STAGE 2: [i: "<<i<<", v-partition "<<v_p<<"] => partition into last-level-partitions (LLPsets)"<<endl; } 
			for(unsigned int t=0; t<edgesin_srcvp[v_p].size(); t++){
				edge3_type edge = edgesin_srcvp[v_p][t];
				unsigned int local_dstvid = get_local2(edge.dstvid);
				unsigned int llp_set = local_dstvid / MAX_APPLYPARTITION_SIZE;
				final_edge_updates[i][v_p][llp_set].push_back(edge);
			}
			if(false){ cout<<"act_pack[STAGE 2 check]:: {srcvid, dstvid}"<<endl; }
		} // iteration end: v_p */
		
		for(unsigned int v_p=0; v_p<universalparams.NUM_UPARTITIONS; v_p++){
			// within a v-partition, partition into last-level-partition sets (LLP sets) 
			if(debug){ cout<<"load_edgeupdates: STAGE 2: [i: "<<i<<", v-partition "<<v_p<<"] => partition into last-level-partitions (LLPsets)"<<endl; } 
			for(unsigned int t=0; t<edgesin_srcvp[v_p].size(); t++){
				edge3_type edge = edgesin_srcvp[v_p][t];
				unsigned int local_dstvid = get_local2(edge.dstvid);
				unsigned int llp_set = local_dstvid / MAX_APPLYPARTITION_SIZE;
				final_edge_updates[i][v_p][llp_set].push_back(edge);
			}
			if(false){ cout<<"act_pack[STAGE 2 check]:: {srcvid, dstvid}"<<endl; }
		} // iteration end: v_p
		
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		for(unsigned int v_p=0; v_p<universalparams.NUM_UPARTITIONS; v_p++){
			for(unsigned int llp_set=0; llp_set<universalparams.NUM_APPLYPARTITIONS; llp_set++){
				if(i==0){ cout<<">>> final_edge_updates["<<i<<"]["<<v_p<<"]["<<llp_set<<"].size(): "<<final_edge_updates[i][v_p][llp_set].size()<<""<<endl; }
			}
		}
		#endif 
	} // iteration end: i(NUM_PEs) end here
	// exit(EXIT_SUCCESS);
	return;
}


