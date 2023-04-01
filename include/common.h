#ifndef COMMON_H
#define COMMON_H
#include <string.h> 
#include <cmath>
 

 
// #define ___ENABLE___DYNAMICGRAPHANALYTICS___
#ifndef ___RUNNING_FPGA_SYNTHESIS___
	#define ___CREATE_ACTPACK_FROM_VECTOR___
#endif 
#define NUM_FPGAS 2 // 1, 2*, 4

#define ALL_MODULES 222
// #define PREPARE_EDGEUPDATES_MODULE 2220
#define PREPROCESSING_MODULE 2220
#define COMMIT_EDGEUPDATES_MODULE 2221
#define PROCESS_EDGES_MODULE 2220
#define APPLY_UPDATES_MODULE 2222
#define GATHER_DSTPROPERTIES_MODULE 2223
#define APPLY_UPDATES_MODULE___AND___GATHER_DSTPROPERTIES_MODULE 2224

#define BAKE_PAGERANK_ALGORITHM_FUNCS
// #define BAKE_SPMV_ALGORITHM_FUNCS
// #define BAKE_HITS_ALGORITHM_FUNCS
// #define BAKE_SSSP_ALGORITHM_FUNCS

#define ___FORCE_SUCCESS___
#define POW_VALID_VDATA 0 // 1,0 // FIXME
#define ___FORCE_SUCCESS_SINGLE_CHANNEL___
// #define ADJUSTMENT_TO_MEET_VHLS_TIMING
#define ___PARTIAL___HBM___ACCESSES___

#define DRAM_ACCESS_LATENCY 8
#define _NUMNANOSECONDS_PER_CLOCKCYCLE_ (6956 / 1000) // 6.956 // {23M uint32 seen processed in 10ms}{10M uint32 seen processed in 4ms}
#define ACTS_AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC ((23 * 1000) / 10) // ((23 / 10) * 1000) // {23M uint32 seen processed in 10ms}{10M uint32 seen processed in 4ms}

#define ___USE_AXI_CHANNEL___

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define SW // SWEMU, HW, *SW
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
#define ___CODE___GATHER_FRONTIERINFOS___ 12
#define ___CODE___NUMBER_OF_EDGEUPDATES_LOADED___ 13
#define ___CODE___NUMBER_OF_EDGEUPDATES_APPLIED___ 14
#define ___CODE___NUMBER_OF_NEW_EDGEUPDATES_ADDED___ 15

//////////////// 

#define NUM_PROCS 2
#define NUM_PEs 12
#define NUM_VALID_PEs 1
#define NUM_VALID_HBM_CHANNELS 1
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
#define MAX_NUM_LLPSETS 32 // 64 // 16(original), 64*
#define NUM_LLP_PER_LLPSET EDGE_PACK_SIZE
#define MAX_NUM_LLP_PER_UPARTITION (MAX_NUM_LLPSETS * NUM_LLP_PER_LLPSET)
#define NAp 666
#define MAXNUMGRAPHITERATIONS 16
#define MAX_NUM_PARTITIONS 16 
#define INVALIDDATA 0xFFFFFFFF 
// #define INVALIDDATA_MINI 0xFFFF
#define INVALIDMASK 0 

#define UNIDENTIFIED_STAGE 0
// #define __UNIDENTIFIED__STATUS__ 0

#define MAXNUMBITS2_ACTPACK_SRCVID 14
#define MAXNUMBITS2_ACTPACK_DESTVID 14
#define MAXNUMBITS2_ACTPACK_EDGEID 4
#define MAXLOCALVALUE2_ACTPACK_SRCVID 0x3FFF // (2^14-1=16383)
#define MAXLOCALVALUE2_ACTPACK_DESTVID 0x3FFF
#define MAXLOCALVALUE2_ACTPACK_EDGEID 0xF
#define INVALIDDATA_SRCVID MAXLOCALVALUE2_ACTPACK_SRCVID
#define INVALIDDATA_DESTVID MAXLOCALVALUE2_ACTPACK_DESTVID

#define MULT_FACTOR (16 / EDGE_PACK_SIZE) // 1
#define MAX_UPARTITION_VECSIZE (8184 * MULT_FACTOR) // NEW**
#define MAX_UPARTITION_SIZE (EDGE_PACK_SIZE * MAX_UPARTITION_VECSIZE) // 131072 
#define MAX_APPLYPARTITION_VECSIZE MAX_UPARTITION_VECSIZE
#define MAX_APPLYPARTITION_SIZE (EDGE_PACK_SIZE * MAX_APPLYPARTITION_VECSIZE) // 131072 

#define VDATA_SUBPARTITION_VECSIZE (MAX_UPARTITION_VECSIZE / NUM_PEs) // 341 (8184 / 24)
#define VDATA_SUBSUBPARTITION_VECSIZE (1 << 4)
#define NUM_SUBPARTITION_PER_PARTITION NUM_PEs

#define NUM_IMPORT_BUFFERS MAX_NUM_UPARTITIONS // MAX_NUM_UPARTITIONS // 32 // FIXME. AUTOMATE. 
#define NUM_EXPORT_BUFFERS MAX_NUM_UPARTITIONS // MAX_NUM_UPARTITIONS //32 
#define INVALID_IOBUFFER_ID (NUM_IMPORT_BUFFERS - 1)
#define IMPORT_EXPORT_GRANULARITY_VECSIZE 8192

#define UPDATES_BUFFER_PACK_SIZE MAX(EDGE_PACK_SIZE, NUM_PEs)

#define MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION (1024 * MULT_FACTOR)

#define BLOCKRAM_SIZE (512 * MULT_FACTOR)
#define DOUBLE_BLOCKRAM_SIZE ((BLOCKRAM_SIZE * 2) * MULT_FACTOR)	
#ifdef ___FORCE_SUCCESS_SINGLE_CHANNEL___
#define VPTR_BUFFER_SIZE (512 * 8) // 16
#define EDGE_BUFFER_SIZE (512 * 8) // 16
#define UPDATES_BUFFER_SIZE (512 * 8) // 16
#define EDGEMAPS_BUFFER_SIZE 1024
#else 
#define VPTR_BUFFER_SIZE (512 * MULT_FACTOR)
#define EDGE_BUFFER_SIZE (512 * MULT_FACTOR)   //8192// 512 // FIXME.
#define UPDATES_BUFFER_SIZE (512 * MULT_FACTOR)	
#endif 	

// #define EDGE_UPDATES_BRAMBUFFER_SIZE 1024
#define EDGE_UPDATES_DRAMBUFFER_LONGSIZE 8192 * 16 // used for cpu implementation only
// #define EDGE_UPDATES_WORKBUFFER_LONGSIZE (EDGE_UPDATES_DRAMBUFFER_LONGSIZE - 1000)
#define EDGE_UPDATES_DRAMBUFFER_SIZE 8192
#define EDGE_UPDATES_WORKBUFFER_SIZE 7000 // 7200
#define EDGE_LLPUPDATES_DRAMBUFFER_SIZE 512

#define GLOBALBUFFER_SIZE 96
#define GLOBALPARAMSCODE__BASEOFFSET__ACTIONS 0
#define GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS 1
#define GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS 2
#define GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS 3
#define GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS 4
#define GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2 5
#define GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS 6
#define GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES 7 //
#define GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES 8 //
#define GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES 9 //
#define GLOBALPARAMSCODE__BASEOFFSET__CSREDGES 10
#define GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES 11 //
#define GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES 12
#define GLOBALPARAMSCODE__BASEOFFSET__VDATAS 13
#define GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP 14
#define GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS 15

#define GLOBALPARAMSCODE__WWSIZE__ACTIONS 20
#define GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS 21
#define GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS 22
#define GLOBALPARAMSCODE__WWSIZE__CSRVPTRS 23
#define GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS 24
#define GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2 25
#define GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS 26
#define GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES 27
#define GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES 28 //
#define GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES 29
#define GLOBALPARAMSCODE__WWSIZE__CSREDGES 30
#define GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES 31
#define GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES 32
#define GLOBALPARAMSCODE__WWSIZE__VDATAS 33
#define GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP 34
#define GLOBALPARAMSCODE__WWSIZE__NFRONTIERS 35

#define GLOBALPARAMSCODE__PARAM__NUM_VERTICES 40
#define GLOBALPARAMSCODE__PARAM__NUM_EDGES 41
#define GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS 42
#define GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS 43
#define GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS 44
#define GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEFRONTIERSFORCONTROLSWITCH 45
#define GLOBALPARAMSCODE__PARAM__MAXDEGREE 46
#define GLOBALPARAMSCODE__PARAM__ALGORITHM 47
#define GLOBALPARAMSCODE__PARAM__ROOTVID 48
#define GLOBALPARAMSCODE__PARAM__RANGEPERCHANNEL 49
#define GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEDSTVID 50
#define GLOBALPARAMSCODE__PARAM__NUM_RUNS 51

#define GLOBALPARAMSCODE___ENABLE___RESETBUFFERSATSTART 60
#define GLOBALPARAMSCODE___ENABLE___PREPAREEDGEUPDATES 61
#define GLOBALPARAMSCODE___ENABLE___PROCESSEDGEUPDATES 62
#define GLOBALPARAMSCODE___ENABLE___PROCESSEDGES 63
#define GLOBALPARAMSCODE___ENABLE___READ_FRONTIER_PROPERTIES 64
#define GLOBALPARAMSCODE___ENABLE___VCPROCESSEDGES 65
#define GLOBALPARAMSCODE___ENABLE___ECUPDATEEDGES 66
#define GLOBALPARAMSCODE___ENABLE___ECPROCESSEDGES 67
#define GLOBALPARAMSCODE___ENABLE___SAVEVCUPDATES 68
#define GLOBALPARAMSCODE___ENABLE___COLLECTACTIVEDSTVIDS 69
#define GLOBALPARAMSCODE___ENABLE___APPLYUPDATESMODULE 70
#define GLOBALPARAMSCODE___ENABLE___READ_DEST_PROPERTIES 71
#define GLOBALPARAMSCODE___ENABLE___APPLYUPDATES 72
#define GLOBALPARAMSCODE___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES 73
#define GLOBALPARAMSCODE___ENABLE___SAVE_DEST_PROPERTIES 74
#define GLOBALPARAMSCODE___ENABLE___EXCHANGEFRONTIERINFOS 75

#define GLOBALPARAMSCODE__ASYNC__BATCH 80
#define GLOBALPARAMSCODE__ASYNC__BATCHSIZE 81

#define GLOBALPARAMSCODE__COMMANDS__COMMAND0 85

////////////////

#define HBM_CHANNEL_READTHROUGHPUT 16 // KVDRAM -> BUFFER: how many uint512_dt wide-words can be read / sec?
#define HBM_CHANNEL_WRITETHROUGHPUT 16 // BUFFER -> KVDRAM: how many uint512_dt wide-words can be written / sec?
#define HBM_CHANNEL_READTHENWRITETHROUGHPUT 16 // KVDRAM(R) -> BUFFER -> KVDRAM(W): how many uint512_dt wide-words can be read / sec?
#define HBM_DRAM_ACCESS_LATENCY 16

#define RESET   "\033[0m"
#define BOLDWHITE   "\033[1m\033[37m"
#define TIMINGRESULTSCOLOR BOLDWHITE

#define __READY__FOR__IMPORT__ 1
#define __READY__FOR__PROCESS__ 2
#define __READY__FOR__EXPORT__ 3
#define __READY__FOR__TRANSPORT__ 4
#define __UNIDENTIFIED__STATUS__ 0

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

typedef edge2_type edge_update_type;

typedef struct {
	edge_update_type data[EDGE_PACK_SIZE];
} edge_update_vec_dt;

typedef struct {
	unsigned int offset;
	unsigned int size;
} map_t;

typedef struct {
	keyvalue_t data[EDGE_PACK_SIZE];
} uint512_vec_dt;

typedef struct {
	int data[HBM_CHANNEL_PACK_SIZE]; // 32
} uint512_ivec_dt;

typedef struct {
	int data[HBM_AXI_PACK_SIZE]; // 16
} uint512_axivec_dt;

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
	// unsigned int mask;
	unsigned int degree;
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

#define LOCAL_OWNED_VERTEX 0
#define REMOTE_OWNED_VERTEX 1

typedef struct {
	unsigned int ptr;
	unsigned int msg;
	unsigned int graph_iteration;
} checkpoint_t;

typedef struct {
	unsigned int fpga; 
	unsigned int module; 
	unsigned int graph_iteration;
	unsigned int start_pu; 
	unsigned int size_pu; 
	unsigned int skip_pu; 
	unsigned int start_pv;
	unsigned int size_pv; 
	unsigned int start_llpset; 
	unsigned int size_llpset; 
	unsigned int start_llpid; 
	unsigned int size_llpid; 
	unsigned int start_gv; 
	unsigned int size_gv;
	unsigned int id_process;
	unsigned int id_import;
	unsigned int id_export;
	unsigned int size_import_export;
	unsigned int status;
} action_t; 

typedef struct {
	unsigned int status;
	unsigned int ready_for_import;
	unsigned int ready_for_export;
	unsigned int ready_for_process;
	unsigned int iteration;
} gas_t;

typedef struct {
	unsigned int ready_for_import;
	unsigned int iteration;
} gas_import_t;

typedef struct {
	unsigned int ready_for_process;
	unsigned int iteration;
} gas_process_t;

typedef struct {
	unsigned int ready_for_export;
	unsigned int iteration;
} gas_export_t;

typedef struct {
	unsigned int fpga;
	unsigned int io_id;
} mapping_t;

// HBM: {vptrs, edges, updatesptrs, updates, vertexprops, frontiers}
typedef uint512_ivec_dt HBM_channel_t;
typedef uint512_axiswvec_dt HBM_channelAXISW_t;
typedef uint512_axivec_dt HBM_channelAXI_t;
#endif
