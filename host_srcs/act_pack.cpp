#include "act_pack.h"
using namespace std;

act_pack::act_pack(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
}
act_pack::~act_pack(){} 

unsigned int get_H2(unsigned int vid){
	return (vid % (FOLD_SIZE * EDGE_PACK_SIZE * NUM_PEs)) / (FOLD_SIZE * EDGE_PACK_SIZE);
}
unsigned int get_local2(unsigned int vid){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int lvid = (y * (FOLD_SIZE * EDGE_PACK_SIZE)) + x;
	return lvid;
}

// unsigned int get_H2(unsigned int vid){
	// return (vid / 234) % NUM_PEs; // FIXME
// }
// unsigned int get_local2(unsigned int vid){
	// unsigned int lvid = vid / NUM_PEs; // FIXME
	// return lvid;
// }

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
	unsigned int max=0; for(unsigned int i=0; i<NUM_PEs; i++){ if(max < edges_in_channel[i].size()){ max = edges_in_channel[i].size(); } }
	unsigned int min=0xFFFFFFFE; for(unsigned int i=0; i<NUM_PEs; i++){ if(min > edges_in_channel[i].size()){ min = edges_in_channel[i].size(); } }
	if(max - min > 20000000){ cout<<"act_pack: ERROR 445. max - min > 20000000. EXITING..."<<endl; exit(EXIT_FAILURE); }  
	// exit(EXIT_SUCCESS);////////////////////////
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"load_edgeupdates: loading edges [STAGE 2]: preparing edges..."<<endl;
	#endif 
	for(unsigned int i=0; i<NUM_PEs; i++){ // NUM_PEs
		if(utilityobj->channel_is_active(i) == false){ continue; }
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"act_pack:: [PE: "<<i<<"]"<<endl;
		#endif 
		for(unsigned int p_u=0; p_u<universalparams.NUM_UPARTITIONS; p_u++){ edgesin_srcvp[p_u].clear(); } // clear 
		
		// within a HBM channel, partition into v-partitions 
		if(debug){ cout<<"STAGE 1: within a HBM channel, partition into v-partitions "<<endl; }
		for(unsigned int t=0; t<edges_in_channel[i].size(); t++){
			edge3_type edge = edges_in_channel[i][t];
	
			unsigned int p_u = (edge.srcvid / MAX_UPARTITION_SIZE);
			if(p_u >= universalparams.NUM_UPARTITIONS){ p_u = universalparams.NUM_UPARTITIONS-1; } 
	
			#ifdef _DEBUGMODE_HOSTCHECKS3
			// utilityobj->checkoutofbounds("act_pack::ERROR 22a::", edge.srcvid, 60000000, edge.srcvid, edge.srcvid, MAX_UPARTITION_SIZE);
			// utilityobj->checkoutofbounds("act_pack::ERROR 22b::", edge.dstvid, 60000000, edge.srcvid, edge.srcvid, MAX_UPARTITION_SIZE);
			utilityobj->checkoutofbounds("act_pack::ERROR 22c::", p_u, universalparams.NUM_UPARTITIONS, edge.srcvid, edge.srcvid, MAX_UPARTITION_SIZE);
			#endif 
			edgesin_srcvp[p_u].push_back(edge);
		}
		
		for(unsigned int p_u=0; p_u<universalparams.NUM_UPARTITIONS; p_u++){
			// within a v-partition, partition into last-level-partition sets (LLP sets) 
			if(debug){ cout<<"load_edgeupdates: STAGE 2: [i: "<<i<<", v-partition "<<p_u<<"] => partition into last-level-partitions (LLPsets)"<<endl; } 
			for(unsigned int t=0; t<edgesin_srcvp[p_u].size(); t++){
				edge3_type edge = edgesin_srcvp[p_u][t];
				unsigned int local_dstvid = get_local2(edge.dstvid);
				unsigned int llp_set = local_dstvid / MAX_APPLYPARTITION_SIZE;
				llp_set = 0; // FIXME.
				edge.dstvid = local_dstvid; 
				final_edge_updates[i][p_u][llp_set].push_back(edge);
			}
		} // iteration end: p_u
		
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		for(unsigned int p_u=0; p_u<universalparams.NUM_UPARTITIONS; p_u++){
			for(unsigned int llp_set=0; llp_set<universalparams.NUM_APPLYPARTITIONS; llp_set++){
				if(i==0){ cout<<">>> final_edge_updates["<<i<<"]["<<p_u<<"]["<<llp_set<<"].size(): "<<final_edge_updates[i][p_u][llp_set].size()<<""<<endl; }
			}
		}
		#endif 
	} // iteration end: i(NUM_PEs) end here
	// exit(EXIT_SUCCESS);
	return;
}



