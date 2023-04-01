#include "../include/common.h"
#ifndef ___RUNNING_FPGA_SYNTHESIS___ // FPGA_IMPL
#include "acts_kernel.h"
using namespace std;
#endif 

#define ___ENABLE___RESETBUFFERSATSTART___ 
#define ___ENABLE___CLEAR_COUNTERS___
#define ___ENABLE___PROCESSEDGES___ 
#define ___ENABLE___READ_FRONTIER_PROPERTIES___
#define ___ENABLE___RESET_BUFFERS___
#define ___ENABLE___ECPROCESSEDGES___II1___ 
// // // // // // // // // // // // // // // // // // #define ___ENABLE___COLLECTACTIVEDSTVIDS___ 
#define ___ENABLE___APPLYUPDATESMODULE___ 
	#define ___ENABLE___READ_DEST_PROPERTIES___ 
	#define ___ENABLE___APPLYUPDATES___II1___
	#define ___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___ 
	#define ___ENABLE___SAVE_DEST_PROPERTIES___ 
#define ___ENABLE___GATHER_FRONTIERINFOS___ 
#define ___ENABLE___REPORT_STATISTICS___
#define _____DEBUG_____	

#define MAXVALID_APPLYPARTITION_VECSIZE (MAX_APPLYPARTITION_VECSIZE >> POW_VALID_VDATA)

#ifdef ___RUNNING_FPGA_SYNTHESIS___ // FPGA_IMPL
#define MY_IFDEF_NFRONTIER() keyvalue_t nfrontier_buffer[EDGE_PACK_SIZE][VDATA_SUBPARTITION_VECSIZE]
#define MY_IFDEF_CFRONTIER_TMP() keyvalue_t URAM_frontiers[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_CFRONTIER() keyvalue_t cfrontier_buffer[NUM_VALID_PEs][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_STATSBUFFERMASKBASED_TMP() unsigned int statsbuffer_maskbased_tmp[EDGE_PACK_SIZE][BLOCKRAM_SIZE]	
#define MY_IFDEF_STATSBUFFERMASKBASED() unsigned int statsbuffer_maskbased[MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE]	
#define MY_IFDEF_STATSBUFFERIDBASED() unsigned int statsbuffer_idbased[MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE]	
#define MY_IFDEF_UPDATESTMPBUFFER() keyvalue_t updates_buffer2[UPDATES_BUFFER_PACK_SIZE][UPDATES_BUFFER_SIZE]	
#define MY_IFDEF_VPTRBUFFER() vtr_t vptr_buffer[VPTR_BUFFER_SIZE]
#define MY_IFDEF_EDGESBUFFER() edge3_type edges_buffer[EDGE_PACK_SIZE][EDGE_BUFFER_SIZE]
#define MY_IFDEF_EDGEUPDATESBUFFER() edge_update_type URAM_edges[EDGE_PACK_SIZE][EDGE_UPDATES_DRAMBUFFER_SIZE]
#define MY_IFDEF_VDATABUFFER() vprop_t URAM_vprop[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_TOPLEVELFUNC() void top_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, \
	HBM_channelAXI_t * HBM_import, HBM_channelAXI_t * HBM_export, \
	unsigned int fpga, unsigned int module, unsigned int graph_iteration, unsigned int start_pu, unsigned int size_pu, unsigned int skip_pu, unsigned int start_pv, unsigned int size_pv, unsigned int start_llpset, unsigned int size_llpset, unsigned int start_llpid, unsigned int size_llpid, unsigned int start_gv, unsigned int size_gv, unsigned int id_process, unsigned int id_import, unsigned int id_export, unsigned int size_import_export, unsigned int status)
#else
#define MY_IFDEF_NFRONTIER() keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_CFRONTIER_TMP() keyvalue_t * URAM_frontiers[EDGE_PACK_SIZE]
#define MY_IFDEF_CFRONTIER() keyvalue_t * cfrontier_buffer[NUM_VALID_PEs]
#define MY_IFDEF_STATSBUFFERMASKBASED_TMP() unsigned int * statsbuffer_maskbased_tmp[EDGE_PACK_SIZE]
#define MY_IFDEF_STATSBUFFERMASKBASED() unsigned int * statsbuffer_maskbased[MAX_NUM_APPLYPARTITIONS]
#define MY_IFDEF_STATSBUFFERIDBASED() unsigned int * statsbuffer_idbased[MAX_NUM_APPLYPARTITIONS]
#define MY_IFDEF_UPDATESTMPBUFFER() keyvalue_t * updates_buffer2[UPDATES_BUFFER_PACK_SIZE]
#define MY_IFDEF_VPTRBUFFER() vtr_t * vptr_buffer
#define MY_IFDEF_EDGESBUFFER() edge3_type * edges_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_EDGEUPDATESBUFFER() edge_update_type * URAM_edges[EDGE_PACK_SIZE]
#define MY_IFDEF_VDATABUFFER() vprop_t * URAM_vprop[EDGE_PACK_SIZE]
#define MY_IFDEF_TOPLEVELFUNC() unsigned int acts_kernel::top_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, \
	HBM_channelAXI_t * HBM_import, HBM_channelAXI_t * HBM_export, \
	unsigned int fpga, unsigned int module, unsigned int graph_iteration, unsigned int start_pu, unsigned int size_pu, unsigned int skip_pu, unsigned int start_pv, unsigned int size_pv, unsigned int start_llpset, unsigned int size_llpset, unsigned int start_llpid, unsigned int size_llpid, unsigned int start_gv, unsigned int size_gv, unsigned int id_process, unsigned int id_import, unsigned int id_export, unsigned int size_import_export, unsigned int status, \
	vector<edge3_type> (&final_edge_updates)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS], unsigned int report_statistics[64])					
#endif

#define APPLY_CMD 2
#define TRANSPORT_CMD 3

#ifndef ___RUNNING_FPGA_SYNTHESIS___ // FPGA_IMPL
unsigned int * globalparams_debug;
// unsigned int _NUMCLOCKCYCLES_[2][32]; 
#endif 

#ifndef ___RUNNING_FPGA_SYNTHESIS___ // FPGA_IMPL 
acts_kernel::acts_kernel(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
	algorithmobj = new algorithm();
}
acts_kernel::~acts_kernel(){} 
#endif 

/////////////////////////////////////////
unsigned int get_H(unsigned int vid){
	return (vid % (FOLD_SIZE * EDGE_PACK_SIZE * NUM_PEs)) / (FOLD_SIZE * EDGE_PACK_SIZE);
}
unsigned int get_local(unsigned int vid){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int lvid = (y * (FOLD_SIZE * EDGE_PACK_SIZE)) + x;
	return lvid;
}
unsigned int get_global(unsigned int lvid, unsigned int H){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y2 = lvid / (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int x2 = lvid % (FOLD_SIZE * EDGE_PACK_SIZE);		
	unsigned int vid = (y2 * W) + (H * (FOLD_SIZE * EDGE_PACK_SIZE)) + x2;
	return vid;
}
unsigned int get_local_to_upartition(unsigned int lvid){
	return lvid % MAX_UPARTITION_SIZE;
}
unsigned int get_local_to_vpartition(unsigned int lvid){
	return lvid % MAX_APPLYPARTITION_SIZE;
}
#ifndef FPGA_IMPL
void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	#ifdef _DEBUGMODE_CHECKS3
	if(data >= upper_bound){ std::cout<<"utility::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }			
	#endif 
}
void check_if_contiguous(keyvalue_t keyvalue[EDGE_PACK_SIZE], keyvalue_t msg1[EDGE_PACK_SIZE], keyvalue_t msg2[EDGE_PACK_SIZE], unsigned int msg1_str, unsigned int msg2_str, unsigned int msg3_str){
	#ifdef _DEBUGMODE_CHECKS3
	for(int v = 0; v < EDGE_PACK_SIZE; v++){ 
		unsigned int local_dstvid = get_local(keyvalue[v].key);
		if(keyvalue[v].key != INVALIDDATA){ 
			if(local_dstvid % EDGE_PACK_SIZE != v){ 
				cout<<"check_if_contiguous(1). ERROR 314. v["<<v<<"].key("<<local_dstvid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<local_dstvid % EDGE_PACK_SIZE<<") != v("<<v<<"). [msg1_str: "<<msg1_str<<", msg2_str: "<<msg2_str<<", msg3_str: "<<msg3_str<<"] EXITING..."<<endl; 
				for(int v = 0; v < EDGE_PACK_SIZE; v++){ cout<<"+++ keyvalue["<<v<<"].key: "<<get_local(keyvalue[v].key)<<"("<<get_local(keyvalue[v].key)%EDGE_PACK_SIZE<<"), keyvalue["<<v<<"].value: "<<keyvalue[v].value<<"("<<keyvalue[v].value%EDGE_PACK_SIZE<<")"<<endl; }
				for(int v = 0; v < EDGE_PACK_SIZE; v++){ cout<<"+++ msg1["<<v<<"].key: "<<msg1[v].key<<"("<<msg1[v].key%EDGE_PACK_SIZE<<"), msg1["<<v<<"].value: "<<msg1[v].value<<"("<<msg1[v].value%EDGE_PACK_SIZE<<")"<<endl; }
				for(int v = 0; v < EDGE_PACK_SIZE; v++){ cout<<"+++ msg2["<<v<<"].key: "<<msg2[v].key<<"("<<msg2[v].key%EDGE_PACK_SIZE<<"), msg2["<<v<<"].value: "<<msg2[v].value<<"("<<msg2[v].value%EDGE_PACK_SIZE<<")"<<endl; }
				exit(EXIT_FAILURE); 
			}
		}
	}
	#endif
}
#endif 

unsigned int hash_edge(unsigned int srcvid, unsigned int dstvid, unsigned int index, unsigned int motion){
	// return ((srcvid % EDGE_UPDATES_DRAMBUFFER_SIZE) + (dstvid % EDGE_UPDATES_DRAMBUFFER_SIZE)) / 2;
	// return index; // REMOVEME.
	
	unsigned int edge_hashid = ((srcvid / 16) * 14) + (((dstvid / 16) % 16) + (dstvid % 16));
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("utility::ERROR 25133::", edge_hashid, EDGE_UPDATES_DRAMBUFFER_SIZE, srcvid, dstvid, NAp);
	#endif
	
	return edge_hashid; // ((srcvid / 16) * 14) + (((dstvid / 16) % 16) + (dstvid % 16)); // original
	// return ((srcvid / 16) * (12 + motion)) + (((dstvid / 16) % 16) + (dstvid % 16));
	
	// return (srcvid + dstvid) % (EDGE_UPDATES_DRAMBUFFER_SIZE;
	// return ((srcvid % (EDGE_UPDATES_DRAMBUFFER_SIZE / 16)) * 16) + (dstvid % 16);	
	// return ((dstvid % (EDGE_UPDATES_DRAMBUFFER_SIZE / 16)) * 16) + (srcvid % 16);
	// return srcvid; //*
	// return dstvid;
}

unsigned int hash2_edge(map_t URAM_map, unsigned int srcvid, unsigned int dstvid, unsigned int padding_factor){
#pragma HLS INLINE
	unsigned int allotted_hashspace = URAM_map.size * padding_factor; 
	#ifdef _DEBUGMODE_CHECKS3
	if(URAM_map.size == 0){ cout<<"-------------------------------- hash2_edge: URAM_map.size == 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
	if(padding_factor == 0){ cout<<"-------------------------------- hash2_edge: padding_factor == 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
	// if(dstvid == 0){ cout<<"-------------------------------- hash2_edge: srcvid = 0. dstvid == 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
	#endif 
	unsigned int edge_hashid = URAM_map.offset + (dstvid % allotted_hashspace); 
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("utility::ERROR 779911::", edge_hashid, EDGE_UPDATES_DRAMBUFFER_SIZE, dstvid, URAM_map.offset, URAM_map.size); 
	#endif
	return edge_hashid;							
}

void rearrangeLayoutVx16B(unsigned int s, keyvalue_t in[EDGE_PACK_SIZE], keyvalue_t out[EDGE_PACK_SIZE]){
	#ifndef FPGA_IMPL
	// checkoutofbounds("acts_kernel::rearrangeLayoutVx16B::ERROR 2925::", s, EDGE_PACK_SIZE, NAp, NAp, NAp);
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

void rearrangeLayoutVx16(unsigned int s, edge2_type in[EDGE_PACK_SIZE], edge2_type out[EDGE_PACK_SIZE]){
	#ifndef FPGA_IMPL
	// checkoutofbounds("acts_kernel::rearrangeLayoutVx16::ERROR 2125::", s, EDGE_PACK_SIZE, NAp, NAp, NAp);
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

unsigned int min_(unsigned int val1, unsigned int val2){
	#pragma HLS INLINE
	if(val1 < val2){ return val1; }
	else { return val2; }
}
void update_dramnumclockcycles(unsigned int _NUMCLOCKCYCLES_[2][32], unsigned int x, unsigned int clock_cycles){
	#ifdef _DEBUGMODE_CHECKS3
	_NUMCLOCKCYCLES_[0][x] += clock_cycles;
	#endif
}
void update_bramnumclockcycles(unsigned int _NUMCLOCKCYCLES_[2][32], unsigned int x, unsigned int clock_cycles){
	#ifdef _DEBUGMODE_CHECKS3
	_NUMCLOCKCYCLES_[0][x] += clock_cycles;
	// _NUMCLOCKCYCLES_[1][x] += clock_cycles;
	#endif
}

/////////////////////////////////////////

value_t process_func_pr(vprop_t uprop, value_t edgew){
	#pragma HLS INLINE 
	value_t res = uprop.prop; 
	return res;
}
value_t reduce_func_pr(vprop_t vprop, value_t res){
	#pragma HLS INLINE 
	float alpha = 0.5;
	return (alpha + ((1 - alpha) * (vprop.prop + res))) / (vprop.degree + 1);
}

value_t process_func_spmv(vprop_t uprop, value_t edgew){ // see graphlily slides
	#pragma HLS INLINE 
	return uprop.prop * edgew;
}
value_t reduce_func_spmv(vprop_t vprop, value_t res){
	#pragma HLS INLINE 
	return vprop.prop + res;
}

value_t process_func_hits(vprop_t uprop, value_t edgew){
	#pragma HLS INLINE 
	return uprop.prop;
}
value_t reduce_func_hits(vprop_t vprop, value_t res){
	#pragma HLS INLINE 
	return vprop.prop + res;
}

value_t process_func_sssp(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata + edgew;
	return res;
}
value_t reduce_func_sssp(value_t vtemp, value_t vdata, value_t res){
	#pragma HLS INLINE 
	if(res < vtemp){ return res; } else { return vtemp; }
}

value_t process_funcG(vprop_t uprop, value_t edgew, unsigned int algo){
	#pragma HLS INLINE 
	// if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == PAGERANK){ return process_func_pr(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CF){ return process_func_cf(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == HITS){ return process_func_hits(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == BFS){ return process_func_bfs(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == SSSP){ return process_func_sssp(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CC){ return process_func_cc(udata, edgew); }

	#ifdef BAKE_PAGERANK_ALGORITHM_FUNCS
	return process_func_pr(uprop, edgew);	
	#endif 
	#ifdef BAKE_SPMV_ALGORITHM_FUNCS
	return process_func_spmv(uprop, edgew);
	#endif 
	#ifdef BAKE_HITS_ALGORITHM_FUNCS
	return process_func_hits(uprop, edgew);
	#endif 
	// return reduce_func_sssp(udata, edgew);
}
value_t reduce_funcG(vprop_t vprop, value_t res, unsigned int algo){
	#pragma HLS INLINE 
	// if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == PAGERANK){ return reduce_func_pr(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CF){ return reduce_func_cf(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == HITS){ return reduce_func_hits(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == BFS){ return reduce_func_bfs(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == SSSP){ return reduce_func_sssp(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CC){ return reduce_func_cc(udata, edgew); }
	
	#ifdef BAKE_PAGERANK_ALGORITHM_FUNCS
	return reduce_func_pr(vprop, res);	
	#endif 
	#ifdef BAKE_SPMV_ALGORITHM_FUNCS
	return reduce_func_spmv(vprop, res);
	#endif 
	#ifdef BAKE_HITS_ALGORITHM_FUNCS	
	return reduce_func_hits(vprop, res);	
	#endif 
	// return reduce_func_sssp(vprop, res);
}

#ifdef ___NOT_YET_IMPLEMENTED___
value_t process_func(vprop_t uprop, value_t edgew, unsigned int algo){
	#pragma HLS INLINE 
	if(GraphAlgo == PAGERANK){
		return process_func_pr(vprop, edgew);
	} else if(GraphAlgo == SPMV){
		return process_func_spmv(vprop, edgew);
	} else if(GraphAlgo == HITS){
		return process_func_hits(vprop, edgew);
	} else if(GraphAlgo == SSSP){
		return process_func_sssp(vprop, edgew);
	} else {
		return process_func_pr(vprop, edgew);
	}
}
value_t reduce_func(vprop_t vprop, value_t res, unsigned int algo){
	#pragma HLS INLINE 
	if(GraphAlgo == PAGERANK){ 
		return reduce_func_pr(vprop, res);
	} else if(GraphAlgo == SPMV){
		return reduce_func_spmv(vprop, res);
	} else if(GraphAlgo == HITS){
		return reduce_func_hits(vprop, res);
	} else if(GraphAlgo == SSSP){
		return reduce_func_sssp(vprop, res);
	} else {
		return reduce_func_pr(vprop, res);
	}
}	
#endif 	

//////////////////////////////////////////////////////////////////
void master_centerinsert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB){
	#pragma HLS INLINE // FIXME_HARDWARE
	HBM_centerA[offset].data[0] = data[0];
	HBM_centerA[offset].data[1] = data[1];
	HBM_centerA[offset].data[2] = data[2];
	HBM_centerA[offset].data[3] = data[3];
	HBM_centerA[offset].data[4] = data[4];
	HBM_centerA[offset].data[5] = data[5];
	HBM_centerA[offset].data[6] = data[6];
	HBM_centerA[offset].data[7] = data[7];
	HBM_centerA[offset].data[8] = data[8];
	HBM_centerA[offset].data[9] = data[9];
	HBM_centerA[offset].data[10] = data[10];
	HBM_centerA[offset].data[11] = data[11];
	HBM_centerA[offset].data[12] = data[12];
	HBM_centerA[offset].data[13] = data[13];
	HBM_centerA[offset].data[14] = data[14];
	HBM_centerA[offset].data[15] = data[15];
	HBM_centerB[offset].data[0] = data[16];
	HBM_centerB[offset].data[1] = data[17];
	HBM_centerB[offset].data[2] = data[18];
	HBM_centerB[offset].data[3] = data[19];
	HBM_centerB[offset].data[4] = data[20];
	HBM_centerB[offset].data[5] = data[21];
	HBM_centerB[offset].data[6] = data[22];
	HBM_centerB[offset].data[7] = data[23];
	HBM_centerB[offset].data[8] = data[24];
	HBM_centerB[offset].data[9] = data[25];
	HBM_centerB[offset].data[10] = data[26];
	HBM_centerB[offset].data[11] = data[27];
	HBM_centerB[offset].data[12] = data[28];
	HBM_centerB[offset].data[13] = data[29];
	HBM_centerB[offset].data[14] = data[30];
	HBM_centerB[offset].data[15] = data[31];
	return;
}
void master_centerretrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB){
	#pragma HLS INLINE // FIXME_HARDWARE
	data[0] = HBM_centerA[offset].data[0];
	data[1] = HBM_centerA[offset].data[1];
	data[2] = HBM_centerA[offset].data[2];
	data[3] = HBM_centerA[offset].data[3];
	data[4] = HBM_centerA[offset].data[4];
	data[5] = HBM_centerA[offset].data[5];
	data[6] = HBM_centerA[offset].data[6];
	data[7] = HBM_centerA[offset].data[7];
	data[8] = HBM_centerA[offset].data[8];
	data[9] = HBM_centerA[offset].data[9];
	data[10] = HBM_centerA[offset].data[10];
	data[11] = HBM_centerA[offset].data[11];
	data[12] = HBM_centerA[offset].data[12];
	data[13] = HBM_centerA[offset].data[13];
	data[14] = HBM_centerA[offset].data[14];
	data[15] = HBM_centerA[offset].data[15];
	data[16] = HBM_centerB[offset].data[0];
	data[17] = HBM_centerB[offset].data[1];
	data[18] = HBM_centerB[offset].data[2];
	data[19] = HBM_centerB[offset].data[3];
	data[20] = HBM_centerB[offset].data[4];
	data[21] = HBM_centerB[offset].data[5];
	data[22] = HBM_centerB[offset].data[6];
	data[23] = HBM_centerB[offset].data[7];
	data[24] = HBM_centerB[offset].data[8];
	data[25] = HBM_centerB[offset].data[9];
	data[26] = HBM_centerB[offset].data[10];
	data[27] = HBM_centerB[offset].data[11];
	data[28] = HBM_centerB[offset].data[12];
	data[29] = HBM_centerB[offset].data[13];
	data[30] = HBM_centerB[offset].data[14];
	data[31] = HBM_centerB[offset].data[15];
	return;
}

void master_insert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE
	HBM_channelA[offset].data[0] = data[0];
	HBM_channelA[offset].data[1] = data[1];
	HBM_channelA[offset].data[2] = data[2];
	HBM_channelA[offset].data[3] = data[3];
	HBM_channelA[offset].data[4] = data[4];
	HBM_channelA[offset].data[5] = data[5];
	HBM_channelA[offset].data[6] = data[6];
	HBM_channelA[offset].data[7] = data[7];
	HBM_channelA[offset].data[8] = data[8];
	HBM_channelA[offset].data[9] = data[9];
	HBM_channelA[offset].data[10] = data[10];
	HBM_channelA[offset].data[11] = data[11];
	HBM_channelA[offset].data[12] = data[12];
	HBM_channelA[offset].data[13] = data[13];
	HBM_channelA[offset].data[14] = data[14];
	HBM_channelA[offset].data[15] = data[15];
	HBM_channelB[offset].data[0] = data[16];
	HBM_channelB[offset].data[1] = data[17];
	HBM_channelB[offset].data[2] = data[18];
	HBM_channelB[offset].data[3] = data[19];
	HBM_channelB[offset].data[4] = data[20];
	HBM_channelB[offset].data[5] = data[21];
	HBM_channelB[offset].data[6] = data[22];
	HBM_channelB[offset].data[7] = data[23];
	HBM_channelB[offset].data[8] = data[24];
	HBM_channelB[offset].data[9] = data[25];
	HBM_channelB[offset].data[10] = data[26];
	HBM_channelB[offset].data[11] = data[27];
	HBM_channelB[offset].data[12] = data[28];
	HBM_channelB[offset].data[13] = data[29];
	HBM_channelB[offset].data[14] = data[30];
	HBM_channelB[offset].data[15] = data[31];
	return;
}
void master_retrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	data[0] = HBM_channelA[offset].data[0];
	data[1] = HBM_channelA[offset].data[1];
	data[2] = HBM_channelA[offset].data[2];
	data[3] = HBM_channelA[offset].data[3];
	data[4] = HBM_channelA[offset].data[4];
	data[5] = HBM_channelA[offset].data[5];
	data[6] = HBM_channelA[offset].data[6];
	data[7] = HBM_channelA[offset].data[7];
	data[8] = HBM_channelA[offset].data[8];
	data[9] = HBM_channelA[offset].data[9];
	data[10] = HBM_channelA[offset].data[10];
	data[11] = HBM_channelA[offset].data[11];
	data[12] = HBM_channelA[offset].data[12];
	data[13] = HBM_channelA[offset].data[13];
	data[14] = HBM_channelA[offset].data[14];
	data[15] = HBM_channelA[offset].data[15];
	data[16] = HBM_channelB[offset].data[0];
	data[17] = HBM_channelB[offset].data[1];
	data[18] = HBM_channelB[offset].data[2];
	data[19] = HBM_channelB[offset].data[3];
	data[20] = HBM_channelB[offset].data[4];
	data[21] = HBM_channelB[offset].data[5];
	data[22] = HBM_channelB[offset].data[6];
	data[23] = HBM_channelB[offset].data[7];
	data[24] = HBM_channelB[offset].data[8];
	data[25] = HBM_channelB[offset].data[9];
	data[26] = HBM_channelB[offset].data[10];
	data[27] = HBM_channelB[offset].data[11];
	data[28] = HBM_channelB[offset].data[12];
	data[29] = HBM_channelB[offset].data[13];
	data[30] = HBM_channelB[offset].data[14];
	data[31] = HBM_channelB[offset].data[15];
	return;
}

void master_insertmany_vec(unsigned int offsets[NUM_VALID_PEs], bool ens[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	
	HBM_channelA0[offsets[0]].data[0] = data[0][0];
	HBM_channelA0[offsets[0]].data[1] = data[0][1];
	HBM_channelA0[offsets[0]].data[2] = data[0][2];
	HBM_channelA0[offsets[0]].data[3] = data[0][3];
	HBM_channelA0[offsets[0]].data[4] = data[0][4];
	HBM_channelA0[offsets[0]].data[5] = data[0][5];
	HBM_channelA0[offsets[0]].data[6] = data[0][6];
	HBM_channelA0[offsets[0]].data[7] = data[0][7];
	HBM_channelA0[offsets[0]].data[8] = data[0][8];
	HBM_channelA0[offsets[0]].data[9] = data[0][9];
	HBM_channelA0[offsets[0]].data[10] = data[0][10];
	HBM_channelA0[offsets[0]].data[11] = data[0][11];
	HBM_channelA0[offsets[0]].data[12] = data[0][12];
	HBM_channelA0[offsets[0]].data[13] = data[0][13];
	HBM_channelA0[offsets[0]].data[14] = data[0][14];
	HBM_channelA0[offsets[0]].data[15] = data[0][15];
	HBM_channelB0[offsets[0]].data[0] = data[0][16];
	HBM_channelB0[offsets[0]].data[1] = data[0][17];
	HBM_channelB0[offsets[0]].data[2] = data[0][18];
	HBM_channelB0[offsets[0]].data[3] = data[0][19];
	HBM_channelB0[offsets[0]].data[4] = data[0][20];
	HBM_channelB0[offsets[0]].data[5] = data[0][21];
	HBM_channelB0[offsets[0]].data[6] = data[0][22];
	HBM_channelB0[offsets[0]].data[7] = data[0][23];
	HBM_channelB0[offsets[0]].data[8] = data[0][24];
	HBM_channelB0[offsets[0]].data[9] = data[0][25];
	HBM_channelB0[offsets[0]].data[10] = data[0][26];
	HBM_channelB0[offsets[0]].data[11] = data[0][27];
	HBM_channelB0[offsets[0]].data[12] = data[0][28];
	HBM_channelB0[offsets[0]].data[13] = data[0][29];
	HBM_channelB0[offsets[0]].data[14] = data[0][30];
	HBM_channelB0[offsets[0]].data[15] = data[0][31];
	
	return;
}
void master_retrievemany_vec(unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	data[0][0] = HBM_channelA0[offsets[0]].data[0];
	data[0][1] = HBM_channelA0[offsets[0]].data[1];
	data[0][2] = HBM_channelA0[offsets[0]].data[2];
	data[0][3] = HBM_channelA0[offsets[0]].data[3];
	data[0][4] = HBM_channelA0[offsets[0]].data[4];
	data[0][5] = HBM_channelA0[offsets[0]].data[5];
	data[0][6] = HBM_channelA0[offsets[0]].data[6];
	data[0][7] = HBM_channelA0[offsets[0]].data[7];
	data[0][8] = HBM_channelA0[offsets[0]].data[8];
	data[0][9] = HBM_channelA0[offsets[0]].data[9];
	data[0][10] = HBM_channelA0[offsets[0]].data[10];
	data[0][11] = HBM_channelA0[offsets[0]].data[11];
	data[0][12] = HBM_channelA0[offsets[0]].data[12];
	data[0][13] = HBM_channelA0[offsets[0]].data[13];
	data[0][14] = HBM_channelA0[offsets[0]].data[14];
	data[0][15] = HBM_channelA0[offsets[0]].data[15];
	data[0][16] = HBM_channelB0[offsets[0]].data[0];
	data[0][17] = HBM_channelB0[offsets[0]].data[1];
	data[0][18] = HBM_channelB0[offsets[0]].data[2];
	data[0][19] = HBM_channelB0[offsets[0]].data[3];
	data[0][20] = HBM_channelB0[offsets[0]].data[4];
	data[0][21] = HBM_channelB0[offsets[0]].data[5];
	data[0][22] = HBM_channelB0[offsets[0]].data[6];
	data[0][23] = HBM_channelB0[offsets[0]].data[7];
	data[0][24] = HBM_channelB0[offsets[0]].data[8];
	data[0][25] = HBM_channelB0[offsets[0]].data[9];
	data[0][26] = HBM_channelB0[offsets[0]].data[10];
	data[0][27] = HBM_channelB0[offsets[0]].data[11];
	data[0][28] = HBM_channelB0[offsets[0]].data[12];
	data[0][29] = HBM_channelB0[offsets[0]].data[13];
	data[0][30] = HBM_channelB0[offsets[0]].data[14];
	data[0][31] = HBM_channelB0[offsets[0]].data[15];
	return;
}

void master_insertmany_Avec(unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	
		HBM_channelA0[offsets[0]].data[0] = data[0][0];	// FIXME
		HBM_channelA0[offsets[0]].data[1] = data[0][1];	// FIXME
		HBM_channelA0[offsets[0]].data[2] = data[0][2];	// FIXME
		HBM_channelA0[offsets[0]].data[3] = data[0][3];	// FIXME
		HBM_channelA0[offsets[0]].data[4] = data[0][4];	// FIXME
		HBM_channelA0[offsets[0]].data[5] = data[0][5];	// FIXME
		HBM_channelA0[offsets[0]].data[6] = data[0][6];	// FIXME
		HBM_channelA0[offsets[0]].data[7] = data[0][7];	// FIXME
		HBM_channelA0[offsets[0]].data[8] = data[0][8];	// FIXME
		HBM_channelA0[offsets[0]].data[9] = data[0][9];	// FIXME
		HBM_channelA0[offsets[0]].data[10] = data[0][10];	// FIXME
		HBM_channelA0[offsets[0]].data[11] = data[0][11];	// FIXME
		HBM_channelA0[offsets[0]].data[12] = data[0][12];	// FIXME
		HBM_channelA0[offsets[0]].data[13] = data[0][13];	// FIXME
		HBM_channelA0[offsets[0]].data[14] = data[0][14];	// FIXME
		HBM_channelA0[offsets[0]].data[15] = data[0][15];	// FIXME
	
	
	return;
}
void master_insertmany_Bvec(unsigned int offsets[NUM_VALID_PEs], bool ens[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	
	if(ens[0]==true){
		HBM_channelB0[offsets[0]].data[0] = data[0][16];
		HBM_channelB0[offsets[0]].data[1] = data[0][17];
		HBM_channelB0[offsets[0]].data[2] = data[0][18];
		HBM_channelB0[offsets[0]].data[3] = data[0][19];
		HBM_channelB0[offsets[0]].data[4] = data[0][20];
		HBM_channelB0[offsets[0]].data[5] = data[0][21];
		HBM_channelB0[offsets[0]].data[6] = data[0][22];
		HBM_channelB0[offsets[0]].data[7] = data[0][23];
		HBM_channelB0[offsets[0]].data[8] = data[0][24];
		HBM_channelB0[offsets[0]].data[9] = data[0][25];
		HBM_channelB0[offsets[0]].data[10] = data[0][26];
		HBM_channelB0[offsets[0]].data[11] = data[0][27];
		HBM_channelB0[offsets[0]].data[12] = data[0][28];
		HBM_channelB0[offsets[0]].data[13] = data[0][29];
		HBM_channelB0[offsets[0]].data[14] = data[0][30];
		HBM_channelB0[offsets[0]].data[15] = data[0][31];
	
	}
	return;
}
void master_retrievemany_Avec(unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	data[0][0] = HBM_channelA0[offsets[0]].data[0];
	data[0][1] = HBM_channelA0[offsets[0]].data[1];
	data[0][2] = HBM_channelA0[offsets[0]].data[2];
	data[0][3] = HBM_channelA0[offsets[0]].data[3];
	data[0][4] = HBM_channelA0[offsets[0]].data[4];
	data[0][5] = HBM_channelA0[offsets[0]].data[5];
	data[0][6] = HBM_channelA0[offsets[0]].data[6];
	data[0][7] = HBM_channelA0[offsets[0]].data[7];
	data[0][8] = HBM_channelA0[offsets[0]].data[8];
	data[0][9] = HBM_channelA0[offsets[0]].data[9];
	data[0][10] = HBM_channelA0[offsets[0]].data[10];
	data[0][11] = HBM_channelA0[offsets[0]].data[11];
	data[0][12] = HBM_channelA0[offsets[0]].data[12];
	data[0][13] = HBM_channelA0[offsets[0]].data[13];
	data[0][14] = HBM_channelA0[offsets[0]].data[14];
	data[0][15] = HBM_channelA0[offsets[0]].data[15];
	
	return;
}
void master_retrievemany_Bvec(unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
 /////////////////
	data[0][16] = HBM_channelB0[offsets[0]].data[0];
 /////////////////
	data[0][17] = HBM_channelB0[offsets[0]].data[1];
 /////////////////
	data[0][18] = HBM_channelB0[offsets[0]].data[2];
 /////////////////
	data[0][19] = HBM_channelB0[offsets[0]].data[3];
 /////////////////
	data[0][20] = HBM_channelB0[offsets[0]].data[4];
 /////////////////
	data[0][21] = HBM_channelB0[offsets[0]].data[5];
 /////////////////
	data[0][22] = HBM_channelB0[offsets[0]].data[6];
 /////////////////
	data[0][23] = HBM_channelB0[offsets[0]].data[7];
 /////////////////
	data[0][24] = HBM_channelB0[offsets[0]].data[8];
 /////////////////
	data[0][25] = HBM_channelB0[offsets[0]].data[9];
 /////////////////
	data[0][26] = HBM_channelB0[offsets[0]].data[10];
 /////////////////
	data[0][27] = HBM_channelB0[offsets[0]].data[11];
 /////////////////
	data[0][28] = HBM_channelB0[offsets[0]].data[12];
 /////////////////
	data[0][29] = HBM_channelB0[offsets[0]].data[13];
 /////////////////
	data[0][30] = HBM_channelB0[offsets[0]].data[14];
 /////////////////
	data[0][31] = HBM_channelB0[offsets[0]].data[15];
	
	
	return;
}

unsigned int master_retrieve(unsigned int offset, unsigned int v, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int datas[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	master_retrieve_vec(offset, datas, HBM_channelA, HBM_channelB, inst);
	return datas[v];
}
void master_retrievemany(unsigned int offsets[NUM_VALID_PEs], unsigned int v, unsigned int data[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	unsigned int datas[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete dim=0
	master_retrievemany_vec(offsets, datas,  HBM_channelA0, HBM_channelB0);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
		data[n] = datas[n][v];
	}
}

void load_edgemaps(unsigned int index, unsigned int base_offset, map_t edge_map[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
#pragma HLS INLINE
	index = index * 2; // '*2' because data is dual: i.e., offset and size
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 9601::", base_offset + (index / HBM_AXI_PACK_SIZE), ((1 << 28)/4)/16, index, HBM_CHANNEL_PACK_SIZE, NAp);
	#endif 
	edge_map[0].offset = HBM_channelA0[base_offset + (index / HBM_AXI_PACK_SIZE)].data[(index % HBM_AXI_PACK_SIZE)];
	edge_map[0].size = HBM_channelA0[base_offset + ((index + 1) / HBM_AXI_PACK_SIZE)].data[((index + 1) % HBM_AXI_PACK_SIZE)];
	
	return;
}
void save_edgemaps(unsigned int index, unsigned int base_offset, map_t edge_map[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
#pragma HLS INLINE
	index = index * 2; // '*2' because data is dual: i.e., offset and size
	HBM_channelA0[base_offset + (index / HBM_AXI_PACK_SIZE)].data[(index % HBM_AXI_PACK_SIZE)] = edge_map[0].offset;
	HBM_channelA0[base_offset + ((index + 1) / HBM_AXI_PACK_SIZE)].data[((index + 1) % HBM_AXI_PACK_SIZE)] = edge_map[0].size;
	
	return;
}

map_t load_vupdate_map(unsigned int offset, unsigned int index,  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
#pragma HLS INLINE
	map_t data;
	data.offset = HBM_channelA0[offset + index].data[0];
	data.size = HBM_channelA0[offset + index].data[1];
	return data; 
}
void save_vupdate_map(unsigned int offset, unsigned int index, map_t data,  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
#pragma HLS INLINE
	HBM_channelA0[offset + index].data[0] = data.offset;
	HBM_channelA0[offset + index].data[1] = data.size;
	return;
}

#ifdef YYYYYYYY
void loadstate_vertexinterval(unsigned int index, unsigned int base_offset, map_t edge_map[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
#pragma HLS INLINE
	state.done = HBM_channelA[base_offset + (index / HBM_AXI_PACK_SIZE)].data[(index % HBM_AXI_PACK_SIZE)];
	return;
}
void savestate_vertexinterval(unsigned int index, unsigned int base_offset, map_t edge_map[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
#pragma HLS INLINE
	HBM_channelA[base_offset + (index / HBM_AXI_PACK_SIZE)].data[(index % HBM_AXI_PACK_SIZE)] = state.done;
	return;
}
#endif 

//////////////////////////////////////////////////////////////////
void insert_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB){
	#pragma HLS INLINE
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data_[2*v] = data[v].key;
		data_[2*v + 1] = data[v].value;
	}
	master_centerinsert_vec(t, data_, HBM_centerA, HBM_centerB);
}
void retrieve_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB){
	#pragma HLS INLINE
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	master_centerretrieve_vec(t, data_, HBM_centerA, HBM_centerB); 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v].key = data_[2*v];
		data[v].value = data_[2*v+1];
	}
	return; 
}

////////////////////////////////////////////////////////////////
unsigned int dretrieve_globalparams(unsigned int base_offset__, unsigned int index, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	return master_retrieve(base_offset__ + index, 0, HBM_channelA, HBM_channelB, inst); // NEW
}

unsigned int dretrieve_vptrdram(unsigned int base_offset__, unsigned int index, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
	checkoutofbounds("acts_kernel::ERROR 901::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, index, HBM_CHANNEL_PACK_SIZE, NAp);
	#endif 
	return master_retrieve(base_offset__ + (index / HBM_CHANNEL_PACK_SIZE), index % HBM_CHANNEL_PACK_SIZE, HBM_channelA, HBM_channelB, inst);
}	
void dretrievemany_vptrdram(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	data[0] = dretrieve_vptrdram(base_offset__, offsets[0], HBM_channelA0, HBM_channelB0, 0);
	return;
}

map_t dretrieve_actpackvptrdram(unsigned int base_offset__, unsigned int p_u, unsigned int llp_set, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int offset = ((p_u * MAX_NUM_LLPSETS) + llp_set) * 2; 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS];
	checkoutofbounds("acts_kernel::ERROR 702::", offset / HBM_CHANNEL_PACK_SIZE, wwsize, NAp, NAp, NAp);
	#endif 
	map_t map;
	unsigned int i = offset / HBM_CHANNEL_PACK_SIZE;
	unsigned int j = offset % HBM_CHANNEL_PACK_SIZE;
	map.offset = master_retrieve(base_offset__ + i, j, HBM_channelA, HBM_channelB, inst);
	map.size = master_retrieve(base_offset__ + i, j + 1, HBM_channelA, HBM_channelB, inst);
	return map;
}
void dretrievemany_actpackvptrdram(unsigned int base_offset__, unsigned int offset__, map_t maps[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int offset = offset__ * 2; 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS];
	checkoutofbounds("acts_kernel::ERROR 702::", offset / HBM_CHANNEL_PACK_SIZE, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int i = offset / HBM_CHANNEL_PACK_SIZE;
	unsigned int j = offset % HBM_CHANNEL_PACK_SIZE;
	
	unsigned int data_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + i;
	}
	master_retrievemany(offsets_, j, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		maps[n].offset = data_[n];
	}
	master_retrievemany(offsets_, j + 1, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		maps[n].size = data_[n];
	}
	return;
}

void dretrievemany_edgeupdates_base(unsigned int offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge_update_vec_dt edge_update_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE]; // NEW
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){
			edge_update_vecs[n].data[v].srcvid = data_[n][2*v];
			edge_update_vecs[n].data[v].dstvid = data_[n][2*v+1];
		}
	}
	return;
}
void dretrievemany_rawedgeupdates(unsigned int offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge_update_vec_dt edge_update_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES];
	checkoutofbounds("acts_kernel::ERROR 71263::", offsets[0], wwsize, NAp, NAp, NAp);
	#endif 	
	return dretrievemany_edgeupdates_base(offset__, offsets, t, edge_update_vecs,  HBM_channelA0, HBM_channelB0);
}
void dretrievemany_edgeupdates(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge_update_vec_dt edge_update_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES];
	checkoutofbounds("acts_kernel::ERROR 71723::", offsets[0], wwsize, NAp, NAp, NAp);
	#endif 
	return dretrievemany_edgeupdates_base(base_offset__, offsets, t, edge_update_vecs,  HBM_channelA0, HBM_channelB0);
}
void dinsertmany_edgeupdates(unsigned int offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge_update_vec_dt edge_update_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE]; // NEW
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	bool ens_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=ens_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + offsets[n] + t;
	}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){
			data_[n][2*v] = edge_update_vecs[n].data[v].srcvid;
			data_[n][2*v+1] = edge_update_vecs[n].data[v].dstvid;
		}
	}
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = true;
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0);
	return;
}

edge3_vec_dt dretrieve_csredges(unsigned int base_offset__, unsigned int index, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE 
	edge3_vec_dt edge3_vec;
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSREDGES];
	checkoutofbounds("acts_kernel::ERROR 703::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data complete
	master_retrieve_vec(base_offset__ + index, data, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		edge3_vec.data[v].srcvid = data[2*v];
		edge3_vec.data[v].dstvid = (data[2*v+1] >> 1) & 0xFFFFFFFF;
		edge3_vec.data[v].weight = 1;
		edge3_vec.data[v].valid = data[2*v+1] & 0x1;
	}
	return edge3_vec;
}

void dinsertmanytoA_actpackedges(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 			
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 704f::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
		
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL	
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){	
		#pragma HLS UNROLL	
			data_[n][v] = (edge3_vecs[n].data[v].srcvid << MAXNUMBITS2_ACTPACK_DESTVID) | edge3_vecs[n].data[v].dstvid;
		}	
	}
	master_insertmany_Avec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	return;
}
void dinsertmanytoB_actpackedges(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 			
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 704f::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	bool ens_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=ens_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = true;
	}
		
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL	
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){	
		#pragma HLS UNROLL	
			data_[n][EDGE_PACK_SIZE + v] = (edge3_vecs[n].data[v].srcvid << MAXNUMBITS2_ACTPACK_DESTVID) | edge3_vecs[n].data[v].dstvid;
		}	
	}
	master_insertmany_Bvec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0);
	return;
}

void dretrievemanyfromA_actpackedges(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 7042ft::", offsets[0] + t, wwsize, offsets[0], t, NAp);
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	
	master_retrievemany_Avec(offsets_, data_,  HBM_channelA0, HBM_channelB0);		

	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL	
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){	
		#pragma HLS UNROLL	
			edge3_vecs[n].data[v].srcvid = (data_[n][v] >> MAXNUMBITS2_ACTPACK_DESTVID) & MAXLOCALVALUE2_ACTPACK_SRCVID;		
			edge3_vecs[n].data[v].dstvid = data_[n][v] & MAXLOCALVALUE2_ACTPACK_DESTVID;		
			edge3_vecs[n].data[v].weight = (data_[n][v] >> (MAXNUMBITS2_ACTPACK_DESTVID + MAXNUMBITS2_ACTPACK_SRCVID)) & MAXLOCALVALUE2_ACTPACK_EDGEID;
			edge3_vecs[n].data[v].valid = NAp;
			if(edge3_vecs[n].data[v].srcvid == INVALIDDATA_SRCVID){ edge3_vecs[n].data[v].srcvid = INVALIDDATA; } //////////////////////////////// CRITICAL
			if(edge3_vecs[n].data[v].dstvid == INVALIDDATA_DESTVID){ edge3_vecs[n].data[v].dstvid = INVALIDDATA; }
		}	
	}
	return;
}
void dretrievemanyfromB_actpackedges(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 7041ft::", offsets[0] + t, wwsize, offsets[0], t, NAp);
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	
	master_retrievemany_Bvec(offsets_, data_,  HBM_channelA0, HBM_channelB0);		
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL	
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){	
		#pragma HLS UNROLL	
			edge3_vecs[n].data[v].srcvid = (data_[n][HBM_AXI_PACK_SIZE + v] >> MAXNUMBITS2_ACTPACK_DESTVID) & MAXLOCALVALUE2_ACTPACK_SRCVID;		
			edge3_vecs[n].data[v].dstvid = data_[n][HBM_AXI_PACK_SIZE + v] & MAXLOCALVALUE2_ACTPACK_DESTVID;		
			edge3_vecs[n].data[v].weight = (data_[n][HBM_AXI_PACK_SIZE + v] >> (MAXNUMBITS2_ACTPACK_DESTVID + MAXNUMBITS2_ACTPACK_SRCVID)) & MAXLOCALVALUE2_ACTPACK_EDGEID;
			edge3_vecs[n].data[v].valid = NAp;
			if(edge3_vecs[n].data[v].srcvid == INVALIDDATA_SRCVID){ edge3_vecs[n].data[v].srcvid = INVALIDDATA; } //////////////////////////////// CRITICAL
			if(edge3_vecs[n].data[v].dstvid == INVALIDDATA_DESTVID){ edge3_vecs[n].data[v].dstvid = INVALIDDATA; }
		}	
	}
	return;
}

void dinsertmany_updatesdram(unsigned int offsets[NUM_VALID_PEs], keyvalue_t data[NUM_VALID_PEs][EDGE_PACK_SIZE], bool ens[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES];
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){ checkoutofbounds("acts_kernel::ERROR 708a::", offsets[n], globalparams_debug[GLOBALPARAMSCODE__BASEOFFSET__VDATAS], NAp, NAp, NAp); }
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	bool ens_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=ens_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = ens[n][0];
	}	

	for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
	#pragma HLS UNROLL
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data_[n][v] = 0;
		}
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			if(ens[n][v] == true){
				data_[n][EDGE_PACK_SIZE + v] = ((data[n][v].value & MAXLOCALVALUE2_ACTPACK_SRCVID) << MAXNUMBITS2_ACTPACK_DESTVID) | (data[n][v].key & MAXLOCALVALUE2_ACTPACK_DESTVID);
			} else {
				data_[n][EDGE_PACK_SIZE + v] = INVALIDDATA;
			}
		}
	}
	master_insertmany_Bvec(offsets, ens_, data_,  HBM_channelA0, HBM_channelB0); // CRITICAL FIXME. the issue.
}
void dretrievemany_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES];
	#endif 

	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + index;
	}
	
	master_retrievemany_Bvec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[n].data[v].key = data_[n][EDGE_PACK_SIZE + v] & MAXLOCALVALUE2_ACTPACK_DESTVID;		
			data[n].data[v].value = (data_[n][EDGE_PACK_SIZE + v] >> MAXNUMBITS2_ACTPACK_DESTVID) & MAXLOCALVALUE2_ACTPACK_SRCVID;	

			if(data[n].data[v].key == INVALIDDATA_DESTVID){ data[n].data[v].key = INVALIDDATA; }  
			if(data[n].data[v].value == INVALIDDATA_SRCVID){ data[n].data[v].value = INVALIDDATA; }			
		}
	}
	return; 
}

void dinsertmany_vdatadram(unsigned int offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, vprop_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 711b::", offsets[0], globalparams_debug[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE]; // NEW
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	bool ens_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=ens_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offsets[n];
	}
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = true;
	}
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data_[n][2*v] = datas[n][v].prop;
			data_[n][2*v+1] = datas[n][v].degree;
		}
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0);
	return;
}
void dretrievemany_vdatadram(unsigned int offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, vprop_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 712::", offsets[0], wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE]; // NEW
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){
			datas[n][v].prop = data_[n][2*v];
			datas[n][v].degree = data_[n][2*v+1];
		}
	}
	return;
}

void dretrievemany_cfrontierdram_tmp(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, keyvalue_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
	checkoutofbounds("acts_kernel::ERROR 714a::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){
		#pragma HLS UNROLL
			datas[n][v].key = data_[n][2*v];
			datas[n][v].value = data_[n][2*v+1];
		}
	}
	return; 
}

void dinsertmany_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int offsets[NUM_VALID_PEs], unsigned int t, keyvalue_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE], bool ens[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	bool ens_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=ens_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offsets[n];
	}
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = ens[n][0];
	}
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL	
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			if(ens[n][v] == true){
				data_[n][2*v] = datas[n][v].key;
				data_[n][2*v+1] = datas[n][v].value;
			} else {
				data_[n][2*v] = INVALIDDATA;
				data_[n][2*v+1] = INVALIDDATA;
			}
		}
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0);
}	

//////////////////////////////////////////////////////////////////
void insert_edgeupdatesbuffer(unsigned int v, unsigned int index, edge_update_type data, MY_IFDEF_EDGEUPDATESBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 992::", index, EDGE_UPDATES_DRAMBUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	URAM_edges[v][index] = data;
}
edge_update_type retrieve_edgeupdatesbuffer(unsigned int v, unsigned int index, MY_IFDEF_EDGEUPDATESBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 993::", index, EDGE_UPDATES_DRAMBUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return URAM_edges[v][index];
}

void insert_vdatabuffer(unsigned int v, unsigned int index, vprop_t data, MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 102::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif
	URAM_vprop[v][index >> POW_VALID_VDATA] = data; 
}
vprop_t retrieve_vdatabuffer(unsigned int v, unsigned int index, MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 103::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif
	return URAM_vprop[v][index >> POW_VALID_VDATA];
}
void insertvec_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 104::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		URAM_vprop[v][index >> POW_VALID_VDATA] = data[v];
	}
}
void retrievevec_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 105::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = URAM_vprop[v][index >> POW_VALID_VDATA];
	}
}
void retrieveandresetvec_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 105::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v].prop = URAM_vprop[v][index >> POW_VALID_VDATA].prop; 
		data[v].degree = INVALIDMASK;  
	}
}

void insert_edgesbuffer(unsigned int v, unsigned int index, edge3_type data, MY_IFDEF_EDGESBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 192::", index, EDGE_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	edges_buffer[v][index] = data;
}
edge3_type retrieve_edgesbuffer(unsigned int v, unsigned int index, MY_IFDEF_EDGESBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 193::", index, EDGE_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return edges_buffer[v][index];
}

void insert_updatestmpbuffer(unsigned int v, unsigned int index, keyvalue_t data, MY_IFDEF_UPDATESTMPBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 107::", index, UPDATES_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	updates_buffer2[v][index] = data;
}
keyvalue_t retrieve_updatestmpbuffer(unsigned int v, unsigned int index, MY_IFDEF_UPDATESTMPBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 108::", index, UPDATES_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return updates_buffer2[v][index];
}

void insert_vptrbuffer(unsigned int t, vtr_t data, MY_IFDEF_VPTRBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 129::", t, VPTR_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	vptr_buffer[t] = data;
}
vtr_t retrieve_vptrbuffer(unsigned int t, MY_IFDEF_VPTRBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 130::", t, VPTR_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return vptr_buffer[t];
}

void insert_nfrontierbuffer(unsigned int v, unsigned int t, frontier_t data, MY_IFDEF_NFRONTIER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 113::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	nfrontier_buffer[v][t] = data;
}
frontier_t retrieve_cfrontierbuffer_tmp(unsigned int v, unsigned int t, MY_IFDEF_CFRONTIER_TMP()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 114::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return URAM_frontiers[v][t];
}
void insertvec_cfrontierbuffer_tmp(unsigned int t, frontier_t data[EDGE_PACK_SIZE], MY_IFDEF_CFRONTIER_TMP()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 115::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		URAM_frontiers[v][t] = data[v];
	}
}
void retrievevec_nfrontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], MY_IFDEF_NFRONTIER()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 116::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = nfrontier_buffer[v][t];
	}
	return; 
}

void insert_statstmpbuffer(unsigned int t, unsigned int v, unsigned int data, MY_IFDEF_STATSBUFFERMASKBASED_TMP()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 121::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	statsbuffer_maskbased_tmp[v][t] = data;
}
unsigned int retrieve_statstmpbuffer(unsigned int t, unsigned int v, MY_IFDEF_STATSBUFFERMASKBASED_TMP()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 1242::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return statsbuffer_maskbased_tmp[v][t]; 
}
void insertvec_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], MY_IFDEF_STATSBUFFERMASKBASED_TMP()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 123::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		statsbuffer_maskbased_tmp[v][t] = data[v];
	}
}
void retrievevec_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], MY_IFDEF_STATSBUFFERMASKBASED_TMP()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 124::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = statsbuffer_maskbased_tmp[v][t];
	}
	return; 
}

void insert_statsbuffer(unsigned int t, unsigned int v, unsigned int data, MY_IFDEF_STATSBUFFERMASKBASED()){	
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 125::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	statsbuffer_maskbased[v][t] = data;
}
unsigned int retrieve_statsbuffer(unsigned int t, unsigned int v, MY_IFDEF_STATSBUFFERMASKBASED()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 126::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	return statsbuffer_maskbased[v][t]; 
}

void insert_stats2buffer(unsigned int t, unsigned int v, unsigned int data, MY_IFDEF_STATSBUFFERIDBASED()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 127::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	statsbuffer_idbased[v][t] = data;
}
unsigned int retrieve_stats2buffer(unsigned int t, unsigned int v, MY_IFDEF_STATSBUFFERIDBASED()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 128::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	return statsbuffer_idbased[v][t]; 
}		

#ifndef ___RUNNING_FPGA_SYNTHESIS___
// #define _DEBUGMODE_KERNELPRINTS4_CREATEACTPACT
#ifdef ___RUNNING_FPGA_SYNTHESIS___
#define MY_IFDEF_SAVE_PARTIALLYPREPARED_EDGEUPDATES() int SAVE_partiallyprepared_edgeupdates(unsigned int base_offset, map_t count[EDGE_PACK_SIZE][EDGE_PACK_SIZE], edge_update_type URAM_edges[EDGE_PACK_SIZE][EDGE_UPDATES_DRAMBUFFER_SIZE], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int _MAX_BUFFER_SIZE, unsigned int globalparams[GLOBALBUFFER_SIZE], unsigned int _NUMCLOCKCYCLES_[2][32])
#define MY_IFDEF_SAVE_FULLYPREPARED_EDGEUPDATES() void SAVE_fullyprepared_edgeupdates(unsigned int base_offset, map_t count[EDGE_PACK_SIZE][EDGE_PACK_SIZE], edge_update_type URAM_edges[EDGE_PACK_SIZE][EDGE_UPDATES_DRAMBUFFER_SIZE], map_t * edgeupdate_maps, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int globalparams[GLOBALBUFFER_SIZE], unsigned int _NUMCLOCKCYCLES_[2][32])
#define MY_IFDEF_CREATE_ACTPACK() unsigned int create_actpack(action_t action, unsigned int destedgeupdates_baseoffset, unsigned int destedgeupdateptrs_baseoffset, edge_update_type URAM_edges[NUM_VALID_PEs][EDGE_PACK_SIZE][EDGE_UPDATES_DRAMBUFFER_SIZE], unsigned int _MAX_BUFFER_SIZE, unsigned int _MAX_WORKBUFFER_SIZE, unsigned int globalparams[GLOBALBUFFER_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, unsigned int _NUMCLOCKCYCLES_[2][32])
#else
#define MY_IFDEF_SAVE_PARTIALLYPREPARED_EDGEUPDATES() int SAVE_partiallyprepared_edgeupdates(unsigned int base_offset, map_t count[EDGE_PACK_SIZE][EDGE_PACK_SIZE], edge_update_type * URAM_edges[EDGE_PACK_SIZE], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int _MAX_BUFFER_SIZE, unsigned int globalparams[GLOBALBUFFER_SIZE], unsigned int _NUMCLOCKCYCLES_[2][32])
#define MY_IFDEF_SAVE_FULLYPREPARED_EDGEUPDATES() void SAVE_fullyprepared_edgeupdates(unsigned int base_offset, map_t count[EDGE_PACK_SIZE][EDGE_PACK_SIZE], edge_update_type * URAM_edges[EDGE_PACK_SIZE], map_t * edgeupdate_maps, map_t edgeupdate2_maps[NUM_FPGAS][NUM_LLP_PER_LLPSET], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int globalparams[GLOBALBUFFER_SIZE], unsigned int _NUMCLOCKCYCLES_[2][32])
#define MY_IFDEF_CREATE_ACTPACK() unsigned int create_actpack(action_t action, unsigned int destedgeupdates_baseoffset, unsigned int destedgeupdateptrs_baseoffset, edge_update_type * URAM_edges[NUM_VALID_PEs][EDGE_PACK_SIZE], unsigned int _MAX_BUFFER_SIZE, unsigned int _MAX_WORKBUFFER_SIZE, unsigned int globalparams[GLOBALBUFFER_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, vector<edge3_type> (&final_edge_updates)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS], unsigned int _NUMCLOCKCYCLES_[2][32])			
#endif

unsigned int TOTALL___;
unsigned int TOTALL_INVALIDS___;

MY_IFDEF_SAVE_PARTIALLYPREPARED_EDGEUPDATES(){ 
	edge_update_type edge_update_in[NUM_LLP_PER_LLPSET];	
	edge_update_type edge_update_out[NUM_LLP_PER_LLPSET];
	unsigned int offset_p[NUM_LLP_PER_LLPSET];
	unsigned int p_[NUM_LLP_PER_LLPSET];
	unsigned int index = 0;
	unsigned int max[NUM_LLP_PER_LLPSET]; 
	unsigned int index_t = 0;
	for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ max[llp_id] = 0; for(unsigned int v=0; v<NUM_LLP_PER_LLPSET; v++){ if(max[llp_id] < count[v][llp_id].size){ max[llp_id] = count[v][llp_id].size; }}}
	// unsigned int max_ = 0; for(unsigned int v=0; v<NUM_LLP_PER_LLPSET; v++){ if(max_ += max[v]); }
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
		rearrangeLayoutVx16(rotateby, edge_update_in, edge_update_out); 		
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
		// checkoutofbounds("acts_kernel::ERROR 221815::", index, globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES], base_offset, index_t, max_);
		checkoutofbounds("acts_kernel::ERROR 221815::", index, _MAX_BUFFER_SIZE, base_offset, index_t, max_);
		#endif 
		update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS//4
	cout<<">>> save_partiallyprepared_edgeupdates (after-cyclic-rotate): index: "<<index<<endl;
	#endif 
	return index;
}	

MY_IFDEF_SAVE_FULLYPREPARED_EDGEUPDATES(){
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
		
		#ifndef ___RUNNING_FPGA_SYNTHESIS___
		unsigned int fpga_sel[NUM_FPGAS][EDGE_PACK_SIZE];
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ fpga_sel[fpga][v] = 0; }}
		#endif 
	
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
			
			#ifndef ___RUNNING_FPGA_SYNTHESIS___
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(edge_update[v].dstvid != INVALIDDATA){ unsigned int fpga = edge_update[v].dstvid % NUM_FPGAS; fpga_sel[fpga][v] += 1; }}
			#endif 
			
			#ifdef ___RUNNING_FPGA_SYNTHESIS___
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
			#else 
			if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 0){ 
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					if(edge_update[v].srcvid != INVALIDDATA){ edge_update[v].srcvid = edge_update[v].srcvid; } else { edge_update[v].srcvid = INVALIDDATA_SRCVID; }
					if(edge_update[v].dstvid != INVALIDDATA){ edge_update[v].dstvid = edge_update[v].dstvid; } else { edge_update[v].dstvid = INVALIDDATA_DESTVID; }
					HBM_channelA[offset + t].data[v] = ((1 & MAXLOCALVALUE2_ACTPACK_EDGEID) << (MAXNUMBITS2_ACTPACK_DESTVID + MAXNUMBITS2_ACTPACK_SRCVID)) | ((edge_update[v].srcvid & MAXLOCALVALUE2_ACTPACK_SRCVID) << MAXNUMBITS2_ACTPACK_DESTVID) | (edge_update[v].dstvid & MAXLOCALVALUE2_ACTPACK_DESTVID);		
					HBM_channelB[offset + t].data[v] = ((1 & MAXLOCALVALUE2_ACTPACK_EDGEID) << (MAXNUMBITS2_ACTPACK_DESTVID + MAXNUMBITS2_ACTPACK_SRCVID)) | ((edge_update[v].srcvid & MAXLOCALVALUE2_ACTPACK_SRCVID) << MAXNUMBITS2_ACTPACK_DESTVID) | (edge_update[v].dstvid & MAXLOCALVALUE2_ACTPACK_DESTVID);
				}
				#ifndef FPGA_IMPL
				// checkoutofbounds("acts_kernel::ERROR 221215::", offset + t, globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS], offset, t, NAp);
				checkoutofbounds("acts_kernel::ERROR 221215::", offset + t, HBM_CHANNEL_SIZE, offset, t, NAp);
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
			#endif 
			// TOTALL___ += 1;
			update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
		}		
		
		edgeupdate_maps[llp_id].size += max;	
		#ifndef ___RUNNING_FPGA_SYNTHESIS___
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
			unsigned int max_val = 0; 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max_val < fpga_sel[fpga][v]){ max_val = fpga_sel[fpga][v]; }}
			edgeupdate2_maps[fpga][llp_id].size += max_val; 
		}
		#endif 
	}
	// exit(EXIT_SUCCESS);
}

MY_IFDEF_CREATE_ACTPACK(){	
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
	
	TOTALL___ = 0;
	TOTALL_INVALIDS___ = 0;
	
	map_t vupdates_map[MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS];
	if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 0){ 
		for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u++){	
			for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; t++){	
				vupdates_map[p_u][t].offset = 0; vupdates_map[p_u][t].size = 0;
			}
		}
	}
	
	#ifndef ___RUNNING_FPGA_SYNTHESIS___
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
	#endif 
	
	CREATE_ACTPACK_BASELOOP1: for(unsigned int p_u=action.start_pu; p_u<action.start_pu + action.size_pu; p_u+=action.skip_pu){ 
	// CREATE_ACTPACK_BASELOOP1: for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u+=1){ // FIXME.
		#ifndef ___RUNNING_FPGA_SYNTHESIS___
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
			
			#ifndef ___RUNNING_FPGA_SYNTHESIS___
			map_t edgeupdate2_maps[NUM_VALID_PEs][NUM_FPGAS][NUM_LLP_PER_LLPSET]; // FIXME. ADD HLS PRAGMAS?
			for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ 
				for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
					for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
						edgeupdate2_maps[n][fpga][llp_id].offset = 0; edgeupdate2_maps[n][fpga][llp_id].size = 0;
					}
				}
			}
			#endif 
			
			map_t rawedge_maps[NUM_VALID_PEs];
			#ifdef ___CREATE_ACTPACK_FROM_VECTOR___
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ rawedge_maps[n].size = final_edge_updates[n][p_u][llp_set].size() / EDGE_PACK_SIZE; }
			#else 
			load_edgemaps((p_u * MAX_NUM_LLPSETS) + llp_set, globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS], rawedge_maps,  HBM_channelA0, HBM_channelB0);
			#endif 
			
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
				if(c == 1){
					LOAD_EDGEMAPS_LOOP1: for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){	
						map_t edge_map_vec[NUM_VALID_PEs];
						load_edgemaps((p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id, destedgeupdateptrs_baseoffset, edge_map_vec,  HBM_channelA0, HBM_channelB0);
						for(unsigned int n=0; n<NUM_VALID_PEs; n++){
						#pragma HLS UNROLL
							edgeupdate_maps[n][llp_id] = edge_map_vec[n];
							// edgeupdate2_maps[n][llp_id] = edge_map_vec[n];
						}
					}	
				}
					
				// partition stage 1
				CREATE_ACTPACK_LOOP1: for(unsigned int t=0; t<work_size; t++){ // _MAX_BUFFER_SIZE, 1024, work_size
				#pragma HLS PIPELINE II=2
					#ifdef ___CREATE_ACTPACK_FROM_VECTOR___
					for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							edge_update_vecs[n].data[v].srcvid = final_edge_updates[n][p_u][llp_set][t*EDGE_PACK_SIZE + v].srcvid % MAX_UPARTITION_SIZE; 
							edge_update_vecs[n].data[v].dstvid = final_edge_updates[n][p_u][llp_set][t*EDGE_PACK_SIZE + v].dstvid % MAX_UPARTITION_SIZE; 
						}
					}
					#else 
					dretrievemany_rawedgeupdates(base_offset, offsets_rawedgeupdates, t, edge_update_vecs,  HBM_channelA0, HBM_channelB0);
					#endif 
					
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
								checkoutofbounds("acts_kernel::ERROR 2215::", p, EDGE_PACK_SIZE, NAp, NAp, NAp);
								#endif 
								if(data != INVALIDDATA){ count[inst][v][p].size += 1; }
							}
						}
					}
					
					update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
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
				#pragma HLS PIPELINE II=2
					#ifdef ___CREATE_ACTPACK_FROM_VECTOR___
					for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							edge_update_vecs[n].data[v].srcvid = final_edge_updates[n][p_u][llp_set][t*EDGE_PACK_SIZE + v].srcvid % MAX_UPARTITION_SIZE; 
							edge_update_vecs[n].data[v].dstvid = final_edge_updates[n][p_u][llp_set][t*EDGE_PACK_SIZE + v].dstvid % MAX_UPARTITION_SIZE; 
						}
					}
					#else 
					dretrievemany_rawedgeupdates(base_offset, offsets_rawedgeupdates, t, edge_update_vecs,  HBM_channelA0, HBM_channelB0);
					#endif 
					for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
					#pragma HLS UNROLL
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							if(edge_update_vecs[inst].data[v].srcvid != INVALIDDATA){ // NEWCHANGE.
								unsigned int data = 0; if(c==0){ data = edge_update_vecs[inst].data[v].srcvid; } else { data = edge_update_vecs[inst].data[v].dstvid; }
								unsigned int p = data % EDGE_PACK_SIZE;

								#ifndef FPGA_IMPL
								checkoutofbounds("acts_kernel::ERROR 2215::", p, EDGE_PACK_SIZE, NAp, NAp, NAp);
								#endif 
								URAM_edges[inst][v][count[inst][v][p].offset + count[inst][v][p].size] = edge_update_vecs[inst].data[v];
								if(data != INVALIDDATA){ count[inst][v][p].size += 1; }
								
								#ifdef _DEBUGMODE_KERNELPRINTS//4	
								if(inst==0 && t<8 && v==0){ cout<<"[finalized edge update: p: "<<p<<", srcvid: "<<edge_update_vecs[inst].data[v].srcvid<<", dstvid: "<<edge_update_vecs[inst].data[v].dstvid<<"]"<<endl; }						
								#endif 
							}
						}
					}
					update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
				}
			
				// store edge updates
				if(c==0){ 
	
					temp_size[0] = SAVE_partiallyprepared_edgeupdates(globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES], count[0], URAM_edges[0], HBM_channelA0, HBM_channelB0, _MAX_BUFFER_SIZE, globalparams, _NUMCLOCKCYCLES_);	
	
				} else {
					#ifdef _DEBUGMODE_KERNELPRINTS4_CREATEACTPACT
					for(unsigned int n=0; n<NUM_VALID_PEs; n++){ for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ cout<<"prepare-edge-updates (before): edgeupdate_maps[n]["<<llp_id<<"]: p_u: "<<p_u<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", offset: "<<edgeupdate_maps[n][llp_id].offset<<", size: "<<edgeupdate_maps[n][llp_id].size<<""<<endl; }}
					#endif 	
	
					SAVE_fullyprepared_edgeupdates(destedgeupdates_baseoffset, count[0], URAM_edges[0], edgeupdate_maps[0], edgeupdate2_maps[0], HBM_channelA0, HBM_channelB0, globalparams, _NUMCLOCKCYCLES_);	
	
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
						#ifndef ___RUNNING_FPGA_SYNTHESIS___
						vupdates_map[p_u][llp_set].size += edgeupdate_maps[0][llp_id].size;
						for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ vupdates2_map[p_u][fpga][llp_set].size += edgeupdate2_maps[0][fpga][llp_id].size; }
						#endif 
					
						save_edgemaps((p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id, destedgeupdateptrs_baseoffset, edge_map_vec,  HBM_channelA0, HBM_channelB0);
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
	
	#ifndef ___RUNNING_FPGA_SYNTHESIS___
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
			// save_vupdate_map(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], t, vertex_updates_map[t],  HBM_channelA0, HBM_channelB0);
			for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; t++){		
				save_vupdate_map(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], (fpga * MAX_NUM_LLPSETS) + t, vertex_updates2_map[fpga][t],  HBM_channelA0, HBM_channelB0);
			}
		}	
	}
	#endif	
	
	// exit(EXIT_SUCCESS);
	return running_offset[0];
}
// exit(EXIT_SUCCESS);
#endif 

void import_frontiers(unsigned int chunk, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_import, action_t action, unsigned int _NUMCLOCKCYCLES_[2][32]){
	#ifdef _DEBUGMODE_KERNELPRINTS4
	cout<<"acts_kernel::run:: importing "<<action.size_import_export<<" 32bit-integers (Host -> FPGA)..."<<endl;
	#endif 
	unsigned int offset_c = 0 * chunk; // FIXME
	IMPORT_LOOP1: for(unsigned int t=0; t<action.size_import_export; t++){ 
	#pragma HLS PIPELINE II=1
		// import (Host -> FPGA)
		HBM_centerA[offset_c + t].data[0] = HBM_import[t].data[0];
		HBM_centerA[offset_c + t].data[1] = HBM_import[t].data[1];
		HBM_centerA[offset_c + t].data[2] = HBM_import[t].data[2];
		HBM_centerA[offset_c + t].data[3] = HBM_import[t].data[3];
		HBM_centerA[offset_c + t].data[4] = HBM_import[t].data[4];
		HBM_centerA[offset_c + t].data[5] = HBM_import[t].data[5];
		HBM_centerA[offset_c + t].data[6] = HBM_import[t].data[6];
		HBM_centerA[offset_c + t].data[7] = HBM_import[t].data[7];
		HBM_centerA[offset_c + t].data[8] = HBM_import[t].data[8];
		HBM_centerA[offset_c + t].data[9] = HBM_import[t].data[9];
		HBM_centerA[offset_c + t].data[10] = HBM_import[t].data[10];
		HBM_centerA[offset_c + t].data[11] = HBM_import[t].data[11];
		HBM_centerA[offset_c + t].data[12] = HBM_import[t].data[12];
		HBM_centerA[offset_c + t].data[13] = HBM_import[t].data[13];
		HBM_centerA[offset_c + t].data[14] = HBM_import[t].data[14];
		HBM_centerA[offset_c + t].data[15] = HBM_import[t].data[15];
	
	}
}	

void export_frontiers(unsigned int chunk, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_export, action_t action, unsigned int _NUMCLOCKCYCLES_[2][32]){	
	#ifdef _DEBUGMODE_KERNELPRINTS4
	cout<<"acts_kernel::run:: exporting "<<action.size_import_export<<" 32bit-integers (FPGA -> Host)..."<<endl;
	#endif 
	unsigned int offset_c = 0 * chunk; // FIXME
	EXPORT_LOOP1: for(unsigned int t=0; t<action.size_import_export; t++){ 
	#pragma HLS PIPELINE II=1
		// export (FPGA -> Host)
		HBM_export[t].data[0] = HBM_centerA[offset_c + t].data[0];
		HBM_export[t].data[1] = HBM_centerA[offset_c + t].data[1];
		HBM_export[t].data[2] = HBM_centerA[offset_c + t].data[2];
		HBM_export[t].data[3] = HBM_centerA[offset_c + t].data[3];
		HBM_export[t].data[4] = HBM_centerA[offset_c + t].data[4];
		HBM_export[t].data[5] = HBM_centerA[offset_c + t].data[5];
		HBM_export[t].data[6] = HBM_centerA[offset_c + t].data[6];
		HBM_export[t].data[7] = HBM_centerA[offset_c + t].data[7];
		HBM_export[t].data[8] = HBM_centerA[offset_c + t].data[8];
		HBM_export[t].data[9] = HBM_centerA[offset_c + t].data[9];
		HBM_export[t].data[10] = HBM_centerA[offset_c + t].data[10];
		HBM_export[t].data[11] = HBM_centerA[offset_c + t].data[11];
		HBM_export[t].data[12] = HBM_centerA[offset_c + t].data[12];
		HBM_export[t].data[13] = HBM_centerA[offset_c + t].data[13];
		HBM_export[t].data[14] = HBM_centerA[offset_c + t].data[14];
		HBM_export[t].data[15] = HBM_centerA[offset_c + t].data[15];
	
	}
}	

void gather_frontiers(unsigned int inst, unsigned int p_v, unsigned int * cfrontier_dram___size, unsigned int * nfrontier_dram___size, offset_t * upartition_vertices, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, unsigned int globalparams[GLOBALBUFFER_SIZE], unsigned int _NUMCLOCKCYCLES_[2][32]){	
	GATHER_FRONTIERS_LOOP1: for(unsigned int local_subpartitionID=0; local_subpartitionID<NUM_SUBPARTITION_PER_PARTITION; local_subpartitionID++){
		unsigned int upartitionID = (p_v * NUM_SUBPARTITION_PER_PARTITION) + local_subpartitionID; // w.r.t. a given hbm (corresponds to number of upartitions)
		unsigned int offset_centerhbm = (upartitionID * MAX_UPARTITION_VECSIZE); 
		unsigned int offset_channelhbm = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + (upartitionID * VDATA_SUBPARTITION_VECSIZE);
		
		GATHER_FRONTIERS_LOOP1B: for(unsigned int t=0; t<nfrontier_dram___size[upartitionID]; t++){
		#pragma HLS PIPELINE II=1
			HBM_centerA[offset_centerhbm + t].data[0] = HBM_channelA[offset_channelhbm + t].data[0];
			HBM_centerA[offset_centerhbm + t].data[1] = HBM_channelA[offset_channelhbm + t].data[1];
			HBM_centerA[offset_centerhbm + t].data[2] = HBM_channelA[offset_channelhbm + t].data[2];
			HBM_centerA[offset_centerhbm + t].data[3] = HBM_channelA[offset_channelhbm + t].data[3];
			HBM_centerA[offset_centerhbm + t].data[4] = HBM_channelA[offset_channelhbm + t].data[4];
			HBM_centerA[offset_centerhbm + t].data[5] = HBM_channelA[offset_channelhbm + t].data[5];
			HBM_centerA[offset_centerhbm + t].data[6] = HBM_channelA[offset_channelhbm + t].data[6];
			HBM_centerA[offset_centerhbm + t].data[7] = HBM_channelA[offset_channelhbm + t].data[7];
			HBM_centerA[offset_centerhbm + t].data[8] = HBM_channelA[offset_channelhbm + t].data[8];
			HBM_centerA[offset_centerhbm + t].data[9] = HBM_channelA[offset_channelhbm + t].data[9];
			HBM_centerA[offset_centerhbm + t].data[10] = HBM_channelA[offset_channelhbm + t].data[10];
			HBM_centerA[offset_centerhbm + t].data[11] = HBM_channelA[offset_channelhbm + t].data[11];
			HBM_centerA[offset_centerhbm + t].data[12] = HBM_channelA[offset_channelhbm + t].data[12];
			HBM_centerA[offset_centerhbm + t].data[13] = HBM_channelA[offset_channelhbm + t].data[13];
			HBM_centerA[offset_centerhbm + t].data[14] = HBM_channelA[offset_channelhbm + t].data[14];
			HBM_centerA[offset_centerhbm + t].data[15] = HBM_channelA[offset_channelhbm + t].data[15];
			HBM_centerB[offset_centerhbm + t].data[0] = HBM_channelB[offset_channelhbm + t].data[0];
			HBM_centerB[offset_centerhbm + t].data[1] = HBM_channelB[offset_channelhbm + t].data[1];
			HBM_centerB[offset_centerhbm + t].data[2] = HBM_channelB[offset_channelhbm + t].data[2];
			HBM_centerB[offset_centerhbm + t].data[3] = HBM_channelB[offset_channelhbm + t].data[3];
			HBM_centerB[offset_centerhbm + t].data[4] = HBM_channelB[offset_channelhbm + t].data[4];
			HBM_centerB[offset_centerhbm + t].data[5] = HBM_channelB[offset_channelhbm + t].data[5];
			HBM_centerB[offset_centerhbm + t].data[6] = HBM_channelB[offset_channelhbm + t].data[6];
			HBM_centerB[offset_centerhbm + t].data[7] = HBM_channelB[offset_channelhbm + t].data[7];
			HBM_centerB[offset_centerhbm + t].data[8] = HBM_channelB[offset_channelhbm + t].data[8];
			HBM_centerB[offset_centerhbm + t].data[9] = HBM_channelB[offset_channelhbm + t].data[9];
			HBM_centerB[offset_centerhbm + t].data[10] = HBM_channelB[offset_channelhbm + t].data[10];
			HBM_centerB[offset_centerhbm + t].data[11] = HBM_channelB[offset_channelhbm + t].data[11];
			HBM_centerB[offset_centerhbm + t].data[12] = HBM_channelB[offset_channelhbm + t].data[12];
			HBM_centerB[offset_centerhbm + t].data[13] = HBM_channelB[offset_channelhbm + t].data[13];
			HBM_centerB[offset_centerhbm + t].data[14] = HBM_channelB[offset_channelhbm + t].data[14];
			HBM_centerB[offset_centerhbm + t].data[15] = HBM_channelB[offset_channelhbm + t].data[15];
	
			if(inst==0){ update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___GATHER_FRONTIERINFOS___, 1); }
		}
		
		upartition_vertices[upartitionID].count += nfrontier_dram___size[upartitionID]; 
		cfrontier_dram___size[upartitionID] += nfrontier_dram___size[upartitionID];
		// cout<<"--- gather_frontiers ------------------------- _NUMCLOCKCYCLES_[0][___CODE___GATHER_FRONTIERINFOS___]: "<<_NUMCLOCKCYCLES_[0][___CODE___GATHER_FRONTIERINFOS___]<<", NUM_SUBPARTITION_PER_PARTITION: "<<NUM_SUBPARTITION_PER_PARTITION<<", VDATA_SUBPARTITION_VECSIZE: "<<VDATA_SUBPARTITION_VECSIZE<<", local_subpartitionID: "<<local_subpartitionID<<", cfrontier_dram___size["<<upartitionID<<"]: "<<cfrontier_dram___size[upartitionID]<<endl;
	}
	return;
}	

extern "C" {	
MY_IFDEF_TOPLEVELFUNC(){		
#pragma HLS INTERFACE m_axi port = HBM_channelA0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = HBM_channelB0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = HBM_centerA offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = HBM_centerB offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = HBM_import offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = HBM_export offset = slave bundle = gmem5

#pragma HLS INTERFACE s_axilite port = HBM_channelA0
#pragma HLS INTERFACE s_axilite port = HBM_channelB0
#pragma HLS INTERFACE s_axilite port = HBM_centerA
#pragma HLS INTERFACE s_axilite port = HBM_centerB
#pragma HLS INTERFACE s_axilite port = HBM_import
#pragma HLS INTERFACE s_axilite port = HBM_export

#pragma HLS INTERFACE s_axilite port = fpga
#pragma HLS INTERFACE s_axilite port = module
#pragma HLS INTERFACE s_axilite port = graph_iteration
#pragma HLS INTERFACE s_axilite port = start_pu 
#pragma HLS INTERFACE s_axilite port = size_pu
#pragma HLS INTERFACE s_axilite port = skip_pu
#pragma HLS INTERFACE s_axilite port = start_pv
#pragma HLS INTERFACE s_axilite port = size_pv
#pragma HLS INTERFACE s_axilite port = start_llpset
#pragma HLS INTERFACE s_axilite port = size_llpset
#pragma HLS INTERFACE s_axilite port = start_llpid
#pragma HLS INTERFACE s_axilite port = size_llpid
#pragma HLS INTERFACE s_axilite port = start_gv
#pragma HLS INTERFACE s_axilite port = size_gv
#pragma HLS INTERFACE s_axilite port = id_process
#pragma HLS INTERFACE s_axilite port = id_import
#pragma HLS INTERFACE s_axilite port = id_export
#pragma HLS INTERFACE s_axilite port = size_import_export
#pragma HLS INTERFACE s_axilite port = status
#pragma HLS INTERFACE s_axilite port = return

	#ifndef ___RUNNING_FPGA_SYNTHESIS___	
	// #ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	// if(status == 1 || true) { cout<<"acts_kernel::run:: dynamic acts started"<<endl; }
	// #else 
	// if(status == 1 || true) { cout<<"acts_kernel::run:: acts started"<<endl; }	
	// #endif 	
	if(start_pv == NAp) { cout<<"acts_kernel::run:: acts started [processing stage]: fpga: "<<fpga<<", start_pu: "<<start_pu<<", start_pv: "<<start_pv<<endl; }
	if(start_pu == NAp) { cout<<"acts_kernel::run:: acts started [applying stage]: fpga: "<<fpga<<", start_pu: "<<start_pu<<", start_pv: "<<start_pv<<endl; }
	#endif 
	
	// commands from host
	action_t action;
	action.fpga = fpga; 
	action.module = module; 
	action.graph_iteration = graph_iteration; 
	action.start_pu = start_pu; 
	action.size_pu = size_pu; 
	action.skip_pu = skip_pu; 
	action.start_pv = start_pv;
	action.size_pv = size_pv; 
	action.start_llpset = start_llpset; 
	action.size_llpset = size_llpset; 
	action.start_llpid = start_llpid; 
	action.size_llpid = size_llpid; 
	action.start_gv = start_gv; 
	action.size_gv = size_gv;
	action.id_process = id_process;
	action.id_import = id_import;
	action.id_export = id_export;
	action.size_import_export = size_import_export;
	action.status = status;
	
	#ifdef _DEBUGMODE_KERNELPRINTS4					
	if(action.id_import != INVALID_IOBUFFER_ID){ cout<< TIMINGRESULTSCOLOR << "--> importing upartition: "<<action.id_import<<"..."<< RESET <<endl; }	
	if(action.id_export != INVALID_IOBUFFER_ID){ cout<< TIMINGRESULTSCOLOR << "<-- exporting vpartition: "<<action.id_export<<"..."<< RESET <<endl; }					
	#endif 
	
	#ifdef ___RUNNING_FPGA_SYNTHESIS___
	if(action.id_process == 88){ return; }
	#else 
	if(action.id_process == 88){ return 0; }
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS4				
	cout<< TIMINGRESULTSCOLOR << "--- processing upartition: "<<action.id_process<<"..."<< RESET <<endl; 				
	#endif 
	
	// declarations
// declaration of BRAM variables
#ifdef ___RUNNING_FPGA_SYNTHESIS___ // FPGA_IMPL
	keyvalue_t nfrontier_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE][VDATA_SUBPARTITION_VECSIZE]; 
	#pragma HLS ARRAY_PARTITION variable=nfrontier_buffer complete dim=1
	#pragma HLS ARRAY_PARTITION variable=nfrontier_buffer complete dim=2
	unsigned int statsbuffer_maskbased_tmp[NUM_VALID_PEs][EDGE_PACK_SIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_maskbased_tmp complete dim=1
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_maskbased_tmp complete dim=2
	unsigned int statsbuffer_maskbased[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_maskbased complete dim=1
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_maskbased complete dim=2
	unsigned int statsbuffer_idbased[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_idbased complete dim=1 
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_idbased complete dim=2
	vtr_t vptr_buffer[NUM_VALID_PEs][VPTR_BUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vptr_buffer complete dim=1
	edge3_type edges_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE][EDGE_BUFFER_SIZE];	
	#pragma HLS ARRAY_PARTITION variable=edges_buffer complete dim=1	
	#pragma HLS ARRAY_PARTITION variable=edges_buffer complete dim=2	
	keyvalue_t updates_buffer2[NUM_VALID_PEs][UPDATES_BUFFER_PACK_SIZE][UPDATES_BUFFER_SIZE];		
	#pragma HLS ARRAY_PARTITION variable=updates_buffer2 complete dim=1
	#pragma HLS ARRAY_PARTITION variable=updates_buffer2 complete dim=2

	keyvalue_t URAM_frontiers[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]; 
	#pragma HLS resource variable=URAM_frontiers core=XPM_MEMORY uram
	#pragma HLS ARRAY_PARTITION variable=URAM_frontiers complete dim=1

	vprop_t URAM_vprop[NUM_VALID_PEs][EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE];
	#pragma HLS resource variable=URAM_vprop core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=URAM_vprop complete dim=1
	#pragma HLS ARRAY_PARTITION variable=URAM_vprop complete dim=2 
	#pragma HLS aggregate variable=URAM_vprop 

	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	edge_update_type URAM_edgeupdates[NUM_VALID_PEs][EDGE_PACK_SIZE][EDGE_UPDATES_DRAMBUFFER_SIZE];	
	#pragma HLS resource variable=URAM_edgeupdates core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=URAM_edgeupdates complete dim=1	
	#pragma HLS ARRAY_PARTITION variable=URAM_edgeupdates complete dim=2	
	#endif 
	
	map_t URAM_map[NUM_VALID_PEs][EDGE_PACK_SIZE][EDGE_UPDATES_DRAMBUFFER_SIZE];
	#pragma HLS resource variable=URAM_map core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=URAM_map complete dim=1	
	#pragma HLS ARRAY_PARTITION variable=URAM_map complete dim=2	
	
	// #ifdef NUM_FPGAS>1	
	keyvalue_t URAM_updates[NUM_VALID_PEs][EDGE_PACK_SIZE][8192];	
	#pragma HLS resource variable=URAM_updates core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=URAM_updates complete dim=1	
	#pragma HLS ARRAY_PARTITION variable=URAM_updates complete dim=2		
	// #endif 
	
#else 
	keyvalue_t * nfrontier_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE]; 
	keyvalue_t * URAM_frontiers[EDGE_PACK_SIZE]; 
	unsigned int * statsbuffer_maskbased_tmp[NUM_VALID_PEs][EDGE_PACK_SIZE]; 
	unsigned int * statsbuffer_maskbased[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
	unsigned int * statsbuffer_idbased[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
	vtr_t * vptr_buffer[NUM_VALID_PEs]; 
	edge3_type * edges_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE];
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	edge_update_type * URAM_edgeupdates[NUM_VALID_PEs][EDGE_PACK_SIZE];
	#endif 
	map_t * URAM_map[NUM_VALID_PEs][EDGE_PACK_SIZE];
	keyvalue_t * updates_buffer2[NUM_VALID_PEs][UPDATES_BUFFER_PACK_SIZE]; 
	vprop_t * URAM_vprop[NUM_VALID_PEs][EDGE_PACK_SIZE];
	keyvalue_t * URAM_updates[NUM_VALID_PEs][EDGE_PACK_SIZE];	
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer[i][v] = new keyvalue_t[VDATA_SUBPARTITION_VECSIZE]; }}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ URAM_frontiers[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }	
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ statsbuffer_maskbased_tmp[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ statsbuffer_maskbased[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ statsbuffer_idbased[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ vptr_buffer[i] = new vtr_t[VPTR_BUFFER_SIZE]; }
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ edges_buffer[i][v] = new edge3_type[EDGE_BUFFER_SIZE]; }}
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ URAM_edgeupdates[i][v] = new edge_update_type[EDGE_UPDATES_DRAMBUFFER_SIZE]; }}
	#endif 
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ URAM_map[i][v] = new map_t[EDGE_UPDATES_DRAMBUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<UPDATES_BUFFER_PACK_SIZE; v++){ updates_buffer2[i][v] = new keyvalue_t[UPDATES_BUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ URAM_vprop[i][v] = new vprop_t[MAXVALID_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ URAM_updates[i][v] = new keyvalue_t[8192]; }}
#endif 

#ifdef FPGA_IMPL	
unsigned int cfrontier_dram___size[MAX_NUM_UPARTITIONS]; 
unsigned int nfrontier_dram___size[NUM_VALID_PEs][MAX_NUM_UPARTITIONS];
#pragma HLS ARRAY_PARTITION variable = nfrontier_dram___size complete dim=1
unsigned int nfrontier_buffer___size[NUM_VALID_PEs][EDGE_PACK_SIZE][NUM_SUBPARTITION_PER_PARTITION]; 
#pragma HLS ARRAY_PARTITION variable = nfrontier_buffer___size complete dim=1
#pragma HLS ARRAY_PARTITION variable = nfrontier_buffer___size complete dim=2
unsigned int updates_tmpbuffer___size[NUM_VALID_PEs][NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = updates_tmpbuffer___size complete dim=1
unsigned int updates_buffer___size[NUM_VALID_PEs][EDGE_PACK_SIZE][MAX_NUM_APPLYPARTITIONS]; 
#pragma HLS ARRAY_PARTITION variable = updates_buffer___size complete dim=1
#pragma HLS ARRAY_PARTITION variable = updates_buffer___size complete dim=2
unsigned int stats_buffer___size[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
#pragma HLS ARRAY_PARTITION variable = stats_buffer___size complete dim=1
unsigned int statsbuffer_idbased___size[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
#pragma HLS ARRAY_PARTITION variable = statsbuffer_idbased___size complete dim=1
offset_t upartition_vertices[MAX_NUM_UPARTITIONS]; 
offset_t vpartition_vertices[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
#pragma HLS ARRAY_PARTITION variable = vpartition_vertices complete dim=1
unsigned int vptrbuffer___size[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = vptrbuffer___size complete // dim=1
unsigned int edges_buffer___size[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = edges_buffer___size complete // dim=1
unsigned int cfrontier_bufferREAL___size[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = cfrontier_bufferREAL___size complete
#pragma HLS ARRAY_MAP variable=nfrontier_dram___size instance=array1 horizontal
#pragma HLS ARRAY_MAP variable=stats_buffer___size instance=array1 horizontal
#pragma HLS ARRAY_MAP variable=statsbuffer_idbased___size instance=array1 horizontal
#pragma HLS ARRAY_MAP variable=vpartition_vertices instance=array1 horizontal
#pragma HLS ARRAY_MAP variable=vptrbuffer___size instance=array2 horizontal
#pragma HLS ARRAY_MAP variable=edges_buffer___size instance=array2 horizontal
#ifndef HW
unsigned int hybrid_map[MAXNUMGRAPHITERATIONS][MAX_NUM_UPARTITIONS]; 
#endif 
#else 
unsigned int * cfrontier_dram___size = new unsigned int[MAX_NUM_UPARTITIONS]; 
unsigned int * nfrontier_dram___size[NUM_VALID_PEs]; for(unsigned int i=0; i<NUM_VALID_PEs; i++){ nfrontier_dram___size[i] = new unsigned int[MAX_NUM_UPARTITIONS]; }
unsigned int * nfrontier_buffer___size[NUM_VALID_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer___size[i][v] = new unsigned int[NUM_SUBPARTITION_PER_PARTITION]; }}
unsigned int updates_tmpbuffer___size[NUM_VALID_PEs][NUM_VALID_PEs]; 
unsigned int * updates_buffer___size[NUM_VALID_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[i][v] = new unsigned int[MAX_NUM_APPLYPARTITIONS]; }}
unsigned int stats_buffer___size[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
unsigned int statsbuffer_idbased___size[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
offset_t * upartition_vertices = new offset_t[MAX_NUM_UPARTITIONS]; 
offset_t * vpartition_vertices[NUM_VALID_PEs]; for(unsigned int i=0; i<NUM_VALID_PEs; i++){ vpartition_vertices[i] = new offset_t[MAX_NUM_APPLYPARTITIONS];  }
unsigned int vptrbuffer___size[NUM_VALID_PEs]; 
unsigned int edges_buffer___size[NUM_VALID_PEs]; 
unsigned int cfrontier_bufferREAL___size[NUM_VALID_PEs]; 
unsigned int * hybrid_map[MAXNUMGRAPHITERATIONS]; for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ hybrid_map[i] = new unsigned int[MAX_NUM_UPARTITIONS]; }
#endif 

edge3_type edges[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = edges complete dim=0
unsigned int res[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = res complete dim=0
unsigned int vtemp_in[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = vtemp_in complete dim=0
unsigned int vtemp_out[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = vtemp_out complete dim=0
vprop_t uprop[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = uprop complete dim=0
keyvalue_t update_in[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = update_in complete dim=0
keyvalue_t update_out[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = update_out complete dim=0
bool ens[NUM_VALID_PEs][EDGE_PACK_SIZE]; 
#pragma HLS ARRAY_PARTITION variable = ens complete dim=0
vprop_t data[EDGE_PACK_SIZE]; 
#pragma HLS ARRAY_PARTITION variable = data complete
vprop_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE]; 
#pragma HLS ARRAY_PARTITION variable = datas complete dim=0
keyvalue_t kvdata[EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = kvdata complete
keyvalue_t kvdatas[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = kvdatas complete dim=0
unsigned int offsets0[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = offsets0 complete
unsigned int offsets[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = offsets complete
unsigned int offsets2[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = offsets2 complete
unsigned int counts[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = counts complete
keyvalue_t actvvs[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = actvvs complete dim=0
frontier_t actvv[EDGE_PACK_SIZE];
unsigned int vid_first0[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = vid_first0 complete
unsigned int vid_first1[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = vid_first1 complete
unsigned int globalparams[GLOBALBUFFER_SIZE];
map_t updatesptrs[NUM_FPGAS][MAX_NUM_LLPSETS];
unsigned int limits[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = limits complete
unsigned int max_limits[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = max_limits complete
uint512_vec_dt updates_vecs[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = updates_vecs complete
uint512_vec_dt edges_vecs[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = edges_vecs complete
unsigned int stats_count[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable=stats_count complete
unsigned int stats_counts[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable=stats_counts complete dim=0
unsigned int edge_counts[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable=edge_counts complete dim=0
unsigned int cummtv2[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = cummtv2 complete
unsigned int masks[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable=masks complete dim=0
edge3_vec_dt edge3_vecs[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable=edge3_vecs complete
map_t maps[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable=maps complete	
unsigned int offsets3[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = offsets3 complete
map_t edgeupdate_map[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable=edgeupdate_map complete
map_t edge_map[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable=edge_map complete
unsigned int offsets_eu[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = offsets_eu complete
unsigned int limits_eu[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = limits_eu complete
unsigned int max_sz = 0; 

#ifdef FPGA_IMPL
unsigned int _NUMCLOCKCYCLES_[2][32]; 
#else 
unsigned int _NUMCLOCKCYCLES_[2][32]; 
#endif 

	
	
	// initialize variables 
unsigned int maxGraphIter = 0;
unsigned int total_num_actvvs = 0; 
#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
bool enable___collectactivedstvids = true;
#else 
bool enable___collectactivedstvids = false; // false
#endif 
unsigned int max_limit = 0, max_limit2 = 0; 
unsigned int CONST1 = NUM_PEs * EDGE_PACK_SIZE; 
unsigned int CONST2 = VDATA_SUBPARTITION_VECSIZE * NUM_PEs * EDGE_PACK_SIZE;
for(unsigned int k=0; k<2; k++){ for(unsigned int t=0; t<32; t++){ _NUMCLOCKCYCLES_[k][t] = 0; } }
unsigned int total_cycles = 0;

// load global parameters	
MY_LOOP209: for(unsigned int t=0; t<GLOBALBUFFER_SIZE; t++){
#pragma HLS PIPELINE II=1
	globalparams[t] = dretrieve_globalparams(0, t, HBM_channelA0, HBM_channelB0, 0);
}
unsigned int __NUM_UPARTITIONS = globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS];
unsigned int __NUM_APPLYPARTITIONS = globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; 
#ifndef FPGA_IMPL
globalparams_debug = (unsigned int *)&globalparams[0];
#endif

int ___ENABLE___RESETBUFFERSATSTART___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___RESETBUFFERSATSTART];
int ___ENABLE___PREPAREEDGEUPDATES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___PREPAREEDGEUPDATES];
int ___ENABLE___PROCESSEDGEUPDATES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___PROCESSEDGEUPDATES];
int ___ENABLE___PROCESSEDGES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___PROCESSEDGES];
	int ___ENABLE___READ_FRONTIER_PROPERTIES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___READ_FRONTIER_PROPERTIES];
	int ___ENABLE___VCPROCESSEDGES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___VCPROCESSEDGES]; 
int ___ENABLE___SAVEVCUPDATES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___SAVEVCUPDATES]; 
int ___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___COLLECTACTIVEDSTVIDS];
int ___ENABLE___APPLYUPDATESMODULE___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___APPLYUPDATESMODULE]; 
	int ___ENABLE___READ_DEST_PROPERTIES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___READ_DEST_PROPERTIES];
	int ___ENABLE___APPLYUPDATES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___APPLYUPDATES]; 
	int ___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES]; 
	int ___ENABLE___SAVE_DEST_PROPERTIES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___SAVE_DEST_PROPERTIES]; 
int ___ENABLE___EXCHANGEFRONTIERINFOS___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___EXCHANGEFRONTIERINFOS]; 

unsigned int threshold___activedstvids = globalparams[GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEDSTVID];
unsigned int async_batch = globalparams[GLOBALPARAMSCODE__ASYNC__BATCH]; 
unsigned int _ASYNC_BATCH_SIZE_ = globalparams[GLOBALPARAMSCODE__ASYNC__BATCHSIZE];	

bool all_vertices_active_in_all_iterations = true; 
if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == PAGERANK || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CF || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == HITS || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == SPMV){ all_vertices_active_in_all_iterations = true; }
for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[p] = 0; }
if(all_vertices_active_in_all_iterations == true){ for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u+=1){ cfrontier_dram___size[p_u] = MAXVALID_APPLYPARTITION_VECSIZE; }} 
else { cfrontier_dram___size[0] = 1; }// activate root vid
unsigned int threshold___activefrontiers = globalparams[GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEFRONTIERSFORCONTROLSWITCH]; 

#ifdef _DEBUGMODE_KERNELPRINTS//4
if(action.status == 1){
	cout<<"=== acts_kernel::run:: parameters ==="<<endl;
	cout<<"=== num vertices: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES]<<" === "<<endl;
	cout<<"=== num edges: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES]<<" === "<<endl;
	cout<<"=== num_pes: "<<NUM_VALID_PEs<<" === "<<endl;
	cout<<"=== EDGE_PACK_SIZE: "<<EDGE_PACK_SIZE<<" === "<<endl;
	cout<<"=== MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<" === "<<endl;
	cout<<"=== NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<" === "<<endl;
	cout<<"=== MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<" === "<<endl;
	cout<<"=== NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<" === "<<endl;
	cout<<"=== MULT_FACTOR: "<<MULT_FACTOR<<" === "<<endl;
	cout<<"---------------------------------------------------------------------- ACTS running "<<NUM_VALID_PEs<<" instances ----------------------------------------------------------------------"<<endl;
	cout<<"### GAS iteration: 0 [1 active vertices]"<<endl; 
}
#endif 

// reset ubuffer 
if(all_vertices_active_in_all_iterations == false){ 
	vprop_t prop; prop.prop = 0; prop.degree = INVALIDMASK; 
	RESET_BUFFERS_lOOP: for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ 
	#pragma HLS PIPELINE II=1
		MY_LOOP194: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			MY_LOOP196: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
			#pragma HLS UNROLL
				insert_vdatabuffer(v, t, prop, URAM_vprop[inst]); 
			}
		}
		update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_FRONTIER_PROPERTIES___, 1);
	}
}
		

	// resets
	if(___ENABLE___RESETBUFFERSATSTART___BOOL___ == 1){
		#ifdef ___ENABLE___RESETBUFFERSATSTART___
// MY_LOOP200: for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[p] = 0; }
MY_LOOP201: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ nfrontier_dram___size[i][p] = 0; }}
MY_LOOP202: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<NUM_SUBPARTITION_PER_PARTITION; p++){ nfrontier_buffer___size[i][v][p] = 0; }}}
MY_LOOP203: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_APPLYPARTITIONS; p++){ updates_buffer___size[i][v][p] = 0; }}}
MY_LOOP204: for(unsigned int p=0; p<__NUM_UPARTITIONS; p++){ upartition_vertices[p].offset = 0; upartition_vertices[p].size = 0; upartition_vertices[p].count = 0; } upartition_vertices[0].count = 1;
MY_LOOP205: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ vpartition_vertices[i][p].offset = 0; vpartition_vertices[i][p].size = 0; vpartition_vertices[i][p].count = 0; }}					
for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ updatesptrs[fpga][p].size = 0; }}
MY_LOOP207: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ vptrbuffer___size[i] = 0; }
MY_LOOP208: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ edges_buffer___size[i] = 0; }
#ifndef HW
for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ for(unsigned int t=0; t<MAX_NUM_UPARTITIONS; t++){ hybrid_map[i][t] = 1; }}
#endif		
		#endif 
	}
	
	// load vertex-updates map 
	LOAD_UPDATEPTRS_lOOP1: for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		#ifdef _DEBUGMODE_KERNELPRINTS4
		if(action.module == ALL_MODULES){ cout<<"acts: offsets for fpga "<<fpga<<endl; }
		#endif 
		LOAD_UPDATEPTRS_lOOP1B: for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; t++){	
		#pragma HLS PIPELINE II=1
			updatesptrs[fpga][t] = load_vupdate_map(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], (fpga * MAX_NUM_LLPSETS) + t,  HBM_channelA0, HBM_channelB0);	
			// if((action.module == ALL_MODULES || action.module == PROCESS_EDGES_MODULE) && action.start_pu == 0){ updatesptrs[fpga][t].size = 0; }	
			if((action.module == ALL_MODULES || action.module == PROCESS_EDGES_MODULE) && ((action.start_pu - action.fpga) == 0)){ 
				#ifdef _DEBUGMODE_KERNELPRINTS4
				cout << "acts: resetting updates space..." <<endl;
				#endif 
				updatesptrs[fpga][t].size = 0; }				
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			if(action.module == ALL_MODULES){ cout<<"acts: updatesptrs["<<fpga<<"]["<<t<<"].offset: "<<updatesptrs[fpga][t].offset<<", updatesptrs["<<fpga<<"]["<<t<<"].size: "<<updatesptrs[fpga][t].size<<endl; }
			#endif 
		}
	}
	
	// turn raw edges into actpact format
	if(___ENABLE___PREPAREEDGEUPDATES___BOOL___ == 1 && action.module == PREPROCESSING_MODULE){
		#ifndef ___RUNNING_FPGA_SYNTHESIS___
		edge_update_type * URAM_edges[NUM_VALID_PEs][EDGE_PACK_SIZE];
		for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ URAM_edges[i][v] = new edge_update_type[EDGE_UPDATES_DRAMBUFFER_LONGSIZE]; }}
		if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 0){ 
			return create_actpack(action, globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2], URAM_edges, EDGE_UPDATES_DRAMBUFFER_LONGSIZE, EDGE_UPDATES_DRAMBUFFER_LONGSIZE, globalparams,  HBM_channelA0, HBM_channelB0, final_edge_updates, _NUMCLOCKCYCLES_);		
		} else if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 1){	
			return create_actpack(action, globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES], globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS], URAM_edges, EDGE_UPDATES_DRAMBUFFER_SIZE, EDGE_UPDATES_WORKBUFFER_SIZE, globalparams,  HBM_channelA0, HBM_channelB0, final_edge_updates, _NUMCLOCKCYCLES_);	
		}
		#endif
	}
	
	// run acts 
	RUN_ACTS_LOOP1: for(unsigned int run=0; run<globalparams[GLOBALPARAMSCODE__PARAM__NUM_RUNS]; run++){
		RUN_ACTS_LOOP1B: for(unsigned int GraphIter=0; GraphIter<globalparams[GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS]; GraphIter++){
			#ifdef _DEBUGMODE_KERNELPRINTS4 
			if(action.status == 1){ cout<<"### processing [run "<<run<<", graph iteration "<<GraphIter<<"]..."<<endl; }
			if(false){ cout<<"### processing action [module "<<action.module<<", start_pu "<<action.start_pu<<", size_pu "<<action.size_pu<<", start_pv "<<action.start_pv<<", size_pv "<<action.size_pv<<", start_llpset "<<action.start_llpset<<", size_llpset "<<action.size_llpset<<", start_llpid "<<action.start_llpid<<", size_llpid "<<action.size_llpid<<"]"<<endl; }
			#endif 
				
			unsigned int MASK_CODE = 1 + GraphIter;
			unsigned int MASK_CODE_PE = 1 + GraphIter;
			unsigned int MASK_CODE_AU = 4094 + 1 + GraphIter;
			
			// capture sparsity distribution for sssp, bfs
// #ifdef GGGGG
unsigned int __NUM_ACTIVE_UPARTITIONS = __NUM_UPARTITIONS; unsigned int __NUM_ACTIVE_LLPSETS = __NUM_APPLYPARTITIONS; unsigned int __PERCENTAGE_ACTIVE_EDGES = 100;
if(all_vertices_active_in_all_iterations == false){
	if(GraphIter == 0){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 1; } 
	else if(GraphIter == 1){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 1; } 
	else if(GraphIter == 2){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 2; } 
	else if(GraphIter == 3){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 5; } 
	else if(GraphIter == 4){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 10; } 
	else if(GraphIter == 5){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 25; } 
	else if(GraphIter == 6){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 40; } 
	else if(GraphIter == 7){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 15; } 
	else if(GraphIter == 8){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 3; } 
	else if(GraphIter == 9){ __NUM_ACTIVE_UPARTITIONS = 1; __NUM_ACTIVE_LLPSETS = 1; __PERCENTAGE_ACTIVE_EDGES = 2; } 
	else { __PERCENTAGE_ACTIVE_EDGES = 1; } 
}	
// #endif 	
			
			// clear counters
			#ifdef ___ENABLE___CLEAR_COUNTERS___
// clear counters
CLEAR_COUNTERS_LOOP1: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ 
#pragma HLS PIPELINE II=1
	for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
	#pragma HLS UNROLL
		stats_buffer___size[inst][p_v] = 0; statsbuffer_idbased___size[inst][p_v] = 0;
		vpartition_vertices[inst][p_v].offset = 0; vpartition_vertices[inst][p_v].size = 0; vpartition_vertices[inst][p_v].count = 0;
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
		#pragma HLS UNROLL
			updates_buffer___size[inst][v][p_v] = 0; 
		}
	}
}	

#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ 
#pragma HLS PIPELINE II=1
	for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ 
		#pragma HLS UNROLL
		for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
			#pragma HLS UNROLL
			insert_statsbuffer(t, p_v, 0, statsbuffer_maskbased[inst]); 
		}
	}
}
#endif 	
			#endif
			
			// process-edges and partition-updates
			#ifdef ___ENABLE___PROCESSEDGES___
			if(action.module == PROCESS_EDGES_MODULE || action.module == ALL_MODULES){
			PROCESS_EDGES_MODULE_LOOP1B: for(unsigned int p_u=action.start_pu; p_u<action.start_pu + action.size_pu; p_u+=action.skip_pu){ 
				MASK_CODE_PE = ((1 + GraphIter) * MAX_NUM_UPARTITIONS) + p_u;
				#ifdef _DEBUGMODE_KERNELPRINTS//4 
				cout<<"$$$ processing edges in upartition "<<p_u<<": [PEs "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<n<<", "; } cout<<"]"<<endl; 
				#endif 
				
				// insert/delete/update edges
				#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
				if(___ENABLE___PROCESSEDGEUPDATES___BOOL___ == 1){	
#define ___ENABLE___LOADEDGEUPDATESTOBRAM___
#define ___ENABLE___COLLECTSTATS___
#define ___ENABLE___MAPEDGEUPDATESTOURAM___
#define ___ENABLE___APPLY_EDGEUPDATES___
#define ___ENABLE___PREPAREFORNEXTROUND___
// #define _DEBUGMODE_KERNELPRINTS4_PROCESSEDGEUPDATES
#define _DEBUGMODE_APPLYEDGEUPDATES_PRINTS4

edge_update_vec_dt edge_update_vecs[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable=edge_update_vecs complete
edge3_vec_dt edge3_vecs[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable=edge3_vecs complete
unsigned int offsets[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = offsets complete
unsigned int maxs[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable=maxs complete

edge_update_type MISSBUFFER_edgeupdates[NUM_VALID_PEs][EDGE_PACK_SIZE][BLOCKRAM_SIZE];
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edgeupdates complete dim=1	
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edgeupdates complete dim=2	
edge3_type MISSBUFFER_edges[NUM_VALID_PEs][EDGE_PACK_SIZE][BLOCKRAM_SIZE];
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edges complete dim=1	
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edges complete dim=2	

unsigned int MISSBUFFER_edgeupdates_index[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edgeupdates_index complete dim=1	
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edgeupdates_index complete dim=2	
unsigned int MISSBUFFER_edges_index[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edgeupdates_index complete dim=1
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edgeupdates_index complete dim=2	

unsigned int max_sz_eus[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable=MISSBUFFER_edgeupdates_index complete

edge_update_vec_dt edge_updates_buffer_vecs[NUM_VALID_PEs][BLOCKRAM_SIZE];	
#pragma HLS ARRAY_PARTITION variable=edge_updates_buffer_vecs complete dim=1
unsigned int edgeupdates_hash_ids[NUM_VALID_PEs][EDGE_PACK_SIZE][BLOCKRAM_SIZE];	
#pragma HLS ARRAY_PARTITION variable=edgeupdates_hash_ids complete dim=1
#pragma HLS ARRAY_PARTITION variable=edgeupdates_hash_ids complete dim=2

// clear URAMs
MY_LOOP1580: for(unsigned int t=0; t<EDGE_UPDATES_DRAMBUFFER_SIZE; t++){ 
#pragma HLS PIPELINE II=1
	for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
	#pragma HLS UNROLL
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL		
			URAM_map[inst][v][t].offset = 0; 
			URAM_map[inst][v][t].size = 0;	
			URAM_edgeupdates[inst][v][t].srcvid = INVALIDDATA; 
			URAM_edgeupdates[inst][v][t].dstvid = INVALIDDATA; 
		}
	}
}

MY_LOOP1481: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ 
#pragma HLS PIPELINE II=1
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL	
		for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
		#pragma HLS UNROLL	
			edgeupdates_hash_ids[inst][v][t] = 0; 
		}
	}
}	

// clear URAMs
// PROCESS_EDGEUPDATES_MODULE_LOOP1B: for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u++){ // 1, __NUM_UPARTITIONS. REMOVEME.
#ifndef ___RUNNING_FPGA_SYNTHESIS___
if(all_vertices_active_in_all_iterations == true){ cout<<"### inserting/deleting/updating edges in upartition "<<p_u<<": [PEs "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<n<<", "; } cout<<"]"<<endl; }
#endif 
EC_PROCESS_EDGEUPDATES_LOOP1: for(unsigned int llp_set=0; llp_set<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; llp_set++){	
	EC_PROCESS_EDGEUPDATES_LOOP1B: for(unsigned int llp_id=0; llp_id<EDGE_PACK_SIZE; llp_id++){ // REMOVEME.
		#ifdef _DEBUGMODE_APPLYEDGEUPDATES_PRINTS4
		cout<<"------------------- update edges: ["<<p_u<<", "<<llp_set<<", "<<llp_id<<"] -------------------"<<endl; 
		#endif 
		
		map_t edgeupdate_maps[NUM_VALID_PEs];
		#pragma HLS ARRAY_PARTITION variable=edgeupdate_maps complete
		map_t edge_maps[NUM_VALID_PEs];
		#pragma HLS ARRAY_PARTITION variable=edge_maps complete	
		edge_update_type invalid_edgeupdate_data; invalid_edgeupdate_data.srcvid = INVALIDDATA; invalid_edgeupdate_data.dstvid = INVALIDDATA; 
		edge3_type invalid_edge_data; invalid_edge_data.srcvid = INVALIDDATA; invalid_edge_data.dstvid = INVALIDDATA; 
		
		// load edges map
		load_edgemaps((p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id, globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2], edge_maps,  HBM_channelA0, HBM_channelB0);
		
		// load edge updates maps 
		load_edgemaps((p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id, globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS], edgeupdate_maps,  HBM_channelA0, HBM_channelB0);
		
		#ifdef _DEBUGMODE_KERNELPRINTS4_PROCESSEDGEUPDATES
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<"process-edge-updates: edgeupdate_maps[n]["<<llp_id<<"]: p_u: "<<p_u<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", offset: "<<edgeupdate_maps[n].offset<<", size: "<<edgeupdate_maps[n].size<<""<<endl; }
		#endif 
		
		// prepare maps, offsets, variables 
		unsigned int max_sz = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ limits[n] = edge_maps[n].size; } for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max_sz < limits[n]){ max_sz = limits[n]; }}			
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = edge_maps[n].offset; }
		#ifdef _DEBUGMODE_KERNELPRINTS4_PROCESSEDGEUPDATES
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<"process-edge-updates: edges map offsets: "; cout<<offsets[n]<<", max_sz: "<<max_sz<<endl; }
		#endif 			

		// prepare maps, offsets, variables 
		unsigned int max_sz_eu = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ limits_eu[n] = edgeupdate_maps[n].size; } for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max_sz_eu < limits_eu[n]){ max_sz_eu = limits_eu[n]; }}
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
			offsets_eu[n] = edgeupdate_maps[n].offset; 
		}
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
			#pragma HLS UNROLL
				max_sz_eus[n][v] = edgeupdate_maps[n].size;
			}
		}
		#ifdef _DEBUGMODE_KERNELPRINTS4_PROCESSEDGEUPDATES
		cout<<"[BEFORE - EDGE UPDATES MAP]: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<edgeupdate_maps[n].size<<", "; } cout<<endl;
		cout<<"[BEFORE - EDGE MAP]: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<edge_maps[n].size<<", "; } cout<<endl;
		#endif 
		if(max_sz_eu == 0){ continue; }
		if(max_sz_eu > BLOCKRAM_SIZE){ max_sz_eu = BLOCKRAM_SIZE; }
		unsigned int padding_factor = EDGE_UPDATES_DRAMBUFFER_SIZE / max_sz_eu; // 12; // 16; // 8;
		#ifdef _DEBUGMODE_CHECKS3
		if(padding_factor == 0){ cout<<"apply-edge-updates: padding_factor == 0. EDGE_UPDATES_DRAMBUFFER_SIZE: "<<EDGE_UPDATES_DRAMBUFFER_SIZE<<", max_sz_eu: "<<max_sz_eu<<". EXITING..."<<endl; exit(EXIT_FAILURE); }
		#endif 
		
		// successive applies until no more edge updates are left
		APPLY_EDGEUPDATES_MAINLOOP1: for(unsigned int c=0; c<1; c++){
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"--------------------------------- [p_u: "<<p_u<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", c: "<<c<<"] edge updates: max_sz: "<<max_sz<<", max_sz_eu: "<<max_sz_eu<<", offsets_eu[0]: "<<offsets_eu[0]<<" --------------------------------------"<<endl;
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<"max_sz_eus for PE "<<n<<": "; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cout<<max_sz_eus[n][v]<<", "; } cout<<endl; }
			#endif 
			
			// clear indexes
			MY_LOOP12735: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				MY_LOOP12746: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				#pragma HLS UNROLL
					MISSBUFFER_edgeupdates_index[inst][v] = 0;
					MISSBUFFER_edges_index[inst][v] = 0;
				}
			}
			
			// load edge updates to BRAM buffer
			#ifdef ___ENABLE___LOADEDGEUPDATESTOBRAM___
			edge_update_vec_dt edge_update_vecs2[NUM_VALID_PEs];	
			#pragma HLS ARRAY_PARTITION variable=edge_update_vecs2 complete
			COLLECTSTATS_EDGEUPDATES_LOOP1: for(unsigned int t=0; t<max_sz_eu; t++){ // max_sz_eu // sz_ // EDGE_UPDATES_DRAMBUFFER_SIZE // EDGE_LLPUPDATES_DRAMBUFFER_SIZE // FIXME // FIXHANG
			#pragma HLS PIPELINE II=2
				dretrievemany_edgeupdates(globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES], offsets_eu, t, edge_update_vecs2,  HBM_channelA0, HBM_channelB0);
				for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
				#pragma HLS UNROLL
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						edge_update_type edge_update = edge_update_vecs2[inst].data[v];
						edge_updates_buffer_vecs[inst][t].data[v] = edge_update;
						unsigned int incr = 0; unsigned int index = 0; 
						if((edge_update.srcvid < EDGE_UPDATES_DRAMBUFFER_SIZE) && (t < max_sz_eu)){ incr = 1; index = edge_update.srcvid; } else { incr = 0; index = 0; }	
						URAM_map[inst][v][index].size += incr;								
					}
				}
			}
			#endif 
			
			// collect edge-update stats 
			#ifdef ___ENABLE___COLLECTSTATS___
			COLLECTSTATS_EDGEUPDATES_LOOP2: for(unsigned int t=1; t<EDGE_UPDATES_DRAMBUFFER_SIZE; t++){ 
			#pragma HLS PIPELINE II=2
				for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){	
				#pragma HLS UNROLL
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						URAM_map[inst][v][t].offset = URAM_map[inst][v][t-1].offset + (URAM_map[inst][v][t-1].size * padding_factor); 
					}
				}
			}	
			#endif 
			
			// print summary
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			unsigned int tots_ = 0; unsigned int tots2_ = 0;
			for(unsigned int t=0; t<EDGE_UPDATES_DRAMBUFFER_SIZE; t++){ for(unsigned int inst=0; inst<1; inst++){ for(unsigned int v=0; v<1; v++){ if(URAM_map[inst][v][t].size > 0){ cout<<"------ URAM_map["<<inst<<"]["<<v<<"]["<<t<<"].offset: "<<URAM_map[inst][v][t].offset<<", URAM_map["<<inst<<"]["<<v<<"]["<<t<<"].size: "<<URAM_map[inst][v][t].size<<endl; tots_ += 1; tots2_ += URAM_map[inst][v][t].size; }}}}
			cout<<"------ summary: used "<<tots_<<" slots (of 8192 slots). tots2: "<<tots2_<<endl;
			#endif 
		
			// load edge updates to URAM
			unsigned int batch_sz = 128; // 256
			#ifdef ___ENABLE___MAPEDGEUPDATESTOURAM___
			if(max_sz_eu > BLOCKRAM_SIZE){ max_sz_eu = BLOCKRAM_SIZE; }
			LOAD_EDGEUPDATES_LOOP1: for(unsigned int t1=0; t1<(max_sz_eu + (batch_sz-1)) / batch_sz; t1++){
				LOAD_EDGEUPDATES_LOOP1B: for(unsigned int t2=0; t2<batch_sz; t2++){ // FIXHANG.
				#pragma HLS PIPELINE II=4
					unsigned int t = (t1 * batch_sz) + t2;
					for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
					#pragma HLS UNROLL
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							edge_update_type edge_update = edge_updates_buffer_vecs[inst][t].data[v];
							#ifdef _DEBUGMODE_CHECKS3
							if(edge_update.srcvid != INVALIDDATA && t < max_sz_eu){ checkoutofbounds("acts_kernel::ERROR 8813::", edge_update.srcvid, MAX_UPARTITION_SIZE, t, v, inst); }
							if(edge_update.srcvid != INVALIDDATA && t < max_sz_eu){ checkoutofbounds("acts_kernel::ERROR 7713::", edge_update.srcvid, EDGE_UPDATES_DRAMBUFFER_SIZE, t, v, inst); }
							#endif
							
							map_t map; map.size = 0;
							if(edge_update.srcvid < EDGE_UPDATES_DRAMBUFFER_SIZE){ map = URAM_map[inst][v][edge_update.srcvid]; }
							
							if(map.size > 0 && t < max_sz_eu){	
								unsigned int edge_hashid = hash2_edge(map, edge_update.srcvid, edge_update.dstvid, padding_factor); 

								#ifdef _DEBUGMODE_KERNELPRINTS//4 
								if(inst==0 && p_u==0 && llp_set==0 && t<2){ cout<<"$$$ load-edge-updates::["<<inst<<"]["<<t<<"]["<<v<<"]: [edge_update-update: srcvid: "<<edge_update.srcvid<<", dstvid: "<<edge_update.dstvid<<"]---[edge_hashid: "<<edge_hashid<<"]"<<endl; }								
								#endif
								
								edge_update_type former_edge_update = URAM_edgeupdates[inst][v][edge_hashid];
								edge_update_type new_edge_update;
								if(former_edge_update.srcvid == INVALIDDATA){ new_edge_update = edge_update; } else { new_edge_update = former_edge_update; }
								
								URAM_edgeupdates[inst][v][edge_hashid] = new_edge_update;
								edgeupdates_hash_ids[inst][v][t] = edge_hashid;
								
								if(former_edge_update.srcvid != INVALIDDATA){
									#ifdef _DEBUGMODE_CHECKS3
									checkoutofbounds("apply-edge-updates::ERROR 25173::", MISSBUFFER_edgeupdates_index[inst][v], BLOCKRAM_SIZE, NAp, NAp, NAp);
									#endif
									MISSBUFFER_edgeupdates[inst][v][MISSBUFFER_edgeupdates_index[inst][v]] = edge_update;
									MISSBUFFER_edgeupdates_index[inst][v] += 1; 
								}
							}
							if(inst==0 && v==0){ update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___NUMBER_OF_EDGEUPDATES_LOADED___, 1); }
						}
					}
				}
			}
			#endif 
			
			// print summary
			#ifdef _DEBUGMODE_APPLYEDGEUPDATES_PRINTS4
			if(false){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cout<<"[AFTERR]: MISSBUFFER_edgeupdates_index[0]["<<v<<"]: "<<MISSBUFFER_edgeupdates_index[0][v]<<endl; }}
			unsigned int total_num_misses_ = 0; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ total_num_misses_ += MISSBUFFER_edgeupdates_index[0][v]; }
			cout<<"[Edge-updates miss summary]: total number of misses: "<<total_num_misses_<<" (of "<<max_sz_eu * EDGE_PACK_SIZE<<")"<<endl;
			#endif 
			// exit(EXIT_SUCCESS);
			
			// apply edge updates to edges
			unsigned int num_hits = 0; unsigned int num_misses = 0;
			#ifdef ___ENABLE___APPLY_EDGEUPDATES___
			PROCESS_EDGEUPDATES_MAINLOOP1D: for(unsigned int t=0; t<max_sz; t++){ // max_sz // 2223
			#pragma HLS PIPELINE II=2
				dretrievemanyfromB_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0);
				
				PROCESS_EDGEUPDATES_LOOP1E: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
				#pragma HLS UNROLL
					PROCESS_EDGEUPDATES_LOOP1F: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						edge3_type edge = edge3_vecs[inst].data[v];
						unsigned int weight = edge.weight;	

						map_t map; map.size = 0;
						if(edge.srcvid < EDGE_UPDATES_DRAMBUFFER_SIZE){ map = URAM_map[inst][v][edge.srcvid]; }
						
						if(map.size > 0){
							unsigned int edge_hashid = hash2_edge(map, edge.srcvid, edge.dstvid, padding_factor);
							unsigned int eu_weight = 1;
							
							#ifdef _DEBUGMODE_KERNELPRINTS//4 
							if(inst==0 && p_u==0 && llp_set==0 && t<8){ cout<<"### process-edge-updates::["<<inst<<"]["<<t<<"]["<<v<<"]:edges: srcvid: "<<edge.srcvid<<", dstvid: "<<edge.dstvid<<"]---[edge: srcvid: "<<edge.srcvid<<", dstvid: "<<edge.dstvid<<"]---[edge_hashid: "<<edge_hashid<<"]"<<endl; }								
							#endif 
							
							edge_update_type edge_update = URAM_edgeupdates[inst][v][edge_hashid];
							if(edge.srcvid == edge_update.srcvid && edge.dstvid == edge_update.dstvid){
								edge3_vecs[inst].data[v].weight += eu_weight;									
								#ifdef _DEBUGMODE_CHECKS3
								if(inst==0){ num_hits += 1; }
								if(inst==0 && false){ cout<<"[process-edge-updates:: hit seen @ ["<<inst<<"]["<<t<<"]["<<v<<"]: srcvid: "<<edge.srcvid<<", dstvid: "<<edge.dstvid<<", edge_hashid: "<<edge_hashid<<"]"<<endl; }
								#endif 
								if(inst==0 && v==0){ update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___NUMBER_OF_EDGEUPDATES_APPLIED___, 1); }
							} else {
								if(edge_update.srcvid == INVALIDDATA || URAM_map[inst][v][edge.srcvid].size == 0){
									#ifdef _DEBUGMODE_CHECKS3
									if(inst==0){ num_hits += 1; }
									#endif 
								} else {		
									MISSBUFFER_edges[inst][v][MISSBUFFER_edges_index[inst][v]] = edge; // FIXME.
									// MISSBUFFER_edges_index[inst][v] += 1;
									#ifdef _DEBUGMODE_CHECKS3
									if(inst==0){ num_misses += 1; }
									checkoutofbounds("utility::ERROR 25183::", MISSBUFFER_edges_index[inst][v], BLOCKRAM_SIZE, NAp, NAp, NAp);
									if(inst==0 && false){ cout<<"[process-edge-updates:: miss seen @ ["<<inst<<"]["<<v<<"]["<<t<<"]: srcvid: "<<edge.srcvid<<", dstvid: "<<edge.dstvid<<", edge_hashid: "<<edge_hashid<<"]"<<endl; }
									#endif 
								}
							}
						}
					}
				}
			
				dinsertmanytoA_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0); // FIXME
				update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
			}
			#endif 
			// exit(EXIT_SUCCESS);
			
			#ifdef _DEBUGMODE_APPLYEDGEUPDATES_PRINTS4
			if(false){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cout<<"[AFTER]: MISSBUFFER_edges_index[0]["<<v<<"]: "<<MISSBUFFER_edges_index[0][v]<<endl; }}
			unsigned int total_num_misses = 0; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ total_num_misses += MISSBUFFER_edges_index[0][v]; }
			cout<<"[Edges miss summary]: total number of misses: "<<total_num_misses<<" (of "<<max_sz * EDGE_PACK_SIZE<<")"<<endl;
			#endif 
			
			// prepare for next round
			#ifdef ___ENABLE___PREPAREFORNEXTROUND___
			// prepare for next round (save misses - edge updates)
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ maxs[n] = 0; }
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
				#pragma HLS UNROLL
				for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(maxs[n] < MISSBUFFER_edgeupdates_index[n][v]){ maxs[n] = MISSBUFFER_edgeupdates_index[n][v]; }}
			}
			unsigned int max = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max < maxs[n]){ max = maxs[n]; }}
			SAVEMISSES_EDGEUPDATES_MAINLOOP1D: for(unsigned int t=0; t<max; t++){ 
			#pragma HLS PIPELINE II=1
				for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
				#pragma HLS UNROLL
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL		
						if(t< MISSBUFFER_edgeupdates_index[inst][v]){ edge_update_vecs[inst].data[v] = MISSBUFFER_edgeupdates[inst][v][t]; } 
						else { edge_update_vecs[inst].data[v] = invalid_edgeupdate_data; }
					}
				}
				dinsertmany_edgeupdates(globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES], offsets_eu, t, edge_update_vecs,  HBM_channelA0, HBM_channelB0);
			}
			// max_sz_eu = max;
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
			#pragma HLS UNROLL
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
				#pragma HLS UNROLL
					max_sz_eus[n][v] = MISSBUFFER_edgeupdates_index[n][v];
				}
			}
			#endif 
			
			// prepare for next round (save misses - edges)
			#ifdef ___ENABLE___PREPAREFORNEXTROUND___XXX
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ maxs[n] = 0; }
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
				#pragma HLS UNROLL
				for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(maxs[n] < MISSBUFFER_edges_index[n][v]){ maxs[n] = MISSBUFFER_edges_index[n][v]; }}
			}
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max < maxs[n]){ max = maxs[n]; }}
			cout<<"------------------------------------- apply-edge-updates: max: "<<max<<endl;
			edge3_vec_dt edge_vecs[NUM_VALID_PEs];
			#pragma HLS ARRAY_PARTITION variable=edge_vecs complete
			SAVEMISSES_EDGES_MAINLOOP1D: for(unsigned int t=0; t<max; t++){ 
			#pragma HLS PIPELINE II=1
				for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
				#pragma HLS UNROLL
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						if(t< MISSBUFFER_edges_index[inst][v]){ edge_vecs[inst].data[v] = MISSBUFFER_edges[inst][v][t]; } 
						else { edge_vecs[inst].data[v] = invalid_edge_data; }
					}
				}
				dinsertmanytoB_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets_eu, t, edge_vecs,  HBM_channelA0, HBM_channelB0); // FIXME
			}
			#endif 
			
			// reset URAM buffers
			unsigned int countZ = 0;
			MY_LOOP1583: for(unsigned int t=0; t<max_sz_eu; t++){
			#pragma HLS PIPELINE II=2
				for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
				#pragma HLS UNROLL
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						edge_update_type edge_update = edge_updates_buffer_vecs[inst][t].data[v];
						if(edge_update.srcvid < EDGE_UPDATES_DRAMBUFFER_SIZE){ 
							unsigned int edge_hashid = edgeupdates_hash_ids[inst][v][t];
							URAM_map[inst][v][edge_update.srcvid].offset = 0; 
							URAM_map[inst][v][edge_update.srcvid].size = 0;
							URAM_edgeupdates[inst][v][edge_hashid].srcvid = INVALIDDATA; //
							URAM_edgeupdates[inst][v][edge_hashid].dstvid = INVALIDDATA; //
						}
					}
				}
			}
		} // c 
		// exit(EXIT_SUCCESS);
	} // llp_id
	// exit(EXIT_SUCCESS); 
} // llp_set
// exit(EXIT_SUCCESS); 
// } // p_u
// exit(EXIT_SUCCESS); 





















								
				}	
				#endif	
				// exit(EXIT_SUCCESS);
				
				// read & map frontier properties 
				#ifdef ___ENABLE___READ_FRONTIER_PROPERTIES___
				if(___ENABLE___READ_FRONTIER_PROPERTIES___BOOL___ == 1){ 
// broadcast active frontiers [done]
unsigned int uoffset_vecsz = p_u * MAX_UPARTITION_VECSIZE; 
unsigned int data[HBM_CHANNEL_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable=data complete
unsigned int offset_c = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP];

for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = 0; }
#ifdef _DEBUGMODE_CHECKS3
checkoutofbounds("acts_kernel::ERROR 12073::", cfrontier_dram___size[p_u], MAX_APPLYPARTITION_VECSIZE+1, NAp, NAp, NAp);
#endif 

unsigned int import_offset = id_import * action.size_import_export;
unsigned int export_offset = id_export * action.size_import_export;

READ_FRONTIER_INFO_LOOP1: for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){ 
#pragma HLS PIPELINE II=1	
	data[0] = HBM_centerA[uoffset_vecsz + t].data[0];
	data[1] = HBM_centerA[uoffset_vecsz + t].data[1];
	data[2] = HBM_centerA[uoffset_vecsz + t].data[2];
	data[3] = HBM_centerA[uoffset_vecsz + t].data[3];
	data[4] = HBM_centerA[uoffset_vecsz + t].data[4];
	data[5] = HBM_centerA[uoffset_vecsz + t].data[5];
	data[6] = HBM_centerA[uoffset_vecsz + t].data[6];
	data[7] = HBM_centerA[uoffset_vecsz + t].data[7];
	data[8] = HBM_centerA[uoffset_vecsz + t].data[8];
	data[9] = HBM_centerA[uoffset_vecsz + t].data[9];
	data[10] = HBM_centerA[uoffset_vecsz + t].data[10];
	data[11] = HBM_centerA[uoffset_vecsz + t].data[11];
	data[12] = HBM_centerA[uoffset_vecsz + t].data[12];
	data[13] = HBM_centerA[uoffset_vecsz + t].data[13];
	data[14] = HBM_centerA[uoffset_vecsz + t].data[14];
	data[15] = HBM_centerA[uoffset_vecsz + t].data[15];
	data[16] = HBM_centerB[uoffset_vecsz + t].data[0];
	data[17] = HBM_centerB[uoffset_vecsz + t].data[1];
	data[18] = HBM_centerB[uoffset_vecsz + t].data[2];
	data[19] = HBM_centerB[uoffset_vecsz + t].data[3];
	data[20] = HBM_centerB[uoffset_vecsz + t].data[4];
	data[21] = HBM_centerB[uoffset_vecsz + t].data[5];
	data[22] = HBM_centerB[uoffset_vecsz + t].data[6];
	data[23] = HBM_centerB[uoffset_vecsz + t].data[7];
	data[24] = HBM_centerB[uoffset_vecsz + t].data[8];
	data[25] = HBM_centerB[uoffset_vecsz + t].data[9];
	data[26] = HBM_centerB[uoffset_vecsz + t].data[10];
	data[27] = HBM_centerB[uoffset_vecsz + t].data[11];
	data[28] = HBM_centerB[uoffset_vecsz + t].data[12];
	data[29] = HBM_centerB[uoffset_vecsz + t].data[13];
	data[30] = HBM_centerB[uoffset_vecsz + t].data[14];
	data[31] = HBM_centerB[uoffset_vecsz + t].data[15];
	
	HBM_channelA0[offset_c + t].data[0] = data[0];
	HBM_channelA0[offset_c + t].data[1] = data[1];
	HBM_channelA0[offset_c + t].data[2] = data[2];
	HBM_channelA0[offset_c + t].data[3] = data[3];
	HBM_channelA0[offset_c + t].data[4] = data[4];
	HBM_channelA0[offset_c + t].data[5] = data[5];
	HBM_channelA0[offset_c + t].data[6] = data[6];
	HBM_channelA0[offset_c + t].data[7] = data[7];
	HBM_channelA0[offset_c + t].data[8] = data[8];
	HBM_channelA0[offset_c + t].data[9] = data[9];
	HBM_channelA0[offset_c + t].data[10] = data[10];
	HBM_channelA0[offset_c + t].data[11] = data[11];
	HBM_channelA0[offset_c + t].data[12] = data[12];
	HBM_channelA0[offset_c + t].data[13] = data[13];
	HBM_channelA0[offset_c + t].data[14] = data[14];
	HBM_channelA0[offset_c + t].data[15] = data[15];
	HBM_channelB0[offset_c + t].data[0] = data[16];
	HBM_channelB0[offset_c + t].data[1] = data[17];
	HBM_channelB0[offset_c + t].data[2] = data[18];
	HBM_channelB0[offset_c + t].data[3] = data[19];
	HBM_channelB0[offset_c + t].data[4] = data[20];
	HBM_channelB0[offset_c + t].data[5] = data[21];
	HBM_channelB0[offset_c + t].data[6] = data[22];
	HBM_channelB0[offset_c + t].data[7] = data[23];
	HBM_channelB0[offset_c + t].data[8] = data[24];
	HBM_channelB0[offset_c + t].data[9] = data[25];
	HBM_channelB0[offset_c + t].data[10] = data[26];
	HBM_channelB0[offset_c + t].data[11] = data[27];
	HBM_channelB0[offset_c + t].data[12] = data[28];
	HBM_channelB0[offset_c + t].data[13] = data[29];
	HBM_channelB0[offset_c + t].data[14] = data[30];
	HBM_channelB0[offset_c + t].data[15] = data[31];
	
	// import (Host -> FPGA)
	if(action.id_import != INVALIDDATA && t<action.size_import_export){	
		HBM_centerA[import_offset + t].data[0] = HBM_import[t].data[0];
		HBM_centerA[import_offset + t].data[1] = HBM_import[t].data[1];
		HBM_centerA[import_offset + t].data[2] = HBM_import[t].data[2];
		HBM_centerA[import_offset + t].data[3] = HBM_import[t].data[3];
		HBM_centerA[import_offset + t].data[4] = HBM_import[t].data[4];
		HBM_centerA[import_offset + t].data[5] = HBM_import[t].data[5];
		HBM_centerA[import_offset + t].data[6] = HBM_import[t].data[6];
		HBM_centerA[import_offset + t].data[7] = HBM_import[t].data[7];
		HBM_centerA[import_offset + t].data[8] = HBM_import[t].data[8];
		HBM_centerA[import_offset + t].data[9] = HBM_import[t].data[9];
		HBM_centerA[import_offset + t].data[10] = HBM_import[t].data[10];
		HBM_centerA[import_offset + t].data[11] = HBM_import[t].data[11];
		HBM_centerA[import_offset + t].data[12] = HBM_import[t].data[12];
		HBM_centerA[import_offset + t].data[13] = HBM_import[t].data[13];
		HBM_centerA[import_offset + t].data[14] = HBM_import[t].data[14];
		HBM_centerA[import_offset + t].data[15] = HBM_import[t].data[15];
	
	}		
	
	// export (FPGA -> Host)
	if(action.id_export != INVALIDDATA && t<action.size_import_export){	
		HBM_export[t].data[0] = HBM_centerB[export_offset + t].data[0];
		HBM_export[t].data[1] = HBM_centerB[export_offset + t].data[1];
		HBM_export[t].data[2] = HBM_centerB[export_offset + t].data[2];
		HBM_export[t].data[3] = HBM_centerB[export_offset + t].data[3];
		HBM_export[t].data[4] = HBM_centerB[export_offset + t].data[4];
		HBM_export[t].data[5] = HBM_centerB[export_offset + t].data[5];
		HBM_export[t].data[6] = HBM_centerB[export_offset + t].data[6];
		HBM_export[t].data[7] = HBM_centerB[export_offset + t].data[7];
		HBM_export[t].data[8] = HBM_centerB[export_offset + t].data[8];
		HBM_export[t].data[9] = HBM_centerB[export_offset + t].data[9];
		HBM_export[t].data[10] = HBM_centerB[export_offset + t].data[10];
		HBM_export[t].data[11] = HBM_centerB[export_offset + t].data[11];
		HBM_export[t].data[12] = HBM_centerB[export_offset + t].data[12];
		HBM_export[t].data[13] = HBM_centerB[export_offset + t].data[13];
		HBM_export[t].data[14] = HBM_centerB[export_offset + t].data[14];
		HBM_export[t].data[15] = HBM_centerB[export_offset + t].data[15];
	}
}

// parallel-read and map active frontiers [done]
MY_LOOP173: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = 0; }
unsigned int uoffset = p_u * MAX_UPARTITION_SIZE;
MY_LOOP175_DEBUG: for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){ 
#pragma HLS PIPELINE II=1
	dretrievemany_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], offsets, t, kvdatas,  HBM_channelA0, HBM_channelB0);
	insertvec_cfrontierbuffer_tmp(t, kvdatas[0], URAM_frontiers); // NEW
	for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
	#pragma HLS UNROLL
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			unsigned int srcvid_lpv__ = (kvdatas[inst][v].key - uoffset) >> EDGE_PACK_SIZE_POW;
			if(kvdatas[inst][v].key != INVALIDDATA && srcvid_lpv__ < MAX_UPARTITION_VECSIZE){
				unsigned int srcvid_lpv = srcvid_lpv__; 
				vprop_t tmp; tmp.prop = kvdatas[inst][v].value; tmp.degree = MASK_CODE_PE;
				insert_vdatabuffer(v, srcvid_lpv, tmp, URAM_vprop[inst]); 
			}
		}
	}
	update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_FRONTIER_PROPERTIES___, 1);
}


					
				}
				#endif
				
				#ifdef _DEBUGMODE_KERNELPRINTS4_XXXXXXXXXX				
				if(action.id_export != INVALIDDATA){ cout<< TIMINGRESULTSCOLOR << "--> importing upartition: "<<action.id_export<<"..."<< RESET <<endl; }	
				cout<< TIMINGRESULTSCOLOR << "--- processing upartition: "<<action.id_process<<"..."<< RESET <<endl; 	
				if(action.id_import != INVALIDDATA){ cout<< TIMINGRESULTSCOLOR << "<-- exporting vpartition: "<<action.id_import<<"..."<< RESET <<endl; }					
				#endif 
				
				// process-edges and partition-updates
				#ifdef ___ENABLE___ECPROCESSEDGES___II1___		
				if(___ENABLE___PROCESSEDGES___BOOL___ == 1){ 
unsigned int offsets[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = offsets complete
offset_t offset_fpga[NUM_VALID_PEs][EDGE_PACK_SIZE][NUM_FPGAS];
#pragma HLS ARRAY_PARTITION variable=offset_fpga complete dim=1	
#pragma HLS ARRAY_PARTITION variable=offset_fpga complete dim=2	
map_t edge_maps[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable=edge_maps complete	
keyvalue_t update_in[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = update_in complete dim=0
keyvalue_t update_out[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = update_out complete dim=0

#ifdef _DEBUGMODE_KERNELPRINTS4 
cout<<"### processing edges in upartition "<<p_u<<": [PEs "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<n<<", "; } cout<<"]"<<endl; 
#endif 

unsigned int import_offset = id_import * action.size_import_export;
unsigned int export_offset = id_export * action.size_import_export;

EC_PROCESS_EDGES_LOOP1: for(unsigned int llp_set=0; llp_set<__NUM_ACTIVE_LLPSETS; llp_set++){	// __NUM_APPLYPARTITIONS
	EC_PROCESS_EDGES_LOOP1B: for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){ 
		// load edges map
		load_edgemaps((p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id, globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2], edge_maps,  HBM_channelA0, HBM_channelB0);
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ edge_maps[n].size = ((__PERCENTAGE_ACTIVE_EDGES * edge_maps[n].size) + (100-1)) / 100; } // FIXME.
		
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"------------------- process edges: ["<<p_u<<", "<<llp_set<<", "<<llp_id<<", "<<edge_maps[0].size<<"] [importing and exporting "<<action.size_import_export<<" 32bit-integers] -------------------"<<endl; 
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<"process-edges: edge_maps["<<n<<"]["<<llp_id<<"].size: "<<edge_maps[n].size<<endl; }
		#endif 
		
		// prepare maps, offsets, variables 
		max_sz = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ limits[n] = edge_maps[n].size; } for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max_sz < limits[n]){ max_sz = limits[n]; }}			
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = edge_maps[n].offset; }	
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<"process-edges: edges map offsets: "; cout<<offsets[n]<<", max_sz: "<<max_sz<<", updatesptrs["<<action.fpga<<"]["<<llp_set<<"].offset: "<<updatesptrs[action.fpga][llp_set].offset<<", updatesptrs["<<action.fpga<<"]["<<llp_set<<"].size: "<<updatesptrs[action.fpga][llp_set].size<<endl; }				
		#endif 	
		
		// process edges and save vertex updates 
		unsigned int batch_size = 512;
		PROCESS_EDGES_MAINLOOP1C: for(unsigned int t1=0; t1<(max_sz + batch_size - 1) / batch_size; t1++){ 
		
			// resets 
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
			#pragma HLS PIPELINE II=1
				for(unsigned int n=0; n<NUM_VALID_PEs; n++){
				#pragma HLS UNROLL
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						offset_fpga[n][v][fpga].offset = 0; offset_fpga[n][v][fpga].size = 0; 
					}
				}
			}
			
			// process edges 
			PROCESS_EDGES_MAINLOOP1D: for(unsigned int t2=0; t2<batch_size; t2++){ 
			#pragma HLS PIPELINE II=1
				unsigned int t = (t1 * 512) + t2;
				dretrievemanyfromA_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0);
				
				EC_PROCESS_EDGES_LOOP1F: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
				#pragma HLS UNROLL
					bool en = true; if(t >= limits[inst]){ en = false; } else { en = true; } 
					edge3_vec_dt edge_vec = edge3_vecs[inst];
					unsigned int rotateby = llp_id;
					
					#ifdef _DEBUGMODE_CHECKS3
					checkoutofbounds("acts_kernel::ERROR 213::", rotateby, EDGE_PACK_SIZE, NAp, NAp, NAp);
					#endif
					
					// read source properties and process edge to form vertex-updates 
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						edge3_type edge = edge_vec.data[v];
						unsigned int srcvid_lpv = edge.srcvid; 
						vprop_t uprop; if(edge.srcvid != INVALIDDATA && edge.srcvid < MAX_UPARTITION_VECSIZE){ uprop = retrieve_vdatabuffer(v, srcvid_lpv, URAM_vprop[inst]); }
						if(edge.srcvid != INVALIDDATA && edge.srcvid < MAX_UPARTITION_VECSIZE){ // && (uprop.degree == MASK_CODE_PE) // FIXME.
							unsigned int res = process_funcG(uprop, 1, globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM]);
							update_in[inst][v].key = edge.dstvid; update_in[inst][v].value = res;
							#ifdef _DEBUGMODE_CHECKS3
							checkoutofbounds("acts_kernel::ERROR 1213c::", edge.srcvid, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
							checkoutofbounds("acts_kernel::ERROR 1213d::", edge.dstvid, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
							#endif
							#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
							std::cout<<"EC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [srcvid_lpv: "<<srcvid_lpv<<", dstvid: "<<edge.dstvid % MAX_APPLYPARTITION_SIZE<<"], [edge.srcvid: "<<edge.srcvid<<", dstvid: "<<edge.dstvid<<"]"<<std::endl; 
							#endif 
						} else {
							update_in[inst][v].key = INVALIDDATA; update_in[inst][v].value = INVALIDDATA;
						}
					}
					
					// circular shift >>>
					rearrangeLayoutVx16B(rotateby, update_in[inst], update_out[inst]); 
					
					// buffer updates 
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						if(update_out[inst][v].key != INVALIDDATA){
							unsigned int fpga = update_out[inst][v].key % NUM_FPGAS; // FIXME?
							URAM_updates[inst][v][(fpga * batch_size) + offset_fpga[inst][v][fpga].size] = update_out[inst][v];
							offset_fpga[inst][v][fpga].size += 1;
							#ifdef _DEBUGMODE_CHECKS3	
							checkoutofbounds("acts_kernel::process-edges::ERROR 8873rrr::", offset_fpga[inst][v][fpga].size, 8192, NAp, offset_fpga[inst][v][fpga].offset, NAp); 
							#endif
						}
					}
				}
				update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
			}
			// exit(EXIT_SUCCESS);
			
			// save vertex updates
			SAVE_VERTEXUPDATES_MAINLOOP1: for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4 
				cout<<"### saving "<<offset_fpga[0][0][fpga].size<<" vertex updates into fpga partition "<<fpga<<"..."<<endl; 
				#endif 
				SAVE_VERTEXUPDATES_MAINLOOP1B: for(unsigned int t=0; t<offset_fpga[0][0][fpga].size; t++){ // FIXME? // FIXHANG? hanging issues?
				#pragma HLS PIPELINE II=1
					for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 	
					#pragma HLS UNROLL
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							update_out[inst][v] = URAM_updates[inst][v][offset_fpga[inst][v][fpga].offset + t];
						}
					}
					
					for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 	
					#pragma HLS UNROLL
						ens[inst][0] = true;	
						offsets3[inst] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES] + updatesptrs[fpga][llp_set].offset + updatesptrs[fpga][llp_set].size + t;
					}
					
					#ifdef _DEBUGMODE_CHECKS3	
					checkoutofbounds("acts_kernel::process-edges::ERROR 8813rrr::", offsets3[0], globalparams_debug[GLOBALPARAMSCODE__BASEOFFSET__VDATAS], NAp, updatesptrs[fpga][llp_set].size, NAp); 
					#endif	
					dinsertmany_updatesdram(offsets3, update_out, ens,  HBM_channelA0, HBM_channelB0);	
				}
			}
			
			// update variables 
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
			#pragma HLS UNROLL
				updatesptrs[fpga][llp_set].size += offset_fpga[0][0][fpga].size;
			}
			
			// collect stats
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ vpartition_vertices[n][llp_set].count += max_sz; }
		}
		// exit(EXIT_SUCCESS);
	}
	// exit(EXIT_SUCCESS); 
}
// exit(EXIT_SUCCESS); 


								
				}
				#endif
			}
			}
			#endif 
			// exit(EXIT_SUCCESS); 
			
			// prepare active destID stats [done] 
			#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
			if(___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ == 1){
if(enable___collectactivedstvids == true){
	PREPARE_ACTIVE_DESTIDSTATS_LOOP1C: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
	#pragma HLS PIPELINE II=4 // CRITICAL NEWCHANGE
		PREPARE_ACTIVE_DESTIDSTATS_LOOP1D: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ // MAX_NUM_APPLYPARTITIONS, __NUM_APPLYPARTITIONS // CRITICAL NEWCHANGE
		#pragma HLS UNROLL
			PREPARE_ACTIVE_DESTIDSTATS_LOOP1E: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				if(retrieve_statsbuffer(t, p_v, statsbuffer_maskbased[inst]) == 1){ 
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					std::cout<<"VC - PREPARE ACTIVE DESTIDS SEEN @: [inst: "<<inst<<"], [p_v: "<<p_v<<", t: "<<t<<"]"<<std::endl;
					#endif
					insert_stats2buffer(statsbuffer_idbased___size[inst][p_v], p_v, t, statsbuffer_idbased[inst]); 
					statsbuffer_idbased___size[inst][p_v] += 1;
				}
			}
		}
		update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECTACTIVEDSTVIDS___, 4); // CRITICAL NEWCHANGE
	}
}	
			}
			#endif 
			
			unsigned int totalactvvs2 = 0; 
			frontier_t actvv[EDGE_PACK_SIZE]; 
			for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u++){ upartition_vertices[p_u].count = 0; }
			for(unsigned int p_u=0; p_u<globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS]; p_u++){ cfrontier_dram___size[p_u] = 0; } // reset

			// apply updates 
			#ifdef ___ENABLE___APPLYUPDATESMODULE___
			if(action.module == APPLY_UPDATES_MODULE || action.module == APPLY_UPDATES_MODULE___AND___GATHER_DSTPROPERTIES_MODULE || action.module == ALL_MODULES){
			APPLY_UPDATES_MODULE_LOOP: for(unsigned int p_v=action.start_pv; p_v<action.start_pv + action.size_pv; p_v++){
				#ifndef FPGA_IMPL
				checkoutofbounds("acts_kernel::ERROR 862::", p_v, globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS], p_v, globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS], NAp);
				#endif 
				bool en = true; if(enable___collectactivedstvids == true){ if(vpartition_vertices[0][p_v].count > 0){ en=true; } else { en=false; }} else { en = true; }
				unsigned int voffset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] + (p_v * MAX_APPLYPARTITION_VECSIZE);
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				cout<<"APPLY_UPDATES_MODULE_LOOP: vpartition_vertices[0]["<<p_v<<"].count: "<<vpartition_vertices[0][p_v].count<<" "<<endl;
				#endif 
				
				if(vpartition_vertices[0][p_v].count > 0 || all_vertices_active_in_all_iterations == true){ 
					// read destination properties
					#ifdef ___ENABLE___READ_DEST_PROPERTIES___
					if(___ENABLE___READ_DEST_PROPERTIES___BOOL___ == 1){
unsigned int import_offset = id_import * action.size_import_export;
unsigned int export_offset = id_export * action.size_import_export;

READ_DEST_PROPERTIES_LOOP1: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = 0; }
if(stats_buffer___size[0][p_v] < threshold___activedstvids && enable___collectactivedstvids == true){
	#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
	if(___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ == 1){
	max_limit = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max_limit < statsbuffer_idbased___size[n][p_v]){ max_limit = statsbuffer_idbased___size[n][p_v]; }}
	READ_DEST_PROPERTIES_LOOP1B: for(unsigned int k=0; k<max_limit; k++){
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			if(k < statsbuffer_idbased___size[n][p_v]){
				offsets[n] = retrieve_stats2buffer(k, p_v, statsbuffer_idbased[n]) * ACTVUPDATESBLOCK_VECSIZE;
				#ifndef FPGA_IMPL
				checkoutofbounds("acts_kernel::ERROR 117::", offsets[n], MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
				#endif 
			}
		}
		update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_DEST_PROPERTIES___, 1);
		READ_DEST_PROPERTIES_LOOP1C: for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
		#pragma HLS PIPELINE II=1
			dretrievemany_vdatadram(voffset, offsets, t, datas,  HBM_channelA0, HBM_channelB0);
			for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				if((k < statsbuffer_idbased___size[inst][p_v]) && (offsets[inst] + t < MAX_APPLYPARTITION_VECSIZE)){ 
					insertvec_vdatabuffer(offsets[inst] + t, datas[inst], URAM_vprop[inst]);
				}
			}
			update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_DEST_PROPERTIES___, 1);
		}
	}
	}
	#endif 
} else {
	READ_DEST_PROPERTIES_LOOP2B: for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
	#pragma HLS PIPELINE II=1
		dretrievemany_vdatadram(voffset, offsets, t, datas,  HBM_channelA0, HBM_channelB0);
		READ_DEST_PROPERTIES_LOOP2C: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			insertvec_vdatabuffer(t, datas[inst], URAM_vprop[inst]);
		}
		
		// import (Host -> FPGA)
		if(action.id_import != INVALIDDATA && t<action.size_import_export){	
			HBM_centerA[import_offset + t].data[0] = HBM_import[t].data[0];
			HBM_centerA[import_offset + t].data[1] = HBM_import[t].data[1];
			HBM_centerA[import_offset + t].data[2] = HBM_import[t].data[2];
			HBM_centerA[import_offset + t].data[3] = HBM_import[t].data[3];
			HBM_centerA[import_offset + t].data[4] = HBM_import[t].data[4];
			HBM_centerA[import_offset + t].data[5] = HBM_import[t].data[5];
			HBM_centerA[import_offset + t].data[6] = HBM_import[t].data[6];
			HBM_centerA[import_offset + t].data[7] = HBM_import[t].data[7];
			HBM_centerA[import_offset + t].data[8] = HBM_import[t].data[8];
			HBM_centerA[import_offset + t].data[9] = HBM_import[t].data[9];
			HBM_centerA[import_offset + t].data[10] = HBM_import[t].data[10];
			HBM_centerA[import_offset + t].data[11] = HBM_import[t].data[11];
			HBM_centerA[import_offset + t].data[12] = HBM_import[t].data[12];
			HBM_centerA[import_offset + t].data[13] = HBM_import[t].data[13];
			HBM_centerA[import_offset + t].data[14] = HBM_import[t].data[14];
			HBM_centerA[import_offset + t].data[15] = HBM_import[t].data[15];
	
		}		
		
		// export (FPGA -> Host)
		if(action.id_export != INVALIDDATA && t<action.size_import_export){	
			HBM_export[t].data[0] = HBM_centerB[export_offset + t].data[0];
			HBM_export[t].data[1] = HBM_centerB[export_offset + t].data[1];
			HBM_export[t].data[2] = HBM_centerB[export_offset + t].data[2];
			HBM_export[t].data[3] = HBM_centerB[export_offset + t].data[3];
			HBM_export[t].data[4] = HBM_centerB[export_offset + t].data[4];
			HBM_export[t].data[5] = HBM_centerB[export_offset + t].data[5];
			HBM_export[t].data[6] = HBM_centerB[export_offset + t].data[6];
			HBM_export[t].data[7] = HBM_centerB[export_offset + t].data[7];
			HBM_export[t].data[8] = HBM_centerB[export_offset + t].data[8];
			HBM_export[t].data[9] = HBM_centerB[export_offset + t].data[9];
			HBM_export[t].data[10] = HBM_centerB[export_offset + t].data[10];
			HBM_export[t].data[11] = HBM_centerB[export_offset + t].data[11];
			HBM_export[t].data[12] = HBM_centerB[export_offset + t].data[12];
			HBM_export[t].data[13] = HBM_centerB[export_offset + t].data[13];
			HBM_export[t].data[14] = HBM_centerB[export_offset + t].data[14];
			HBM_export[t].data[15] = HBM_centerB[export_offset + t].data[15];
		}
		
		update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_DEST_PROPERTIES___, 1);
	}
}
					
					}
					#endif 
					
					#ifdef _DEBUGMODE_KERNELPRINTS4_XXXXXXXXXX
					if(action.id_import != INVALIDDATA){ cout<< TIMINGRESULTSCOLOR << "<-- exporting vpartition: "<<action.id_import<<"..."<< RESET <<endl; }	 
					if(action.id_export != INVALIDDATA){ cout<< TIMINGRESULTSCOLOR << "--> importing upartition: "<<action.id_export<<"..."<< RESET <<endl; }	
					#endif 
					
					// apply updates [done]
					#ifdef ___ENABLE___APPLYUPDATES___II1___ 
					if(___ENABLE___APPLYUPDATES___BOOL___ == 1){
						for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
uint512_vec_dt updates_vecs[NUM_VALID_PEs];
unsigned int import_offset = id_import * action.size_import_export;
unsigned int export_offset = id_export * action.size_import_export;
max_limit = 0; for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ limits[inst] = updatesptrs[fpga][p_v].size; } for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}

#ifdef _DEBUGMODE_KERNELPRINTS4 
if(en == true){ cout<<"### applying vertex updates in vpartition "<<p_v<<" (fpga "<<fpga<<"): [PEs "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<n<<", "; } cout<<"]"<<endl; } 
#endif 

APPLY_UPDATES_LOOP1: for(unsigned int t=0; t<max_limit; t++){			
#pragma HLS PIPELINE II=1
// #pragma HLS dependence variable=URAM_vprop type=inter false
	dretrievemany_udatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES] + updatesptrs[fpga][p_v].offset, p_v, t, updates_vecs,  HBM_channelA0, HBM_channelB0); // NEW
	for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
	#pragma HLS UNROLL
		// if(t < limits[inst]){
		uint512_vec_dt updates_vec = updates_vecs[inst];
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			keyvalue_t update = updates_vec.data[v];
			unsigned int dstvid_lp = update.key; 
			unsigned int dstvid_lpv = update.key / EDGE_PACK_SIZE;
			
			if(update.key < MAX_APPLYPARTITION_SIZE){
				#ifdef _DEBUGMODE_CHECKS3
				if(t < limits[inst]){ checkoutofbounds("acts_kernel::ERROR 727a::", dstvid_lp, MAX_APPLYPARTITION_SIZE, NAp, inst, update.key); checkoutofbounds("acts_kernel::ERROR 727b::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, NAp, inst, update.key); }					
				#endif
				// vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, URAM_vprop[inst]);
				vprop_t vprop; vprop.prop = dstvid_lpv; // FIXME.
				unsigned int new_vprop = reduce_funcG(vprop, update.value, globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM]);
				if(t < limits[inst]){  // && (new_vprop != vprop.prop) // FIXME.
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					std::cout<<"APPLY (ACT-PACK) UPDATE SEEN @: t: "<<t<<", inst: ["<<inst<<"]: dstvid_lp: "<<dstvid_lp<<", dstvid_lpv: "<<dstvid_lpv<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<std::endl;
					#endif
					vprop_t newprop; newprop.prop = new_vprop; newprop.degree = MASK_CODE_AU;
					insert_vdatabuffer(v, dstvid_lpv, newprop, URAM_vprop[inst]);
					
					if(inst==0 && v==0){ update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___APPLYUPDATES___, 1); }
				}
			}
		}
		// }
	}
	
	// #ifdef _DEBUGMODE_CHECKS3
	// checkoutofbounds("acts_kernel::apply-updates: ERROR 727a::", import_offset + t, HBM_CHANNEL_SIZE, import_offset, NAp, NAp);
	// checkoutofbounds("acts_kernel::apply-updates: ERROR 727b::", export_offset + t, HBM_CHANNEL_SIZE, export_offset, NAp, NAp);		
	// checkoutofbounds("acts_kernel::apply-updates: ERROR 727c::", t, IMPORT_EXPORT_GRANULARITY_VECSIZE, t, NAp, NAp);
	// checkoutofbounds("acts_kernel::apply-updates: ERROR 727d::", t, IMPORT_EXPORT_GRANULARITY_VECSIZE, t, NAp, NAp);	
	// checkoutofbounds("acts_kernel::apply-updates: ERROR 727e::", action.size_import_export, IMPORT_EXPORT_GRANULARITY_VECSIZE+1, t, NAp, NAp);	
	// #endif 
	
	/* // import (Host -> FPGA)
	if(action.id_import != INVALIDDATA && t<action.size_import_export){	
		HBM_centerA[import_offset + t].data[0] = HBM_import[t].data[0];
		HBM_centerA[import_offset + t].data[1] = HBM_import[t].data[1];
		HBM_centerA[import_offset + t].data[2] = HBM_import[t].data[2];
		HBM_centerA[import_offset + t].data[3] = HBM_import[t].data[3];
		HBM_centerA[import_offset + t].data[4] = HBM_import[t].data[4];
		HBM_centerA[import_offset + t].data[5] = HBM_import[t].data[5];
		HBM_centerA[import_offset + t].data[6] = HBM_import[t].data[6];
		HBM_centerA[import_offset + t].data[7] = HBM_import[t].data[7];
		HBM_centerA[import_offset + t].data[8] = HBM_import[t].data[8];
		HBM_centerA[import_offset + t].data[9] = HBM_import[t].data[9];
		HBM_centerA[import_offset + t].data[10] = HBM_import[t].data[10];
		HBM_centerA[import_offset + t].data[11] = HBM_import[t].data[11];
		HBM_centerA[import_offset + t].data[12] = HBM_import[t].data[12];
		HBM_centerA[import_offset + t].data[13] = HBM_import[t].data[13];
		HBM_centerA[import_offset + t].data[14] = HBM_import[t].data[14];
		HBM_centerA[import_offset + t].data[15] = HBM_import[t].data[15];
	
	}				
	
	// export (FPGA -> Host)
	if(action.id_export != INVALIDDATA && t<action.size_import_export){	
		HBM_export[t].data[0] = HBM_centerB[export_offset + t].data[0];
		HBM_export[t].data[1] = HBM_centerB[export_offset + t].data[1];
		HBM_export[t].data[2] = HBM_centerB[export_offset + t].data[2];
		HBM_export[t].data[3] = HBM_centerB[export_offset + t].data[3];
		HBM_export[t].data[4] = HBM_centerB[export_offset + t].data[4];
		HBM_export[t].data[5] = HBM_centerB[export_offset + t].data[5];
		HBM_export[t].data[6] = HBM_centerB[export_offset + t].data[6];
		HBM_export[t].data[7] = HBM_centerB[export_offset + t].data[7];
		HBM_export[t].data[8] = HBM_centerB[export_offset + t].data[8];
		HBM_export[t].data[9] = HBM_centerB[export_offset + t].data[9];
		HBM_export[t].data[10] = HBM_centerB[export_offset + t].data[10];
		HBM_export[t].data[11] = HBM_centerB[export_offset + t].data[11];
		HBM_export[t].data[12] = HBM_centerB[export_offset + t].data[12];
		HBM_export[t].data[13] = HBM_centerB[export_offset + t].data[13];
		HBM_export[t].data[14] = HBM_centerB[export_offset + t].data[14];
		HBM_export[t].data[15] = HBM_centerB[export_offset + t].data[15];
	}	 */	
}		

// #ifdef _DEBUGMODE_KERNELPRINTS4 
// if(en == true){ cout<<"### [---FINISH---] applying vertex updates in vpartition "<<p_v<<" (fpga "<<fpga<<"): [PEs "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<n<<", "; } cout<<"]"<<endl; } 
// #endif 	



	
						}						
					}
					#endif 
					
					// collect and save frontier information
					#ifdef ___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___
					if(___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___BOOL___ == 1){
keyvalue_t invalidkv; invalidkv.key = INVALIDDATA; invalidkv.value = INVALIDDATA;
keyvalue_t frontier_data[NUM_VALID_PEs][EDGE_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable = frontier_data complete dim=0

COLLECT_AND_SAVE_FRONTIERS_LOOP1: for(unsigned int local_subpartitionID=0; local_subpartitionID<NUM_SUBPARTITION_PER_PARTITION; local_subpartitionID++){
	
	unsigned int upartitionID = (p_v * NUM_SUBPARTITION_PER_PARTITION) + local_subpartitionID;
	unsigned int save_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + (upartitionID * MAX_APPLYPARTITION_VECSIZE);
	
	COLLECT_FRONTIERS_LOOP: for(unsigned int t=0; t<VDATA_SUBPARTITION_VECSIZE; t++){
	#pragma HLS PIPELINE II=1
		for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
			#pragma HLS UNROLL
				vprop_t vprop =  retrieve_vdatabuffer(v, t, URAM_vprop[inst]);
				unsigned int vid = 0; //FIXME.
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				std::cout<<"COLLECT FRONTIER INFORMATION SEEN @: inst: ["<<inst<<"]: t: "<<t<<", v: "<<v<<", p__u__: "<<(vid / MAX_UPARTITION_SIZE)<<", vid: "<<vid<<std::endl;
				#endif
				frontier_t actvv; actvv.key = vid; actvv.value = vprop.prop; 
				insert_nfrontierbuffer(v, t, actvv, nfrontier_buffer[inst]); 
			}
		}
		update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___, 1);
	}
	
	// save frontier information [done]
	SAVE_FRONTIERS_LOOP: for(unsigned int t=0; t<VDATA_SUBPARTITION_VECSIZE; t++){
	#pragma HLS PIPELINE II=1
		for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			retrievevec_nfrontierbuffer(t, frontier_data[inst], nfrontier_buffer[inst]); 
		}
		for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			offsets2[inst] = save_offset + t;
		}
		dinsertmany_nfrontierdram(save_offset, upartitionID, offsets2, t, frontier_data, ens,  HBM_channelA0, HBM_channelB0);
		// update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___, 1);
	}
	
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		nfrontier_dram___size[n][upartitionID] = VDATA_SUBPARTITION_VECSIZE; 
	}
}
					
					}
					#endif 
					
					// save destination properties
					#ifdef ___ENABLE___SAVE_DEST_PROPERTIES___
					if(___ENABLE___SAVE_DEST_PROPERTIES___BOOL___ == 1){	
MY_LOOP211: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = 0; }
if(stats_buffer___size[0][p_v] < threshold___activedstvids && enable___collectactivedstvids == true){
	#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
	if(___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ == 1){
	max_limit = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max_limit < statsbuffer_idbased___size[n][p_v]){ max_limit = statsbuffer_idbased___size[n][p_v]; }}
	SAVE_DEST_PROPERTIES_LOOP1: for(unsigned int k=0; k<max_limit; k++){	
		SAVE_DEST_PROPERTIES_LOOP1B: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			if(k < statsbuffer_idbased___size[n][p_v]){
				offsets[n] = retrieve_stats2buffer(k, p_v, statsbuffer_idbased[n]) * ACTVUPDATESBLOCK_VECSIZE;
				#ifndef FPGA_IMPL
				checkoutofbounds("acts_kernel::ERROR 1167::", k, BLOCKRAM_SIZE, NAp, NAp, NAp);
				checkoutofbounds("acts_kernel::ERROR 1171::", offsets[n], MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
				#endif 
			}
		}
		update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___SAVE_DEST_PROPERTIES___, 1);
		SAVE_DEST_PROPERTIES_LOOP1C: for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
		#pragma HLS PIPELINE II=1
			for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				if((k < statsbuffer_idbased___size[inst][p_v]) && (offsets[inst] + t < MAX_APPLYPARTITION_VECSIZE)){ 
					retrievevec_vdatabuffer(offsets[inst] + t, datas[inst], URAM_vprop[inst]);
					MY_LOOP215: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						datas[inst][v].mask = INVALIDMASK; // clear mask
					} 
				}
			}
			MY_LOOP10302: for(unsigned int n=0; n<NUM_VALID_PEs; n++){
			#pragma HLS UNROLL
				offsets2[n] = voffset + offsets[n] + t;
			}
			dinsertmany_vdatadram(voffset, offsets2, t, datas,  HBM_channelA0, HBM_channelB0);
			update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___SAVE_DEST_PROPERTIES___, 1);
		}
	}
	}
	#endif 
} else {
	SAVE_DEST_PROPERTIES_LOOP2: for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
	#pragma HLS PIPELINE II=1
		SAVE_DEST_PROPERTIES_LOOP2B: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			retrieveandresetvec_vdatabuffer(t, datas[inst], URAM_vprop[inst]);
		}
		MY_LOOP10202: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			offsets2[n] = voffset + offsets[n] + t; 
		}
		dinsertmany_vdatadram(voffset, offsets2, t, datas,  HBM_channelA0, HBM_channelB0);
		update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___SAVE_DEST_PROPERTIES___, 1);
	}
}
				
					}
					#endif 
					
					// gather frontiers to center hbm
					#ifdef ___ENABLE___GATHER_FRONTIERINFOS___
					if(___ENABLE___EXCHANGEFRONTIERINFOS___BOOL___ == 1){	
	
						#ifndef ___RUNNING_FPGA_SYNTHESIS___
						cout<<"### gathering dst vertex properties for vpartition "<<p_v<<": [PEs "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<n<<", "; } cout<<"]"<<endl; 
						#endif 
						gather_frontiers(0, p_v, cfrontier_dram___size, nfrontier_dram___size[0], upartition_vertices, HBM_channelA0, HBM_channelB0, HBM_centerA, HBM_centerB, globalparams, _NUMCLOCKCYCLES_);
	
					}
					#endif
				}
			} 
			}
			#endif
			
			#ifdef ___ENABLE___REPORT_STATISTICS___
#ifdef _DEBUGMODE_CHECKS3 

#ifdef _DEBUGMODE_KERNELPRINTS4
if(action.status == 1){ 
	cout<<"[acts: RESETBUFFERSATSTART, ";
	cout<<"READ_FRONTIERS, "; 
	cout<<"PROCESSEDGES, "; 
	cout<<"READ_DESTS, "; 
	cout<<"APPLYUPDATES, "; 
	cout<<"COLLECT_FRONTIERS, "; 
	cout<<"SAVE_DEST, "; 
	cout<<"GATHER_FRONTIERS] "; 
	cout<<endl;
}																										

cout<<">>> [";
cout<<_NUMCLOCKCYCLES_[0][___CODE___RESETBUFFERSATSTART___]<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___READ_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___ECPROCESSEDGES___] * EDGE_PACK_SIZE<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___READ_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATES___] * EDGE_PACK_SIZE<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___SAVE_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___GATHER_FRONTIERINFOS___] * EDGE_PACK_SIZE * NUM_PEs<<"";
cout<<"]";
cout<<endl;

#ifdef _DEBUGMODE_KERNELPRINTS//4
if(action.status == 1){ 
	cout<<"[# EDGEUPDATES_LOADED, "; 
	cout<<"# EDGEUPDATES_APPLIED, "; 
	cout<<"# NEW_EDGEUPDATES_ADDED]";
	cout<<endl;
}	

cout<<" [";
cout<<_NUMCLOCKCYCLES_[0][___CODE___NUMBER_OF_EDGEUPDATES_LOADED___] * EDGE_PACK_SIZE<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___NUMBER_OF_EDGEUPDATES_APPLIED___] * EDGE_PACK_SIZE<<", ";
cout<<_NUMCLOCKCYCLES_[0][___CODE___NUMBER_OF_NEW_EDGEUPDATES_ADDED___] * EDGE_PACK_SIZE<<"";
cout<<"]";
cout<<endl;
#endif 

report_statistics[___CODE___RESETBUFFERSATSTART___] += _NUMCLOCKCYCLES_[0][___CODE___RESETBUFFERSATSTART___];
report_statistics[___CODE___READ_FRONTIER_PROPERTIES___] += _NUMCLOCKCYCLES_[0][___CODE___READ_FRONTIER_PROPERTIES___];
report_statistics[___CODE___ECPROCESSEDGES___] += _NUMCLOCKCYCLES_[0][___CODE___ECPROCESSEDGES___];
report_statistics[___CODE___READ_DEST_PROPERTIES___] += _NUMCLOCKCYCLES_[0][___CODE___READ_DEST_PROPERTIES___];
report_statistics[___CODE___APPLYUPDATES___] += _NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATES___];
report_statistics[___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___] += _NUMCLOCKCYCLES_[0][___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___];
report_statistics[___CODE___SAVE_DEST_PROPERTIES___] += _NUMCLOCKCYCLES_[0][___CODE___SAVE_DEST_PROPERTIES___];
report_statistics[___CODE___GATHER_FRONTIERINFOS___] += _NUMCLOCKCYCLES_[0][___CODE___GATHER_FRONTIERINFOS___];

// cout<<"~~~~~~~~~~~~~~ report_statistics[___CODE___READ_FRONTIER_PROPERTIES___]: "<<report_statistics[___CODE___READ_FRONTIER_PROPERTIES___]<<", _NUMCLOCKCYCLES_[0][___CODE___READ_FRONTIER_PROPERTIES___]: "<<_NUMCLOCKCYCLES_[0][___CODE___READ_FRONTIER_PROPERTIES___]<<endl;				
#endif 

#ifdef _DEBUGMODE_KERNELPRINTS//4
if(action.status == 1){
	float total_cycles_iter = 
	+ _NUMCLOCKCYCLES_[0][___CODE___RESETBUFFERSATSTART___]
	+ _NUMCLOCKCYCLES_[0][___CODE___PROCESSEDGES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___READ_FRONTIER_PROPERTIES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___VCPROCESSEDGES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___ECPROCESSEDGES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___SAVEVCUPDATES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___COLLECTACTIVEDSTVIDS___]
	+ _NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATESMODULE___]
	+ _NUMCLOCKCYCLES_[0][___CODE___READ_DEST_PROPERTIES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___SAVE_DEST_PROPERTIES___]
	+ _NUMCLOCKCYCLES_[0][___CODE___GATHER_FRONTIERINFOS___];
	float time_lapse = (total_cycles_iter * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000;
	cout<<"acts_kernel SUMMARY:: number of clock cycles seen: "<<total_cycles_iter<<" ("<<time_lapse<<" milliseconds)"<<endl;
	float time_lapse1 = (total_cycles_iter / 1000000) * _NUMNANOSECONDS_PER_CLOCKCYCLE_;
	float time_lapse2 = (float)((total_cycles_iter * EDGE_PACK_SIZE) / 1000) / (float)ACTS_AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC;
	float million_edges_per_sec = ((globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES] / 1000000) / time_lapse) * 1000;
	float billion_edges_per_sec = million_edges_per_sec / 1000;
	cout<<"acts_kernel SUMMARY:: time elapsed for iteration "<<GraphIter<<": "<<time_lapse<<" milliseconds)"<<endl;
}
for(unsigned int t=0; t<16; t++){ total_cycles += _NUMCLOCKCYCLES_[0][t]; }
for(unsigned int t=0; t<16; t++){ _NUMCLOCKCYCLES_[0][t] = 0; }
#endif 
#endif	
			#endif 
			
#ifndef HW // _DEBUGMODE_KERNELPRINTS4
if(action.status == 1 && all_vertices_active_in_all_iterations == false){
	for(unsigned int t=0; t<__NUM_UPARTITIONS; t++){ if(hybrid_map[GraphIter][t] == 0){ std::cout<<"V, "; } else if(hybrid_map[GraphIter][t] == 1){ std::cout<<"E, "; } else { std::cout<<"-, "; }} std::cout<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<< TIMINGRESULTSCOLOR <<"### GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<< RESET << std::endl; 
	for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ if(upartition_vertices[p_u].count > 0){ std::cout<<"1, "; } else { std::cout<<"0, "; }} std::cout<<std::endl;
	if(totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS){ std::cout<<"no more active vertices to process. breaking out... "<<std::endl; }
}
#endif 
total_num_actvvs += totalactvvs2;
if((totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS) && (all_vertices_active_in_all_iterations != true)){ maxGraphIter = GraphIter; break; }
for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ nfrontier_dram___size[inst][p_u] = 0; }}	
		} // NUM_ITs
	}
	
	// save state
	if(action.module == PROCESS_EDGES_MODULE){ //  || action.module == ALL_MODULES){
		SAVE_UPDATEPTRS_lOOP1: for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
			SAVE_UPDATEPTRS_lOOP2: for(unsigned int t=0; t<globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; t++){	
				save_vupdate_map(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], (fpga * MAX_NUM_LLPSETS) + t, updatesptrs[fpga][t],  HBM_channelA0, HBM_channelB0);
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				cout<<"finish: updatesptrs["<<fpga<<"]["<<t<<"].offset: "<<updatesptrs[fpga][t].offset<<", updatesptrs["<<fpga<<"]["<<t<<"].size: "<<updatesptrs[fpga][t].size<<endl;
				#endif 
			}
		}
	}
	
	// report result
	#ifdef _DEBUGMODE_CHECKS3
	if(action.status == 1){
float time_lapse = (total_cycles * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000;
float million_edges_per_sec = (((globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES] * globalparams[GLOBALPARAMSCODE__PARAM__NUM_RUNS]) / 1000000) / time_lapse) * 1000;
float billion_edges_per_sec = million_edges_per_sec / 1000;
#ifdef _DEBUGMODE_KERNELPRINTS//4
cout<<"acts_kernel SUMMARY:: total number of active vertices processed / HBM CHANNEL: "<<total_num_actvvs<<endl;
cout<<"acts_kernel SUMMARY:: total number of edges processed per run: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES]<<endl;
cout<<"acts_kernel SUMMARY:: total number of runs: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_RUNS]<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen: "<<total_cycles<<" ("<<(total_cycles * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000<<" milliseconds)"<<endl;
cout<< TIMINGRESULTSCOLOR << ">>> acts_kernel SUMMARY:: TIME ELAPSE: "<<time_lapse<<" ms. THROUGHPUT: "<<million_edges_per_sec<<" MTEPS; THROUGHPUT: "<<billion_edges_per_sec<<" BTEPS"<< RESET << endl;
#endif 
	
	}
	#endif	
	
	#ifdef ___RUNNING_FPGA_SYNTHESIS___
	return;
	#else 
	return 0;	
	#endif 
}
}		