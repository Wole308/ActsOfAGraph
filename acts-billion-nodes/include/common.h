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

#define _SINGLEKERNEL
#ifdef FPGA_IMPL
#define _WIDEWORD
#endif 

#ifdef FPGA_IMPL
#define ENABLE_KERNEL_PROFILING
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
#define HBM_CHANNEL_SIZE (((1 << 28) / 4) / 16)
#define VDATA_PACKINGSIZE 16 
#define EDGE_PACK_SIZE 16
#define FOLD_SIZE 1
#define MAX_NUM_UPARTITIONS 512
// #define MAX_NUM_APPLYPARTITIONS 6 
#define MAX_NUM_APPLYPARTITIONS 16 // FIXME.
#define MAX_NUM_LLPSETS 16
#define NAp 666
#define MAXNUMGRAPHITERATIONS 16
#define MAX_NUM_PARTITIONS 16
#define INVALIDDATA 0xFFFFFFFF 

#define MAX_UPARTITION_VECSIZE 8184 // 8192
#define MAX_UPARTITION_SIZE (EDGE_PACK_SIZE * MAX_UPARTITION_VECSIZE) // 131072 
#define MAX_APPLYPARTITION_VECSIZE MAX_UPARTITION_VECSIZE
#define MAX_APPLYPARTITION_SIZE (EDGE_PACK_SIZE * MAX_APPLYPARTITION_VECSIZE) // 131072 
#define MAX_ACTVV_VECSIZE (MAX_UPARTITION_VECSIZE / NUM_PEs) // 341
#define MAX_ACTVV_SIZE (EDGE_PACK_SIZE * MAX_ACTVV_VECSIZE) // 5456
#define NUM_ACTVVPARTITIONS_PER_APPLYPARTITION 24
#define MAX_NUM_ACTVVPARTITIONS (MAX_NUM_APPLYPARTITIONS * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION * NUM_PEs) // 384

#define CSRBUFFER_SIZE 4096 // 512
#define CSRDRAM_SIZE 16384 // 4096
#define ACTVUPDATESBLOCK_VECSIZE 16
#define BLOCKRAM_SIZE 512
#define DOUBLE_BLOCKRAM_SIZE (BLOCKRAM_SIZE * 2)

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

#ifdef _WIDEWORD
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
	unsigned int weight;
} edge2_type; 

typedef struct {
	keyy_t srcvid;
	keyy_t dstvid;
	bool valid;
} edge3_type; 

typedef struct {
	edge2_type data[EDGE_PACK_SIZE + 2];
} edge2_vec_dt;

typedef struct {
	edge3_type data[EDGE_PACK_SIZE + 2];
} edge3_vec_dt;

typedef struct {
	unsigned int offset;
	unsigned int size;
} map_t;

#ifdef _WIDEWORD
typedef ap_uint<1024> uint1024_dt;
#else
typedef struct {
	keyvalue_t data[16];
} uint1024_dt;
#endif

#ifdef _WIDEWORD
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
	value_t data[EDGE_PACK_SIZE];
} uint512_evec_dt;

typedef struct {
	keyvalue_t data[EDGE_PACK_SIZE];
} uint512_uvec_dt;

typedef struct {
	unsigned int data[EDGE_PACK_SIZE];
} uint512_ivec_dt;

typedef struct {
	unsigned int offset;
    unsigned int size;
} metadata_t;

typedef struct {
	unsigned int A;
    unsigned int B;
} tuple_t;

typedef unsigned int vdata_t;

typedef struct {
	unsigned int data;
	unsigned int mask;
} vprop_t;

typedef struct {
	vprop_t data[EDGE_PACK_SIZE];
} vprop_vec_t;

#ifdef _WIDEWORD
typedef ap_uint<32> vtxbuffer_type;
#else
typedef unsigned int vtxbuffer_type;
#endif 

typedef struct {
	unsigned int DRAM_BASE_KVS;
	
	unsigned int ENABLE_RUNKERNELCOMMAND;
	unsigned int ENABLE_PROCESSCOMMAND;
	unsigned int ENABLE_PARTITIONCOMMAND;
	unsigned int ENABLE_APPLYUPDATESCOMMAND;
	unsigned int ENABLE_MERGECOMMAND;
	unsigned int ENABLE_EXCHANGECOMMAND;
	unsigned int GPMODE;
	unsigned int ACTSCONFIG_INSERTSTATSMETADATAINEDGES;
	
	unsigned int EVALUATION_ACTS_MEMORYLAYOUT;
	unsigned int EVALUATION_ACTS_PARTITIONINGLOGIC;
	unsigned int EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC;
	unsigned int EVALUATION_ACTS_HYBRIDLOGIC;
	unsigned int EVALUATION_ACTS_VERYBARE;
	
	unsigned int THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD;
	unsigned int THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION;
	unsigned int THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION;
	unsigned int THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION;

	unsigned int BASEOFFSETKVS_MESSAGESDATA;
	unsigned int VOLUMEOFFSETKVS_WORKDATA;
	unsigned int VOLUMEOFFSETKVS_WORKSPACEDATA;
	unsigned int BASEOFFSETKVS_EDGES0DATA;
	unsigned int BASEOFFSETKVS_EDGESDATA;
	unsigned int BASEOFFSETKVS_VERTEXPTR;
	unsigned int BASEOFFSETKVS_SRCVERTICESDATA;
	unsigned int BASEOFFSETKVS_DESTVERTICESDATA;
	unsigned int BASEOFFSETKVS_ACTIVEVERTICESDATA;
	unsigned int BASEOFFSETKVS_ACTIVEUPROPBLOCKS;
	unsigned int BASEOFFSETKVS_ACTIVEEDGEBLOCKS;
	unsigned int BASEOFFSETKVS_ACTIVEUPDATEBLOCKS;
	unsigned int BASEOFFSETKVS_EDGESMAP;
	unsigned int BASEOFFSETKVS_EDGEBLOCKMAP;
	unsigned int BASEOFFSETKVS_VERTICESPARTITIONMASK;
	unsigned int BASEOFFSETKVS_STATSDRAM;
	unsigned int BASEOFFSETKVS_EDGESSTATSDRAM;
	unsigned int BASEOFFSETKVS_VERTEXUPDATESDATA;
	unsigned int BASEOFFSETKVS_KVDRAM;
	unsigned int BASEOFFSETKVS_KVDRAMWORKSPACE;
	unsigned int BASEOFFSETKVS_OTHERINFOS;
	unsigned int BASEOFFSETKVS_ENDOFFILE;
	
	unsigned int BUFFERBASEOFFSETKVS_VDATA1;
	unsigned int BUFFERBASEOFFSETKVS_VDATA2;
	unsigned int BUFFERBASEOFFSETKVS_VDATA3;
	unsigned int BUFFERBASEOFFSETKVS_VDATA4;
	unsigned int BUFFERBASEOFFSETKVS_VDATA5;

	unsigned int SIZE_MESSAGESDRAM;
	unsigned int VOLUMESIZEU32_WORKDATA;
	unsigned int VOLUMESIZEU32_WORKSPACEDATA;
	unsigned int SIZE_EDGES0;
	unsigned int SIZE_EDGES;
	unsigned int SIZE_VERTEXPTRS;
	unsigned int SIZE_SRCVERTICESDATA;
	unsigned int SIZE_DESTVERTICESDATA;
	unsigned int SIZE_ACTIVEVERTICESDATA;
	unsigned int SIZE_ACTIVEUPROPBLOCKS;
	unsigned int SIZE_ACTIVEEDGEBLOCKS;
	unsigned int SIZE_ACTIVEUPDATEBLOCKS;
	unsigned int SIZE_EDGESMAP;
	unsigned int SIZE_EDGEBLOCKMAP;
	unsigned int SIZE_VERTICESPARTITIONMASK;
	unsigned int SIZE_KVSTATSDRAM;
	unsigned int SIZE_EDGESSTATSDRAM;
	unsigned int SIZE_VERTEXUPDATESDATA;
	unsigned int SIZE_KVDRAM;
	unsigned int SIZE_KVDRAMWORKSPACE;
	unsigned int SIZE_OTHERINFOS;
	unsigned int SIZEKVS2_PROCESSEDGESPARTITION;
	unsigned int SIZE_REDUCE;
	unsigned int SIZEKVS2_REDUCEPARTITION;
	unsigned int SIZEKVS_VMASKBUFFER;
	unsigned int SIZE_BATCHRANGE;
	unsigned int SIZE_RUN;

	unsigned int POW_MESSAGESDRAM;
	unsigned int POW_EDGES0;
	unsigned int POW_EDGES;
	unsigned int POW_VERTEXPTRS;
	unsigned int POW_SRCVERTICESDATA;
	unsigned int POW_DESTVERTICESDATA;
	unsigned int POW_ACTIVEEDGEBLOCKS;
	unsigned int POW_VERTICESDATAMASK;
	unsigned int POW_KVSTATSDRAM;
	unsigned int POW_EDGESSTATSDRAM;
	unsigned int POW_VERTEXUPDATESDATA;
	unsigned int POW_KVDRAM;
	unsigned int POW_KVDRAMWORKSPACE;
	unsigned int POW_REDUCE;
	unsigned int POW_BATCHRANGE;
	
	unsigned int NUM_PROCESSEDGESPARTITIONS;
	unsigned int NUM_REDUCEPARTITIONS;

	unsigned int ALGORITHMINFO_GRAPHITERATIONID;
	unsigned int ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int ALGORITHMINFO_GRAPHALGORITHMCLASS; // computed online

	unsigned int ACTSPARAMS_BEGINLOP;
	unsigned int ACTSPARAMS_NUMLOPS;
	unsigned int ACTSPARAMS_TREEDEPTH;
	unsigned int ACTSPARAMS_FINALNUMPARTITIONS;
	unsigned int ACTSPARAMS_SRCVOFFSET;
	unsigned int ACTSPARAMS_SRCVSIZE;
	unsigned int ACTSPARAMS_DESTVOFFSET;
	unsigned int ACTSPARAMS_NUMEDGECHUNKSINABUFFER;
	unsigned int ACTSPARAMS_INSTID;
	unsigned int ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM;
	unsigned int ACTSPARAMS_NUM_PARTITIONS;
	unsigned int ACTSPARAMS_NUM_PARTITIONS_LOP1;
	unsigned int ACTSPARAMS_NUM_PARTITIONS_LOP2;
	unsigned int ACTSPARAMS_BATCH_RANGE;
	unsigned int ACTSPARAMS_POW_PARTITIONS;
	unsigned int ACTSPARAMS_POW_PARTITIONS_LOP1;
	unsigned int ACTSPARAMS_POW_PARTITIONS_LOP2;
	unsigned int ACTSPARAMS_POW_PARTITIONS_LOP3;
	unsigned int ACTSPARAMS_WORKBUFFER_SIZE;
	unsigned int ACTSPARAMS_MAXHBMCAPACITY_KVS;
	unsigned int ACTSPARAMS_VPTR_SHRINK_RATIO;
	unsigned int ACTSPARAMS_READOFFLINESTATS;
	unsigned int ACTSPARAMS_UNUSED4;
	unsigned int ACTSPARAMS_NUMCOMPUTEUNITS_SLR2;
	unsigned int ACTSPARAMS_NUMCOMPUTEUNITS_SLR1;
	unsigned int ACTSPARAMS_NUMCOMPUTEUNITS_SLR0;
	
	unsigned int DUMMYCHKPOINT;
	unsigned int BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM;
	unsigned int RETURNVALUES;
	unsigned int MAILBOX;

	unsigned int VARS_WORKBATCH;
} globalparams_t;

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
	unsigned int offset;
	unsigned int size;
	unsigned int count;
} offset_t;
#endif

