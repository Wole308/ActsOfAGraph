#ifndef COMMON_H
#define COMMON_H
#include "config_params.h"
#include <string.h> 
#include <cmath>
 

// CUSTOMPROVISION: { classname__top_usrcv_nudstv.cpp, reduceupdates.cpp, }

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define TESTKERNEL
#define SW // SWEMU, HW, SW
#define PR_ALGORITHM //  // PR_ALGORITHM, CF_ALGORITHM, HITS_ALGORITHM, CC_ALGORITHM, SSSP_ALGORITHM, BFS_ALGORITHM
#if (defined(SWEMU) || defined(HW))
#define FPGA_IMPL
#endif 

// #define RANKING_ALGORITHM
#define TRAVERSAL_ALGORITHM
// #define BIT_TRAVERSAL_ALGORITHM ////////////////// NEWCHANGE.

#if defined(BFS_ALGORITHM) || defined(SSSP_ALGORITHM)
#define CONFIG_ALGORITHMTYPE_RANDOMACTIVEVERTICES // { utility.cpp }
#endif

#define _SINGLEKERNEL
#ifdef FPGA_IMPL
#define _WIDEWORD
// #define _WIDEWORD_FOR_KV
#endif 

#define ALLVERTEXISACTIVE_ALGORITHM
#if defined(BFS_ALGORITHM) && defined(CONFIG_FORBFS_REPRESENTVDATASASBITS)
#define ALGORITHMTYPE_REPRESENTVDATASASBITS
#endif 

#define PARAM_ACTSONLYMODE 1
#define PARAM_TRADGPONLYMODE 2
#define PARAM_HYBRIDGPMODE 3

#define ACTS
#define CONFIG_ACTS_PERFORMANCEOFALGORITHM // { mem_access.cpp, acts.cpp }
#define CONFIG_ACTS_MEMORYLAYOUT // { processedges.cpp } // CRITICAL FIXME?
#define CONFIG_ACTS_PARTITIONINGLOGIC // { partitionupdates.cpp, acts.cpp }
#define CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC // { common.h, reduceupdates2.cpp, acts_util.cpp, classname__top_usrcv_udstv.cpp, acts.cpp }
#define CONFIG_ACTS_HYBRIDLOGIC // CRITICAL FIXME?
// #define CONFIG_IMPACT_OF_SCALING_NUMPEs	
	
////////////////

#ifndef TESTKERNEL_IMPACTOFRANGE
#define LOADTEMPSPACETOFPGA
#endif 

#define GRAPHISUNDIRECTED

#ifdef FPGA_IMPL
#define ENABLE_KERNEL_PROFILING
#endif 

#define _DEBUGMODE_HEADER //
#if defined (FPGA_IMPL) // && defined (HW) // REMOVEME. 
#else
#define _DEBUGMODE_STATS // CRITICAL ADDME
// #define _DEBUGMODE_CHECKS
#define _DEBUGMODE_CHECKS2 // CRITICAL ADDME
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

#ifndef FPGA_IMPL
// #define _DEBUGMODE_KERNELPRINTS_TRACE3 //
#endif 

////////////////

#define PAGERANK 222
#define CF 333
#define HITS 444
#define SPMV 555
#define BFS 666
#define SSSP 777
#define ALGORITHMCLASS_ALLVERTEXISACTIVE 2222 // {classname__top_usrcv_udstv, classname__processedges_splitdstvxs, classname__top_nusrcv_nudstv}
#define ALGORITHMCLASS_NOTALLVERTEXISACTIVE 3333

////////////////

#define NUM_PEs 24
#define MAXNUM_PEs 32
#define NUMSYNCTHREADS 
#define NUMUTILITYTHREADS 16 // NUMCPUTHREADS // FIXME?
#define MAXNUM_VPs 1024 // 512 // 256 // NEWCHANGE***
#define MAXNUM_LLPs 256 // NEWCHANGE.
#define MAXNUM_LLPSETs 16 // NEWCHANGE NOW.

////////////////
#if NUM_PEs==3
#define NUMCOMPUTEUNITS_SLR1AND2 1 
#define NUMCOMPUTEUNITS_SLR0 1
#define NUMCOMPUTEUNITS_SLR1 1
#define NUMCOMPUTEUNITS_SLR2 1
#endif 
#if NUM_PEs==12
#define NUMCOMPUTEUNITS_SLR1AND2 5 
#define NUMCOMPUTEUNITS_SLR0 2
#define NUMCOMPUTEUNITS_SLR1 5
#define NUMCOMPUTEUNITS_SLR2 5
#endif 
#if NUM_PEs==14
#define NUMCOMPUTEUNITS_SLR1AND2 5 
#define NUMCOMPUTEUNITS_SLR0 4
#define NUMCOMPUTEUNITS_SLR1 5
#define NUMCOMPUTEUNITS_SLR2 5
#endif 
#if NUM_PEs==16
#define NUMCOMPUTEUNITS_SLR1AND2 6 
#define NUMCOMPUTEUNITS_SLR0 4
#define NUMCOMPUTEUNITS_SLR1 6
#define NUMCOMPUTEUNITS_SLR2 6
#endif 
#if NUM_PEs==18
#define NUMCOMPUTEUNITS_SLR1AND2 6 
#define NUMCOMPUTEUNITS_SLR0 6
#define NUMCOMPUTEUNITS_SLR1 6
#define NUMCOMPUTEUNITS_SLR2 6
#endif 
#if NUM_PEs==20
#define NUMCOMPUTEUNITS_SLR1AND2 7 
#define NUMCOMPUTEUNITS_SLR0 6
#define NUMCOMPUTEUNITS_SLR1 7
#define NUMCOMPUTEUNITS_SLR2 7
#endif 
#if NUM_PEs==22
#define NUMCOMPUTEUNITS_SLR1AND2 8 
#define NUMCOMPUTEUNITS_SLR0 6
#define NUMCOMPUTEUNITS_SLR1 8
#define NUMCOMPUTEUNITS_SLR2 8
#endif 
#if NUM_PEs==24
#define NUMCOMPUTEUNITS_SLR1AND2 9 
#define NUMCOMPUTEUNITS_SLR0 6
#define NUMCOMPUTEUNITS_SLR1 9
#define NUMCOMPUTEUNITS_SLR2 9
#endif 
#if NUM_PEs==25
#define NUMCOMPUTEUNITS_SLR1AND2 9 
#define NUMCOMPUTEUNITS_SLR0 7
#define NUMCOMPUTEUNITS_SLR1 9
#define NUMCOMPUTEUNITS_SLR2 9
#endif 
#if NUM_PEs==32
#define NUMCOMPUTEUNITS_SLR1AND2 10 
#define NUMCOMPUTEUNITS_SLR0 12
#define NUMCOMPUTEUNITS_SLR1 10
#define NUMCOMPUTEUNITS_SLR2 10
#endif 

////////////////

#define DATAWIDTH 512 
#define VECTOR_SIZE 8
#define VECTOR2_SIZE (VECTOR_SIZE * 2)
#define VECTOR2_SIZE_POW 4
#define HBM_BITWIDTHx32 16
#define HBM_CHANNEL_VECTOR_SIZE 16
#define VECTOR1024_SIZE 16
#define DATATYPE_SIZE 32
#define NUMCOMPUTEUNITS 24 
#define NUMCOMPUTEUNITS_HALF (NUMCOMPUTEUNITS / 2)
#define NUMINTSINKEYVALUETYPE 2
#define VDATA_PACKINGSIZE_POW 4 // AUTOMATEME.
#define VDATA_PACKINGSIZE (1 << VDATA_PACKINGSIZE_POW) //
#define VDATA_BITWIDTH 32
#ifdef CONFIG_UPDATEPACKINGx16
	#define UPDATEDATA_PACKINGSIZE 16
	#define EDGEDATA_PACKINGSIZE 16 
#else 
	#define UPDATEDATA_PACKINGSIZE 8
	#define EDGEDATA_PACKINGSIZE 8
#endif
#define KEYVALUEDATA_PACKINGSIZE 8
#define VALUEDATA_PACKINGSIZE 16
// #define VDATA_BITWIDTH 64 
#define STATS_PACKINGSIZE 18

// messages area
#define MESSAGESDRAMSZ 1024
#define BASEOFFSET_MESSAGESDATA 0 
#define BASEOFFSET_MESSAGESDATA_KVS (BASEOFFSET_MESSAGESDATA / VECTOR_SIZE) 

#define MESSAGES_SIZE 1024
#define MESSAGES_SIZE_KVS (MESSAGES_SIZE / VECTOR_SIZE)
#define MESSAGES_EDGESSTATSDRAM_SIZE 32

#define MESSAGES_MAILBOX_SIZE 32//16 // 8//16
#define MESSAGES_MAILBOX1_ACTSMODE 0

#define MESSAGES_RETURNVALUES_SIZE 512 // 32//16 // 8//16
#define MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED 0
#define MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED 32
#define MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 64
#define MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION 96
#define MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR 128
#define MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES 192 // 256
#define MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP 256
#define MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP 257
#define MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP 258
#define MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP 259
#define MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP 260

#define MESSAGES_ENABLE_RUNKERNELCOMMAND (0)
#define MESSAGES_ENABLE_PROCESSCOMMAND (MESSAGES_ENABLE_RUNKERNELCOMMAND + 1)
#define MESSAGES_ENABLE_PARTITIONCOMMAND (MESSAGES_ENABLE_PROCESSCOMMAND + 1)
#define MESSAGES_ENABLE_APPLYUPDATESCOMMAND (MESSAGES_ENABLE_PARTITIONCOMMAND + 1)
#define MESSAGES_ENABLE_MERGECOMMAND (MESSAGES_ENABLE_APPLYUPDATESCOMMAND + 1)
#define MESSAGES_ENABLE_EXCHANGECOMMAND (MESSAGES_ENABLE_MERGECOMMAND + 1)
#define MESSAGES_GPMODE (MESSAGES_ENABLE_EXCHANGECOMMAND + 1)
#define MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES (MESSAGES_GPMODE + 1) // USED FOR PASSING CONFIG_INSERTSTATSMETADATAINEDGES

#define MESSAGES_EVALUATION_ACTS_MEMORYLAYOUT (MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES + 1)
#define MESSAGES_EVALUATION_ACTS_PARTITIONINGLOGIC (MESSAGES_EVALUATION_ACTS_MEMORYLAYOUT + 1)
#define MESSAGES_EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC (MESSAGES_EVALUATION_ACTS_PARTITIONINGLOGIC + 1)
#define MESSAGES_EVALUATION_ACTS_HYBRIDLOGIC (MESSAGES_EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC + 1)
#define MESSAGES_EVALUATION_ACTS_VERYBARE (MESSAGES_EVALUATION_ACTS_HYBRIDLOGIC + 1)

#define MESSAGES_THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD (MESSAGES_EVALUATION_ACTS_VERYBARE + 1)
#define MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION (MESSAGES_THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD + 1)
#define MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION (MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION + 1)
#define MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION (MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION + 1)

#define MESSAGES_BASEOFFSETKVS_MESSAGESDATA (MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION + 1)
#define MESSAGES_VOLUMEOFFSETKVS_WORKDATA (MESSAGES_BASEOFFSETKVS_MESSAGESDATA + 1) // FIXME. seperate these..........
#define MESSAGES_VOLUMEOFFSETKVS_WORKSPACEDATA (MESSAGES_VOLUMEOFFSETKVS_WORKDATA + 1) // FIXME. seperate these..........
#define MESSAGES_BASEOFFSETKVS_EDGESDATA (MESSAGES_VOLUMEOFFSETKVS_WORKSPACEDATA + 1)
#define MESSAGES_BASEOFFSETKVS_VERTEXPTR (MESSAGES_BASEOFFSETKVS_EDGESDATA + 1)
#define MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA (MESSAGES_BASEOFFSETKVS_VERTEXPTR + 1)
#define MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA (MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA + 1)
#define MESSAGES_BASEOFFSETKVS_ACTIVEUPROPBLOCKS (MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA + 1)
#define MESSAGES_BASEOFFSETKVS_ACTIVEEDGEBLOCKS (MESSAGES_BASEOFFSETKVS_ACTIVEUPROPBLOCKS + 1)
#define MESSAGES_BASEOFFSETKVS_ACTIVEUPDATEBLOCKS (MESSAGES_BASEOFFSETKVS_ACTIVEEDGEBLOCKS + 1)
#define MESSAGES_BASEOFFSETKVS_EDGESMAP (MESSAGES_BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + 1)
#define MESSAGES_BASEOFFSETKVS_EDGEBLOCKMAP (MESSAGES_BASEOFFSETKVS_EDGESMAP + 1)
#define MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK (MESSAGES_BASEOFFSETKVS_EDGEBLOCKMAP + 1)
#define MESSAGES_BASEOFFSETKVS_STATSDRAM (MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK + 1)
#define MESSAGES_BASEOFFSETKVS_EDGESSTATSDRAM (MESSAGES_BASEOFFSETKVS_STATSDRAM + 1) // 
#define MESSAGES_BASEOFFSETKVS_VERTEXUPDATESDATA (MESSAGES_BASEOFFSETKVS_EDGESSTATSDRAM + 1) /////////////
#define MESSAGES_BASEOFFSETKVS_KVDRAM (MESSAGES_BASEOFFSETKVS_VERTEXUPDATESDATA + 1)
#define MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE (MESSAGES_BASEOFFSETKVS_KVDRAM + 1)
#define MESSAGES_ACTSMESSAGE_OTHERINFOS (MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE + 1)

#define MESSAGES_BUFFERBASEOFFSETKVS_VDATA1 (MESSAGES_ACTSMESSAGE_OTHERINFOS + 1) // NEWCHANGE.
#define MESSAGES_BUFFERBASEOFFSETKVS_VDATA2 (MESSAGES_BUFFERBASEOFFSETKVS_VDATA1 + 1)
#define MESSAGES_BUFFERBASEOFFSETKVS_VDATA3 (MESSAGES_BUFFERBASEOFFSETKVS_VDATA2 + 1)
#define MESSAGES_BUFFERBASEOFFSETKVS_VDATA4 (MESSAGES_BUFFERBASEOFFSETKVS_VDATA3 + 1)
#define MESSAGES_BUFFERBASEOFFSETKVS_VDATA5 (MESSAGES_BUFFERBASEOFFSETKVS_VDATA4 + 1)

#define MESSAGES_SIZE_MESSAGESDATA (MESSAGES_BUFFERBASEOFFSETKVS_VDATA5 + 1)
#define MESSAGES_VOLUMESIZEU32_WORKDATA (MESSAGES_SIZE_MESSAGESDATA + 1)
#define MESSAGES_VOLUMESIZEU32_WORKSPACEDATA (MESSAGES_VOLUMESIZEU32_WORKDATA + 1)
#define MESSAGES_SIZE_EDGES (MESSAGES_VOLUMESIZEU32_WORKSPACEDATA + 1)
#define MESSAGES_SIZE_VERTEXPTRS (MESSAGES_SIZE_EDGES + 1)
#define MESSAGES_SIZE_SRCVERTICESDATA (MESSAGES_SIZE_VERTEXPTRS + 1)
#define MESSAGES_SIZE_DESTVERTICESDATA (MESSAGES_SIZE_SRCVERTICESDATA + 1)
#define MESSAGES_SIZE_ACTIVEUPROPBLOCKS (MESSAGES_SIZE_DESTVERTICESDATA + 1)
#define MESSAGES_SIZE_ACTIVEEDGEBLOCKS (MESSAGES_SIZE_ACTIVEUPROPBLOCKS + 1)
#define MESSAGES_SIZE_ACTIVEUPDATEBLOCKS (MESSAGES_SIZE_ACTIVEEDGEBLOCKS + 1) 
#define MESSAGES_SIZE_EDGESMAP (MESSAGES_SIZE_ACTIVEUPDATEBLOCKS + 1)
#define MESSAGES_SIZE_EDGEBLOCKMAP (MESSAGES_SIZE_EDGESMAP + 1)
#define MESSAGES_SIZE_VERTICESPARTITIONMASK (MESSAGES_SIZE_EDGEBLOCKMAP + 1)
#define MESSAGES_SIZE_KVSTATSDRAM (MESSAGES_SIZE_VERTICESPARTITIONMASK + 1)
#define MESSAGES_SIZE_EDGESSTATSDRAM (MESSAGES_SIZE_KVSTATSDRAM + 1) //
#define MESSAGES_SIZE_VERTEXUPDATESDATA (MESSAGES_SIZE_EDGESSTATSDRAM + 1)
#define MESSAGES_SIZE_KVDRAM (MESSAGES_SIZE_VERTEXUPDATESDATA + 1)
#define MESSAGES_SIZE_KVDRAMWORKSPACE (MESSAGES_SIZE_KVDRAM + 1)
#define MESSAGES_SIZE_OTHERINFOS (MESSAGES_SIZE_KVDRAMWORKSPACE + 1)
#define MESSAGES_SIZEKVS_PROCESSEDGESPARTITION (MESSAGES_SIZE_OTHERINFOS + 1) 
#define MESSAGES_SIZE_REDUCE (MESSAGES_SIZEKVS_PROCESSEDGESPARTITION + 1)
#define MESSAGES_SIZEKVS_REDUCEPARTITION (MESSAGES_SIZE_REDUCE + 1) 
#define MESSAGES_SIZEKVS_VMASKBUFFER (MESSAGES_SIZEKVS_REDUCEPARTITION + 1) 
#define MESSAGES_SIZE_BATCHRANGE (MESSAGES_SIZEKVS_VMASKBUFFER + 1)
#define MESSAGES_SIZE_RUN (MESSAGES_SIZE_BATCHRANGE + 1)

#define MESSAGES_POW_MESSAGESDRAM (MESSAGES_SIZE_RUN + 1)
#define MESSAGES_POW_EDGES (MESSAGES_POW_MESSAGESDRAM + 1)
#define MESSAGES_POW_VERTEXPTRS (MESSAGES_POW_EDGES + 1)
#define MESSAGES_POW_SRCVERTICESDATA (MESSAGES_POW_VERTEXPTRS + 1)
#define MESSAGES_POW_DESTVERTICESDATA (MESSAGES_POW_SRCVERTICESDATA + 1)
#define MESSAGES_POW_ACTIVEEDGEBLOCKS (MESSAGES_POW_DESTVERTICESDATA + 1)
#define MESSAGES_POW_VERTICESDATAMASK (MESSAGES_POW_ACTIVEEDGEBLOCKS + 1)
#define MESSAGES_POW_KVSTATSDRAM (MESSAGES_POW_VERTICESDATAMASK + 1)
#define MESSAGES_POW_EDGESSTATSDRAM (MESSAGES_POW_KVSTATSDRAM + 1) //
#define MESSAGES_POW_VERTEXUPDATESDATA (MESSAGES_POW_EDGESSTATSDRAM + 1)
#define MESSAGES_POW_KVDRAM (MESSAGES_POW_VERTEXUPDATESDATA + 1)
#define MESSAGES_POW_KVDRAMWORKSPACE (MESSAGES_POW_KVDRAM + 1)
#define MESSAGES_POW_REDUCE (MESSAGES_POW_KVDRAMWORKSPACE + 1)
#define MESSAGES_POW_BATCHRANGE (MESSAGES_POW_REDUCE + 1)

#define MESSAGES_NUM_PROCESSEDGESPARTITIONS (MESSAGES_POW_BATCHRANGE + 1)
#define MESSAGES_NUM_REDUCEPARTITIONS (MESSAGES_NUM_PROCESSEDGESPARTITIONS + 1)

#define MESSAGES_ALGORITHMINFO_GRAPHITERATIONID (MESSAGES_NUM_REDUCEPARTITIONS + 1)
#define MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID (MESSAGES_ALGORITHMINFO_GRAPHITERATIONID + 1)

#define MESSAGES_ACTSPARAMS_BEGINLOP (MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID + 1)
#define MESSAGES_ACTSPARAMS_NUMLOPS (MESSAGES_ACTSPARAMS_BEGINLOP + 1)
#define MESSAGES_ACTSPARAMS_TREEDEPTH (MESSAGES_ACTSPARAMS_NUMLOPS + 1)
#define MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS (MESSAGES_ACTSPARAMS_TREEDEPTH + 1)
#define MESSAGES_ACTSPARAMS_SRCVOFFSET (MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS + 1)
#define MESSAGES_ACTSPARAMS_SRCVSIZE (MESSAGES_ACTSPARAMS_SRCVOFFSET + 1)
#define MESSAGES_ACTSPARAMS_DESTVOFFSET (MESSAGES_ACTSPARAMS_SRCVSIZE + 1)
#define MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER (MESSAGES_ACTSPARAMS_DESTVOFFSET + 1)
#define MESSAGES_ACTSPARAMS_INSTID (MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER + 1)
#define MESSAGES_ACTSPARAMS_NUM_EDGE_BANKS (MESSAGES_ACTSPARAMS_INSTID + 1)
#define MESSAGES_ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM (MESSAGES_ACTSPARAMS_NUM_EDGE_BANKS + 1)
#define MESSAGES_ACTSPARAMS_NUM_PARTITIONS (MESSAGES_ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM + 1)
#define MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP1 (MESSAGES_ACTSPARAMS_NUM_PARTITIONS + 1)
#define MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP2 (MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP1 + 1)
#define MESSAGES_ACTSPARAMS_BATCH_RANGE (MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP2 + 1)
#define MESSAGES_ACTSPARAMS_POW_PARTITIONS (MESSAGES_ACTSPARAMS_BATCH_RANGE + 1)
#define MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP1 (MESSAGES_ACTSPARAMS_POW_PARTITIONS + 1)
#define MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP2 (MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP1 + 1)
#define MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP3 (MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP2 + 1)
#define MESSAGES_ACTSPARAMS_WORKBUFFER_SIZE (MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP3 + 1)
#define MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS (MESSAGES_ACTSPARAMS_WORKBUFFER_SIZE + 1)
#define MESSAGES_ACTSPARAMS_VPTR_SHRINK_RATIO (MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS + 1) /////////////////////// pass through loadgraph.cpp
#define MESSAGES_ACTSPARAMS_READOFFLINESTATS (MESSAGES_ACTSPARAMS_VPTR_SHRINK_RATIO + 1)
#define MESSAGES_ACTSPARAMS_UNUSED4 (MESSAGES_ACTSPARAMS_READOFFLINESTATS + 1)
#define MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 (MESSAGES_ACTSPARAMS_UNUSED4 + 1)
#define MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR1 (MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + 1)
#define MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR0 (MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR1 + 1)

#define MESSAGES_DUMMYCHKPOINT (MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR0 + 1)
#define MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM (MESSAGES_DUMMYCHKPOINT + 1)
#define MESSAGES_RETURNVALUES (MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + MESSAGES_EDGESSTATSDRAM_SIZE)
#define MESSAGES_MAILBOX (MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_SIZE)

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

#define NAp 666
// #define INVALIDDATA 0x0FFFFFFF 
#define INVALIDDATA 0xFFFFFFFF 
#define UNUSEDDATA 987654321

#define ONE 1
#define TWO 2

#define SNAP 1
#define SYNTHETIC 2

#define OFFSETOF_DSTV_IN_EDGEDSTVDATA 0
#define SIZEOF_DSTV_IN_EDGEDSTVDATA 22
#define OFFSETOF_SRCV_IN_EDGEDSTVDATA 22
#define SIZEOF_SRCV_IN_EDGEDSTVDATA 10

#define EDGESTATUS_VALIDEDGE 5
#define EDGESTATUS_INVALIDEDGE 6
#define EDGESTATUS_BITMAP 7
#define EDGESTATUS_SOURCEV 8
#define EDGESTATUS_DUMMYEDGE 9

#define BRAM_BIT_WIDTH 32

#define ACTSPROCESSMODE 0
#define ACTSPARTITIONMODE 1
#define ACTSREDUCEMODE 2
#define ACTSFETCHMODE 3
#define ACTSCOMMITMODE 4
#define READUPROPERTYMODE 5
#define READVPROPERTYMODE 6
#define SAVEVPROPERTYMODE 7

// acts-main parameters 
#define NUMPIPELINES_PARTITIONUPDATES 2
#if NUMPIPELINES_PARTITIONUPDATES==1
#define PUP0
#endif 
#if NUMPIPELINES_PARTITIONUPDATES==2
#define PUP0
#define PUP1
#endif

#define ACTSPROC_AUTOMATE_ACROSSDATASETS
#define ACTSSYNC_AUTOMATE_ACROSSALGORITHMS

#define REDUCEBUFFERFACTOR 8 // CONSTANT

// #define ENABLE_SUBVMASKING
#define SUBPMASKFACTOR_POW 4
#define SUBPMASKFACTOR 16 // NOTE: CONSTANT

// buffer parameters
#define BLOCKRAM_SIZE 512
#define DOUBLE_BLOCKRAM_SIZE (BLOCKRAM_SIZE * 2)

#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
#define MAX_PROC_SRAMSZ_POW 13//13
#else 
// #define MAX_PROC_SRAMSZ_POW 10
#define MAX_PROC_SRAMSZ_POW 9
#endif

#define NUM_VERTICES_PER_UPROPBLOCK (NUM_PEs * EDGEDATA_PACKINGSIZE) // 384 (= 24 kvs)
#define NUM_VERTICESKVS_PER_UPROPBLOCK (NUM_VERTICES_PER_UPROPBLOCK / VDATA_PACKINGSIZE) // (= 24 kvs)
#define NUM_UPROPBLOCKS_PER_VPARTITION (((1 << MAX_PROC_SRAMSZ_POW) * VDATA_PACKINGSIZE) / NUM_VERTICES_PER_UPROPBLOCK) // 341
#define MAXNUM_UPROPBLOCKS_PER_VPARTITION 512

#define NUM_EDGESKVS_PER_UPROPBLOCK 32
#define MAXNUM_EDGEBLOCKS_PER_VPARTITION 1024 // 512 // 256 // 1024 // (= 32 vs)

#define POW_SHRINKCONSTANT_UPDATEBLOCKS 8 // 2^8=256 

#ifdef CONFIG_USEURAM_FOR_DESTVBUFFER
#define MAX_RED_SRAMSZ_POW 13//13 
#else 
// #define MAX_RED_SRAMSZ_POW 10
#define MAX_RED_SRAMSZ_POW 9
#endif
#define MAX_RED_SRAMSZ (1 << MAX_RED_SRAMSZ_POW)

#define MAX_BLOCKRAM_VSRCDATA_SIZE (1 << MAX_PROC_SRAMSZ_POW)
#define MAX_BLOCKRAM_VDESTDATA_SIZE	MAX_BLOCKRAM_VSRCDATA_SIZE

#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS // {actscommon.h, loadedges_random.cpp, processedges.cpp, algorithm.cpp, loadgraph.cpp, mem_access.cpp, algo_funcs.cpp}				
#define VDATA_SHRINK_RATIO_POW 4 // CONSTANT. // NUM_PARTITIONS_POW
#else 
#define VDATA_SHRINK_RATIO_POW 0
#endif 
#define VDATA_SHRINK_RATIO (1 << VDATA_SHRINK_RATIO_POW)

#define OPT_NUM_PARTITIONS_POW 4
#define OPT_NUM_PARTITIONS (1 << OPT_NUM_PARTITIONS_POW)
#define MAX_NUM_PARTITIONS 16

#define BLOCKRAM_VDATA_SECTION0 0
// #define BLOCKRAM_VDATA_SECTION1 (MAX_BLOCKRAXMEM_VDATA_SIZE / 2)
#define BLOCKRAM_VDATA_SECTION1 0 // FIXME.

#define BLOCKRAM_CURRPMASK_SIZE 512 // 1024 // 512 // 256 // // NEWCHANGE***
#define BLOCKRAM_NEXTPMASK_SIZE BLOCKRAM_CURRPMASK_SIZE 
#define BLOCKRAM_CUMMTVPMASK_SIZE BLOCKRAM_CURRPMASK_SIZE

#define BLOCKRAM_GLOBALSTATS_SIZE BLOCKRAM_SIZE 
#define BLOCKRAM_GLOBALSTATS_BIGSIZE BLOCKRAM_SIZE 

#define MAX_SRCBUFFER_SIZE (BLOCKRAM_SIZE - (4 * 4))
#define MAX_WORKBUFFER_SIZE (MAX_SRCBUFFER_SIZE - (NUM_PARTITIONS*4))

#define MAX_SOURCEBLOCKRAM_SIZE BLOCKRAM_SIZE 
#define MAX_DESTBLOCKRAM_SIZE MAX_SOURCEBLOCKRAM_SIZE

#define MAX_ACTVVBUFFER_SIZE (BLOCKRAM_SIZE * 2)

#define BLOCKRAM_UPDATEBLOCK_POW 9
#define BLOCKRAM_UPDATEBLOCK_SIZE (1 << BLOCKRAM_UPDATEBLOCK_POW)

#define MAX_NUMPROCESSEDGESPARTITIONS 16384 // FIXME.

#define MAX_NUM_EDGE_BANKS 1

#define EDGESSTATSDRAMSZ 8

#define MAXNUMGRAPHITERATIONS 16

#define MAX_MAXHBMCAPACITY_V ((1 << 28) / 4) // (256MB/4=64M)
#define MAX_MAXHBMCAPACITY_KV ((1 << 28) / 8) // (256MB/8=32M)
#define MAX_MAXHBMCAPACITY_VS (MAX_MAXHBMCAPACITY_V / VECTOR2_SIZE) // (64M/16=4M)
#define MAX_MAXHBMCAPACITY_KVS (MAX_MAXHBMCAPACITY_KV / VECTOR_SIZE) // (32M/8=4M)
#define MAX_KVSOURCEDRAMSZ (MAX_MAXHBMCAPACITY_KVS * VECTOR_SIZE) // max HBM capacity (256MB)
#define MAX_KVSOURCEDRAMSZ_KVS (MAX_KVSOURCEDRAMSZ / VECTOR_SIZE)
#define MAX_PADDEDKVSOURCEDRAMSZ MAX_KVSOURCEDRAMSZ
#define MAX_PADDEDKVSOURCEDRAMSZ_KVS (MAX_PADDEDKVSOURCEDRAMSZ / VECTOR_SIZE)

#define SRC_VOLUME_SIZE_KVS (6000000 / VECTOR2_SIZE)

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

#define BEGINOFFSETOF_VDATA 1
#define OFFSETOF_VDATA 1
#define SIZEOF_VDATA 31
#define BEGINOFFSETOF_VMASK 0
#define OFFSETOF_VMASK 0
#define SIZEOF_VMASK 1

#define OFFSETOF_EDIR 31
#define SIZEOF_EDIR 1
 
#define SIZEOF_VDATAKEY 16
#define SIZEOF_VDATAVALUE 16

// #define MAXVDATA 0xFFFFFFFF 
#define MAXVDATA 0b01111111111111111111111111111111

#define NUM_COLLECTIONS 4 // 16
#define COLLECTIONS_BUFFERSZ 4//2 // NEWCHANGE.
#define TRAVERSEDEDGES_COLLECTIONID 0
#define PROCESSEDGES_COLLECTIONID 1
#define REDUCEUPDATES_COLLECTIONID 2 
#define ACTIVEEDGESPROCESSED_COLLECTIONID 3

#define MASK_DSTVID 0b00000000000000111111111111111111
#define MASK_SRCVID 0b00000000000000000011111111111111 
#define DSTVID_BITSZ 18
#define SRCVID_BITSZ 14

typedef unsigned int vertex_t;
typedef unsigned int edge_t;

typedef unsigned int keyy_t;
typedef unsigned int value_t;
typedef unsigned int value2_dt;

typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int vector_type;
typedef unsigned int step_type;
typedef unsigned int bool_type;
typedef unsigned int analysis_type;
typedef unsigned int visitstate_type;

typedef struct {
	unsigned int key;
	unsigned int value;
} keyvalue_t;

typedef struct {
	unsigned int key;
	unsigned int value;
	unsigned int mask;
} keyvaluemask_t;

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

typedef struct {
	keyvalue1_type data[16];
} unitBRAMwidth_type; 

typedef struct {
	#ifdef _WIDEWORD
	ap_uint<1> data[32];
	#else 
	unsigned int data[32];
	#endif 
} uint32_vec_dt;

#ifdef _WIDEWORD
typedef ap_uint<1> unit1_type;
#else 
typedef unsigned int unit1_type;
#endif 

typedef unsigned int pmask_dt;

typedef unsigned int stats_t;

#ifdef _WIDEWORD
typedef ap_uint<3> unit3_type;
#else 
typedef unsigned int unit3_type;
#endif 

#ifdef _WIDEWORD
typedef ap_uint<4> unit4_type;
#else 
typedef unsigned int unit4_type;
#endif 

#ifdef _WIDEWORD
typedef ap_uint<16> uint16_type;
#else 
typedef unsigned int uint16_type;
#endif

#ifdef _WIDEWORD
typedef ap_uint<VDATA_BITWIDTH> uint32_type; // REMOVEME.
#else
typedef unsigned int uint32_type;
#endif

#ifdef _WIDEWORD
typedef ap_uint<64> uint64_type;
#else
typedef keyvalue_t uint64_type;
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
	keyy_t eblockid;
	bool valid;
} edge2_type; 

// typedef struct {
	// edge2_type data[EDGEDATA_PACKINGSIZE];
// } edge2_vec_dt;

typedef struct {
	edge2_type data[EDGEDATA_PACKINGSIZE + 1];
} edge2_vec_dt;

typedef struct {
	unsigned int offset;
	unsigned int size;
} map_t;

typedef struct {
	unsigned int index;
} edgeblock_hashtable_dt;

typedef struct {
	keyy_t key;
} vptr_type;

typedef struct {
	keyy_t key;
	unsigned int count;
} vptr2_type;

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
	// keyvalue_t data[VECTOR_SIZE];
	keyvalue_t data[KEYVALUEDATA_PACKINGSIZE];
} uint512_vec_dt;

typedef struct {
	keyvalue_t data[UPDATEDATA_PACKINGSIZE]; 
} uint512_uvec_dt;

typedef struct {
	// keyvalue_t data[EDGEDATA_PACKINGSIZE];
	value_t data[EDGEDATA_PACKINGSIZE];
} uint512_evec_dt;

typedef struct {
	unsigned int data[VECTOR2_SIZE];
} uint512_ivec_dt;

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

typedef struct {
	unsigned int A;
    unsigned int B;
} tuple_t;

typedef struct {
	unsigned int A;
    unsigned int B;
	unsigned int C;
	unsigned int D;
} triple_t;

typedef struct {
	#ifdef _WIDEWORD
	ap_uint<16> key;
	ap_uint<16> value;
	#else 
	unsigned int key;
	unsigned int value;
	#endif 
} keyvalue_capsule_t;

#ifdef _WIDEWORD_FOR_KV
typedef ap_uint<32> keyvalue_buffer_t;
#else
typedef struct {
	unsigned int key;
	unsigned int value;
} keyvalue_buffer_t;
#endif

typedef unsigned int vdata_t;

typedef struct {
	vdata_t data;
} keyvalue_vbuffer_t;

#ifdef _WIDEWORD
typedef ap_uint<32> vtxbuffer_type;
#else
typedef unsigned int vtxbuffer_type;
#endif 

typedef struct {
	unsigned int data[3];
} wide_word_bits_3t; 

typedef struct {
	unsigned int data1;
} collection_t;

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
	unsigned int DRAM_BASE_KVS;
	
	unsigned int ENABLE_RUNKERNELCOMMAND;
	unsigned int ENABLE_PROCESSCOMMAND;
	unsigned int ENABLE_PARTITIONCOMMAND;
	unsigned int ENABLE_APPLYUPDATESCOMMAND;
	unsigned int ENABLE_MERGECOMMAND;
	unsigned int ENABLE_EXCHANGECOMMAND;
	unsigned int GPMODE;
	unsigned int ACTSCONFIG_INSERTSTATSMETADATAINEDGES;
	
	unsigned int EVALUATION_ACTS_MEMORYLAYOUT;
	unsigned int EVALUATION_ACTS_PARTITIONINGLOGIC;
	unsigned int EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC;
	unsigned int EVALUATION_ACTS_HYBRIDLOGIC;
	unsigned int EVALUATION_ACTS_VERYBARE;
	
	unsigned int THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD;
	unsigned int THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION;
	unsigned int THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION;
	unsigned int THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION;

	unsigned int BASEOFFSETKVS_MESSAGESDATA;
	unsigned int VOLUMEOFFSETKVS_WORKDATA;
	unsigned int VOLUMEOFFSETKVS_WORKSPACEDATA;
	unsigned int BASEOFFSETKVS_EDGESDATA;
	unsigned int BASEOFFSETKVS_VERTEXPTR;
	unsigned int BASEOFFSETKVS_SRCVERTICESDATA;
	unsigned int BASEOFFSETKVS_DESTVERTICESDATA;
	unsigned int BASEOFFSETKVS_ACTIVEUPROPBLOCKS;
	unsigned int BASEOFFSETKVS_ACTIVEEDGEBLOCKS;
	unsigned int BASEOFFSETKVS_ACTIVEUPDATEBLOCKS;
	unsigned int BASEOFFSETKVS_EDGESMAP;
	unsigned int BASEOFFSETKVS_EDGEBLOCKMAP;
	unsigned int BASEOFFSETKVS_VERTICESPARTITIONMASK;
	unsigned int BASEOFFSETKVS_STATSDRAM;
	unsigned int BASEOFFSETKVS_EDGESSTATSDRAM;
	unsigned int BASEOFFSETKVS_VERTEXUPDATESDATA;
	unsigned int BASEOFFSETKVS_KVDRAM;
	unsigned int BASEOFFSETKVS_KVDRAMWORKSPACE;
	unsigned int BASEOFFSETKVS_OTHERINFOS;
	unsigned int BASEOFFSETKVS_ENDOFFILE;
	
	unsigned int BUFFERBASEOFFSETKVS_VDATA1;
	unsigned int BUFFERBASEOFFSETKVS_VDATA2;
	unsigned int BUFFERBASEOFFSETKVS_VDATA3;
	unsigned int BUFFERBASEOFFSETKVS_VDATA4;
	unsigned int BUFFERBASEOFFSETKVS_VDATA5;

	unsigned int SIZE_MESSAGESDRAM;
	unsigned int VOLUMESIZEU32_WORKDATA;
	unsigned int VOLUMESIZEU32_WORKSPACEDATA;
	unsigned int SIZE_EDGES;
	unsigned int SIZE_VERTEXPTRS;
	unsigned int SIZE_SRCVERTICESDATA;
	unsigned int SIZE_DESTVERTICESDATA;
	unsigned int SIZE_ACTIVEUPROPBLOCKS;
	unsigned int SIZE_ACTIVEEDGEBLOCKS;
	unsigned int SIZE_ACTIVEUPDATEBLOCKS;
	unsigned int SIZE_EDGESMAP;
	unsigned int SIZE_EDGEBLOCKMAP;
	unsigned int SIZE_VERTICESPARTITIONMASK;
	unsigned int SIZE_KVSTATSDRAM;
	unsigned int SIZE_EDGESSTATSDRAM;
	unsigned int SIZE_VERTEXUPDATESDATA;
	unsigned int SIZE_KVDRAM;
	unsigned int SIZE_KVDRAMWORKSPACE;
	unsigned int SIZE_OTHERINFOS;
	unsigned int SIZEKVS2_PROCESSEDGESPARTITION;
	unsigned int SIZE_REDUCE;
	unsigned int SIZEKVS2_REDUCEPARTITION;
	unsigned int SIZEKVS_VMASKBUFFER;
	unsigned int SIZE_BATCHRANGE;
	unsigned int SIZE_RUN;

	unsigned int POW_MESSAGESDRAM;
	unsigned int POW_EDGES;
	unsigned int POW_VERTEXPTRS;
	unsigned int POW_SRCVERTICESDATA;
	unsigned int POW_DESTVERTICESDATA;
	unsigned int POW_ACTIVEEDGEBLOCKS;
	unsigned int POW_VERTICESDATAMASK;
	unsigned int POW_KVSTATSDRAM;
	unsigned int POW_EDGESSTATSDRAM;
	unsigned int POW_VERTEXUPDATESDATA;
	unsigned int POW_KVDRAM;
	unsigned int POW_KVDRAMWORKSPACE;
	unsigned int POW_REDUCE;
	unsigned int POW_BATCHRANGE;
	
	unsigned int NUM_PROCESSEDGESPARTITIONS;
	unsigned int NUM_REDUCEPARTITIONS;

	unsigned int ALGORITHMINFO_GRAPHITERATIONID;
	unsigned int ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int ALGORITHMINFO_GRAPHALGORITHMCLASS; // computed online

	unsigned int ACTSPARAMS_BEGINLOP;
	unsigned int ACTSPARAMS_NUMLOPS;
	unsigned int ACTSPARAMS_TREEDEPTH;
	unsigned int ACTSPARAMS_FINALNUMPARTITIONS;
	unsigned int ACTSPARAMS_SRCVOFFSET;
	unsigned int ACTSPARAMS_SRCVSIZE;
	unsigned int ACTSPARAMS_DESTVOFFSET;
	unsigned int ACTSPARAMS_NUMEDGECHUNKSINABUFFER;
	unsigned int ACTSPARAMS_INSTID;
	unsigned int ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM;
	unsigned int ACTSPARAMS_NUM_PARTITIONS;
	unsigned int ACTSPARAMS_NUM_PARTITIONS_LOP1;
	unsigned int ACTSPARAMS_NUM_PARTITIONS_LOP2;
	unsigned int ACTSPARAMS_BATCH_RANGE;
	unsigned int ACTSPARAMS_POW_PARTITIONS;
	unsigned int ACTSPARAMS_POW_PARTITIONS_LOP1;
	unsigned int ACTSPARAMS_POW_PARTITIONS_LOP2;
	unsigned int ACTSPARAMS_POW_PARTITIONS_LOP3;
	unsigned int ACTSPARAMS_WORKBUFFER_SIZE;
	unsigned int ACTSPARAMS_MAXHBMCAPACITY_KVS;
	unsigned int ACTSPARAMS_VPTR_SHRINK_RATIO;
	unsigned int ACTSPARAMS_READOFFLINESTATS;
	unsigned int ACTSPARAMS_UNUSED4;
	unsigned int ACTSPARAMS_NUMCOMPUTEUNITS_SLR2;
	unsigned int ACTSPARAMS_NUMCOMPUTEUNITS_SLR1;
	unsigned int ACTSPARAMS_NUMCOMPUTEUNITS_SLR0;
	
	unsigned int DUMMYCHKPOINT;
	unsigned int BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM;
	unsigned int RETURNVALUES;
	unsigned int MAILBOX;

	unsigned int VARS_WORKBATCH;
} globalparams_t;

typedef struct {
	// string GRAPH_NAME;
	// string GRAPH_PATH;
	
	unsigned int ALGORITHM; 
	unsigned int NUM_ITERATIONS;
	unsigned int ROOTVID;
	
	unsigned int NUM_VERTICES;
	unsigned int NUM_EDGES;
	unsigned int AVERAGENUM_WORKEDGES_PER_CHANNEL;
	
	bool ISUNDIRECTEDGRAPH;
	
	unsigned int NUM_PARTITIONS_POW;
	unsigned int NUM_PARTITIONS;

	unsigned int OPT_PROC_SRAMSZ_POW;
	unsigned int PROC_SRAMSZ_POW;
	unsigned int PROC_SRAMSZ;
	
	unsigned int OPT_RED_SRAMSZ_POW;
	unsigned int RED_SRAMSZ_POW;
	unsigned int RED_SRAMSZ;

	unsigned int KVDATA_RANGE_POW;
	unsigned int KVDATA_RANGE;

	unsigned int BATCH_RANGE_POW;
	unsigned int BATCH_RANGE;
	unsigned int BATCH_RANGE_KVS;

	unsigned int TREE_DEPTH;

	unsigned int REDUCESZ_POW;
	unsigned int REDUCESZ;

	unsigned int REDUCEPARTITIONSZ;
	unsigned int REDUCEPARTITIONSZ_KVS2;
	unsigned int NUMREDUCEPARTITIONS;

	unsigned int PROCESSPARTITIONSZ;
	unsigned int SUB_CHUNK_HEIGHT;
	unsigned int PROCESSPARTITIONSZ_KVS2;
	unsigned int NUMPROCESSEDGESPARTITIONS;
	
	unsigned int NUMLASTLEVELPARTITIONS;

	unsigned int NUM_EDGE_BANKS;
	
	unsigned int VPTR_SHRINK_RATIO;

	unsigned int MAXHBMCAPACITY_UINT32; // (256MB/4=64M)
	unsigned int MAXHBMCAPACITY_KVS2; // (64M/16=4M)
	unsigned int KVSOURCEDRAMSZ; // MAXHBMCAPACITY_KV // max HBM capacity (256MB)
	unsigned int HBMWORKCAPACITY_UINT32;
	
	unsigned int KVSOURCEDRAMSZ_KVS;
	unsigned int PADDEDKVSOURCEDRAMSZ;
	unsigned int PADDEDKVSOURCEDRAMSZ_KVS;
	
	unsigned int VOLUMEOFFSETKVS_WORKDATA; 
	unsigned int VOLUMEOFFSETKVS_WORKSPACEDATA; 
	unsigned int VOLUMESIZEU32_WORKDATA; 
	unsigned int VOLUMESIZEU32_WORKSPACEDATA; 

	unsigned int VDRAMSZ;
	unsigned int PADDEDVDRAMSZ;
	unsigned int PADDEDVDRAMSZ_KVS;
	
	unsigned int DRAMPADD;
	unsigned int DRAMPADD_KVS;

	unsigned int DRAMPADD_VPTRS;
	unsigned int DRAMPADD_VPTRS_KVS;
	
	unsigned int SRCBUFFER_SIZE;
	unsigned int WORKBUFFER_SIZE;
	
	unsigned int EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM;
	
	// unsigned int HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION; //2 // 2*,8
	// unsigned int HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION; //2 // 1, 2*, 16, 32, 64
	// unsigned int HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION; //2 // 1, 2* // CONFIG_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION
} universalparams_t;

typedef struct {
	globalparams_t globalparamsK; 
	globalparams_t globalparamsE;
	globalparams_t globalparamsV;
	globalparams_t globalparamsM;
} globalparams_TWOt;

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

typedef struct {
	unsigned int offset_srcbase;
	unsigned int offset_dstbase;
	unsigned int offset_begin;
	unsigned int offset_end;
	unsigned int size;
	unsigned int offset_buffer_begin;
} workload_t;

typedef struct {
	unsigned int first;
	unsigned int last;
	unsigned int edgebankID;
	unsigned int v_chunkid;
	unsigned int stage;
	unsigned int laststage;
	unsigned int currentLOP;
	unsigned int lastLOP;
	unsigned int source_partition;
	unsigned int first_source_partition;
	unsigned int last_source_partition;
	unsigned int num_source_partitions;
	unsigned int sourcestatsmarker;
	unsigned int deststatsmarker;
	unsigned int num_active_edgeblocks;
	unsigned int num_active_updateblocks;
	unsigned int num_active_edges_in_channel;
	unsigned int EN_PROCESS; 
	unsigned int EN_PARTITION; 
	unsigned int EN_REDUCE;
	unsigned int EN_PROCESSANDREDUCE;
	unsigned int EN_PROCESSANDPARTITION;
} globalposition_t;

typedef struct {
	bool active;
	int beginoffset;
	int endoffset;
	int s_base;
	int local_rowoffset;
} sliceinfo_t;

typedef struct {
	sliceinfo_t slice[4];
} sliceinfos_t;

typedef struct {
	unsigned int incr;
	unsigned int dstvid;
} parsededge_t;

typedef struct {
	unsigned int srcvoffset[NUM_PEs];
	unsigned int srcvsize[NUM_PEs];
	unsigned int destvoffset[NUM_PEs];
	unsigned int actvvsize[NUM_PEs];
	unsigned int firstvid[NUM_PEs];
	unsigned int edgeoffset[NUM_PEs];
	unsigned int edgessize[NUM_PEs];
	unsigned int batchoffset[NUM_PEs];
	unsigned int batchsize[NUM_PEs];
	unsigned int runsize[NUM_PEs]; 
	unsigned int numedgesretrieved[1];
	unsigned int totalnumedgesinfile[1];
	edge_t * vertexptrs[NUM_PEs];
	edge2_type * edgesbuffer[NUM_PEs];
	edge_t tempvertexptrs[NUM_PEs][2];
	edge2_type tempedges[NUM_PEs][2];
} container_t;
#endif

