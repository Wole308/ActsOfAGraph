#include "../include/common.h"
#ifndef FPGA_IMPL
#include "acts_async_kernel.h"
using namespace std;
#endif 

// ___ENABLE___ACTS___
// ___ENABLE___TEST3___

#define RUN_TEST_KERNEL

#ifdef RUN_TEST_KERNEL	
#define ___ENABLE___RESETBUFFERSATSTART___ 
#define ___ENABLE___CLEAR_COUNTERS___
#define ___ENABLE___PROCESSEDGES___ 
#define ___ENABLE___READ_FRONTIER_PROPERTIES___
#define ___ENABLE___RESET_BUFFERS___
// #define ___ENABLE___VCPROCESSEDGES___ 
#define ___ENABLE___ECPROCESSEDGES___II1___ 
// #define ___ENABLE___SAVEVCUPDATES___ 
// // // // // // // // // // // // // // // // // // #define ___ENABLE___COLLECTACTIVEDSTVIDS___ 
#define ___ENABLE___APPLYUPDATESMODULE___ 
	#define ___ENABLE___READ_DEST_PROPERTIES___ 
	#define ___ENABLE___APPLYUPDATES___II1___
	#define ___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___ 
	#define ___ENABLE___SAVE_DEST_PROPERTIES___ 
#define ___ENABLE___EXCHANGEFRONTIERINFOS___ 
#define ___ENABLE___REPORT_STATISTICS___

#define ___CODE___RESETBUFFERSATSTART___ 0
// #define ___CODE___NUMBER_OF_EDGEUPDATES_LOADED___ 1
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
#define ___CODE___NUMBER_OF_EDGEUPDATES_LOADED___ 13
#define ___CODE___NUMBER_OF_EDGEUPDATES_APPLIED___ 14
#define ___CODE___NUMBER_OF_NEW_EDGEUPDATES_ADDED___ 15

#define MAXVALID_APPLYPARTITION_VECSIZE (MAX_APPLYPARTITION_VECSIZE >> POW_VALID_VDATA)

#ifdef FPGA_IMPL
#define MY_IFDEF_NFRONTIER() keyvalue_t nfrontier_buffer[EDGE_PACK_SIZE][MAX_ACTVV_VECSIZE]
#define MY_IFDEF_CFRONTIER_TMP() keyvalue_t cfrontier_buffer_tmp[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_CFRONTIER() keyvalue_t cfrontier_buffer[NUM_VALID_PEs][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_STATSBUFFERMASKBASED_TMP() unsigned int statsbuffer_maskbased_tmp[EDGE_PACK_SIZE][BLOCKRAM_SIZE]	
#define MY_IFDEF_STATSBUFFERMASKBASED() unsigned int statsbuffer_maskbased[MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE]	
#define MY_IFDEF_STATSBUFFERIDBASED() unsigned int statsbuffer_idbased[MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE]	
#define MY_IFDEF_UPDATESTMPBUFFER() keyvalue_t updates_buffer2[UPDATES_BUFFER_PACK_SIZE][UPDATES_BUFFER_SIZE]	
#define MY_IFDEF_VPTRBUFFER() vtr_t vptr_buffer[VPTR_BUFFER_SIZE]
#define MY_IFDEF_EDGESBUFFER() edge3_type edges_buffer[EDGE_PACK_SIZE][EDGE_BUFFER_SIZE]
#define MY_IFDEF_EDGEUPDATESBUFFER() edge_update_type edgeupdates_buffer[EDGE_PACK_SIZE][EDGE_UPDATES_BUFFER_SIZE]
#define MY_IFDEF_VDATABUFFER() vprop_t vdata_buffer[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]
#define MY_IFDEF_NEXTLEVELFUNC() void next_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, unsigned int batch)
#define MY_IFDEF_TOPLEVELFUNC() void top_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, unsigned int batch)
#else
#define MY_IFDEF_NFRONTIER() keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_CFRONTIER_TMP() keyvalue_t * cfrontier_buffer_tmp[EDGE_PACK_SIZE]
#define MY_IFDEF_CFRONTIER() keyvalue_t * cfrontier_buffer[NUM_VALID_PEs]
#define MY_IFDEF_STATSBUFFERMASKBASED_TMP() unsigned int * statsbuffer_maskbased_tmp[EDGE_PACK_SIZE]
#define MY_IFDEF_STATSBUFFERMASKBASED() unsigned int * statsbuffer_maskbased[MAX_NUM_APPLYPARTITIONS]
#define MY_IFDEF_STATSBUFFERIDBASED() unsigned int * statsbuffer_idbased[MAX_NUM_APPLYPARTITIONS]
#define MY_IFDEF_UPDATESTMPBUFFER() keyvalue_t * updates_buffer2[UPDATES_BUFFER_PACK_SIZE]
#define MY_IFDEF_VPTRBUFFER() vtr_t * vptr_buffer
#define MY_IFDEF_EDGESBUFFER() edge3_type * edges_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_EDGEUPDATESBUFFER() edge_update_type * edgeupdates_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_VDATABUFFER() vprop_t * vdata_buffer[EDGE_PACK_SIZE]
#define MY_IFDEF_NEXTLEVELFUNC() void acts_kernel::next_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, unsigned int batch)
#define MY_IFDEF_TOPLEVELFUNC() void acts_kernel::top_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, unsigned int batch)
#endif

#define APPLY_CMD 2
#define TRANSPORT_CMD 3

#ifndef FPGA_IMPL
unsigned int * globalparams_debug;
unsigned int _NUMCLOCKCYCLES_[2][32]; 
#endif 

#ifndef FPGA_IMPL
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
#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
unsigned int hash_edge(unsigned int srcvid, unsigned int dstvid, unsigned int index){
	// return ((srcvid % EDGE_UPDATES_BUFFER_SIZE) + (dstvid % EDGE_UPDATES_BUFFER_SIZE)) / 2;
	return index; // REMOVEME.
}
#endif 

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

value_t process_func_cc(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata; // udata + edgew;
	return res;
}
value_t reduce_func_cc(value_t vtemp, value_t vdata, value_t res){
	#pragma HLS INLINE 
	if(res < vtemp){ return res; } else { return vtemp; }
}

value_t process_funcG(value_t udata, value_t edgew, unsigned int algo){
	#pragma HLS INLINE 
	// if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == PAGERANK){ return process_func_pr(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CF){ return process_func_cf(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == HITS){ return process_func_hits(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == BFS){ return process_func_bfs(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == SSSP){ return process_func_sssp(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CC){ return process_func_cc(udata, edgew); }
	// return process_func_bfs(udata, edgew);
	return process_func_sssp(udata, edgew);
	// return process_func_cc(udata, edgew);
}
value_t reduce_funcG(value_t vtemp, value_t vdata, value_t res, unsigned int algo){
	#pragma HLS INLINE 
	// if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == PAGERANK){ return reduce_func_pr(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CF){ return reduce_func_cf(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == HITS){ return reduce_func_hits(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == BFS){ return reduce_func_bfs(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == SSSP){ return reduce_func_sssp(udata, edgew); }
	// else if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CC){ return reduce_func_cc(udata, edgew); }
	// return reduce_func_bfs(vtemp, vdata, res);
	return reduce_func_sssp(vtemp, vdata, res);
	// return reduce_func_cc(vtemp, vdata, res);
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

/////////////////////////////////////////
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
	#pragma HLS INLINE // FIXME_HARDWARE
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

void master_insertmany_vec(unsigned int offsets[NUM_VALID_PEs], bool ens[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	
	
	HBM_channelA1[offsets[1]].data[0] = data[1][0];
	HBM_channelA1[offsets[1]].data[1] = data[1][1];
	HBM_channelA1[offsets[1]].data[2] = data[1][2];
	HBM_channelA1[offsets[1]].data[3] = data[1][3];
	HBM_channelA1[offsets[1]].data[4] = data[1][4];
	HBM_channelA1[offsets[1]].data[5] = data[1][5];
	HBM_channelA1[offsets[1]].data[6] = data[1][6];
	HBM_channelA1[offsets[1]].data[7] = data[1][7];
	HBM_channelA1[offsets[1]].data[8] = data[1][8];
	HBM_channelA1[offsets[1]].data[9] = data[1][9];
	HBM_channelA1[offsets[1]].data[10] = data[1][10];
	HBM_channelA1[offsets[1]].data[11] = data[1][11];
	HBM_channelA1[offsets[1]].data[12] = data[1][12];
	HBM_channelA1[offsets[1]].data[13] = data[1][13];
	HBM_channelA1[offsets[1]].data[14] = data[1][14];
	HBM_channelA1[offsets[1]].data[15] = data[1][15];
	HBM_channelB1[offsets[1]].data[0] = data[1][16];
	HBM_channelB1[offsets[1]].data[1] = data[1][17];
	HBM_channelB1[offsets[1]].data[2] = data[1][18];
	HBM_channelB1[offsets[1]].data[3] = data[1][19];
	HBM_channelB1[offsets[1]].data[4] = data[1][20];
	HBM_channelB1[offsets[1]].data[5] = data[1][21];
	HBM_channelB1[offsets[1]].data[6] = data[1][22];
	HBM_channelB1[offsets[1]].data[7] = data[1][23];
	HBM_channelB1[offsets[1]].data[8] = data[1][24];
	HBM_channelB1[offsets[1]].data[9] = data[1][25];
	HBM_channelB1[offsets[1]].data[10] = data[1][26];
	HBM_channelB1[offsets[1]].data[11] = data[1][27];
	HBM_channelB1[offsets[1]].data[12] = data[1][28];
	HBM_channelB1[offsets[1]].data[13] = data[1][29];
	HBM_channelB1[offsets[1]].data[14] = data[1][30];
	HBM_channelB1[offsets[1]].data[15] = data[1][31];
	
	
	HBM_channelA2[offsets[2]].data[0] = data[2][0];
	HBM_channelA2[offsets[2]].data[1] = data[2][1];
	HBM_channelA2[offsets[2]].data[2] = data[2][2];
	HBM_channelA2[offsets[2]].data[3] = data[2][3];
	HBM_channelA2[offsets[2]].data[4] = data[2][4];
	HBM_channelA2[offsets[2]].data[5] = data[2][5];
	HBM_channelA2[offsets[2]].data[6] = data[2][6];
	HBM_channelA2[offsets[2]].data[7] = data[2][7];
	HBM_channelA2[offsets[2]].data[8] = data[2][8];
	HBM_channelA2[offsets[2]].data[9] = data[2][9];
	HBM_channelA2[offsets[2]].data[10] = data[2][10];
	HBM_channelA2[offsets[2]].data[11] = data[2][11];
	HBM_channelA2[offsets[2]].data[12] = data[2][12];
	HBM_channelA2[offsets[2]].data[13] = data[2][13];
	HBM_channelA2[offsets[2]].data[14] = data[2][14];
	HBM_channelA2[offsets[2]].data[15] = data[2][15];
	HBM_channelB2[offsets[2]].data[0] = data[2][16];
	HBM_channelB2[offsets[2]].data[1] = data[2][17];
	HBM_channelB2[offsets[2]].data[2] = data[2][18];
	HBM_channelB2[offsets[2]].data[3] = data[2][19];
	HBM_channelB2[offsets[2]].data[4] = data[2][20];
	HBM_channelB2[offsets[2]].data[5] = data[2][21];
	HBM_channelB2[offsets[2]].data[6] = data[2][22];
	HBM_channelB2[offsets[2]].data[7] = data[2][23];
	HBM_channelB2[offsets[2]].data[8] = data[2][24];
	HBM_channelB2[offsets[2]].data[9] = data[2][25];
	HBM_channelB2[offsets[2]].data[10] = data[2][26];
	HBM_channelB2[offsets[2]].data[11] = data[2][27];
	HBM_channelB2[offsets[2]].data[12] = data[2][28];
	HBM_channelB2[offsets[2]].data[13] = data[2][29];
	HBM_channelB2[offsets[2]].data[14] = data[2][30];
	HBM_channelB2[offsets[2]].data[15] = data[2][31];
	
	
	HBM_channelA3[offsets[3]].data[0] = data[3][0];
	HBM_channelA3[offsets[3]].data[1] = data[3][1];
	HBM_channelA3[offsets[3]].data[2] = data[3][2];
	HBM_channelA3[offsets[3]].data[3] = data[3][3];
	HBM_channelA3[offsets[3]].data[4] = data[3][4];
	HBM_channelA3[offsets[3]].data[5] = data[3][5];
	HBM_channelA3[offsets[3]].data[6] = data[3][6];
	HBM_channelA3[offsets[3]].data[7] = data[3][7];
	HBM_channelA3[offsets[3]].data[8] = data[3][8];
	HBM_channelA3[offsets[3]].data[9] = data[3][9];
	HBM_channelA3[offsets[3]].data[10] = data[3][10];
	HBM_channelA3[offsets[3]].data[11] = data[3][11];
	HBM_channelA3[offsets[3]].data[12] = data[3][12];
	HBM_channelA3[offsets[3]].data[13] = data[3][13];
	HBM_channelA3[offsets[3]].data[14] = data[3][14];
	HBM_channelA3[offsets[3]].data[15] = data[3][15];
	HBM_channelB3[offsets[3]].data[0] = data[3][16];
	HBM_channelB3[offsets[3]].data[1] = data[3][17];
	HBM_channelB3[offsets[3]].data[2] = data[3][18];
	HBM_channelB3[offsets[3]].data[3] = data[3][19];
	HBM_channelB3[offsets[3]].data[4] = data[3][20];
	HBM_channelB3[offsets[3]].data[5] = data[3][21];
	HBM_channelB3[offsets[3]].data[6] = data[3][22];
	HBM_channelB3[offsets[3]].data[7] = data[3][23];
	HBM_channelB3[offsets[3]].data[8] = data[3][24];
	HBM_channelB3[offsets[3]].data[9] = data[3][25];
	HBM_channelB3[offsets[3]].data[10] = data[3][26];
	HBM_channelB3[offsets[3]].data[11] = data[3][27];
	HBM_channelB3[offsets[3]].data[12] = data[3][28];
	HBM_channelB3[offsets[3]].data[13] = data[3][29];
	HBM_channelB3[offsets[3]].data[14] = data[3][30];
	HBM_channelB3[offsets[3]].data[15] = data[3][31];
	
	
	HBM_channelA4[offsets[4]].data[0] = data[4][0];
	HBM_channelA4[offsets[4]].data[1] = data[4][1];
	HBM_channelA4[offsets[4]].data[2] = data[4][2];
	HBM_channelA4[offsets[4]].data[3] = data[4][3];
	HBM_channelA4[offsets[4]].data[4] = data[4][4];
	HBM_channelA4[offsets[4]].data[5] = data[4][5];
	HBM_channelA4[offsets[4]].data[6] = data[4][6];
	HBM_channelA4[offsets[4]].data[7] = data[4][7];
	HBM_channelA4[offsets[4]].data[8] = data[4][8];
	HBM_channelA4[offsets[4]].data[9] = data[4][9];
	HBM_channelA4[offsets[4]].data[10] = data[4][10];
	HBM_channelA4[offsets[4]].data[11] = data[4][11];
	HBM_channelA4[offsets[4]].data[12] = data[4][12];
	HBM_channelA4[offsets[4]].data[13] = data[4][13];
	HBM_channelA4[offsets[4]].data[14] = data[4][14];
	HBM_channelA4[offsets[4]].data[15] = data[4][15];
	HBM_channelB4[offsets[4]].data[0] = data[4][16];
	HBM_channelB4[offsets[4]].data[1] = data[4][17];
	HBM_channelB4[offsets[4]].data[2] = data[4][18];
	HBM_channelB4[offsets[4]].data[3] = data[4][19];
	HBM_channelB4[offsets[4]].data[4] = data[4][20];
	HBM_channelB4[offsets[4]].data[5] = data[4][21];
	HBM_channelB4[offsets[4]].data[6] = data[4][22];
	HBM_channelB4[offsets[4]].data[7] = data[4][23];
	HBM_channelB4[offsets[4]].data[8] = data[4][24];
	HBM_channelB4[offsets[4]].data[9] = data[4][25];
	HBM_channelB4[offsets[4]].data[10] = data[4][26];
	HBM_channelB4[offsets[4]].data[11] = data[4][27];
	HBM_channelB4[offsets[4]].data[12] = data[4][28];
	HBM_channelB4[offsets[4]].data[13] = data[4][29];
	HBM_channelB4[offsets[4]].data[14] = data[4][30];
	HBM_channelB4[offsets[4]].data[15] = data[4][31];
	
	
	HBM_channelA5[offsets[5]].data[0] = data[5][0];
	HBM_channelA5[offsets[5]].data[1] = data[5][1];
	HBM_channelA5[offsets[5]].data[2] = data[5][2];
	HBM_channelA5[offsets[5]].data[3] = data[5][3];
	HBM_channelA5[offsets[5]].data[4] = data[5][4];
	HBM_channelA5[offsets[5]].data[5] = data[5][5];
	HBM_channelA5[offsets[5]].data[6] = data[5][6];
	HBM_channelA5[offsets[5]].data[7] = data[5][7];
	HBM_channelA5[offsets[5]].data[8] = data[5][8];
	HBM_channelA5[offsets[5]].data[9] = data[5][9];
	HBM_channelA5[offsets[5]].data[10] = data[5][10];
	HBM_channelA5[offsets[5]].data[11] = data[5][11];
	HBM_channelA5[offsets[5]].data[12] = data[5][12];
	HBM_channelA5[offsets[5]].data[13] = data[5][13];
	HBM_channelA5[offsets[5]].data[14] = data[5][14];
	HBM_channelA5[offsets[5]].data[15] = data[5][15];
	HBM_channelB5[offsets[5]].data[0] = data[5][16];
	HBM_channelB5[offsets[5]].data[1] = data[5][17];
	HBM_channelB5[offsets[5]].data[2] = data[5][18];
	HBM_channelB5[offsets[5]].data[3] = data[5][19];
	HBM_channelB5[offsets[5]].data[4] = data[5][20];
	HBM_channelB5[offsets[5]].data[5] = data[5][21];
	HBM_channelB5[offsets[5]].data[6] = data[5][22];
	HBM_channelB5[offsets[5]].data[7] = data[5][23];
	HBM_channelB5[offsets[5]].data[8] = data[5][24];
	HBM_channelB5[offsets[5]].data[9] = data[5][25];
	HBM_channelB5[offsets[5]].data[10] = data[5][26];
	HBM_channelB5[offsets[5]].data[11] = data[5][27];
	HBM_channelB5[offsets[5]].data[12] = data[5][28];
	HBM_channelB5[offsets[5]].data[13] = data[5][29];
	HBM_channelB5[offsets[5]].data[14] = data[5][30];
	HBM_channelB5[offsets[5]].data[15] = data[5][31];
	
	return;
}
void master_retrievemany_vec(unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	data[1][0] = HBM_channelA1[offsets[1]].data[0];
	data[1][1] = HBM_channelA1[offsets[1]].data[1];
	data[1][2] = HBM_channelA1[offsets[1]].data[2];
	data[1][3] = HBM_channelA1[offsets[1]].data[3];
	data[1][4] = HBM_channelA1[offsets[1]].data[4];
	data[1][5] = HBM_channelA1[offsets[1]].data[5];
	data[1][6] = HBM_channelA1[offsets[1]].data[6];
	data[1][7] = HBM_channelA1[offsets[1]].data[7];
	data[1][8] = HBM_channelA1[offsets[1]].data[8];
	data[1][9] = HBM_channelA1[offsets[1]].data[9];
	data[1][10] = HBM_channelA1[offsets[1]].data[10];
	data[1][11] = HBM_channelA1[offsets[1]].data[11];
	data[1][12] = HBM_channelA1[offsets[1]].data[12];
	data[1][13] = HBM_channelA1[offsets[1]].data[13];
	data[1][14] = HBM_channelA1[offsets[1]].data[14];
	data[1][15] = HBM_channelA1[offsets[1]].data[15];
	data[1][16] = HBM_channelB1[offsets[1]].data[0];
	data[1][17] = HBM_channelB1[offsets[1]].data[1];
	data[1][18] = HBM_channelB1[offsets[1]].data[2];
	data[1][19] = HBM_channelB1[offsets[1]].data[3];
	data[1][20] = HBM_channelB1[offsets[1]].data[4];
	data[1][21] = HBM_channelB1[offsets[1]].data[5];
	data[1][22] = HBM_channelB1[offsets[1]].data[6];
	data[1][23] = HBM_channelB1[offsets[1]].data[7];
	data[1][24] = HBM_channelB1[offsets[1]].data[8];
	data[1][25] = HBM_channelB1[offsets[1]].data[9];
	data[1][26] = HBM_channelB1[offsets[1]].data[10];
	data[1][27] = HBM_channelB1[offsets[1]].data[11];
	data[1][28] = HBM_channelB1[offsets[1]].data[12];
	data[1][29] = HBM_channelB1[offsets[1]].data[13];
	data[1][30] = HBM_channelB1[offsets[1]].data[14];
	data[1][31] = HBM_channelB1[offsets[1]].data[15];
	data[2][0] = HBM_channelA2[offsets[2]].data[0];
	data[2][1] = HBM_channelA2[offsets[2]].data[1];
	data[2][2] = HBM_channelA2[offsets[2]].data[2];
	data[2][3] = HBM_channelA2[offsets[2]].data[3];
	data[2][4] = HBM_channelA2[offsets[2]].data[4];
	data[2][5] = HBM_channelA2[offsets[2]].data[5];
	data[2][6] = HBM_channelA2[offsets[2]].data[6];
	data[2][7] = HBM_channelA2[offsets[2]].data[7];
	data[2][8] = HBM_channelA2[offsets[2]].data[8];
	data[2][9] = HBM_channelA2[offsets[2]].data[9];
	data[2][10] = HBM_channelA2[offsets[2]].data[10];
	data[2][11] = HBM_channelA2[offsets[2]].data[11];
	data[2][12] = HBM_channelA2[offsets[2]].data[12];
	data[2][13] = HBM_channelA2[offsets[2]].data[13];
	data[2][14] = HBM_channelA2[offsets[2]].data[14];
	data[2][15] = HBM_channelA2[offsets[2]].data[15];
	data[2][16] = HBM_channelB2[offsets[2]].data[0];
	data[2][17] = HBM_channelB2[offsets[2]].data[1];
	data[2][18] = HBM_channelB2[offsets[2]].data[2];
	data[2][19] = HBM_channelB2[offsets[2]].data[3];
	data[2][20] = HBM_channelB2[offsets[2]].data[4];
	data[2][21] = HBM_channelB2[offsets[2]].data[5];
	data[2][22] = HBM_channelB2[offsets[2]].data[6];
	data[2][23] = HBM_channelB2[offsets[2]].data[7];
	data[2][24] = HBM_channelB2[offsets[2]].data[8];
	data[2][25] = HBM_channelB2[offsets[2]].data[9];
	data[2][26] = HBM_channelB2[offsets[2]].data[10];
	data[2][27] = HBM_channelB2[offsets[2]].data[11];
	data[2][28] = HBM_channelB2[offsets[2]].data[12];
	data[2][29] = HBM_channelB2[offsets[2]].data[13];
	data[2][30] = HBM_channelB2[offsets[2]].data[14];
	data[2][31] = HBM_channelB2[offsets[2]].data[15];
	data[3][0] = HBM_channelA3[offsets[3]].data[0];
	data[3][1] = HBM_channelA3[offsets[3]].data[1];
	data[3][2] = HBM_channelA3[offsets[3]].data[2];
	data[3][3] = HBM_channelA3[offsets[3]].data[3];
	data[3][4] = HBM_channelA3[offsets[3]].data[4];
	data[3][5] = HBM_channelA3[offsets[3]].data[5];
	data[3][6] = HBM_channelA3[offsets[3]].data[6];
	data[3][7] = HBM_channelA3[offsets[3]].data[7];
	data[3][8] = HBM_channelA3[offsets[3]].data[8];
	data[3][9] = HBM_channelA3[offsets[3]].data[9];
	data[3][10] = HBM_channelA3[offsets[3]].data[10];
	data[3][11] = HBM_channelA3[offsets[3]].data[11];
	data[3][12] = HBM_channelA3[offsets[3]].data[12];
	data[3][13] = HBM_channelA3[offsets[3]].data[13];
	data[3][14] = HBM_channelA3[offsets[3]].data[14];
	data[3][15] = HBM_channelA3[offsets[3]].data[15];
	data[3][16] = HBM_channelB3[offsets[3]].data[0];
	data[3][17] = HBM_channelB3[offsets[3]].data[1];
	data[3][18] = HBM_channelB3[offsets[3]].data[2];
	data[3][19] = HBM_channelB3[offsets[3]].data[3];
	data[3][20] = HBM_channelB3[offsets[3]].data[4];
	data[3][21] = HBM_channelB3[offsets[3]].data[5];
	data[3][22] = HBM_channelB3[offsets[3]].data[6];
	data[3][23] = HBM_channelB3[offsets[3]].data[7];
	data[3][24] = HBM_channelB3[offsets[3]].data[8];
	data[3][25] = HBM_channelB3[offsets[3]].data[9];
	data[3][26] = HBM_channelB3[offsets[3]].data[10];
	data[3][27] = HBM_channelB3[offsets[3]].data[11];
	data[3][28] = HBM_channelB3[offsets[3]].data[12];
	data[3][29] = HBM_channelB3[offsets[3]].data[13];
	data[3][30] = HBM_channelB3[offsets[3]].data[14];
	data[3][31] = HBM_channelB3[offsets[3]].data[15];
	data[4][0] = HBM_channelA4[offsets[4]].data[0];
	data[4][1] = HBM_channelA4[offsets[4]].data[1];
	data[4][2] = HBM_channelA4[offsets[4]].data[2];
	data[4][3] = HBM_channelA4[offsets[4]].data[3];
	data[4][4] = HBM_channelA4[offsets[4]].data[4];
	data[4][5] = HBM_channelA4[offsets[4]].data[5];
	data[4][6] = HBM_channelA4[offsets[4]].data[6];
	data[4][7] = HBM_channelA4[offsets[4]].data[7];
	data[4][8] = HBM_channelA4[offsets[4]].data[8];
	data[4][9] = HBM_channelA4[offsets[4]].data[9];
	data[4][10] = HBM_channelA4[offsets[4]].data[10];
	data[4][11] = HBM_channelA4[offsets[4]].data[11];
	data[4][12] = HBM_channelA4[offsets[4]].data[12];
	data[4][13] = HBM_channelA4[offsets[4]].data[13];
	data[4][14] = HBM_channelA4[offsets[4]].data[14];
	data[4][15] = HBM_channelA4[offsets[4]].data[15];
	data[4][16] = HBM_channelB4[offsets[4]].data[0];
	data[4][17] = HBM_channelB4[offsets[4]].data[1];
	data[4][18] = HBM_channelB4[offsets[4]].data[2];
	data[4][19] = HBM_channelB4[offsets[4]].data[3];
	data[4][20] = HBM_channelB4[offsets[4]].data[4];
	data[4][21] = HBM_channelB4[offsets[4]].data[5];
	data[4][22] = HBM_channelB4[offsets[4]].data[6];
	data[4][23] = HBM_channelB4[offsets[4]].data[7];
	data[4][24] = HBM_channelB4[offsets[4]].data[8];
	data[4][25] = HBM_channelB4[offsets[4]].data[9];
	data[4][26] = HBM_channelB4[offsets[4]].data[10];
	data[4][27] = HBM_channelB4[offsets[4]].data[11];
	data[4][28] = HBM_channelB4[offsets[4]].data[12];
	data[4][29] = HBM_channelB4[offsets[4]].data[13];
	data[4][30] = HBM_channelB4[offsets[4]].data[14];
	data[4][31] = HBM_channelB4[offsets[4]].data[15];
	data[5][0] = HBM_channelA5[offsets[5]].data[0];
	data[5][1] = HBM_channelA5[offsets[5]].data[1];
	data[5][2] = HBM_channelA5[offsets[5]].data[2];
	data[5][3] = HBM_channelA5[offsets[5]].data[3];
	data[5][4] = HBM_channelA5[offsets[5]].data[4];
	data[5][5] = HBM_channelA5[offsets[5]].data[5];
	data[5][6] = HBM_channelA5[offsets[5]].data[6];
	data[5][7] = HBM_channelA5[offsets[5]].data[7];
	data[5][8] = HBM_channelA5[offsets[5]].data[8];
	data[5][9] = HBM_channelA5[offsets[5]].data[9];
	data[5][10] = HBM_channelA5[offsets[5]].data[10];
	data[5][11] = HBM_channelA5[offsets[5]].data[11];
	data[5][12] = HBM_channelA5[offsets[5]].data[12];
	data[5][13] = HBM_channelA5[offsets[5]].data[13];
	data[5][14] = HBM_channelA5[offsets[5]].data[14];
	data[5][15] = HBM_channelA5[offsets[5]].data[15];
	data[5][16] = HBM_channelB5[offsets[5]].data[0];
	data[5][17] = HBM_channelB5[offsets[5]].data[1];
	data[5][18] = HBM_channelB5[offsets[5]].data[2];
	data[5][19] = HBM_channelB5[offsets[5]].data[3];
	data[5][20] = HBM_channelB5[offsets[5]].data[4];
	data[5][21] = HBM_channelB5[offsets[5]].data[5];
	data[5][22] = HBM_channelB5[offsets[5]].data[6];
	data[5][23] = HBM_channelB5[offsets[5]].data[7];
	data[5][24] = HBM_channelB5[offsets[5]].data[8];
	data[5][25] = HBM_channelB5[offsets[5]].data[9];
	data[5][26] = HBM_channelB5[offsets[5]].data[10];
	data[5][27] = HBM_channelB5[offsets[5]].data[11];
	data[5][28] = HBM_channelB5[offsets[5]].data[12];
	data[5][29] = HBM_channelB5[offsets[5]].data[13];
	data[5][30] = HBM_channelB5[offsets[5]].data[14];
	data[5][31] = HBM_channelB5[offsets[5]].data[15];
	return;
}

void master_insertmany_Bvec(unsigned int offsets[NUM_VALID_PEs], bool ens[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	
	if(ens[1]==true){
		HBM_channelB1[offsets[1]].data[0] = data[1][16];
		HBM_channelB1[offsets[1]].data[1] = data[1][17];
		HBM_channelB1[offsets[1]].data[2] = data[1][18];
		HBM_channelB1[offsets[1]].data[3] = data[1][19];
		HBM_channelB1[offsets[1]].data[4] = data[1][20];
		HBM_channelB1[offsets[1]].data[5] = data[1][21];
		HBM_channelB1[offsets[1]].data[6] = data[1][22];
		HBM_channelB1[offsets[1]].data[7] = data[1][23];
		HBM_channelB1[offsets[1]].data[8] = data[1][24];
		HBM_channelB1[offsets[1]].data[9] = data[1][25];
		HBM_channelB1[offsets[1]].data[10] = data[1][26];
		HBM_channelB1[offsets[1]].data[11] = data[1][27];
		HBM_channelB1[offsets[1]].data[12] = data[1][28];
		HBM_channelB1[offsets[1]].data[13] = data[1][29];
		HBM_channelB1[offsets[1]].data[14] = data[1][30];
		HBM_channelB1[offsets[1]].data[15] = data[1][31];
	
	}
	
	if(ens[2]==true){
		HBM_channelB2[offsets[2]].data[0] = data[2][16];
		HBM_channelB2[offsets[2]].data[1] = data[2][17];
		HBM_channelB2[offsets[2]].data[2] = data[2][18];
		HBM_channelB2[offsets[2]].data[3] = data[2][19];
		HBM_channelB2[offsets[2]].data[4] = data[2][20];
		HBM_channelB2[offsets[2]].data[5] = data[2][21];
		HBM_channelB2[offsets[2]].data[6] = data[2][22];
		HBM_channelB2[offsets[2]].data[7] = data[2][23];
		HBM_channelB2[offsets[2]].data[8] = data[2][24];
		HBM_channelB2[offsets[2]].data[9] = data[2][25];
		HBM_channelB2[offsets[2]].data[10] = data[2][26];
		HBM_channelB2[offsets[2]].data[11] = data[2][27];
		HBM_channelB2[offsets[2]].data[12] = data[2][28];
		HBM_channelB2[offsets[2]].data[13] = data[2][29];
		HBM_channelB2[offsets[2]].data[14] = data[2][30];
		HBM_channelB2[offsets[2]].data[15] = data[2][31];
	
	}
	
	if(ens[3]==true){
		HBM_channelB3[offsets[3]].data[0] = data[3][16];
		HBM_channelB3[offsets[3]].data[1] = data[3][17];
		HBM_channelB3[offsets[3]].data[2] = data[3][18];
		HBM_channelB3[offsets[3]].data[3] = data[3][19];
		HBM_channelB3[offsets[3]].data[4] = data[3][20];
		HBM_channelB3[offsets[3]].data[5] = data[3][21];
		HBM_channelB3[offsets[3]].data[6] = data[3][22];
		HBM_channelB3[offsets[3]].data[7] = data[3][23];
		HBM_channelB3[offsets[3]].data[8] = data[3][24];
		HBM_channelB3[offsets[3]].data[9] = data[3][25];
		HBM_channelB3[offsets[3]].data[10] = data[3][26];
		HBM_channelB3[offsets[3]].data[11] = data[3][27];
		HBM_channelB3[offsets[3]].data[12] = data[3][28];
		HBM_channelB3[offsets[3]].data[13] = data[3][29];
		HBM_channelB3[offsets[3]].data[14] = data[3][30];
		HBM_channelB3[offsets[3]].data[15] = data[3][31];
	
	}
	
	if(ens[4]==true){
		HBM_channelB4[offsets[4]].data[0] = data[4][16];
		HBM_channelB4[offsets[4]].data[1] = data[4][17];
		HBM_channelB4[offsets[4]].data[2] = data[4][18];
		HBM_channelB4[offsets[4]].data[3] = data[4][19];
		HBM_channelB4[offsets[4]].data[4] = data[4][20];
		HBM_channelB4[offsets[4]].data[5] = data[4][21];
		HBM_channelB4[offsets[4]].data[6] = data[4][22];
		HBM_channelB4[offsets[4]].data[7] = data[4][23];
		HBM_channelB4[offsets[4]].data[8] = data[4][24];
		HBM_channelB4[offsets[4]].data[9] = data[4][25];
		HBM_channelB4[offsets[4]].data[10] = data[4][26];
		HBM_channelB4[offsets[4]].data[11] = data[4][27];
		HBM_channelB4[offsets[4]].data[12] = data[4][28];
		HBM_channelB4[offsets[4]].data[13] = data[4][29];
		HBM_channelB4[offsets[4]].data[14] = data[4][30];
		HBM_channelB4[offsets[4]].data[15] = data[4][31];
	
	}
	
	if(ens[5]==true){
		HBM_channelB5[offsets[5]].data[0] = data[5][16];
		HBM_channelB5[offsets[5]].data[1] = data[5][17];
		HBM_channelB5[offsets[5]].data[2] = data[5][18];
		HBM_channelB5[offsets[5]].data[3] = data[5][19];
		HBM_channelB5[offsets[5]].data[4] = data[5][20];
		HBM_channelB5[offsets[5]].data[5] = data[5][21];
		HBM_channelB5[offsets[5]].data[6] = data[5][22];
		HBM_channelB5[offsets[5]].data[7] = data[5][23];
		HBM_channelB5[offsets[5]].data[8] = data[5][24];
		HBM_channelB5[offsets[5]].data[9] = data[5][25];
		HBM_channelB5[offsets[5]].data[10] = data[5][26];
		HBM_channelB5[offsets[5]].data[11] = data[5][27];
		HBM_channelB5[offsets[5]].data[12] = data[5][28];
		HBM_channelB5[offsets[5]].data[13] = data[5][29];
		HBM_channelB5[offsets[5]].data[14] = data[5][30];
		HBM_channelB5[offsets[5]].data[15] = data[5][31];
	
	}
	return;
}
#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
void master_insertmany_Cvec(unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
	#pragma HLS INLINE 
	
		HBM_channelA0[offsets[0]].data[0] = data[0][16];	// FIXME
		// HBM_channelC0[offsets[0]].data[0] = data[0][16];	
		HBM_channelA0[offsets[0]].data[1] = data[0][17];	// FIXME
		// HBM_channelC0[offsets[0]].data[1] = data[0][17];	
		HBM_channelA0[offsets[0]].data[2] = data[0][18];	// FIXME
		// HBM_channelC0[offsets[0]].data[2] = data[0][18];	
		HBM_channelA0[offsets[0]].data[3] = data[0][19];	// FIXME
		// HBM_channelC0[offsets[0]].data[3] = data[0][19];	
		HBM_channelA0[offsets[0]].data[4] = data[0][20];	// FIXME
		// HBM_channelC0[offsets[0]].data[4] = data[0][20];	
		HBM_channelA0[offsets[0]].data[5] = data[0][21];	// FIXME
		// HBM_channelC0[offsets[0]].data[5] = data[0][21];	
		HBM_channelA0[offsets[0]].data[6] = data[0][22];	// FIXME
		// HBM_channelC0[offsets[0]].data[6] = data[0][22];	
		HBM_channelA0[offsets[0]].data[7] = data[0][23];	// FIXME
		// HBM_channelC0[offsets[0]].data[7] = data[0][23];	
		HBM_channelA0[offsets[0]].data[8] = data[0][24];	// FIXME
		// HBM_channelC0[offsets[0]].data[8] = data[0][24];	
		HBM_channelA0[offsets[0]].data[9] = data[0][25];	// FIXME
		// HBM_channelC0[offsets[0]].data[9] = data[0][25];	
		HBM_channelA0[offsets[0]].data[10] = data[0][26];	// FIXME
		// HBM_channelC0[offsets[0]].data[10] = data[0][26];	
		HBM_channelA0[offsets[0]].data[11] = data[0][27];	// FIXME
		// HBM_channelC0[offsets[0]].data[11] = data[0][27];	
		HBM_channelA0[offsets[0]].data[12] = data[0][28];	// FIXME
		// HBM_channelC0[offsets[0]].data[12] = data[0][28];	
		HBM_channelA0[offsets[0]].data[13] = data[0][29];	// FIXME
		// HBM_channelC0[offsets[0]].data[13] = data[0][29];	
		HBM_channelA0[offsets[0]].data[14] = data[0][30];	// FIXME
		// HBM_channelC0[offsets[0]].data[14] = data[0][30];	
		HBM_channelA0[offsets[0]].data[15] = data[0][31];	// FIXME
		// HBM_channelC0[offsets[0]].data[15] = data[0][31];	
	
	
		HBM_channelA1[offsets[1]].data[0] = data[1][16];	// FIXME
		// HBM_channelC1[offsets[1]].data[0] = data[1][16];	
		HBM_channelA1[offsets[1]].data[1] = data[1][17];	// FIXME
		// HBM_channelC1[offsets[1]].data[1] = data[1][17];	
		HBM_channelA1[offsets[1]].data[2] = data[1][18];	// FIXME
		// HBM_channelC1[offsets[1]].data[2] = data[1][18];	
		HBM_channelA1[offsets[1]].data[3] = data[1][19];	// FIXME
		// HBM_channelC1[offsets[1]].data[3] = data[1][19];	
		HBM_channelA1[offsets[1]].data[4] = data[1][20];	// FIXME
		// HBM_channelC1[offsets[1]].data[4] = data[1][20];	
		HBM_channelA1[offsets[1]].data[5] = data[1][21];	// FIXME
		// HBM_channelC1[offsets[1]].data[5] = data[1][21];	
		HBM_channelA1[offsets[1]].data[6] = data[1][22];	// FIXME
		// HBM_channelC1[offsets[1]].data[6] = data[1][22];	
		HBM_channelA1[offsets[1]].data[7] = data[1][23];	// FIXME
		// HBM_channelC1[offsets[1]].data[7] = data[1][23];	
		HBM_channelA1[offsets[1]].data[8] = data[1][24];	// FIXME
		// HBM_channelC1[offsets[1]].data[8] = data[1][24];	
		HBM_channelA1[offsets[1]].data[9] = data[1][25];	// FIXME
		// HBM_channelC1[offsets[1]].data[9] = data[1][25];	
		HBM_channelA1[offsets[1]].data[10] = data[1][26];	// FIXME
		// HBM_channelC1[offsets[1]].data[10] = data[1][26];	
		HBM_channelA1[offsets[1]].data[11] = data[1][27];	// FIXME
		// HBM_channelC1[offsets[1]].data[11] = data[1][27];	
		HBM_channelA1[offsets[1]].data[12] = data[1][28];	// FIXME
		// HBM_channelC1[offsets[1]].data[12] = data[1][28];	
		HBM_channelA1[offsets[1]].data[13] = data[1][29];	// FIXME
		// HBM_channelC1[offsets[1]].data[13] = data[1][29];	
		HBM_channelA1[offsets[1]].data[14] = data[1][30];	// FIXME
		// HBM_channelC1[offsets[1]].data[14] = data[1][30];	
		HBM_channelA1[offsets[1]].data[15] = data[1][31];	// FIXME
		// HBM_channelC1[offsets[1]].data[15] = data[1][31];	
	
	
		HBM_channelA2[offsets[2]].data[0] = data[2][16];	// FIXME
		// HBM_channelC2[offsets[2]].data[0] = data[2][16];	
		HBM_channelA2[offsets[2]].data[1] = data[2][17];	// FIXME
		// HBM_channelC2[offsets[2]].data[1] = data[2][17];	
		HBM_channelA2[offsets[2]].data[2] = data[2][18];	// FIXME
		// HBM_channelC2[offsets[2]].data[2] = data[2][18];	
		HBM_channelA2[offsets[2]].data[3] = data[2][19];	// FIXME
		// HBM_channelC2[offsets[2]].data[3] = data[2][19];	
		HBM_channelA2[offsets[2]].data[4] = data[2][20];	// FIXME
		// HBM_channelC2[offsets[2]].data[4] = data[2][20];	
		HBM_channelA2[offsets[2]].data[5] = data[2][21];	// FIXME
		// HBM_channelC2[offsets[2]].data[5] = data[2][21];	
		HBM_channelA2[offsets[2]].data[6] = data[2][22];	// FIXME
		// HBM_channelC2[offsets[2]].data[6] = data[2][22];	
		HBM_channelA2[offsets[2]].data[7] = data[2][23];	// FIXME
		// HBM_channelC2[offsets[2]].data[7] = data[2][23];	
		HBM_channelA2[offsets[2]].data[8] = data[2][24];	// FIXME
		// HBM_channelC2[offsets[2]].data[8] = data[2][24];	
		HBM_channelA2[offsets[2]].data[9] = data[2][25];	// FIXME
		// HBM_channelC2[offsets[2]].data[9] = data[2][25];	
		HBM_channelA2[offsets[2]].data[10] = data[2][26];	// FIXME
		// HBM_channelC2[offsets[2]].data[10] = data[2][26];	
		HBM_channelA2[offsets[2]].data[11] = data[2][27];	// FIXME
		// HBM_channelC2[offsets[2]].data[11] = data[2][27];	
		HBM_channelA2[offsets[2]].data[12] = data[2][28];	// FIXME
		// HBM_channelC2[offsets[2]].data[12] = data[2][28];	
		HBM_channelA2[offsets[2]].data[13] = data[2][29];	// FIXME
		// HBM_channelC2[offsets[2]].data[13] = data[2][29];	
		HBM_channelA2[offsets[2]].data[14] = data[2][30];	// FIXME
		// HBM_channelC2[offsets[2]].data[14] = data[2][30];	
		HBM_channelA2[offsets[2]].data[15] = data[2][31];	// FIXME
		// HBM_channelC2[offsets[2]].data[15] = data[2][31];	
	
	
		HBM_channelA3[offsets[3]].data[0] = data[3][16];	// FIXME
		// HBM_channelC3[offsets[3]].data[0] = data[3][16];	
		HBM_channelA3[offsets[3]].data[1] = data[3][17];	// FIXME
		// HBM_channelC3[offsets[3]].data[1] = data[3][17];	
		HBM_channelA3[offsets[3]].data[2] = data[3][18];	// FIXME
		// HBM_channelC3[offsets[3]].data[2] = data[3][18];	
		HBM_channelA3[offsets[3]].data[3] = data[3][19];	// FIXME
		// HBM_channelC3[offsets[3]].data[3] = data[3][19];	
		HBM_channelA3[offsets[3]].data[4] = data[3][20];	// FIXME
		// HBM_channelC3[offsets[3]].data[4] = data[3][20];	
		HBM_channelA3[offsets[3]].data[5] = data[3][21];	// FIXME
		// HBM_channelC3[offsets[3]].data[5] = data[3][21];	
		HBM_channelA3[offsets[3]].data[6] = data[3][22];	// FIXME
		// HBM_channelC3[offsets[3]].data[6] = data[3][22];	
		HBM_channelA3[offsets[3]].data[7] = data[3][23];	// FIXME
		// HBM_channelC3[offsets[3]].data[7] = data[3][23];	
		HBM_channelA3[offsets[3]].data[8] = data[3][24];	// FIXME
		// HBM_channelC3[offsets[3]].data[8] = data[3][24];	
		HBM_channelA3[offsets[3]].data[9] = data[3][25];	// FIXME
		// HBM_channelC3[offsets[3]].data[9] = data[3][25];	
		HBM_channelA3[offsets[3]].data[10] = data[3][26];	// FIXME
		// HBM_channelC3[offsets[3]].data[10] = data[3][26];	
		HBM_channelA3[offsets[3]].data[11] = data[3][27];	// FIXME
		// HBM_channelC3[offsets[3]].data[11] = data[3][27];	
		HBM_channelA3[offsets[3]].data[12] = data[3][28];	// FIXME
		// HBM_channelC3[offsets[3]].data[12] = data[3][28];	
		HBM_channelA3[offsets[3]].data[13] = data[3][29];	// FIXME
		// HBM_channelC3[offsets[3]].data[13] = data[3][29];	
		HBM_channelA3[offsets[3]].data[14] = data[3][30];	// FIXME
		// HBM_channelC3[offsets[3]].data[14] = data[3][30];	
		HBM_channelA3[offsets[3]].data[15] = data[3][31];	// FIXME
		// HBM_channelC3[offsets[3]].data[15] = data[3][31];	
	
	
		HBM_channelA4[offsets[4]].data[0] = data[4][16];	// FIXME
		// HBM_channelC4[offsets[4]].data[0] = data[4][16];	
		HBM_channelA4[offsets[4]].data[1] = data[4][17];	// FIXME
		// HBM_channelC4[offsets[4]].data[1] = data[4][17];	
		HBM_channelA4[offsets[4]].data[2] = data[4][18];	// FIXME
		// HBM_channelC4[offsets[4]].data[2] = data[4][18];	
		HBM_channelA4[offsets[4]].data[3] = data[4][19];	// FIXME
		// HBM_channelC4[offsets[4]].data[3] = data[4][19];	
		HBM_channelA4[offsets[4]].data[4] = data[4][20];	// FIXME
		// HBM_channelC4[offsets[4]].data[4] = data[4][20];	
		HBM_channelA4[offsets[4]].data[5] = data[4][21];	// FIXME
		// HBM_channelC4[offsets[4]].data[5] = data[4][21];	
		HBM_channelA4[offsets[4]].data[6] = data[4][22];	// FIXME
		// HBM_channelC4[offsets[4]].data[6] = data[4][22];	
		HBM_channelA4[offsets[4]].data[7] = data[4][23];	// FIXME
		// HBM_channelC4[offsets[4]].data[7] = data[4][23];	
		HBM_channelA4[offsets[4]].data[8] = data[4][24];	// FIXME
		// HBM_channelC4[offsets[4]].data[8] = data[4][24];	
		HBM_channelA4[offsets[4]].data[9] = data[4][25];	// FIXME
		// HBM_channelC4[offsets[4]].data[9] = data[4][25];	
		HBM_channelA4[offsets[4]].data[10] = data[4][26];	// FIXME
		// HBM_channelC4[offsets[4]].data[10] = data[4][26];	
		HBM_channelA4[offsets[4]].data[11] = data[4][27];	// FIXME
		// HBM_channelC4[offsets[4]].data[11] = data[4][27];	
		HBM_channelA4[offsets[4]].data[12] = data[4][28];	// FIXME
		// HBM_channelC4[offsets[4]].data[12] = data[4][28];	
		HBM_channelA4[offsets[4]].data[13] = data[4][29];	// FIXME
		// HBM_channelC4[offsets[4]].data[13] = data[4][29];	
		HBM_channelA4[offsets[4]].data[14] = data[4][30];	// FIXME
		// HBM_channelC4[offsets[4]].data[14] = data[4][30];	
		HBM_channelA4[offsets[4]].data[15] = data[4][31];	// FIXME
		// HBM_channelC4[offsets[4]].data[15] = data[4][31];	
	
	
		HBM_channelA5[offsets[5]].data[0] = data[5][16];	// FIXME
		// HBM_channelC5[offsets[5]].data[0] = data[5][16];	
		HBM_channelA5[offsets[5]].data[1] = data[5][17];	// FIXME
		// HBM_channelC5[offsets[5]].data[1] = data[5][17];	
		HBM_channelA5[offsets[5]].data[2] = data[5][18];	// FIXME
		// HBM_channelC5[offsets[5]].data[2] = data[5][18];	
		HBM_channelA5[offsets[5]].data[3] = data[5][19];	// FIXME
		// HBM_channelC5[offsets[5]].data[3] = data[5][19];	
		HBM_channelA5[offsets[5]].data[4] = data[5][20];	// FIXME
		// HBM_channelC5[offsets[5]].data[4] = data[5][20];	
		HBM_channelA5[offsets[5]].data[5] = data[5][21];	// FIXME
		// HBM_channelC5[offsets[5]].data[5] = data[5][21];	
		HBM_channelA5[offsets[5]].data[6] = data[5][22];	// FIXME
		// HBM_channelC5[offsets[5]].data[6] = data[5][22];	
		HBM_channelA5[offsets[5]].data[7] = data[5][23];	// FIXME
		// HBM_channelC5[offsets[5]].data[7] = data[5][23];	
		HBM_channelA5[offsets[5]].data[8] = data[5][24];	// FIXME
		// HBM_channelC5[offsets[5]].data[8] = data[5][24];	
		HBM_channelA5[offsets[5]].data[9] = data[5][25];	// FIXME
		// HBM_channelC5[offsets[5]].data[9] = data[5][25];	
		HBM_channelA5[offsets[5]].data[10] = data[5][26];	// FIXME
		// HBM_channelC5[offsets[5]].data[10] = data[5][26];	
		HBM_channelA5[offsets[5]].data[11] = data[5][27];	// FIXME
		// HBM_channelC5[offsets[5]].data[11] = data[5][27];	
		HBM_channelA5[offsets[5]].data[12] = data[5][28];	// FIXME
		// HBM_channelC5[offsets[5]].data[12] = data[5][28];	
		HBM_channelA5[offsets[5]].data[13] = data[5][29];	// FIXME
		// HBM_channelC5[offsets[5]].data[13] = data[5][29];	
		HBM_channelA5[offsets[5]].data[14] = data[5][30];	// FIXME
		// HBM_channelC5[offsets[5]].data[14] = data[5][30];	
		HBM_channelA5[offsets[5]].data[15] = data[5][31];	// FIXME
		// HBM_channelC5[offsets[5]].data[15] = data[5][31];	
	
	
	return;
}
#endif 
void master_retrievemany_Avec(unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	data[1][0] = HBM_channelA1[offsets[1]].data[0];
	data[1][1] = HBM_channelA1[offsets[1]].data[1];
	data[1][2] = HBM_channelA1[offsets[1]].data[2];
	data[1][3] = HBM_channelA1[offsets[1]].data[3];
	data[1][4] = HBM_channelA1[offsets[1]].data[4];
	data[1][5] = HBM_channelA1[offsets[1]].data[5];
	data[1][6] = HBM_channelA1[offsets[1]].data[6];
	data[1][7] = HBM_channelA1[offsets[1]].data[7];
	data[1][8] = HBM_channelA1[offsets[1]].data[8];
	data[1][9] = HBM_channelA1[offsets[1]].data[9];
	data[1][10] = HBM_channelA1[offsets[1]].data[10];
	data[1][11] = HBM_channelA1[offsets[1]].data[11];
	data[1][12] = HBM_channelA1[offsets[1]].data[12];
	data[1][13] = HBM_channelA1[offsets[1]].data[13];
	data[1][14] = HBM_channelA1[offsets[1]].data[14];
	data[1][15] = HBM_channelA1[offsets[1]].data[15];
	data[2][0] = HBM_channelA2[offsets[2]].data[0];
	data[2][1] = HBM_channelA2[offsets[2]].data[1];
	data[2][2] = HBM_channelA2[offsets[2]].data[2];
	data[2][3] = HBM_channelA2[offsets[2]].data[3];
	data[2][4] = HBM_channelA2[offsets[2]].data[4];
	data[2][5] = HBM_channelA2[offsets[2]].data[5];
	data[2][6] = HBM_channelA2[offsets[2]].data[6];
	data[2][7] = HBM_channelA2[offsets[2]].data[7];
	data[2][8] = HBM_channelA2[offsets[2]].data[8];
	data[2][9] = HBM_channelA2[offsets[2]].data[9];
	data[2][10] = HBM_channelA2[offsets[2]].data[10];
	data[2][11] = HBM_channelA2[offsets[2]].data[11];
	data[2][12] = HBM_channelA2[offsets[2]].data[12];
	data[2][13] = HBM_channelA2[offsets[2]].data[13];
	data[2][14] = HBM_channelA2[offsets[2]].data[14];
	data[2][15] = HBM_channelA2[offsets[2]].data[15];
	data[3][0] = HBM_channelA3[offsets[3]].data[0];
	data[3][1] = HBM_channelA3[offsets[3]].data[1];
	data[3][2] = HBM_channelA3[offsets[3]].data[2];
	data[3][3] = HBM_channelA3[offsets[3]].data[3];
	data[3][4] = HBM_channelA3[offsets[3]].data[4];
	data[3][5] = HBM_channelA3[offsets[3]].data[5];
	data[3][6] = HBM_channelA3[offsets[3]].data[6];
	data[3][7] = HBM_channelA3[offsets[3]].data[7];
	data[3][8] = HBM_channelA3[offsets[3]].data[8];
	data[3][9] = HBM_channelA3[offsets[3]].data[9];
	data[3][10] = HBM_channelA3[offsets[3]].data[10];
	data[3][11] = HBM_channelA3[offsets[3]].data[11];
	data[3][12] = HBM_channelA3[offsets[3]].data[12];
	data[3][13] = HBM_channelA3[offsets[3]].data[13];
	data[3][14] = HBM_channelA3[offsets[3]].data[14];
	data[3][15] = HBM_channelA3[offsets[3]].data[15];
	data[4][0] = HBM_channelA4[offsets[4]].data[0];
	data[4][1] = HBM_channelA4[offsets[4]].data[1];
	data[4][2] = HBM_channelA4[offsets[4]].data[2];
	data[4][3] = HBM_channelA4[offsets[4]].data[3];
	data[4][4] = HBM_channelA4[offsets[4]].data[4];
	data[4][5] = HBM_channelA4[offsets[4]].data[5];
	data[4][6] = HBM_channelA4[offsets[4]].data[6];
	data[4][7] = HBM_channelA4[offsets[4]].data[7];
	data[4][8] = HBM_channelA4[offsets[4]].data[8];
	data[4][9] = HBM_channelA4[offsets[4]].data[9];
	data[4][10] = HBM_channelA4[offsets[4]].data[10];
	data[4][11] = HBM_channelA4[offsets[4]].data[11];
	data[4][12] = HBM_channelA4[offsets[4]].data[12];
	data[4][13] = HBM_channelA4[offsets[4]].data[13];
	data[4][14] = HBM_channelA4[offsets[4]].data[14];
	data[4][15] = HBM_channelA4[offsets[4]].data[15];
	data[5][0] = HBM_channelA5[offsets[5]].data[0];
	data[5][1] = HBM_channelA5[offsets[5]].data[1];
	data[5][2] = HBM_channelA5[offsets[5]].data[2];
	data[5][3] = HBM_channelA5[offsets[5]].data[3];
	data[5][4] = HBM_channelA5[offsets[5]].data[4];
	data[5][5] = HBM_channelA5[offsets[5]].data[5];
	data[5][6] = HBM_channelA5[offsets[5]].data[6];
	data[5][7] = HBM_channelA5[offsets[5]].data[7];
	data[5][8] = HBM_channelA5[offsets[5]].data[8];
	data[5][9] = HBM_channelA5[offsets[5]].data[9];
	data[5][10] = HBM_channelA5[offsets[5]].data[10];
	data[5][11] = HBM_channelA5[offsets[5]].data[11];
	data[5][12] = HBM_channelA5[offsets[5]].data[12];
	data[5][13] = HBM_channelA5[offsets[5]].data[13];
	data[5][14] = HBM_channelA5[offsets[5]].data[14];
	data[5][15] = HBM_channelA5[offsets[5]].data[15];
	
	return;
}
void master_retrievemany_Bvec(unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	
 /////////////////
	data[1][16] = HBM_channelB1[offsets[1]].data[0];
 /////////////////
	data[1][17] = HBM_channelB1[offsets[1]].data[1];
 /////////////////
	data[1][18] = HBM_channelB1[offsets[1]].data[2];
 /////////////////
	data[1][19] = HBM_channelB1[offsets[1]].data[3];
 /////////////////
	data[1][20] = HBM_channelB1[offsets[1]].data[4];
 /////////////////
	data[1][21] = HBM_channelB1[offsets[1]].data[5];
 /////////////////
	data[1][22] = HBM_channelB1[offsets[1]].data[6];
 /////////////////
	data[1][23] = HBM_channelB1[offsets[1]].data[7];
 /////////////////
	data[1][24] = HBM_channelB1[offsets[1]].data[8];
 /////////////////
	data[1][25] = HBM_channelB1[offsets[1]].data[9];
 /////////////////
	data[1][26] = HBM_channelB1[offsets[1]].data[10];
 /////////////////
	data[1][27] = HBM_channelB1[offsets[1]].data[11];
 /////////////////
	data[1][28] = HBM_channelB1[offsets[1]].data[12];
 /////////////////
	data[1][29] = HBM_channelB1[offsets[1]].data[13];
 /////////////////
	data[1][30] = HBM_channelB1[offsets[1]].data[14];
 /////////////////
	data[1][31] = HBM_channelB1[offsets[1]].data[15];
	
 /////////////////
	data[2][16] = HBM_channelB2[offsets[2]].data[0];
 /////////////////
	data[2][17] = HBM_channelB2[offsets[2]].data[1];
 /////////////////
	data[2][18] = HBM_channelB2[offsets[2]].data[2];
 /////////////////
	data[2][19] = HBM_channelB2[offsets[2]].data[3];
 /////////////////
	data[2][20] = HBM_channelB2[offsets[2]].data[4];
 /////////////////
	data[2][21] = HBM_channelB2[offsets[2]].data[5];
 /////////////////
	data[2][22] = HBM_channelB2[offsets[2]].data[6];
 /////////////////
	data[2][23] = HBM_channelB2[offsets[2]].data[7];
 /////////////////
	data[2][24] = HBM_channelB2[offsets[2]].data[8];
 /////////////////
	data[2][25] = HBM_channelB2[offsets[2]].data[9];
 /////////////////
	data[2][26] = HBM_channelB2[offsets[2]].data[10];
 /////////////////
	data[2][27] = HBM_channelB2[offsets[2]].data[11];
 /////////////////
	data[2][28] = HBM_channelB2[offsets[2]].data[12];
 /////////////////
	data[2][29] = HBM_channelB2[offsets[2]].data[13];
 /////////////////
	data[2][30] = HBM_channelB2[offsets[2]].data[14];
 /////////////////
	data[2][31] = HBM_channelB2[offsets[2]].data[15];
	
 /////////////////
	data[3][16] = HBM_channelB3[offsets[3]].data[0];
 /////////////////
	data[3][17] = HBM_channelB3[offsets[3]].data[1];
 /////////////////
	data[3][18] = HBM_channelB3[offsets[3]].data[2];
 /////////////////
	data[3][19] = HBM_channelB3[offsets[3]].data[3];
 /////////////////
	data[3][20] = HBM_channelB3[offsets[3]].data[4];
 /////////////////
	data[3][21] = HBM_channelB3[offsets[3]].data[5];
 /////////////////
	data[3][22] = HBM_channelB3[offsets[3]].data[6];
 /////////////////
	data[3][23] = HBM_channelB3[offsets[3]].data[7];
 /////////////////
	data[3][24] = HBM_channelB3[offsets[3]].data[8];
 /////////////////
	data[3][25] = HBM_channelB3[offsets[3]].data[9];
 /////////////////
	data[3][26] = HBM_channelB3[offsets[3]].data[10];
 /////////////////
	data[3][27] = HBM_channelB3[offsets[3]].data[11];
 /////////////////
	data[3][28] = HBM_channelB3[offsets[3]].data[12];
 /////////////////
	data[3][29] = HBM_channelB3[offsets[3]].data[13];
 /////////////////
	data[3][30] = HBM_channelB3[offsets[3]].data[14];
 /////////////////
	data[3][31] = HBM_channelB3[offsets[3]].data[15];
	
 /////////////////
	data[4][16] = HBM_channelB4[offsets[4]].data[0];
 /////////////////
	data[4][17] = HBM_channelB4[offsets[4]].data[1];
 /////////////////
	data[4][18] = HBM_channelB4[offsets[4]].data[2];
 /////////////////
	data[4][19] = HBM_channelB4[offsets[4]].data[3];
 /////////////////
	data[4][20] = HBM_channelB4[offsets[4]].data[4];
 /////////////////
	data[4][21] = HBM_channelB4[offsets[4]].data[5];
 /////////////////
	data[4][22] = HBM_channelB4[offsets[4]].data[6];
 /////////////////
	data[4][23] = HBM_channelB4[offsets[4]].data[7];
 /////////////////
	data[4][24] = HBM_channelB4[offsets[4]].data[8];
 /////////////////
	data[4][25] = HBM_channelB4[offsets[4]].data[9];
 /////////////////
	data[4][26] = HBM_channelB4[offsets[4]].data[10];
 /////////////////
	data[4][27] = HBM_channelB4[offsets[4]].data[11];
 /////////////////
	data[4][28] = HBM_channelB4[offsets[4]].data[12];
 /////////////////
	data[4][29] = HBM_channelB4[offsets[4]].data[13];
 /////////////////
	data[4][30] = HBM_channelB4[offsets[4]].data[14];
 /////////////////
	data[4][31] = HBM_channelB4[offsets[4]].data[15];
	
 /////////////////
	data[5][16] = HBM_channelB5[offsets[5]].data[0];
 /////////////////
	data[5][17] = HBM_channelB5[offsets[5]].data[1];
 /////////////////
	data[5][18] = HBM_channelB5[offsets[5]].data[2];
 /////////////////
	data[5][19] = HBM_channelB5[offsets[5]].data[3];
 /////////////////
	data[5][20] = HBM_channelB5[offsets[5]].data[4];
 /////////////////
	data[5][21] = HBM_channelB5[offsets[5]].data[5];
 /////////////////
	data[5][22] = HBM_channelB5[offsets[5]].data[6];
 /////////////////
	data[5][23] = HBM_channelB5[offsets[5]].data[7];
 /////////////////
	data[5][24] = HBM_channelB5[offsets[5]].data[8];
 /////////////////
	data[5][25] = HBM_channelB5[offsets[5]].data[9];
 /////////////////
	data[5][26] = HBM_channelB5[offsets[5]].data[10];
 /////////////////
	data[5][27] = HBM_channelB5[offsets[5]].data[11];
 /////////////////
	data[5][28] = HBM_channelB5[offsets[5]].data[12];
 /////////////////
	data[5][29] = HBM_channelB5[offsets[5]].data[13];
 /////////////////
	data[5][30] = HBM_channelB5[offsets[5]].data[14];
 /////////////////
	data[5][31] = HBM_channelB5[offsets[5]].data[15];
	
	
	return;
}

unsigned int master_retrieve(unsigned int offset, unsigned int v, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int datas[HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	master_retrieve_vec(offset, datas, HBM_channelA, HBM_channelB, inst);
	return datas[v];
}
void master_retrievemany(unsigned int offsets[NUM_VALID_PEs], unsigned int v, unsigned int data[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int datas[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete dim=0
	master_retrievemany_vec(offsets, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
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
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
	checkoutofbounds("acts_kernel::ERROR 901::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, index, HBM_CHANNEL_PACK_SIZE, NAp);
	#endif 
	return master_retrieve(base_offset__ + (index / HBM_CHANNEL_PACK_SIZE), index % HBM_CHANNEL_PACK_SIZE, HBM_channelA, HBM_channelB, inst);
}	
void dretrievemany_vptrdram(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int data[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
	#pragma HLS INLINE // FIXME_HARDWARE
	data[0] = dretrieve_vptrdram(base_offset__, offsets[0], HBM_channelA0, HBM_channelB0, 0);
	data[1] = dretrieve_vptrdram(base_offset__, offsets[1], HBM_channelA1, HBM_channelB1, 1);
	data[2] = dretrieve_vptrdram(base_offset__, offsets[2], HBM_channelA2, HBM_channelB2, 2);
	data[3] = dretrieve_vptrdram(base_offset__, offsets[3], HBM_channelA3, HBM_channelB3, 3);
	data[4] = dretrieve_vptrdram(base_offset__, offsets[4], HBM_channelA4, HBM_channelB4, 4);
	data[5] = dretrieve_vptrdram(base_offset__, offsets[5], HBM_channelA5, HBM_channelB5, 5);
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
void dretrievemany_actpackvptrdram___dmm(unsigned int base_offset__, unsigned int p_u, unsigned int llp_set, map_t maps[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
	#pragma HLS INLINE // FIXME_HARDWARE
	unsigned int offset = ((p_u * MAX_NUM_LLPSETS) + llp_set) * 2; 
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
	master_retrievemany(offsets_, j, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		maps[n].offset = data_[n];
	}
	master_retrievemany(offsets_, j + 1, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		maps[n].size = data_[n];
	}
	return;
}
void dretrievemany_actpackvptrdram(unsigned int base_offset__, unsigned int offset__, map_t maps[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	master_retrievemany(offsets_, j, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		maps[n].offset = data_[n];
	}
	master_retrievemany(offsets_, j + 1, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		maps[n].size = data_[n];
	}
	return;
}

edge3_vec_dt dretrieve_csredges(unsigned int base_offset__, unsigned int index, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
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

#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
void dinsertmany_actpackedges(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	master_insertmany_Cvec(offsets, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	return;
}
#endif 
void dretrievemany_actpackedges(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
	#pragma HLS INLINE 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 704ft::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	
	master_retrievemany_Avec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);		
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

unsigned int dretrieve_updatesptrs(unsigned int base_offset__, unsigned int index, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
	checkoutofbounds("acts_kernel::ERROR 701::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, inst, NAp, NAp);
	#endif 
	return master_retrieve(base_offset__ + index, 0, HBM_channelA, HBM_channelB, inst); // NEW
}

void dinsertmany_updatesdram(unsigned int offsets[NUM_VALID_PEs], keyvalue_t data[NUM_VALID_PEs][EDGE_PACK_SIZE], bool ens[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
	#pragma HLS INLINE // FIXME_HARDWARE
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
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
	master_insertmany_Bvec(offsets, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5); // CRITICAL FIXME. the issue.
}
void dretrievemany_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data[NUM_VALID_PEs],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5, unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#pragma HLS INLINE 
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	checkoutofbounds("acts_kernel::ERROR 710a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 

	unsigned int data_[NUM_VALID_PEs][HBM_CHANNEL_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + index;
	}
	
	master_retrievemany_Bvec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
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

void dinsertmany_vdatadram(unsigned int offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, vprop_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
			data_[n][2*v+1] = datas[n][v].mask;
		}
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	return;
}
void dretrievemany_vdatadram(unsigned int offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, vprop_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){
			datas[n][v].prop = data_[n][2*v];
			datas[n][v].mask = data_[n][2*v+1];
		}
	}
	return;
}

void dretrievemany_cfrontierdram_tmp(unsigned int base_offset__, unsigned int offsets[NUM_VALID_PEs], unsigned int t, keyvalue_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
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

void dinsertmany_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int offsets[NUM_VALID_PEs], unsigned int t, keyvalue_t datas[NUM_VALID_PEs][EDGE_PACK_SIZE], bool ens[NUM_VALID_PEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
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
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
}	

//////////////////////////////////////////////////////////////////
#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
void insert_edgeupdatesbuffer(unsigned int v, unsigned int index, edge_update_type data, MY_IFDEF_EDGEUPDATESBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 992::", index, EDGE_UPDATES_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	edgeupdates_buffer[v][index] = data;
}
edge_update_type retrieve_edgeupdatesbuffer(unsigned int v, unsigned int index, MY_IFDEF_EDGEUPDATESBUFFER()){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 993::", index, EDGE_UPDATES_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return edgeupdates_buffer[v][index];
}
#endif 

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
	
			
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
			#pragma HLS UNROLL
				if(actvv[v].key != INVALIDDATA){ 
					totalactvvs2_tmp[v] += 1;
				}
			}
			#ifndef FPGA_IMPL
			if(inst==0){ update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___EXCHANGEFRONTIERINFOS___, 1); }
			#endif 
		}
		upartition_vertices[p_actvv_].count += nfrontier_dram___size[p_actvv_]; 
		cfrontier_dram___size[p_actvv_] += nfrontier_dram___size[p_actvv_];
	}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ *totalactvvs2 += totalactvvs2_tmp[v]; }
}	

//////////////////////////////////////////////////////////////////	
MY_IFDEF_NEXTLEVELFUNC(){		
	#ifdef _DEBUGMODE_KERNELPRINTS4
	cout<<"acts_kernel::run:: acts started "<<endl;
	#endif 
	
	// declaration of BRAM variables
// declaration of BRAM variables
#ifdef FPGA_IMPL
keyvalue_t nfrontier_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE][MAX_ACTVV_VECSIZE]; 
#pragma HLS ARRAY_PARTITION variable=nfrontier_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable=nfrontier_buffer complete dim=2
keyvalue_t cfrontier_buffer_tmp[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]; 
#pragma HLS resource variable=cfrontier_buffer_tmp core=XPM_MEMORY uram
#pragma HLS ARRAY_PARTITION variable=cfrontier_buffer_tmp complete dim=1
keyvalue_t cfrontier_buffer[NUM_VALID_PEs][MAXVALID_APPLYPARTITION_VECSIZE];
#pragma HLS resource variable=cfrontier_buffer core=XPM_MEMORY uram
#pragma HLS ARRAY_PARTITION variable=cfrontier_buffer complete dim=1
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
#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
edge_update_type edgeupdates_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE][EDGE_UPDATES_BUFFER_SIZE];	
#pragma HLS ARRAY_PARTITION variable=edgeupdates_buffer complete dim=1	
#pragma HLS ARRAY_PARTITION variable=edgeupdates_buffer complete dim=2	
edge_update_type edgeupdates_outbuffer[NUM_VALID_PEs][EDGE_PACK_SIZE][EDGE_UPDATES_BUFFER_SIZE];	
#pragma HLS ARRAY_PARTITION variable=edgeupdates_outbuffer complete dim=1	
#pragma HLS ARRAY_PARTITION variable=edgeupdates_outbuffer complete dim=2	
#endif 
keyvalue_t updates_buffer2[NUM_VALID_PEs][UPDATES_BUFFER_PACK_SIZE][UPDATES_BUFFER_SIZE];		
#pragma HLS ARRAY_PARTITION variable=updates_buffer2 complete dim=1
#pragma HLS ARRAY_PARTITION variable=updates_buffer2 complete dim=2
vprop_t vdata_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE];
#pragma HLS resource variable=vdata_buffer core=XPM_MEMORY uram 
#pragma HLS ARRAY_PARTITION variable=vdata_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable=vdata_buffer complete dim=2 
#pragma HLS aggregate variable=vdata_buffer 
#else 
keyvalue_t * nfrontier_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE]; 
keyvalue_t * cfrontier_buffer_tmp[EDGE_PACK_SIZE]; 
keyvalue_t * cfrontier_buffer[NUM_VALID_PEs]; 
unsigned int * statsbuffer_maskbased_tmp[NUM_VALID_PEs][EDGE_PACK_SIZE]; 
unsigned int * statsbuffer_maskbased[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
unsigned int * statsbuffer_idbased[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
vtr_t * vptr_buffer[NUM_VALID_PEs]; 
edge3_type * edges_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE];
#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
edge_update_type * edgeupdates_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE];
edge_update_type * edgeupdates_outbuffer[NUM_VALID_PEs][EDGE_PACK_SIZE];
#endif 
keyvalue_t * updates_buffer2[NUM_VALID_PEs][UPDATES_BUFFER_PACK_SIZE]; 
vprop_t * vdata_buffer[NUM_VALID_PEs][EDGE_PACK_SIZE];
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer[i][v] = new keyvalue_t[MAX_ACTVV_VECSIZE]; }}
for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_buffer_tmp[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ cfrontier_buffer[i] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ statsbuffer_maskbased_tmp[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ statsbuffer_maskbased[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ statsbuffer_idbased[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ vptr_buffer[i] = new vtr_t[VPTR_BUFFER_SIZE]; }
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ edges_buffer[i][v] = new edge3_type[EDGE_BUFFER_SIZE]; }}
#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ edgeupdates_buffer[i][v] = new edge_update_type[EDGE_UPDATES_BUFFER_SIZE]; }}
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ edgeupdates_outbuffer[i][v] = new edge_update_type[EDGE_UPDATES_BUFFER_SIZE]; }}
#endif 
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<UPDATES_BUFFER_PACK_SIZE; v++){ updates_buffer2[i][v] = new keyvalue_t[UPDATES_BUFFER_SIZE]; }}
for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ vdata_buffer[i][v] = new vprop_t[MAXVALID_APPLYPARTITION_VECSIZE]; }}
#endif 

#ifdef FPGA_IMPL	
unsigned int cfrontier_dram___size[MAX_NUM_UPARTITIONS]; 
unsigned int nfrontier_dram___size[NUM_VALID_PEs][MAX_NUM_UPARTITIONS];
#pragma HLS ARRAY_PARTITION variable = nfrontier_dram___size complete dim=1
unsigned int nfrontier_buffer___size[NUM_VALID_PEs][EDGE_PACK_SIZE][NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; 
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
unsigned int actpackupdates_dram___size[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
#pragma HLS ARRAY_PARTITION variable = actpackupdates_dram___size complete dim=1
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
#pragma HLS ARRAY_MAP variable=actpackupdates_dram___size instance=array1 horizontal
#pragma HLS ARRAY_MAP variable=vptrbuffer___size instance=array2 horizontal
#pragma HLS ARRAY_MAP variable=edges_buffer___size instance=array2 horizontal
#ifndef HW
unsigned int hybrid_map[MAXNUMGRAPHITERATIONS][MAX_NUM_UPARTITIONS]; 
#endif 
#else 
unsigned int * cfrontier_dram___size = new unsigned int[MAX_NUM_UPARTITIONS]; 
unsigned int * nfrontier_dram___size[NUM_VALID_PEs]; for(unsigned int i=0; i<NUM_VALID_PEs; i++){ nfrontier_dram___size[i] = new unsigned int[MAX_NUM_UPARTITIONS]; }
unsigned int * nfrontier_buffer___size[NUM_VALID_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer___size[i][v] = new unsigned int[NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; }}
unsigned int updates_tmpbuffer___size[NUM_VALID_PEs][NUM_VALID_PEs]; 
unsigned int * updates_buffer___size[NUM_VALID_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[i][v] = new unsigned int[MAX_NUM_APPLYPARTITIONS]; }}
unsigned int stats_buffer___size[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
unsigned int statsbuffer_idbased___size[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
offset_t * upartition_vertices = new offset_t[MAX_NUM_UPARTITIONS]; 
offset_t * vpartition_vertices[NUM_VALID_PEs]; for(unsigned int i=0; i<NUM_VALID_PEs; i++){ vpartition_vertices[i] = new offset_t[MAX_NUM_APPLYPARTITIONS];  }
unsigned int actpackupdates_dram___size[NUM_VALID_PEs][MAX_NUM_APPLYPARTITIONS]; 
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
unsigned int updatesptrs[MAX_NUM_LLPSETS];
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
map_t edgeupdate_maps[NUM_VALID_PEs][BLOCKRAM_SIZE];
#pragma HLS ARRAY_PARTITION variable=edgeupdate_maps 	
map_t edgeupdate_map[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable=edgeupdate_map complete
map_t edge_map[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable=edge_map complete
map_t edge_maps[NUM_VALID_PEs][BLOCKRAM_SIZE];
#pragma HLS ARRAY_PARTITION variable=edge_maps	
unsigned int offsets_eu[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = offsets_eu complete
unsigned int limits_eu[NUM_VALID_PEs];
#pragma HLS ARRAY_PARTITION variable = limits_eu complete
unsigned int max_sz = 0; 

#ifdef FPGA_IMPL
unsigned int _NUMCLOCKCYCLES_[2][32]; 
#endif 		
	
	unsigned int maxGraphIter = 0;
	unsigned int total_num_actvvs = 0; 
	bool enable___vertexcentric = true;
	#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
	bool enable___collectactivedstvids = true;
	#else 
	bool enable___collectactivedstvids = false; // false
	#endif 
	unsigned int max_limit = 0, max_limit2 = 0; 
	unsigned int CONST1 = NUM_PEs * EDGE_PACK_SIZE; 
	unsigned int CONST2 = MAX_ACTVV_VECSIZE * NUM_PEs * EDGE_PACK_SIZE;
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
	
	unsigned int threshold___activedstvids = globalparams[GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEDSTVID];
	
	#ifdef ___ENABLE___RESETBUFFERSATSTART___
	if(___ENABLE___RESETBUFFERSATSTART___BOOL___ == 1){
MY_LOOP200: for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[p] = 0; }
MY_LOOP201: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ nfrontier_dram___size[i][p] = 0; }}
MY_LOOP202: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p++){ nfrontier_buffer___size[i][v][p] = 0; }}}
MY_LOOP203: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_APPLYPARTITIONS; p++){ updates_buffer___size[i][v][p] = 0; }}}
MY_LOOP204: for(unsigned int p=0; p<__NUM_UPARTITIONS; p++){ upartition_vertices[p].offset = 0; upartition_vertices[p].size = 0; upartition_vertices[p].count = 0; } upartition_vertices[0].count = 1;
MY_LOOP205: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ vpartition_vertices[i][p].offset = 0; vpartition_vertices[i][p].size = 0; vpartition_vertices[i][p].count = 0; }}
MY_LOOP206: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ actpackupdates_dram___size[i][p] = 0; }}
MY_LOOP207: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ vptrbuffer___size[i] = 0; }
MY_LOOP208: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ edges_buffer___size[i] = 0; }
#ifndef HW
for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ for(unsigned int t=0; t<MAX_NUM_UPARTITIONS; t++){ hybrid_map[i][t] = NAp; }}
#endif		
	}
	#endif
	
	bool all_vertices_active_in_all_iterations = false; 
	if(globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == PAGERANK || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == CF || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == HITS || globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM] == SPMV){ all_vertices_active_in_all_iterations = true; }
	if(all_vertices_active_in_all_iterations == true){ cfrontier_dram___size[0] = MAXVALID_APPLYPARTITION_VECSIZE; } else { cfrontier_dram___size[0] = 1; }// activate root vid
	unsigned int threshold___activefrontiers = globalparams[GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEFRONTIERSFORCONTROLSWITCH]; 
	
	#ifdef _DEBUGMODE_KERNELPRINTS4
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
	#endif 
	
	// reset ubuffer 
	vprop_t prop; prop.prop = 0; prop.mask = INVALIDMASK; 
	MY_LOOP195: for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ 
	#pragma HLS PIPELINE II=1
		MY_LOOP194: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			MY_LOOP196: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
			#pragma HLS UNROLL
				insert_vdatabuffer(v, t, prop, vdata_buffer[inst]); 
			}
		}
		update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_FRONTIER_PROPERTIES___, 1);
	}
	
	// load updates ptrs
	MY_LOOP193: for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
	#pragma HLS PIPELINE II=1
		updatesptrs[t] = dretrieve_updatesptrs(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], t, HBM_channelA0, HBM_channelB0, 0);
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"--- updatesptrs["<<t<<"]: "<<updatesptrs[t]<<endl;
		#endif 
	}
	
	// load async commands (overlap pcie communication with compuation)
	unsigned int async_batch = globalparams[GLOBALPARAMSCODE__ASYNC__BATCH]; unsigned int _ASYNC_BATCH_SIZE_ = globalparams[GLOBALPARAMSCODE__ASYNC__BATCHSIZE];
	if(batch > 0){
		___ENABLE___PROCESSEDGES___BOOL___ = 0;
		___ENABLE___SAVEVCUPDATES___BOOL___ = 0;
		___ENABLE___COLLECTACTIVEDSTVIDS___BOOL___ = 0;
	}
	
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	unsigned int num_edgeblocks_per_upartition = EDGE_PACK_SIZE;	
	#else 
	unsigned int num_edgeblocks_per_upartition = 1;	
	#endif 
	
	// run acts 
	RUN_ACTS_LOOP1: for(unsigned int run=0; run<globalparams[GLOBALPARAMSCODE__PARAM__NUM_RUNS]; run++){
	RUN_ACTS_LOOP1B: for(unsigned int GraphIter=0; GraphIter<globalparams[GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS]; GraphIter++){
		#ifdef _DEBUGMODE_KERNELPRINTS4 
		cout<<"### processing [run "<<run<<", graph iteration "<<GraphIter<<"]..."<<endl; 
		#endif 
				
		unsigned int MASK_CODE = 1 + GraphIter;
		unsigned int MASK_CODE_PE = 1 + GraphIter;
		unsigned int MASK_CODE_AU = 4094 + 1 + GraphIter;
		
		// clear counters [done]
		#ifdef ___ENABLE___CLEAR_COUNTERS___
// clear counters [done]
CLEAR_COUNTERS_LOOP1: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
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
		#endif 
			
		// process-edges and partition-updates
		PROCESS_EDGES_MODULE_LOOP1: for(unsigned int sweep=0; sweep<2; sweep++){
			// process-edges and partition-updates
			#ifdef ___ENABLE___PROCESSEDGES___
			if(___ENABLE___PROCESSEDGES___BOOL___ == 1){
			PROCESS_EDGES_MODULE_LOOP1B: for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ // 1, __NUM_UPARTITIONS. REMOVEME.
				MASK_CODE_PE = ((1 + GraphIter) * MAX_NUM_UPARTITIONS) + p_u;
				bool ___use_vertex_centric___ = false; 
				bool en = false;
				if((upartition_vertices[p_u].count < threshold___activefrontiers && enable___vertexcentric == true) && (all_vertices_active_in_all_iterations == false)){ ___use_vertex_centric___ = true; } else { ___use_vertex_centric___ = false; }
				
				#ifdef _DEBUGMODE_KERNELPRINTS4 
				if(all_vertices_active_in_all_iterations == true && sweep == 1){ cout<<"### processing edges in upartition "<<p_u<<"..."<<endl; }
				#endif 
			
				// read & map frontier properties 
				#ifdef ___ENABLE___READ_FRONTIER_PROPERTIES___
				if(___ENABLE___READ_FRONTIER_PROPERTIES___BOOL___ == 1){ 
// broadcast active frontiers [done]
unsigned int uoffset_vecsz = p_u * MAX_UPARTITION_VECSIZE; 
MY_LOOP178: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = 0; }
#ifdef _DEBUGMODE_CHECKS3
checkoutofbounds("acts_kernel::ERROR 12073::", cfrontier_dram___size[p_u], MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
#endif 

unsigned int data[HBM_CHANNEL_PACK_SIZE];
#pragma HLS ARRAY_PARTITION variable=data complete
unsigned int offset_c = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP];
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
	HBM_channelA1[offset_c + t].data[0] = data[0];
	HBM_channelA1[offset_c + t].data[1] = data[1];
	HBM_channelA1[offset_c + t].data[2] = data[2];
	HBM_channelA1[offset_c + t].data[3] = data[3];
	HBM_channelA1[offset_c + t].data[4] = data[4];
	HBM_channelA1[offset_c + t].data[5] = data[5];
	HBM_channelA1[offset_c + t].data[6] = data[6];
	HBM_channelA1[offset_c + t].data[7] = data[7];
	HBM_channelA1[offset_c + t].data[8] = data[8];
	HBM_channelA1[offset_c + t].data[9] = data[9];
	HBM_channelA1[offset_c + t].data[10] = data[10];
	HBM_channelA1[offset_c + t].data[11] = data[11];
	HBM_channelA1[offset_c + t].data[12] = data[12];
	HBM_channelA1[offset_c + t].data[13] = data[13];
	HBM_channelA1[offset_c + t].data[14] = data[14];
	HBM_channelA1[offset_c + t].data[15] = data[15];
	HBM_channelB1[offset_c + t].data[0] = data[16];
	HBM_channelB1[offset_c + t].data[1] = data[17];
	HBM_channelB1[offset_c + t].data[2] = data[18];
	HBM_channelB1[offset_c + t].data[3] = data[19];
	HBM_channelB1[offset_c + t].data[4] = data[20];
	HBM_channelB1[offset_c + t].data[5] = data[21];
	HBM_channelB1[offset_c + t].data[6] = data[22];
	HBM_channelB1[offset_c + t].data[7] = data[23];
	HBM_channelB1[offset_c + t].data[8] = data[24];
	HBM_channelB1[offset_c + t].data[9] = data[25];
	HBM_channelB1[offset_c + t].data[10] = data[26];
	HBM_channelB1[offset_c + t].data[11] = data[27];
	HBM_channelB1[offset_c + t].data[12] = data[28];
	HBM_channelB1[offset_c + t].data[13] = data[29];
	HBM_channelB1[offset_c + t].data[14] = data[30];
	HBM_channelB1[offset_c + t].data[15] = data[31];
	HBM_channelA2[offset_c + t].data[0] = data[0];
	HBM_channelA2[offset_c + t].data[1] = data[1];
	HBM_channelA2[offset_c + t].data[2] = data[2];
	HBM_channelA2[offset_c + t].data[3] = data[3];
	HBM_channelA2[offset_c + t].data[4] = data[4];
	HBM_channelA2[offset_c + t].data[5] = data[5];
	HBM_channelA2[offset_c + t].data[6] = data[6];
	HBM_channelA2[offset_c + t].data[7] = data[7];
	HBM_channelA2[offset_c + t].data[8] = data[8];
	HBM_channelA2[offset_c + t].data[9] = data[9];
	HBM_channelA2[offset_c + t].data[10] = data[10];
	HBM_channelA2[offset_c + t].data[11] = data[11];
	HBM_channelA2[offset_c + t].data[12] = data[12];
	HBM_channelA2[offset_c + t].data[13] = data[13];
	HBM_channelA2[offset_c + t].data[14] = data[14];
	HBM_channelA2[offset_c + t].data[15] = data[15];
	HBM_channelB2[offset_c + t].data[0] = data[16];
	HBM_channelB2[offset_c + t].data[1] = data[17];
	HBM_channelB2[offset_c + t].data[2] = data[18];
	HBM_channelB2[offset_c + t].data[3] = data[19];
	HBM_channelB2[offset_c + t].data[4] = data[20];
	HBM_channelB2[offset_c + t].data[5] = data[21];
	HBM_channelB2[offset_c + t].data[6] = data[22];
	HBM_channelB2[offset_c + t].data[7] = data[23];
	HBM_channelB2[offset_c + t].data[8] = data[24];
	HBM_channelB2[offset_c + t].data[9] = data[25];
	HBM_channelB2[offset_c + t].data[10] = data[26];
	HBM_channelB2[offset_c + t].data[11] = data[27];
	HBM_channelB2[offset_c + t].data[12] = data[28];
	HBM_channelB2[offset_c + t].data[13] = data[29];
	HBM_channelB2[offset_c + t].data[14] = data[30];
	HBM_channelB2[offset_c + t].data[15] = data[31];
	HBM_channelA3[offset_c + t].data[0] = data[0];
	HBM_channelA3[offset_c + t].data[1] = data[1];
	HBM_channelA3[offset_c + t].data[2] = data[2];
	HBM_channelA3[offset_c + t].data[3] = data[3];
	HBM_channelA3[offset_c + t].data[4] = data[4];
	HBM_channelA3[offset_c + t].data[5] = data[5];
	HBM_channelA3[offset_c + t].data[6] = data[6];
	HBM_channelA3[offset_c + t].data[7] = data[7];
	HBM_channelA3[offset_c + t].data[8] = data[8];
	HBM_channelA3[offset_c + t].data[9] = data[9];
	HBM_channelA3[offset_c + t].data[10] = data[10];
	HBM_channelA3[offset_c + t].data[11] = data[11];
	HBM_channelA3[offset_c + t].data[12] = data[12];
	HBM_channelA3[offset_c + t].data[13] = data[13];
	HBM_channelA3[offset_c + t].data[14] = data[14];
	HBM_channelA3[offset_c + t].data[15] = data[15];
	HBM_channelB3[offset_c + t].data[0] = data[16];
	HBM_channelB3[offset_c + t].data[1] = data[17];
	HBM_channelB3[offset_c + t].data[2] = data[18];
	HBM_channelB3[offset_c + t].data[3] = data[19];
	HBM_channelB3[offset_c + t].data[4] = data[20];
	HBM_channelB3[offset_c + t].data[5] = data[21];
	HBM_channelB3[offset_c + t].data[6] = data[22];
	HBM_channelB3[offset_c + t].data[7] = data[23];
	HBM_channelB3[offset_c + t].data[8] = data[24];
	HBM_channelB3[offset_c + t].data[9] = data[25];
	HBM_channelB3[offset_c + t].data[10] = data[26];
	HBM_channelB3[offset_c + t].data[11] = data[27];
	HBM_channelB3[offset_c + t].data[12] = data[28];
	HBM_channelB3[offset_c + t].data[13] = data[29];
	HBM_channelB3[offset_c + t].data[14] = data[30];
	HBM_channelB3[offset_c + t].data[15] = data[31];
	HBM_channelA4[offset_c + t].data[0] = data[0];
	HBM_channelA4[offset_c + t].data[1] = data[1];
	HBM_channelA4[offset_c + t].data[2] = data[2];
	HBM_channelA4[offset_c + t].data[3] = data[3];
	HBM_channelA4[offset_c + t].data[4] = data[4];
	HBM_channelA4[offset_c + t].data[5] = data[5];
	HBM_channelA4[offset_c + t].data[6] = data[6];
	HBM_channelA4[offset_c + t].data[7] = data[7];
	HBM_channelA4[offset_c + t].data[8] = data[8];
	HBM_channelA4[offset_c + t].data[9] = data[9];
	HBM_channelA4[offset_c + t].data[10] = data[10];
	HBM_channelA4[offset_c + t].data[11] = data[11];
	HBM_channelA4[offset_c + t].data[12] = data[12];
	HBM_channelA4[offset_c + t].data[13] = data[13];
	HBM_channelA4[offset_c + t].data[14] = data[14];
	HBM_channelA4[offset_c + t].data[15] = data[15];
	HBM_channelB4[offset_c + t].data[0] = data[16];
	HBM_channelB4[offset_c + t].data[1] = data[17];
	HBM_channelB4[offset_c + t].data[2] = data[18];
	HBM_channelB4[offset_c + t].data[3] = data[19];
	HBM_channelB4[offset_c + t].data[4] = data[20];
	HBM_channelB4[offset_c + t].data[5] = data[21];
	HBM_channelB4[offset_c + t].data[6] = data[22];
	HBM_channelB4[offset_c + t].data[7] = data[23];
	HBM_channelB4[offset_c + t].data[8] = data[24];
	HBM_channelB4[offset_c + t].data[9] = data[25];
	HBM_channelB4[offset_c + t].data[10] = data[26];
	HBM_channelB4[offset_c + t].data[11] = data[27];
	HBM_channelB4[offset_c + t].data[12] = data[28];
	HBM_channelB4[offset_c + t].data[13] = data[29];
	HBM_channelB4[offset_c + t].data[14] = data[30];
	HBM_channelB4[offset_c + t].data[15] = data[31];
	HBM_channelA5[offset_c + t].data[0] = data[0];
	HBM_channelA5[offset_c + t].data[1] = data[1];
	HBM_channelA5[offset_c + t].data[2] = data[2];
	HBM_channelA5[offset_c + t].data[3] = data[3];
	HBM_channelA5[offset_c + t].data[4] = data[4];
	HBM_channelA5[offset_c + t].data[5] = data[5];
	HBM_channelA5[offset_c + t].data[6] = data[6];
	HBM_channelA5[offset_c + t].data[7] = data[7];
	HBM_channelA5[offset_c + t].data[8] = data[8];
	HBM_channelA5[offset_c + t].data[9] = data[9];
	HBM_channelA5[offset_c + t].data[10] = data[10];
	HBM_channelA5[offset_c + t].data[11] = data[11];
	HBM_channelA5[offset_c + t].data[12] = data[12];
	HBM_channelA5[offset_c + t].data[13] = data[13];
	HBM_channelA5[offset_c + t].data[14] = data[14];
	HBM_channelA5[offset_c + t].data[15] = data[15];
	HBM_channelB5[offset_c + t].data[0] = data[16];
	HBM_channelB5[offset_c + t].data[1] = data[17];
	HBM_channelB5[offset_c + t].data[2] = data[18];
	HBM_channelB5[offset_c + t].data[3] = data[19];
	HBM_channelB5[offset_c + t].data[4] = data[20];
	HBM_channelB5[offset_c + t].data[5] = data[21];
	HBM_channelB5[offset_c + t].data[6] = data[22];
	HBM_channelB5[offset_c + t].data[7] = data[23];
	HBM_channelB5[offset_c + t].data[8] = data[24];
	HBM_channelB5[offset_c + t].data[9] = data[25];
	HBM_channelB5[offset_c + t].data[10] = data[26];
	HBM_channelB5[offset_c + t].data[11] = data[27];
	HBM_channelB5[offset_c + t].data[12] = data[28];
	HBM_channelB5[offset_c + t].data[13] = data[29];
	HBM_channelB5[offset_c + t].data[14] = data[30];
	HBM_channelB5[offset_c + t].data[15] = data[31];
}

// parallel-read and map active frontiers [done]
MY_LOOP173: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = 0; }
unsigned int uoffset = p_u * MAX_UPARTITION_SIZE;
MY_LOOP175_DEBUG: for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){ 
#pragma HLS PIPELINE II=1
	dretrievemany_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], offsets, t, kvdatas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	insertvec_cfrontierbuffer_tmp(t, kvdatas[0], cfrontier_buffer_tmp); // NEW
	if(sweep==1){  
		MY_LOOP176: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
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
				
				// reset buffers [done]
				#ifdef ___ENABLE___RESET_BUFFERS___
// reset temp stats buffer [done]
if(___use_vertex_centric___ == false){
	MY_LOOP170: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ 
	#pragma HLS PIPELINE II=1
		MY_LOOP171: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
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
MY_LOOP168: for(unsigned int j=0; j<NUM_VALID_PEs; j++){ 
#pragma HLS PIPELINE II=1
	MY_LOOP169: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ 
	#pragma HLS UNROLL
		updates_tmpbuffer___size[i][j] = 0; 
	} 
	update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_FRONTIER_PROPERTIES___, 1);
}
	
				#endif 
		
				// process-edges and partition-updates
				if(upartition_vertices[p_u].count > 0 || all_vertices_active_in_all_iterations == true){
					if(sweep == 0 && ___use_vertex_centric___ == true){
						#ifdef ___ENABLE___VCPROCESSEDGES___
						if(___ENABLE___VCPROCESSEDGES___BOOL___ == 1){
#ifndef HW
hybrid_map[GraphIter][p_u] = 0;
#endif 

// reset buffers [done]
for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
#pragma HLS UNROLL
	vptrbuffer___size[inst] = 0; edges_buffer___size[inst] = 0; // reset
} 

// distribute vptrs [done]
for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ cummtv2[inst] = 0; }
for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cfrontier_bufferREAL___size[n] = 0; }
VC_READVPTRS_LOOP1: for(unsigned int pingpong=0; pingpong<cfrontier_dram___size[p_u] * EDGE_PACK_SIZE; pingpong++){
#pragma HLS PIPELINE II=1
	frontier_t actvv = retrieve_cfrontierbuffer_tmp(pingpong % EDGE_PACK_SIZE, pingpong / EDGE_PACK_SIZE, cfrontier_buffer_tmp); // NEW
	if(actvv.key != INVALIDDATA){
		unsigned int inst_ = actvv.key % NUM_VALID_PEs;
		cfrontier_buffer[inst_][cfrontier_bufferREAL___size[inst_]] = actvv;
		cfrontier_bufferREAL___size[inst_] += 1;
		update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 1);
		#ifndef FPGA_IMPL
		checkoutofbounds("acts_kernel::ERROR 073::", cfrontier_bufferREAL___size[inst_], MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
	}
} 

// load vptrs [done]
max_limit = 0; MY_LOOP167: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max_limit < cfrontier_bufferREAL___size[n]){ max_limit = cfrontier_bufferREAL___size[n]; }} 
// cout<<"--------------------- max_limit: "<<max_limit<<endl;
VC_READVPTRS_LOOP2: for(unsigned int t=0; t<max_limit; t++){ // FIXME_HARDWARE. CAUSE OF POOR FREQUENCY?
	unsigned int begins_u32[NUM_VALID_PEs]; 
	unsigned int ends_u32[NUM_VALID_PEs];
	frontier_t actvvs[NUM_VALID_PEs];
	
	VC_READVPTRS_LOOP2B: for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		actvvs[n] = cfrontier_buffer[n][t];
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
	dretrievemany_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], offsets, begins_u32,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 1 + DRAM_ACCESS_LATENCY);
	
	VC_READVPTRS_LOOP2C: for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		offsets[n] += 1; 
	}
	dretrievemany_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], offsets, ends_u32,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___VCPROCESSEDGES___, 1 + DRAM_ACCESS_LATENCY);
	
	#ifdef ___FORCE_SUCCESS___
	MY_LOOP10023: for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		if(ens[n][0] == false){ begins_u32[n] = 0; ends_u32[n] = 0; }
	}
	#endif 
	
	VC_READVPTRS_LOOP2D: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
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
		#ifdef ___FORCE_SUCCESS___
		vptrbuffer___size[inst] += 1; // if(vptrbuffer___size[inst] < VPTR_BUFFER_SIZE-1){ vptrbuffer___size[inst] += 1; }
		#else 
		vptrbuffer___size[inst] += 1;		
		#endif 
		cummtv2[inst] += data.size;
	}
}

// calculate batches [done]
max_limit = 0; MY_LOOP166: for(unsigned int in=0; in<NUM_VALID_PEs; in++){ if(max_limit < cummtv2[in]){ max_limit = cummtv2[in]; }} 
unsigned int num_batches = 1; unsigned int cummtv2_ = max_limit / EDGE_PACK_SIZE;
if(cummtv2_ >= EDGE_BUFFER_SIZE){
	if(cummtv2_ / 4 < EDGE_BUFFER_SIZE){ num_batches = 4; }
	else if(cummtv2_ / 8 < EDGE_BUFFER_SIZE){ num_batches = 8; } 
	else if(cummtv2_ / 16 < EDGE_BUFFER_SIZE){ num_batches = 16; }
	#ifndef FPGA_IMPL
	else { cout<<"acts_kernel:: ERROR 2367: suitable n not found. EXITING..."<<endl; cout<<"----- max_limit: "<<max_limit<<",  cummtv2_: "<<cummtv2_<<endl; for(unsigned int in=0; in<NUM_VALID_PEs; in++){ cout<<"----- cummtv2["<<in<<"]: "<<cummtv2[in]<<endl; } exit(EXIT_FAILURE); }
	#endif 
}
max_limit = 0; for(unsigned int in=0; in<NUM_VALID_PEs; in++){ if(max_limit < vptrbuffer___size[in]){ max_limit = vptrbuffer___size[in]; }}
unsigned int batch_size = (max_limit + (num_batches - 1)) / num_batches;

// read and process edges 
VC_READANDPROCESS_EDGES_LOOP1: for(unsigned int n=0; n<num_batches; n++){
	MY_LOOP163: for(unsigned int in=0; in<NUM_VALID_PEs; in++){ 
	#pragma HLS UNROLL
		edges_buffer___size[in] = 0; // reset
	} 
	
	// load edges
	VC_READ_EDGES_LOOP1: for(unsigned int k=0; k<batch_size; k++){ 
	
		load_edges(0, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[0], vptr_buffer[0], &edges_buffer___size[0], edges_buffer[0], HBM_channelA0, HBM_channelB0);
	
		load_edges(1, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[1], vptr_buffer[1], &edges_buffer___size[1], edges_buffer[1], HBM_channelA1, HBM_channelB1);
	
		load_edges(2, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[2], vptr_buffer[2], &edges_buffer___size[2], edges_buffer[2], HBM_channelA2, HBM_channelB2);
	
		load_edges(3, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[3], vptr_buffer[3], &edges_buffer___size[3], edges_buffer[3], HBM_channelA3, HBM_channelB3);
	
		load_edges(4, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[4], vptr_buffer[4], &edges_buffer___size[4], edges_buffer[4], HBM_channelA4, HBM_channelB4);
	
		load_edges(5, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[5], vptr_buffer[5], &edges_buffer___size[5], edges_buffer[5], HBM_channelA5, HBM_channelB5);
	
	}			

	// process & partition edges [done]
	max_limit = 0; for(unsigned int in=0; in<NUM_VALID_PEs; in++){ if(max_limit < edges_buffer___size[in]){ max_limit = edges_buffer___size[in]; }} 
	VC_PROCESS_EDGES_LOOP1: for(unsigned int t=0; t<max_limit; t++){
		VC_PROCESS_EDGES_LOOP1B: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS PIPELINE II=2
			VC_PROCESS_EDGES_LOOP1C: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
			#pragma HLS UNROLL
				edge3_type edge = retrieve_edgesbuffer(v, t, edges_buffer[inst]); // NEW
				if(edge.dstvid != INVALIDDATA){
					unsigned int uprop = edge.srcvid; // edge.srcvid has uprop information
					unsigned int res = process_funcG(uprop, 1, globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM]);
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3 
					std::cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", uprop: "<<uprop<<", res: "<<res<<"]"<<std::endl;
					std::cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [local partition: "<<p_u<<", vid: "<<NAp<<"]"<<std::endl;
					#endif 
					
					#ifdef ___FORCE_SUCCESS_SINGLE_CHANNEL___
					edge.dstvid = get_local(edge.dstvid); 
					unsigned int inst_mirror = 0;												
					#else 
					unsigned int inst_mirror = get_H(edge.dstvid);	
					#endif
					keyvalue_t update; update.key = edge.dstvid; update.value = res;
					#ifndef FPGA_IMPL
					checkoutofbounds("acts_kernel::ERROR 813a::", inst_mirror, NUM_PEs,  edge.dstvid, inst_mirror, NAp);
					#endif 
					
					bool save;
					#ifdef ___FORCE_SUCCESS___
					if(inst_mirror < NUM_VALID_PEs){ if(t < edges_buffer___size[inst] && updates_tmpbuffer___size[inst][inst_mirror] < UPDATES_BUFFER_SIZE){ save = true; } else { save = false; }} else { save = false; }
					#else 
					if(t < edges_buffer___size[inst]){ save = true; } else { save = false; }
					#endif 
					if(save){
						#ifdef _DEBUGMODE_CHECKS3
						checkoutofbounds("acts_kernel::ERROR 8134a::", inst_mirror, NUM_VALID_PEs,  edge.dstvid, inst_mirror, NAp);
						#endif 
						#if not defined(___FORCE_SUCCESS___) && not defined(FPGA_IMPL)
						checkoutofbounds("acts_kernel::ERROR 813a::", inst_mirror, NUM_VALID_PEs,  edge.dstvid, inst_mirror, NAp);
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
VC_EXCHANGE_UPDATES_LOOP1: for(unsigned int inst_mirror=0; inst_mirror<NUM_VALID_PEs; inst_mirror++){
	VC_EXCHANGE_UPDATES_LOOP1B: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
		VC_EXCHANGE_UPDATES_LOOP1C: for(unsigned int t=0; t<updates_tmpbuffer___size[inst][inst_mirror]; t++){
		#pragma HLS PIPELINE II=3 
			keyvalue_t update = retrieve_updatestmpbuffer(inst_mirror, t, updates_buffer2[inst]);
			
			unsigned int vid_l = get_local(update.key); // NEW**
			unsigned int vid_lp = get_local_to_vpartition(vid_l);
			unsigned int p_v = vid_l / MAX_APPLYPARTITION_SIZE;
			keyvalue_t update_local = update; update_local.key = vid_lp;// / EDGE_PACK_SIZE; // CRITICAL NEWCHANGE.
			// unsigned int v = update_local.key % EDGE_PACK_SIZE;
			unsigned int v = vid_lp % EDGE_PACK_SIZE; 
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"VC - EXCHANGE UPDATE SEEN @: [inst: "<<inst<<", inst_mirror: "<<inst_mirror<<"], [update.key: "<<update.key<<", update.value: "<<update.value<<"]"<<endl;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS3
			if((vid_lp % EDGE_PACK_SIZE) != v){ cout<<"acts_kernel 12347:: vid_lp("<<vid_lp<<") % v("<<v<<") != 0. EXITING..."<<endl; for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"update_local.key ("<<(update_local.key % EDGE_PACK_SIZE)<<"): "<<update_local.key<<""<<endl; } exit(EXIT_FAILURE); }				
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
			
			unsigned int dstvid_lpv = vid_lp / EDGE_PACK_SIZE; 
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
					} else if (sweep == 1  && ___use_vertex_centric___ == false) { // edge-centric [done]
						// load edge maps
						for(unsigned int edge_block=0; edge_block<__NUM_APPLYPARTITIONS * num_edgeblocks_per_upartition; edge_block++){ 
							#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___							
							dretrievemany_actpackvptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2], ((p_u * MAX_NUM_LLPS) + edge_block), edge_map,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
							#else 
							dretrievemany_actpackvptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS], ((p_u * MAX_NUM_LLPSETS) + edge_block), edge_map,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
							#endif 
							for(unsigned int n=0; n<NUM_VALID_PEs; n++){
							#pragma HLS UNROLL
								edge_maps[n][edge_block] = edge_map[n];
							}
						}
						
						#ifdef ___ENABLE___ECPROCESSEDGES___II1___
						#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
// load maps 
for(unsigned int edge_block=0; edge_block<__NUM_APPLYPARTITIONS * EDGE_PACK_SIZE; edge_block++){ 
	dretrievemany_actpackvptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS], ((p_u * MAX_NUM_LLPS) + edge_block), edgeupdate_map,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5); 
	for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		edgeupdate_maps[n][edge_block] = edgeupdate_map[n];
	}
}

EC_PROCESS_EDGEUPDATES_LOOP1: for(unsigned int llp_set=0; llp_set<__NUM_APPLYPARTITIONS; llp_set++){	
	EC_PROCESS_EDGEUPDATES_LOOP1B: for(unsigned int llp_id=0; llp_id<EDGE_PACK_SIZE; llp_id++){ // REMOVEME.
		unsigned int edge_block = (llp_set * EDGE_PACK_SIZE) + llp_id;
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"------------------- update edges: ["<<p_u<<", "<<llp_set<<", "<<llp_id<<", "<<edge_block<<"] -------------------"<<endl; 
		#endif 
		
		// prepare maps, offsets, variables 
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = edge_maps[n][edge_block].offset; }	

		// prepare maps, offsets, variables 
		unsigned int max_sz_eu = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ limits_eu[n] = edgeupdate_maps[n][edge_block].size; } for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max_sz_eu < limits_eu[n]){ max_sz_eu = limits_eu[n]; }}
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
			offsets_eu[n] = edgeupdate_maps[n][edge_block].offset; 
		}	
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"[BEFORE - EDGE UPDATES MAP]: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<edgeupdate_maps[n][edge_block].size<<", "; } cout<<endl;
		cout<<"[BEFORE - EDGE MAP]: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<edge_maps[n][edge_block].size<<", "; } cout<<endl;
		#endif 
		
		// clear edge updates buffers
		#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___ // FIXME.
		MY_LOOP180: for(unsigned int t=0; t<EDGE_UPDATES_BUFFER_SIZE; t++){ 
		#pragma HLS PIPELINE II=1
			MY_LOOP181: for(unsigned int i=0; i<NUM_VALID_PEs; i++){ 
			#pragma HLS UNROLL
				MY_LOOP182: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
				#pragma HLS UNROLL
					edgeupdates_buffer[i][v][t].srcvid = INVALIDDATA; 
					edgeupdates_buffer[i][v][t].dstvid = INVALIDDATA; 
				}
			}			
		}	
		#endif 
		
		// populate edge updates buffers
		unsigned int sz = max_sz_eu; if(max_sz_eu > EDGE_UPDATES_BUFFER_SIZE){ sz = EDGE_UPDATES_BUFFER_SIZE; }
		LOAD_EDGEUPDATES_LOOP1: for(unsigned int t=0; t<sz; t++){ 
		#pragma HLS PIPELINE II=1
			dretrievemany_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES], offsets_eu, t, edge3_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
			MY_LOOP1273: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				MY_LOOP1274: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				#pragma HLS UNROLL
					unsigned int uniq_srcvid = edge3_vecs[inst].data[v].srcvid;
					unsigned int uniq_dstvid = edge3_vecs[inst].data[v].dstvid;
					unsigned int edge_hashid = hash_edge(uniq_srcvid, uniq_dstvid, t);
					
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					if(inst==0 && t<4){ cout<<"[hashing edge updates into buffer locations: edge["<<inst<<"]["<<t<<"]["<<v<<"]: srcvid: "<<edge3_vecs[inst].data[v].srcvid<<", dstvid: "<<edge3_vecs[inst].data[v].dstvid<<", weight: "<<edge3_vecs[inst].data[v].weight<<", edge_hashid: "<<edge_hashid<<"]"<<endl; }
					#endif 
					#ifdef _DEBUGMODE_CHECKS3
					checkoutofbounds("acts_kernel::ERROR 2513::", edge_hashid, EDGE_UPDATES_BUFFER_SIZE, NAp, NAp, NAp);
					#endif
					insert_edgeupdatesbuffer(v, edge_hashid, edge3_vecs[inst].data[v], edgeupdates_buffer[inst]); 
					update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___NUMBER_OF_EDGEUPDATES_LOADED___, 1);
				}
			}
		}
		
		// update trackers
		#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
		for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
		#pragma HLS UNROLL
			if(edgeupdate_maps[inst][edge_block].size >= sz){ edgeupdate_maps[inst][edge_block].size -= sz; } else { edgeupdate_maps[inst][edge_block].size = 0; }
		}
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"[EDGE UPDATES MAP(BEFORE)]: "; for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ cout<<edgeupdate_maps[inst][edge_block].size + sz<<", "; } cout<<endl;
		cout<<"[EDGE UPDATES MAP(AFTER)]: "; for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ cout<<edgeupdate_maps[inst][edge_block].size<<", "; } cout<<endl;
		#endif 
		#endif 
		
		// update edges 
		edge_update_type invalid_data; invalid_data.srcvid = INVALIDDATA; invalid_data.dstvid = INVALIDDATA; invalid_data.weight = INVALIDDATA; invalid_data.valid = INVALIDDATA;
		EC_PROCESS_EDGEUPDATES_MAINLOOP1D: for(unsigned int t=0; t<max_sz_eu; t++){ 
		#pragma HLS PIPELINE II=1
			dretrievemany_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	
			EC_PROCESS_EDGEUPDATES_LOOP1E: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				edge3_vec_dt edge_vec_out;
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ // FIXME. for(unsigned int v=1; v<EDGE_PACK_SIZE; v++) // FIXME.
					unsigned int uniq_srcvid = edge3_vecs[inst].data[v].srcvid;		
					unsigned int uniq_dstvid = edge3_vecs[inst].data[v].dstvid;		
					unsigned int weight = edge3_vecs[inst].data[v].weight;		
					unsigned int edge_hashid = hash_edge(uniq_srcvid, uniq_dstvid, t);
					
					edge_update_type edge_update = retrieve_edgeupdatesbuffer(v, edge_hashid, edgeupdates_buffer[inst]);
					unsigned int uniq_eu_srcvid = edge_update.srcvid;
					unsigned int uniq_eu_dstvid = edge_update.dstvid;
					unsigned int eu_weight = edge_update.weight;
					
					if(uniq_srcvid == uniq_eu_srcvid && uniq_dstvid == uniq_eu_dstvid){
						// edge update found. merge edge with edge update & invalidate the edge update 
						edge3_vecs[inst].data[v].weight += eu_weight; 
						insert_edgeupdatesbuffer(v, edge_hashid, invalid_data, edgeupdates_buffer[inst]); // FIXME. can prevent complete pipelining with II=1
						
						#ifdef _DEBUGMODE_KERNELPRINTS//4	
						if(inst==0 && t<1){ cout<<"[newly updated edge: edge["<<inst<<"]["<<t<<"]["<<v<<"]: srcvid: "<<edge3_vecs[inst].data[v].srcvid<<", dstvid: "<<edge3_vecs[inst].data[v].dstvid<<", new weight: "<<edge3_vecs[inst].data[v].weight<<", old weight: "<<edge3_vecs[inst].data[v].weight - eu_weight<<", edge_hashid: "<<edge_hashid<<"]"<<endl; }
						#endif 	
						update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___NUMBER_OF_EDGEUPDATES_APPLIED___, 1);
					} else {
						#ifdef _DEBUGMODE_KERNELPRINTS//4	
						if(inst==0 && t<1){ 
							cout<<"[un-updated edge: edge["<<inst<<"]["<<t<<"]["<<v<<"]: srcvid: "<<edge3_vecs[inst].data[v].srcvid<<", dstvid: "<<edge3_vecs[inst].data[v].dstvid<<", new weight: "<<edge3_vecs[inst].data[v].weight<<", old weight: "<<edge3_vecs[inst].data[v].weight - eu_weight<<", edge_hashid: "<<edge_hashid<<"]"<<endl; 
							cout<<"[edge update: edge_update: srcvid: "<<edge_update.srcvid<<", dstvid: "<<edge_update.dstvid<<", new weight: "<<edge_update.weight<<", old weight: "<<edge_update.weight<<", edge_hashid: "<<edge_hashid<<"]"<<endl; 
						}
						#endif 	
					}
				}
			}
		
			dinsertmany_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5); // FIXME
			update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
		}
		
		// compress unclaimed edge updates and save
		for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
			#pragma HLS UNROLL
				edge_counts[inst][v] = 0;
			}
		}
		
		COMPRESS_EDGEUPDATES_LOOP1: for(unsigned int t=0; t<sz; t++){ 
		#pragma HLS PIPELINE II=1
			for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				#pragma HLS UNROLL
					edge_update_type edge_update = retrieve_edgeupdatesbuffer(v, t, edgeupdates_buffer[inst]);		
					if(edge_update.srcvid != INVALIDDATA){
						insert_edgeupdatesbuffer(v, edge_counts[inst][v], edge_update, edgeupdates_outbuffer[inst]); 
						edge_counts[inst][v] += 1;
						update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___NUMBER_OF_NEW_EDGEUPDATES_ADDED___, 1);
					}		
				}
			}
		}
		
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			limits[n] = 0; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(limits[n] < edge_counts[n][v]){ limits[n] = edge_counts[n][v]; }}	
		}
		unsigned int maxsz = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){  if(maxsz < limits[n]){ maxsz = limits[n]; }}	
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = edge_maps[n][edge_block].offset + edge_maps[n][edge_block].size; }	
		
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"edge counts: [maxsz: "<<maxsz<<"]: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cout<<edge_counts[n][v]<<", "; }} cout<<endl;
		#endif
		SAVE_NEWEDGEUPDATES_LOOP1: for(unsigned int t=0; t<maxsz; t++){ 
		#pragma HLS PIPELINE II=1
			for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				#pragma HLS UNROLL
					edge3_vecs[inst].data[v] = retrieve_edgeupdatesbuffer(v, t, edgeupdates_buffer[inst]);
					update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___NUMBER_OF_NEW_EDGEUPDATES_ADDED___, 1);
				}
			}
			dinsertmany_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);	// FIXME
		}
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ edge_maps[n][edge_block].size += maxsz; }		
		
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"[AFTER - EDGE MAP]: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<edge_maps[n][edge_block].size<<", "; } cout<<endl;
		cout<<"[AFTER - EDGE UPDATES MAP]: "; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ cout<<edgeupdate_maps[n][edge_block].size<<", "; } cout<<endl;
		#endif 
		// exit(EXIT_SUCCESS);
	}
	// exit(EXIT_SUCCESS); 
}
// exit(EXIT_SUCCESS); 


							
						#endif
						#endif 
					
						#ifdef ___ENABLE___ECPROCESSEDGES___II1___
						if(___ENABLE___ECPROCESSEDGES___BOOL___ == 1){
#ifndef HW
hybrid_map[GraphIter][p_u] = 1;
#endif 

EC_PROCESS_EDGES_LOOP1: for(unsigned int llp_set=0; llp_set<__NUM_APPLYPARTITIONS; llp_set++){	// 1 __NUM_APPLYPARTITIONS REMOVEME
	EC_PROCESS_EDGES_LOOP1B: for(unsigned int llp_id=0; llp_id<num_edgeblocks_per_upartition; llp_id++){ // 1 num_its REMOVEME
		unsigned int edge_block = (llp_set * num_edgeblocks_per_upartition) + llp_id;
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		cout<<"------------------- process edges: ["<<p_u<<", "<<llp_set<<", "<<llp_id<<"] -------------------"<<endl; 
		#endif 

		// prepare maps, offsets, variables 
		max_sz = 0; for(unsigned int n=0; n<NUM_VALID_PEs; n++){ limits[n] = edge_maps[n][edge_block].size; } for(unsigned int n=0; n<NUM_VALID_PEs; n++){ if(max_sz < limits[n]){ max_sz = limits[n]; }}			
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = edge_maps[n][edge_block].offset; }		
		
		MY_LOOP1529: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ counts[n] = 0; }
		MY_LOOP1519: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets2[n] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[llp_set]; }
			
		// process edges
		EC_PROCESS_EDGES_MAINLOOP1D: for(unsigned int t=0; t<max_sz; t++){ 
		#pragma HLS PIPELINE II=1
			dretrievemany_actpackedges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
			
			keyvalue_t update_in[NUM_VALID_PEs][EDGE_PACK_SIZE];
			#pragma HLS ARRAY_PARTITION variable = update_in complete dim=0
			keyvalue_t update_out[NUM_VALID_PEs][EDGE_PACK_SIZE];
			#pragma HLS ARRAY_PARTITION variable = update_out complete dim=0
			
			EC_PROCESS_EDGES_LOOP1F: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				bool en = true; if(t >= limits[inst]){ en = false; } else { en = true; } 
				edge3_vec_dt edge_vec = edge3_vecs[inst];
				unsigned int rotateby = edge_vec.data[0].weight; 
				#ifdef _DEBUGMODE_CHECKS3
				checkoutofbounds("acts_kernel::ERROR 213::", rotateby, EDGE_PACK_SIZE, NAp, NAp, NAp);
				#endif
				
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					if(inst==0 && t<4 && edge3_vecs[inst].data[v].weight>1){ cout<<"[newly updated edge: edge["<<inst<<"]["<<t<<"]["<<v<<"]: srcvid: "<<edge3_vecs[inst].data[v].srcvid<<", dstvid: "<<edge3_vecs[inst].data[v].dstvid<<", weight: "<<edge3_vecs[inst].data[v].weight<<"]"<<endl; }
				}
				#endif 
				#ifdef _DEBUGMODE_CHECKS3
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					edge3_type edge = edge_vec.data[v];
					if(edge.srcvid != INVALIDDATA){
						checkoutofbounds("acts_kernel::ERROR 1213a::", edge.srcvid, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
						checkoutofbounds("acts_kernel::ERROR 1213b::", edge.dstvid, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
					}
				}
				#endif
				
				// read source properties and process edge to form vertex-updates 
				MY_LOOP16214: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				#pragma HLS UNROLL
					edge3_type edge = edge_vec.data[v];
					unsigned int srcvid_lpv = edge.srcvid; 
					vprop_t uprop; if(edge.srcvid != INVALIDDATA && edge.srcvid < MAX_UPARTITION_VECSIZE){ uprop = retrieve_vdatabuffer(v, srcvid_lpv, vdata_buffer[inst]); }
					if(edge.srcvid != INVALIDDATA && edge.srcvid < MAX_UPARTITION_VECSIZE && (uprop.mask == MASK_CODE_PE || all_vertices_active_in_all_iterations == true)){
						unsigned int res = process_funcG(uprop.prop, 1, globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM]);
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
				#ifdef _DEBUGMODE_CHECKS3___XXX
				if(en == true){ check_if_contiguous(update_out[inst], update_in[inst], update_in[inst], rotateby, 0, rotateby); }
				#endif		
				
				// collect stats 
				bool isvalid = true;
				if( update_out[inst][0].key==INVALIDDATA&& update_out[inst][1].key==INVALIDDATA&& update_out[inst][2].key==INVALIDDATA&& update_out[inst][3].key==INVALIDDATA&& update_out[inst][4].key==INVALIDDATA&& update_out[inst][5].key==INVALIDDATA&& update_out[inst][6].key==INVALIDDATA&& update_out[inst][7].key==INVALIDDATA&& update_out[inst][8].key==INVALIDDATA&& update_out[inst][9].key==INVALIDDATA&& update_out[inst][10].key==INVALIDDATA&& update_out[inst][11].key==INVALIDDATA&& update_out[inst][12].key==INVALIDDATA&& update_out[inst][13].key==INVALIDDATA&& update_out[inst][14].key==INVALIDDATA&& update_out[inst][15].key==INVALIDDATA){ isvalid = false; }
				if(en == true && isvalid == true){
					MY_LOOP16234: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						if(update_out[inst][v].key != INVALIDDATA){
							#ifndef FPGA_IMPL
							if(update_out[inst][v].key != INVALIDDATA){ checkoutofbounds("acts_kernel::ERROR 623::", ((update_out[inst][v].key / EDGE_PACK_SIZE) >> ACTVUPDATESBLOCK_VECSIZE_POW), BLOCKRAM_SIZE, update_out[inst][v].key, (update_out[inst][v].key / EDGE_PACK_SIZE), (MAX_APPLYPARTITION_VECSIZE / BLOCKRAM_SIZE)); }				
							#endif
							unsigned int dstvid_lpv = update_out[inst][v].key / EDGE_PACK_SIZE;
							if(update_out[inst][v].key != INVALIDDATA){ insert_statstmpbuffer((dstvid_lpv >> ACTVUPDATESBLOCK_VECSIZE_POW), v, 1, statsbuffer_maskbased_tmp[inst]); }
						}
					}
				}
				
				if(en == true && (isvalid == true || all_vertices_active_in_all_iterations == true)){	
					counts[inst] += 1;
					ens[inst][0] = true;									
				} else {
					ens[inst][0] = false;
				}
			}
			
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
			#pragma HLS UNROLL
				offsets3[n] = offsets2[n] + actpackupdates_dram___size[n][llp_set] + counts[n];
			}	
			dinsertmany_updatesdram(offsets3, update_out, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);	
			update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___ECPROCESSEDGES___, 1);
		}
		
		// update variables 
		MY_LOOP161: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			actpackupdates_dram___size[n][llp_set] += counts[n];
		}
		
		// gather stats
		unsigned int data[EDGE_PACK_SIZE];
		if(all_vertices_active_in_all_iterations == false){
			EC_PROCESS_EDGES_LOOP1I: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
			#pragma HLS PIPELINE II=1
				for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
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
		// exit(EXIT_SUCCESS);
	}
	// exit(EXIT_SUCCESS); 
}
// exit(EXIT_SUCCESS); 


							
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
unsigned int offsets1[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = offsets1 complete
unsigned int offsets2[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = offsets2 complete
unsigned int offsets3[NUM_VALID_PEs]; 
#pragma HLS ARRAY_PARTITION variable = offsets3 complete
if(sweep==0){
	SAVE_CSR_UPDATES_LOOP1: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
		unsigned int voffsett = p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION;
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			offsets1[n] = actpackupdates_dram___size[n][p_v]; offsets2[n] = actpackupdates_dram___size[n][p_v]; // NEW*
		} 
		max_limit = 0; for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ max_limits[inst] = 0; }
		SAVE_CSR_UPDATES_LOOP1B: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
		#pragma HLS UNROLL
			SAVE_CSR_UPDATES_LOOP1B1: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max_limits[inst] < updates_buffer___size[inst][v][p_v]){ max_limits[inst] = updates_buffer___size[inst][v][p_v]; }}
		} 
		SAVE_CSR_UPDATES_LOOP1C: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ if(max_limit < max_limits[inst]){ max_limit = max_limits[inst]; }}
		
		keyvalue_t updates_vecs[NUM_VALID_PEs][EDGE_PACK_SIZE]; 
		#pragma HLS ARRAY_PARTITION variable = updates_vecs complete dim=0
		keyvalue_t invalidkv; invalidkv.key = INVALIDDATA; invalidkv.value = INVALIDDATA;
		SAVE_CSR_UPDATES_LOOP1D: for(unsigned int t=0; t<max_limit; t++){ // FIXME? CAUSING HANGING?
		#pragma HLS PIPELINE II=1
			SAVE_CSR_UPDATES_LOOP1E: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
			#pragma HLS UNROLL
				SAVE_CSR_UPDATES_LOOP1F: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				#pragma HLS UNROLL
					if(t < updates_buffer___size[inst][v][p_v]){ 
						vprop_t tmp = retrieve_vdatabuffer(v, voffsett + t, vdata_buffer[inst]); 
						updates_vecs[inst][v].key = tmp.prop; updates_vecs[inst][v].value = tmp.mask;
						#ifdef _DEBUGMODE_CHECKS3
						if((updates_vecs[inst][v].key % EDGE_PACK_SIZE) != v){ cout<<"acts_kernel 1234:: updates_vecs["<<inst<<"]["<<v<<"].key("<<updates_vecs[inst][v].key<<") % v("<<v<<") != 0. EXITING..."<<endl; for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"updates_vecs["<<inst<<"].data["<<v1<<"].key ("<<(updates_vecs[inst][v].key % EDGE_PACK_SIZE)<<"): "<<updates_vecs[inst][v1].key<<""<<endl; } exit(EXIT_FAILURE); }				
						#endif 
					}
					#ifdef ___FALSE___
					else { updates_vecs[inst][v] = invalidkv; } // VHLS-RESOLUTION
					#endif 
					if(t < updates_buffer___size[inst][v][p_v]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					if(t < updates_buffer___size[inst][v][p_v]){ std::cout<<"VC - COMMIT CSR UPDATES SEEN @: [inst: "<<inst<<"], [updates_vecs["<<inst<<"]["<<v<<"].key: "<<updates_vecs[inst][v].key<<", updates_vecs["<<inst<<"]["<<v<<"].value: "<<updates_vecs[inst][v].value<<"]"<<std::endl; }
					#endif
				}
			}
			#ifndef FPGA_IMPL
			checkoutofbounds("acts_kernel::ERROR 152::", actpackupdates_dram___size[0][p_v], HBM_CHANNEL_SIZE, NAp, NAp, NAp);
			#endif 
			for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
			#pragma HLS UNROLL
				offsets3[n] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v] + offsets1[n] + t;
			}
			dinsertmany_updatesdram(offsets3, updates_vecs, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, updatesptrs);
			SAVE_CSR_UPDATES_LOOP1G: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
			#pragma HLS UNROLL
				if(t < max_limits[n]){ offsets2[n] += 1; }
			}
			update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___SAVEVCUPDATES___, 1);
		}
		for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			actpackupdates_dram___size[n][p_v] = offsets2[n]; 
		}
	}
}		
			}
			#endif 
		}
		// exit(EXIT_SUCCESS); ////////////////////////////////////////////////////////
		
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
					// if(p_v < __NUM_APPLYPARTITIONS){ // CRITICAL NEWCHANGE
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						std::cout<<"VC - PREPARE ACTIVE DESTIDS SEEN @: [inst: "<<inst<<"], [p_v: "<<p_v<<", t: "<<t<<"]"<<std::endl;
						#endif
						insert_stats2buffer(statsbuffer_idbased___size[inst][p_v], p_v, t, statsbuffer_idbased[inst]); 
						statsbuffer_idbased___size[inst][p_v] += 1;
					// }
				}
			}
		}
		update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECTACTIVEDSTVIDS___, 4); // CRITICAL NEWCHANGE
	}
}	
		}
		#endif 
		
		// apply updates
		unsigned int totalactvvs2 = 0; 
		#ifdef ___ENABLE___APPLYUPDATESMODULE___
		if(___ENABLE___APPLYUPDATESMODULE___BOOL___ == 1){
		frontier_t actvv[EDGE_PACK_SIZE]; 
		MY_LOOP311: for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ upartition_vertices[p_u].count = 0; }
		MY_LOOP312: for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ cfrontier_dram___size[p_u] = 0; } // reset

		unsigned int start_partition = batch * _ASYNC_BATCH_SIZE_;
		unsigned int stop_partition = (batch + 1) * _ASYNC_BATCH_SIZE_; 
		if(stop_partition > __NUM_APPLYPARTITIONS * 2){ stop_partition = (__NUM_APPLYPARTITIONS * 2) - ((batch + 1) * _ASYNC_BATCH_SIZE_); }
		
		// apply updates 
		APPLY_UPDATES_MODULE_LOOP: for(unsigned int p_v=start_partition; p_v<stop_partition; p_v++){
			#ifndef FPGA_IMPL
			checkoutofbounds("acts_kernel::ERROR 862::", p_v, __NUM_APPLYPARTITIONS, p_v, __NUM_APPLYPARTITIONS, NAp);
			#endif 
			
			bool en = true; if(enable___collectactivedstvids == true){ if(vpartition_vertices[0][p_v].count > 0){ en=true; } else { en=false; }} else { en = true; }
			unsigned int voffset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] + (p_v * MAX_APPLYPARTITION_VECSIZE);
			
			if(vpartition_vertices[0][p_v].count > 0 || all_vertices_active_in_all_iterations == true){ 
				#ifdef _DEBUGMODE_KERNELPRINTS4
				if(all_vertices_active_in_all_iterations == true && en == true){ cout<<"###>>> applying updates in vpartition "<<p_v<<"..."<<endl; }
				#endif
			
				// read destination properties
				#ifdef ___ENABLE___READ_DEST_PROPERTIES___
				if(___ENABLE___READ_DEST_PROPERTIES___BOOL___ == 1){
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
			dretrievemany_vdatadram(voffset, offsets, t, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
			for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
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
		dretrievemany_vdatadram(voffset, offsets, t, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
		READ_DEST_PROPERTIES_LOOP2C: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
		#pragma HLS UNROLL
			insertvec_vdatabuffer(t, datas[inst], vdata_buffer[inst]);
		}
		update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___READ_DEST_PROPERTIES___, 1);
	}
}
					
				}
				#endif 
				
				// apply updates [done]
				#ifdef ___ENABLE___APPLYUPDATES___II1___ 
				if(___ENABLE___APPLYUPDATES___BOOL___ == 1){
uint512_vec_dt updates_vecs[NUM_VALID_PEs];
max_limit = 0; for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ limits[inst] = actpackupdates_dram___size[inst][p_v]; } for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}

APPLY_UPDATES_LOOP1: for(unsigned int t=0; t<max_limit; t++){			
#pragma HLS PIPELINE II=1
	dretrievemany_udatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, t, updates_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, updatesptrs); // NEW
	MY_LOOP1521: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
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
					// vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, vdata_buffer[inst]);
					vprop_t vprop; vprop.prop = dstvid_lpv; // REMOVEME.
					unsigned int new_vprop = reduce_funcG(vprop.prop, vprop.prop, update.value, globalparams[GLOBALPARAMSCODE__PARAM__ALGORITHM]);
					if((all_vertices_active_in_all_iterations == true || new_vprop != vprop.prop) && t < limits[inst]){ 
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						std::cout<<"APPLY (ACT-PACK) UPDATE SEEN @: t: "<<t<<", inst: ["<<inst<<"]: dstvid_lp: "<<dstvid_lp<<", dstvid_lpv: "<<dstvid_lpv<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<std::endl;
						#endif
						vprop_t newprop; newprop.prop = new_vprop; newprop.mask = MASK_CODE_AU;
						insert_vdatabuffer(v, dstvid_lpv, newprop, vdata_buffer[inst]);
						
						if(inst==0 && v==0){ update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___APPLYUPDATES___, 1); }
					}
				}
			}
		}
	}
	// update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___APPLYUPDATES___, 1);
}	
					 
				}
				#endif 
				
				// collect and save frontier information
				#ifdef ___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___
				if(___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___BOOL___ == 1){
MY_LOOP234: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ offsets[n] = 0; }
MY_LOOP235: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
#pragma HLS UNROLL
	MY_LOOP236: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		MY_LOOP237: for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){ nfrontier_buffer___size[n][v][p_actvv] = 0; }
	}
}
MY_LOOP238: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
#pragma HLS UNROLL
	vid_first0[n] = get_global(((p_v * MAX_APPLYPARTITION_SIZE) + ((0 * MAX_ACTVV_VECSIZE + 0) * EDGE_PACK_SIZE + 0)), n); 
}

COLLECT_AND_SAVE_FRONTIER_INFO_LOOP1: for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
	unsigned int temp_[NUM_VALID_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=temp_ complete dim=0
	unsigned int temp_2_[NUM_VALID_PEs];
	#pragma HLS ARRAY_PARTITION variable=temp_2_ complete
	unsigned int p_actvv_ = (p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION) + p_actvv;
	MY_LOOP100: for(unsigned int n=0; n<NUM_VALID_PEs; n++){
	#pragma HLS UNROLL
		MY_LOOP101: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			temp_[n][v] = nfrontier_buffer___size[n][v][p_actvv];
		}
		temp_2_[n] = nfrontier_dram___size[n][p_actvv_];
	}
	
	// collect frontier information [done]
	MY_LOOP103: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
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
		unsigned int mask = 1; if(enable___collectactivedstvids == true){ mask = statsbuffer_maskbased[0][p_v][offset_aup]+statsbuffer_maskbased[1][p_v][offset_aup]+statsbuffer_maskbased[2][p_v][offset_aup]+statsbuffer_maskbased[3][p_v][offset_aup]+statsbuffer_maskbased[4][p_v][offset_aup]+statsbuffer_maskbased[5][p_v][offset_aup]; } else { mask = 1; }
		if(mask > 0){ 	
			COLLECT_FRONTIER_INFO_LOOP1B: for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
			#pragma HLS PIPELINE II=1
				unsigned int dstvid_lpv = baseoffset0_aup + baseoffset1_aup + t;
				bool en; if(baseoffset1_aup + t < MAX_ACTVV_VECSIZE && dstvid_lpv < MAX_APPLYPARTITION_VECSIZE){ en = true; } else { en = false; }
				MY_LOOP114: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
				#pragma HLS UNROLL
					MY_LOOP104: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						if(en==true){ 
							#ifdef _DEBUGMODE_CHECKS3
							checkoutofbounds("acts_kernel::ERROR 713::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, p_actvv, au_p, t); 
							#endif	
							vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, vdata_buffer[inst]);
							unsigned int vid = vid_first1[inst] + v;
							#ifdef _DEBUGMODE_CHECKS3
							if((vid % EDGE_PACK_SIZE != v)){ cout<<"acts_kernel:: ERROR 234. vid("<<vid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<vid % EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
							#endif
							if(all_vertices_active_in_all_iterations == true || vprop.mask == MASK_CODE_AU){ 
								#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
								std::cout<<"COLLECT FRONTIER INFORMATION SEEN @: inst: ["<<inst<<"]: dstvid_lpv: "<<dstvid_lpv<<", v: "<<v<<", p__u__: "<<(vid / MAX_UPARTITION_SIZE)<<", vid: "<<vid<<std::endl;
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
				MY_LOOP105: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
				#pragma HLS UNROLL
					vid_first1[inst] += CONST1; 
				}
				update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___, 1);
			}
		}
		baseoffset1_aup += ACTVUPDATESBLOCK_VECSIZE;
	}
	
	// save frontier information [done]
	unsigned int max_limit = 0; for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ max_limits[inst] = 0; }
	MY_LOOP106: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
		MY_LOOP107: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){
		#pragma HLS UNROLL
			if(max_limits[inst] < temp_[inst][v]){ max_limits[inst] = temp_[inst][v]; }
		}
	}
	MY_LOOP108: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ if(max_limit < max_limits[inst]){ max_limit = max_limits[inst]; }}
	unsigned int offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + (p_actvv_ * MAX_APPLYPARTITION_VECSIZE);
	keyvalue_t invalidkv; invalidkv.key = INVALIDDATA; invalidkv.value = INVALIDDATA;
	SAVE_FRONTIER_INFO_LOOP2B: for(unsigned int t=0; t<max_limit; t++){
	#pragma HLS PIPELINE II=1
		MY_LOOP1092: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
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
		MY_LOOP1019: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			offsets2[n] = offset + offsets[n] + t;
		}
		dinsertmany_nfrontierdram(offset, p_actvv_, offsets2, t, actvvs, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
		MY_LOOP110: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			if(t < max_limits[n]){ temp_2_[n] += 1; }
		}
		update_bramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___, 1);
	}
	
	MY_LOOP111: for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ 
	#pragma HLS UNROLL
		vid_first0[inst] += CONST2; 
	}
	MY_LOOP112: for(unsigned int n=0; n<NUM_VALID_PEs; n++){
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
					retrievevec_vdatabuffer(offsets[inst] + t, datas[inst], vdata_buffer[inst]);
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
			dinsertmany_vdatadram(voffset, offsets2, t, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
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
			retrieveandresetvec_vdatabuffer(t, datas[inst], vdata_buffer[inst]);
		}
		MY_LOOP10202: for(unsigned int n=0; n<NUM_VALID_PEs; n++){ 
		#pragma HLS UNROLL
			offsets2[n] = voffset + offsets[n] + t; 
		}
		dinsertmany_vdatadram(voffset, offsets2, t, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
		update_dramnumclockcycles(_NUMCLOCKCYCLES_, ___CODE___SAVE_DEST_PROPERTIES___, 1);
	}
}
				
				}
				#endif 
			}
		} 
		
		// transport dest to center and ddr (for export to remote fpga)
		GATHER_FRONTIERS_MODULE_LOOP: for(unsigned int p_v=start_partition; p_v<stop_partition; p_v++){
			#ifdef _DEBUGMODE_KERNELPRINTS4
			if(vpartition_vertices[0][p_v].count > 0 || all_vertices_active_in_all_iterations == true){ cout<<"###>>> gathering vertex properties in "<<p_v<<" for transport..."<<endl; }
			#endif
				
			#ifdef ___ENABLE___EXCHANGEFRONTIERINFOS___
			if(___ENABLE___EXCHANGEFRONTIERINFOS___BOOL___ == 1){
transport_frontier(0, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[0], upartition_vertices, HBM_channelA0, HBM_channelB0, HBM_centerA, HBM_centerB, &totalactvvs2);
transport_frontier(1, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[1], upartition_vertices, HBM_channelA1, HBM_channelB1, HBM_centerA, HBM_centerB, &totalactvvs2);
transport_frontier(2, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[2], upartition_vertices, HBM_channelA2, HBM_channelB2, HBM_centerA, HBM_centerB, &totalactvvs2);
transport_frontier(3, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[3], upartition_vertices, HBM_channelA3, HBM_channelB3, HBM_centerA, HBM_centerB, &totalactvvs2);
transport_frontier(4, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[4], upartition_vertices, HBM_channelA4, HBM_channelB4, HBM_centerA, HBM_centerB, &totalactvvs2);
transport_frontier(5, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[5], upartition_vertices, HBM_channelA5, HBM_channelB5, HBM_centerA, HBM_centerB, &totalactvvs2);
			
			}
			#endif 
		}
		}
		#endif
		
		#ifdef ___ENABLE___REPORT_STATISTICS___
#ifdef _DEBUGMODE_CHECKS3 
#ifdef _DEBUGMODE_KERNELPRINTS4
if(all_vertices_active_in_all_iterations == true){ 
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___RESETBUFFERSATSTART___: "<<_NUMCLOCKCYCLES_[0][___CODE___RESETBUFFERSATSTART___]<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___READ_FRONTIER_PROPERTIES___: "<<_NUMCLOCKCYCLES_[0][___CODE___READ_FRONTIER_PROPERTIES___]<<" (total # vertices: "<<_NUMCLOCKCYCLES_[0][___CODE___READ_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<")"<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___VCPROCESSEDGES___: "<<_NUMCLOCKCYCLES_[0][___CODE___VCPROCESSEDGES___]<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___ECPROCESSEDGES___: "<<_NUMCLOCKCYCLES_[0][___CODE___ECPROCESSEDGES___]<<" (# edges per hbm channel: "<<_NUMCLOCKCYCLES_[0][___CODE___ECPROCESSEDGES___] * EDGE_PACK_SIZE<<")"<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___SAVEVCUPDATES___: "<<_NUMCLOCKCYCLES_[0][___CODE___SAVEVCUPDATES___]<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___COLLECTACTIVEDSTVIDS___: "<<_NUMCLOCKCYCLES_[0][___CODE___COLLECTACTIVEDSTVIDS___]<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___READ_DEST_PROPERTIES___: "<<_NUMCLOCKCYCLES_[0][___CODE___READ_DEST_PROPERTIES___]<<" (total # vertices: "<<_NUMCLOCKCYCLES_[0][___CODE___READ_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<")"<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___APPLYUPDATES___: "<<_NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATES___]<<" (# updates per hbm channel: "<<_NUMCLOCKCYCLES_[0][___CODE___APPLYUPDATES___] * EDGE_PACK_SIZE<<")"<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___: "<<_NUMCLOCKCYCLES_[0][___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___]<<" (total # frontiers: "<<_NUMCLOCKCYCLES_[0][___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<")"<<endl;			
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___SAVE_DEST_PROPERTIES___: "<<_NUMCLOCKCYCLES_[0][___CODE___SAVE_DEST_PROPERTIES___]<<" (total # vertices: "<<_NUMCLOCKCYCLES_[0][___CODE___SAVE_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<")"<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___EXCHANGEFRONTIERINFOS___: "<<_NUMCLOCKCYCLES_[0][___CODE___EXCHANGEFRONTIERINFOS___]<<" (total # frontiers: "<<_NUMCLOCKCYCLES_[0][___CODE___EXCHANGEFRONTIERINFOS___] * EDGE_PACK_SIZE * NUM_PEs<<")"<<endl;

cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___NUMBER_OF_EDGEUPDATES_LOADED___: "<<_NUMCLOCKCYCLES_[0][___CODE___NUMBER_OF_EDGEUPDATES_LOADED___]<<endl; // " (# edges per hbm channel: "<<_NUMCLOCKCYCLES_[0][___CODE___NUMBER_OF_EDGEUPDATES_LOADED___] * EDGE_PACK_SIZE<<")"<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___NUMBER_OF_EDGEUPDATES_APPLIED___: "<<_NUMCLOCKCYCLES_[0][___CODE___NUMBER_OF_EDGEUPDATES_APPLIED___]<<endl;
cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen @ ___CODE___NUMBER_OF_NEW_EDGEUPDATES_ADDED___: "<<_NUMCLOCKCYCLES_[0][___CODE___NUMBER_OF_NEW_EDGEUPDATES_ADDED___]<<endl;

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
		#endif 
			
		#ifndef HW // _DEBUGMODE_KERNELPRINTS4
		for(unsigned int t=0; t<__NUM_UPARTITIONS; t++){ if(hybrid_map[GraphIter][t] == 0){ std::cout<<"V, "; } else if(hybrid_map[GraphIter][t] == 1){ std::cout<<"E, "; } else { std::cout<<"-, "; }} std::cout<<std::endl;
		std::cout<<""<<std::endl;
		std::cout<< TIMINGRESULTSCOLOR <<"### GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<< RESET << std::endl; 
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ if(upartition_vertices[p_u].count > 0){ std::cout<<"1, "; } else { std::cout<<"0, "; }} std::cout<<std::endl;
		if(totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS){ std::cout<<"no more active vertices to process. breaking out... "<<std::endl; }
		#endif 
		total_num_actvvs += totalactvvs2;
		if(totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS){ maxGraphIter = GraphIter; break; }
		for(unsigned int inst=0; inst<NUM_VALID_PEs; inst++){ for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ nfrontier_dram___size[inst][p_u] = 0; }}
	} // NUM_ITs
	}

	#ifdef _DEBUGMODE_CHECKS3
	float time_lapse = (total_cycles * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000;
	float million_edges_per_sec = (((globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES] * globalparams[GLOBALPARAMSCODE__PARAM__NUM_RUNS]) / 1000000) / time_lapse) * 1000;
	float billion_edges_per_sec = million_edges_per_sec / 1000;
	cout<<"acts_kernel SUMMARY:: total number of active vertices processed / HBM CHANNEL: "<<total_num_actvvs<<endl;
	cout<<"acts_kernel SUMMARY:: total number of edges processed per run: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES]<<endl;
	cout<<"acts_kernel SUMMARY:: total number of runs: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_RUNS]<<endl;
	cout<<"acts_kernel SUMMARY:: number of DRAM clock cycles seen: "<<total_cycles<<" ("<<(total_cycles * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000<<" milliseconds)"<<endl;
	cout<< TIMINGRESULTSCOLOR << ">>> acts_kernel SUMMARY:: TIME ELAPSE: "<<time_lapse<<" ms. THROUGHPUT: "<<million_edges_per_sec<<" MTEPS; THROUGHPUT: "<<billion_edges_per_sec<<" BTEPS"<< RESET << endl;
	#endif
	return;
}

#define TEST_ME
#define TEST_VECSIZE 512

#ifndef TEST_ME
extern "C" {	
MY_IFDEF_TOPLEVELFUNC(){		
#pragma HLS INTERFACE m_axi port = HBM_channelA0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = HBM_channelB0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = HBM_channelA1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = HBM_channelB1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = HBM_channelA2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = HBM_channelB2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = HBM_channelA3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = HBM_channelB3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = HBM_channelA4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = HBM_channelB4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = HBM_channelA5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = HBM_channelB5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = HBM_centerA offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = HBM_centerB offset = slave bundle = gmem13

#pragma HLS INTERFACE s_axilite port = HBM_channelA0
#pragma HLS INTERFACE s_axilite port = HBM_channelB0
#pragma HLS INTERFACE s_axilite port = HBM_channelA1
#pragma HLS INTERFACE s_axilite port = HBM_channelB1
#pragma HLS INTERFACE s_axilite port = HBM_channelA2
#pragma HLS INTERFACE s_axilite port = HBM_channelB2
#pragma HLS INTERFACE s_axilite port = HBM_channelA3
#pragma HLS INTERFACE s_axilite port = HBM_channelB3
#pragma HLS INTERFACE s_axilite port = HBM_channelA4
#pragma HLS INTERFACE s_axilite port = HBM_channelB4
#pragma HLS INTERFACE s_axilite port = HBM_channelA5
#pragma HLS INTERFACE s_axilite port = HBM_channelB5
	
#pragma HLS INTERFACE s_axilite port = HBM_centerA
#pragma HLS INTERFACE s_axilite port = HBM_centerB

#pragma HLS INTERFACE s_axilite port = batch
#pragma HLS INTERFACE s_axilite port = return

	next_function( HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_centerA, HBM_centerB, batch);
		
	return;
}
}
#endif 

#ifdef TEST_ME
extern "C" {	
MY_IFDEF_TOPLEVELFUNC(){		
#pragma HLS INTERFACE m_axi port = HBM_channelA0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = HBM_channelB0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = HBM_channelA1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = HBM_channelB1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = HBM_channelA2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = HBM_channelB2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = HBM_channelA3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = HBM_channelB3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = HBM_channelA4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = HBM_channelB4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = HBM_channelA5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = HBM_channelB5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = HBM_centerA offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = HBM_centerB offset = slave bundle = gmem13

#pragma HLS INTERFACE s_axilite port = HBM_channelA0
#pragma HLS INTERFACE s_axilite port = HBM_channelB0
#pragma HLS INTERFACE s_axilite port = HBM_channelA1
#pragma HLS INTERFACE s_axilite port = HBM_channelB1
#pragma HLS INTERFACE s_axilite port = HBM_channelA2
#pragma HLS INTERFACE s_axilite port = HBM_channelB2
#pragma HLS INTERFACE s_axilite port = HBM_channelA3
#pragma HLS INTERFACE s_axilite port = HBM_channelB3
#pragma HLS INTERFACE s_axilite port = HBM_channelA4
#pragma HLS INTERFACE s_axilite port = HBM_channelB4
#pragma HLS INTERFACE s_axilite port = HBM_channelA5
#pragma HLS INTERFACE s_axilite port = HBM_channelB5
	
#pragma HLS INTERFACE s_axilite port = HBM_centerA
#pragma HLS INTERFACE s_axilite port = HBM_centerB

#pragma HLS INTERFACE s_axilite port = batch
#pragma HLS INTERFACE s_axilite port = return

	#ifdef FPGA_IMPL
	HBM_channelAXI_t dram_bufferA0[TEST_VECSIZE];
	HBM_channelAXI_t dram_bufferB0[TEST_VECSIZE];
	// #pragma HLS resource variable=dram_bufferA0 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferA0 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferA0 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferA0 
	// #pragma HLS resource variable=dram_bufferB0 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferB0 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferB0 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferB0 
	HBM_channelAXI_t dram_bufferA1[TEST_VECSIZE];
	HBM_channelAXI_t dram_bufferB1[TEST_VECSIZE];
	// #pragma HLS resource variable=dram_bufferA1 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferA1 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferA1 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferA1 
	// #pragma HLS resource variable=dram_bufferB1 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferB1 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferB1 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferB1 
	HBM_channelAXI_t dram_bufferA2[TEST_VECSIZE];
	HBM_channelAXI_t dram_bufferB2[TEST_VECSIZE];
	// #pragma HLS resource variable=dram_bufferA2 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferA2 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferA2 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferA2 
	// #pragma HLS resource variable=dram_bufferB2 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferB2 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferB2 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferB2 
	HBM_channelAXI_t dram_bufferA3[TEST_VECSIZE];
	HBM_channelAXI_t dram_bufferB3[TEST_VECSIZE];
	// #pragma HLS resource variable=dram_bufferA3 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferA3 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferA3 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferA3 
	// #pragma HLS resource variable=dram_bufferB3 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferB3 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferB3 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferB3 
	HBM_channelAXI_t dram_bufferA4[TEST_VECSIZE];
	HBM_channelAXI_t dram_bufferB4[TEST_VECSIZE];
	// #pragma HLS resource variable=dram_bufferA4 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferA4 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferA4 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferA4 
	// #pragma HLS resource variable=dram_bufferB4 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferB4 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferB4 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferB4 
	HBM_channelAXI_t dram_bufferA5[TEST_VECSIZE];
	HBM_channelAXI_t dram_bufferB5[TEST_VECSIZE];
	// #pragma HLS resource variable=dram_bufferA5 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferA5 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferA5 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferA5 
	// #pragma HLS resource variable=dram_bufferB5 core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_bufferB5 //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_bufferB5 complete dim=2 
	// #pragma HLS aggregate variable=dram_bufferB5 
	#else 
	HBM_channelAXI_t * dram_bufferA0;
	HBM_channelAXI_t * dram_bufferB0;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferA0 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferB0 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	HBM_channelAXI_t * dram_bufferA1;
	HBM_channelAXI_t * dram_bufferB1;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferA1 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferB1 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	HBM_channelAXI_t * dram_bufferA2;
	HBM_channelAXI_t * dram_bufferB2;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferA2 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferB2 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	HBM_channelAXI_t * dram_bufferA3;
	HBM_channelAXI_t * dram_bufferB3;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferA3 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferB3 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	HBM_channelAXI_t * dram_bufferA4;
	HBM_channelAXI_t * dram_bufferB4;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferA4 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferB4 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	HBM_channelAXI_t * dram_bufferA5;
	HBM_channelAXI_t * dram_bufferB5;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferA5 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_bufferB5 = new HBM_channelAXI_t[TEST_VECSIZE]; }
	#endif 
	
	#ifdef FPGA_IMPL
	HBM_channelAXI_t dram_centerbufferA[TEST_VECSIZE];
	HBM_channelAXI_t dram_centerbufferB[TEST_VECSIZE];
	// #pragma HLS resource variable=dram_centerbufferA core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_centerbufferA //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_centerbufferA complete dim=2 
	// #pragma HLS aggregate variable=dram_centerbufferA 
	// #pragma HLS resource variable=dram_centerbufferB core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=dram_centerbufferB //complete dim=1
	// #pragma HLS ARRAY_PARTITION variable=dram_centerbufferB complete dim=2 
	// #pragma HLS aggregate variable=dram_centerbufferB 
	#else 
	HBM_channelAXI_t * dram_centerbufferA;	
	HBM_channelAXI_t * dram_centerbufferB;	
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_centerbufferA = new HBM_channelAXI_t[TEST_VECSIZE]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ dram_centerbufferB = new HBM_channelAXI_t[TEST_VECSIZE]; }
	#endif 
	
	unsigned int offset_n = batch;
	
	for(unsigned int i=0; i<batch; i++){
		unsigned int offset_n = i * batch;
		
		BASELOAD_LOOP1: for(unsigned int t=0; t<TEST_VECSIZE; t++){
		#pragma HLS PIPELINE II=1
			dram_bufferA0[t].data[0] = HBM_channelA0[offset_n + t].data[0];
			dram_bufferA0[t].data[1] = HBM_channelA0[offset_n + t].data[1];
			dram_bufferA0[t].data[2] = HBM_channelA0[offset_n + t].data[2];
			dram_bufferA0[t].data[3] = HBM_channelA0[offset_n + t].data[3];
			dram_bufferA0[t].data[4] = HBM_channelA0[offset_n + t].data[4];
			dram_bufferA0[t].data[5] = HBM_channelA0[offset_n + t].data[5];
			dram_bufferA0[t].data[6] = HBM_channelA0[offset_n + t].data[6];
			dram_bufferA0[t].data[7] = HBM_channelA0[offset_n + t].data[7];
			dram_bufferA0[t].data[8] = HBM_channelA0[offset_n + t].data[8];
			dram_bufferA0[t].data[9] = HBM_channelA0[offset_n + t].data[9];
			dram_bufferA0[t].data[10] = HBM_channelA0[offset_n + t].data[10];
			dram_bufferA0[t].data[11] = HBM_channelA0[offset_n + t].data[11];
			dram_bufferA0[t].data[12] = HBM_channelA0[offset_n + t].data[12];
			dram_bufferA0[t].data[13] = HBM_channelA0[offset_n + t].data[13];
			dram_bufferA0[t].data[14] = HBM_channelA0[offset_n + t].data[14];
			dram_bufferA0[t].data[15] = HBM_channelA0[offset_n + t].data[15];
			dram_bufferA1[t].data[0] = HBM_channelA1[offset_n + t].data[0];
			dram_bufferA1[t].data[1] = HBM_channelA1[offset_n + t].data[1];
			dram_bufferA1[t].data[2] = HBM_channelA1[offset_n + t].data[2];
			dram_bufferA1[t].data[3] = HBM_channelA1[offset_n + t].data[3];
			dram_bufferA1[t].data[4] = HBM_channelA1[offset_n + t].data[4];
			dram_bufferA1[t].data[5] = HBM_channelA1[offset_n + t].data[5];
			dram_bufferA1[t].data[6] = HBM_channelA1[offset_n + t].data[6];
			dram_bufferA1[t].data[7] = HBM_channelA1[offset_n + t].data[7];
			dram_bufferA1[t].data[8] = HBM_channelA1[offset_n + t].data[8];
			dram_bufferA1[t].data[9] = HBM_channelA1[offset_n + t].data[9];
			dram_bufferA1[t].data[10] = HBM_channelA1[offset_n + t].data[10];
			dram_bufferA1[t].data[11] = HBM_channelA1[offset_n + t].data[11];
			dram_bufferA1[t].data[12] = HBM_channelA1[offset_n + t].data[12];
			dram_bufferA1[t].data[13] = HBM_channelA1[offset_n + t].data[13];
			dram_bufferA1[t].data[14] = HBM_channelA1[offset_n + t].data[14];
			dram_bufferA1[t].data[15] = HBM_channelA1[offset_n + t].data[15];
			dram_bufferA2[t].data[0] = HBM_channelA2[offset_n + t].data[0];
			dram_bufferA2[t].data[1] = HBM_channelA2[offset_n + t].data[1];
			dram_bufferA2[t].data[2] = HBM_channelA2[offset_n + t].data[2];
			dram_bufferA2[t].data[3] = HBM_channelA2[offset_n + t].data[3];
			dram_bufferA2[t].data[4] = HBM_channelA2[offset_n + t].data[4];
			dram_bufferA2[t].data[5] = HBM_channelA2[offset_n + t].data[5];
			dram_bufferA2[t].data[6] = HBM_channelA2[offset_n + t].data[6];
			dram_bufferA2[t].data[7] = HBM_channelA2[offset_n + t].data[7];
			dram_bufferA2[t].data[8] = HBM_channelA2[offset_n + t].data[8];
			dram_bufferA2[t].data[9] = HBM_channelA2[offset_n + t].data[9];
			dram_bufferA2[t].data[10] = HBM_channelA2[offset_n + t].data[10];
			dram_bufferA2[t].data[11] = HBM_channelA2[offset_n + t].data[11];
			dram_bufferA2[t].data[12] = HBM_channelA2[offset_n + t].data[12];
			dram_bufferA2[t].data[13] = HBM_channelA2[offset_n + t].data[13];
			dram_bufferA2[t].data[14] = HBM_channelA2[offset_n + t].data[14];
			dram_bufferA2[t].data[15] = HBM_channelA2[offset_n + t].data[15];
			dram_bufferA3[t].data[0] = HBM_channelA3[offset_n + t].data[0];
			dram_bufferA3[t].data[1] = HBM_channelA3[offset_n + t].data[1];
			dram_bufferA3[t].data[2] = HBM_channelA3[offset_n + t].data[2];
			dram_bufferA3[t].data[3] = HBM_channelA3[offset_n + t].data[3];
			dram_bufferA3[t].data[4] = HBM_channelA3[offset_n + t].data[4];
			dram_bufferA3[t].data[5] = HBM_channelA3[offset_n + t].data[5];
			dram_bufferA3[t].data[6] = HBM_channelA3[offset_n + t].data[6];
			dram_bufferA3[t].data[7] = HBM_channelA3[offset_n + t].data[7];
			dram_bufferA3[t].data[8] = HBM_channelA3[offset_n + t].data[8];
			dram_bufferA3[t].data[9] = HBM_channelA3[offset_n + t].data[9];
			dram_bufferA3[t].data[10] = HBM_channelA3[offset_n + t].data[10];
			dram_bufferA3[t].data[11] = HBM_channelA3[offset_n + t].data[11];
			dram_bufferA3[t].data[12] = HBM_channelA3[offset_n + t].data[12];
			dram_bufferA3[t].data[13] = HBM_channelA3[offset_n + t].data[13];
			dram_bufferA3[t].data[14] = HBM_channelA3[offset_n + t].data[14];
			dram_bufferA3[t].data[15] = HBM_channelA3[offset_n + t].data[15];
			dram_bufferA4[t].data[0] = HBM_channelA4[offset_n + t].data[0];
			dram_bufferA4[t].data[1] = HBM_channelA4[offset_n + t].data[1];
			dram_bufferA4[t].data[2] = HBM_channelA4[offset_n + t].data[2];
			dram_bufferA4[t].data[3] = HBM_channelA4[offset_n + t].data[3];
			dram_bufferA4[t].data[4] = HBM_channelA4[offset_n + t].data[4];
			dram_bufferA4[t].data[5] = HBM_channelA4[offset_n + t].data[5];
			dram_bufferA4[t].data[6] = HBM_channelA4[offset_n + t].data[6];
			dram_bufferA4[t].data[7] = HBM_channelA4[offset_n + t].data[7];
			dram_bufferA4[t].data[8] = HBM_channelA4[offset_n + t].data[8];
			dram_bufferA4[t].data[9] = HBM_channelA4[offset_n + t].data[9];
			dram_bufferA4[t].data[10] = HBM_channelA4[offset_n + t].data[10];
			dram_bufferA4[t].data[11] = HBM_channelA4[offset_n + t].data[11];
			dram_bufferA4[t].data[12] = HBM_channelA4[offset_n + t].data[12];
			dram_bufferA4[t].data[13] = HBM_channelA4[offset_n + t].data[13];
			dram_bufferA4[t].data[14] = HBM_channelA4[offset_n + t].data[14];
			dram_bufferA4[t].data[15] = HBM_channelA4[offset_n + t].data[15];
			dram_bufferA5[t].data[0] = HBM_channelA5[offset_n + t].data[0];
			dram_bufferA5[t].data[1] = HBM_channelA5[offset_n + t].data[1];
			dram_bufferA5[t].data[2] = HBM_channelA5[offset_n + t].data[2];
			dram_bufferA5[t].data[3] = HBM_channelA5[offset_n + t].data[3];
			dram_bufferA5[t].data[4] = HBM_channelA5[offset_n + t].data[4];
			dram_bufferA5[t].data[5] = HBM_channelA5[offset_n + t].data[5];
			dram_bufferA5[t].data[6] = HBM_channelA5[offset_n + t].data[6];
			dram_bufferA5[t].data[7] = HBM_channelA5[offset_n + t].data[7];
			dram_bufferA5[t].data[8] = HBM_channelA5[offset_n + t].data[8];
			dram_bufferA5[t].data[9] = HBM_channelA5[offset_n + t].data[9];
			dram_bufferA5[t].data[10] = HBM_channelA5[offset_n + t].data[10];
			dram_bufferA5[t].data[11] = HBM_channelA5[offset_n + t].data[11];
			dram_bufferA5[t].data[12] = HBM_channelA5[offset_n + t].data[12];
			dram_bufferA5[t].data[13] = HBM_channelA5[offset_n + t].data[13];
			dram_bufferA5[t].data[14] = HBM_channelA5[offset_n + t].data[14];
			dram_bufferA5[t].data[15] = HBM_channelA5[offset_n + t].data[15];
			dram_bufferB0[t].data[0] = HBM_channelB0[offset_n + t].data[0];
			dram_bufferB0[t].data[1] = HBM_channelB0[offset_n + t].data[1];
			dram_bufferB0[t].data[2] = HBM_channelB0[offset_n + t].data[2];
			dram_bufferB0[t].data[3] = HBM_channelB0[offset_n + t].data[3];
			dram_bufferB0[t].data[4] = HBM_channelB0[offset_n + t].data[4];
			dram_bufferB0[t].data[5] = HBM_channelB0[offset_n + t].data[5];
			dram_bufferB0[t].data[6] = HBM_channelB0[offset_n + t].data[6];
			dram_bufferB0[t].data[7] = HBM_channelB0[offset_n + t].data[7];
			dram_bufferB0[t].data[8] = HBM_channelB0[offset_n + t].data[8];
			dram_bufferB0[t].data[9] = HBM_channelB0[offset_n + t].data[9];
			dram_bufferB0[t].data[10] = HBM_channelB0[offset_n + t].data[10];
			dram_bufferB0[t].data[11] = HBM_channelB0[offset_n + t].data[11];
			dram_bufferB0[t].data[12] = HBM_channelB0[offset_n + t].data[12];
			dram_bufferB0[t].data[13] = HBM_channelB0[offset_n + t].data[13];
			dram_bufferB0[t].data[14] = HBM_channelB0[offset_n + t].data[14];
			dram_bufferB0[t].data[15] = HBM_channelB0[offset_n + t].data[15];
	
			dram_bufferB1[t].data[0] = HBM_channelB1[offset_n + t].data[0];
			dram_bufferB1[t].data[1] = HBM_channelB1[offset_n + t].data[1];
			dram_bufferB1[t].data[2] = HBM_channelB1[offset_n + t].data[2];
			dram_bufferB1[t].data[3] = HBM_channelB1[offset_n + t].data[3];
			dram_bufferB1[t].data[4] = HBM_channelB1[offset_n + t].data[4];
			dram_bufferB1[t].data[5] = HBM_channelB1[offset_n + t].data[5];
			dram_bufferB1[t].data[6] = HBM_channelB1[offset_n + t].data[6];
			dram_bufferB1[t].data[7] = HBM_channelB1[offset_n + t].data[7];
			dram_bufferB1[t].data[8] = HBM_channelB1[offset_n + t].data[8];
			dram_bufferB1[t].data[9] = HBM_channelB1[offset_n + t].data[9];
			dram_bufferB1[t].data[10] = HBM_channelB1[offset_n + t].data[10];
			dram_bufferB1[t].data[11] = HBM_channelB1[offset_n + t].data[11];
			dram_bufferB1[t].data[12] = HBM_channelB1[offset_n + t].data[12];
			dram_bufferB1[t].data[13] = HBM_channelB1[offset_n + t].data[13];
			dram_bufferB1[t].data[14] = HBM_channelB1[offset_n + t].data[14];
			dram_bufferB1[t].data[15] = HBM_channelB1[offset_n + t].data[15];
	
			dram_bufferB2[t].data[0] = HBM_channelB2[offset_n + t].data[0];
			dram_bufferB2[t].data[1] = HBM_channelB2[offset_n + t].data[1];
			dram_bufferB2[t].data[2] = HBM_channelB2[offset_n + t].data[2];
			dram_bufferB2[t].data[3] = HBM_channelB2[offset_n + t].data[3];
			dram_bufferB2[t].data[4] = HBM_channelB2[offset_n + t].data[4];
			dram_bufferB2[t].data[5] = HBM_channelB2[offset_n + t].data[5];
			dram_bufferB2[t].data[6] = HBM_channelB2[offset_n + t].data[6];
			dram_bufferB2[t].data[7] = HBM_channelB2[offset_n + t].data[7];
			dram_bufferB2[t].data[8] = HBM_channelB2[offset_n + t].data[8];
			dram_bufferB2[t].data[9] = HBM_channelB2[offset_n + t].data[9];
			dram_bufferB2[t].data[10] = HBM_channelB2[offset_n + t].data[10];
			dram_bufferB2[t].data[11] = HBM_channelB2[offset_n + t].data[11];
			dram_bufferB2[t].data[12] = HBM_channelB2[offset_n + t].data[12];
			dram_bufferB2[t].data[13] = HBM_channelB2[offset_n + t].data[13];
			dram_bufferB2[t].data[14] = HBM_channelB2[offset_n + t].data[14];
			dram_bufferB2[t].data[15] = HBM_channelB2[offset_n + t].data[15];
	
			dram_bufferB3[t].data[0] = HBM_channelB3[offset_n + t].data[0];
			dram_bufferB3[t].data[1] = HBM_channelB3[offset_n + t].data[1];
			dram_bufferB3[t].data[2] = HBM_channelB3[offset_n + t].data[2];
			dram_bufferB3[t].data[3] = HBM_channelB3[offset_n + t].data[3];
			dram_bufferB3[t].data[4] = HBM_channelB3[offset_n + t].data[4];
			dram_bufferB3[t].data[5] = HBM_channelB3[offset_n + t].data[5];
			dram_bufferB3[t].data[6] = HBM_channelB3[offset_n + t].data[6];
			dram_bufferB3[t].data[7] = HBM_channelB3[offset_n + t].data[7];
			dram_bufferB3[t].data[8] = HBM_channelB3[offset_n + t].data[8];
			dram_bufferB3[t].data[9] = HBM_channelB3[offset_n + t].data[9];
			dram_bufferB3[t].data[10] = HBM_channelB3[offset_n + t].data[10];
			dram_bufferB3[t].data[11] = HBM_channelB3[offset_n + t].data[11];
			dram_bufferB3[t].data[12] = HBM_channelB3[offset_n + t].data[12];
			dram_bufferB3[t].data[13] = HBM_channelB3[offset_n + t].data[13];
			dram_bufferB3[t].data[14] = HBM_channelB3[offset_n + t].data[14];
			dram_bufferB3[t].data[15] = HBM_channelB3[offset_n + t].data[15];
	
			dram_bufferB4[t].data[0] = HBM_channelB4[offset_n + t].data[0];
			dram_bufferB4[t].data[1] = HBM_channelB4[offset_n + t].data[1];
			dram_bufferB4[t].data[2] = HBM_channelB4[offset_n + t].data[2];
			dram_bufferB4[t].data[3] = HBM_channelB4[offset_n + t].data[3];
			dram_bufferB4[t].data[4] = HBM_channelB4[offset_n + t].data[4];
			dram_bufferB4[t].data[5] = HBM_channelB4[offset_n + t].data[5];
			dram_bufferB4[t].data[6] = HBM_channelB4[offset_n + t].data[6];
			dram_bufferB4[t].data[7] = HBM_channelB4[offset_n + t].data[7];
			dram_bufferB4[t].data[8] = HBM_channelB4[offset_n + t].data[8];
			dram_bufferB4[t].data[9] = HBM_channelB4[offset_n + t].data[9];
			dram_bufferB4[t].data[10] = HBM_channelB4[offset_n + t].data[10];
			dram_bufferB4[t].data[11] = HBM_channelB4[offset_n + t].data[11];
			dram_bufferB4[t].data[12] = HBM_channelB4[offset_n + t].data[12];
			dram_bufferB4[t].data[13] = HBM_channelB4[offset_n + t].data[13];
			dram_bufferB4[t].data[14] = HBM_channelB4[offset_n + t].data[14];
			dram_bufferB4[t].data[15] = HBM_channelB4[offset_n + t].data[15];
	
			dram_bufferB5[t].data[0] = HBM_channelB5[offset_n + t].data[0];
			dram_bufferB5[t].data[1] = HBM_channelB5[offset_n + t].data[1];
			dram_bufferB5[t].data[2] = HBM_channelB5[offset_n + t].data[2];
			dram_bufferB5[t].data[3] = HBM_channelB5[offset_n + t].data[3];
			dram_bufferB5[t].data[4] = HBM_channelB5[offset_n + t].data[4];
			dram_bufferB5[t].data[5] = HBM_channelB5[offset_n + t].data[5];
			dram_bufferB5[t].data[6] = HBM_channelB5[offset_n + t].data[6];
			dram_bufferB5[t].data[7] = HBM_channelB5[offset_n + t].data[7];
			dram_bufferB5[t].data[8] = HBM_channelB5[offset_n + t].data[8];
			dram_bufferB5[t].data[9] = HBM_channelB5[offset_n + t].data[9];
			dram_bufferB5[t].data[10] = HBM_channelB5[offset_n + t].data[10];
			dram_bufferB5[t].data[11] = HBM_channelB5[offset_n + t].data[11];
			dram_bufferB5[t].data[12] = HBM_channelB5[offset_n + t].data[12];
			dram_bufferB5[t].data[13] = HBM_channelB5[offset_n + t].data[13];
			dram_bufferB5[t].data[14] = HBM_channelB5[offset_n + t].data[14];
			dram_bufferB5[t].data[15] = HBM_channelB5[offset_n + t].data[15];
	
			
			dram_centerbufferA[t].data[0] = HBM_centerA[offset_n + t].data[0];
			dram_centerbufferA[t].data[1] = HBM_centerA[offset_n + t].data[1];
			dram_centerbufferA[t].data[2] = HBM_centerA[offset_n + t].data[2];
			dram_centerbufferA[t].data[3] = HBM_centerA[offset_n + t].data[3];
			dram_centerbufferA[t].data[4] = HBM_centerA[offset_n + t].data[4];
			dram_centerbufferA[t].data[5] = HBM_centerA[offset_n + t].data[5];
			dram_centerbufferA[t].data[6] = HBM_centerA[offset_n + t].data[6];
			dram_centerbufferA[t].data[7] = HBM_centerA[offset_n + t].data[7];
			dram_centerbufferA[t].data[8] = HBM_centerA[offset_n + t].data[8];
			dram_centerbufferA[t].data[9] = HBM_centerA[offset_n + t].data[9];
			dram_centerbufferA[t].data[10] = HBM_centerA[offset_n + t].data[10];
			dram_centerbufferA[t].data[11] = HBM_centerA[offset_n + t].data[11];
			dram_centerbufferA[t].data[12] = HBM_centerA[offset_n + t].data[12];
			dram_centerbufferA[t].data[13] = HBM_centerA[offset_n + t].data[13];
			dram_centerbufferA[t].data[14] = HBM_centerA[offset_n + t].data[14];
			dram_centerbufferA[t].data[15] = HBM_centerA[offset_n + t].data[15];
			dram_centerbufferB[t].data[0] = HBM_centerB[offset_n + t].data[0];
			dram_centerbufferB[t].data[1] = HBM_centerB[offset_n + t].data[1];
			dram_centerbufferB[t].data[2] = HBM_centerB[offset_n + t].data[2];
			dram_centerbufferB[t].data[3] = HBM_centerB[offset_n + t].data[3];
			dram_centerbufferB[t].data[4] = HBM_centerB[offset_n + t].data[4];
			dram_centerbufferB[t].data[5] = HBM_centerB[offset_n + t].data[5];
			dram_centerbufferB[t].data[6] = HBM_centerB[offset_n + t].data[6];
			dram_centerbufferB[t].data[7] = HBM_centerB[offset_n + t].data[7];
			dram_centerbufferB[t].data[8] = HBM_centerB[offset_n + t].data[8];
			dram_centerbufferB[t].data[9] = HBM_centerB[offset_n + t].data[9];
			dram_centerbufferB[t].data[10] = HBM_centerB[offset_n + t].data[10];
			dram_centerbufferB[t].data[11] = HBM_centerB[offset_n + t].data[11];
			dram_centerbufferB[t].data[12] = HBM_centerB[offset_n + t].data[12];
			dram_centerbufferB[t].data[13] = HBM_centerB[offset_n + t].data[13];
			dram_centerbufferB[t].data[14] = HBM_centerB[offset_n + t].data[14];
			dram_centerbufferB[t].data[15] = HBM_centerB[offset_n + t].data[15];
		}
		
		// next_function( dram_bufferA0, dram_bufferB0, dram_bufferA1, dram_bufferB1, dram_bufferA2, dram_bufferB2, dram_bufferA3, dram_bufferB3, dram_bufferA4, dram_bufferB4, dram_bufferA5, dram_bufferB5, dram_centerbufferA, dram_centerbufferB, batch);
			
		BASECOMMIT_LOOP1: for(unsigned int t=0; t<TEST_VECSIZE; t++){
		#pragma HLS PIPELINE II=1
			HBM_channelA0[offset_n + t].data[0] = dram_bufferA0[t].data[0];
			HBM_channelA0[offset_n + t].data[1] = dram_bufferA0[t].data[1];
			HBM_channelA0[offset_n + t].data[2] = dram_bufferA0[t].data[2];
			HBM_channelA0[offset_n + t].data[3] = dram_bufferA0[t].data[3];
			HBM_channelA0[offset_n + t].data[4] = dram_bufferA0[t].data[4];
			HBM_channelA0[offset_n + t].data[5] = dram_bufferA0[t].data[5];
			HBM_channelA0[offset_n + t].data[6] = dram_bufferA0[t].data[6];
			HBM_channelA0[offset_n + t].data[7] = dram_bufferA0[t].data[7];
			HBM_channelA0[offset_n + t].data[8] = dram_bufferA0[t].data[8];
			HBM_channelA0[offset_n + t].data[9] = dram_bufferA0[t].data[9];
			HBM_channelA0[offset_n + t].data[10] = dram_bufferA0[t].data[10];
			HBM_channelA0[offset_n + t].data[11] = dram_bufferA0[t].data[11];
			HBM_channelA0[offset_n + t].data[12] = dram_bufferA0[t].data[12];
			HBM_channelA0[offset_n + t].data[13] = dram_bufferA0[t].data[13];
			HBM_channelA0[offset_n + t].data[14] = dram_bufferA0[t].data[14];
			HBM_channelA0[offset_n + t].data[15] = dram_bufferA0[t].data[15];
			HBM_channelA1[offset_n + t].data[0] = dram_bufferA1[t].data[0];
			HBM_channelA1[offset_n + t].data[1] = dram_bufferA1[t].data[1];
			HBM_channelA1[offset_n + t].data[2] = dram_bufferA1[t].data[2];
			HBM_channelA1[offset_n + t].data[3] = dram_bufferA1[t].data[3];
			HBM_channelA1[offset_n + t].data[4] = dram_bufferA1[t].data[4];
			HBM_channelA1[offset_n + t].data[5] = dram_bufferA1[t].data[5];
			HBM_channelA1[offset_n + t].data[6] = dram_bufferA1[t].data[6];
			HBM_channelA1[offset_n + t].data[7] = dram_bufferA1[t].data[7];
			HBM_channelA1[offset_n + t].data[8] = dram_bufferA1[t].data[8];
			HBM_channelA1[offset_n + t].data[9] = dram_bufferA1[t].data[9];
			HBM_channelA1[offset_n + t].data[10] = dram_bufferA1[t].data[10];
			HBM_channelA1[offset_n + t].data[11] = dram_bufferA1[t].data[11];
			HBM_channelA1[offset_n + t].data[12] = dram_bufferA1[t].data[12];
			HBM_channelA1[offset_n + t].data[13] = dram_bufferA1[t].data[13];
			HBM_channelA1[offset_n + t].data[14] = dram_bufferA1[t].data[14];
			HBM_channelA1[offset_n + t].data[15] = dram_bufferA1[t].data[15];
			HBM_channelA2[offset_n + t].data[0] = dram_bufferA2[t].data[0];
			HBM_channelA2[offset_n + t].data[1] = dram_bufferA2[t].data[1];
			HBM_channelA2[offset_n + t].data[2] = dram_bufferA2[t].data[2];
			HBM_channelA2[offset_n + t].data[3] = dram_bufferA2[t].data[3];
			HBM_channelA2[offset_n + t].data[4] = dram_bufferA2[t].data[4];
			HBM_channelA2[offset_n + t].data[5] = dram_bufferA2[t].data[5];
			HBM_channelA2[offset_n + t].data[6] = dram_bufferA2[t].data[6];
			HBM_channelA2[offset_n + t].data[7] = dram_bufferA2[t].data[7];
			HBM_channelA2[offset_n + t].data[8] = dram_bufferA2[t].data[8];
			HBM_channelA2[offset_n + t].data[9] = dram_bufferA2[t].data[9];
			HBM_channelA2[offset_n + t].data[10] = dram_bufferA2[t].data[10];
			HBM_channelA2[offset_n + t].data[11] = dram_bufferA2[t].data[11];
			HBM_channelA2[offset_n + t].data[12] = dram_bufferA2[t].data[12];
			HBM_channelA2[offset_n + t].data[13] = dram_bufferA2[t].data[13];
			HBM_channelA2[offset_n + t].data[14] = dram_bufferA2[t].data[14];
			HBM_channelA2[offset_n + t].data[15] = dram_bufferA2[t].data[15];
			HBM_channelA3[offset_n + t].data[0] = dram_bufferA3[t].data[0];
			HBM_channelA3[offset_n + t].data[1] = dram_bufferA3[t].data[1];
			HBM_channelA3[offset_n + t].data[2] = dram_bufferA3[t].data[2];
			HBM_channelA3[offset_n + t].data[3] = dram_bufferA3[t].data[3];
			HBM_channelA3[offset_n + t].data[4] = dram_bufferA3[t].data[4];
			HBM_channelA3[offset_n + t].data[5] = dram_bufferA3[t].data[5];
			HBM_channelA3[offset_n + t].data[6] = dram_bufferA3[t].data[6];
			HBM_channelA3[offset_n + t].data[7] = dram_bufferA3[t].data[7];
			HBM_channelA3[offset_n + t].data[8] = dram_bufferA3[t].data[8];
			HBM_channelA3[offset_n + t].data[9] = dram_bufferA3[t].data[9];
			HBM_channelA3[offset_n + t].data[10] = dram_bufferA3[t].data[10];
			HBM_channelA3[offset_n + t].data[11] = dram_bufferA3[t].data[11];
			HBM_channelA3[offset_n + t].data[12] = dram_bufferA3[t].data[12];
			HBM_channelA3[offset_n + t].data[13] = dram_bufferA3[t].data[13];
			HBM_channelA3[offset_n + t].data[14] = dram_bufferA3[t].data[14];
			HBM_channelA3[offset_n + t].data[15] = dram_bufferA3[t].data[15];
			HBM_channelA4[offset_n + t].data[0] = dram_bufferA4[t].data[0];
			HBM_channelA4[offset_n + t].data[1] = dram_bufferA4[t].data[1];
			HBM_channelA4[offset_n + t].data[2] = dram_bufferA4[t].data[2];
			HBM_channelA4[offset_n + t].data[3] = dram_bufferA4[t].data[3];
			HBM_channelA4[offset_n + t].data[4] = dram_bufferA4[t].data[4];
			HBM_channelA4[offset_n + t].data[5] = dram_bufferA4[t].data[5];
			HBM_channelA4[offset_n + t].data[6] = dram_bufferA4[t].data[6];
			HBM_channelA4[offset_n + t].data[7] = dram_bufferA4[t].data[7];
			HBM_channelA4[offset_n + t].data[8] = dram_bufferA4[t].data[8];
			HBM_channelA4[offset_n + t].data[9] = dram_bufferA4[t].data[9];
			HBM_channelA4[offset_n + t].data[10] = dram_bufferA4[t].data[10];
			HBM_channelA4[offset_n + t].data[11] = dram_bufferA4[t].data[11];
			HBM_channelA4[offset_n + t].data[12] = dram_bufferA4[t].data[12];
			HBM_channelA4[offset_n + t].data[13] = dram_bufferA4[t].data[13];
			HBM_channelA4[offset_n + t].data[14] = dram_bufferA4[t].data[14];
			HBM_channelA4[offset_n + t].data[15] = dram_bufferA4[t].data[15];
			HBM_channelA5[offset_n + t].data[0] = dram_bufferA5[t].data[0];
			HBM_channelA5[offset_n + t].data[1] = dram_bufferA5[t].data[1];
			HBM_channelA5[offset_n + t].data[2] = dram_bufferA5[t].data[2];
			HBM_channelA5[offset_n + t].data[3] = dram_bufferA5[t].data[3];
			HBM_channelA5[offset_n + t].data[4] = dram_bufferA5[t].data[4];
			HBM_channelA5[offset_n + t].data[5] = dram_bufferA5[t].data[5];
			HBM_channelA5[offset_n + t].data[6] = dram_bufferA5[t].data[6];
			HBM_channelA5[offset_n + t].data[7] = dram_bufferA5[t].data[7];
			HBM_channelA5[offset_n + t].data[8] = dram_bufferA5[t].data[8];
			HBM_channelA5[offset_n + t].data[9] = dram_bufferA5[t].data[9];
			HBM_channelA5[offset_n + t].data[10] = dram_bufferA5[t].data[10];
			HBM_channelA5[offset_n + t].data[11] = dram_bufferA5[t].data[11];
			HBM_channelA5[offset_n + t].data[12] = dram_bufferA5[t].data[12];
			HBM_channelA5[offset_n + t].data[13] = dram_bufferA5[t].data[13];
			HBM_channelA5[offset_n + t].data[14] = dram_bufferA5[t].data[14];
			HBM_channelA5[offset_n + t].data[15] = dram_bufferA5[t].data[15];
			HBM_channelB0[offset_n + t].data[0] = dram_bufferB0[t].data[0];
			HBM_channelB0[offset_n + t].data[1] = dram_bufferB0[t].data[1];
			HBM_channelB0[offset_n + t].data[2] = dram_bufferB0[t].data[2];
			HBM_channelB0[offset_n + t].data[3] = dram_bufferB0[t].data[3];
			HBM_channelB0[offset_n + t].data[4] = dram_bufferB0[t].data[4];
			HBM_channelB0[offset_n + t].data[5] = dram_bufferB0[t].data[5];
			HBM_channelB0[offset_n + t].data[6] = dram_bufferB0[t].data[6];
			HBM_channelB0[offset_n + t].data[7] = dram_bufferB0[t].data[7];
			HBM_channelB0[offset_n + t].data[8] = dram_bufferB0[t].data[8];
			HBM_channelB0[offset_n + t].data[9] = dram_bufferB0[t].data[9];
			HBM_channelB0[offset_n + t].data[10] = dram_bufferB0[t].data[10];
			HBM_channelB0[offset_n + t].data[11] = dram_bufferB0[t].data[11];
			HBM_channelB0[offset_n + t].data[12] = dram_bufferB0[t].data[12];
			HBM_channelB0[offset_n + t].data[13] = dram_bufferB0[t].data[13];
			HBM_channelB0[offset_n + t].data[14] = dram_bufferB0[t].data[14];
			HBM_channelB0[offset_n + t].data[15] = dram_bufferB0[t].data[15];
	
			HBM_channelB1[offset_n + t].data[0] = dram_bufferB1[t].data[0];
			HBM_channelB1[offset_n + t].data[1] = dram_bufferB1[t].data[1];
			HBM_channelB1[offset_n + t].data[2] = dram_bufferB1[t].data[2];
			HBM_channelB1[offset_n + t].data[3] = dram_bufferB1[t].data[3];
			HBM_channelB1[offset_n + t].data[4] = dram_bufferB1[t].data[4];
			HBM_channelB1[offset_n + t].data[5] = dram_bufferB1[t].data[5];
			HBM_channelB1[offset_n + t].data[6] = dram_bufferB1[t].data[6];
			HBM_channelB1[offset_n + t].data[7] = dram_bufferB1[t].data[7];
			HBM_channelB1[offset_n + t].data[8] = dram_bufferB1[t].data[8];
			HBM_channelB1[offset_n + t].data[9] = dram_bufferB1[t].data[9];
			HBM_channelB1[offset_n + t].data[10] = dram_bufferB1[t].data[10];
			HBM_channelB1[offset_n + t].data[11] = dram_bufferB1[t].data[11];
			HBM_channelB1[offset_n + t].data[12] = dram_bufferB1[t].data[12];
			HBM_channelB1[offset_n + t].data[13] = dram_bufferB1[t].data[13];
			HBM_channelB1[offset_n + t].data[14] = dram_bufferB1[t].data[14];
			HBM_channelB1[offset_n + t].data[15] = dram_bufferB1[t].data[15];
	
			HBM_channelB2[offset_n + t].data[0] = dram_bufferB2[t].data[0];
			HBM_channelB2[offset_n + t].data[1] = dram_bufferB2[t].data[1];
			HBM_channelB2[offset_n + t].data[2] = dram_bufferB2[t].data[2];
			HBM_channelB2[offset_n + t].data[3] = dram_bufferB2[t].data[3];
			HBM_channelB2[offset_n + t].data[4] = dram_bufferB2[t].data[4];
			HBM_channelB2[offset_n + t].data[5] = dram_bufferB2[t].data[5];
			HBM_channelB2[offset_n + t].data[6] = dram_bufferB2[t].data[6];
			HBM_channelB2[offset_n + t].data[7] = dram_bufferB2[t].data[7];
			HBM_channelB2[offset_n + t].data[8] = dram_bufferB2[t].data[8];
			HBM_channelB2[offset_n + t].data[9] = dram_bufferB2[t].data[9];
			HBM_channelB2[offset_n + t].data[10] = dram_bufferB2[t].data[10];
			HBM_channelB2[offset_n + t].data[11] = dram_bufferB2[t].data[11];
			HBM_channelB2[offset_n + t].data[12] = dram_bufferB2[t].data[12];
			HBM_channelB2[offset_n + t].data[13] = dram_bufferB2[t].data[13];
			HBM_channelB2[offset_n + t].data[14] = dram_bufferB2[t].data[14];
			HBM_channelB2[offset_n + t].data[15] = dram_bufferB2[t].data[15];
	
			HBM_channelB3[offset_n + t].data[0] = dram_bufferB3[t].data[0];
			HBM_channelB3[offset_n + t].data[1] = dram_bufferB3[t].data[1];
			HBM_channelB3[offset_n + t].data[2] = dram_bufferB3[t].data[2];
			HBM_channelB3[offset_n + t].data[3] = dram_bufferB3[t].data[3];
			HBM_channelB3[offset_n + t].data[4] = dram_bufferB3[t].data[4];
			HBM_channelB3[offset_n + t].data[5] = dram_bufferB3[t].data[5];
			HBM_channelB3[offset_n + t].data[6] = dram_bufferB3[t].data[6];
			HBM_channelB3[offset_n + t].data[7] = dram_bufferB3[t].data[7];
			HBM_channelB3[offset_n + t].data[8] = dram_bufferB3[t].data[8];
			HBM_channelB3[offset_n + t].data[9] = dram_bufferB3[t].data[9];
			HBM_channelB3[offset_n + t].data[10] = dram_bufferB3[t].data[10];
			HBM_channelB3[offset_n + t].data[11] = dram_bufferB3[t].data[11];
			HBM_channelB3[offset_n + t].data[12] = dram_bufferB3[t].data[12];
			HBM_channelB3[offset_n + t].data[13] = dram_bufferB3[t].data[13];
			HBM_channelB3[offset_n + t].data[14] = dram_bufferB3[t].data[14];
			HBM_channelB3[offset_n + t].data[15] = dram_bufferB3[t].data[15];
	
			HBM_channelB4[offset_n + t].data[0] = dram_bufferB4[t].data[0];
			HBM_channelB4[offset_n + t].data[1] = dram_bufferB4[t].data[1];
			HBM_channelB4[offset_n + t].data[2] = dram_bufferB4[t].data[2];
			HBM_channelB4[offset_n + t].data[3] = dram_bufferB4[t].data[3];
			HBM_channelB4[offset_n + t].data[4] = dram_bufferB4[t].data[4];
			HBM_channelB4[offset_n + t].data[5] = dram_bufferB4[t].data[5];
			HBM_channelB4[offset_n + t].data[6] = dram_bufferB4[t].data[6];
			HBM_channelB4[offset_n + t].data[7] = dram_bufferB4[t].data[7];
			HBM_channelB4[offset_n + t].data[8] = dram_bufferB4[t].data[8];
			HBM_channelB4[offset_n + t].data[9] = dram_bufferB4[t].data[9];
			HBM_channelB4[offset_n + t].data[10] = dram_bufferB4[t].data[10];
			HBM_channelB4[offset_n + t].data[11] = dram_bufferB4[t].data[11];
			HBM_channelB4[offset_n + t].data[12] = dram_bufferB4[t].data[12];
			HBM_channelB4[offset_n + t].data[13] = dram_bufferB4[t].data[13];
			HBM_channelB4[offset_n + t].data[14] = dram_bufferB4[t].data[14];
			HBM_channelB4[offset_n + t].data[15] = dram_bufferB4[t].data[15];
	
			HBM_channelB5[offset_n + t].data[0] = dram_bufferB5[t].data[0];
			HBM_channelB5[offset_n + t].data[1] = dram_bufferB5[t].data[1];
			HBM_channelB5[offset_n + t].data[2] = dram_bufferB5[t].data[2];
			HBM_channelB5[offset_n + t].data[3] = dram_bufferB5[t].data[3];
			HBM_channelB5[offset_n + t].data[4] = dram_bufferB5[t].data[4];
			HBM_channelB5[offset_n + t].data[5] = dram_bufferB5[t].data[5];
			HBM_channelB5[offset_n + t].data[6] = dram_bufferB5[t].data[6];
			HBM_channelB5[offset_n + t].data[7] = dram_bufferB5[t].data[7];
			HBM_channelB5[offset_n + t].data[8] = dram_bufferB5[t].data[8];
			HBM_channelB5[offset_n + t].data[9] = dram_bufferB5[t].data[9];
			HBM_channelB5[offset_n + t].data[10] = dram_bufferB5[t].data[10];
			HBM_channelB5[offset_n + t].data[11] = dram_bufferB5[t].data[11];
			HBM_channelB5[offset_n + t].data[12] = dram_bufferB5[t].data[12];
			HBM_channelB5[offset_n + t].data[13] = dram_bufferB5[t].data[13];
			HBM_channelB5[offset_n + t].data[14] = dram_bufferB5[t].data[14];
			HBM_channelB5[offset_n + t].data[15] = dram_bufferB5[t].data[15];
	
			
			HBM_centerA[offset_n + t].data[0] = dram_centerbufferA[t].data[0];
			HBM_centerA[offset_n + t].data[1] = dram_centerbufferA[t].data[1];
			HBM_centerA[offset_n + t].data[2] = dram_centerbufferA[t].data[2];
			HBM_centerA[offset_n + t].data[3] = dram_centerbufferA[t].data[3];
			HBM_centerA[offset_n + t].data[4] = dram_centerbufferA[t].data[4];
			HBM_centerA[offset_n + t].data[5] = dram_centerbufferA[t].data[5];
			HBM_centerA[offset_n + t].data[6] = dram_centerbufferA[t].data[6];
			HBM_centerA[offset_n + t].data[7] = dram_centerbufferA[t].data[7];
			HBM_centerA[offset_n + t].data[8] = dram_centerbufferA[t].data[8];
			HBM_centerA[offset_n + t].data[9] = dram_centerbufferA[t].data[9];
			HBM_centerA[offset_n + t].data[10] = dram_centerbufferA[t].data[10];
			HBM_centerA[offset_n + t].data[11] = dram_centerbufferA[t].data[11];
			HBM_centerA[offset_n + t].data[12] = dram_centerbufferA[t].data[12];
			HBM_centerA[offset_n + t].data[13] = dram_centerbufferA[t].data[13];
			HBM_centerA[offset_n + t].data[14] = dram_centerbufferA[t].data[14];
			HBM_centerA[offset_n + t].data[15] = dram_centerbufferA[t].data[15];
			HBM_centerB[offset_n + t].data[0] = dram_centerbufferB[t].data[0];
			HBM_centerB[offset_n + t].data[1] = dram_centerbufferB[t].data[1];
			HBM_centerB[offset_n + t].data[2] = dram_centerbufferB[t].data[2];
			HBM_centerB[offset_n + t].data[3] = dram_centerbufferB[t].data[3];
			HBM_centerB[offset_n + t].data[4] = dram_centerbufferB[t].data[4];
			HBM_centerB[offset_n + t].data[5] = dram_centerbufferB[t].data[5];
			HBM_centerB[offset_n + t].data[6] = dram_centerbufferB[t].data[6];
			HBM_centerB[offset_n + t].data[7] = dram_centerbufferB[t].data[7];
			HBM_centerB[offset_n + t].data[8] = dram_centerbufferB[t].data[8];
			HBM_centerB[offset_n + t].data[9] = dram_centerbufferB[t].data[9];
			HBM_centerB[offset_n + t].data[10] = dram_centerbufferB[t].data[10];
			HBM_centerB[offset_n + t].data[11] = dram_centerbufferB[t].data[11];
			HBM_centerB[offset_n + t].data[12] = dram_centerbufferB[t].data[12];
			HBM_centerB[offset_n + t].data[13] = dram_centerbufferB[t].data[13];
			HBM_centerB[offset_n + t].data[14] = dram_centerbufferB[t].data[14];
			HBM_centerB[offset_n + t].data[15] = dram_centerbufferB[t].data[15];
		}
	}
	return;
}
}
#endif 

		
#endif 	