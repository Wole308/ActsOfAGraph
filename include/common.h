#ifndef COMMON_H
#define COMMON_H
#include "config_params.h"

#define SW // SWEMU, HW, SW
#define ACTGRAPH_SETUP // ACTGRAPH_SETUP, GRAFBOOST_SETUP
#define PR_ALGORITHM // PR_ALGORITHM, BFS_ALGORITHM, SSSP_ALGORITHM, BC_ALGORITHM, ADVANCE_ALGORITHM
#define _KRON21_2M_91M 
#if (defined(SWEMU) || defined(HW))
#define FPGA_IMPL
#endif 
#define CRABTREE_PLATFORM // AWS_PLATFORM, CRABTREE_PLATFORM
#ifdef ADVANCE_ALGORITHM
#define PR_ALGORITHM
#endif 

#define ACTSMODEL_LW
#ifdef GRAFBOOST_SETUP
#define SORTREDUCEMODEL
#endif 

#define LOCKE
#define _SINGLEKERNEL
#ifdef FPGA_IMPL
#define _WIDEWORD
#endif
#ifndef PR_ALGORITHM
#define ACTIVEVERTICESBASEDALGORITHM
#endif
#ifdef LOCKE
#define CONFIG_FACTOROUTSSDLATENCY
#define CONFIG_FACTOROUTOCLDATATRANSFERS
#endif 

#define ACTS
// #define TESTKERNEL_ACTSMAX //

////////////////

#if defined(ACTGRAPH_SETUP) && not defined(TESTKERNEL_ACTSMAX) && defined(PR_ALGORITHM) // FIXME. REMOVEME. fixme for bfs
#if defined(_ORKUT_3M_106M) || defined(_HOLLYWOOD_1M_57M) || defined(_INDOCHINA_7M_194M) || defined(_KRON21_2M_91M) || defined(_ROADNET_2M_3M) || defined(_FLICKR_1M_10M)		
// #define INMEMORYGP
#endif
#endif 

////////////////

#define _DEBUGMODE_HEADER //
#if defined (FPGA_IMPL) && defined (HW)
#else
#define _DEBUGMODE_STATS // 
// #define _DEBUGMODE_CHECKS
#define _DEBUGMODE_CHECKS2 // 
#define _DEBUGMODE_CHECKS3 //
// #define _DEBUGMODE_PRINTS
// #define _DEBUGMODE_KERNELPRINTS
#define _DEBUGMODE_KERNELPRINTS2 //
#define _DEBUGMODE_KERNELPRINTS3 //
#endif
#if defined(SW) & defined(TESTKERNEL)
#define _DEBUGMODE_STATS // 
#define _DEBUGMODE_CHECKS2 //
#define _DEBUGMODE_KERNELPRINTS2 //
#endif
// #define _DEBUGMODE_HOSTCHECKS
#define _DEBUGMODE_HOSTCHECKS2 //
// #define _DEBUGMODE_HOSTPRINTS
// #define _DEBUGMODE_HOSTPRINTS2 //
#define _DEBUGMODE_HOSTPRINTS3 //
// #define _DEBUGMODE_TIMERS
// #define _DEBUGMODE_TIMERS2
#define _DEBUGMODE_TIMERS3

////////////////

#define PAGERANK 222
#define BREADTHFIRSTSEARCH 333
#define SSSP 444
#define BETWEENNESSCENTRALITY 555

////////////////

#define NUMSUPERCPUTHREADS 1
#define NUMCPUTHREADS 1 // FIXME. overridden
#define NUMSUBCPUTHREADS_POW 0
#define NUMSUBCPUTHREADS (1 << NUMSUBCPUTHREADS_POW) 
#define NUMUTILITYTHREADS 16 // NUMCPUTHREADS // FIXME?

////////////////

#define DATAWIDTH 512 
#define VECTOR_SIZE 8
#define VECTOR1024_SIZE 16
#define DATATYPE_SIZE 32

#define NUMDRAMBANKS 4
#define NUMINSTANCES 1

#define NUM_PARTITIONS_POW 4
#define NUM_PARTITIONS (1 << NUM_PARTITIONS_POW)

#if defined(ACTGRAPH_SETUP)
#define MAXNUMSSDPARTITIONS 16
#define NUMSSDPARTITIONS_POW 4
#else 
#define MAXNUMSSDPARTITIONS 1
#define NUMSSDPARTITIONS_POW 0
#endif 
#define NUMSSDPARTITIONS (1 << NUMSSDPARTITIONS_POW)

#define MAXNUMVERTEXBANKS 16
#define MAXNUMEDGEBANKS 16
#define MAXNUMVERTICESPERBANK (KVDATA_RANGE / MAXNUMEDGEBANKS)
#define MAXNUMVERTICESPERBANK_KVS (MAXNUMVERTICESPERBANK / VECTOR_SIZE)

#define KVDATA_RANGE_POW 21
#define KVDATA_RANGE (1 << KVDATA_RANGE_POW)

#define NUMWORKERS 1
#define NUMSUBWORKERS 3

////////////////

#define SKEWRATIO_POW 7
#define SKEWRATIO (1 << SKEWRATIO_POW) // 128

#define KVDATA_RANGE_PERSSDPARTITION_POW (KVDATA_RANGE_POW - NUMSSDPARTITIONS_POW)
#define KVDATA_RANGE_PERSSDPARTITION (1 << KVDATA_RANGE_PERSSDPARTITION_POW)
#define KVDATA_RANGE_PERSSDPARTITION_KVS (KVDATA_RANGE_PERSSDPARTITION / VECTOR_SIZE)

#define KVDATA_RANGE_PERSSDPARTITION2_POW (KVDATA_RANGE_POW - NUMSSDPARTITIONS_POW - SKEWRATIO_POW)
#define KVDATA_RANGE_PERSSDPARTITION2 (1 << KVDATA_RANGE_PERSSDPARTITION2_POW)
#define KVDATA_RANGE_PERSSDPARTITION2_KVS (KVDATA_RANGE_PERSSDPARTITION2 / VECTOR_SIZE)

#define BATCH_RANGE_POW KVDATA_RANGE_PERSSDPARTITION_POW
#define BATCH_RANGE (1 << BATCH_RANGE_POW)
#define BATCH_RANGE_KVS (BATCH_RANGE / VECTOR_SIZE)

#define BATCH_RANGE2_POW KVDATA_RANGE_PERSSDPARTITION2_POW
#define BATCH_RANGE2 (1 << BATCH_RANGE2_POW)
#define BATCH_RANGE2_KVS (BATCH_RANGE2 / VECTOR_SIZE)

/** ACTS constraints equation! (NUM_PARTITIONS_POWx = KVDATA_RANGE_PERSSDPARTITION_POW - SRAMSZ_POW) */
#ifdef ACTSMODEL
#define SRAMSZ_POW 14
#endif 
#ifdef ACTSMODEL_LW
#define SRAMSZ_POW 10
#endif 
#define SRAMSZ (1 << SRAMSZ_POW)

#define APPROXTREE_DEPTH ((BATCH_RANGE_POW - SRAMSZ_POW + NUM_PARTITIONS_POW - 1) / NUM_PARTITIONS_POW)
#define TREE_DEPTH APPROXTREE_DEPTH

#define APPROXTREE_DEPTH2 ((BATCH_RANGE2_POW - SRAMSZ_POW + NUM_PARTITIONS_POW - 1) / NUM_PARTITIONS_POW)
#define TREE_DEPTH2 APPROXTREE_DEPTH2

#define NUMLASTLEVELPARTITIONS (1 << (NUM_PARTITIONS_POW * TREE_DEPTH))

#define NUMLASTLEVELPARTITIONS2 (1 << (NUM_PARTITIONS_POW * TREE_DEPTH2))

////////////////

#ifdef INMEMORYGP
#define MAXKVDATA_BATCHSIZE 6000000
#else 
#define MAXKVDATA_BATCHSIZE 10000000
// #define MAXKVDATA_BATCHSIZE 3000000 // REMOVEME.
// #define MAXKVDATA_BATCHSIZE 1000000
#endif 
#define MAXKVDATA_BATCHSIZE_KVS (MAXKVDATA_BATCHSIZE / VECTOR_SIZE)
#define KVDATA_BATCHSIZE MAXKVDATA_BATCHSIZE
#define KVDATA_BATCHSIZE_KVS (KVDATA_BATCHSIZE / VECTOR_SIZE)
#define EDGES_BATCHSIZE ((MAXKVDATA_BATCHSIZE / 2) * NUMSUBCPUTHREADS) 
#define EDGES_BATCHSIZE_KVS (EDGES_BATCHSIZE / VECTOR_SIZE)
#define PADDEDEDGES_BATCHSIZE (EDGES_BATCHSIZE + 1000000)

#if defined(FPGA_IMPL)
#define DRAMBATCHFACTOR 10
#else
#define DRAMBATCHFACTOR 1
#endif

////////////////

#define MESSAGES_SIZE 1024
#define MESSAGES_SIZE_KVS (MESSAGES_SIZE / VECTOR_SIZE)
#define MESSAGES_COMMANDID 0
#define MESSAGES_RUNKERNELCOMMANDID (MESSAGES_COMMANDID + 1)
#define MESSAGES_PROCESSCOMMANDID (MESSAGES_RUNKERNELCOMMANDID + 1)
#define MESSAGES_COLLECTSTATSCOMMANDID (MESSAGES_PROCESSCOMMANDID + 1)
#define MESSAGES_PARTITIONCOMMANDID (MESSAGES_COLLECTSTATSCOMMANDID + 1)
#define MESSAGES_APPLYUPDATESCOMMANDID (MESSAGES_PARTITIONCOMMANDID + 1)
#define MESSAGES_FINALNUMPARTITIONSID (MESSAGES_APPLYUPDATESCOMMANDID + 1)
#define MESSAGES_TREEDEPTHID (MESSAGES_FINALNUMPARTITIONSID + 1)
#define MESSAGES_SSDPARTITIONID (MESSAGES_TREEDEPTHID + 1)
#define MESSAGES_SRCVOFFSET (MESSAGES_SSDPARTITIONID + 1)
#define MESSAGES_SRCVSIZE (MESSAGES_SRCVOFFSET + 1)
#define MESSAGES_SRCVSIZE_KVS (MESSAGES_SRCVSIZE + 1)
#define MESSAGES_EDGESSIZE (MESSAGES_SRCVSIZE_KVS + 1)
#define MESSAGES_EDGESSIZE_KVS (MESSAGES_EDGESSIZE + 1)
#define MESSAGES_DESTVOFFSET (MESSAGES_EDGESSIZE_KVS + 1)
#define MESSAGES_ACTVVSIZE (MESSAGES_DESTVOFFSET + 1)
#define MESSAGES_FIRSTVID (MESSAGES_ACTVVSIZE + 1)
#define MESSAGES_FIRSTKEY (MESSAGES_FIRSTVID + 1)
#define MESSAGES_FIRSTVALUE (MESSAGES_FIRSTKEY + 1)
#define MESSAGES_TREEDEPTH (MESSAGES_FIRSTVALUE + 1)
#define MESSAGES_FINALNUMPARTITIONS (MESSAGES_TREEDEPTH + 1)
#define MESSAGES_BATCHSIZE (MESSAGES_FINALNUMPARTITIONS + 1)
#define MESSAGES_RUNSIZE (MESSAGES_BATCHSIZE + 1)
#define MESSAGES_RUNSIZE_KVS (MESSAGES_RUNSIZE + 1)
#define MESSAGES_NEXTBATCHOFFSET (MESSAGES_RUNSIZE_KVS + 1)
#define MESSAGES_GRAPHITERATIONID (MESSAGES_NEXTBATCHOFFSET + 1)
#define MESSAGES_GRAPHALGORITHMID (MESSAGES_GRAPHITERATIONID + 1)
#define MESSAGES_STATSALREADYCOLLECTED (MESSAGES_GRAPHALGORITHMID + 1)
#define MESSAGES_GROUPID (MESSAGES_STATSALREADYCOLLECTED + 1)
#define MESSAGES_BEGINLOP (MESSAGES_GROUPID + 1)
#define MESSAGES_ENDLOP (MESSAGES_BEGINLOP + 1)
#define MESSAGES_NUMLOPS (MESSAGES_ENDLOP + 1)
#define MESSAGES_BATCHRANGE (MESSAGES_NUMLOPS + 1)
#define MESSAGES_BATCHRANGE_KVS (MESSAGES_BATCHRANGE + 1)
#define MESSAGES_BATCHRANGE_POW (MESSAGES_BATCHRANGE_KVS + 1)
#define MESSAGES_APPLYVERTEXBUFFERSZ (MESSAGES_BATCHRANGE_POW + 1)
#define MESSAGES_APPLYVERTEXBUFFERSZ_KVS (MESSAGES_APPLYVERTEXBUFFERSZ + 1)
#define MESSAGES_BASEOFFSET_MESSAGESDRAM_KVS (MESSAGES_APPLYVERTEXBUFFERSZ_KVS + 1)
#define MESSAGES_BASEOFFSET_KVDRAMBUFFER_KVS (MESSAGES_BASEOFFSET_MESSAGESDRAM_KVS + 1)
#define MESSAGES_BASEOFFSET_KVDRAM_KVS (MESSAGES_BASEOFFSET_KVDRAMBUFFER_KVS + 1)
#define MESSAGES_BASEOFFSET_KVDRAMWORKSPACE_KVS (MESSAGES_BASEOFFSET_KVDRAM_KVS + 1)
#define MESSAGES_BASEOFFSET_STATSDRAM_KVS (MESSAGES_BASEOFFSET_KVDRAMWORKSPACE_KVS + 1)
#define MESSAGES_BASEOFFSET_ACTIVEVERTICES_KVS (MESSAGES_BASEOFFSET_STATSDRAM_KVS + 1)
#define MESSAGES_BASEOFFSET_EDGESDATA_KVS (MESSAGES_BASEOFFSET_ACTIVEVERTICES_KVS + 1)
#define MESSAGES_BASEOFFSET_VERTEXPTR_KVS (MESSAGES_BASEOFFSET_EDGESDATA_KVS + 1)
#define MESSAGES_BASEOFFSET_VERTICESDATA_KVS (MESSAGES_BASEOFFSET_VERTEXPTR_KVS + 1)

////////////////

#ifdef FPGA_IMPL
#define NUMFLAGS 1//2
#else 
#define NUMFLAGS 1
#endif

#define PROCESSEDGESCMD 1
#define PARTITIONCMD 2
#define APPLYUPDATESCMD 3

#define SRC_DST 5
#define DST_SRC 6
#define DST_SRC_EDGEW 7

#define DIRECTEDGRAPH 5
#define UNDIRECTEDGRAPH 6

#define ON 5
#define OFF 6

#define SOURCE 5
#define DEST 6

#define ACTSINFINITY 777777777

#define ACTIVE 1
#define INACTIVE 0

#define FINISHED 5
#define NOTFINISHED 6

#define NONE 6
#define NOTEXIST -1

#define STREAMING_ACCESS_TYPE 5
#define PARTIAL_ACCESS_TYPE 6

#define RESET   "\033[0m"
#define BOLDWHITE   "\033[1m\033[37m"
#define TIMINGRESULTSCOLOR BOLDWHITE

#define NUMBITSINCHAR 8
#define NUMBITSINUNSIGNEDINT (NUMBITSINCHAR * 4)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define GETMASK(index, size) (((1 << (size)) - 1) << (index))
#define READFROM(data, index, size) (((data) & GETMASK((index), (size))) >> (index))
#define WRITETO(data, index, size, value) ((data) = ((data) & (~GETMASK((index), (size)))) | ((value) << (index)))
#define FIELD(data, name, index, size) \
  inline decltype(data) name() { return READFROM(data, index, size); } \
  inline void set_##name(decltype(data) value) { WRITETO(data, index, size, value); }
#ifndef FPGA_IMPL // FIXME.
struct _bitData {
  unsigned int bitData;
  FIELD(bitData, one, 0, 1)
  FIELD(bitData, two, 1, 2)
};
#endif

#define INFINITI ((1 << 30) - 1)

#define NAp 666
#define INVALIDDATA 123456789

#define NUM_KAPI 3 // number of kernel args per instance

#define NUMGROUPS 1
#define LOWINDEGREESGROUPID 0
#define HIGHINDEGREESGROUPID 1

typedef unsigned int vertex_t;
#if (defined(_LARGEDATASET_1B) || defined(_LARGEDATASET_4B))
typedef unsigned long edge_t;
#else 
typedef unsigned int edge_t;
#endif 

typedef unsigned int keyy_t;
typedef unsigned int value_t;

typedef struct {
	unsigned int key;
	unsigned int value;
} keyvalue_t;

typedef struct {
	// vertex_t vid;
	vertex_t outdegree;
} vertexprop_t;

typedef struct {
	keyy_t dstvid;
} edgeprop1_t;

typedef struct {
	keyy_t dstvid;
	keyy_t srcvid; 
} edgeprop2_t;

typedef struct {
	keyy_t srcvid;
	keyy_t dstvid; 
} edge_type;

typedef struct {
	keyy_t dstvid; 
} edge2_type;

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

#ifdef _WIDEWORD
typedef ap_uint<256> uint256_dt;
#else
typedef struct {
	keyvalue_t data[4];
} uint256_dt;
#endif

#ifdef _WIDEWORD
typedef ap_uint<128> uint128_dt;
#else
typedef struct {
	keyvalue_t data[2];
} uint128_dt;
#endif

#ifdef _WIDEWORD
typedef ap_uint<64> uint64_dt;
#else
typedef struct {
	keyvalue_t data[1];
} uint64_dt;
#endif

typedef struct {
	keyvalue_t data[VECTOR_SIZE];
} uint512_vec_dt;

typedef struct {
	unsigned int offset;
    unsigned int size;
} metadata_t;

typedef struct {
	unsigned int reof;
    unsigned int weof;
} tracker_t;

typedef struct {
	unsigned int begin;
    unsigned int size;
} margin_t;

typedef struct {
	unsigned int begin;
    unsigned int end;
} margin2_t;

typedef struct {
    vertexprop_t data[VECTOR_SIZE];
} vertexpropset_t;

typedef struct {
	vertex_t gvid;
	vertex_t lvid;
} lgvertex_t;

typedef struct {
	unsigned int type;
	unsigned int numedgebanks;
	unsigned int numvertexbanks;
	bool changed;
} heuristicstype_t;

typedef struct {
	keyy_t srcvid;
	keyy_t dstvid;
	value_t ew;
} smartedge_t;

typedef struct {
	unsigned int id;
	unsigned int offset;
	unsigned int size;
} block_t;

typedef struct {
	unsigned long totalsize;
    long double totaltime_SSDtransfers_ms;
	long double totaltime_OCLtransfers_ms;
	long double totaltime_ms;
	long double totaltime_overallexcludingOCLandSSDtransfers_ms;
	unsigned int graph_iterationidx; 
} runsummary_t;

typedef struct {
	unsigned int datasize;
	unsigned int message;
} kvresults_t;
#endif

