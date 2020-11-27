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

#ifdef FPGA_IMPL
typedef unsigned int batch_type;
typedef ap_uint<13> buffer_type;
typedef ap_uint<8> partition_type;
typedef ap_uint<8> vector_type;
typedef ap_uint<8> step_type;
typedef ap_uint<8> bool_type;
typedef unsigned int analysis_type;
#else 
typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int vector_type;
typedef unsigned int step_type;
typedef unsigned int bool_type;
typedef unsigned int analysis_type;
#endif

typedef struct {
	unsigned int key;
	unsigned int value;
} skeyvalue_t;

typedef struct {
	unsigned int currentLOP;
	unsigned int upperlimit;
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
	unsigned int info;
} travstate_t;

typedef struct {
	unsigned int enablecollectglobalstats;
	unsigned int enablepartition;
	unsigned int enablereduce;
	unsigned int enableprocessedges;
} config_t;

typedef struct {
	unsigned int command;
	unsigned int runkernelcommand;
	unsigned int processcommand;
	unsigned int collectstatscommand;
	unsigned int partitioncommand;
	unsigned int reducecommand;
	unsigned int finalnumpartitions;
	unsigned int treedepthid;
	unsigned int ssdpartitionid;
	unsigned int srcvoffset;
	unsigned int srcvsize;
	unsigned int srcvsize_kvs;
	unsigned int edgessize;
	unsigned int edgessize_kvs;
	unsigned int destvoffset;
	unsigned int actvvsize; //
	unsigned int firstvid;
	unsigned int firstkey;
	unsigned int firstvalue;
	unsigned int treedepth;
	unsigned int LLOPnumpartitions;
	unsigned int batchsize;
	unsigned int runsize;
	unsigned int runsize_kvs;
	unsigned int nextbatchoffset;
	unsigned int GraphIter;
	unsigned int GraphAlgo;
	unsigned int statsalreadycollected;
	unsigned int groupid;
	unsigned int beginLOP;
	unsigned int endLOP;
	unsigned int numLOPs;
	unsigned int batch_range;
	unsigned int batch_range_kvs;
	unsigned int batch_range_pow;
	unsigned int applyvertexbuffersz;
	unsigned int applyvertexbuffersz_kvs;

	unsigned int baseoffset_messagesdram_kvs;
	unsigned int baseoffset_kvdrambuffer_kvs;
	unsigned int baseoffset_kvdram_kvs;
	unsigned int baseoffset_kvdramworkspace_kvs;
	unsigned int baseoffset_statsdram_kvs;
	unsigned int baseoffset_activevertices_kvs;
	unsigned int baseoffset_edgesdata_kvs;
	unsigned int baseoffset_vertexptr_kvs;
	unsigned int baseoffset_verticesdata_kvs;
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

// buffer parameters
#define APPLYVERTEXBUFFERSZ (1 << (BATCH_RANGE_POW - (TREE_DEPTH * NUM_PARTITIONS_POW)))
#define APPLYVERTEXBUFFERSZ_KVS (APPLYVERTEXBUFFERSZ / VECTOR_SIZE) 

#define APPLYVERTEXBUFFERSZ2 (1 << (BATCH_RANGE2_POW - (TREE_DEPTH2 * NUM_PARTITIONS_POW)))
#define APPLYVERTEXBUFFERSZ2_KVS (APPLYVERTEXBUFFERSZ2 / VECTOR_SIZE)

// #define PADDEDDESTBUFFER_SIZE 512
// #define SRCBUFFER_SIZE (PADDEDDESTBUFFER_SIZE - ((4 * 3 * NUM_PARTITIONS) / VECTOR_SIZE))

#define PADDEDDESTBUFFER_SIZE 512
#ifdef ACTSFAST
#define SRCBUFFER_SIZE (512 - (16 * 4))
#else 
#define SRCBUFFER_SIZE (PADDEDDESTBUFFER_SIZE - ((4 * 3 * NUM_PARTITIONS) / VECTOR_SIZE))
#endif 

// #define CAPSULEBUFFER_SIZE 0 // not used
// #define NFACTOR 0 // not used
// #define PADDEDBUFFER_SIZE PADDEDDESTBUFFER_SIZE // not used

// global parameters
// dram sizes
#define MESSAGESDRAMSZ 1024

#define KVDRAMBUFFERSZ 0
#define KVDRAMBUFFERSZ_KVS (KVDRAMBUFFERSZ / VECTOR_SIZE)

#define KVDRAMPADDING ((MAXKVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE) * (NUM_PARTITIONS * VECTOR_SIZE * 2) * 2)

#define KVDRAMSZ (MAXKVDATA_BATCHSIZE + KVDRAMPADDING)
#define KVDRAMSZ_KVS (KVDRAMSZ / VECTOR_SIZE)

#define KVDRAMWORKSPACESZ KVDRAMSZ
#define KVDRAMWORKSPACESZ_KVS (KVDRAMWORKSPACESZ / VECTOR_SIZE)

#define KVSTATSDRAMSZ (NUMLASTLEVELPARTITIONS * (TREE_DEPTH + 1) * VECTOR_SIZE)
// #define KVSTATS_SIZE KVSTATSDRAMSZ // not used
#define KVSTATSDRAMSZ_KVS KVSTATSDRAMSZ

#ifdef INMEMORYGP // NOTE: POC (point-of-concern) // FIXME?
#define ACTIVEVERTICESSZ 0 // (KVDATA_RANGE / 2) // REMOVEME?
#else 
#define ACTIVEVERTICESSZ 0
#endif 
#define ACTIVEVERTICESSZ_KVS (ACTIVEVERTICESSZ / VECTOR_SIZE)

#ifdef INMEMORYGP
#define EDGESSZ MAXKVDATA_BATCHSIZE
#define VERTEXPTRSSZ KVDATA_RANGE 
#else 
#define EDGESSZ 0
#define VERTEXPTRSSZ 0
#endif
#define EDGESSZ_KVS (EDGESSZ / VECTOR_SIZE)
#define VERTEXPTRSSZ_KVS (VERTEXPTRSSZ / VECTOR_SIZE)

#define VERTICESDATASZ (BATCH_RANGE / 2)
#define VERTICESDATASZ_KVS (VERTICESDATASZ / VECTOR_SIZE)

#define KVSOURCEDRAMSZ (MESSAGESDRAMSZ + KVDRAMBUFFERSZ + KVDRAMSZ + KVDRAMWORKSPACESZ + KVSTATSDRAMSZ + ACTIVEVERTICESSZ + EDGESSZ + VERTEXPTRSSZ + VERTICESDATASZ)						
#define KVSOURCEDRAMSZ_KVS (KVSOURCEDRAMSZ / VECTOR_SIZE)
#define PADDEDKVSOURCEDRAMSZ KVSOURCEDRAMSZ
#define PADDEDKVSOURCEDRAMSZ_KVS (PADDEDKVSOURCEDRAMSZ / VECTOR_SIZE)

#define INPUTDATASZ PADDEDKVSOURCEDRAMSZ
#define INPUTDATASZ_KVS (INPUTDATASZ / VECTOR_SIZE)

// base addresses [messages, kvdram, kvdramworkspace, stats, verticesdata]
#define BASEOFFSET_MESSAGESDRAM 0 
#define BASEOFFSET_MESSAGESDRAM_KVS (BASEOFFSET_MESSAGESDRAM / VECTOR_SIZE) 

#define BASEOFFSET_KVDRAMBUFFER (BASEOFFSET_MESSAGESDRAM + MESSAGESDRAMSZ)
#define BASEOFFSET_KVDRAMBUFFER_KVS (BASEOFFSET_KVDRAMBUFFER / VECTOR_SIZE) 

#define BASEOFFSET_KVDRAM (BASEOFFSET_KVDRAMBUFFER + KVDRAMBUFFERSZ)
#define BASEOFFSET_KVDRAM_KVS (BASEOFFSET_KVDRAM / VECTOR_SIZE)

#define BASEOFFSET_KVDRAMWORKSPACE (BASEOFFSET_KVDRAM + KVDRAMSZ)
#define BASEOFFSET_KVDRAMWORKSPACE_KVS (BASEOFFSET_KVDRAMWORKSPACE / VECTOR_SIZE)

#define BASEOFFSET_STATSDRAM (BASEOFFSET_KVDRAMWORKSPACE + KVDRAMSZ) 
#define BASEOFFSET_STATSDRAM_KVS (BASEOFFSET_STATSDRAM / VECTOR_SIZE)

#define BASEOFFSET_ACTIVEVERTICES (BASEOFFSET_STATSDRAM + KVSTATSDRAMSZ) 
#define BASEOFFSET_ACTIVEVERTICES_KVS (BASEOFFSET_ACTIVEVERTICES / VECTOR_SIZE)

#define BASEOFFSET_EDGESDATA (BASEOFFSET_ACTIVEVERTICES + ACTIVEVERTICESSZ) 
#define BASEOFFSET_EDGESDATA_KVS (BASEOFFSET_EDGESDATA / VECTOR_SIZE)

#define BASEOFFSET_VERTEXPTR (BASEOFFSET_EDGESDATA + EDGESSZ) 
#define BASEOFFSET_VERTEXPTR_KVS (BASEOFFSET_VERTEXPTR / VECTOR_SIZE)

#define BASEOFFSET_VERTICESDATA (BASEOFFSET_VERTEXPTR + VERTEXPTRSSZ) 
#define BASEOFFSET_VERTICESDATA_KVS (BASEOFFSET_VERTICESDATA / VECTOR_SIZE)

// others
#define COLLECTGLOBALSTATSCOMMAND 2
#define COLLECTSTATSCOMMAND 3
#define PARTITIONKEYVALUECOMMAND 4
#define COLLECTSTATSANDPARTITIONCOMMAND 5
#define APPENDINVALIDSCOMMAND 6
#define NOCOMMAND 8

#define NOBUFFERADDRESS 999999999
#endif 







