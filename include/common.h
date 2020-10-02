#ifndef COMMON_H
#define COMMON_H
#include "config_params.h"

////////////////

#define SW // SWEMU, HW, SW
#define ACTGRAPH_SETUP // ACTGRAPH_SETUP, GRAFBOOST_SETUP
#define PR_ALGORITHM // PR_ALGORITHM, BFS_ALGORITHM, BC_ALGORITHM
#define _TWITTERDATASET_67M 
#if (defined(SWEMU) || defined(HW))
#define FPGA_IMPL
#endif 
#define CRABTREE_PLATFORM // AWS_PLATFORM, CRABTREE_PLATFORM
#define LOCKE
#define _SINGLEKERNEL
#ifdef FPGA_IMPL
// #define _WIDEWORD // REMOVEME.
#endif
#ifndef PR_ALGORITHM
#define ACTIVEVERTICESBASEDALGORITHM
#endif
#ifdef LOCKE
#define CONFIG_FACTOROUTSSDLATENCY
#define CONFIG_FACTOROUTOCLDATATRANSFERS
#endif 
#define ACTS
#define SKEWRATIO_POW 7
#define SKEWRATIO (1 << SKEWRATIO_POW) // 128

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
// #define _DEBUGMODE_HOSTPRINTS
#define _DEBUGMODE_HOSTPRINTS2 //
#define _DEBUGMODE_HOSTPRINTS3 //
// #define _DEBUGMODE_TIMERS
#define _DEBUGMODE_TIMERS2

////////////////

#define PAGERANK 222
#define BREADTHFIRSTSEARCH 333
#define BETWEENNESSCENTRALITY 444

////////////////

#define NUMSUPERCPUTHREADS 1
#define NUMCPUTHREADS 1 // FIXME. overridden
#define NUMSUBCPUTHREADS_POW 4
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

#define MAXNUMSSDPARTITIONS 16
#define NUMSSDPARTITIONS_POW 4
#define NUMSSDPARTITIONS (1 << NUMSSDPARTITIONS_POW)

#define MAXNUMVERTEXBANKS 16
#define MAXNUMEDGEBANKS 16
#define MAXNUMVERTICESPERBANK (KVDATA_RANGE / MAXNUMEDGEBANKS)
#define MAXNUMVERTICESPERBANK_KVS (MAXNUMVERTICESPERBANK / VECTOR_SIZE)

#define KVDATA_RANGE_POW 26
#define KVDATA_RANGE (1 << KVDATA_RANGE_POW)

#define NUMWORKERS 1
#define NUMSUBWORKERS 1

////////////////

#define KVDATA_RANGE_PERSSDPARTITION_POW (KVDATA_RANGE_POW - NUMSSDPARTITIONS_POW)
#define KVDATA_RANGE_PERSSDPARTITION (1 << KVDATA_RANGE_PERSSDPARTITION_POW)
#define KVDATA_RANGE_PERSSDPARTITION_KVS (KVDATA_RANGE_PERSSDPARTITION / VECTOR_SIZE)

#define KVDATA_RANGE_PERSSDPARTITION2_POW (KVDATA_RANGE_POW - NUMSSDPARTITIONS_POW - SKEWRATIO_POW)
#define KVDATA_RANGE_PERSSDPARTITION2 (1 << KVDATA_RANGE_PERSSDPARTITION2_POW)
#define KVDATA_RANGE_PERSSDPARTITION2_KVS (KVDATA_RANGE_PERSSDPARTITION2 / VECTOR_SIZE)

#if defined(TESTKERNEL) && defined(ACTSMODEL_LW) // REMOVEME.
#define BATCH_RANGE_POW (KVDATA_RANGE_PERSSDPARTITION_POW - 2) // FIXME?
#else 
#define BATCH_RANGE_POW (KVDATA_RANGE_PERSSDPARTITION_POW - NUMSUBCPUTHREADS_POW)
#endif 
/// #define BATCH_RANGE_POW (KVDATA_RANGE_PERSSDPARTITION_POW - NUMSUBCPUTHREADS_POW)
#define BATCH_RANGE (1 << BATCH_RANGE_POW)
#define BATCH_RANGE_KVS (BATCH_RANGE / VECTOR_SIZE)

#define BATCH_RANGE2_POW (KVDATA_RANGE_PERSSDPARTITION2_POW - NUMSUBCPUTHREADS_POW) // (BATCH_RANGE_POW - SKEWRATIO_POW)
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

#if defined(ACTSMODEL)
    #ifdef TESTKERNEL
	#define KVDATA_BATCHSIZE (1 << 26)
	#else 
	// #define KVDATA_BATCHSIZE (1 << 24)
	#define KVDATA_BATCHSIZE 10000000
	#endif 
#elif defined(ACTSMODEL_LW)
	#define KVDATA_BATCHSIZE 10000000
#else
    #define KVDATA_BATCHSIZE (1 << 24)
#endif
#define KVDATA_BATCHSIZE_KVS (KVDATA_BATCHSIZE / VECTOR_SIZE)

#if defined(FPGA_IMPL)
#define DRAMBATCHFACTOR 1 // 4
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
#define MESSAGES_VOFFSET (MESSAGES_SSDPARTITIONID + 1)
#define MESSAGES_VSIZE (MESSAGES_VOFFSET + 1)
#define MESSAGES_TREEDEPTH (MESSAGES_VSIZE + 1)
#define MESSAGES_FINALNUMPARTITIONS (MESSAGES_TREEDEPTH + 1)
#define MESSAGES_BATCHSIZE (MESSAGES_FINALNUMPARTITIONS + 1)
#define MESSAGES_RUNSIZE (MESSAGES_BATCHSIZE + 1)
#define MESSAGES_NEXTBATCHOFFSET (MESSAGES_RUNSIZE + 1)
#define MESSAGES_GRAPHITERATIONID (MESSAGES_NEXTBATCHOFFSET + 1)
#define MESSAGES_GRAPHALGORITHMID (MESSAGES_GRAPHITERATIONID + 1)
#define MESSAGES_STATSALREADYCOLLECTED (MESSAGES_GRAPHALGORITHMID + 1)
#define MESSAGES_GROUPID (MESSAGES_STATSALREADYCOLLECTED + 1)
#define MESSAGES_BEGINLOP (MESSAGES_GROUPID + 1)
#define MESSAGES_ENDLOP (MESSAGES_BEGINLOP + 1)
#define MESSAGES_BATCHRANGE (MESSAGES_ENDLOP + 1)
#define MESSAGES_BATCHRANGE_POW (MESSAGES_BATCHRANGE + 1)
#define MESSAGES_APPLYVERTEXBUFFERSZ (MESSAGES_BATCHRANGE_POW + 1)
#define MESSAGES_APPLYVERTEXBUFFERSZ_KVS (MESSAGES_APPLYVERTEXBUFFERSZ + 1)

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

#ifdef ACTGRAPH_SETUP
typedef vertex_t prvertexoffset_t;
typedef edge_t bfsvertexoffset_t;
#else 
typedef edge_t prvertexoffset_t;
typedef edge_t bfsvertexoffset_t;
#endif 

#ifdef PR_ALGORITHM
typedef prvertexoffset_t xvertexoffset_t;
#else 
typedef bfsvertexoffset_t xvertexoffset_t;
#endif 

#ifdef FPGA_IMPL
#ifdef FORCDFINISH
typedef unsigned int dramsz_dtype;
typedef unsigned int kvbuffersz_dtype;
#else 
typedef ap_uint<30> dramsz_dtype;
typedef ap_uint<16> kvbuffersz_dtype;
#endif 
#else 
typedef unsigned int dramsz_dtype;
typedef unsigned int kvbuffersz_dtype;
#endif 

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

