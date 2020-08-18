#ifndef COMMON_H
#define COMMON_H
#include "../config/config_params.h"

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
// #define _DEBUGMODE_KERNELPRINTS2 //
#define _DEBUGMODE_KERNELPRINTS3 //
#endif
#if defined(SW) & defined(TESTKERNEL)
#define _DEBUGMODE_KERNELPRINTS2 //
#endif
// #define _DEBUGMODE_HOSTPRINTS
#define _DEBUGMODE_HOSTPRINTS2 //
// #define _DEBUGMODE_TIMERS
#define _DEBUGMODE_TIMERS2

////////////////

#define DATAWIDTH 512 
#define VECTOR_SIZE 8

#define NUMDRAMBANKS 4
#define NUMINSTANCES 1

#define NUM_PARTITIONS_POW 4
#define NUM_PARTITIONS (1 << NUM_PARTITIONS_POW)

#define MAXNUMSSDPARTITIONS 4
#define NUMSSDPARTITIONS_POW 2
#define NUMSSDPARTITIONS (1 << NUMSSDPARTITIONS_POW)

#define MAXNUMVERTEXBANKS 4
#define MAXNUMEDGEBANKS 4
#define MAXNUMVERTICESPERBANK (KVDATA_RANGE / MAXNUMEDGEBANKS)
#define MAXNUMVERTICESPERBANK_KVS (MAXNUMVERTICESPERBANK / VECTOR_SIZE)

#define NUMCPUTHREADSY 1
#define NUMCPUTHREADS 4 // FIXME. overridden
// #define NUMCPUTHREADS 8

#define KVDATA_RANGE_POW 26
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

/** ACTS constraints equation! (NUM_PARTITIONS_POWx = KVDATA_RANGE_PERSSDPARTITION_POW - SRAMSZ_POW) */
#define SRAMSZ_POW 14
#define SRAMSZ (1 << SRAMSZ_POW)
#define APPROXTREE_DEPTH ((KVDATA_RANGE_PERSSDPARTITION_POW - SRAMSZ_POW + NUM_PARTITIONS_POW - 1) / NUM_PARTITIONS_POW)
#define TREE_DEPTH APPROXTREE_DEPTH

////////////////

#ifdef TESTKERNEL
#define KVDATA_BATCHSIZE (1 << 26)
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

#define NFACTOR 4
#define PADDEDDESTBUFFER_SIZE ((1 << 14) / VECTOR_SIZE) // NEWCHANGE. HARDWARECOMPLAINT. using > 2048 drastically dropped frequency 
#define SRCBUFFER_SIZE ((PADDEDDESTBUFFER_SIZE - NUM_PARTITIONS) / NFACTOR)

#define PADDEDBUFFER_SIZE PADDEDDESTBUFFER_SIZE
#define CAPSULEBUFFER_SIZE 512

#define APPLYVERTEXBUFFERSZ (1 << (KVDATA_RANGE_PERSSDPARTITION_POW - (TREE_DEPTH * NUM_PARTITIONS_POW)))
#define APPLYVERTEXBUFFERSZ_KVS (APPLYVERTEXBUFFERSZ / VECTOR_SIZE)

////////////////

#define NUMLASTLEVELPARTITIONS (1 << (NUM_PARTITIONS_POW * TREE_DEPTH))
#define KVSTATS_SIZE (VECTOR_SIZE + (NUMLASTLEVELPARTITIONS * TREE_DEPTH)) // NEWCHANGE.
#define KVDRAMPADDING (((DRAMBATCHFACTOR * KVDATA_BATCHSIZE_KVS) / (SRCBUFFER_SIZE * NFACTOR)) * (NUM_PARTITIONS * VECTOR_SIZE)) * 16	// FIXME. NEWCHANGE. FIXMEFORREALWORKLOADS. '16' is extra space 

// #define KVDRAMBUFFERSZ KVDATA_BATCHSIZE
#define KVDRAMBUFFERSZ 0
#define KVDRAMBUFFERSZ_KVS (KVDRAMBUFFERSZ / VECTOR_SIZE)
#define KVDRAMSZ ((DRAMBATCHFACTOR * KVDATA_BATCHSIZE) + KVDRAMPADDING)
#define KVDRAMSZ_KVS (KVDRAMSZ / VECTOR_SIZE)
#define KVDRAMWORKSPACESZ KVDRAMSZ
#define KVDRAMWORKSPACESZ_KVS (KVDRAMWORKSPACESZ / VECTOR_SIZE)
#define CAPSULESZ_KVS (((KVDRAMSZ_KVS / CAPSULEBUFFER_SIZE) * TREE_DEPTH) * 4) // FIXME. '4' is extra space.
#define CAPSULESZ (CAPSULESZ_KVS * VECTOR_SIZE)

#define MESSAGESDRAMSZ 1024
#define STATSDRAMSZ KVSTATS_SIZE
#define CAPSULEMETADATADRAMSZ (NUMLASTLEVELPARTITIONS * TREE_DEPTH)

#define BASEOFFSET_KVDRAMBUFFER 0 
#define BASEOFFSET_KVDRAMBUFFER_KVS (BASEOFFSET_KVDRAMBUFFER / VECTOR_SIZE) 
#define BASEOFFSET_KVDRAM (BASEOFFSET_KVDRAMBUFFER + KVDRAMBUFFERSZ)
#define BASEOFFSET_KVDRAM_KVS (BASEOFFSET_KVDRAM / VECTOR_SIZE)
#define BASEOFFSET_KVDRAMWORKSPACE (BASEOFFSET_KVDRAM + KVDRAMSZ)
#define BASEOFFSET_KVDRAMWORKSPACE_KVS (BASEOFFSET_KVDRAMWORKSPACE / VECTOR_SIZE)
#define BASEOFFSET_CAPSULES (BASEOFFSET_KVDRAMWORKSPACE + KVDRAMWORKSPACESZ)
#define BASEOFFSET_CAPSULES_KVS (BASEOFFSET_CAPSULES / VECTOR_SIZE)

#define BASEOFFSET_MESSAGESDRAM 0
#define BASEOFFSET_STATSDRAM (BASEOFFSET_MESSAGESDRAM + MESSAGESDRAMSZ)
#define BASEOFFSET_CAPSULEMETADATA (BASEOFFSET_STATSDRAM + CAPSULEMETADATADRAMSZ)

#define KVSOURCEDRAMSZ (KVDRAMBUFFERSZ + KVDRAMSZ + KVDRAMWORKSPACESZ + CAPSULESZ)
#define KVSOURCEDRAMSZ_KVS (KVSOURCEDRAMSZ / VECTOR_SIZE)
#define KVSTATSDRAMSZ (MESSAGESDRAMSZ + STATSDRAMSZ + CAPSULEMETADATADRAMSZ)
#define LASTLEVELPARTITIONPADDING (NUM_PARTITIONS * 2)
#define KVBUFFERPADDING (NUM_PARTITIONS * 2)
#define KVSOURCEPADDING_KVS 0
#define PADDEDKVSOURCEDRAMSZ (KVSOURCEDRAMSZ + 4*KVSOURCEPADDING_KVS)
#define PADDEDKVSOURCEDRAMSZ_KVS (PADDEDKVSOURCEDRAMSZ / VECTOR_SIZE)

////////////////

#ifdef FPGA_IMPL
#define NUMFLAGS 2
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
#define MESSAGES_STATSALREADYCOLLECTED (MESSAGES_GRAPHITERATIONID + 1)

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

typedef struct {
	unsigned long totalsize;
    long double totaltime_SSDtransfers_ms;
	long double totaltime_OCLtransfers_ms;
	long double totaltime_ms;
	long double totaltime_overallexcludingOCLandSSDtransfers_ms;
	unsigned int graph_iterationidx; 
} runsummary_t;

// global debugging signals
extern unsigned long globaldebugger_totalbytesreadfromfile; 
extern unsigned long globaldebugger_totalbyteswrittentofile; 
extern edge_t * globaldebugger_totalkvstats;
extern edge_t globaldebugger_totalkeyvaluesstransferredtokernel;
extern edge_t globaldebugger_totalkeyvaluesstransferredfromkernel;
#endif

