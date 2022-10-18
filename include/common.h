#ifndef COMMON_H
#define COMMON_H
#include "config_params.h"
#include <string.h> 
#include <cmath>
 

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define RK
#define SW // SWEMU, HW, SW
#define PR_ALGORITHM //  // PR_ALGORITHM, CF_ALGORITHM, HITS_ALGORITHM, CC_ALGORITHM, SSSP_ALGORITHM, BFS_ALGORITHM
#if (defined(SWEMU) || defined(HW))
#define FPGA_IMPL
#endif 

// #define RANKING_ALGORITHM
#define TRAVERSAL_ALGORITHM

#if defined(BFS_ALGORITHM) || defined(SSSP_ALGORITHM)
#define CONFIG_ALGORITHMTYPE_RANDOMACTIVEVERTICES // { utility.cpp }
#endif

#define _DEBUGMODE_HEADER //
#if defined (FPGA_IMPL) // && defined (HW) // REMOVEME. 
#else
#define _DEBUGMODE_STATS // CRITICAL ADDME
// #define _DEBUGMODE_CHECKS
#define _DEBUGMODE_CHECKS2 // CRITICAL ADDME
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

#ifndef FPGA_IMPL
// #define _DEBUGMODE_KERNELPRINTS_TRACE3 //
#endif 

////////////////

#define PAGERANK 222
#define CF 333
#define HITS 444
#define SPMV 555
#define BFS 666
#define SSSP 777
#define ALGORITHMCLASS_ALLVERTEXISACTIVE 2222 
#define ALGORITHMCLASS_NOTALLVERTEXISACTIVE 3333

//////////////// 

#define NUM_PEs 24
#define MAXNUM_VPs 1024
#define MAXNUM_LLPs 256 
#define MAXNUM_LLPSETs 16 
#define VECTOR_SIZE 8
#define VECTOR2_SIZE 16
#define HBM_CHANNEL_VECTOR_SIZE 16
#define VDATA_PACKINGSIZE 16 
#define EDGE_PACK_SIZE 16
#define HBM_CHANNEL_PACK_SIZE 32
#define HBM_CHANNEL_SIZE (((1 << 28) / 4) / EDGE_PACK_SIZE) // in EDGE_PACK_SIZE (4194304)
#define FOLD_SIZE 1
#define MAX_NUM_UPARTITIONS 512
#define MAX_NUM_APPLYPARTITIONS 16 
#define MAX_NUM_LLPSETS 16
#define NAp 666
#define MAXNUMGRAPHITERATIONS 16
#define MAX_NUM_PARTITIONS 16 
#define INVALIDDATA 0xFFFFFFFF 
#define INVALIDMASK 0 

#define MAX_UPARTITION_VECSIZE 8184 // 8192
#define MAX_UPARTITION_SIZE (EDGE_PACK_SIZE * MAX_UPARTITION_VECSIZE) // 131072 
#define MAX_APPLYPARTITION_VECSIZE MAX_UPARTITION_VECSIZE
#define MAX_APPLYPARTITION_SIZE (EDGE_PACK_SIZE * MAX_APPLYPARTITION_VECSIZE) // 131072 
#define MAX_ACTVV_VECSIZE (MAX_UPARTITION_VECSIZE / NUM_PEs) // 341
#define MAX_ACTVV_SIZE (EDGE_PACK_SIZE * MAX_ACTVV_VECSIZE) // 5456
#define NUM_ACTVVPARTITIONS_PER_APPLYPARTITION 24
#define MAX_NUM_ACTVVPARTITIONS (MAX_NUM_APPLYPARTITIONS * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION * NUM_PEs) // 384

#define CSRBUFFER_SIZE 4096 // 512
#define MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION 1024
#define CSRDRAM_SIZE 16384 // 4096
#define ACTVUPDATESBLOCK_VECSIZE 16
#define BLOCKRAM_SIZE 512
#define DOUBLE_BLOCKRAM_SIZE (BLOCKRAM_SIZE * 2)
#define VPTR_BUFFER_SIZE 512
#define VPTR_BUFFERMETADATA_SIZE 512
#define EDGE_BUFFER_SIZE 512//8192// 512 // FIXME.

#define GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS 0
#define GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS 1
#define GLOBALPARAMSCODE__BASEOFFSET__CSREDGES 2
#define GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES 3
#define GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS 4
#define GLOBALPARAMSCODE__BASEOFFSET__UPDATES 5
#define GLOBALPARAMSCODE__BASEOFFSET__VDATAS 6
#define GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP 7
#define GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS 8

#define GLOBALPARAMSCODE__WWSIZE__CSRVPTRS 10
#define GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS 11
#define GLOBALPARAMSCODE__WWSIZE__CSREDGES 12
#define GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES 13
#define GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS 14
#define GLOBALPARAMSCODE__WWSIZE__UPDATES 15
#define GLOBALPARAMSCODE__WWSIZE__VDATAS 16
#define GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP 17
#define GLOBALPARAMSCODE__WWSIZE__NFRONTIERS 18

////////////////

#define ENABLE__SPARSEPROC

////////////////

#if NUM_PEs==3
#define NUMCOMPUTEUNITS_SLR0 1
#define NUMCOMPUTEUNITS_SLR1 1
#define NUMCOMPUTEUNITS_SLR2 1
#endif 
#if NUM_PEs==12
#define NUMCOMPUTEUNITS_SLR0 2
#define NUMCOMPUTEUNITS_SLR1 5
#define NUMCOMPUTEUNITS_SLR2 5
#endif 
#if NUM_PEs==14
#define NUMCOMPUTEUNITS_SLR0 4
#define NUMCOMPUTEUNITS_SLR1 5
#define NUMCOMPUTEUNITS_SLR2 5
#endif 
#if NUM_PEs==16
#define NUMCOMPUTEUNITS_SLR0 4 
#define NUMCOMPUTEUNITS_SLR1 6
#define NUMCOMPUTEUNITS_SLR2 6
#endif 
#if NUM_PEs==18
#define NUMCOMPUTEUNITS_SLR0 6
#define NUMCOMPUTEUNITS_SLR1 6
#define NUMCOMPUTEUNITS_SLR2 6
#endif 
#if NUM_PEs==20
#define NUMCOMPUTEUNITS_SLR0 6
#define NUMCOMPUTEUNITS_SLR1 7
#define NUMCOMPUTEUNITS_SLR2 7
#endif 
#if NUM_PEs==22
#define NUMCOMPUTEUNITS_SLR0 6
#define NUMCOMPUTEUNITS_SLR1 8
#define NUMCOMPUTEUNITS_SLR2 8
#endif 
#if NUM_PEs==24
#define NUMCOMPUTEUNITS_SLR0 6
#define NUMCOMPUTEUNITS_SLR1 9
#define NUMCOMPUTEUNITS_SLR2 9
#endif 
#if NUM_PEs==25
#define NUMCOMPUTEUNITS_SLR0 7
#define NUMCOMPUTEUNITS_SLR1 9
#define NUMCOMPUTEUNITS_SLR2 9
#endif 
#if NUM_PEs==32
#define NUMCOMPUTEUNITS_SLR0 12
#define NUMCOMPUTEUNITS_SLR1 10
#define NUMCOMPUTEUNITS_SLR2 10
#endif 

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

#ifdef FPGA_IMPL
typedef ap_uint<1> unit1_type;
#else 
typedef unsigned int unit1_type;
#endif 

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

typedef struct {
	unsigned int offset;
	unsigned int size;
} map_t;

#ifdef FPGA_IMPL
typedef ap_uint<1024> uint1024_dt;
#else
typedef struct {
	keyvalue_t data[16];
} uint1024_dt;
#endif

#ifdef FPGA_IMPL
typedef ap_uint<DATAWIDTH> uint512_dt;
#else
typedef struct {
	keyvalue_t data[VECTOR_SIZE];
} uint512_dt;
#endif

typedef struct {
	keyvalue_t data[EDGE_PACK_SIZE];
} uint512_vec_dt;

typedef struct {
	unsigned int data[HBM_CHANNEL_PACK_SIZE];
} uint512_ivec_dt;

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

#ifdef FPGA_IMPL
typedef ap_uint<32> vtxbuffer_type;
#else
typedef unsigned int vtxbuffer_type;
#endif 

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

typedef struct {
	keyvalue_t * cfrontier_dram[EDGE_PACK_SIZE][MAX_NUM_UPARTITIONS]; // [MAX_UPARTITION_SIZE]
} HBM_center_t;

typedef struct {
	// unsigned int * v_ptr; // [NUM_VERTICES / NUM_PEs]
	// map_t * vptr_actpack[MAX_NUM_UPARTITIONS];
	// edge3_vec_dt * csr_pack_edges; // [~fit] 
	// edge3_vec_dt * act_pack_edges; // [~fit]
	uint512_vec_dt * updates_dram[MAX_NUM_APPLYPARTITIONS]; // [MAX_NUM_APPLYPARTITIONS][HBM_CHANNEL_SIZE]
	// vprop_vec_t * vdatas_dram[MAX_NUM_APPLYPARTITIONS]; // [MAX_NUM_APPLYPARTITIONS][MAX_APPLYPARTITION_VECSIZE]
	// uint512_vec_dt * cfrontier_dram_tmp;  // [MAX_APPLYPARTITION_VECSIZE]
	// uint512_vec_dt * nfrontier_dram[MAX_NUM_UPARTITIONS];  // [MAX_NUM_UPARTITIONS][MAX_APPLYPARTITION_VECSIZE]
	
	// {vptrs, edges, updatesptrs, updates, vertexprops, frontiers}
	uint512_ivec_dt * globalparams;
	uint512_ivec_dt * HBM; // [~]
} HBM_channel_t;

typedef struct {
	keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]; // [EDGE_PACK_SIZE][MAX_APPLYPARTITION_VECSIZE]
	keyvalue_t * cfrontier_buffer[EDGE_PACK_SIZE]; // [EDGE_PACK_SIZE][MAX_APPLYPARTITION_VECSIZE]
	unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]; // [EDGE_PACK_SIZE][BLOCKRAM_SIZE]
	unsigned int * stats_buffer[MAX_NUM_APPLYPARTITIONS]; // [MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE]
	unsigned int * stats2_buffer[MAX_NUM_APPLYPARTITIONS]; // [MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE]
	keyvalue_t * updates_tmpbuffer[NUM_PEs]; // [NUM_PEs][CSRBUFFER_SIZE(//FIXME.)]
	keyvalue_t * updates_buffer[EDGE_PACK_SIZE]; // [EDGE_PACK_SIZE][MAX_APPLYPARTITION_VECSIZE]
	vtr_t * vptr_buffer; // [NUM_PEs][VPTR_BUFFER_SIZE]
	edge3_type * edges_buffer[EDGE_PACK_SIZE]; // [EDGE_PACK_SIZE][EDGE_BUFFER_SIZE]
	vprop_t * vertex_buffer[EDGE_PACK_SIZE]; // [EDGE_PACK_SIZE][MAX_APPLYPARTITION_VECSIZE]
} BRAM_channel_t;
#endif
