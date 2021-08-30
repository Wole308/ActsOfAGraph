#ifndef COMMON_H
#define COMMON_H
#include "config_params.h"
#include <string.h>
#include <cmath>
#include <ap_int.h>

#define HW // SWEMU, HW, SW
#define ACTGRAPH_SETUP // ACTGRAPH_SETUP, GRAFBOOST_SETUP
#define BFS_ALGORITHM // PR_ALGORITHM, BFS_ALGORITHM, SSSP_ALGORITHM
#define _ORKUT_3M_106M 
#if (defined(SWEMU) || defined(HW))
#define FPGA_IMPL
#endif 
#define CRABTREE_PLATFORM // AWS_PLATFORM, CRABTREE_PLATFORM
// #define SW_IMPL //

#ifdef GRAFBOOST_SETUP
#define SORTREDUCEMODEL
#endif 

#define USEHBMMEMORY

#define LOCKE
#define _SINGLEKERNEL
#ifdef FPGA_IMPL
#define _WIDEWORD
#endif
#ifdef PR_ALGORITHM
#define ALLVERTEXISACTIVE_ALGORITHM
#else 
#define RANDOMVERTEXISACTIVE_ALGORITHM
#endif

#define ACTS
////////////////

#ifndef TESTKERNEL_IMPACTOFRANGE
#define LOADTEMPSPACETOFPGA
#endif 

#define GRAPHISUNDIRECTED

#ifdef FPGA_IMPL
#define ENABLE_KERNEL_PROFILING
#endif 

#define _DEBUGMODE_HEADER //
#if defined (FPGA_IMPL) && defined (HW)
#else
#define _DEBUGMODE_STATS // CRITICAL ADDME
// #define _DEBUGMODE_CHECKS
#define _DEBUGMODE_CHECKS2 // CRITICAL ADDME
#define _DEBUGMODE_CHECKS3 //
// #define _DEBUGMODE_PRINTS
// #define _DEBUGMODE_KERNELPRINTS
#define _DEBUGMODE_KERNELPRINTS2 //
#define _DEBUGMODE_KERNELPRINTS3 //
// #define _DEBUGMODE_RUNKERNELPRINTS //
// #define _DEBUGMODE_PROCACTVVSPRINTS //
#endif
// #define _DEBUGMODE_HOSTCHECKS
#define _DEBUGMODE_HOSTCHECKS2 //
// #define _DEBUGMODE_HOSTPRINTS
// #define _DEBUGMODE_HOSTPRINTS2 //
#define _DEBUGMODE_HOSTPRINTS3 //
// #define _DEBUGMODE_TIMERS
#define _DEBUGMODE_TIMERS2
#define _DEBUGMODE_TIMERS3

////////////////

#define PAGERANK 222
#define BFS 333
#define SSSP 444
#define BETWEENNESSCENTRALITY 555

////////////////

#define NUMSUBCPUTHREADS 32
#define NUMSYNCTHREADS 16
#define NUMUTILITYTHREADS 16 // NUMCPUTHREADS // FIXME?

////////////////

#define DATAWIDTH 512 
#define VECTOR_SIZE 8
#define VECTOR2_SIZE (VECTOR_SIZE * 2)
#define VECTOR1024_SIZE 16
#define DATATYPE_SIZE 32
#define NUMCOMPUTEUNITS 32
#define NUMCOMPUTEUNITS_HALF (NUMCOMPUTEUNITS / 2)
#define NUMINTSINKEYVALUETYPE 2
#define VDATA_PACKINGSIZE_POW 4 // AUTOMATEME.
#define VMASK_PACKINGSIZE_POW 4 // AUTOMATEME
// #define VDATA_PACKINGSIZE (VECTOR_SIZE * 2) //
// #define VMASK_PACKINGSIZE (VECTOR_SIZE * 2) //
#define VDATA_PACKINGSIZE (1 << VDATA_PACKINGSIZE_POW) //
#define VMASK_PACKINGSIZE (1 << VMASK_PACKINGSIZE_POW) //

#define NUMDRAMBANKS 4
#define NUMINSTANCES 1

#ifdef ENABLERECURSIVEPARTITIONING
	#ifdef SW_IMPL
	#define SRAMSZ_POW 18
	#else 
	#define SRAMSZ_POW 10 // 1024
	#endif 
#else
	#define SRAMSZ_POW 14 // 16384
#endif
#define SRAMSZ (1 << SRAMSZ_POW)

#ifdef ENABLERECURSIVEPARTITIONING
	#define NUM_PARTITIONS_POW 4
#else
	// #define NUM_PARTITIONS_POW 8 // FIXME. CRITICAL AUTOMATEME.
	#define NUM_PARTITIONS_POW (BATCH_RANGE_POW - SRAMSZ_POW) // FIXME. CRITICAL AUTOMATEME.
#endif 
#define NUM_PARTITIONS (1 << NUM_PARTITIONS_POW)

#define MAXNUMVERTEXBANKS 1
#define MAXNUMEDGEBANKS 1
#define MAXNUMVERTICESPERBANK (KVDATA_RANGE / MAXNUMEDGEBANKS)
#define MAXNUMVERTICESPERBANK_KVS (MAXNUMVERTICESPERBANK / VECTOR_SIZE)

#define KVDATA_RANGE_POW 22
#define KVDATA_RANGE (1 << KVDATA_RANGE_POW)

#define NUMWORKERS 1
#define NUMSUBWORKERS 1

#define FETFACTOR (VDATA_PACKINGSIZE / VECTOR_SIZE) // how many stacks of VDATA_PACKINGSIZE vertices are read for every last-level partition

////////////////

#define BATCH_RANGE_POW KVDATA_RANGE_POW
#define BATCH_RANGE (1 << BATCH_RANGE_POW)
#define BATCH_RANGE_KVS (BATCH_RANGE / VECTOR_SIZE)

#define BATCHRANGESZ (BATCH_RANGE / 2)
#define BATCHRANGESZ_KVS (BATCHRANGESZ / VECTOR_SIZE)

/* #ifdef ENABLERECURSIVEPARTITIONING
	#ifdef SW_IMPL
	#define SRAMSZ_POW 18 // 14,18
	#else 
	#define SRAMSZ_POW 10
	#endif 
#else
	#define SRAMSZ_POW 14 // 16384
#endif
#define SRAMSZ (1 << SRAMSZ_POW) */

// tree-depth:sramsz constraint
/** ACTS constraints equation! (NUM_PARTITIONS_POWx = KVDATA_RANGE_POW - SRAMSZ_POW) */
#ifdef ENABLERECURSIVEPARTITIONING
#define TREE_DEPTH (((BATCH_RANGE_POW - SRAMSZ_POW) + (NUM_PARTITIONS_POW - 1)) / NUM_PARTITIONS_POW)
#else 
#define TREE_DEPTH 1
#endif 

#ifdef ENABLERECURSIVEPARTITIONING
	#define REDUCESZ_POW (BATCH_RANGE_POW - (TREE_DEPTH * NUM_PARTITIONS_POW))
#else
	// #define REDUCESZ_POW ((BATCH_RANGE_POW - (TREE_DEPTH * NUM_PARTITIONS_POW)) - VDATA_PACKINGSIZE_POW) // NOTE: this is because for non-recursive, there is no parallelism in reduce				
	#define REDUCESZ_POW (SRAMSZ_POW - VDATA_PACKINGSIZE_POW) // NOTE: this is because for non-recursive, there is no parallelism in reduce
#endif
#define REDUCESZ (1 << REDUCESZ_POW) // 1024
#define REDUCEBUFFERSZ (REDUCESZ / 2) // 512
#define MAXREDUCEBUFFERSZ DOUBLE_BLOCKRAM_SIZE // AUTOMATEME.

// #define VMASKBUFFERSZ_KVS ((REDUCESZ * NUM_PARTITIONS) / 512) // 32 //>>> 16384=[WidthxHeight*]=512*32
#define VMASKBUFFERSZ_KVS ((REDUCESZ * VDATA_PACKINGSIZE) / 512) // 32 //>>> 16384=[WidthxHeight*]=512*32

#define NUMLASTLEVELPARTITIONS (1 << (NUM_PARTITIONS_POW * TREE_DEPTH))
#define TOTALNUMPARTITIONS 273 // CRITICAL FIXME.

////////////////

#define MODEL_BATCHSIZE 10000000
#define MODEL_BATCHSIZE_KVS (MODEL_BATCHSIZE / VECTOR_SIZE)

////////////////

#define MESSAGES_SIZE 1024
#define MESSAGES_SIZE_KVS (MESSAGES_SIZE / VECTOR_SIZE)

#define MESSAGES_ENABLE_RUNKERNELCOMMAND 0
#define MESSAGES_ENABLE_PROCESSCOMMAND (MESSAGES_ENABLE_RUNKERNELCOMMAND + 1)
#define MESSAGES_ENABLE_PARTITIONCOMMAND (MESSAGES_ENABLE_PROCESSCOMMAND + 1)
#define MESSAGES_ENABLE_APPLYUPDATESCOMMAND (MESSAGES_ENABLE_PARTITIONCOMMAND + 1)
#define MESSAGES_ENABLE_SAVEVMASK (MESSAGES_ENABLE_APPLYUPDATESCOMMAND + 1)
#define MESSAGES_ENABLE_SAVEVMASKP (MESSAGES_ENABLE_SAVEVMASK + 1)

#define MESSAGES_BASEOFFSETKVS_MESSAGESDATA (MESSAGES_ENABLE_SAVEVMASKP + 1)
#define MESSAGES_BASEOFFSETKVS_EDGESDATA (MESSAGES_BASEOFFSETKVS_MESSAGESDATA + 1)
#define MESSAGES_BASEOFFSETKVS_VERTEXPTR (MESSAGES_BASEOFFSETKVS_EDGESDATA + 1)
#define MESSAGES_BASEOFFSETKVS_VERTICESDATA (MESSAGES_BASEOFFSETKVS_VERTEXPTR + 1)
#define MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES (MESSAGES_BASEOFFSETKVS_VERTICESDATA + 1)
#define MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK (MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES + 1)
#define MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK (MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK + 1)
#define MESSAGES_BASEOFFSETKVS_STATSDRAM (MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK + 1)
#define MESSAGES_BASEOFFSETKVS_KVDRAM (MESSAGES_BASEOFFSETKVS_STATSDRAM + 1)
#define MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE (MESSAGES_BASEOFFSETKVS_KVDRAM + 1)

#define MESSAGES_SIZE_MESSAGESDATA (MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE + 1)
#define MESSAGES_SIZE_EDGES (MESSAGES_SIZE_MESSAGESDATA + 1)
#define MESSAGES_SIZE_VERTEXPTRS (MESSAGES_SIZE_EDGES + 1)
#define MESSAGES_SIZE_VERTICESDATA (MESSAGES_SIZE_VERTEXPTRS + 1)
#define MESSAGES_SIZE_ACTIVEVERTICES (MESSAGES_SIZE_VERTICESDATA + 1)
#define MESSAGES_SIZE_VERTICESDATAMASK (MESSAGES_SIZE_ACTIVEVERTICES + 1)
#define MESSAGES_SIZE_VERTICESPARTITIONMASK (MESSAGES_SIZE_VERTICESDATAMASK + 1)
#define MESSAGES_SIZE_KVSTATSDRAM (MESSAGES_SIZE_VERTICESPARTITIONMASK + 1)
#define MESSAGES_SIZE_KVDRAM (MESSAGES_SIZE_KVSTATSDRAM + 1)
#define MESSAGES_SIZE_KVDRAMWORKSPACE (MESSAGES_SIZE_KVDRAM + 1)
#define MESSAGES_SIZE_REDUCE (MESSAGES_SIZE_KVDRAMWORKSPACE + 1)
#define MESSAGES_SIZE_BATCHRANGE (MESSAGES_SIZE_REDUCE + 1)
#define MESSAGES_SIZE_RUN (MESSAGES_SIZE_BATCHRANGE + 1)

#define MESSAGES_POW_MESSAGESDRAM (MESSAGES_SIZE_RUN + 1)
#define MESSAGES_POW_EDGES (MESSAGES_POW_MESSAGESDRAM + 1)
#define MESSAGES_POW_VERTEXPTRS (MESSAGES_POW_EDGES + 1)
#define MESSAGES_POW_VERTICESDATA (MESSAGES_POW_VERTEXPTRS + 1)
#define MESSAGES_POW_ACTIVEVERTICES (MESSAGES_POW_VERTICESDATA + 1)
#define MESSAGES_POW_VERTICESDATAMASK (MESSAGES_POW_ACTIVEVERTICES + 1)
#define MESSAGES_POW_KVSTATSDRAM (MESSAGES_POW_VERTICESDATAMASK + 1)
#define MESSAGES_POW_KVDRAM (MESSAGES_POW_KVSTATSDRAM + 1)
#define MESSAGES_POW_KVDRAMWORKSPACE (MESSAGES_POW_KVDRAM + 1)
#define MESSAGES_POW_REDUCE (MESSAGES_POW_KVDRAMWORKSPACE + 1)
#define MESSAGES_POW_BATCHRANGE (MESSAGES_POW_REDUCE + 1)

#define MESSAGES_ALGORITHMINFO_GRAPHITERATIONID (MESSAGES_POW_BATCHRANGE + 1)
#define MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID (MESSAGES_ALGORITHMINFO_GRAPHITERATIONID + 1)

#define MESSAGES_ACTSPARAMS_BEGINLOP (MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID + 1)
#define MESSAGES_ACTSPARAMS_NUMLOPS (MESSAGES_ACTSPARAMS_BEGINLOP + 1)
#define MESSAGES_ACTSPARAMS_TREEDEPTH (MESSAGES_ACTSPARAMS_NUMLOPS + 1)
#define MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS (MESSAGES_ACTSPARAMS_TREEDEPTH + 1)
#define MESSAGES_ACTSPARAMS_SRCVOFFSET (MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS + 1)
#define MESSAGES_ACTSPARAMS_SRCVSIZE (MESSAGES_ACTSPARAMS_SRCVOFFSET + 1)
#define MESSAGES_ACTSPARAMS_DESTVOFFSET (MESSAGES_ACTSPARAMS_SRCVSIZE + 1)

#define MESSAGES_RETURN_RETURNVALUES (MESSAGES_ACTSPARAMS_DESTVOFFSET + 1)

////////////////

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

#define BitVal(data,y) ( (data>>y) & 1)      /** Return Data.Y value   **/
#define SetBit(data,y)    data |= (1 << y)    /** Set Data.Y   to 1    **/
#define ClearBit(data,y)  data &= ~(1 << y)   /** Clear Data.Y to 0    **/
#define TogleBit(data,y)     (data ^=BitVal(y))     /** Togle Data.Y  value  **/
#define Togle(data)   (data =~data )         /** Togle Data value     **/

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
// #define INFINITI (0xFFFFFFFF) // FIXME. is this better?

#define NAp 666
#define INVALIDDATA 0x0FFFFFFF 
#define UNUSEDDATA 987654321

#define ONE 1
#define TWO 2

#define SNAP 1
#define SYNTHETIC 2

typedef unsigned int vertex_t;
#if (defined(_LARGEDATASET_1B) || defined(_LARGEDATASET_4B))
typedef unsigned long edge_t;
#else 
typedef unsigned int edge_t;
#endif 

typedef unsigned int keyy_t;
typedef unsigned int value_t;
typedef unsigned int value2_dt;

typedef struct {
	unsigned int key;
	unsigned int value;
} keyvalue_t;

typedef struct {
	#ifdef _WIDEWORD
	ap_uint<1> key;
	ap_uint<1> value;
	#else 
	unsigned int key;
	unsigned int value;
	#endif 
} keyvalue1_type;

typedef keyvalue_t keyvalue16_type;

typedef struct {
	unsigned int key;
	unsigned int value;
} keyvalue_vec_bittype;

// typedef struct {
	// keyvalue1_type data[NUM_PARTITIONS];
// } unitBRAMwidth_type; 
typedef struct {
	keyvalue1_type data[16];
} unitBRAMwidth_type; 

#ifdef _WIDEWORD
typedef ap_uint<64> uint64_type;
#else
typedef keyvalue_t uint64_type;
#endif

#ifdef _WIDEWORD
typedef ap_uint<32> uint32_type;
#else
typedef unsigned int uint32_type;
#endif

typedef struct {
	// vertex_t vid;
	vertex_t outdegree;
} vertexprop_t;

typedef struct {
	keyy_t dstvid; 
} edge_type;

typedef struct {
	keyy_t srcvid;
	keyy_t dstvid;
} edge2_type;

typedef struct {
	keyy_t key;
} vptr_type;

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
	keyvalue_t data[VECTOR_SIZE];
} uint512_vec_dt;

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

#ifdef _WIDEWORD
typedef ap_uint<64> ulong_dt;
#else
typedef unsigned long ulong_dt;
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

