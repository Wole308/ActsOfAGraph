#ifndef COMMON_H
#define COMMON_H
#include "../config/config_params.h"

////////////////

#define SW // SWEMU, HW, SW
#define ACTGRAPH_SETUP // ACTGRAPH_SETUP, GRAFBOOST_SETUP
#define PR_ALGORITHM // PR_ALGORITHM, BFS_ALGORITHM, BC_ALGORITHM
#define _LARGEDATASET_1B 
#if (defined(SWEMU) || defined(HW))
#define FPGA_IMPL
#endif 
#define CRABTREE_PLATFORM // AWS_PLATFORM, CRABTREE_PLATFORM
#define NOLOCKE
#define _SINGLEKERNEL
#ifdef FPGA_IMPL
#define _WIDEWORD
#endif
#ifndef PR_ALGORITHM
#define ACTIVEVERTICESBASEDALGORITHM
#endif

////////////////

#define _DEBUGMODE_HEADER //
#if defined (FPGA_IMPL) && defined (HW)
#else
#define _DEBUGMODE_STATS // 
// #define _DEBUGMODE_CHECKS
// #define _DEBUGMODE_CHECKS2 // 
#define _DEBUGMODE_CHECKS3 //
// #define _DEBUGMODE_PRINTS
// #define _DEBUGMODE_KERNELPRINTS
#define _DEBUGMODE_KERNELPRINTS2 //
#define _DEBUGMODE_KERNELPRINTS3 //
#endif
// #define _DEBUGMODE_HOSTPRINTS
#define _DEBUGMODE_HOSTPRINTS2
// #define _DEBUGMODE_TIMERS
#define _DEBUGMODE_TIMERS2

////////////////

#define DATAWIDTH 512 
#define VECTOR_SIZE 8

#define NUMDRAMBANKS 4
#define NUMINSTANCES 4

#define NUM_PARTITIONS_POW 4
#define NUM_PARTITIONS (1 << NUM_PARTITIONS_POW)

#define MAXNUMSSDPARTITIONS 16
#define NUMSSDPARTITIONS_POW 4
#define NUMSSDPARTITIONS (1 << NUMSSDPARTITIONS_POW)

#define MAXNUMVERTEXBANKS 16
#define MAXNUMEDGEBANKS 16
#define MAXNUMVERTICESPERBANK (KVDATA_RANGE / MAXNUMEDGEBANKS)
#define MAXNUMVERTICESPERBANK_KVS (MAXNUMVERTICESPERBANK / VECTOR_SIZE)

#define NUMCPUTHREADS 4

#ifdef _EQUALDATASET_X
#define TREE_DEPTH 2
#endif
#ifdef _LARGEDATASET_1M
#define TREE_DEPTH 2
#endif
#ifdef _LARGEDATASET_67M
#define TREE_DEPTH 3
#endif
#ifdef _LARGEDATASET_268M
#define TREE_DEPTH 3
#endif
#ifdef _LARGEDATASET_1B
#define TREE_DEPTH 3
#endif
#ifdef _LARGEDATASET_4B
#define TREE_DEPTH 4
#endif

#ifdef _EQUALDATASET_X
#define KVDATA_RANGE_POW 24
#endif
#ifdef _LARGEDATASET_1M 
#define KVDATA_RANGE_POW 20
#endif
#ifdef _LARGEDATASET_67M
#define KVDATA_RANGE_POW 26
#endif
#ifdef _LARGEDATASET_268M 
#define KVDATA_RANGE_POW 28 
#endif
#ifdef _LARGEDATASET_1B 
#define KVDATA_RANGE_POW 30 
#endif
#ifdef _LARGEDATASET_4B 
#define KVDATA_RANGE_POW 30 
#endif
#define KVDATA_RANGE (1 << KVDATA_RANGE_POW)

#define NUMWORKERS 1
#define NUMSUBWORKERS 4

#define KVDATA_RANGE_PERSSDPARTITION_POW (KVDATA_RANGE_POW - NUMSSDPARTITIONS_POW)
#define KVDATA_RANGE_PERSSDPARTITION (1 << KVDATA_RANGE_PERSSDPARTITION_POW)
#define KVDATA_RANGE_PERSSDPARTITION_KVS (KVDATA_RANGE_PERSSDPARTITION / VECTOR_SIZE)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION0 (0 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION1 (1 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION2 (2 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION3 (3 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION4 (4 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION5 (5 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION6 (6 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION7 (7 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION8 (8 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION9 (9 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION10 (10 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION11 (11 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION12 (12 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION13 (13 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION14 (14 * KVDATA_RANGE_PERSSDPARTITION)
#define KVDATA_RANGEOFFSET_FORSSDPARTITION15 (15 * KVDATA_RANGE_PERSSDPARTITION)

//////////////// crabtree
// #define KVBUFFER_SIZE (1 << 12)
#define KVBUFFER_SIZE (1 << 9)

// #define KVDATA_BATCHSIZE (1 << 24)
#define KVDATA_BATCHSIZE (1 << 26)
// #define KVDATA_BATCHSIZE (1 << 27)
// #define KVDATA_BATCHSIZE (1 << 28)
#define KVDATA_BATCHSIZE_KVS (KVDATA_BATCHSIZE / VECTOR_SIZE)

#define NUMLASTLEVELPARTITIONS (1 << (NUM_PARTITIONS_POW * TREE_DEPTH))
#define KVSTATS_SIZE (NUMLASTLEVELPARTITIONS * TREE_DEPTH)
#define KVSTATS_SIZE_KVS (KVSTATS_SIZE / VECTOR_SIZE)
#define KVSTATS_SIZE_POW (NUM_PARTITIONS_POW * TREE_DEPTH)

#define DRAMBATCHFACTOR 1
#define KVDRAMSZ (DRAMBATCHFACTOR * KVDATA_BATCHSIZE)
#define KVDRAMSZ_KVS (KVDRAMSZ / VECTOR_SIZE)
#define KVDRAMWORKSPACESZ KVDRAMSZ
#define KVDRAMWORKSPACESZ_KVS (KVDRAMWORKSPACESZ / VECTOR_SIZE)
#define CAPSULESZ ((KVDRAMSZ / KVBUFFER_SIZE) * NUM_PARTITIONS)

#define CAPSULESZ_KVS (CAPSULESZ / VECTOR_SIZE)
#define STATSDRAMSZ KVSTATS_SIZE
#define MESSAGESDRAMSZ MESSAGES_SIZE

#define KVSOURCEDRAMSZ (KVDRAMSZ + KVDRAMWORKSPACESZ + CAPSULESZ)
#define KVSOURCEDRAMSZ_KVS (KVSOURCEDRAMSZ / VECTOR_SIZE)
#define LASTLEVELPARTITIONPADDING (NUM_PARTITIONS * 2)
#define KVBUFFERPADDING (NUM_PARTITIONS * 2)
#define KVSOURCEPADDING_KVS (((KVDRAMSZ_KVS / KVBUFFER_SIZE) * KVBUFFERPADDING) + (NUMLASTLEVELPARTITIONS * LASTLEVELPARTITIONPADDING))
#define PADDEDKVSOURCEDRAMSZ (KVSOURCEDRAMSZ + 4*KVSOURCEPADDING_KVS)
#define PADDEDKVSOURCEDRAMSZ_KVS (PADDEDKVSOURCEDRAMSZ / VECTOR_SIZE)
#define KVSTATSDRAMSZ (STATSDRAMSZ + MESSAGESDRAMSZ)

#define BASEOFFSET_KVDRAM 0 
#define BASEOFFSET_KVDRAM_KVS (BASEOFFSET_KVDRAM / VECTOR_SIZE)
#define BASEOFFSET_KVDRAMWORKSPACE (BASEOFFSET_KVDRAM + KVDRAMSZ)
#define BASEOFFSET_KVDRAMWORKSPACE_KVS (BASEOFFSET_KVDRAMWORKSPACE / VECTOR_SIZE)
#define BASEOFFSET_CAPSULES (BASEOFFSET_KVDRAMWORKSPACE + KVDRAMWORKSPACESZ)
#define BASEOFFSET_CAPSULES_KVS (BASEOFFSET_CAPSULES / VECTOR_SIZE)
#define BASEOFFSET_MESSAGESDRAM 0
#define BASEOFFSET_STATSDRAM MESSAGESDRAMSZ

////////////////

#define PROCESSEDGESCMD 1
#define PARTITIONCMD 2
#define APPLYUPDATESCMD 3

#define SRC_DST 5
#define DST_SRC 6

#define DIRECTEDGRAPH 5
#define UNDIRECTEDGRAPH 6

#define ON 5
#define OFF 6

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

#define NUM_KAPI 3 // number of kernel args per instance

////////////////

#define MESSAGES_SIZE 1024
#define MESSAGES_SIZE_KVS (MESSAGES_SIZE / VECTOR_SIZE)
#define MESSAGES_COMMANDID 0
#define MESSAGES_RUNKERNELCOMMANDID (MESSAGES_COMMANDID + 1)
#define MESSAGES_PROCESSCOMMANDID (MESSAGES_RUNKERNELCOMMANDID + 1)
#define MESSAGES_PARTITIONCOMMANDID (MESSAGES_PROCESSCOMMANDID + 1)
#define MESSAGES_APPLYUPDATESCOMMANDID (MESSAGES_PARTITIONCOMMANDID + 1)
#define MESSAGES_KVDATASIZEID (MESSAGES_APPLYUPDATESCOMMANDID + 1)
#define MESSAGES_FINALNUMPARTITIONSID (MESSAGES_KVDATASIZEID + 1)
#define MESSAGES_TREEDEPTHID (MESSAGES_FINALNUMPARTITIONSID + 1)
#define MESSAGES_ITERATIONID (MESSAGES_TREEDEPTHID + 1)
#define MESSAGES_SSDPARTITIONID (MESSAGES_ITERATIONID + 1)
#define MESSAGES_VOFFSET (MESSAGES_SSDPARTITIONID + 1)
#define MESSAGES_VSIZE (MESSAGES_VOFFSET + 1)
#define MESSAGES_TREEDEPTH (MESSAGES_VSIZE + 1)
#define MESSAGES_FINALNUMPARTITIONS (MESSAGES_TREEDEPTH + 1)
#define MESSAGES_KVDATA_BATCHSIZE_KVS (MESSAGES_FINALNUMPARTITIONS + 1)
#define MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID (MESSAGES_KVDATA_BATCHSIZE_KVS + 1)
#define MESSAGES_GRAPHITERATIONID (MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID + 1)
#define MESSAGES_CAPSULESTATSBATCHSZ_KVS (MESSAGES_GRAPHITERATIONID + 1)

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
typedef ap_uint<DATAWIDTH> uint512_dt;
#else
typedef struct {
	keyvalue_t data[VECTOR_SIZE];
} uint512_dt;
#endif

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
	keyvalue_t data[VECTOR_SIZE];
} uint512_vec_dt;

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

// global debugging signals
extern unsigned long globaldebugger_totalbytesreadfromfile; 
extern unsigned long globaldebugger_totalbyteswrittentofile; 
extern edge_t * globaldebugger_totalkvstats;
extern edge_t globaldebugger_totalkeyvaluesstransferredtokernel;
extern edge_t globaldebugger_totalkeyvaluesstransferredfromkernel;
#endif

