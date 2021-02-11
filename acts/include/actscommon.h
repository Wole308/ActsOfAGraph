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
	unsigned int key;
	unsigned int value;
} skeyvalue_t;

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
	unsigned int applyvertexbuffersz_pow;
	unsigned int loadfactorforreduce;
	unsigned int baseoffset_messagesdram_kvs;
	unsigned int baseoffset_kvdram_kvs;
	unsigned int baseoffset_kvdramworkspace_kvs;
	unsigned int baseoffset_statsdram_kvs;
	unsigned int baseoffset_edgesdata_kvs;
	unsigned int baseoffset_vertexptr_kvs;
	unsigned int baseoffset_verticesdata_kvs;
	unsigned int baseoffset_activevertices_kvs;
	unsigned int baseoffset_verticesdatamask_kvs;
	unsigned int kvstatssz;
	unsigned int baseoffset_returnvalues;
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

#ifdef EMBEDDEDCOLLECTSTATS
#define GLOBALSTATSBUFFERSZ PADDEDDESTBUFFER_SIZE
#else 
#define GLOBALSTATSBUFFERSZ NUM_PARTITIONS	
#endif 

// buffer parameters
#define PADDEDDESTBUFFER_SIZE 512
#define SRCBUFFER_SIZE (512 - (4 * 4))

#define NUM_KVDRAMBUFFERS 24
#define BUFFERBASE_VDATA 0
#define BUFFERBASE_VMASK 16

// global parameters
// messages area
#define MESSAGESDRAMSZ 1024

// workspace area
#define KVDRAMPADDING ((MAXKVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE) * (NUM_PARTITIONS * VECTOR_SIZE * 2) * 2)
#define KVDRAMSZ (MAXKVDATA_BATCHSIZE + KVDRAMPADDING) 
#define KVDRAMSZ_KVS (KVDRAMSZ / VECTOR_SIZE)

#define KVDRAMWORKSPACESZ KVDRAMSZ 
#define KVDRAMWORKSPACESZ_KVS (KVDRAMWORKSPACESZ / VECTOR_SIZE)

#define KVSTATSSZ (NUMLASTLEVELPARTITIONS * (TREE_DEPTH + 1))
#define KVSTATSDRAMSZ (NUMLASTLEVELPARTITIONS * (TREE_DEPTH + 1) * VECTOR_SIZE)
#define KVSTATSDRAMSZ_KVS KVSTATSDRAMSZ

// edges workload area
#define EDGESSZ MAXKVDATA_BATCHSIZE
#define EDGESSZ_KVS (EDGESSZ / VECTOR_SIZE)

#define VERTEXPTRSSZ KVDATA_RANGE 
#define VERTEXPTRSSZ_KVS (VERTEXPTRSSZ / VECTOR_SIZE)

// vertices workload area
#define VERTICESDATASZ (BATCH_RANGE / 2)
#define VERTICESDATASZ_KVS (VERTICESDATASZ / VECTOR_SIZE)

// active vertices workload area
#ifdef ALLVERTEXISACTIVE_ALGORITHM
#define ACTIVEVERTICESSZ 0
#else 
#define ACTIVEVERTICESSZ KVDATA_RANGE	
#endif 
#define ACTIVEVERTICESSZ_KVS (ACTIVEVERTICESSZ / VECTOR_SIZE)

#define VERTICESDATAMASKSZ (BATCH_RANGE / 32)
#define VERTICESDATAMASKSZ_KVS (VERTICESDATAMASKSZ / VECTOR_SIZE)

#define MESSAGES_AREASZ (MESSAGESDRAMSZ)
#define WORKSPACE_AREASZ (KVDRAMSZ + KVDRAMWORKSPACESZ + KVSTATSDRAMSZ)
#define EDGESWORKLOAD_AREASZ (EDGESSZ + VERTEXPTRSSZ)
#define VERTICESWORKLOAD_AREASZ (VERTICESDATASZ)
#define ACTIVEVERTICESWORKLOAD_AREASZ (ACTIVEVERTICESSZ + VERTICESDATAMASKSZ)

// #ifdef DISPATCHTYPE_ASYNC
#if defined(DISPATCHTYPE_SYNC) || defined(DISPATCHTYPE_ASYNC) // CRITICAL FIXME.
	#define KVSOURCEDRAMSZ (MESSAGES_AREASZ + WORKSPACE_AREASZ + EDGESWORKLOAD_AREASZ + VERTICESWORKLOAD_AREASZ + ACTIVEVERTICESWORKLOAD_AREASZ)			
#else
	#define KVSOURCEDRAMSZ (MESSAGES_AREASZ + WORKSPACE_AREASZ + EDGESWORKLOAD_AREASZ)
#endif 					
#define VDRAMSZ (MESSAGES_AREASZ + VERTICESWORKLOAD_AREASZ + ACTIVEVERTICESWORKLOAD_AREASZ)	

#define KVSOURCEDRAMSZ_KVS (KVSOURCEDRAMSZ / VECTOR_SIZE)
#define PADDEDKVSOURCEDRAMSZ (KVSOURCEDRAMSZ + (8*VECTOR_SIZE)) // 'padding for stats'
#define PADDEDKVSOURCEDRAMSZ_KVS (PADDEDKVSOURCEDRAMSZ / VECTOR_SIZE)

#define PADDEDVDRAMSZ VDRAMSZ
#define PADDEDVDRAMSZ_KVS (PADDEDVDRAMSZ / VECTOR_SIZE)

// base addresses
#define BASEOFFSET_MESSAGESDRAM 0 
#define BASEOFFSET_MESSAGESDRAM_KVS (BASEOFFSET_MESSAGESDRAM / VECTOR_SIZE) 

#define BASEOFFSET_KVDRAM (BASEOFFSET_MESSAGESDRAM + MESSAGESDRAMSZ)
#define BASEOFFSET_KVDRAM_KVS (BASEOFFSET_KVDRAM / VECTOR_SIZE)

#define BASEOFFSET_KVDRAMWORKSPACE (BASEOFFSET_KVDRAM + KVDRAMSZ)
#define BASEOFFSET_KVDRAMWORKSPACE_KVS (BASEOFFSET_KVDRAMWORKSPACE / VECTOR_SIZE)

#define BASEOFFSET_STATSDRAM (BASEOFFSET_KVDRAMWORKSPACE + KVDRAMWORKSPACESZ) 
#define BASEOFFSET_STATSDRAM_KVS (BASEOFFSET_STATSDRAM / VECTOR_SIZE)

// #ifdef DISPATCHTYPE_ASYNC
#if defined(DISPATCHTYPE_SYNC) || defined(DISPATCHTYPE_ASYNC) // CRITICAL FIXME.
	#define BASEOFFSET_EDGESDATA (BASEOFFSET_STATSDRAM + KVSTATSDRAMSZ) 
	#define BASEOFFSET_EDGESDATA_KVS (BASEOFFSET_EDGESDATA / VECTOR_SIZE)
	
	#define BASEOFFSET_VERTEXPTR (BASEOFFSET_EDGESDATA + EDGESSZ) 
	#define BASEOFFSET_VERTEXPTR_KVS (BASEOFFSET_VERTEXPTR / VECTOR_SIZE)
	
	#define BASEOFFSET_VERTICESDATA (BASEOFFSET_VERTEXPTR + VERTEXPTRSSZ) 
	#define BASEOFFSET_VERTICESDATA_KVS (BASEOFFSET_VERTICESDATA / VECTOR_SIZE)
	
	#define BASEOFFSET_ACTIVEVERTICES (BASEOFFSET_VERTICESDATA + VERTICESDATASZ) 
	#define BASEOFFSET_ACTIVEVERTICES_KVS (BASEOFFSET_ACTIVEVERTICES / VECTOR_SIZE)
	
	#define BASEOFFSET_VERTICESDATAMASK (BASEOFFSET_ACTIVEVERTICES + ACTIVEVERTICESSZ) 
	#define BASEOFFSET_VERTICESDATAMASK_KVS (BASEOFFSET_VERTICESDATAMASK / VECTOR_SIZE)
#endif  
	
// #if defined(DISPATCHTYPE_SYNC) || defined(DISPATCHTYPE2) // REMOVEME. depreciate DISPATCHTYPE2 later
#if defined(DISPATCHTYPE2)
	// kvsource
	#define BASEOFFSET_EDGESDATA (BASEOFFSET_STATSDRAM + KVSTATSDRAMSZ) 
	#define BASEOFFSET_EDGESDATA_KVS (BASEOFFSET_EDGESDATA / VECTOR_SIZE)

	#define BASEOFFSET_VERTEXPTR (BASEOFFSET_EDGESDATA + EDGESSZ) 
	#define BASEOFFSET_VERTEXPTR_KVS (BASEOFFSET_VERTEXPTR / VECTOR_SIZE)

	// vdram
	#define BASEOFFSET_VERTICESDATA (BASEOFFSET_MESSAGESDRAM + MESSAGESDRAMSZ) 
	#define BASEOFFSET_VERTICESDATA_KVS (BASEOFFSET_VERTICESDATA / VECTOR_SIZE)
	
	#define BASEOFFSET_ACTIVEVERTICES (BASEOFFSET_VERTICESDATA + VERTICESDATASZ)
	#define BASEOFFSET_ACTIVEVERTICES_KVS (BASEOFFSET_ACTIVEVERTICES / VECTOR_SIZE)

	#define BASEOFFSET_VERTICESDATAMASK (BASEOFFSET_ACTIVEVERTICES + ACTIVEVERTICESSZ) 
	#define BASEOFFSET_VERTICESDATAMASK_KVS (BASEOFFSET_VERTICESDATAMASK / VECTOR_SIZE)
#endif 

// others
#define COLLECTGLOBALSTATSCOMMAND 2
#define COLLECTSTATSCOMMAND 3
#define PARTITIONKEYVALUECOMMAND 4
#define COLLECTSTATSANDPARTITIONCOMMAND 5
#define APPENDINVALIDSCOMMAND 6
#define NOCOMMAND 8

#define NOBUFFERADDRESS 999999999
#endif 







