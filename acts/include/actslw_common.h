#ifndef ACTSLW_COMMON_H
#define ACTSLW_COMMON_H
#include "../../include/config_params.h"
#include "../../include/common.h"
#if (defined(HW) || defined(SWEMU))
#include <string.h>
#include <cmath>
#include <ap_int.h>
#endif

// buffer parameters
#define APPLYVERTEXBUFFERSZ (1 << (BATCH_RANGE_POW - (TREE_DEPTH * NUM_PARTITIONS_POW)))
#define APPLYVERTEXBUFFERSZ_KVS (APPLYVERTEXBUFFERSZ / VECTOR_SIZE) 

#define APPLYVERTEXBUFFERSZ2 (1 << (BATCH_RANGE2_POW - (TREE_DEPTH2 * NUM_PARTITIONS_POW)))
#define APPLYVERTEXBUFFERSZ2_KVS (APPLYVERTEXBUFFERSZ2 / VECTOR_SIZE)

#define PADDEDDESTBUFFER_SIZE 512
#define SRCBUFFER_SIZE (PADDEDDESTBUFFER_SIZE - ((4 * 3 * NUM_PARTITIONS) / VECTOR_SIZE))

#define CAPSULEBUFFER_SIZE 0 // not used
#define NFACTOR 0 // not used
#define PADDEDBUFFER_SIZE PADDEDDESTBUFFER_SIZE // not used

// global parameters
// dram sizes
#define MESSAGESDRAMSZ 1024

#define KVDRAMBUFFERSZ 0
#define KVDRAMBUFFERSZ_KVS (KVDRAMBUFFERSZ / VECTOR_SIZE)

// #define KVDRAMPADDING ((MAXKVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE) * (NUM_PARTITIONS * VECTOR_SIZE)) // REMOVEME. OBSOLETE
#define KVDRAMPADDING ((MAXKVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE) * (NUM_PARTITIONS * VECTOR_SIZE * 2) * 2)
// #define KVDRAMPADDING ((MAXKVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE) * (4 * 3 * NUM_PARTITIONS * VECTOR_SIZE))

#define KVDRAMSZ (MAXKVDATA_BATCHSIZE + KVDRAMPADDING)
#define KVDRAMSZ_KVS (KVDRAMSZ / VECTOR_SIZE)

#define KVDRAMWORKSPACESZ KVDRAMSZ
#define KVDRAMWORKSPACESZ_KVS (KVDRAMWORKSPACESZ / VECTOR_SIZE)

// #define KVSTATSDRAMSZ ((((NUMLASTLEVELPARTITIONS + (PADDEDDESTBUFFER_SIZE - 1)) / PADDEDDESTBUFFER_SIZE) * PADDEDDESTBUFFER_SIZE) * VECTOR_SIZE) // REMOVEME. OBSOLETE.				
#define KVSTATSDRAMSZ (NUMLASTLEVELPARTITIONS * (TREE_DEPTH + 1) * VECTOR_SIZE)
#define KVSTATS_SIZE KVSTATSDRAMSZ // not used
#define KVSTATSDRAMSZ_KVS KVSTATSDRAMSZ

#define KVSOURCEDRAMSZ (MESSAGESDRAMSZ + KVDRAMBUFFERSZ + KVDRAMSZ + KVDRAMWORKSPACESZ + KVSTATSDRAMSZ + (BATCH_RANGE / 2))
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

#define BASEOFFSET_CAPSULES 0 // not used
#define BASEOFFSET_CAPSULES_KVS 0 // not used

#define BASEOFFSET_STATSDRAM (BASEOFFSET_KVDRAMWORKSPACE + KVDRAMSZ) 
#define BASEOFFSET_STATSDRAM_KVS (BASEOFFSET_STATSDRAM / VECTOR_SIZE)

#define BASEOFFSET_VERTICESDATA (BASEOFFSET_STATSDRAM + KVSTATSDRAMSZ) 
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


