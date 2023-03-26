#include "create_act_pack.h"
using namespace std;

create_act_pack::create_act_pack(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
}
create_act_pack::~create_act_pack(){} 

void checkoutofbounds_(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	#ifdef _DEBUGMODE_CHECKS3
	if(data >= upper_bound){ std::cout<<"utility::checkoutofbounds_: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }			
	#endif 
}

void rearrange_layout(unsigned int s, edge2_type in[EDGE_PACK_SIZE], edge2_type out[EDGE_PACK_SIZE]){
	#ifndef FPGA_IMPL
	checkoutofbounds_("acts_kernel::rearrange_layout::ERROR 2125::", s, EDGE_PACK_SIZE, NAp, NAp, NAp);
	#endif 
	if(s==0){ 
		out[0] = in[0]; 
		out[1] = in[1]; 
		out[2] = in[2]; 
		out[3] = in[3]; 
		out[4] = in[4]; 
		out[5] = in[5]; 
		out[6] = in[6]; 
		out[7] = in[7]; 
		out[8] = in[8]; 
		out[9] = in[9]; 
		out[10] = in[10]; 
		out[11] = in[11]; 
		out[12] = in[12]; 
		out[13] = in[13]; 
		out[14] = in[14]; 
		out[15] = in[15]; 
	}
	else if(s==1){ 
		out[1] = in[0]; 
		out[2] = in[1]; 
		out[3] = in[2]; 
		out[4] = in[3]; 
		out[5] = in[4]; 
		out[6] = in[5]; 
		out[7] = in[6]; 
		out[8] = in[7]; 
		out[9] = in[8]; 
		out[10] = in[9]; 
		out[11] = in[10]; 
		out[12] = in[11]; 
		out[13] = in[12]; 
		out[14] = in[13]; 
		out[15] = in[14]; 
		out[0] = in[15]; 
	}
	else if(s==2){ 
		out[2] = in[0]; 
		out[3] = in[1]; 
		out[4] = in[2]; 
		out[5] = in[3]; 
		out[6] = in[4]; 
		out[7] = in[5]; 
		out[8] = in[6]; 
		out[9] = in[7]; 
		out[10] = in[8]; 
		out[11] = in[9]; 
		out[12] = in[10]; 
		out[13] = in[11]; 
		out[14] = in[12]; 
		out[15] = in[13]; 
		out[0] = in[14]; 
		out[1] = in[15]; 
	}
	else if(s==3){ 
		out[3] = in[0]; 
		out[4] = in[1]; 
		out[5] = in[2]; 
		out[6] = in[3]; 
		out[7] = in[4]; 
		out[8] = in[5]; 
		out[9] = in[6]; 
		out[10] = in[7]; 
		out[11] = in[8]; 
		out[12] = in[9]; 
		out[13] = in[10]; 
		out[14] = in[11]; 
		out[15] = in[12]; 
		out[0] = in[13]; 
		out[1] = in[14]; 
		out[2] = in[15]; 
	}
	else if(s==4){ 
		out[4] = in[0]; 
		out[5] = in[1]; 
		out[6] = in[2]; 
		out[7] = in[3]; 
		out[8] = in[4]; 
		out[9] = in[5]; 
		out[10] = in[6]; 
		out[11] = in[7]; 
		out[12] = in[8]; 
		out[13] = in[9]; 
		out[14] = in[10]; 
		out[15] = in[11]; 
		out[0] = in[12]; 
		out[1] = in[13]; 
		out[2] = in[14]; 
		out[3] = in[15]; 
	}
	else if(s==5){ 
		out[5] = in[0]; 
		out[6] = in[1]; 
		out[7] = in[2]; 
		out[8] = in[3]; 
		out[9] = in[4]; 
		out[10] = in[5]; 
		out[11] = in[6]; 
		out[12] = in[7]; 
		out[13] = in[8]; 
		out[14] = in[9]; 
		out[15] = in[10]; 
		out[0] = in[11]; 
		out[1] = in[12]; 
		out[2] = in[13]; 
		out[3] = in[14]; 
		out[4] = in[15]; 
	}
	else if(s==6){ 
		out[6] = in[0]; 
		out[7] = in[1]; 
		out[8] = in[2]; 
		out[9] = in[3]; 
		out[10] = in[4]; 
		out[11] = in[5]; 
		out[12] = in[6]; 
		out[13] = in[7]; 
		out[14] = in[8]; 
		out[15] = in[9]; 
		out[0] = in[10]; 
		out[1] = in[11]; 
		out[2] = in[12]; 
		out[3] = in[13]; 
		out[4] = in[14]; 
		out[5] = in[15]; 
	}
	else if(s==7){ 
		out[7] = in[0]; 
		out[8] = in[1]; 
		out[9] = in[2]; 
		out[10] = in[3]; 
		out[11] = in[4]; 
		out[12] = in[5]; 
		out[13] = in[6]; 
		out[14] = in[7]; 
		out[15] = in[8]; 
		out[0] = in[9]; 
		out[1] = in[10]; 
		out[2] = in[11]; 
		out[3] = in[12]; 
		out[4] = in[13]; 
		out[5] = in[14]; 
		out[6] = in[15]; 
	}
	else if(s==8){ 
		out[8] = in[0]; 
		out[9] = in[1]; 
		out[10] = in[2]; 
		out[11] = in[3]; 
		out[12] = in[4]; 
		out[13] = in[5]; 
		out[14] = in[6]; 
		out[15] = in[7]; 
		out[0] = in[8]; 
		out[1] = in[9]; 
		out[2] = in[10]; 
		out[3] = in[11]; 
		out[4] = in[12]; 
		out[5] = in[13]; 
		out[6] = in[14]; 
		out[7] = in[15]; 
	}
	else if(s==9){ 
		out[9] = in[0]; 
		out[10] = in[1]; 
		out[11] = in[2]; 
		out[12] = in[3]; 
		out[13] = in[4]; 
		out[14] = in[5]; 
		out[15] = in[6]; 
		out[0] = in[7]; 
		out[1] = in[8]; 
		out[2] = in[9]; 
		out[3] = in[10]; 
		out[4] = in[11]; 
		out[5] = in[12]; 
		out[6] = in[13]; 
		out[7] = in[14]; 
		out[8] = in[15]; 
	}
	else if(s==10){ 
		out[10] = in[0]; 
		out[11] = in[1]; 
		out[12] = in[2]; 
		out[13] = in[3]; 
		out[14] = in[4]; 
		out[15] = in[5]; 
		out[0] = in[6]; 
		out[1] = in[7]; 
		out[2] = in[8]; 
		out[3] = in[9]; 
		out[4] = in[10]; 
		out[5] = in[11]; 
		out[6] = in[12]; 
		out[7] = in[13]; 
		out[8] = in[14]; 
		out[9] = in[15]; 
	}
	else if(s==11){ 
		out[11] = in[0]; 
		out[12] = in[1]; 
		out[13] = in[2]; 
		out[14] = in[3]; 
		out[15] = in[4]; 
		out[0] = in[5]; 
		out[1] = in[6]; 
		out[2] = in[7]; 
		out[3] = in[8]; 
		out[4] = in[9]; 
		out[5] = in[10]; 
		out[6] = in[11]; 
		out[7] = in[12]; 
		out[8] = in[13]; 
		out[9] = in[14]; 
		out[10] = in[15]; 
	}
	else if(s==12){ 
		out[12] = in[0]; 
		out[13] = in[1]; 
		out[14] = in[2]; 
		out[15] = in[3]; 
		out[0] = in[4]; 
		out[1] = in[5]; 
		out[2] = in[6]; 
		out[3] = in[7]; 
		out[4] = in[8]; 
		out[5] = in[9]; 
		out[6] = in[10]; 
		out[7] = in[11]; 
		out[8] = in[12]; 
		out[9] = in[13]; 
		out[10] = in[14]; 
		out[11] = in[15]; 
	}
	else if(s==13){ 
		out[13] = in[0]; 
		out[14] = in[1]; 
		out[15] = in[2]; 
		out[0] = in[3]; 
		out[1] = in[4]; 
		out[2] = in[5]; 
		out[3] = in[6]; 
		out[4] = in[7]; 
		out[5] = in[8]; 
		out[6] = in[9]; 
		out[7] = in[10]; 
		out[8] = in[11]; 
		out[9] = in[12]; 
		out[10] = in[13]; 
		out[11] = in[14]; 
		out[12] = in[15]; 
	}
	else if(s==14){ 
		out[14] = in[0]; 
		out[15] = in[1]; 
		out[0] = in[2]; 
		out[1] = in[3]; 
		out[2] = in[4]; 
		out[3] = in[5]; 
		out[4] = in[6]; 
		out[5] = in[7]; 
		out[6] = in[8]; 
		out[7] = in[9]; 
		out[8] = in[10]; 
		out[9] = in[11]; 
		out[10] = in[12]; 
		out[11] = in[13]; 
		out[12] = in[14]; 
		out[13] = in[15]; 
	}
	else { 
		out[15] = in[0]; 
		out[0] = in[1]; 
		out[1] = in[2]; 
		out[2] = in[3]; 
		out[3] = in[4]; 
		out[4] = in[5]; 
		out[5] = in[6]; 
		out[6] = in[7]; 
		out[7] = in[8]; 
		out[8] = in[9]; 
		out[9] = in[10]; 
		out[10] = in[11]; 
		out[11] = in[12]; 
		out[12] = in[13]; 
		out[13] = in[14]; 
		out[14] = in[15]; 
	}
	return;
}

int save_tmp_edges(unsigned int base_offset, map_t count[EDGE_PACK_SIZE][EDGE_PACK_SIZE], edge_update_type * URAM_edges[EDGE_PACK_SIZE], HBM_channelAXISW_t * HBM_channelA, HBM_channelAXISW_t * HBM_channelB, unsigned int _MAX_BUFFER_SIZE, unsigned int globalparams[GLOBALBUFFER_SIZE]){
	edge_update_type edge_update_in[NUM_LLP_PER_LLPSET];	
	edge_update_type edge_update_out[NUM_LLP_PER_LLPSET];
	unsigned int offset_p[NUM_LLP_PER_LLPSET];
	unsigned int p_[NUM_LLP_PER_LLPSET];
	unsigned int index = 0;
	unsigned int max[NUM_LLP_PER_LLPSET]; 
	unsigned int index_t = 0;
	for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ max[llp_id] = 0; for(unsigned int v=0; v<NUM_LLP_PER_LLPSET; v++){ if(max[llp_id] < count[v][llp_id].size){ max[llp_id] = count[v][llp_id].size; }}}
	unsigned int max_ = 0; for(unsigned int v=0; v<NUM_LLP_PER_LLPSET; v++){ max_ += max[v]; }
	
	unsigned int llp_id = 0;
	for(unsigned int t=0; t<max_; t++){ 
	#pragma HLS PIPELINE II=1
		if(index_t >= max[llp_id]){ llp_id += 1; index_t = 0; }
		
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 	
		#pragma HLS UNROLL
			unsigned int p_ = ((EDGE_PACK_SIZE + v - llp_id) % EDGE_PACK_SIZE);		
			if(index_t < count[v][p_].size){ edge_update_in[v] = URAM_edges[v][count[v][p_].offset + index_t]; } 		
			else { edge_update_in[v].srcvid = INVALIDDATA; edge_update_in[v].dstvid = INVALIDDATA; }	
		}
		
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		if(t==0){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cout<<"save_partiallyprepared_edgeupdates (before-cyclic-rotate): llp_id: "<<llp_id<<", t:"<<t<<", edge_update_in["<<v<<"].srcvid: "<<edge_update_in[v].srcvid<<" ("<<edge_update_in[v].srcvid % EDGE_PACK_SIZE<<"), edge_update_in["<<v<<"].dstvid: "<<edge_update_in[v].dstvid<<" ("<<edge_update_in[v].dstvid % EDGE_PACK_SIZE<<")"<<endl; }}
		#endif 
		unsigned int rotateby = edge_update_in[0].srcvid % EDGE_PACK_SIZE;
		rearrange_layout(rotateby, edge_update_in, edge_update_out); 		
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		if(t==0){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cout<<"save_partiallyprepared_edgeupdates (after-cyclic-rotate): llp_id: "<<llp_id<<", t:"<<t<<", edge_update_out["<<v<<"].srcvid: "<<edge_update_out[v].srcvid<<" ("<<edge_update_out[v].srcvid % EDGE_PACK_SIZE<<"), edge_update_out["<<v<<"].dstvid: "<<edge_update_out[v].dstvid<<" ("<<edge_update_out[v].dstvid % EDGE_PACK_SIZE<<")"<<endl; }}
		#endif 
		
		for(unsigned int v=0; v<EDGE_PACK_SIZE/2; v++){				
		#pragma HLS UNROLL
			HBM_channelA[base_offset + index].data[2*v] = edge_update_out[v].srcvid;
			HBM_channelA[base_offset + index].data[2*v + 1] = edge_update_out[v].dstvid;
		}
		for(unsigned int v=0; v<EDGE_PACK_SIZE/2; v++){				
		#pragma HLS UNROLL
			HBM_channelB[base_offset + index].data[2*v] = edge_update_out[EDGE_PACK_SIZE/2 + v].srcvid;
			HBM_channelB[base_offset + index].data[2*v + 1] = edge_update_out[EDGE_PACK_SIZE/2 + v].dstvid;
		}
		index += 1;	
		index_t += 1;
		#ifndef FPGA_IMPL
		checkoutofbounds_("acts_kernel::ERROR 221815::", index, _MAX_BUFFER_SIZE, base_offset, index_t, max_);
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS//4
	cout<<">>> save_partiallyprepared_edgeupdates (after-cyclic-rotate): index: "<<index<<endl;
	#endif 
	return index;
}	

void save_final_edges(unsigned int base_offset, map_t count[EDGE_PACK_SIZE][EDGE_PACK_SIZE], edge_update_type * URAM_edges[EDGE_PACK_SIZE], map_t * edgeupdate_maps, map_t edgeupdate2_maps[NUM_FPGAS][NUM_LLP_PER_LLPSET], HBM_channelAXISW_t * HBM_channelA, HBM_channelAXISW_t * HBM_channelB, unsigned int globalparams[GLOBALBUFFER_SIZE]){
	unsigned int offset_p[EDGE_PACK_SIZE];
	unsigned int p_[EDGE_PACK_SIZE];
	edge_update_type edge_update[EDGE_PACK_SIZE];	
	
	for(unsigned int llp_id=0; llp_id<EDGE_PACK_SIZE; llp_id++){	
		unsigned int offset = base_offset + edgeupdate_maps[llp_id].offset + edgeupdate_maps[llp_id].size;
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ p_[v] =  ((llp_id + v) % EDGE_PACK_SIZE); offset_p[v] = count[v][p_[v]].offset; }
		unsigned int max = 0; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max < count[v][llp_id].size){ max = count[v][llp_id].size; }}	
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"save_fullyprepared_edgeupdates: llp_id: "<<llp_id<<endl; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cout<<""<<p_[v]<<", "; } cout<<endl;
		cout<<"save_fullyprepared_edgeupdates: llp_id: "<<llp_id<<endl; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cout<<""<<offset_p[v]<<", "; } cout<<endl;
		#endif 
		
		unsigned int fpga_sel[NUM_FPGAS][EDGE_PACK_SIZE];
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ fpga_sel[fpga][v] = 0; }}
		
		SAVE_FULLYPREPARED_EDGEUPDATES: for(unsigned int t=0; t<max; t++){
		#pragma HLS PIPELINE II=1
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 	
			#pragma HLS UNROLL
				edge_update_type edge_update_ = URAM_edges[v][offset_p[v] + t];
				if(t < count[v][p_[v]].size){ edge_update[v].srcvid = edge_update_.srcvid / EDGE_PACK_SIZE; edge_update[v].dstvid = edge_update_.dstvid / EDGE_PACK_SIZE; } 
				else { edge_update[v].srcvid = INVALIDDATA; edge_update[v].dstvid = INVALIDDATA; }
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				if(t==0){ cout<<"save_fullyprepared_edgeupdates: llp_id: "<<llp_id<<", t:"<<t<<", edge_update["<<v<<"].srcvid: "<<edge_update[v].srcvid<<" ("<<edge_update[v].srcvid % EDGE_PACK_SIZE<<"), edge_update["<<v<<"].dstvid: "<<edge_update[v].dstvid<<" ("<<edge_update[v].dstvid % EDGE_PACK_SIZE<<")"<<endl; }
				#endif 
			}
			
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(edge_update[v].dstvid != INVALIDDATA){ unsigned int fpga = edge_update[v].dstvid % NUM_FPGAS; fpga_sel[fpga][v] += 1; }}
			
			if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 0){ 
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					if(edge_update[v].srcvid != INVALIDDATA){ edge_update[v].srcvid = edge_update[v].srcvid; } else { edge_update[v].srcvid = INVALIDDATA_SRCVID; }
					if(edge_update[v].dstvid != INVALIDDATA){ edge_update[v].dstvid = edge_update[v].dstvid; } else { edge_update[v].dstvid = INVALIDDATA_DESTVID; }
					HBM_channelA[offset + t].data[v] = ((1 & MAXLOCALVALUE2_ACTPACK_EDGEID) << (MAXNUMBITS2_ACTPACK_DESTVID + MAXNUMBITS2_ACTPACK_SRCVID)) | ((edge_update[v].srcvid & MAXLOCALVALUE2_ACTPACK_SRCVID) << MAXNUMBITS2_ACTPACK_DESTVID) | (edge_update[v].dstvid & MAXLOCALVALUE2_ACTPACK_DESTVID);		
					HBM_channelB[offset + t].data[v] = ((1 & MAXLOCALVALUE2_ACTPACK_EDGEID) << (MAXNUMBITS2_ACTPACK_DESTVID + MAXNUMBITS2_ACTPACK_SRCVID)) | ((edge_update[v].srcvid & MAXLOCALVALUE2_ACTPACK_SRCVID) << MAXNUMBITS2_ACTPACK_DESTVID) | (edge_update[v].dstvid & MAXLOCALVALUE2_ACTPACK_DESTVID);
				}
				#ifndef FPGA_IMPL
				// checkoutofbounds_("acts_kernel::ERROR 221215::", offset + t, globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS], offset, t, NAp);
				checkoutofbounds_("acts_kernel::ERROR 221215::", offset + t, HBM_CHANNEL_SIZE, offset, t, NAp);
				#endif 
			} else {
				for(unsigned int v=0; v<EDGE_PACK_SIZE/2; v++){				
				#pragma HLS UNROLL
					HBM_channelA[offset + t].data[2*v] = edge_update[v].srcvid;
					HBM_channelA[offset + t].data[2*v + 1] = edge_update[v].dstvid;
				}
				for(unsigned int v=0; v<EDGE_PACK_SIZE/2; v++){				
				#pragma HLS UNROLL
					HBM_channelB[offset + t].data[2*v] = edge_update[EDGE_PACK_SIZE/2 + v].srcvid;
					HBM_channelB[offset + t].data[2*v + 1] = edge_update[EDGE_PACK_SIZE/2 + v].dstvid;
				}
			}
		}		
		
		edgeupdate_maps[llp_id].size += max;
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
			unsigned int max_val = 0; 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max_val < fpga_sel[fpga][v]){ max_val = fpga_sel[fpga][v]; }}
			edgeupdate2_maps[fpga][llp_id].size += max_val; 
		}
	}
	// exit(EXIT_SUCCESS);
}
			
unsigned int create_act_pack::create_actpack(action_t action, unsigned int offset_dest, unsigned int offset_destptrs, edge_update_type * URAM_edges[NUM_VALID_PEs][EDGE_PACK_SIZE], unsigned int _MAX_BUFFER_SIZE, unsigned int _MAX_WORKBUFFER_SIZE, unsigned int globalparams[GLOBALBUFFER_SIZE], HBM_channelAXISW_t * HBM_channelA[NUM_PEs], HBM_channelAXISW_t * HBM_channelB[NUM_PEs], vector<edge3_type> (&final_edge_updates)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS]){				
	map_t count[NUM_VALID_PEs][EDGE_PACK_SIZE][EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=count complete dim=1
	#pragma HLS ARRAY_PARTITION variable=count complete dim=2
	unsigned int offsets_rawedgeupdates[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable = offsets_rawedgeupdates complete	
	map_t edgeupdate_maps[NUM_VALID_PEs][NUM_LLP_PER_LLPSET];
	#pragma HLS ARRAY_PARTITION variable=edgeupdate_maps 
	edge_update_vec_dt edge_update_vecs[NUM_VALID_PEs];
	unsigned int temp_size[NUM_VALID_PEs];
	unsigned int base_offset; 
	unsigned int total_sz = 0;
	unsigned int running_offset[NUM_VALID_PEs]; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ running_offset[n] = 0; }
	
	map_t vupdates_map[MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS];
	if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 0){ 
		for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u++){	
			for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; t++){	
				vupdates_map[p_u][t].offset = 0; vupdates_map[p_u][t].size = 0;
			}
		}
	}
	
	map_t vupdates2_map[MAX_NUM_UPARTITIONS][NUM_FPGAS][MAX_NUM_LLPSETS];
	if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 0){ 
		for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u++){	
			for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; t++){	
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
					vupdates2_map[p_u][fpga][t].offset = 0; vupdates2_map[p_u][fpga][t].size = 0;
				}
			}
		}
	}
	
	CREATE_ACTPACK_BASELOOP1: for(unsigned int p_u=action.start_pu; p_u<action.start_pu + action.size_pu; p_u+=action.skip_pu){ 
	// CREATE_ACTPACK_BASELOOP1: for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u+=1){ // FIXME.
		#ifdef _DEBUGMODE_KERNELPRINTS4
		cout<<"### preparing edge updates in upartition "<<p_u<<": [PEs "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<n<<", "; } cout<<"]"<<endl; 
		#endif 
		CREATE_ACTPACK_BASELOOP1B: for(unsigned int llp_set=0; llp_set<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; llp_set++){ 
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<">>> preparing edge updates in upartition "<<p_u<<", llp_set: "<<llp_set<<"...."<<endl; 
			#endif 
			for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ 
				for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
				#pragma HLS UNROLL
					edgeupdate_maps[n][llp_id].offset = 0; edgeupdate_maps[n][llp_id].size = 0;
				}
			}
			
			map_t edgeupdate2_maps[NUM_VALID_PEs][NUM_FPGAS][NUM_LLP_PER_LLPSET]; // FIXME. ADD HLS PRAGMAS?
			for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ 
				for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
					for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
						edgeupdate2_maps[n][fpga][llp_id].offset = 0; edgeupdate2_maps[n][fpga][llp_id].size = 0;
					}
				}
			}
			
			map_t rawedge_maps[NUM_VALID_PEs];
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ rawedge_maps[n].size = final_edge_updates[n][p_u][llp_set].size() / EDGE_PACK_SIZE; }
			
			for(unsigned int c=0; c<2; c++){
				if(c==0){ base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES]; } 
				else { base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES]; }
				
				unsigned int maxnum_rawedges = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(maxnum_rawedges < rawedge_maps[n].size){ maxnum_rawedges = rawedge_maps[n].size; }}
				if(maxnum_rawedges >= _MAX_WORKBUFFER_SIZE){ maxnum_rawedges = _MAX_WORKBUFFER_SIZE; } // trim
				unsigned int maxnum_preprocedges = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(maxnum_preprocedges < temp_size[n]){ maxnum_preprocedges = temp_size[n]; }}
				unsigned int work_size;	if(c==0){ work_size = maxnum_rawedges; } else { work_size = maxnum_preprocedges; }
				#ifdef _DEBUGMODE_CHECKS3
				if(c==1 && work_size > _MAX_BUFFER_SIZE){ cout<<"ALERT: c("<<c<<")==1 && work_size("<<work_size<<") > _MAX_BUFFER_SIZE("<<_MAX_BUFFER_SIZE<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
				#endif
				
				// cout<<">>> acts_kernel::prepare-edge-updates:: c: "<<c<<", p_u: "<<p_u<<", llp_set: "<<llp_set<<", work_size: "<<work_size<<", final_edge_updates[0]["<<p_u<<"]["<<llp_set<<"].size(): "<<final_edge_updates[0][p_u][llp_set].size() / EDGE_PACK_SIZE<<endl; 
				#ifdef _DEBUGMODE_KERNELPRINTS4_CREATEACTPACT
				cout<<">>> acts_kernel::prepare-edge-updates:: c: "<<c<<", p_u: "<<p_u<<", llp_set: "<<llp_set<<", work_size: "<<work_size<<", final_edge_updates[0]["<<p_u<<"]["<<llp_set<<"].size(): "<<final_edge_updates[0][p_u][llp_set].size() / EDGE_PACK_SIZE<<endl; 
				cout<<"acts_kernel::prepare-edge-updates:: rawedges_maps: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<rawedge_maps[n].size<<", "; } cout<<endl;
				if(c==1){ cout<<"acts_kernel::prepare-edge-updates:: temp_size: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<temp_size[n]<<", "; } cout<<endl; }
				#endif 
				
				// reset
				LOOP1734: for(unsigned int t=0; t<EDGE_PACK_SIZE; t++){
				#pragma HLS PIPELINE II=1
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						for(unsigned int n=0; n<NUM_VALID_PEs; n++){
						#pragma HLS UNROLL
							count[n][v][t].offset = 0;
							count[n][v][t].size = 0;
						}
					}
				}
				for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
				#pragma HLS UNROLL
					if(c==0){ offsets_rawedgeupdates[n] = rawedge_maps[n].offset; }
					else { offsets_rawedgeupdates[n] = 0; }
				}
			
				// load edge-updates map	
				// HBM_channelAXISW_t * HBM_channelA[NUM_PEs], HBM_channelAXISW_t * HBM_channelB[NUM_PEs]
				if(c == 1){
					LOAD_EDGEMAPS_LOOP1: for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){	
						// map_t edge_map_vec[NUM_VALID_PEs];
						// load_edgemaps((p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id, offset_destptrs, edge_map_vec, {% include '___HBM___ARGS___.template' %});
						
						
						unsigned int index = (p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id;
						index = index * 2; // '*2' because data is dual: i.e., offset and size
						#ifdef _DEBUGMODE_CHECKS3
						checkoutofbounds_("acts_kernel::ERROR 9601::", offset_destptrs + (index / HBM_AXI_PACK_SIZE), ((1 << 28)/4)/16, index, HBM_CHANNEL_PACK_SIZE, NAp);
						#endif 
						for(unsigned int i=0; i<NUM_VALID_PEs; i++){
							edgeupdate_maps[i][llp_id].offset = HBM_channelA[i][offset_destptrs + (index / HBM_AXI_PACK_SIZE)].data[(index % HBM_AXI_PACK_SIZE)];
							edgeupdate_maps[i][llp_id].size = HBM_channelA[i][offset_destptrs + ((index + 1) / HBM_AXI_PACK_SIZE)].data[((index + 1) % HBM_AXI_PACK_SIZE)];
						}
						
						// for(unsigned int n=0; n<NUM_VALID_PEs; n++){
						// #pragma HLS UNROLL
							// edgeupdate_maps[n][llp_id] = edge_map_vec[n];
						// }	
					}	
				}
					
				// partition stage 1
				CREATE_ACTPACK_LOOP1: for(unsigned int t=0; t<work_size; t++){ // _MAX_BUFFER_SIZE, 1024, work_size
				#pragma HLS PIPELINE II={{context['II_CREATE_ACTPACK_LOOP1']}}
					for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							edge_update_vecs[n].data[v].srcvid = final_edge_updates[n][p_u][llp_set][t*EDGE_PACK_SIZE + v].srcvid % MAX_UPARTITION_SIZE; 
							edge_update_vecs[n].data[v].dstvid = final_edge_updates[n][p_u][llp_set][t*EDGE_PACK_SIZE + v].dstvid % MAX_UPARTITION_SIZE; 
						}
					}
					
					for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
					#pragma HLS UNROLL
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							if(edge_update_vecs[inst].data[v].srcvid != INVALIDDATA){
								#ifdef _DEBUGMODE_KERNELPRINTS//4
								if(inst == 0 && t<2){ cout<<">>> collect-stats: [t:"<<t<<"]: edge_update_vecs["<<inst<<"].data["<<v<<"].srcvid: "<<edge_update_vecs[inst].data[v].srcvid<<" ("<<edge_update_vecs[inst].data[v].srcvid % EDGE_PACK_SIZE<<"), edge_update_vecs["<<inst<<"].data["<<v<<"].dstvid: "<<edge_update_vecs[inst].data[v].dstvid<<" ("<<edge_update_vecs[inst].data[v].dstvid % EDGE_PACK_SIZE<<")"<<endl; }		
								if(inst == 0 && t<2){ cout<<">>> collect-stats: [t:"<<t<<"]: edge_update_vecs2["<<inst<<"].data["<<v<<"].srcvid: "<<edge_update_vecs2[inst].data[v].srcvid<<" ("<<edge_update_vecs2[inst].data[v].srcvid % EDGE_PACK_SIZE<<"), edge_update_vecs2["<<inst<<"].data["<<v<<"].dstvid: "<<edge_update_vecs2[inst].data[v].dstvid<<" ("<<edge_update_vecs2[inst].data[v].dstvid % EDGE_PACK_SIZE<<")"<<endl; }		
								#endif 
								unsigned int data = 0; if(c==0){ data = edge_update_vecs[inst].data[v].srcvid; } else { data = edge_update_vecs[inst].data[v].dstvid; }
								unsigned int p = data % EDGE_PACK_SIZE;
								#ifndef FPGA_IMPL
								checkoutofbounds_("acts_kernel::ERROR 2215::", p, EDGE_PACK_SIZE, NAp, NAp, NAp);
								#endif 
								if(data != INVALIDDATA){ count[inst][v][p].size += 1; }
							}
						}
					}
				}
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				for(unsigned int p=0; p<EDGE_PACK_SIZE; p++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int n=0; n<1; n++){ cout<<"c: "<<c<<" (before): count["<<n<<"]["<<v<<"]["<<p<<"].offset: "<<count[n][v][p].offset<<", count["<<n<<"]["<<v<<"]["<<p<<"].size: "<<count[n][v][p].size<<endl; }}}		
				#endif 
				
				// calculate counts offsets 
				LOOP1334: for(unsigned int p=1; p<EDGE_PACK_SIZE; p++){ 
				#pragma HLS PIPELINE II=1
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						for(unsigned int n=0; n<NUM_VALID_PEs; n++){
						#pragma HLS UNROLL
							count[n][v][p].offset = count[n][v][p-1].offset + count[n][v][p-1].size;					
							#ifdef _DEBUGMODE_CHECKS3
							if(count[0][v][p-1].offset + count[0][v][p-1].size > _MAX_BUFFER_SIZE){ cout<<"prepare-edge-updates:: ALERT: count[0]["<<v<<"]["<<p-1<<"].offset ("<<count[0][v][p-1].offset<<") + count[0]["<<v<<"]["<<p-1<<"].size ("<<count[0][v][p-1].size<<") (:"<<count[0][v][p-1].offset + count[0][v][p-1].size<<":) >= EDGE_UPDATES_DRAMBUFFER_SIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }					
							#endif	
						}	
					}	
				}		
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				for(unsigned int p=0; p<EDGE_PACK_SIZE; p++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int n=0; n<1; n++){ cout<<"c: "<<c<<" (after): count["<<n<<"]["<<v<<"]["<<p<<"].offset: "<<count[n][v][p].offset<<", count["<<n<<"]["<<v<<"]["<<p<<"].size: "<<count[n][v][p].size<<endl; }}}	
				#endif 
				
				// reset counts sizes
				LOOP1234: for(unsigned int p=0; p<EDGE_PACK_SIZE; p++){ 
				#pragma HLS PIPELINE II=1
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						for(unsigned int n=0; n<NUM_VALID_PEs; n++){
						#pragma HLS UNROLL
							count[n][v][p].size = 0;
						}
					}
				}

				// partition stage 2
				CREATE_ACTPACK_LOOP2: for(unsigned int t=0; t<work_size; t++){ // _MAX_BUFFER_SIZE, 1024, work_size
				#pragma HLS PIPELINE II={{context['II_CREATE_ACTPACK_LOOP2']}}
					for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							edge_update_vecs[n].data[v].srcvid = final_edge_updates[n][p_u][llp_set][t*EDGE_PACK_SIZE + v].srcvid % MAX_UPARTITION_SIZE; 
							edge_update_vecs[n].data[v].dstvid = final_edge_updates[n][p_u][llp_set][t*EDGE_PACK_SIZE + v].dstvid % MAX_UPARTITION_SIZE; 
						}
					}
					for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
					#pragma HLS UNROLL
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							if(edge_update_vecs[inst].data[v].srcvid != INVALIDDATA){ // NEWCHANGE.
								unsigned int data = 0; if(c==0){ data = edge_update_vecs[inst].data[v].srcvid; } else { data = edge_update_vecs[inst].data[v].dstvid; }
								unsigned int p = data % EDGE_PACK_SIZE;

								#ifndef FPGA_IMPL
								checkoutofbounds_("acts_kernel::ERROR 2215::", p, EDGE_PACK_SIZE, NAp, NAp, NAp);
								#endif 
								URAM_edges[inst][v][count[inst][v][p].offset + count[inst][v][p].size] = edge_update_vecs[inst].data[v];
								if(data != INVALIDDATA){ count[inst][v][p].size += 1; }
								
								#ifdef _DEBUGMODE_KERNELPRINTS//4	
								if(inst==0 && t<8 && v==0){ cout<<"[finalized edge update: p: "<<p<<", srcvid: "<<edge_update_vecs[inst].data[v].srcvid<<", dstvid: "<<edge_update_vecs[inst].data[v].dstvid<<"]"<<endl; }						
								#endif 
							}
						}
					}	
				}
			
				// store edge updates
				if(c==0){ 
					for(unsigned int i=0; i<NUM_VALID_PEs; i++){
						temp_size[i] = save_tmp_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES], count[i], URAM_edges[i], HBM_channelA[i], HBM_channelB[i], _MAX_BUFFER_SIZE, globalparams);	
					}
				} else {
					#ifdef _DEBUGMODE_KERNELPRINTS4_CREATEACTPACT
					for(unsigned int n=0; n<NUM_VALID_PEs; n++){ for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ cout<<"prepare-edge-updates (before): edgeupdate_maps[n]["<<llp_id<<"]: p_u: "<<p_u<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", offset: "<<edgeupdate_maps[n][llp_id].offset<<", size: "<<edgeupdate_maps[n][llp_id].size<<""<<endl; }}
					#endif 	
					for(unsigned int i=0; i<NUM_VALID_PEs; i++){
						save_final_edges(offset_dest, count[i], URAM_edges[i], edgeupdate_maps[i], edgeupdate2_maps[i], HBM_channelA[i], HBM_channelB[i], globalparams);	
					}
					#ifdef _DEBUGMODE_KERNELPRINTS4_CREATEACTPACT
					for(unsigned int n=0; n<NUM_VALID_PEs; n++){ for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ cout<<"prepare-edge-updates (after): edgeupdate_maps[n]["<<llp_id<<"]: p_u: "<<p_u<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", offset: "<<edgeupdate_maps[n][llp_id].offset<<", size: "<<edgeupdate_maps[n][llp_id].size<<""<<endl; }}
					#endif 
				}	

				// update edge maps
				if(c == 1){ 
					SAVE_EDGEMAPS_LOOP1: for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){	
						for(unsigned int n=0; n<NUM_VALID_PEs; n++){
							edgeupdate_maps[n][llp_id].offset = running_offset[n]; // set offset
							#ifdef _DEBUGMODE_KERNELPRINTS//4
							cout<<"^^^^^^^^^^ prepare-edge-updates: edgeupdate_maps["<<n<<"]["<<llp_id<<"].offset: "<<edgeupdate_maps[n][llp_id].offset<<", edgeupdate_maps["<<n<<"]["<<llp_id<<"].size: "<<edgeupdate_maps[n][llp_id].size<<""<<endl; 
							cout<<"---------- prepare-edge-updates: running_offset["<<n<<"]: "<<running_offset[n]<<""<<endl; 
							#endif 
							running_offset[n] += edgeupdate_maps[n][llp_id].size;
							// for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ running_offset[n] += edgeupdate2_maps[n][fpga][llp_id].size; }
						}
						
						map_t edge_map_vec[NUM_VALID_PEs];
						for(unsigned int n=0; n<NUM_VALID_PEs; n++){
						#pragma HLS UNROLL
							edge_map_vec[n] = edgeupdate_maps[n][llp_id];
						}
						vupdates_map[p_u][llp_set].size += edgeupdate_maps[0][llp_id].size;
						for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ vupdates2_map[p_u][fpga][llp_set].size += edgeupdate2_maps[0][fpga][llp_id].size; }
						
						unsigned int index = (p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id;
						unsigned int base_offset = offset_destptrs;
						index = index * 2; // '*2' because data is dual: i.e., offset and size
						for(unsigned int i=0; i<NUM_VALID_PEs; i++){
							HBM_channelA[i][base_offset + (index / HBM_AXI_PACK_SIZE)].data[(index % HBM_AXI_PACK_SIZE)] = edge_map_vec[i].offset;
						}
						for(unsigned int i=0; i<NUM_VALID_PEs; i++){
							HBM_channelA[i][base_offset + ((index + 1) / HBM_AXI_PACK_SIZE)].data[((index + 1) % HBM_AXI_PACK_SIZE)] = edge_map_vec[i].size;
						}	
						
						// save_edgemaps((p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id, offset_destptrs, edge_map_vec, {% include '___HBM___ARGS___.template' %});
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<"prepare-edge-updates: edgeupdate_maps["<<n<<"]["<<llp_id<<"].offset: "<<edgeupdate_maps[n][llp_id].offset<<", edgeupdate_maps["<<n<<"]["<<llp_id<<"].size: "<<edgeupdate_maps[n][llp_id].size<<""<<endl; }
						#endif
					}
				}
				
				#ifdef _DEBUGMODE_CHECKS3
				for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ total_sz += edgeupdate_maps[0][llp_id].size; }
				#endif 
				// exit(EXIT_SUCCESS); 
			} // c
			// exit(EXIT_SUCCESS); 
		} // llp_set
		// exit(EXIT_SUCCESS);
	} // p_u
	
	if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 0){ 
		map_t vertex_updates_map[MAX_NUM_LLPSETS]; 
		map_t vertex_updates2_map[NUM_FPGAS][MAX_NUM_LLPSETS]; 
		for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]+1; t++){ 
			vertex_updates_map[t].offset=0; vertex_updates_map[t].size=0;
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ vertex_updates2_map[fpga][t].offset=0; vertex_updates2_map[fpga][t].size=0; }
		}
		for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u++){	
			for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; t++){ 
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				cout<<"finish: vupdates_map["<<p_u<<"]["<<t<<"].size: "<<vupdates_map[p_u][t].size<<endl;
				#endif 
				vertex_updates_map[t].size += vupdates_map[p_u][t].size; 
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
					vertex_updates2_map[fpga][t].size += vupdates2_map[p_u][fpga][t].size; 
				}
			}
		}	
		for(unsigned int t=1; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]+1; t++){	
			vertex_updates_map[t].offset = vertex_updates_map[t-1].offset + vertex_updates_map[t-1].size;	
		}	
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){		
			for(unsigned int t=1; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]+1; t++){	
				vertex_updates2_map[fpga][t].offset = vertex_updates2_map[fpga][t-1].offset + vertex_updates2_map[fpga][t-1].size;	
			}	
			unsigned int last_index = globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS] - 1;
			vertex_updates2_map[fpga+1][0].offset = vertex_updates2_map[fpga][last_index].offset + vertex_updates2_map[fpga][last_index].size; 
		}	
		for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]+1; t++){	
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"finish: vertex_updates_map["<<t<<"].offset: "<<vertex_updates_map[t].offset<<", vertex_updates_map["<<t<<"].size: "<<vertex_updates_map[t].size<<endl;
			#endif
		}	
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){	
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"finish: offsets for fpga "<<fpga<<endl;
			#endif 
			for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]+1; t++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				cout<<"finish: [vertex_updates2_map["<<fpga<<"]["<<t<<"].offset: "<<vertex_updates2_map[fpga][t].offset<<", vertex_updates2_map["<<fpga<<"]["<<t<<"].size: "<<vertex_updates2_map[fpga][t].size<<"]"<<endl;
				#endif 
			}
		}	
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){	
			for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]+1; t++){	
				vertex_updates_map[t].size = 0;	
				vertex_updates2_map[fpga][t].size = 0;	
			}
		}	
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
			// save_vupdate_map(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], t, vertex_updates_map[t], {% include '___HBM___ARGS___.template' %});
			for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; t++){		
				// save_vupdate_map(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], (fpga * MAX_NUM_LLPSETS) + t, vertex_updates2_map[fpga][t], {% include '___HBM___ARGS___.template' %});
			
				unsigned int index = (fpga * MAX_NUM_LLPSETS) + t;
				HBM_channelA[0][globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] + index].data[0] = vertex_updates2_map[fpga][t].offset;
				HBM_channelA[0][globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] + index].data[1] = vertex_updates2_map[fpga][t].size;
			
			
			}
		}	
	}
	
	// exit(EXIT_SUCCESS);
	return running_offset[0];
}
// exit(EXIT_SUCCESS);




