#include "../include/common.h"
#ifndef FPGA_IMPL
#include "acts_kernel.h"
using namespace std;
#endif 
// __NOT__YET__IMPLEMENTED___
// __OBSOLETE___
// #define ___FALSE___ 

#define ___ENABLE___RESETBUFFERSATSTART___ 
#define ___ENABLE___PROCESSEDGES___ 
#define ___ENABLE___READ_FRONTIER_PROPERTIES___
#define ___ENABLE___VCPROCESSEDGES___ 
#define ___ENABLE___ECPROCESSEDGES___ 
#define ___ENABLE___SAVEVCUPDATES___ 
// // // // // // // // // #define ___ENABLE___COLLECTACTIVEDSTVIDS___ 
#define ___ENABLE___APPLYUPDATESMODULE___ 
	#define ___ENABLE___READ_DEST_PROPERTIES___ 
	#define ___ENABLE___APPLYUPDATES___ 
	#define ___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___ 
	#define ___ENABLE___SAVE_DEST_PROPERTIES___ 
#define ___ENABLE___EXCHANGEFRONTIERINFOS___ 

#define ___CODE___RESETBUFFERSATSTART___ 0
#define ___CODE___PROCESSEDGES___ 1
#define ___CODE___READ_FRONTIER_PROPERTIES___ 2
#define ___CODE___VCPROCESSEDGES___ 3
#define ___CODE___ECPROCESSEDGES___ 4
#define ___CODE___SAVEVCUPDATES___ 5
#define ___CODE___COLLECTACTIVEDSTVIDS___ 6
#define ___CODE___APPLYUPDATESMODULE___ 7
	#define ___CODE___READ_DEST_PROPERTIES___ 8
	#define ___CODE___APPLYUPDATES___ 9
	#define ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___ 10
	#define ___CODE___SAVE_DEST_PROPERTIES___ 11
#define ___CODE___EXCHANGEFRONTIERINFOS___ 12

#define MAXVALID_APPLYPARTITION_VECSIZE (MAX_APPLYPARTITION_VECSIZE >> POW_VALID_VDATA)

#ifdef FPGA_IMPL
#define MY_IFDEF_NFRONTIER() keyvalue_t nfrontier_buffer[EDGE_PACK_SIZE][MAX_ACTVV_VECSIZE]
#define MY_IFDEF_CFRONTIER_TMP() keyvalue_t cfrontier_buffer_tmp[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_CFRONTIER() keyvalue_t cfrontier_buffer[VALID_NUMPEs][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_STATSBUFFERMASKBASED_TMP() unsigned int statsbuffer_maskbased_tmp[EDGE_PACK_SIZE][BLOCKRAM_SIZE]	
#define MY_IFDEF_STATSBUFFERMASKBASED() unsigned int statsbuffer_maskbased[MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE]	
#define MY_IFDEF_STATSBUFFERIDBASED() unsigned int statsbuffer_idbased[MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE]	
#define MY_IFDEF_UPDATESTMPBUFFER() keyvalue_t updates_buffer2[UPDATES_BUFFER_PACK_SIZE][UPDATES_BUFFER_SIZE]	
#define MY_IFDEF_VPTRBUFFER() vtr_t vptr_buffer[VPTR_BUFFER_SIZE]
#define MY_IFDEF_EDGESBUFFER() edge3_type edges_buffer[EDGE_PACK_SIZE][EDGE_BUFFER_SIZE]
#define MY_IFDEF_VDATABUFFER() vprop_t vdata_buffer[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_TOPLEVELFUNC() void top_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB)
#else
#define MY_IFDEF_NFRONTIER() keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_CFRONTIER_TMP() keyvalue_t * cfrontier_buffer_tmp[EDGE_PACK_SIZE]
#define MY_IFDEF_CFRONTIER() keyvalue_t * cfrontier_buffer[VALID_NUMPEs]
#define MY_IFDEF_STATSBUFFERMASKBASED_TMP() unsigned int * statsbuffer_maskbased_tmp[EDGE_PACK_SIZE]
#define MY_IFDEF_STATSBUFFERMASKBASED() unsigned int * statsbuffer_maskbased[MAX_NUM_APPLYPARTITIONS]
#define MY_IFDEF_STATSBUFFERIDBASED() unsigned int * statsbuffer_idbased[MAX_NUM_APPLYPARTITIONS]
#define MY_IFDEF_UPDATESTMPBUFFER() keyvalue_t * updates_buffer2[UPDATES_BUFFER_PACK_SIZE]
#define MY_IFDEF_VPTRBUFFER() vtr_t * vptr_buffer
#define MY_IFDEF_EDGESBUFFER() edge3_type * edges_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_VDATABUFFER() vprop_t * vdata_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_TOPLEVELFUNC() void acts_kernel::top_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB)
#endif

#ifndef FPGA_IMPL
unsigned int * globalparams_debug;
unsigned int _NUMCLOCKCYCLES_[2][16]; 
#endif 

#ifndef FPGA_IMPL
acts_kernel::acts_kernel(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
	algorithmobj = new algorithm();
}
acts_kernel::~acts_kernel(){} 
#endif 

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

void rearrangeLayoutVx16B(unsigned int s, keyvalue_t in[EDGE_PACK_SIZE], keyvalue_t out[EDGE_PACK_SIZE]){
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
void update_dramnumclockcycles(unsigned int _NUMCLOCKCYCLES_[2][16], unsigned int x, unsigned int clock_cycles){
	#ifdef _DEBUGMODE_CHECKS3
	_NUMCLOCKCYCLES_[0][x] += clock_cycles;
	#endif
}
void update_bramnumclockcycles(unsigned int _NUMCLOCKCYCLES_[2][16], unsigned int x, unsigned int clock_cycles){
	#ifdef _DEBUGMODE_CHECKS3
	_NUMCLOCKCYCLES_[0][x] += clock_cycles;
	// _NUMCLOCKCYCLES_[1][x] += clock_cycles;
	#endif
}

////////////////////////////////////////////////////////////////// 
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

//////////////////////////////////////////////////////////////////
void master_centerinsert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef FPGA_IMPL 
		HBM_centerA[offset].range(31, 0) = data[0];
		HBM_centerA[offset].range(63, 32) = data[1];
		HBM_centerA[offset].range(95, 64) = data[2];
		HBM_centerA[offset].range(127, 96) = data[3];
		HBM_centerA[offset].range(159, 128) = data[4];
		HBM_centerA[offset].range(191, 160) = data[5];
		HBM_centerA[offset].range(223, 192) = data[6];
		HBM_centerA[offset].range(255, 224) = data[7];
		HBM_centerA[offset].range(287, 256) = data[8];
		HBM_centerA[offset].range(319, 288) = data[9];
		HBM_centerA[offset].range(351, 320) = data[10];
		HBM_centerA[offset].range(383, 352) = data[11];
		HBM_centerA[offset].range(415, 384) = data[12];
		HBM_centerA[offset].range(447, 416) = data[13];
		HBM_centerA[offset].range(479, 448) = data[14];
		HBM_centerA[offset].range(511, 480) = data[15];
		HBM_centerB[offset].range(31, 0) = data[16];
		HBM_centerB[offset].range(63, 32) = data[17];
		HBM_centerB[offset].range(95, 64) = data[18];
		HBM_centerB[offset].range(127, 96) = data[19];
		HBM_centerB[offset].range(159, 128) = data[20];
		HBM_centerB[offset].range(191, 160) = data[21];
		HBM_centerB[offset].range(223, 192) = data[22];
		HBM_centerB[offset].range(255, 224) = data[23];
		HBM_centerB[offset].range(287, 256) = data[24];
		HBM_centerB[offset].range(319, 288) = data[25];
		HBM_centerB[offset].range(351, 320) = data[26];
		HBM_centerB[offset].range(383, 352) = data[27];
		HBM_centerB[offset].range(415, 384) = data[28];
		HBM_centerB[offset].range(447, 416) = data[29];
		HBM_centerB[offset].range(479, 448) = data[30];
		HBM_centerB[offset].range(511, 480) = data[31];
	#else 
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
	#endif
	return;
}
void master_centerretrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef FPGA_IMPL 
		data[0] = HBM_centerA[offset].range(31, 0);
		data[1] = HBM_centerA[offset].range(63, 32);
		data[2] = HBM_centerA[offset].range(95, 64);
		data[3] = HBM_centerA[offset].range(127, 96);
		data[4] = HBM_centerA[offset].range(159, 128);
		data[5] = HBM_centerA[offset].range(191, 160);
		data[6] = HBM_centerA[offset].range(223, 192);
		data[7] = HBM_centerA[offset].range(255, 224);
		data[8] = HBM_centerA[offset].range(287, 256);
		data[9] = HBM_centerA[offset].range(319, 288);
		data[10] = HBM_centerA[offset].range(351, 320);
		data[11] = HBM_centerA[offset].range(383, 352);
		data[12] = HBM_centerA[offset].range(415, 384);
		data[13] = HBM_centerA[offset].range(447, 416);
		data[14] = HBM_centerA[offset].range(479, 448);
		data[15] = HBM_centerA[offset].range(511, 480);
		data[16] = HBM_centerB[offset].range(31, 0);
		data[17] = HBM_centerB[offset].range(63, 32);
		data[18] = HBM_centerB[offset].range(95, 64);
		data[19] = HBM_centerB[offset].range(127, 96);
		data[20] = HBM_centerB[offset].range(159, 128);
		data[21] = HBM_centerB[offset].range(191, 160);
		data[22] = HBM_centerB[offset].range(223, 192);
		data[23] = HBM_centerB[offset].range(255, 224);
		data[24] = HBM_centerB[offset].range(287, 256);
		data[25] = HBM_centerB[offset].range(319, 288);
		data[26] = HBM_centerB[offset].range(351, 320);
		data[27] = HBM_centerB[offset].range(383, 352);
		data[28] = HBM_centerB[offset].range(415, 384);
		data[29] = HBM_centerB[offset].range(447, 416);
		data[30] = HBM_centerB[offset].range(479, 448);
		data[31] = HBM_centerB[offset].range(511, 480);
	#else 
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
	#endif
	return;
}

void master_insert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef FPGA_IMPL 
		HBM_channelA[offset].range(31, 0) = data[0];
		HBM_channelA[offset].range(63, 32) = data[1];
		HBM_channelA[offset].range(95, 64) = data[2];
		HBM_channelA[offset].range(127, 96) = data[3];
		HBM_channelA[offset].range(159, 128) = data[4];
		HBM_channelA[offset].range(191, 160) = data[5];
		HBM_channelA[offset].range(223, 192) = data[6];
		HBM_channelA[offset].range(255, 224) = data[7];
		HBM_channelA[offset].range(287, 256) = data[8];
		HBM_channelA[offset].range(319, 288) = data[9];
		HBM_channelA[offset].range(351, 320) = data[10];
		HBM_channelA[offset].range(383, 352) = data[11];
		HBM_channelA[offset].range(415, 384) = data[12];
		HBM_channelA[offset].range(447, 416) = data[13];
		HBM_channelA[offset].range(479, 448) = data[14];
		HBM_channelA[offset].range(511, 480) = data[15];
		HBM_channelB[offset].range(31, 0) = data[16];
		HBM_channelB[offset].range(63, 32) = data[17];
		HBM_channelB[offset].range(95, 64) = data[18];
		HBM_channelB[offset].range(127, 96) = data[19];
		HBM_channelB[offset].range(159, 128) = data[20];
		HBM_channelB[offset].range(191, 160) = data[21];
		HBM_channelB[offset].range(223, 192) = data[22];
		HBM_channelB[offset].range(255, 224) = data[23];
		HBM_channelB[offset].range(287, 256) = data[24];
		HBM_channelB[offset].range(319, 288) = data[25];
		HBM_channelB[offset].range(351, 320) = data[26];
		HBM_channelB[offset].range(383, 352) = data[27];
		HBM_channelB[offset].range(415, 384) = data[28];
		HBM_channelB[offset].range(447, 416) = data[29];
		HBM_channelB[offset].range(479, 448) = data[30];
		HBM_channelB[offset].range(511, 480) = data[31];
	#else 
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
	#endif
	return;
}
void master_retrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef FPGA_IMPL 
		data[0] = HBM_channelA[offset].range(31, 0);
		data[1] = HBM_channelA[offset].range(63, 32);
		data[2] = HBM_channelA[offset].range(95, 64);
		data[3] = HBM_channelA[offset].range(127, 96);
		data[4] = HBM_channelA[offset].range(159, 128);
		data[5] = HBM_channelA[offset].range(191, 160);
		data[6] = HBM_channelA[offset].range(223, 192);
		data[7] = HBM_channelA[offset].range(255, 224);
		data[8] = HBM_channelA[offset].range(287, 256);
		data[9] = HBM_channelA[offset].range(319, 288);
		data[10] = HBM_channelA[offset].range(351, 320);
		data[11] = HBM_channelA[offset].range(383, 352);
		data[12] = HBM_channelA[offset].range(415, 384);
		data[13] = HBM_channelA[offset].range(447, 416);
		data[14] = HBM_channelA[offset].range(479, 448);
		data[15] = HBM_channelA[offset].range(511, 480);
		data[16] = HBM_channelB[offset].range(31, 0);
		data[17] = HBM_channelB[offset].range(63, 32);
		data[18] = HBM_channelB[offset].range(95, 64);
		data[19] = HBM_channelB[offset].range(127, 96);
		data[20] = HBM_channelB[offset].range(159, 128);
		data[21] = HBM_channelB[offset].range(191, 160);
		data[22] = HBM_channelB[offset].range(223, 192);
		data[23] = HBM_channelB[offset].range(255, 224);
		data[24] = HBM_channelB[offset].range(287, 256);
		data[25] = HBM_channelB[offset].range(319, 288);
		data[26] = HBM_channelB[offset].range(351, 320);
		data[27] = HBM_channelB[offset].range(383, 352);
		data[28] = HBM_channelB[offset].range(415, 384);
		data[29] = HBM_channelB[offset].range(447, 416);
		data[30] = HBM_channelB[offset].range(479, 448);
		data[31] = HBM_channelB[offset].range(511, 480);
	#else 
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
	#endif 
	return;
}

void master_insertmany_vec(unsigned int offsets[VALID_NUMPEs], bool ens[VALID_NUMPEs], unsigned int data[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef FPGA_IMPL 
	
			HBM_channelA0[offsets[0]].range(31, 0) = data[0][0];
			HBM_channelA0[offsets[0]].range(63, 32) = data[0][1];
			HBM_channelA0[offsets[0]].range(95, 64) = data[0][2];
			HBM_channelA0[offsets[0]].range(127, 96) = data[0][3];
			HBM_channelA0[offsets[0]].range(159, 128) = data[0][4];
			HBM_channelA0[offsets[0]].range(191, 160) = data[0][5];
			HBM_channelA0[offsets[0]].range(223, 192) = data[0][6];
			HBM_channelA0[offsets[0]].range(255, 224) = data[0][7];
			HBM_channelA0[offsets[0]].range(287, 256) = data[0][8];
			HBM_channelA0[offsets[0]].range(319, 288) = data[0][9];
			HBM_channelA0[offsets[0]].range(351, 320) = data[0][10];
			HBM_channelA0[offsets[0]].range(383, 352) = data[0][11];
			HBM_channelA0[offsets[0]].range(415, 384) = data[0][12];
			HBM_channelA0[offsets[0]].range(447, 416) = data[0][13];
			HBM_channelA0[offsets[0]].range(479, 448) = data[0][14];
			HBM_channelA0[offsets[0]].range(511, 480) = data[0][15];
			HBM_channelB0[offsets[0]].range(31, 0) = data[0][16];
			HBM_channelB0[offsets[0]].range(63, 32) = data[0][17];
			HBM_channelB0[offsets[0]].range(95, 64) = data[0][18];
			HBM_channelB0[offsets[0]].range(127, 96) = data[0][19];
			HBM_channelB0[offsets[0]].range(159, 128) = data[0][20];
			HBM_channelB0[offsets[0]].range(191, 160) = data[0][21];
			HBM_channelB0[offsets[0]].range(223, 192) = data[0][22];
			HBM_channelB0[offsets[0]].range(255, 224) = data[0][23];
			HBM_channelB0[offsets[0]].range(287, 256) = data[0][24];
			HBM_channelB0[offsets[0]].range(319, 288) = data[0][25];
			HBM_channelB0[offsets[0]].range(351, 320) = data[0][26];
			HBM_channelB0[offsets[0]].range(383, 352) = data[0][27];
			HBM_channelB0[offsets[0]].range(415, 384) = data[0][28];
			HBM_channelB0[offsets[0]].range(447, 416) = data[0][29];
			HBM_channelB0[offsets[0]].range(479, 448) = data[0][30];
			HBM_channelB0[offsets[0]].range(511, 480) = data[0][31];
	
	#else 
	
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
	
	#endif 
	return;
}
void master_insertmanyG_vec(unsigned int offsets[VALID_NUMPEs], bool ens[VALID_NUMPEs], unsigned int data[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef FPGA_IMPL 
	
		if(ens[0]==true){
			HBM_channelA0[offsets[0]].range(31, 0) = data[0][0];
			HBM_channelA0[offsets[0]].range(63, 32) = data[0][1];
			HBM_channelA0[offsets[0]].range(95, 64) = data[0][2];
			HBM_channelA0[offsets[0]].range(127, 96) = data[0][3];
			HBM_channelA0[offsets[0]].range(159, 128) = data[0][4];
			HBM_channelA0[offsets[0]].range(191, 160) = data[0][5];
			HBM_channelA0[offsets[0]].range(223, 192) = data[0][6];
			HBM_channelA0[offsets[0]].range(255, 224) = data[0][7];
			HBM_channelA0[offsets[0]].range(287, 256) = data[0][8];
			HBM_channelA0[offsets[0]].range(319, 288) = data[0][9];
			HBM_channelA0[offsets[0]].range(351, 320) = data[0][10];
			HBM_channelA0[offsets[0]].range(383, 352) = data[0][11];
			HBM_channelA0[offsets[0]].range(415, 384) = data[0][12];
			HBM_channelA0[offsets[0]].range(447, 416) = data[0][13];
			HBM_channelA0[offsets[0]].range(479, 448) = data[0][14];
			HBM_channelA0[offsets[0]].range(511, 480) = data[0][15];
			HBM_channelB0[offsets[0]].range(31, 0) = data[0][16];
			HBM_channelB0[offsets[0]].range(63, 32) = data[0][17];
			HBM_channelB0[offsets[0]].range(95, 64) = data[0][18];
			HBM_channelB0[offsets[0]].range(127, 96) = data[0][19];
			HBM_channelB0[offsets[0]].range(159, 128) = data[0][20];
			HBM_channelB0[offsets[0]].range(191, 160) = data[0][21];
			HBM_channelB0[offsets[0]].range(223, 192) = data[0][22];
			HBM_channelB0[offsets[0]].range(255, 224) = data[0][23];
			HBM_channelB0[offsets[0]].range(287, 256) = data[0][24];
			HBM_channelB0[offsets[0]].range(319, 288) = data[0][25];
			HBM_channelB0[offsets[0]].range(351, 320) = data[0][26];
			HBM_channelB0[offsets[0]].range(383, 352) = data[0][27];
			HBM_channelB0[offsets[0]].range(415, 384) = data[0][28];
			HBM_channelB0[offsets[0]].range(447, 416) = data[0][29];
			HBM_channelB0[offsets[0]].range(479, 448) = data[0][30];
			HBM_channelB0[offsets[0]].range(511, 480) = data[0][31];
	
		}
	#else 
	
		if(ens[0]==true){
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
	
		}
	#endif 
	return;
}
void master_retrievemany_vec(unsigned int offsets[VALID_NUMPEs], unsigned int data[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef FPGA_IMPL 
		data[0][0] = HBM_channelA0[offsets[0]].range(31, 0);
		data[0][1] = HBM_channelA0[offsets[0]].range(63, 32);
		data[0][2] = HBM_channelA0[offsets[0]].range(95, 64);
		data[0][3] = HBM_channelA0[offsets[0]].range(127, 96);
		data[0][4] = HBM_channelA0[offsets[0]].range(159, 128);
		data[0][5] = HBM_channelA0[offsets[0]].range(191, 160);
		data[0][6] = HBM_channelA0[offsets[0]].range(223, 192);
		data[0][7] = HBM_channelA0[offsets[0]].range(255, 224);
		data[0][8] = HBM_channelA0[offsets[0]].range(287, 256);
		data[0][9] = HBM_channelA0[offsets[0]].range(319, 288);
		data[0][10] = HBM_channelA0[offsets[0]].range(351, 320);
		data[0][11] = HBM_channelA0[offsets[0]].range(383, 352);
		data[0][12] = HBM_channelA0[offsets[0]].range(415, 384);
		data[0][13] = HBM_channelA0[offsets[0]].range(447, 416);
		data[0][14] = HBM_channelA0[offsets[0]].range(479, 448);
		data[0][15] = HBM_channelA0[offsets[0]].range(511, 480);
		data[0][16] = HBM_channelB0[offsets[0]].range(31, 0);
		data[0][17] = HBM_channelB0[offsets[0]].range(63, 32);
		data[0][18] = HBM_channelB0[offsets[0]].range(95, 64);
		data[0][19] = HBM_channelB0[offsets[0]].range(127, 96);
		data[0][20] = HBM_channelB0[offsets[0]].range(159, 128);
		data[0][21] = HBM_channelB0[offsets[0]].range(191, 160);
		data[0][22] = HBM_channelB0[offsets[0]].range(223, 192);
		data[0][23] = HBM_channelB0[offsets[0]].range(255, 224);
		data[0][24] = HBM_channelB0[offsets[0]].range(287, 256);
		data[0][25] = HBM_channelB0[offsets[0]].range(319, 288);
		data[0][26] = HBM_channelB0[offsets[0]].range(351, 320);
		data[0][27] = HBM_channelB0[offsets[0]].range(383, 352);
		data[0][28] = HBM_channelB0[offsets[0]].range(415, 384);
		data[0][29] = HBM_channelB0[offsets[0]].range(447, 416);
		data[0][30] = HBM_channelB0[offsets[0]].range(479, 448);
		data[0][31] = HBM_channelB0[offsets[0]].range(511, 480);
	#else
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
	#endif 
	return;
}

unsigned int master_retrieve(unsigned int offset, unsigned int v, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int datas[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	master_retrieve_vec(offset, datas, HBM_channelA, HBM_channelB, inst);
	return datas[v];
}
void master_retrievemany(unsigned int offsets[VALID_NUMPEs], unsigned int v, unsigned int data[VALID_NUMPEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int datas[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete dim=0
	master_retrievemany_vec(offsets, datas,  HBM_channelA0, HBM_channelB0);
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
		data[n] = datas[n][v];
	}
}

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
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
	checkoutofbounds("acts_kernel::ERROR 901::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, index, HBM_CHANNEL_PACK_SIZE, NAp);
	#endif 
	return master_retrieve(base_offset__ + (index / HBM_CHANNEL_PACK_SIZE), index % HBM_CHANNEL_PACK_SIZE, HBM_channelA, HBM_channelB, inst);
}	
void dretrievemany_vptrdram(unsigned int base_offset__, unsigned int offsets[VALID_NUMPEs], unsigned int data[VALID_NUMPEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	data[0] = dretrieve_vptrdram(base_offset__, offsets[0], HBM_channelA0, HBM_channelB0, 0);
	return;
}

map_t dretrieve_actpackvptrdram(unsigned int base_offset__, unsigned int p_u, unsigned int llp_set, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
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
void dretrievemany_actpackvptrdram(unsigned int base_offset__, unsigned int p_u, unsigned int llp_set, map_t maps[VALID_NUMPEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	unsigned int offset = ((p_u * MAX_NUM_LLPSETS) + llp_set) * 2; 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS];
	checkoutofbounds("acts_kernel::ERROR 702::", offset / HBM_CHANNEL_PACK_SIZE, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int i = offset / HBM_CHANNEL_PACK_SIZE;
	unsigned int j = offset % HBM_CHANNEL_PACK_SIZE;
	
	unsigned int data_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + i;
	}
	master_retrievemany(offsets_, j, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		maps[n].offset = data_[n];
	}
	master_retrievemany(offsets_, j + 1, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		maps[n].size = data_[n];
	}
	return;
}

edge3_vec_dt dretrieve_csredges(unsigned int base_offset__, unsigned int index, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
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

void dretrievemany_act_pack_edges(unsigned int base_offset__, unsigned int offsets[VALID_NUMPEs], unsigned int t, edge3_vec_dt edge3_vecs[VALID_NUMPEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 704f::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<VALID_NUMPEs; n++){
		#pragma HLS UNROLL
			edge3_vecs[n].data[v].srcvid = data_[n][2*v];
			edge3_vecs[n].data[v].dstvid = data_[n][2*v+1];
			edge3_vecs[n].data[v].weight = 1;
			edge3_vecs[n].data[v].valid = NAp;
		}
	}
	return;
}
void dretrievemany_actpackedges(unsigned int base_offset__, unsigned int offsets[VALID_NUMPEs], unsigned int t, uint512_vec_dt data[VALID_NUMPEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 704f::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<VALID_NUMPEs; n++){
			data[n].data[v].key = data_[n][2*v];
			data[n].data[v].value = data_[n][2*v+1];
		}
	}
	return;
}

unsigned int dretrieve_updatesptrs(unsigned int base_offset__, unsigned int index, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
	checkoutofbounds("acts_kernel::ERROR 701::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, inst, NAp, NAp);
	#endif 
	return master_retrieve(base_offset__ + index, 0, HBM_channelA, HBM_channelB, inst); // NEW
}

void dinsert_updatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst, unsigned int globalparams[32], unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	checkoutofbounds("acts_kernel::ERROR 708a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 708b::", llp_set, MAX_NUM_APPLYPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_kernel::ERROR 708c::", index, HBM_CHANNEL_SIZE, index, v, NAp);
		#endif 
		data_[2*v] = data.data[v].key;
		data_[2*v + 1] = data.data[v].value;
	}
	master_insert_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
}
void dretrieve_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt * data, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst, unsigned int globalparams[32], unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	checkoutofbounds("acts_kernel::ERROR 710a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	master_retrieve_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 710b::", llp_set, MAX_NUM_APPLYPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_kernel::ERROR 710c::", index, HBM_CHANNEL_SIZE, index, v, NAp);
		#endif 
		data->data[v].key = data_[2*v]; 
		data->data[v].value = data_[2*v + 1];		
	}
	return; 
}
void dinsertmany_updatesdram(unsigned int offset__, unsigned int llp_set, unsigned int offsets[VALID_NUMPEs], unsigned int t, uint512_vec_dt data[VALID_NUMPEs], bool ens[VALID_NUMPEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	for(unsigned int n=0; n<VALID_NUMPEs; n++){ checkoutofbounds("acts_kernel::ERROR 708a::", offsets[n], globalparams_debug[GLOBALPARAMSCODE__BASEOFFSET__VDATAS], NAp, NAp, NAp); }
	#endif 
	
	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	bool ens_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=ens_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offsets[n]; 
	}
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		ens_[n] = ens[n][0];
	}	
	for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
	#pragma HLS UNROLL
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			if(ens[n][v] == true){
				data_[n][2*v] = data[n].data[v].key;
				data_[n][2*v+1] = data[n].data[v].value;
			} else {
				data_[n][2*v] = INVALIDDATA;
				data_[n][2*v+1] = INVALIDDATA;
			}
		}
	}

	// master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0); // CRITICAL FIXME. the issue.
	master_insertmanyG_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0); // CRITICAL FIXME. the issue.
}
void dretrievemany_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data[VALID_NUMPEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	checkoutofbounds("acts_kernel::ERROR 710a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 

	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + index;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<VALID_NUMPEs; n++){
		#pragma HLS UNROLL
			data[n].data[v].key = data_[n][2*v];
			data[n].data[v].value = data_[n][2*v+1];
		}
	}
	return; 
}

void dinsert_vdatadram(unsigned int offset__, unsigned int index, vprop_t data[EDGE_PACK_SIZE], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 711::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data_[2*v] = data[v].prop;
		data_[2*v + 1] = data[v].mask;
	}
	master_insert_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	return;
}
void dretrieve_vdatadram(unsigned int offset__, unsigned int index, vprop_t data[EDGE_PACK_SIZE], HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 712::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	master_retrieve_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v].prop = data_[2*v];
		data[v].mask = data_[2*v + 1];
	}
	return;
}
void dinsertmany_vdatadram(unsigned int offset__, unsigned int offsets[VALID_NUMPEs], unsigned int t, vprop_t datas[VALID_NUMPEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 711b::", offsets[0], globalparams_debug[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE]; // NEW
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	bool ens_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=ens_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offsets[n];
	}
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		ens_[n] = true;
	}
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data_[n][2*v] = datas[n][v].prop;
			data_[n][2*v+1] = datas[n][v].mask;
		}
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0);
	return;
}
void dretrievemany_vdatadram(unsigned int offset__, unsigned int offsets[VALID_NUMPEs], unsigned int t, vprop_t datas[VALID_NUMPEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 712::", offsets[0], wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE]; // NEW
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<VALID_NUMPEs; n++){
			datas[n][v].prop = data_[n][2*v];
			datas[n][v].mask = data_[n][2*v+1];
		}
	}
	return;
}

void dretrievemany_cfrontierdram_tmp(unsigned int base_offset__, unsigned int offsets[VALID_NUMPEs], unsigned int t, keyvalue_t datas[VALID_NUMPEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
	checkoutofbounds("acts_kernel::ERROR 714a::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<VALID_NUMPEs; n++){
		#pragma HLS UNROLL
			datas[n][v].key = data_[n][2*v];
			datas[n][v].value = data_[n][2*v+1];
		}
	}
	return; 
}

void dinsertmany_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int offsets[VALID_NUMPEs], unsigned int t, keyvalue_t datas[VALID_NUMPEs][EDGE_PACK_SIZE], bool ens[VALID_NUMPEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0){
	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	bool ens_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=ens_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offsets[n];
	}
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		ens_[n] = ens[n][0];
	}
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		// for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		// #pragma HLS UNROLL
			// data_[n][2*v] = datas[n][v].key;
			// data_[n][2*v+1] = datas[n][v].value;
		// }	
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
void insert_vdatabuffer(unsigned int v, unsigned int index, vprop_t data, MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 102::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif
	vdata_buffer[v][index >> POW_VALID_VDATA] = data; 
}
vprop_t retrieve_vdatabuffer(unsigned int v, unsigned int index, MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 103::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif
	return vdata_buffer[v][index >> POW_VALID_VDATA];
}
void insertvec_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 104::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		vdata_buffer[v][index >> POW_VALID_VDATA] = data[v];
	}
}
void retrievevec_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 105::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = vdata_buffer[v][index >> POW_VALID_VDATA];
	}
}
void retrieveandresetvec_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], MY_IFDEF_VDATABUFFER()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 105::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v].prop = vdata_buffer[v][index >> POW_VALID_VDATA].prop; 
		data[v].mask = INVALIDMASK;  
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
	return cfrontier_buffer_tmp[v][t];
}
void insertvec_cfrontierbuffer_tmp(unsigned int t, frontier_t data[EDGE_PACK_SIZE], MY_IFDEF_CFRONTIER_TMP()){
	#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 115::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		cfrontier_buffer_tmp[v][t] = data[v];
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

//////////////////////////////////////////////////////////////////
value_t process_func_pr(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata + edgew;
	return res;
}
value_t reduce_func_pr(value_t vtemp, value_t vdata, value_t res){
	#pragma HLS INLINE 
	value_t temp = vtemp + res;
	return temp;
}

value_t process_func_cf(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata;
	return res;
}
value_t reduce_func_cf(value_t vtemp, value_t vdata, value_t res){
	#pragma HLS INLINE 
	unsigned int lamda = 1; unsigned int ew=1;
	value_t temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
	return temp;
}

value_t process_func_hits(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata;
	return res;
}
value_t reduce_func_hits(value_t vtemp, value_t vdata, value_t res){
	#pragma HLS INLINE 
	value_t temp = vtemp + res;
	return temp;
}

value_t process_func_bfs(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = NAp;
	return res;
}
value_t reduce_func_bfs(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter){
	#pragma HLS INLINE 
	value_t temp; if(vtemp < GraphIter){ return vtemp; } else { return GraphIter; }
	return temp;
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

value_t process_funcG(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata + edgew;
	return res;
}
value_t reduce_funcG(value_t vtemp, value_t vdata, value_t res){
	#pragma HLS INLINE 
	if(res < vtemp){ return res; } else { return vtemp; }
}

value_t process_func(value_t udata, value_t edgew, unsigned int GraphAlgo){
	#pragma HLS INLINE 
	if(GraphAlgo == PAGERANK){
		return process_func_pr(udata, edgew);
	} else if(GraphAlgo == CF){
		return process_func_cf(udata, edgew);
	} else if(GraphAlgo == HITS){
		return process_func_hits(udata, edgew);
	} else if(GraphAlgo == SSSP){
		return process_func_sssp(udata, edgew);
	} else if(GraphAlgo == BFS){
		return process_func_bfs(udata, edgew);
	} else {
		return process_func_pr(udata, edgew);
	}
}
value_t reduce_func(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	#pragma HLS INLINE 
	if(GraphAlgo == PAGERANK){ 
		return reduce_func_pr(vtemp, vdata, res);
	} else if(GraphAlgo == CF){
		return reduce_func_cf(vtemp, vdata, res);
	} else if(GraphAlgo == HITS){
		return reduce_func_hits(vtemp, vdata, res);
	} else if(GraphAlgo == SSSP){
		return reduce_func_sssp(vtemp, vdata, res);
	} else if(GraphAlgo == BFS){
		return reduce_func_bfs(vtemp, vdata, res, GraphIter);
	} else {
		return reduce_func_pr(vtemp, vdata, res);
	}
}

//////////////////////////////////////////////////////////////////
void load_edges(unsigned int inst, unsigned int offset_vptrbuffer, unsigned int baseoffset_csrdram, unsigned int vptrbuffer___size, MY_IFDEF_VPTRBUFFER(), unsigned int * edges_buffer___size, MY_IFDEF_EDGESBUFFER(), HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB){
	if(offset_vptrbuffer < vptrbuffer___size){ // NEW
		vtr_t vptr_data = retrieve_vptrbuffer(offset_vptrbuffer, vptr_buffer); 
		unsigned int vid = vptr_data.vid;
		unsigned int edges_size = vptr_data.end - vptr_data.begin; 
		unsigned int edgelist_size = (edges_size / EDGE_PACK_SIZE) + 2; if((edges_size == 0) || (vptr_data.end < vptr_data.begin)){ edgelist_size = 0; }
		unsigned int uprop = vptr_data.prop;
		
		#ifdef ___FORCE_SUCCESS___
		if(*edges_buffer___size + edgelist_size >= EDGE_BUFFER_SIZE){ edgelist_size = EDGE_BUFFER_SIZE - *edges_buffer___size; }
		#endif
		
		VC_READ_EDGES_LOOP1B: for(unsigned int t=0; t<edgelist_size; t++){
		#pragma HLS PIPELINE II=1
			edge3_vec_dt edges = dretrieve_csredges(baseoffset_csrdram, (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA, HBM_channelB, inst);
			edge3_type edge_list[EDGE_PACK_SIZE];
			MY_LOOP711: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
			#pragma HLS UNROLL
				if(edges.data[v].srcvid == vid && edges.data[v].srcvid != INVALIDDATA){ edge_list[v] = edges.data[v]; edge_list[v].srcvid = uprop; } else { edge_list[v].srcvid = INVALIDDATA; edge_list[v].dstvid = INVALIDDATA; }
			}
			MY_LOOP712: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
			#pragma HLS UNROLL
				#ifndef FPGA_IMPL
				checkoutofbounds("acts_kernel::ERROR 812::", *edges_buffer___size, EDGE_BUFFER_SIZE, vptrbuffer___size, inst, edgelist_size);
				#endif 
				insert_edgesbuffer(v, *edges_buffer___size, edge_list[v], edges_buffer);
			}
			*edges_buffer___size += 1;	
			#ifndef FPGA_IMPL
			if(inst==0){ update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 1); }
			#endif 
		}
	}
	return;
}

void transport_frontier(unsigned int inst, unsigned int p_v, unsigned int baseoffset_nfdram, unsigned int * cfrontier_dram___size, unsigned int * nfrontier_dram___size, offset_t * upartition_vertices, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, unsigned int * totalactvvs2){
	frontier_t actvv[EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = actvv complete
	unsigned int totalactvvs2_tmp[EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = totalactvvs2_tmp complete
	unsigned int data[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data complete
				
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ totalactvvs2_tmp[v] = 0; }
	TRANSPORT_FRONTIER_PROPERTIES_LOOP1: for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
		unsigned int p_actvv_ = p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION + p_actvv;
		#ifndef FPGA_IMPL
		checkoutofbounds("acts_kernel::ERROR 225::", p_actvv_, MAX_NUM_UPARTITIONS, NAp, NAp, NAp);
		#endif 
		unsigned int offset_n = baseoffset_nfdram + (p_actvv_ * MAX_APPLYPARTITION_VECSIZE);
		unsigned int offset_c = p_actvv_ * MAX_UPARTITION_VECSIZE;
		unsigned int st = cfrontier_dram___size[p_actvv_];
		TRANSPORT_FRONTIER_PROPERTIES_LOOP1B: for(unsigned int t=0; t<nfrontier_dram___size[p_actvv_]; t++){
		#pragma HLS PIPELINE II=1
			#ifdef FPGA_IMPL 
			data[0] = HBM_channelA[offset_n + t].range(31, 0);
			data[1] = HBM_channelA[offset_n + t].range(63, 32);
			data[2] = HBM_channelA[offset_n + t].range(95, 64);
			data[3] = HBM_channelA[offset_n + t].range(127, 96);
			data[4] = HBM_channelA[offset_n + t].range(159, 128);
			data[5] = HBM_channelA[offset_n + t].range(191, 160);
			data[6] = HBM_channelA[offset_n + t].range(223, 192);
			data[7] = HBM_channelA[offset_n + t].range(255, 224);
			data[8] = HBM_channelA[offset_n + t].range(287, 256);
			data[9] = HBM_channelA[offset_n + t].range(319, 288);
			data[10] = HBM_channelA[offset_n + t].range(351, 320);
			data[11] = HBM_channelA[offset_n + t].range(383, 352);
			data[12] = HBM_channelA[offset_n + t].range(415, 384);
			data[13] = HBM_channelA[offset_n + t].range(447, 416);
			data[14] = HBM_channelA[offset_n + t].range(479, 448);
			data[15] = HBM_channelA[offset_n + t].range(511, 480);
			data[16] = HBM_channelB[offset_n + t].range(31, 0);
			data[17] = HBM_channelB[offset_n + t].range(63, 32);
			data[18] = HBM_channelB[offset_n + t].range(95, 64);
			data[19] = HBM_channelB[offset_n + t].range(127, 96);
			data[20] = HBM_channelB[offset_n + t].range(159, 128);
			data[21] = HBM_channelB[offset_n + t].range(191, 160);
			data[22] = HBM_channelB[offset_n + t].range(223, 192);
			data[23] = HBM_channelB[offset_n + t].range(255, 224);
			data[24] = HBM_channelB[offset_n + t].range(287, 256);
			data[25] = HBM_channelB[offset_n + t].range(319, 288);
			data[26] = HBM_channelB[offset_n + t].range(351, 320);
			data[27] = HBM_channelB[offset_n + t].range(383, 352);
			data[28] = HBM_channelB[offset_n + t].range(415, 384);
			data[29] = HBM_channelB[offset_n + t].range(447, 416);
			data[30] = HBM_channelB[offset_n + t].range(479, 448);
			data[31] = HBM_channelB[offset_n + t].range(511, 480);
			
			HBM_centerA[offset_c + st + t].range(31, 0) = data[0];
			HBM_centerA[offset_c + st + t].range(63, 32) = data[1];
			HBM_centerA[offset_c + st + t].range(95, 64) = data[2];
			HBM_centerA[offset_c + st + t].range(127, 96) = data[3];
			HBM_centerA[offset_c + st + t].range(159, 128) = data[4];
			HBM_centerA[offset_c + st + t].range(191, 160) = data[5];
			HBM_centerA[offset_c + st + t].range(223, 192) = data[6];
			HBM_centerA[offset_c + st + t].range(255, 224) = data[7];
			HBM_centerA[offset_c + st + t].range(287, 256) = data[8];
			HBM_centerA[offset_c + st + t].range(319, 288) = data[9];
			HBM_centerA[offset_c + st + t].range(351, 320) = data[10];
			HBM_centerA[offset_c + st + t].range(383, 352) = data[11];
			HBM_centerA[offset_c + st + t].range(415, 384) = data[12];
			HBM_centerA[offset_c + st + t].range(447, 416) = data[13];
			HBM_centerA[offset_c + st + t].range(479, 448) = data[14];
			HBM_centerA[offset_c + st + t].range(511, 480) = data[15];
			HBM_centerB[offset_c + st + t].range(31, 0) = data[16];
			HBM_centerB[offset_c + st + t].range(63, 32) = data[17];
			HBM_centerB[offset_c + st + t].range(95, 64) = data[18];
			HBM_centerB[offset_c + st + t].range(127, 96) = data[19];
			HBM_centerB[offset_c + st + t].range(159, 128) = data[20];
			HBM_centerB[offset_c + st + t].range(191, 160) = data[21];
			HBM_centerB[offset_c + st + t].range(223, 192) = data[22];
			HBM_centerB[offset_c + st + t].range(255, 224) = data[23];
			HBM_centerB[offset_c + st + t].range(287, 256) = data[24];
			HBM_centerB[offset_c + st + t].range(319, 288) = data[25];
			HBM_centerB[offset_c + st + t].range(351, 320) = data[26];
			HBM_centerB[offset_c + st + t].range(383, 352) = data[27];
			HBM_centerB[offset_c + st + t].range(415, 384) = data[28];
			HBM_centerB[offset_c + st + t].range(447, 416) = data[29];
			HBM_centerB[offset_c + st + t].range(479, 448) = data[30];
			HBM_centerB[offset_c + st + t].range(511, 480) = data[31];
			#else 
			data[0] = HBM_channelA[offset_n + t].data[0];
			data[1] = HBM_channelA[offset_n + t].data[1];
			data[2] = HBM_channelA[offset_n + t].data[2];
			data[3] = HBM_channelA[offset_n + t].data[3];
			data[4] = HBM_channelA[offset_n + t].data[4];
			data[5] = HBM_channelA[offset_n + t].data[5];
			data[6] = HBM_channelA[offset_n + t].data[6];
			data[7] = HBM_channelA[offset_n + t].data[7];
			data[8] = HBM_channelA[offset_n + t].data[8];
			data[9] = HBM_channelA[offset_n + t].data[9];
			data[10] = HBM_channelA[offset_n + t].data[10];
			data[11] = HBM_channelA[offset_n + t].data[11];
			data[12] = HBM_channelA[offset_n + t].data[12];
			data[13] = HBM_channelA[offset_n + t].data[13];
			data[14] = HBM_channelA[offset_n + t].data[14];
			data[15] = HBM_channelA[offset_n + t].data[15];
			data[16] = HBM_channelB[offset_n + t].data[0];
			data[17] = HBM_channelB[offset_n + t].data[1];
			data[18] = HBM_channelB[offset_n + t].data[2];
			data[19] = HBM_channelB[offset_n + t].data[3];
			data[20] = HBM_channelB[offset_n + t].data[4];
			data[21] = HBM_channelB[offset_n + t].data[5];
			data[22] = HBM_channelB[offset_n + t].data[6];
			data[23] = HBM_channelB[offset_n + t].data[7];
			data[24] = HBM_channelB[offset_n + t].data[8];
			data[25] = HBM_channelB[offset_n + t].data[9];
			data[26] = HBM_channelB[offset_n + t].data[10];
			data[27] = HBM_channelB[offset_n + t].data[11];
			data[28] = HBM_channelB[offset_n + t].data[12];
			data[29] = HBM_channelB[offset_n + t].data[13];
			data[30] = HBM_channelB[offset_n + t].data[14];
			data[31] = HBM_channelB[offset_n + t].data[15];
			
			HBM_centerA[offset_c + st + t].data[0] = data[0];
			HBM_centerA[offset_c + st + t].data[1] = data[1];
			HBM_centerA[offset_c + st + t].data[2] = data[2];
			HBM_centerA[offset_c + st + t].data[3] = data[3];
			HBM_centerA[offset_c + st + t].data[4] = data[4];
			HBM_centerA[offset_c + st + t].data[5] = data[5];
			HBM_centerA[offset_c + st + t].data[6] = data[6];
			HBM_centerA[offset_c + st + t].data[7] = data[7];
			HBM_centerA[offset_c + st + t].data[8] = data[8];
			HBM_centerA[offset_c + st + t].data[9] = data[9];
			HBM_centerA[offset_c + st + t].data[10] = data[10];
			HBM_centerA[offset_c + st + t].data[11] = data[11];
			HBM_centerA[offset_c + st + t].data[12] = data[12];
			HBM_centerA[offset_c + st + t].data[13] = data[13];
			HBM_centerA[offset_c + st + t].data[14] = data[14];
			HBM_centerA[offset_c + st + t].data[15] = data[15];
			HBM_centerB[offset_c + st + t].data[0] = data[16];
			HBM_centerB[offset_c + st + t].data[1] = data[17];
			HBM_centerB[offset_c + st + t].data[2] = data[18];
			HBM_centerB[offset_c + st + t].data[3] = data[19];
			HBM_centerB[offset_c + st + t].data[4] = data[20];
			HBM_centerB[offset_c + st + t].data[5] = data[21];
			HBM_centerB[offset_c + st + t].data[6] = data[22];
			HBM_centerB[offset_c + st + t].data[7] = data[23];
			HBM_centerB[offset_c + st + t].data[8] = data[24];
			HBM_centerB[offset_c + st + t].data[9] = data[25];
			HBM_centerB[offset_c + st + t].data[10] = data[26];
			HBM_centerB[offset_c + st + t].data[11] = data[27];
			HBM_centerB[offset_c + st + t].data[12] = data[28];
			HBM_centerB[offset_c + st + t].data[13] = data[29];
			HBM_centerB[offset_c + st + t].data[14] = data[30];
			HBM_centerB[offset_c + st + t].data[15] = data[31];
			#endif 
		
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
			#pragma HLS UNROLL
				if(actvv[v].key != INVALIDDATA){ 
					totalactvvs2_tmp[v] += 1;
				}
			}
			#ifndef FPGA_IMPL
			update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___EXCHANGEFRONTIERINFOS___, 1);
			#endif 
				
		}
		upartition_vertices[p_actvv_].count += nfrontier_dram___size[p_actvv_]; 
		cfrontier_dram___size[p_actvv_] += nfrontier_dram___size[p_actvv_];
	}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ *totalactvvs2 += totalactvvs2_tmp[v]; }
}	

//////////////////////////////////////////////////////////////////
extern "C" {
// void top_function(HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB){
MY_IFDEF_TOPLEVELFUNC(){
#pragma HLS INTERFACE m_axi port = HBM_channelA0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = HBM_channelB0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = HBM_centerA offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = HBM_centerB offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = HBM_channelA0 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB0 bundle = control
	
#pragma HLS INTERFACE s_axilite port = HBM_centerA bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_centerB bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control
	
	#ifdef _DEBUGMODE_KERNELPRINTS4
	cout<<"acts_kernel::run:: acts started "<<endl;
	#endif 

	// #ifdef RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
	// declaration of BRAM variables
	#ifdef FPGA_IMPL
	keyvalue_t nfrontier_buffer[VALID_NUMPEs][EDGE_PACK_SIZE][MAX_ACTVV_VECSIZE]; 
	#pragma HLS ARRAY_PARTITION variable=nfrontier_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=nfrontier_buffer complete dim=2
	keyvalue_t cfrontier_buffer_tmp[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]; 
	#pragma HLS resource variable=cfrontier_buffer_tmp core=XPM_MEMORY uram
	#pragma HLS ARRAY_PARTITION variable=cfrontier_buffer_tmp complete dim=1
	keyvalue_t cfrontier_buffer[VALID_NUMPEs][MAXVALID_APPLYPARTITION_VECSIZE];
	#pragma HLS resource variable=cfrontier_buffer core=XPM_MEMORY uram
	#pragma HLS ARRAY_PARTITION variable=cfrontier_buffer complete dim=1
	unsigned int statsbuffer_maskbased_tmp[VALID_NUMPEs][EDGE_PACK_SIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_maskbased_tmp complete dim=1
    #pragma HLS ARRAY_PARTITION variable=statsbuffer_maskbased_tmp complete dim=2
	unsigned int statsbuffer_maskbased[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_maskbased complete dim=1
    #pragma HLS ARRAY_PARTITION variable=statsbuffer_maskbased complete dim=2
	unsigned int statsbuffer_idbased[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable=statsbuffer_idbased complete dim=1 
    #pragma HLS ARRAY_PARTITION variable=statsbuffer_idbased complete dim=2
	vtr_t vptr_buffer[VALID_NUMPEs][VPTR_BUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vptr_buffer complete dim=1
	edge3_type edges_buffer[VALID_NUMPEs][EDGE_PACK_SIZE][EDGE_BUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edges_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=edges_buffer complete dim=2
	keyvalue_t updates_buffer2[VALID_NUMPEs][UPDATES_BUFFER_PACK_SIZE][UPDATES_BUFFER_SIZE]; // EDGE_PACK_SIZE
	#pragma HLS ARRAY_PARTITION variable=updates_buffer2 complete dim=1
    #pragma HLS ARRAY_PARTITION variable=updates_buffer2 complete dim=2
	vprop_t vdata_buffer[VALID_NUMPEs][EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE];
	#pragma HLS resource variable=vdata_buffer core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=vdata_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=vdata_buffer complete dim=2 
	#pragma HLS aggregate variable=vdata_buffer 
	#else 
	keyvalue_t * nfrontier_buffer[VALID_NUMPEs][EDGE_PACK_SIZE]; 
	keyvalue_t * cfrontier_buffer_tmp[EDGE_PACK_SIZE]; 
	keyvalue_t * cfrontier_buffer[VALID_NUMPEs]; 
	unsigned int * statsbuffer_maskbased_tmp[VALID_NUMPEs][EDGE_PACK_SIZE]; 
	unsigned int * statsbuffer_maskbased[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	unsigned int * statsbuffer_idbased[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	vtr_t * vptr_buffer[VALID_NUMPEs]; 
	edge3_type * edges_buffer[VALID_NUMPEs][EDGE_PACK_SIZE];
	keyvalue_t * updates_buffer2[VALID_NUMPEs][UPDATES_BUFFER_PACK_SIZE]; 
	vprop_t * vdata_buffer[VALID_NUMPEs][EDGE_PACK_SIZE];
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer[i][v] = new keyvalue_t[MAX_ACTVV_VECSIZE]; }}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_buffer_tmp[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ cfrontier_buffer[i] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ statsbuffer_maskbased_tmp[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ statsbuffer_maskbased[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ statsbuffer_idbased[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ vptr_buffer[i] = new vtr_t[VPTR_BUFFER_SIZE]; }
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ edges_buffer[i][v] = new edge3_type[EDGE_BUFFER_SIZE]; }}
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<UPDATES_BUFFER_PACK_SIZE; v++){ updates_buffer2[i][v] = new keyvalue_t[UPDATES_BUFFER_SIZE]; }}
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ vdata_buffer[i][v] = new vprop_t[MAX_APPLYPARTITION_VECSIZE]; }}
	#endif 
	
	#ifdef FPGA_IMPL	
	unsigned int cfrontier_dram___size[MAX_NUM_UPARTITIONS]; 
	unsigned int nfrontier_dram___size[VALID_NUMPEs][MAX_NUM_UPARTITIONS];
	#pragma HLS ARRAY_PARTITION variable = nfrontier_dram___size complete dim=1
	unsigned int nfrontier_buffer___size[VALID_NUMPEs][EDGE_PACK_SIZE][NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; 
	#pragma HLS ARRAY_PARTITION variable = nfrontier_buffer___size complete dim=1
	#pragma HLS ARRAY_PARTITION variable = nfrontier_buffer___size complete dim=2
	unsigned int updates_tmpbuffer___size[VALID_NUMPEs][VALID_NUMPEs]; 
	#pragma HLS ARRAY_PARTITION variable = updates_tmpbuffer___size complete dim=1
	unsigned int updates_buffer___size[VALID_NUMPEs][EDGE_PACK_SIZE][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = updates_buffer___size complete dim=1
	#pragma HLS ARRAY_PARTITION variable = updates_buffer___size complete dim=2
	unsigned int stats_buffer___size[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = stats_buffer___size complete dim=1
	unsigned int statsbuffer_idbased___size[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = statsbuffer_idbased___size complete dim=1
	offset_t upartition_vertices[MAX_NUM_UPARTITIONS]; 
	offset_t vpartition_vertices[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = vpartition_vertices complete dim=1
	unsigned int actpackupdates_dram___size[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = actpackupdates_dram___size complete dim=1
	unsigned int vptrbuffer___size[VALID_NUMPEs]; 
	#pragma HLS ARRAY_PARTITION variable = vptrbuffer___size complete // dim=1
	unsigned int edges_buffer___size[VALID_NUMPEs]; 
	#pragma HLS ARRAY_PARTITION variable = edges_buffer___size complete // dim=1
	unsigned int cfrontier_bufferREAL___size[VALID_NUMPEs]; 
	#pragma HLS ARRAY_PARTITION variable = cfrontier_bufferREAL___size complete
	#pragma HLS ARRAY_MAP variable=nfrontier_dram___size instance=array1 horizontal
	#pragma HLS ARRAY_MAP variable=stats_buffer___size instance=array1 horizontal
	#pragma HLS ARRAY_MAP variable=statsbuffer_idbased___size instance=array1 horizontal
	#pragma HLS ARRAY_MAP variable=vpartition_vertices instance=array1 horizontal
	#pragma HLS ARRAY_MAP variable=actpackupdates_dram___size instance=array1 horizontal
	#pragma HLS ARRAY_MAP variable=vptrbuffer___size instance=array2 horizontal
	#pragma HLS ARRAY_MAP variable=edges_buffer___size instance=array2 horizontal
	#else 
	unsigned int * cfrontier_dram___size = new unsigned int[MAX_NUM_UPARTITIONS]; 
	unsigned int * nfrontier_dram___size[VALID_NUMPEs]; for(unsigned int i=0; i<VALID_NUMPEs; i++){ nfrontier_dram___size[i] = new unsigned int[MAX_NUM_UPARTITIONS]; }
	unsigned int * nfrontier_buffer___size[VALID_NUMPEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer___size[i][v] = new unsigned int[NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; }}
	unsigned int updates_tmpbuffer___size[VALID_NUMPEs][VALID_NUMPEs]; 
	unsigned int * updates_buffer___size[VALID_NUMPEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[i][v] = new unsigned int[MAX_NUM_APPLYPARTITIONS]; }}
	unsigned int stats_buffer___size[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	unsigned int statsbuffer_idbased___size[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	offset_t * upartition_vertices = new offset_t[MAX_NUM_UPARTITIONS]; 
	offset_t * vpartition_vertices[VALID_NUMPEs]; for(unsigned int i=0; i<VALID_NUMPEs; i++){ vpartition_vertices[i] = new offset_t[MAX_NUM_APPLYPARTITIONS];  }
	unsigned int actpackupdates_dram___size[VALID_NUMPEs][MAX_NUM_APPLYPARTITIONS]; 
	unsigned int vptrbuffer___size[VALID_NUMPEs]; 
	unsigned int edges_buffer___size[VALID_NUMPEs]; 
	unsigned int cfrontier_bufferREAL___size[VALID_NUMPEs]; 
	unsigned int * hybrid_map[MAXNUMGRAPHITERATIONS]; for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ hybrid_map[i] = new unsigned int[MAX_NUM_UPARTITIONS]; }
	#endif 
	
	edge3_type edges[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = edges complete dim=0
	unsigned int res[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = res complete dim=0
	unsigned int vtemp_in[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vtemp_in complete dim=0
	unsigned int vtemp_out[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vtemp_out complete dim=0
	vprop_t uprop[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = uprop complete dim=0
	keyvalue_t update_in[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = update_in complete dim=0
	keyvalue_t update_out[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = update_out complete dim=0
	bool ens[VALID_NUMPEs][EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = ens complete dim=0
	vprop_t data[EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = data complete
	vprop_t datas[VALID_NUMPEs][EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = datas complete dim=0
	keyvalue_t kvdata[EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = kvdata complete
	keyvalue_t kvdatas[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = kvdatas complete dim=0
	unsigned int offsets0[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable = offsets0 complete
	unsigned int offsets[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable = offsets complete
	unsigned int offsets2[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable = offsets2 complete
	keyvalue_t actvvs[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = actvvs complete dim=0
	frontier_t actvv[EDGE_PACK_SIZE];
	unsigned int vid_first0[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable = vid_first0 complete
	unsigned int vid_first1[VALID_NUMPEs]; 
	#pragma HLS ARRAY_PARTITION variable = vid_first1 complete
	unsigned int globalparams[GLOBALBUFFER_SIZE];
	// #pragma HLS ARRAY_PARTITION variable = globalparams complete
	unsigned int updatesptrs[MAX_NUM_LLPSETS];
	unsigned int limits[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable = limits complete
	unsigned int max_limits[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable = max_limits complete
	uint512_vec_dt updates_vecs[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable = updates_vecs complete
	uint512_vec_dt edges_vecs[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable = edges_vecs complete
	unsigned int stats_count[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=stats_count complete
	unsigned int stats_counts[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=stats_counts complete dim=0
	unsigned int cummtv2[VALID_NUMPEs]; 
	#pragma HLS ARRAY_PARTITION variable = cummtv2 complete
	uint1_dt masks[VALID_NUMPEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=masks complete dim=0
	edge3_vec_dt edge3_vecs[VALID_NUMPEs]; 
	#pragma HLS ARRAY_PARTITION variable=edge3_vecs complete
	map_t maps[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=maps complete	
	#ifdef FPGA_IMPL
	unsigned int _NUMCLOCKCYCLES_[2][16]; 
	#endif 
	
	unsigned int maxGraphIter = 0;
	bool run_vertex_centric = true;
	bool enable___vertexcentric = true;
	#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
	bool enable___collectactivedstvids = true;
	#else 
	bool enable___collectactivedstvids = false; // false
	#endif 
	unsigned int threshold___activedstvids = 16; // 0xFFFFFFFF;
	unsigned int max_limit = 0, max_limit2 = 0; 
	unsigned int CONST1 = NUM_PEs * EDGE_PACK_SIZE; 
	unsigned int CONST2 = MAX_ACTVV_VECSIZE * NUM_PEs * EDGE_PACK_SIZE;
	for(unsigned int k=0; k<2; k++){ for(unsigned int t=0; t<16; t++){ _NUMCLOCKCYCLES_[k][t] = 0; } }
	unsigned int total_cycles = 0;
	
	// load global parameters
	unsigned int first__bool__ = 1;
	if(first__bool__ == 1){ 
	MY_LOOP209: for(unsigned int t=0; t<GLOBALBUFFER_SIZE; t++){
	#pragma HLS PIPELINE II=1
		globalparams[t] = dretrieve_globalparams(0, t, HBM_channelA0, HBM_channelB0, 0);
	}
	}
	unsigned int __NUM_UPARTITIONS = globalparams[GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS];
	unsigned int __NUM_APPLYPARTITIONS = globalparams[GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS]; 
	#ifndef FPGA_IMPL
	globalparams_debug = (unsigned int *)&globalparams[0];
	#endif
	
	unsigned int write_data[HBM_CHANNEL_PACK_SIZE];
	for(unsigned int t=0; t<HBM_CHANNEL_PACK_SIZE; t++){ write_data[t] = 0; }
	MY_LOOP2091: for(unsigned int t=0; t<GLOBALBUFFER_SIZE; t++){
		for(unsigned int t1=0; t1<HBM_CHANNEL_PACK_SIZE; t1++){ write_data[t1] = 0; }
		write_data[1] = globalparams[t];
		master_insert_vec(t, write_data, HBM_channelA0, HBM_channelB0, 0);
	}
	
	int ___ENABLE___RESETBUFFERSATSTART___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___RESETBUFFERSATSTART];
	int ___ENABLE___PROCESSEDGES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___PROCESSEDGES];
	int ___ENABLE___READ_FRONTIER_PROPERTIES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___READ_FRONTIER_PROPERTIES];
	int ___ENABLE___VCPROCESSEDGES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___VCPROCESSEDGES]; 
	int ___ENABLE___ECPROCESSEDGES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___ECPROCESSEDGES]; 
	int ___ENABLE___SAVEVCUPDATES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___SAVEVCUPDATES]; 
	int ___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___COLLECTACTIVEDSTVIDS];
	int ___ENABLE___APPLYUPDATESMODULE___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___APPLYUPDATESMODULE]; 
		int ___ENABLE___READ_DEST_PROPERTIES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___READ_DEST_PROPERTIES];
		int ___ENABLE___APPLYUPDATES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___APPLYUPDATES]; 
		int ___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES]; 
		int ___ENABLE___SAVE_DEST_PROPERTIES___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___SAVE_DEST_PROPERTIES]; 
	int ___ENABLE___EXCHANGEFRONTIERINFOS___BOOL___ = globalparams[GLOBALPARAMSCODE___ENABLE___EXCHANGEFRONTIERINFOS]; 
	
	#ifdef ___ENABLE___RESETBUFFERSATSTART___
	if(___ENABLE___RESETBUFFERSATSTART___BOOL___ == 1){
	MY_LOOP200: for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[p] = 0; }
	MY_LOOP201: for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ nfrontier_dram___size[i][p] = 0; }}
	MY_LOOP202: for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p++){ nfrontier_buffer___size[i][v][p] = 0; }}}
	MY_LOOP203: for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_APPLYPARTITIONS; p++){ updates_buffer___size[i][v][p] = 0; }}}
	MY_LOOP204: for(unsigned int p=0; p<__NUM_UPARTITIONS; p++){ upartition_vertices[p].offset = 0; upartition_vertices[p].size = 0; upartition_vertices[p].count = 0; } upartition_vertices[0].count = 1;
	MY_LOOP205: for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ vpartition_vertices[i][p].offset = 0; vpartition_vertices[i][p].size = 0; vpartition_vertices[i][p].count = 0; }}
	MY_LOOP206: for(unsigned int i=0; i<VALID_NUMPEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ actpackupdates_dram___size[i][p] = 0; }}
	MY_LOOP207: for(unsigned int i=0; i<VALID_NUMPEs; i++){ vptrbuffer___size[i] = 0; }
	MY_LOOP208: for(unsigned int i=0; i<VALID_NUMPEs; i++){ edges_buffer___size[i] = 0; }
	#ifndef FPGA_IMPL
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ for(unsigned int t=0; t<MAX_NUM_UPARTITIONS; t++){ hybrid_map[i][t] = NAp; }}
	#endif 
	}
	#endif 
	
	bool all_vertices_active_in_all_iterations = false; 
	if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == PAGERANK || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CF || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == HITS || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == SPMV){ all_vertices_active_in_all_iterations = true; }
	if(all_vertices_active_in_all_iterations == true){ cfrontier_dram___size[0] = MAXVALID_APPLYPARTITION_VECSIZE; } else { cfrontier_dram___size[0] = 1; }// activate root vid
	unsigned int threshold___activefrontiers = globalparams[GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEFRONTIERSFORCONTROLSWITCH]; 
	
	#ifdef _DEBUGMODE_KERNELPRINTS4
	cout<<"acts_kernel::run:: NUM_VERTICES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES]<<", NUM_EDGES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES]<<", MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<", __NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<", MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<", __NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<endl;			
	cout<<"---------------------------------------------------------------------- ACTS running "<<VALID_NUMPEs<<" instances ----------------------------------------------------------------------"<<endl;
	if(run_vertex_centric == true){ cout<<"### Vertex-Centric GAS iteration: 0 [1 active vertices]"<<endl; } else { cout<<"### Edge-Centric GAS iteration: 0 [1 active vertices]"<<endl; }
	#endif 
	
	// reset ubuffer 
	if(___ENABLE___PROCESSEDGES___BOOL___ == 1){ // FIXME_HARDWARE.
	vprop_t prop; prop.prop = 0; prop.mask = INVALIDMASK; 
	MY_LOOP195: for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ 
	#pragma HLS PIPELINE II=1
		MY_LOOP194: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
		#pragma HLS UNROLL
			MY_LOOP196: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
			#pragma HLS UNROLL
				insert_vdatabuffer(v, t, prop, vdata_buffer[inst]); 
			}
		}
		update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_FRONTIER_PROPERTIES___, 1);
	}
	}
	
	// load updates ptrs
	if(___ENABLE___SAVEVCUPDATES___BOOL___ == 1){ // FIXME_HARDWARE.
	MY_LOOP193: for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
	#pragma HLS PIPELINE II=1
		updatesptrs[t] = dretrieve_updatesptrs(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], t, HBM_channelA0, HBM_channelB0, 0);
		#ifdef _DEBUGMODE_KERNELPRINTS4
		cout<<"--- updatesptrs["<<t<<"]: "<<updatesptrs[t]<<endl;
		#endif 
	}
	}
	
	// #ifdef ___REFACTORING_FOR_VHLS_DEBUGGING___
	// for(unsigned int u=0; u<MAX_NUM_UPARTITIONS; u++){ cfrontier_dram___size[u] = globalparams[u]; } // FIXME. removeme
	// for(unsigned int u=0; u<MAX_NUM_UPARTITIONS; u++){ upartition_vertices[u].count = globalparams[u+1]; } // FIXME. removeme
	// #endif 
	
	// #ifdef RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
	// run acts 
	RUN_ACTS_LOOP: for(unsigned int GraphIter=0; GraphIter<globalparams[GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS]; GraphIter++){
		unsigned int MASK_CODE = 1 + GraphIter;
		unsigned int MASK_CODE_PE = 1 + GraphIter;
		unsigned int MASK_CODE_AU = 4094 + 1 + GraphIter;
		
		// clear counters [done]
		CLEAR_COUNTERS_LOOP1: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
		#pragma HLS UNROLL
			CLEAR_COUNTERS_LOOP1B: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ 
				actpackupdates_dram___size[inst][p_v] = 0;
				stats_buffer___size[inst][p_v] = 0; statsbuffer_idbased___size[inst][p_v] = 0;
				vpartition_vertices[inst][p_v].offset = 0; vpartition_vertices[inst][p_v].size = 0; vpartition_vertices[inst][p_v].count = 0;
				MY_LOOP191: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[inst][v][p_v] = 0; }
			}	
			CLEAR_COUNTERS_LOOP1C: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ 
			#pragma HLS UNROLL
				MY_LOOP192: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ insert_statsbuffer(t, p_v, 0, statsbuffer_maskbased[inst]); }
			}
		}
		
		// process-edges and partition-updates
		PROCESS_EDGES_MODULE_LOOP1: for(unsigned int n=0; n<2; n++){
			// process-edges and partition-updates
			#ifdef ___ENABLE___PROCESSEDGES___
			if(___ENABLE___PROCESSEDGES___BOOL___ == 1){
			PROCESS_EDGES_MODULE_LOOP1B: for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){
				MASK_CODE_PE = ((1 + GraphIter) * MAX_NUM_UPARTITIONS) + p_u;
				bool ___use_vertex_centric___ = false; 
				bool en = false;
				if((upartition_vertices[p_u].count < threshold___activefrontiers && enable___vertexcentric == true) && (all_vertices_active_in_all_iterations == false)){ ___use_vertex_centric___ = true; } else { ___use_vertex_centric___ = false; }
				
				#ifdef ___ENABLE___READ_FRONTIER_PROPERTIES___
				if(___ENABLE___READ_FRONTIER_PROPERTIES___BOOL___ == 1){
				// broadcast active frontiers [done]
				unsigned int uoffset_vecsz = p_u * MAX_UPARTITION_VECSIZE; 
				MY_LOOP178: for(unsigned int n=0; n<VALID_NUMPEs; n++){ offsets[n] = 0; }
				#ifdef _DEBUGMODE_CHECKS3
				checkoutofbounds("acts_kernel::ERROR 12073::", cfrontier_dram___size[p_u], MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
				#endif 
				
				unsigned int data[HBM_CHANNEL_PACK_SIZE];
				#pragma HLS ARRAY_PARTITION variable=data complete
				unsigned int offset_c = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP];
				MY_LOOP179: for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){
				#pragma HLS PIPELINE II=1
					#ifdef FPGA_IMPL 
					data[0] = HBM_centerA[uoffset_vecsz + t].range(31, 0);
					data[1] = HBM_centerA[uoffset_vecsz + t].range(63, 32);
					data[2] = HBM_centerA[uoffset_vecsz + t].range(95, 64);
					data[3] = HBM_centerA[uoffset_vecsz + t].range(127, 96);
					data[4] = HBM_centerA[uoffset_vecsz + t].range(159, 128);
					data[5] = HBM_centerA[uoffset_vecsz + t].range(191, 160);
					data[6] = HBM_centerA[uoffset_vecsz + t].range(223, 192);
					data[7] = HBM_centerA[uoffset_vecsz + t].range(255, 224);
					data[8] = HBM_centerA[uoffset_vecsz + t].range(287, 256);
					data[9] = HBM_centerA[uoffset_vecsz + t].range(319, 288);
					data[10] = HBM_centerA[uoffset_vecsz + t].range(351, 320);
					data[11] = HBM_centerA[uoffset_vecsz + t].range(383, 352);
					data[12] = HBM_centerA[uoffset_vecsz + t].range(415, 384);
					data[13] = HBM_centerA[uoffset_vecsz + t].range(447, 416);
					data[14] = HBM_centerA[uoffset_vecsz + t].range(479, 448);
					data[15] = HBM_centerA[uoffset_vecsz + t].range(511, 480);
					data[16] = HBM_centerB[uoffset_vecsz + t].range(31, 0);
					data[17] = HBM_centerB[uoffset_vecsz + t].range(63, 32);
					data[18] = HBM_centerB[uoffset_vecsz + t].range(95, 64);
					data[19] = HBM_centerB[uoffset_vecsz + t].range(127, 96);
					data[20] = HBM_centerB[uoffset_vecsz + t].range(159, 128);
					data[21] = HBM_centerB[uoffset_vecsz + t].range(191, 160);
					data[22] = HBM_centerB[uoffset_vecsz + t].range(223, 192);
					data[23] = HBM_centerB[uoffset_vecsz + t].range(255, 224);
					data[24] = HBM_centerB[uoffset_vecsz + t].range(287, 256);
					data[25] = HBM_centerB[uoffset_vecsz + t].range(319, 288);
					data[26] = HBM_centerB[uoffset_vecsz + t].range(351, 320);
					data[27] = HBM_centerB[uoffset_vecsz + t].range(383, 352);
					data[28] = HBM_centerB[uoffset_vecsz + t].range(415, 384);
					data[29] = HBM_centerB[uoffset_vecsz + t].range(447, 416);
					data[30] = HBM_centerB[uoffset_vecsz + t].range(479, 448);
					data[31] = HBM_centerB[uoffset_vecsz + t].range(511, 480);
					
					HBM_channelA0[offset_c + t].range(31, 0) = data[0];
					HBM_channelA0[offset_c + t].range(63, 32) = data[1];
					HBM_channelA0[offset_c + t].range(95, 64) = data[2];
					HBM_channelA0[offset_c + t].range(127, 96) = data[3];
					HBM_channelA0[offset_c + t].range(159, 128) = data[4];
					HBM_channelA0[offset_c + t].range(191, 160) = data[5];
					HBM_channelA0[offset_c + t].range(223, 192) = data[6];
					HBM_channelA0[offset_c + t].range(255, 224) = data[7];
					HBM_channelA0[offset_c + t].range(287, 256) = data[8];
					HBM_channelA0[offset_c + t].range(319, 288) = data[9];
					HBM_channelA0[offset_c + t].range(351, 320) = data[10];
					HBM_channelA0[offset_c + t].range(383, 352) = data[11];
					HBM_channelA0[offset_c + t].range(415, 384) = data[12];
					HBM_channelA0[offset_c + t].range(447, 416) = data[13];
					HBM_channelA0[offset_c + t].range(479, 448) = data[14];
					HBM_channelA0[offset_c + t].range(511, 480) = data[15];
					HBM_channelB0[offset_c + t].range(31, 0) = data[16];
					HBM_channelB0[offset_c + t].range(63, 32) = data[17];
					HBM_channelB0[offset_c + t].range(95, 64) = data[18];
					HBM_channelB0[offset_c + t].range(127, 96) = data[19];
					HBM_channelB0[offset_c + t].range(159, 128) = data[20];
					HBM_channelB0[offset_c + t].range(191, 160) = data[21];
					HBM_channelB0[offset_c + t].range(223, 192) = data[22];
					HBM_channelB0[offset_c + t].range(255, 224) = data[23];
					HBM_channelB0[offset_c + t].range(287, 256) = data[24];
					HBM_channelB0[offset_c + t].range(319, 288) = data[25];
					HBM_channelB0[offset_c + t].range(351, 320) = data[26];
					HBM_channelB0[offset_c + t].range(383, 352) = data[27];
					HBM_channelB0[offset_c + t].range(415, 384) = data[28];
					HBM_channelB0[offset_c + t].range(447, 416) = data[29];
					HBM_channelB0[offset_c + t].range(479, 448) = data[30];
					HBM_channelB0[offset_c + t].range(511, 480) = data[31];
					#else 
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
	
					#endif 
				}
				
				// parallel-read and map active frontiers [done]
				MY_LOOP173: for(unsigned int n=0; n<VALID_NUMPEs; n++){ offsets[n] = 0; }
				unsigned int uoffset = p_u * MAX_UPARTITION_SIZE;
				MY_LOOP175_DEBUG: for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){
				#pragma HLS PIPELINE II=1
					dretrievemany_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], offsets, t, kvdatas,  HBM_channelA0, HBM_channelB0);
					insertvec_cfrontierbuffer_tmp(t, kvdatas[0], cfrontier_buffer_tmp); // NEW
					if(n==1){  
						MY_LOOP176: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
						#pragma HLS UNROLL
							MY_LOOP177: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS UNROLL
								unsigned int srcvid_lpv__ = (kvdatas[inst][v].key - uoffset) >> EDGE_PACK_SIZE_POW;
								if(kvdatas[inst][v].key != INVALIDDATA && srcvid_lpv__ < MAX_UPARTITION_VECSIZE){
									unsigned int srcvid_lpv = srcvid_lpv__; 
									vprop_t tmp; tmp.prop = kvdatas[inst][v].value; tmp.mask = MASK_CODE_PE;
									insert_vdatabuffer(v, srcvid_lpv, tmp, vdata_buffer[inst]); 
								}
							}
						}
					}
					update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_FRONTIER_PROPERTIES___, 1);
				}
				}
				#endif 
				
				// reset temp stats buffer [done]
				if(___use_vertex_centric___ == false){
					MY_LOOP170: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ 
					#pragma HLS PIPELINE II=1
						MY_LOOP171: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
						#pragma HLS UNROLL
							MY_LOOP172: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
							#pragma HLS UNROLL
								insert_statstmpbuffer(t, v, 0, statsbuffer_maskbased_tmp[inst]); 
							}
						}
						update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_FRONTIER_PROPERTIES___, 1);
					}
				}
				// reset updates tmpbuffer [done]
				MY_LOOP168: for(unsigned int j=0; j<VALID_NUMPEs; j++){ 
				#pragma HLS PIPELINE II=1
					MY_LOOP169: for(unsigned int i=0; i<VALID_NUMPEs; i++){ 
					#pragma HLS UNROLL
						updates_tmpbuffer___size[i][j] = 0; 
					} 
					update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_FRONTIER_PROPERTIES___, 1);
				}
				
				// process-edges and partition-updates
				if(upartition_vertices[p_u].count > 0 || all_vertices_active_in_all_iterations == true){
					if(n == 0 && ___use_vertex_centric___ == true){
						#ifdef ___ENABLE___VCPROCESSEDGES___
						if(___ENABLE___VCPROCESSEDGES___BOOL___ == 1){
						#ifndef FPGA_IMPL
						hybrid_map[GraphIter][p_u] = 0;
						#endif 
						
						// reset buffers [done]
						for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
						#pragma HLS UNROLL
							vptrbuffer___size[inst] = 0; edges_buffer___size[inst] = 0; // reset
						} 
						
						// distribute vptrs [done]
						for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ cummtv2[inst] = 0; }
						for(unsigned int n=0; n<VALID_NUMPEs; n++){ cfrontier_bufferREAL___size[n] = 0; }
						VC_READVPTRS_LOOP1: for(unsigned int pingpong=0; pingpong<cfrontier_dram___size[p_u] * EDGE_PACK_SIZE; pingpong++){
						#pragma HLS PIPELINE II=1
							frontier_t actvv = retrieve_cfrontierbuffer_tmp(pingpong % EDGE_PACK_SIZE, pingpong / EDGE_PACK_SIZE, cfrontier_buffer_tmp); // NEW
							if(actvv.key != INVALIDDATA){
								unsigned int inst_ = actvv.key % VALID_NUMPEs;
								// cout<<"--------------------------------------- (1) actvv.key: "<<actvv.key<<endl;		
								cfrontier_buffer[inst_][cfrontier_bufferREAL___size[inst_]] = actvv;
								cfrontier_bufferREAL___size[inst_] += 1;
								update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 1);
								#ifndef FPGA_IMPL
								checkoutofbounds("acts_kernel::ERROR 073::", cfrontier_bufferREAL___size[inst_], MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
								#endif 
							}
						} 
						
						// load vptrs [done]
						max_limit = 0; MY_LOOP167: for(unsigned int n=0; n<VALID_NUMPEs; n++){ if(max_limit < cfrontier_bufferREAL___size[n]){ max_limit = cfrontier_bufferREAL___size[n]; }} 
						VC_READVPTRS_LOOP2: for(unsigned int t=0; t<max_limit; t++){ // FIXME_HARDWARE. CAUSE OF POOR FREQUENCY?
							unsigned int begins_u32[VALID_NUMPEs]; 
							unsigned int ends_u32[VALID_NUMPEs];
							frontier_t actvvs[VALID_NUMPEs];
							
							VC_READVPTRS_LOOP2B: for(unsigned int n=0; n<VALID_NUMPEs; n++){
							#pragma HLS UNROLL
								actvvs[n] = cfrontier_buffer[n][t];
								// cout<<"--------------------------------------- (1) actvvs["<<n<<"].key: "<<actvvs[n].key<<endl;		
								unsigned int hvid = 0;
								if(t < cfrontier_bufferREAL___size[n]){ hvid = actvvs[n].key / NUM_PEs; } 
								#ifdef ___FORCE_SUCCESS___
								if((t < cfrontier_bufferREAL___size[n]) && (hvid / HBM_CHANNEL_PACK_SIZE < globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS])){ ens[n][0] = true; } else { ens[n][0] = false; hvid = 0; }
								#endif
								#ifdef _DEBUGMODE_CHECKS3
								checkoutofbounds("acts_kernel::ERROR 901ccc::", (hvid / HBM_CHANNEL_PACK_SIZE), globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS], hvid, HBM_CHANNEL_PACK_SIZE, hvid / HBM_CHANNEL_PACK_SIZE);
								#endif 
								offsets[n] = hvid; 
							}
							dretrievemany_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], offsets, begins_u32,  HBM_channelA0, HBM_channelB0);
							update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 1 + DRAM_ACCESS_LATENCY);
							
							VC_READVPTRS_LOOP2C: for(unsigned int n=0; n<VALID_NUMPEs; n++){
							#pragma HLS UNROLL
								offsets[n] += 1; 
							}
							dretrievemany_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], offsets, ends_u32,  HBM_channelA0, HBM_channelB0);
							update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 1 + DRAM_ACCESS_LATENCY);
							
							#ifdef ___FORCE_SUCCESS___
							MY_LOOP10023: for(unsigned int n=0; n<VALID_NUMPEs; n++){
							#pragma HLS UNROLL
								if(ens[n][0] == false){ begins_u32[n] = 0; ends_u32[n] = 0; }
							}
							#endif 
							
							VC_READVPTRS_LOOP2D: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
							#pragma HLS UNROLL
								vtr_t data;
								data.begin = begins_u32[inst]; 
								data.end = ends_u32[inst]; 
								data.size = data.end - data.begin;
								if(data.end < data.begin){ data.size = 0; }
								// if(data.end < data.begin || data.size > globalparams[GLOBALPARAMSCODE__PARAM__MAXDEGREE]){ data.size = 0; }
								if(data.size >= (globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES] * HBM_CHANNEL_PACK_SIZE)){ data.size = 0; }
								data.vid = actvvs[inst].key; 
								data.prop = actvvs[inst].value;
								
								#ifndef FPGA_IMPL
								checkoutofbounds("acts_kernel::ERROR 0034::", vptrbuffer___size[inst], VPTR_BUFFER_SIZE, data.size, data.begin, data.end);
								#endif 
								insert_vptrbuffer(vptrbuffer___size[inst], data, vptr_buffer[inst]);
								vptrbuffer___size[inst] += 1;	
								cummtv2[inst] += data.size;
							}
						}
						
						// calculate batches [done]
						max_limit = 0; MY_LOOP166: for(unsigned int in=0; in<VALID_NUMPEs; in++){ if(max_limit < cummtv2[in]){ max_limit = cummtv2[in]; }} 
						unsigned int num_batches = 1; unsigned int cummtv2_ = max_limit / EDGE_PACK_SIZE;
						if(cummtv2_ >= EDGE_BUFFER_SIZE){
							if(cummtv2_ / 4 < EDGE_BUFFER_SIZE){ num_batches = 4; }
							else if(cummtv2_ / 8 < EDGE_BUFFER_SIZE){ num_batches = 8; } 
							else if(cummtv2_ / 16 < EDGE_BUFFER_SIZE){ num_batches = 16; }
							#ifndef FPGA_IMPL
							else { cout<<"acts_kernel:: ERROR 2367: suitable n not found. EXITING..."<<endl; cout<<"----- max_limit: "<<max_limit<<",  cummtv2_: "<<cummtv2_<<endl; for(unsigned int in=0; in<VALID_NUMPEs; in++){ cout<<"----- cummtv2["<<in<<"]: "<<cummtv2[in]<<endl; } exit(EXIT_FAILURE); }
							#endif 
						}
						max_limit = 0; for(unsigned int in=0; in<VALID_NUMPEs; in++){ if(max_limit < vptrbuffer___size[in]){ max_limit = vptrbuffer___size[in]; }}
						unsigned int batch_size = (max_limit + (num_batches - 1)) / num_batches;
					
						// read and process edges 
						VC_READANDPROCESS_EDGES_LOOP1: for(unsigned int n=0; n<num_batches; n++){
							MY_LOOP163: for(unsigned int in=0; in<VALID_NUMPEs; in++){ 
							#pragma HLS UNROLL
								edges_buffer___size[in] = 0; // reset
							} 
							
							// load edges
							VC_READ_EDGES_LOOP1: for(unsigned int k=0; k<batch_size; k++){ 
	
								load_edges(0, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[0], vptr_buffer[0], &edges_buffer___size[0], edges_buffer[0], HBM_channelA0, HBM_channelB0);
	
							}			
						
							// process & partition edges [done]
							max_limit = 0; for(unsigned int in=0; in<VALID_NUMPEs; in++){ if(max_limit < edges_buffer___size[in]){ max_limit = edges_buffer___size[in]; }} 
							VC_PROCESS_EDGES_LOOP1: for(unsigned int t=0; t<max_limit; t++){
								VC_PROCESS_EDGES_LOOP1B: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
								#pragma HLS PIPELINE II=2
									VC_PROCESS_EDGES_LOOP1C: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
									#pragma HLS UNROLL
										edge3_type edge = retrieve_edgesbuffer(v, t, edges_buffer[inst]); // NEW
										if(edge.dstvid != INVALIDDATA){
											unsigned int uprop = edge.srcvid; // edge.srcvid has uprop information
											unsigned int res = process_funcG(uprop, 1);
											#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3 // _DEBUGMODE_KERNELPRINTS // 4 //_DEBUGMODE_KERNELPRINTS_TRACE3
											cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", uprop: "<<uprop<<", res: "<<res<<"]"<<endl;
											cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [local partition: "<<p_u<<", vid: "<<NAp<<"]"<<endl;
											#endif 
											
											unsigned int inst_mirror = get_H(edge.dstvid);
											keyvalue_t update; update.key = edge.dstvid; update.value = res;
											#ifndef FPGA_IMPL
											checkoutofbounds("acts_kernel::ERROR 813a::", inst_mirror, NUM_PEs,  edge.dstvid, inst_mirror, NAp);
											#endif 
											
											bool save;
											#ifdef ___FORCE_SUCCESS___
											if(inst_mirror < VALID_NUMPEs){ if(t < edges_buffer___size[inst] && updates_tmpbuffer___size[inst][inst_mirror] < UPDATES_BUFFER_SIZE){ save = true; } else { save = false; }} else { save = false; }
											#else 
											if(t < edges_buffer___size[inst]){ save = true; } else { save = false; }
											#endif 
											if(save){
												#ifdef _DEBUGMODE_CHECKS3
												checkoutofbounds("acts_kernel::ERROR 8134a::", inst_mirror, VALID_NUMPEs,  edge.dstvid, inst_mirror, NAp);
												#endif 
												#if not defined(___FORCE_SUCCESS___) && not defined(FPGA_IMPL)
												checkoutofbounds("acts_kernel::ERROR 813a::", inst_mirror, VALID_NUMPEs,  edge.dstvid, inst_mirror, NAp);
												checkoutofbounds("acts_kernel::ERROR 813b::", updates_tmpbuffer___size[inst][inst_mirror], UPDATES_BUFFER_SIZE, NAp, NAp, NAp);
												#endif 
												insert_updatestmpbuffer(inst_mirror, updates_tmpbuffer___size[inst][inst_mirror], update, updates_buffer2[inst]); //
												updates_tmpbuffer___size[inst][inst_mirror] += 1;	
											}
										}
									}
									update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 2); 
								}
							}
						}
						
						// exchange updates between HBMs [done]
						VC_EXCHANGE_UPDATES_LOOP1: for(unsigned int inst_mirror=0; inst_mirror<VALID_NUMPEs; inst_mirror++){
							VC_EXCHANGE_UPDATES_LOOP1B: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
								VC_EXCHANGE_UPDATES_LOOP1C: for(unsigned int t=0; t<updates_tmpbuffer___size[inst][inst_mirror]; t++){
								#pragma HLS PIPELINE II=3 
									keyvalue_t update = retrieve_updatestmpbuffer(inst_mirror, t, updates_buffer2[inst]);
									
									unsigned int vid_l = get_local(update.key); // NEW**
									unsigned int vid_lp = get_local_to_vpartition(vid_l);
									unsigned int p_v = vid_l / MAX_APPLYPARTITION_SIZE;
									keyvalue_t update_local = update; update_local.key = vid_lp;
									unsigned int v = update_local.key % EDGE_PACK_SIZE;
									#ifdef _DEBUGMODE_KERNELPRINTS//4
									cout<<"VC - EXCHANGE UPDATE SEEN @: [inst: "<<inst<<", inst_mirror: "<<inst_mirror<<"], [update.key: "<<update.key<<", update.value: "<<update.value<<"]"<<endl;
									#endif 
									
									#ifdef _DEBUGMODE_CHECKS3
									if((update_local.key % EDGE_PACK_SIZE) != v){ cout<<"acts_kernel 12347:: update_local.key("<<update_local.key<<") % v("<<v<<") != 0. EXITING..."<<endl; for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"update_local.key ("<<(update_local.key % EDGE_PACK_SIZE)<<"): "<<update_local.key<<""<<endl; } exit(EXIT_FAILURE); }				
									#endif 
									
									#ifdef _DEBUGMODE_CHECKS3
									checkoutofbounds("acts_kernel::ERROR 1232::", p_v, __NUM_APPLYPARTITIONS, update.key, get_local(update.key), MAX_APPLYPARTITION_SIZE);
									checkoutofbounds("acts_kernel::ERROR 1222::", (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + updates_buffer___size[inst_mirror][v][p_v], MAX_APPLYPARTITION_VECSIZE, updates_buffer___size[inst_mirror][v][p_v], inst_mirror, p_v);
									#endif 
									vprop_t tmp; tmp.prop = update_local.key; tmp.mask = update_local.value;
									insert_vdatabuffer(v, (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + updates_buffer___size[inst_mirror][v][p_v], tmp, vdata_buffer[inst_mirror]); 
									#ifdef ___FORCE_SUCCESS___
									if(updates_buffer___size[inst_mirror][v][p_v] + 8 < MAX_APPLYPARTITION_VECSIZE){ updates_buffer___size[inst_mirror][v][p_v] += 1; } 
									#else 
									updates_buffer___size[inst_mirror][v][p_v] += 1;
									#endif 
									vpartition_vertices[0][p_v].count = 1;
									
									unsigned int dstvid_lpv = update_local.key / EDGE_PACK_SIZE; 
									#ifndef FPGA_IMPL
									checkoutofbounds("acts_kernel::ERROR 171::", dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE, BLOCKRAM_SIZE, NAp, NAp, NAp);
									checkoutofbounds("acts_kernel::ERROR 172::", p_v, __NUM_APPLYPARTITIONS, NAp, NAp, NAp);
									#endif
									insert_statsbuffer(dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE, p_v, 1, statsbuffer_maskbased[inst_mirror]);
									if(inst==0){ update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 3); }
								}
							}
						}
						}
						#endif 
					} else if (n == 1  && ___use_vertex_centric___ == false) { // edge-centric [done]
						#ifdef ___ENABLE___ECPROCESSEDGES___
						if(___ENABLE___ECPROCESSEDGES___BOOL___ == 1){
						unsigned int offsets3[VALID_NUMPEs]; 
						#pragma HLS ARRAY_PARTITION variable = offsets3 complete
						
						#ifndef FPGA_IMPL
						hybrid_map[GraphIter][p_u] = 1;
						#endif 
					
						EC_PROCESS_EDGES_LOOP1: for(unsigned int llp_set=0; llp_set<__NUM_APPLYPARTITIONS; llp_set++){
							map_t maps[VALID_NUMPEs];
							#pragma HLS ARRAY_PARTITION variable=maps complete	
							dretrievemany_actpackvptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS], p_u, llp_set, maps,  HBM_channelA0, HBM_channelB0);
							unsigned int max_sz = 0; for(unsigned int n=0; n<VALID_NUMPEs; n++){ limits[n] = maps[n].size; } for(unsigned int n=0; n<VALID_NUMPEs; n++){ if(max_sz < limits[n]){ max_sz = limits[n]; }}
							
							// process edges [done]
							EC_PROCESS_EDGES_LOOP1B: for(unsigned int it=0; it<(max_sz + (BLOCKRAM_SIZE - 1)) / BLOCKRAM_SIZE; it++){
								MY_LOOP141: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
								#pragma HLS UNROLL
									stats_count[n] = 0; 
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										stats_counts[n][v] = 0; 
									}
								} 
								unsigned int sz = 0; if((it + 1) * BLOCKRAM_SIZE >= max_sz){ sz = max_sz - (it * BLOCKRAM_SIZE); } else { sz = BLOCKRAM_SIZE; }
								MY_LOOP1514: for(unsigned int n=0; n<VALID_NUMPEs; n++){ offsets[n] = maps[n].offset; }
								MY_LOOP1512: for(unsigned int n=0; n<VALID_NUMPEs; n++){ offsets2[n] = actpackupdates_dram___size[n][llp_set]; }
								
								// read and process 
								EC_PROCESS_EDGES_MAINLOOP1D: for(unsigned int t_=0; t_<sz; t_++){ 
								#pragma HLS PIPELINE II=1
									unsigned int t = (it * BLOCKRAM_SIZE) + t_;
									dretrievemany_act_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0);
									
									keyvalue_t update_in[VALID_NUMPEs][EDGE_PACK_SIZE];
									#pragma HLS ARRAY_PARTITION variable = update_in complete dim=0
									keyvalue_t update_out[VALID_NUMPEs][EDGE_PACK_SIZE];
									#pragma HLS ARRAY_PARTITION variable = update_out complete dim=0
									
									EC_PROCESS_EDGES_LOOP1E: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
									#pragma HLS UNROLL
										bool en = true; if(t >= limits[inst]){ en = false; } else { en = true; } // NEW*
										edge3_vec_dt edge_vec = edge3_vecs[inst];
										unsigned int sample_key = edge_vec.data[0].dstvid % EDGE_PACK_SIZE; unsigned int rotateby = sample_key; 	
										#ifdef _DEBUGMODE_CHECKS3
										checkoutofbounds("acts_kernel::ERROR 213::", rotateby, EDGE_PACK_SIZE, NAp, NAp, NAp);
										#endif
											
										MY_LOOP16214: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
										#pragma HLS UNROLL
											edge3_type edge = edge_vec.data[v];
											unsigned int srcvid_lpv = edge.srcvid;
											vprop_t uprop; if(edge.srcvid != INVALIDDATA && edge.srcvid < MAX_UPARTITION_VECSIZE){ uprop = retrieve_vdatabuffer(v, srcvid_lpv, vdata_buffer[inst]); }
											if(edge.srcvid != INVALIDDATA && edge.srcvid < MAX_UPARTITION_VECSIZE && (uprop.mask == MASK_CODE_PE || all_vertices_active_in_all_iterations == true)){
												unsigned int res = process_funcG(uprop.prop, 1);
												update_in[inst][v].key = edge.dstvid; update_in[inst][v].value = res;
												#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
												cout<<"EC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [srcvid_lpv: "<<srcvid_lpv<<", dstvid: "<<edge.dstvid % MAX_APPLYPARTITION_SIZE<<"], [edge.srcvid: "<<edge.srcvid<<", dstvid: "<<edge.dstvid<<"]"<<endl; 
												#endif 
											} else {
												update_in[inst][v].key = INVALIDDATA; update_in[inst][v].value = INVALIDDATA;
											}
										}
										
										// circular shift >>>
										// #ifdef ADJUSTMENT_TO_MEET_VHLS_TIMING
											// MY_LOOP16714: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
											// #pragma HLS UNROLL
												// update_out[inst][v] = update_in[inst][v];
											// }
										// #else
											rearrangeLayoutVx16B(rotateby, update_in[inst], update_out[inst]); 
											#ifdef _DEBUGMODE_CHECKS3
											if(en == true){ check_if_contiguous(update_out[inst], update_in[inst], update_in[inst], sample_key, 0, rotateby); }
											#endif
										// #endif 
										
										bool isvalid = true;
										if(en == true && isvalid == true){
											MY_LOOP16234: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
											#pragma HLS UNROLL
												if(update_out[inst][v].key != INVALIDDATA){
													#ifndef FPGA_IMPL
													if(update_out[inst][v].key != INVALIDDATA){ checkoutofbounds("acts_kernel::ERROR 623::", ((update_out[inst][v].key / EDGE_PACK_SIZE) >> ACTVUPDATESBLOCK_VECSIZE_POW), BLOCKRAM_SIZE, update_out[inst][v].key, (update_out[inst][v].key / EDGE_PACK_SIZE), (MAX_APPLYPARTITION_VECSIZE / BLOCKRAM_SIZE)); }
													#endif 
													insert_updatestmpbuffer(v, stats_counts[inst][v], update_out[inst][v], updates_buffer2[inst]); 
													stats_counts[inst][v] += 1;
													unsigned int dstvid_lpv = update_out[inst][v].key / EDGE_PACK_SIZE;
													if(update_out[inst][v].key != INVALIDDATA){ insert_statstmpbuffer((dstvid_lpv >> ACTVUPDATESBLOCK_VECSIZE_POW), v, 1, statsbuffer_maskbased_tmp[inst]); }
												}
											}
										}
									}
									update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
								}
								
								// write 
								unsigned int max_sz2 = 0; for(unsigned int n=0; n<VALID_NUMPEs; n++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max_sz2 < stats_counts[n][v]){ max_sz2 = stats_counts[n][v]; }}}
								keyvalue_t invalidkv; invalidkv.key = INVALIDDATA; invalidkv.value = INVALIDDATA;
								EC_SAVE_UPDATES_LOOP1F: for(unsigned int t=0; t<max_sz2; t++){
								#pragma HLS PIPELINE II=1
									MY_LOOP15213: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
									#pragma HLS UNROLL
										MY_LOOP15313: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
										#pragma HLS UNROLL
											if(t < stats_counts[inst][v]){ 
												updates_vecs[inst].data[v] = retrieve_updatestmpbuffer(v, t, updates_buffer2[inst]);
											} 
											#ifdef ___FALSE___
											else { updates_vecs[inst].data[v] = invalidkv; }	
											#endif 
											if(t < stats_counts[inst][v]){ ens[inst][v] = true; } else { ens[inst][v] = false;  }
										}
									}
									for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
									#pragma HLS UNROLL
										offsets3[n] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[llp_set] + offsets2[n] + t;
									}
									dinsertmany_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[llp_set], llp_set, offsets3, t, updates_vecs, ens,  HBM_channelA0, HBM_channelB0, updatesptrs);		
									update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
								}
								
								for(unsigned int n=0; n<VALID_NUMPEs; n++){ max_limits[n] = 0; } 
								for(unsigned int n=0; n<VALID_NUMPEs; n++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max_limits[n] < stats_counts[n][v]){ max_limits[n] = stats_counts[n][v]; }}}
								MY_LOOP161: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
								#pragma HLS UNROLL
									actpackupdates_dram___size[n][llp_set] += max_limits[n]; // NEW*
								}
							}
							
							// gather stats [done]
							unsigned int data[EDGE_PACK_SIZE];
							EC_PROCESS_EDGES_LOOP1I: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
							#pragma HLS PIPELINE II=1
								for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
								#pragma HLS UNROLL
									retrievevec_statstmpbuffer(t, data, statsbuffer_maskbased_tmp[inst]); 
									#ifndef FPGA_IMPL
									checkoutofbounds("acts_kernel::ERROR 023::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
									#endif
									if(  data[0]==1|| data[1]==1|| data[2]==1|| data[3]==1|| data[4]==1|| data[5]==1|| data[6]==1|| data[7]==1|| data[8]==1|| data[9]==1|| data[10]==1|| data[11]==1|| data[12]==1|| data[13]==1|| data[14]==1|| data[15]==1 )
									{ 
										insert_statsbuffer(t, llp_set, 1, statsbuffer_maskbased[inst]); 
										vpartition_vertices[inst][llp_set].count = 1;
									}
								}
								update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
							}
						}
						}
						#endif 
					} else {
						// do nothing
					}
				}
			}
			}
			#endif 
			
			// save csr updates [done]
			#ifdef ___ENABLE___SAVEVCUPDATES___
			if(___ENABLE___SAVEVCUPDATES___BOOL___ == 1){
			unsigned int offsets1[VALID_NUMPEs]; 
			#pragma HLS ARRAY_PARTITION variable = offsets1 complete
			unsigned int offsets2[VALID_NUMPEs]; 
			#pragma HLS ARRAY_PARTITION variable = offsets2 complete
			unsigned int offsets3[VALID_NUMPEs]; 
			#pragma HLS ARRAY_PARTITION variable = offsets3 complete
			if(n==0){
				SAVE_CSR_UPDATES_LOOP1: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
					unsigned int voffsett = p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION;
					for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
					#pragma HLS UNROLL
						offsets1[n] = actpackupdates_dram___size[n][p_v]; offsets2[n] = actpackupdates_dram___size[n][p_v]; // NEW*
					} 
					max_limit = 0; for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ max_limits[inst] = 0; }
					SAVE_CSR_UPDATES_LOOP1B: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
					#pragma HLS UNROLL
						SAVE_CSR_UPDATES_LOOP1B1: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max_limits[inst] < updates_buffer___size[inst][v][p_v]){ max_limits[inst] = updates_buffer___size[inst][v][p_v]; }}
					} 
					SAVE_CSR_UPDATES_LOOP1C: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ if(max_limit < max_limits[inst]){ max_limit = max_limits[inst]; }}
					
					uint512_vec_dt updates_vecs[VALID_NUMPEs]; 
					#pragma HLS ARRAY_PARTITION variable = updates_vecs complete
					keyvalue_t invalidkv; invalidkv.key = INVALIDDATA; invalidkv.value = INVALIDDATA;
					SAVE_CSR_UPDATES_LOOP1D: for(unsigned int t=0; t<max_limit; t++){ // FIXME? CAUSING HANGING?
					#pragma HLS PIPELINE II=1
						SAVE_CSR_UPDATES_LOOP1E: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
						#pragma HLS UNROLL
							SAVE_CSR_UPDATES_LOOP1F: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS UNROLL
								if(t < updates_buffer___size[inst][v][p_v]){ 
									vprop_t tmp = retrieve_vdatabuffer(v, voffsett + t, vdata_buffer[inst]); 
									updates_vecs[inst].data[v].key = tmp.prop; updates_vecs[inst].data[v].value = tmp.mask;
									#ifdef _DEBUGMODE_CHECKS3
									if((updates_vecs[inst].data[v].key % EDGE_PACK_SIZE) != v){ cout<<"acts_kernel 1234:: updates_vecs["<<inst<<"].data["<<v<<"].key("<<updates_vecs[inst].data[v].key<<") % v("<<v<<") != 0. EXITING..."<<endl; for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"updates_vecs["<<inst<<"].data["<<v1<<"].key ("<<(updates_vecs[inst].data[v].key % EDGE_PACK_SIZE)<<"): "<<updates_vecs[inst].data[v1].key<<""<<endl; } exit(EXIT_FAILURE); }				
									#endif 
								}
								#ifdef ___FALSE___
								else { updates_vecs[inst].data[v] = invalidkv; } // VHLS-RESOLUTION
								#endif 
								if(t < updates_buffer___size[inst][v][p_v]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
								#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
								if(t < updates_buffer___size[inst][v][p_v]){ cout<<"VC - COMMIT CSR UPDATES SEEN @: [inst: "<<inst<<"], [updates_vecs["<<inst<<"].data["<<v<<"].key: "<<updates_vecs[inst].data[v].key<<", updates_vecs["<<inst<<"].data["<<v<<"].value: "<<updates_vecs[inst].data[v].value<<"]"<<endl; }
								#endif
							}
						}
						#ifndef FPGA_IMPL
						checkoutofbounds("acts_kernel::ERROR 152::", actpackupdates_dram___size[0][p_v], HBM_CHANNEL_SIZE, NAp, NAp, NAp);
						#endif 
						for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
						#pragma HLS UNROLL
							offsets3[n] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v] + offsets1[n] + t;
						}
						dinsertmany_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, offsets3, t, updates_vecs, ens,  HBM_channelA0, HBM_channelB0, updatesptrs);
						SAVE_CSR_UPDATES_LOOP1G: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
						#pragma HLS UNROLL
							if(t < max_limits[n]){ offsets2[n] += 1; }
						}
						update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___SAVEVCUPDATES___, 1);
					}
					for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
					#pragma HLS UNROLL
						actpackupdates_dram___size[n][p_v] = offsets2[n]; 
					}
				}
			}
			}
			#endif 
		}
		
		// prepare active destID stats [done] 
		#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
		if(___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ == 1){
		if(enable___collectactivedstvids == true){
			PREPARE_ACTIVE_DESTIDSTATS_LOOP1C: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
			#pragma HLS PIPELINE II=1
				PREPARE_ACTIVE_DESTIDSTATS_LOOP1D: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
				#pragma HLS UNROLL
					PREPARE_ACTIVE_DESTIDSTATS_LOOP1E: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
					#pragma HLS UNROLL
						if(retrieve_statsbuffer(t, p_v, statsbuffer_maskbased[inst]) == 1){ 
							#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
							cout<<"VC - PREPARE ACTIVE DESTIDS SEEN @: [inst: "<<inst<<"], [p_v: "<<p_v<<", t: "<<t<<"]"<<endl;
							#endif
							insert_stats2buffer(statsbuffer_idbased___size[inst][p_v], p_v, t, statsbuffer_idbased[inst]); 
							statsbuffer_idbased___size[inst][p_v] += 1;
						}
					}
				}
				update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECTACTIVEDSTVIDS___, 1);
			}
		}
		}
		#endif 
		
		// apply updates
		#ifdef ___ENABLE___APPLYUPDATESMODULE___
		if(___ENABLE___APPLYUPDATESMODULE___BOOL___ == 1){
		APPLY_UPDATES_MODULE_LOOP: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			bool en = true; if(enable___collectactivedstvids == true){ if(vpartition_vertices[0][p_v].count > 0){ en=true; } else { en=false; }} else { en = true; }
			unsigned int voffset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] + (p_v * MAX_APPLYPARTITION_VECSIZE);
			
			if(vpartition_vertices[0][p_v].count > 0){ 
				// read destination properties
				#ifdef ___ENABLE___READ_DEST_PROPERTIES___
				if(___ENABLE___READ_DEST_PROPERTIES___BOOL___ == 1){
				READ_DEST_PROPERTIES_LOOP1: for(unsigned int n=0; n<VALID_NUMPEs; n++){ offsets[n] = 0; }
				if(stats_buffer___size[0][p_v] < threshold___activedstvids && enable___collectactivedstvids == true){
					#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
					if(___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ == 1){
					max_limit = 0; for(unsigned int n=0; n<VALID_NUMPEs; n++){ if(max_limit < statsbuffer_idbased___size[n][p_v]){ max_limit = statsbuffer_idbased___size[n][p_v]; }}
					READ_DEST_PROPERTIES_LOOP1B: for(unsigned int k=0; k<max_limit; k++){
						for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
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
							for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
							#pragma HLS UNROLL
								if((k < statsbuffer_idbased___size[inst][p_v]) && (offsets[inst] + t < MAX_APPLYPARTITION_VECSIZE)){ 
									insertvec_vdatabuffer(offsets[inst] + t, datas[inst], vdata_buffer[inst]);
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
						READ_DEST_PROPERTIES_LOOP2C: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
						#pragma HLS UNROLL
							insertvec_vdatabuffer(t, datas[inst], vdata_buffer[inst]);
						}
						update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_DEST_PROPERTIES___, 1);
					}
				}
				}
				#endif 
				
				// apply updates [done] 
				#ifdef ___ENABLE___APPLYUPDATES___
				if(___ENABLE___APPLYUPDATES___BOOL___ == 1){
				uint512_vec_dt updates_vecs[VALID_NUMPEs];
				max_limit = 0; for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ limits[inst] = actpackupdates_dram___size[inst][p_v]; } for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
				
				APPLY_UPDATES_LOOP1: for(unsigned int it=0; it<(max_limit + (BLOCKRAM_SIZE - 1)) / BLOCKRAM_SIZE; it++){
					unsigned int sz = 0; if((it + 1) * BLOCKRAM_SIZE >= max_limit){ sz = max_limit - (it * BLOCKRAM_SIZE); } else { sz = BLOCKRAM_SIZE; }
					APPLY_UPDATES_LOOP1A: for(unsigned int t_=0; t_<sz; t_++){ 
					#pragma HLS PIPELINE II=1
						unsigned int t = (it * BLOCKRAM_SIZE) + t_;
						dretrievemany_udatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, t, updates_vecs,  HBM_channelA0, HBM_channelB0, updatesptrs); // NEW
						for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
						#pragma HLS UNROLL
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS UNROLL
								insert_updatestmpbuffer(v, t_, updates_vecs[inst].data[v], updates_buffer2[inst]); 
							}
						}
						update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___APPLYUPDATES___, 1);
					}
					
					APPLY_UPDATES_LOOP1B: for(unsigned int t_=0; t_<sz; t_++){
					#pragma HLS PIPELINE II=4
						unsigned int t = (it * BLOCKRAM_SIZE) + t_;
						MY_LOOP1621: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
						#pragma HLS UNROLL
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS UNROLL
								updates_vecs[inst].data[v] = retrieve_updatestmpbuffer(v, t_, updates_buffer2[inst]);
							}
						}
						MY_LOOP1521: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
						#pragma HLS UNROLL
							if(t < limits[inst]){
								uint512_vec_dt updates_vec = updates_vecs[inst];
								APPLY_UPDATES_LOOP1C: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
								#pragma HLS UNROLL
									keyvalue_t update = updates_vec.data[v];
									unsigned int dstvid_lp = update.key; 
									unsigned int dstvid_lpv = update.key / EDGE_PACK_SIZE; 
									if(update.key != INVALIDDATA && update.key < MAX_APPLYPARTITION_SIZE){
										#ifdef _DEBUGMODE_CHECKS3
										#ifndef ___FORCE_SUCCESS___
										if(t < limits[inst] && ((dstvid_lp % EDGE_PACK_SIZE) != v)){ cout<<"acts_kernel 234:: dstvid_lp("<<dstvid_lp<<") % v("<<v<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
										#endif 
										if(t < limits[inst]){ checkoutofbounds("acts_kernel::ERROR 727a::", dstvid_lp, MAX_APPLYPARTITION_SIZE, NAp, inst, update.key); checkoutofbounds("acts_kernel::ERROR 727b::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, NAp, inst, update.key); }
										#endif
										vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, vdata_buffer[inst]);
										unsigned int new_vprop = reduce_funcG(vprop.prop, vprop.prop, update.value);
										if(new_vprop != vprop.prop && t < limits[inst]){ 
											#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
											cout<<"APPLY (ACT-PACK) UPDATE SEEN @: inst: ["<<inst<<"]: dstvid_lp: "<<dstvid_lp<<", dstvid_lpv: "<<dstvid_lpv<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<endl;
											#endif
											vprop_t newprop; newprop.prop = new_vprop; newprop.mask = MASK_CODE_AU;
											insert_vdatabuffer(v, dstvid_lpv, newprop, vdata_buffer[inst]);
										}
									}
								}
							}
						}
						update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___APPLYUPDATES___, 4);
					}
				}	
				}
				#endif 
				
				// collect and save frontier information
				#ifdef ___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___
				if(___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___BOOL___ == 1){
				MY_LOOP234: for(unsigned int n=0; n<VALID_NUMPEs; n++){ offsets[n] = 0; }
				MY_LOOP235: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
				#pragma HLS UNROLL
					MY_LOOP236: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						MY_LOOP237: for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){ nfrontier_buffer___size[n][v][p_actvv] = 0; }
					}
				}
				MY_LOOP238: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
				#pragma HLS UNROLL
					vid_first0[n] = get_global(((p_v * MAX_APPLYPARTITION_SIZE) + ((0 * MAX_ACTVV_VECSIZE + 0) * EDGE_PACK_SIZE + 0)), n); 
				}
				
				unsigned int n_____ = 0;
				unsigned int n_aup = 0;
				COLLECT_AND_SAVE_FRONTIER_INFO_LOOP1: for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
					unsigned int temp_[VALID_NUMPEs][EDGE_PACK_SIZE];
					#pragma HLS ARRAY_PARTITION variable=temp_ complete dim=0
					unsigned int temp_2_[VALID_NUMPEs];
					#pragma HLS ARRAY_PARTITION variable=temp_2_ complete
					unsigned int p_actvv_ = (p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION) + p_actvv;
					MY_LOOP100: for(unsigned int n=0; n<VALID_NUMPEs; n++){
					#pragma HLS UNROLL
						MY_LOOP101: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							temp_[n][v] = nfrontier_buffer___size[n][v][p_actvv];
						}
						temp_2_[n] = nfrontier_dram___size[n][p_actvv_];
					}
					
					// collect frontier information [done]
					MY_LOOP103: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
					#pragma HLS UNROLL
						vid_first1[inst] = vid_first0[inst]; 
					}

					unsigned int num_its = (MAX_ACTVV_VECSIZE + (ACTVUPDATESBLOCK_VECSIZE - 1)) / ACTVUPDATESBLOCK_VECSIZE;
					unsigned int baseoffset0_aup = p_actvv * MAX_ACTVV_VECSIZE;
					unsigned int baseoffset1_aup = 0;
					COLLECT_FRONTIER_INFO_LOOP1A: for(unsigned int au_p=0; au_p<num_its; au_p++){
						unsigned int offset_aup = (baseoffset0_aup + baseoffset1_aup) / ACTVUPDATESBLOCK_VECSIZE;
						#ifdef _DEBUGMODE_CHECKS3
						checkoutofbounds("acts_kernel::ERROR 772::", ((p_actvv * MAX_ACTVV_VECSIZE) + (au_p * ACTVUPDATESBLOCK_VECSIZE)), MAX_APPLYPARTITION_VECSIZE, p_actvv, au_p, MAX_ACTVV_VECSIZE);
						checkoutofbounds("acts_kernel::ERROR 773::", offset_aup, BLOCKRAM_SIZE, p_actvv, au_p, MAX_ACTVV_VECSIZE);
						#endif 
						unsigned int mask = 1; if(enable___collectactivedstvids == true){ mask = statsbuffer_maskbased[0][p_v][offset_aup]; } else { mask = 1; }
						if(mask > 0){ 	
							COLLECT_FRONTIER_INFO_LOOP1B: for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								unsigned int dstvid_lpv = baseoffset0_aup + baseoffset1_aup + t;
								bool en; if(baseoffset1_aup + t < MAX_ACTVV_VECSIZE && dstvid_lpv < MAX_APPLYPARTITION_VECSIZE){ en = true; } else { en = false; }
								MY_LOOP114: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
								#pragma HLS UNROLL
									MY_LOOP104: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										if(en==true){ 
											#ifdef _DEBUGMODE_CHECKS3
											checkoutofbounds("acts_kernel::ERROR 713::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, p_actvv, au_p, t); 
											#endif 
											// checkoutofbounds("acts_kernel::ERROR 103-d::", dstvid_lpv, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
											vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, vdata_buffer[inst]);
											unsigned int vid = vid_first1[inst] + v;
											#ifdef _DEBUGMODE_CHECKS3
											if((vid % EDGE_PACK_SIZE != v)){ cout<<"acts_kernel:: ERROR 234. vid("<<vid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<vid % EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
											#endif
											if(vprop.mask == MASK_CODE_AU){ 
												#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
												cout<<"COLLECT FRONTIER INFORMATION SEEN @: inst: ["<<inst<<"]: dstvid_lpv: "<<dstvid_lpv<<", v: "<<v<<", p__u__: "<<(vid / MAX_UPARTITION_SIZE)<<", vid: "<<vid<<endl;
												#endif
												frontier_t actvv; actvv.key = vid; actvv.value = vprop.prop; 
												#ifndef FPGA_IMPL
												checkoutofbounds("acts_kernel::ERROR 21888::", temp_[inst][v], MAX_ACTVV_VECSIZE, NAp, NAp, NAp);
												#endif
												insert_nfrontierbuffer(v, temp_[inst][v], actvv, nfrontier_buffer[inst]); // NEW*
												temp_[inst][v] += 1;
											}
										}
									}
								}
								MY_LOOP105: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
								#pragma HLS UNROLL
									vid_first1[inst] += CONST1; 
								}
								update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___, 1);
							}
						}
						baseoffset1_aup += ACTVUPDATESBLOCK_VECSIZE;
					}
					
					// save frontier information [done]
					unsigned int max_limit = 0; for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ max_limits[inst] = 0; }
					MY_LOOP106: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
						MY_LOOP107: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
						#pragma HLS UNROLL
							if(max_limits[inst] < temp_[inst][v]){ max_limits[inst] = temp_[inst][v]; }
						}
					}
					MY_LOOP108: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ if(max_limit < max_limits[inst]){ max_limit = max_limits[inst]; }}
					unsigned int offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + (p_actvv_ * MAX_APPLYPARTITION_VECSIZE);
					keyvalue_t invalidkv; invalidkv.key = INVALIDDATA; invalidkv.value = INVALIDDATA;
					SAVE_FRONTIER_INFO_LOOP2B: for(unsigned int t=0; t<max_limit; t++){
					#pragma HLS PIPELINE II=1
						MY_LOOP1092: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
						#pragma HLS UNROLL
							#ifndef FPGA_IMPL
							checkoutofbounds("acts_kernel::ERROR 21777::", t, MAX_ACTVV_VECSIZE, NAp, NAp, NAp);
							#endif 
							retrievevec_nfrontierbuffer(t, actvvs[inst], nfrontier_buffer[inst]); 
							MY_LOOP109: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 	
							#pragma HLS UNROLL
								if(t >= temp_[inst][v]){ actvvs[inst][v] = invalidkv; } 
								if(t < max_limits[inst]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
							}	
						}
						MY_LOOP1019: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
						#pragma HLS UNROLL
							offsets2[n] = offset + offsets[n] + t;
						}
						dinsertmany_nfrontierdram(offset, p_actvv_, offsets2, t, actvvs, ens,  HBM_channelA0, HBM_channelB0);
						MY_LOOP110: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
						#pragma HLS UNROLL
							if(t < max_limits[n]){ temp_2_[n] += 1; }
						}
						update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___, 1);
					}
					
					MY_LOOP111: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
					#pragma HLS UNROLL
						vid_first0[inst] += CONST2; 
					}
					MY_LOOP112: for(unsigned int n=0; n<VALID_NUMPEs; n++){
					#pragma HLS UNROLL
						MY_LOOP113: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							nfrontier_buffer___size[n][v][p_actvv] = temp_[n][v];
						}
						nfrontier_dram___size[n][p_actvv_] = temp_2_[n];
					}
				}
				}
				#endif 
				
				// save destination properties
				#ifdef ___ENABLE___SAVE_DEST_PROPERTIES___
				if(___ENABLE___SAVE_DEST_PROPERTIES___BOOL___ == 1){
				MY_LOOP211: for(unsigned int n=0; n<VALID_NUMPEs; n++){ offsets[n] = 0; }
				if(stats_buffer___size[0][p_v] < threshold___activedstvids && enable___collectactivedstvids == true){
					#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
					if(___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ == 1){
					max_limit = 0; for(unsigned int n=0; n<VALID_NUMPEs; n++){ if(max_limit < statsbuffer_idbased___size[n][p_v]){ max_limit = statsbuffer_idbased___size[n][p_v]; }}
					SAVE_DEST_PROPERTIES_LOOP1: for(unsigned int k=0; k<max_limit; k++){	
						SAVE_DEST_PROPERTIES_LOOP1B: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
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
							for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){
							#pragma HLS UNROLL
								if((k < statsbuffer_idbased___size[inst][p_v]) && (offsets[inst] + t < MAX_APPLYPARTITION_VECSIZE)){ 
									retrievevec_vdatabuffer(offsets[inst] + t, datas[inst], vdata_buffer[inst]);
									MY_LOOP215: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
									#pragma HLS UNROLL
										datas[inst][v].mask = INVALIDMASK; // clear mask
									} 
								}
							}
							MY_LOOP10302: for(unsigned int n=0; n<VALID_NUMPEs; n++){
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
						SAVE_DEST_PROPERTIES_LOOP2B: for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ 
						#pragma HLS UNROLL
							retrieveandresetvec_vdatabuffer(t, datas[inst], vdata_buffer[inst]);
						}
						MY_LOOP10202: for(unsigned int n=0; n<VALID_NUMPEs; n++){ 
						#pragma HLS UNROLL
							offsets2[n] = voffset + offsets[n] + t; 
						}
						dinsertmany_vdatadram(voffset, offsets2, t, datas,  HBM_channelA0, HBM_channelB0);
						update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___SAVE_DEST_PROPERTIES___, 1);
					}
				}
				}
				#endif 
			}
		} // p_v
		}
		#endif 
		
		// end of GAS iteration - gathering frontier information
		unsigned int totalactvvs2 = 0; 
		#ifdef ___ENABLE___EXCHANGEFRONTIERINFOS___
		if(___ENABLE___EXCHANGEFRONTIERINFOS___BOOL___ == 1){
		frontier_t actvv[EDGE_PACK_SIZE]; 
		MY_LOOP311: for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ upartition_vertices[p_u].count = 0; }
		MY_LOOP312: for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ cfrontier_dram___size[p_u] = 0; } // reset
		EXCHANGE_FRONTIER_MODULE_LOOP: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			transport_frontier(0, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[0], upartition_vertices, HBM_channelA0, HBM_channelB0, HBM_centerA, HBM_centerB, &totalactvvs2);
		}
		}
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS3 
		#ifdef _DEBUGMODE_KERNELPRINTS4
		if(all_vertices_active_in_all_iterations == true){
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___RESETBUFFERSATSTART___: "<<_NUMCLOCKCYCLES_[0][___CODE___RESETBUFFERSATSTART___]<<endl;
		// cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___PROCESSEDGES___: "<<_NUMCLOCKCYCLES_[0][___CODE___PROCESSEDGES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___READ_FRONTIER_PROPERTIES___: "<<_NUMCLOCKCYCLES_[0][___CODE___READ_FRONTIER_PROPERTIES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___VCPROCESSEDGES___: "<<_NUMCLOCKCYCLES_[0][___CODE___VCPROCESSEDGES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___ECPROCESSEDGES___: "<<_NUMCLOCKCYCLES_[0][___CODE___ECPROCESSEDGES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___SAVEVCUPDATES___: "<<_NUMCLOCKCYCLES_[0][___CODE___SAVEVCUPDATES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___COLLECTACTIVEDSTVIDS___: "<<_NUMCLOCKCYCLES_[0][___CODE___COLLECTACTIVEDSTVIDS___]<<endl;
		// cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___APPLYUPDATESMODULE___: "<<_NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATESMODULE___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___READ_DEST_PROPERTIES___: "<<_NUMCLOCKCYCLES_[0][___CODE___READ_DEST_PROPERTIES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___APPLYUPDATES___: "<<_NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___: "<<_NUMCLOCKCYCLES_[0][___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___SAVE_DEST_PROPERTIES___: "<<_NUMCLOCKCYCLES_[0][___CODE___SAVE_DEST_PROPERTIES___]<<endl;
		cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___EXCHANGEFRONTIERINFOS___: "<<_NUMCLOCKCYCLES_[0][___CODE___EXCHANGEFRONTIERINFOS___]<<endl;
		
		cout<<"acts_kernel SUMMARY:: number of edges processed: "<<(_NUMCLOCKCYCLES_[0][___CODE___ECPROCESSEDGES___] * EDGE_PACK_SIZE) / (1+1)<<endl;
		cout<<"acts_kernel SUMMARY:: number of vertex updates applied: "<<(_NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATES___] * EDGE_PACK_SIZE) / (1+4)<<endl;
		}
		#endif 
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
		+ _NUMCLOCKCYCLES_[0][___CODE___EXCHANGEFRONTIERINFOS___];
		float time_lapse = (total_cycles_iter * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000;
		cout<<"acts_kernel SUMMARY:: number of clock cycles seen: "<<total_cycles_iter<<" ("<<time_lapse<<" milliseconds)"<<endl;
		float time_lapse1 = (total_cycles_iter / 1000000) * _NUMNANOSECONDS_PER_CLOCKCYCLE_;
		float time_lapse2 = (float)((total_cycles_iter * EDGE_PACK_SIZE) / 1000) / (float)ACTS_AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC;
		float million_edges_per_sec = ((globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES] / 1000000) / time_lapse) * 1000;
		float billion_edges_per_sec = million_edges_per_sec / 1000;
		cout<<"acts_kernel SUMMARY:: time elapsed for iteration "<<GraphIter<<": "<<time_lapse<<" milliseconds)"<<endl;
		for(unsigned int t=0; t<16; t++){ total_cycles += _NUMCLOCKCYCLES_[0][t]; }
		for(unsigned int t=0; t<16; t++){ _NUMCLOCKCYCLES_[0][t] = 0; }
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS4
		for(unsigned int t=0; t<__NUM_UPARTITIONS; t++){ if(hybrid_map[GraphIter][t] == 0){ cout<<"V, "; } else if(hybrid_map[GraphIter][t] == 1){ cout<<"E, "; } else { cout<<"-, "; }} cout<<endl;
		cout<<""<<endl;
		if(run_vertex_centric == true){ cout<< TIMINGRESULTSCOLOR <<"### Vertex-Centric GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<< RESET << endl; } else { cout<< TIMINGRESULTSCOLOR <<"### Edge-Centric GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<< RESET << endl; }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ if(upartition_vertices[p_u].count > 0){ cout<<"1, "; } else { cout<<"0, "; }} cout<<endl;
		if(totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS){ cout<<"no more active vertices to process. breaking out... "<<endl; }
		#endif 
		if(totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS){ maxGraphIter = GraphIter; break; }
		for(unsigned int inst=0; inst<VALID_NUMPEs; inst++){ for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ nfrontier_dram___size[inst][p_u] = 0; }}
	} // NUM_ITs

	#ifdef _DEBUGMODE_CHECKS3
	float time_lapse = (total_cycles * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000;
	float million_edges_per_sec = ((globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES] / 1000000) / time_lapse) * 1000;
	float billion_edges_per_sec = million_edges_per_sec / 1000;
	cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen: "<<total_cycles<<" ("<<(total_cycles * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000<<" milliseconds)"<<endl;
	cout<< TIMINGRESULTSCOLOR << ">>> acts_kernel SUMMARY:: TIME ELAPSE: "<<time_lapse<<" ms. THROUGHPUT: "<<million_edges_per_sec<<" MTEPS; THROUGHPUT: "<<billion_edges_per_sec<<" BTEPS"<< RESET << endl;
	#endif
	
	// #endif 
	return;
}
}