#ifndef COMMON_H
#define COMMON_H
#include <string.h> 
#include <cmath>
#include <ap_int.h>
// #include "ap_fixed.h"	
#include <vector> 
#include<hls_vector.h> 
#include<hls_stream.h> 
#include <iostream>
 

// #define ___ENABLE___DYNAMICGRAPHANALYTICS___

#define ___FORCE_SUCCESS___
#define POW_VALID_VDATA 1
#define ___FORCE_SUCCESS_SINGLE_CHANNEL___
// #define ADJUSTMENT_TO_MEET_VHLS_TIMING
#define ___PARTIAL___HBM___ACCESSES___

#define DRAM_ACCESS_LATENCY 8
#define _NUMNANOSECONDS_PER_CLOCKCYCLE_ (6956 / 1000) // 6.956 // {23M uint32 seen processed in 10ms}{10M uint32 seen processed in 4ms}
#define ACTS_AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC ((23 * 1000) / 10) // ((23 / 10) * 1000) // {23M uint32 seen processed in 10ms}{10M uint32 seen processed in 4ms}

#define ___USE_AXI_CHANNEL___

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define HW // SWEMU, HW, *SW
#if (defined(SWEMU) || defined(HW))
#define FPGA_IMPL
#endif 

// #define RANKING_ALGORITHM_TYPE
// #define TRAVERSAL_ALGORITHM_TYPE
// #define ALL_ALGORITHM_TYPES

#if defined(BFS_ALGORITHM) || defined(SSSP_ALGORITHM)
#define CONFIG_ALGORITHMTYPE_RANDOMACTIVEVERTICES // { utility.cpp }
#endif

#define _DEBUGMODE_HEADER //
#if defined (FPGA_IMPL) // && defined (HW) // REMOVEME. 
#else
#define _DEBUGMODE_STATS // 
// #define _DEBUGMODE_CHECKS
#define _DEBUGMODE_CHECKS2 // 
#define _DEBUGMODE_CHECKS3 // 
// #define _DEBUGMODE_PRINTS
// #define _DEBUGMODE_KERNELPRINTS
// #define _DEBUGMODE_KERNELPRINTS2 //
// #define _DEBUGMODE_KERNELPRINTS3 //
#define _DEBUGMODE_KERNELPRINTS4 //
// #define _DEBUGMODE_RUNKERNELPRINTS // 
// #define _DEBUGMODE_PROCACTVVSPRINTS //
#endif
// #define _DEBUGMODE_HOSTCHECKS
#define _DEBUGMODE_HOSTCHECKS2 //
#define _DEBUGMODE_HOSTCHECKS3 //
// #define _DEBUGMODE_HOSTPRINTS 
// #define _DEBUGMODE_HOSTPRINTS2 //
// #define _DEBUGMODE_HOSTPRINTS3 //
#define _DEBUGMODE_HOSTPRINTS4 //
// #define _DEBUGMODE_TIMERS
#define _DEBUGMODE_TIMERS2
#define _DEBUGMODE_TIMERS3
#define GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
#define HOST_PRINT_RESULTS

#ifndef HW
// #define _DEBUGMODE_KERNELPRINTS_TRACE3 //
#endif 

////////////////

#define PAGERANK 222
#define CF 333
#define HITS 444
#define SPMV 555
#define BFS 666
#define SSSP 777
#define CC 888
#define ALGORITHMCLASS_ALLVERTEXISACTIVE 2222 
#define ALGORITHMCLASS_NOTALLVERTEXISACTIVE 3333

//////////////// 

#define NUM_PROCS 2
#define NUM_PEs 12
#define NUM_VALID_PEs 6
#define NUM_AXI_CHANNELS_IN_PE 4
#define EDGE_PACK_SIZE_POW 4 // 1 4*
#define EDGE_PACK_SIZE (1 << EDGE_PACK_SIZE_POW) // 2, 16*
#define HBM_CHANNEL_PACK_SIZE (EDGE_PACK_SIZE * 2) // 32*
#define HBM_AXI_PACK_SIZE (HBM_CHANNEL_PACK_SIZE / 2) // 16* // NEW**
#define HBM_AXI_PACK_BITSIZE (HBM_AXI_PACK_SIZE * 32) // 512* // NEW**
#define HBM_CHANNEL_BYTESIZE (1 << 28)
#define HBM_CHANNEL_INTSIZE (HBM_CHANNEL_BYTESIZE / 4)
#define HBM_CHANNEL_SIZE ((HBM_CHANNEL_BYTESIZE / 4) / EDGE_PACK_SIZE) // {4194304 EDGE_PACK_SIZEs, 67108864 uints, 256MB}
#define FOLD_SIZE 1
#define MAX_NUM_UPARTITIONS 512
#define MAX_NUM_APPLYPARTITIONS 32 // 16(original), 32
#define MAX_NUM_LLPSETS 64 // 16(original), 64*
#define MAX_NUM_LLPS (64 * EDGE_PACK_SIZE)
#define NAp 666
#define MAXNUMGRAPHITERATIONS 16
#define MAX_NUM_PARTITIONS 16 
#define INVALIDDATA 0xFFFFFFFF 
// #define INVALIDDATA_MINI 0xFFFF
#define INVALIDMASK 0 

#define MAXNUMBITS2_ACTPACK_SRCVID 14
#define MAXNUMBITS2_ACTPACK_DESTVID 14
#define MAXNUMBITS2_ACTPACK_EDGEID 4
#define MAXLOCALVALUE2_ACTPACK_SRCVID 0x3FFF // (2^14-1=16383)
#define MAXLOCALVALUE2_ACTPACK_DESTVID 0x3FFF
#define MAXLOCALVALUE2_ACTPACK_EDGEID 0xF
#define INVALIDDATA_SRCVID MAXLOCALVALUE2_ACTPACK_SRCVID
#define INVALIDDATA_DESTVID MAXLOCALVALUE2_ACTPACK_DESTVID

/* #define MAXNUMBITS_ACTPACK_SRCVID 14	
#define MAXNUMBITS_ACTPACK_DESTVID 18
// #define MAXLOCALVALUE_ACTPACK_SRCVID (0x4000-1)
// #define MAXLOCALVALUE_ACTPACK_DESTVID (0x40000-1)
#define MAXLOCALVALUE_ACTPACK_SRCVID 0x3FFF
#define MAXLOCALVALUE_ACTPACK_DESTVID 0x3FFFF //{note that this is larger}
#define INVALIDDATA_SRCVID MAXLOCALVALUE_ACTPACK_SRCVID
#define INVALIDDATA_DESTVID MAXLOCALVALUE_ACTPACK_DESTVID */

#define MULT_FACTOR (16 / EDGE_PACK_SIZE) // 1
#define MAX_UPARTITION_VECSIZE (8184 * MULT_FACTOR) // NEW**
#define MAX_UPARTITION_SIZE (EDGE_PACK_SIZE * MAX_UPARTITION_VECSIZE) // 131072 
#define MAX_APPLYPARTITION_VECSIZE MAX_UPARTITION_VECSIZE
#define MAX_APPLYPARTITION_SIZE (EDGE_PACK_SIZE * MAX_APPLYPARTITION_VECSIZE) // 131072 
#define MAX_ACTVV_VECSIZE (MAX_UPARTITION_VECSIZE / NUM_PEs) // 341, 682*
#define MAX_ACTVV_SIZE (EDGE_PACK_SIZE * MAX_ACTVV_VECSIZE) // 5456
#define NUM_ACTVVPARTITIONS_PER_APPLYPARTITION NUM_PEs // 12
#define MAX_NUM_ACTVVPARTITIONS (MAX_NUM_APPLYPARTITIONS * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION * NUM_PEs) // 384

#define UPDATES_BUFFER_PACK_SIZE MAX(EDGE_PACK_SIZE, NUM_PEs)

#define MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION (1024 * MULT_FACTOR)
#define ACTVUPDATESBLOCK_VECSIZE_POW 4
#define ACTVUPDATESBLOCK_VECSIZE ((1 << ACTVUPDATESBLOCK_VECSIZE_POW) * MULT_FACTOR) // 16
#define BLOCKRAM_SIZE (512 * MULT_FACTOR)
#define DOUBLE_BLOCKRAM_SIZE ((BLOCKRAM_SIZE * 2) * MULT_FACTOR)	
#ifdef ___FORCE_SUCCESS_SINGLE_CHANNEL___
#define VPTR_BUFFER_SIZE (512 * 8) // 16
#define EDGE_BUFFER_SIZE (512 * 8) // 16
#define UPDATES_BUFFER_SIZE (512 * 8) // 16
// #define EDGE_UPDATES_BUFFER_SIZE (512 * 8)
#else 
#define VPTR_BUFFER_SIZE (512 * MULT_FACTOR)
#define EDGE_BUFFER_SIZE (512 * MULT_FACTOR)   //8192// 512 // FIXME.
#define UPDATES_BUFFER_SIZE (512 * MULT_FACTOR)	
// #define EDGE_UPDATES_BUFFER_SIZE (512 * MULT_FACTOR)
#endif 	
#define EDGE_UPDATES_BUFFER_SIZE 8192

#define GLOBALBUFFER_SIZE 64
#define GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS 0
#define GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS 1
#define GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS 2
#define GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2 3
#define GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS 4
#define GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES 5
#define GLOBALPARAMSCODE__BASEOFFSET__CSREDGES 6
#define GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES 7
#define GLOBALPARAMSCODE__BASEOFFSET__UPDATES 8
#define GLOBALPARAMSCODE__BASEOFFSET__VDATAS 9
#define GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP 10
#define GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS 11

#define GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS 15
#define GLOBALPARAMSCODE__WWSIZE__CSRVPTRS 16
#define GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS 17
#define GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2 18
#define GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS 19
#define GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES 20
#define GLOBALPARAMSCODE__WWSIZE__CSREDGES 21
#define GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES 22
#define GLOBALPARAMSCODE__WWSIZE__UPDATES 23
#define GLOBALPARAMSCODE__WWSIZE__VDATAS 24
#define GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP 25
#define GLOBALPARAMSCODE__WWSIZE__NFRONTIERS 26

#define GLOBALPARAMSCODE__PARAM__NUM_VERTICES 30
#define GLOBALPARAMSCODE__PARAM__NUM_EDGES 31
#define GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS 32
#define GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS 33
#define GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS 34
#define GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEFRONTIERSFORCONTROLSWITCH 35
#define GLOBALPARAMSCODE__PARAM__MAXDEGREE 36
#define GLOBALPARAMSCODE__PARAM__ALGORITHM 37
#define GLOBALPARAMSCODE__PARAM__ROOTVID 38
#define GLOBALPARAMSCODE__PARAM__RANGEPERCHANNEL 39
#define GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEDSTVID 40
#define GLOBALPARAMSCODE__PARAM__NUM_RUNS 41

#define GLOBALPARAMSCODE___ENABLE___RESETBUFFERSATSTART 45
#define GLOBALPARAMSCODE___ENABLE___PROCESSEDGES 46
#define GLOBALPARAMSCODE___ENABLE___READ_FRONTIER_PROPERTIES 47
#define GLOBALPARAMSCODE___ENABLE___VCPROCESSEDGES 48
#define GLOBALPARAMSCODE___ENABLE___ECPROCESSEDGES 49
#define GLOBALPARAMSCODE___ENABLE___SAVEVCUPDATES 50
#define GLOBALPARAMSCODE___ENABLE___COLLECTACTIVEDSTVIDS 51
#define GLOBALPARAMSCODE___ENABLE___APPLYUPDATESMODULE 52
#define GLOBALPARAMSCODE___ENABLE___READ_DEST_PROPERTIES 53
#define GLOBALPARAMSCODE___ENABLE___APPLYUPDATES 54
#define GLOBALPARAMSCODE___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES 55
#define GLOBALPARAMSCODE___ENABLE___SAVE_DEST_PROPERTIES 56
#define GLOBALPARAMSCODE___ENABLE___EXCHANGEFRONTIERINFOS 57

#define GLOBALPARAMSCODE__ASYNC__BATCH 60
#define GLOBALPARAMSCODE__ASYNC__BATCHSIZE 61

////////////////

#define HBM_CHANNEL_READTHROUGHPUT 16 // KVDRAM -> BUFFER: how many uint512_dt wide-words can be read / sec?
#define HBM_CHANNEL_WRITETHROUGHPUT 16 // BUFFER -> KVDRAM: how many uint512_dt wide-words can be written / sec?
#define HBM_CHANNEL_READTHENWRITETHROUGHPUT 16 // KVDRAM(R) -> BUFFER -> KVDRAM(W): how many uint512_dt wide-words can be read / sec?
#define HBM_DRAM_ACCESS_LATENCY 16

#define RESET   "\033[0m"
#define BOLDWHITE   "\033[1m\033[37m"
#define TIMINGRESULTSCOLOR BOLDWHITE

typedef unsigned int vertex_t;
typedef unsigned int edge_t;

typedef unsigned int keyy_t;
typedef unsigned int value_t;

typedef struct {
	unsigned int key;
	unsigned int value;
} keyvalue_t;

typedef keyvalue_t frontier_t;

typedef struct {
	keyy_t dstvid; 
} edge_type;

typedef struct {
	keyy_t srcvid;	
	keyy_t dstvid; 
} edge2_type;

typedef struct {
	keyy_t srcvid;
	keyy_t dstvid;
	unsigned int weight;
	unsigned int valid;
} edge3_type; 

typedef struct {
	edge3_type data[EDGE_PACK_SIZE];
} edge3_vec_dt;

// typedef struct {
	// keyy_t srcvid;
	// keyy_t dstvid;
// } edge_update_type; 
typedef edge3_type edge_update_type;

typedef struct {
	unsigned int offset;
	unsigned int size;
} map_t;

// #ifdef FPGA_IMPL
// typedef ap_uint<1> uint1_dt;
// #else
// typedef unsigned int uint1_dt;
// #endif

typedef struct {
	keyvalue_t data[EDGE_PACK_SIZE];
} uint512_vec_dt;

typedef struct {
	int data[HBM_CHANNEL_PACK_SIZE]; // 32
} uint512_ivec_dt;

// #ifdef FPGA_IMPL
// typedef ap_uint<HBM_AXI_PACK_BITSIZE> uint512_axivec_dt;
// #else
typedef struct {
	int data[HBM_AXI_PACK_SIZE]; // 16
} uint512_axivec_dt;
// #endif

typedef struct {
	int data[HBM_AXI_PACK_SIZE]; // 16
} uint512_axiswvec_dt;

typedef struct {
	unsigned int begin;
	unsigned int end;
    unsigned int size;
	unsigned int vid;
	unsigned int prop;
} vtr_t;

typedef struct {
	unsigned int A;
    unsigned int B;
} tuple_t;

typedef unsigned int vdata_t;

typedef struct {
	unsigned int prop;
	unsigned int mask;
} vprop_t;

typedef struct {
	vprop_t data[EDGE_PACK_SIZE];
} vprop_vec_t;

typedef struct {
	unsigned int ALGORITHM; 
	unsigned int NUM_ITERATIONS;
	unsigned int ROOTVID;
	
	unsigned int NUM_VERTICES;
	unsigned int NUM_EDGES;
	unsigned int AVERAGENUM_WORKEDGES_PER_CHANNEL;

	unsigned int NUM_UPARTITIONS;
	unsigned int NUM_APPLYPARTITIONS; // NUM_PEs
	
	unsigned int NUM_PARTITIONS;
} universalparams_t;

typedef struct {
	unsigned int BASEOFFSET_VPROP;
} globalparams_t;

typedef struct {
	unsigned int offset;
	unsigned int size;
	unsigned int count;
} offset_t;

// HBM: {vptrs, edges, updatesptrs, updates, vertexprops, frontiers}
typedef uint512_ivec_dt HBM_channel_t;
typedef uint512_axiswvec_dt HBM_channelAXISW_t;
typedef uint512_axivec_dt HBM_channelAXI_t;
#endif
