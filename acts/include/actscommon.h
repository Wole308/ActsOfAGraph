#ifndef ACTSCOMMON_H
#define ACTSCOMMON_H
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#if (defined(HW) || defined(SWEMU))
#include <string.h>
#include <cmath>
#include <ap_int.h>
#endif
#include "../../include/config_params.h"
#include "../../include/common.h"
using namespace std;

// #define _DEBUGMODE_KERNELPRINTS_TRACE
#define _DEBUGMODE_KERNELPRINTS_TRACE3 //
// #define _DEBUGMODE_SUBPMASKING_TRACE

#define PROCESSMODULE
#define PARTITIONMODULE
#define REDUCEMODULE //
#define SYNCHRONIZEMODULE //

#define PROCESSMODE 0
#define PARTITIONMODE 1
#define REDUCEMODE 2

// buffer parameters
#define BLOCKRAM_SIZE 512
#define DOUBLE_BLOCKRAM_SIZE (BLOCKRAM_SIZE * 2)

#ifdef ACTS_PARTITION_AND_REDUCE_STRETEGY
#define SRCBUFFER_SIZE (BLOCKRAM_SIZE - (4 * 4))
#else 
#define SRCBUFFER_SIZE (DOUBLE_BLOCKRAM_SIZE - (4 * 4))
#endif 
// #define SRCBUFFER_SIZE (512 - (4 * 4))

#ifdef ACTS_PARTITION_AND_REDUCE_STRETEGY // NEWCHANGE.
#define WORKBUFFER_SIZE (SRCBUFFER_SIZE - (NUM_PARTITIONS*4))
#else 
#define WORKBUFFER_SIZE (SRCBUFFER_SIZE - (NUM_PARTITIONS*1))
#endif 

#ifdef ACTS_PARTITION_AND_REDUCE_STRETEGY
#define SOURCEBLOCKRAM_SIZE BLOCKRAM_SIZE // NEWCHANGE.
#else 
#define SOURCEBLOCKRAM_SIZE DOUBLE_BLOCKRAM_SIZE // NEWCHANGE.
#endif 
#define DESTBLOCKRAM_SIZE SOURCEBLOCKRAM_SIZE

#define DRAMPADD (16 * BLOCKRAM_SIZE * VECTOR_SIZE) // NEWCHANGE
#define DRAMPADD_KVS (DRAMPADD / VECTOR_SIZE) // to avoid any spill-overs 

// global parameters
// messages area
#define MESSAGESDRAMSZ 1024

// edges workload area
#define VERTEXPTRSSZ ((KVDATA_RANGE / NUMINTSINKEYVALUETYPE) + DRAMPADD)
#define VERTEXPTRSSZ_KVS (VERTEXPTRSSZ / VECTOR_SIZE)

// vertices workload area
#define VERTICESDATASZ (BATCH_RANGE / NUMINTSINKEYVALUETYPE)
#define VERTICESDATASZ_KVS (VERTICESDATASZ / VECTOR_SIZE)

// active vertices workload area
#define ACTIVEVERTICESSZ 0
#define ACTIVEVERTICESSZ_KVS (ACTIVEVERTICESSZ / VECTOR_SIZE)

#define VERTICESDATAMASKSZ (BATCH_RANGE / 32)
#define VERTICESDATAMASKSZ_KVS (VERTICESDATAMASKSZ / VECTOR_SIZE)

#define KVSTATSSZ (NUMLASTLEVELPARTITIONS * 2)
#define KVSTATSDRAMSZ (KVSTATSSZ * VECTOR_SIZE)
#define KVSTATSDRAMSZ_KVS KVSTATSDRAMSZ

#ifdef USEHBMMEMORY
#define KVSOURCEDRAMSZ ((1 << 28) / 8) // max HBM capacity (256MB)
#endif 
#ifdef USEDDRAMMEMORY 
// #define KVSOURCEDRAMSZ (1 << (31-3)) // ((1 << 31) / 8) // max HBM capacity (8GB)
// #define KVSOURCEDRAMSZ (1 << (30-3)) // max HBM capacity (8GB)
// #define KVSOURCEDRAMSZ 100377960 /////////////////////////////////////////////// FORMERONE

// #define KVSOURCEDRAMSZ (1 << (31-3)) // ((1 << 31) / 8)
#if NUMSUBCPUTHREADS==1
#define KVSOURCEDRAMSZ (1 << (32-3)) // ((1 << 31) / 8)
#else 
#define KVSOURCEDRAMSZ (1 << (31-3)) // ((1 << 31) / 8)
#endif

#endif 
#define KVSOURCEDRAMSZ_KVS (KVSOURCEDRAMSZ / VECTOR_SIZE)
#define PADDEDKVSOURCEDRAMSZ KVSOURCEDRAMSZ
#define PADDEDKVSOURCEDRAMSZ_KVS (PADDEDKVSOURCEDRAMSZ / VECTOR_SIZE)

#define VDRAMSZ KVSOURCEDRAMSZ
#define PADDEDVDRAMSZ VDRAMSZ
#define PADDEDVDRAMSZ_KVS (PADDEDVDRAMSZ / VECTOR_SIZE)

// base addresses
#define BASEOFFSET_MESSAGESDATA 0 
#define BASEOFFSET_MESSAGESDATA_KVS (BASEOFFSET_MESSAGESDATA / VECTOR_SIZE) 

// others
#define COLLECTGLOBALSTATSCOMMAND 2
#define COLLECTSTATSCOMMAND 3
#define PARTITIONKEYVALUECOMMAND 4
#define COLLECTSTATSANDPARTITIONCOMMAND 5
#define APPENDINVALIDSCOMMAND 6
#define NOCOMMAND 8

#define NOBUFFERADDRESS 999999999

#define SIZEOF_KEY 22
#define SIZEOF_VALUE 10
#define SIZEOF_VDATAKEY 16
#define SIZEOF_VDATAVALUE 16

#ifdef FPGA_IMPL
typedef unsigned int batch_type;
typedef ap_uint<13> buffer_type;
// typedef ap_uint<8> partition_type; // FIXME. some bug somewhere
typedef unsigned int partition_type; //
typedef ap_uint<8> vector_type;
typedef ap_uint<8> step_type;
typedef ap_uint<8> bool_type;
typedef unsigned int analysis_type;
typedef ap_uint<2> visitstate_type;
#else 
typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int vector_type;
typedef unsigned int step_type;
typedef unsigned int bool_type;
typedef unsigned int analysis_type;
typedef unsigned int visitstate_type;
#endif

typedef struct {
	#ifdef _WIDEWORD
	ap_uint<16> key; // CRITICAL REMOVEME.
	ap_uint<16> value;
	#else 
	unsigned int key;
	unsigned int value;
	#endif 
} keyvalue_capsule_t;

#ifdef _WIDEWORD
typedef ap_uint<32> keyvalue_buffer_t;
#else
typedef struct {
	unsigned int key;
	unsigned int value;
} keyvalue_buffer_t;
#endif 

#ifdef _WIDEWORD
typedef ap_uint<32> keyvalue_vbuffer_t;
#else
typedef struct {
	unsigned int key;
	unsigned int value;
} keyvalue_vbuffer_t;
#endif 

#ifdef _WIDEWORD
typedef ap_uint<32> vtxbuffer_type;
#else
typedef unsigned int vtxbuffer_type;
#endif 

typedef struct {
	unsigned int currentLOP;
	unsigned int upperlimit;
	unsigned int upperpartition;
	unsigned int source_partition;
	unsigned int worksourcebaseaddress_kvs;
	unsigned int workdestbaseaddress_kvs;
} sweepparams_t;

typedef struct {
	unsigned int i;
	unsigned int i_kvs;
	unsigned int begin;
	unsigned int begin_kvs;
	unsigned int end;
	unsigned int end_kvs;
	unsigned int size;
	unsigned int size_kvs;
	unsigned int skip;
	unsigned int skip_kvs;
	unsigned int i2;
	unsigned int i2_kvs;
	unsigned int info;
} travstate_t;

typedef struct {
	unsigned int i;
	unsigned int begin;
	unsigned int size;	
} state_t;

typedef struct {
	unsigned int enablecollectglobalstats;
	unsigned int enablepartition;
	unsigned int enablereduce;
	unsigned int enableprocessedges;
} config_t;

typedef struct {
	unsigned int ENABLE_RUNKERNELCOMMAND;
	unsigned int ENABLE_PROCESSCOMMAND;
	unsigned int ENABLE_PARTITIONCOMMAND;
	unsigned int ENABLE_APPLYUPDATESCOMMAND;
	unsigned int ENABLE_SAVEVMASK;
	unsigned int ENABLE_SAVEVMASKP;

	unsigned int BASEOFFSETKVS_MESSAGESDATA;
	unsigned int BASEOFFSETKVS_EDGESDATA;
	unsigned int BASEOFFSETKVS_VERTEXPTR;
	unsigned int BASEOFFSETKVS_VERTICESDATA;
	unsigned int BASEOFFSETKVS_ACTIVEVERTICES;
	unsigned int BASEOFFSETKVS_VERTICESDATAMASK;
	unsigned int BASEOFFSETKVS_VERTICESPARTITIONMASK;
	unsigned int BASEOFFSETKVS_STATSDRAM;
	unsigned int BASEOFFSETKVS_KVDRAM;
	unsigned int BASEOFFSETKVS_KVDRAMWORKSPACE;

	unsigned int SIZE_MESSAGESDRAM;
	unsigned int SIZE_EDGES;
	unsigned int SIZE_VERTEXPTRS;
	unsigned int SIZE_VERTICESDATA;
	unsigned int SIZE_ACTIVEVERTICES;
	unsigned int SIZE_VERTICESDATAMASK;
	unsigned int SIZE_VERTICESPARTITIONMASK;
	unsigned int SIZE_KVSTATSDRAM;
	unsigned int SIZE_KVDRAM;
	unsigned int SIZE_KVDRAMWORKSPACE;
	unsigned int SIZE_REDUCE;
	unsigned int SIZE_BATCHRANGE;
	unsigned int SIZE_RUN;

	unsigned int POW_MESSAGESDRAM;
	unsigned int POW_EDGES;
	unsigned int POW_VERTEXPTRS;
	unsigned int POW_VERTICESDATA;
	unsigned int POW_ACTIVEVERTICES;
	unsigned int POW_VERTICESDATAMASK;
	unsigned int POW_KVSTATSDRAM;
	unsigned int POW_KVDRAM;
	unsigned int POW_KVDRAMWORKSPACE;
	unsigned int POW_REDUCE;
	unsigned int POW_BATCHRANGE;

	unsigned int ALGORITHMINFO_GRAPHITERATIONID;
	unsigned int ALGORITHMINFO_GRAPHALGORITHMID;

	unsigned int ACTSPARAMS_BEGINLOP;
	unsigned int ACTSPARAMS_NUMLOPS;
	unsigned int ACTSPARAMS_TREEDEPTH;
	unsigned int ACTSPARAMS_FINALNUMPARTITIONS;
	unsigned int ACTSPARAMS_SRCVOFFSET;
	unsigned int ACTSPARAMS_SRCVSIZE;
	unsigned int ACTSPARAMS_DESTVOFFSET;

	unsigned int RETURN_RETURNVALUES;
} globalparams_t;

typedef struct {
	unsigned int baseaddr_worksourcekvs_kvs;
	unsigned int baseaddr_workdestkvs_kvs;
	unsigned int baseaddr_capsule_kvs;
	unsigned int baseaddr_stats;
	unsigned int num_source_partitions;
	unsigned int nextsourceoffset_partition;
	unsigned int nextdestoffset_partition;
	unsigned int nextsourceoffset_kv;
	unsigned int nextdestoffset_kv;
	unsigned int sourceskipsize_partition;
	unsigned int destskipsize_partition;
	unsigned int sourceskipsize_kv;
	unsigned int destskipsize_kv;
	unsigned int destpartitionrange;
	unsigned int currentLOP;
	unsigned int upperlimit;
} clopparams_t;

typedef struct {
	unsigned int begin_kvs;
	unsigned int size_kvs;
	unsigned int step_kvs;
	unsigned int end_kvs;
} partitionparams_t;

typedef struct {
	unsigned int globaloffset;
	unsigned int localoffset;
} offset_t;

typedef struct {
	int nextoffset_kvs;
	int chunksize_kvs;
} fetchmessage_t;
#endif 







