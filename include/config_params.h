#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

// susanna (john wesley's mother)

/** // special algorithm changes:
app.cpp 
mem_acces.cpp 
classname__top_usrcv_nudstv.cpp
reduceupdates.cpp
loadgraph.cpp 

*/

// #define ENABLE_PERFECTACCURACY //
// #define ENABLE_VOICEOUTREDUCEERRORS //
// #define ENABLE_VOICEOUTKERNELERRORS //
// #define ENABLE_VOICEOUTPROCESSEDGESERRORS //

// #define ENABLE_APPROXIMATEPARTITIONWRITES // CRITICAL FIXME. FPGA hangs if this is disabled // REMOVEME.

// === parameters for ACTS design (actscommon_h, goclkernel.cpp, app.cpp, actsproc.cpp, common_h, loadgraph) ===
#define ENABLERECURSIVEPARTITIONING // OBSOLETE.
#define ACTS_PARTITION_AND_REDUCE_STRETEGY

#define HWIMPLFOR_ACTSPROC
#define HWIMPLFOR_ACTSSYNC

#define USEHBMMEMORY
// #define USEDDRAMMEMORY

#define CONFIG_UNIFYSRCV // {app.cpp}
#define CONFIG_SPLIT_DESTVTXS

// #define CONFIG_READVDATA_SLIDE // access format for source vertices 
#define CONFIG_READVDATA_SLIDEANDREARRANGE

#define SEQUENTIALIZE_VIDS_IN_A_DRAM_ROW // uses MAX_NUM_UNIQ_EDGES_PER_VEC in {common.h}

// ********************************************** POWERFUL CONFIGS **********************************************
#define CONFIG_RELEASE_VERSION1
// #define CONFIG_RELEASE_VERSION2 // { (1) mergeVs fixed. }
#define CONFIG_RELEASE_VERSION4 // { (1) exchangeVs fixed. }

#define CONFIG_GOLDENKERNEL // { processedges.cpp, reduceupdates.cpp, algorithm.cpp, classname__top_usrcv_nudstv.cpp, mem_acces.cpp, trad_gp.cpp }
// #define CONFIG_ALL_EVALUATIONTYPES_IN_ONE_KERNEL // for evaluation { processedges2.cpp reduceupdates.cpp, acts.cpp, partitionupdates.cpp } // NB: Big Buffers here
#define CONFIG_INCLUDE_IMPL_WITH_TREEDEPTH_OF_1 // {actscommon.h, classname__top_usrcv_nudstv.cpp, acts_util.cpp, mem_access.cpp, config_params.h}
// ********************************************** POWERFUL CONFIGS **********************************************

#define CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS // NEW {classname__top_usrcv_nudstv.cpp}

#define CONFIG_INSERTSTATSMETADATAINEDGES // {acts.cpp, loadedges_random.cpp, classname__top_usrcv_nudstv.cpp, processedges2.cpp}

#define CONFIG_VDATAIS32BITSWIDE // NEW, FIXME. IMPLEMENT FOR ALL CASES.

#define CONFIG_ENABLEPROCESSMODULE
#define CONFIG_ENABLEPARTITIONMODULE
#define CONFIG_ENABLEREDUCEMODULE //
#define CONFIG_ENABLESYNCHRONIZEMODULE //

#define CONFIG_ENABLECLASS_ALGO_FUNCS
#define CONFIG_ENABLECLASS_ACTSUTILITY
#define CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
#define CONFIG_ENABLECLASS_PROCESSEDGES
#define CONFIG_ENABLECLASS_REDUCEUPDATES	
#define CONFIG_ENABLECLASS_PARTITIONUPDATES
#define CONFIG_ENABLECLASS_MEM_ACCESS
#define CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
#define CONFIG_ENABLECLASS_ACTS_UTIL
#define CONFIG_ENABLECLASS_ACTS
#define CONFIG_ENABLECLASS_ACTS_MERGE
#define CONFIG_ENABLECLASS_MYDEBUG

#define CONFIG_ACTSONLYMODE // (OBSOLETE)

// creating graphs 
// #define CONFIG_CREATEGRAPHS_CHECK_FOR_NONINCREASINGEDGES

// #define CONFIG_SKELETAL_ALGORITHM // {loadgraph.cpp, swkernel.cpp, goclkernel.cpp, classname__top_usrcv_nudstv.cpp, mem_acces.cpp} // FIXME.
// #define CONFIG_COLLECT_DATAS1_DURING_RUN // { data collected in processedges.cpp & reduceupdates.cpp }
#define CONFIG_COLLECT_DATAS2_DURING_RUN // {loops entered, number of vptrs across source partitions, number of edges across source partitions}
#define CONFIG_COLLECT_DATAS3_DURING_RUN // { data collected in actsutility.cpp }

#define CONFIG_PROCESSEDGES_READOFFLINESTATS // NEWCHANGE.

// #define CONFIG_FORBFS_REPRESENTVDATASASBITS // { common.h } // NEWCHANGE.
// #define CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP // {mem_access.cpp, load_edges.cpp, processedges.cpp} // NEWCHANGE. ///////////////////////////////////////////
// #define CONFIG_MEMACCESS_COMBINEMULTIPLEVPROPREADS // {classname__top_usrcv_nudstv.cpp, processedges.cpp}

#define CONFIG_SELECTIVEVERTEXPARTITIONS // OBSOLETE.
#define CONFIG_PRELOADEDVERTEXPARTITIONMASKS

#define CONFIG_FORCEDCORRECT_LOADEDGES
#define CONFIG_FORCEDCORRECT_TRADGP

#define CONFIG_ENABLE_PROFILING

#define CONFIG_HYBRIDGPMODE_MAXVTHRESHOLD (1 << 12)
#define CONFIG_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION (1 << 6)
#define CONFIG_HYBRIDGPMODE_MDRAMSECTIONSZ (1 << 14)

// #define CONFIG_SEPERATESRCVDESTVBUFFER
#define CONFIG_USEURAM_FOR_SRCVBUFFER // CRITICAL REMOVEME!
#define CONFIG_USEURAM_FOR_DESTVBUFFER // { classname__mem_access.cpp, classname__reduceupdates.cpp }
#define CONFIG_USEURAM_PACKBYTWOS

// #define CONFIG_BITMASK_NOT_USED // { classname__mem_convert_and_access.cpp }

#endif

/**
CHANGES:
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#define OPT_SRAMSZ_POW 10  (formerly ON)
#define CONFIG_ACTS_MEMORYLAYOUT switched OFF  (formerly ON)
#define CONFIG_ACTS_HYBRIDLOGIC switched OFF  (formerly ON)
#define CONFIG_GOLDENKERNEL switched OFF  (formerly ON)
#define CONFIG_FORBFS_REPRESENTVDATASASBITS switched OFF  (formerly ON)
#define CONFIG_SKELETAL_ALGORITHM switched OFF  (formerly ON)

_WIDEWORD_FOR_KV

reduceupdates.cpp: #pragma HLS dependence variable=vbuffer inter false // NEWCHANGE.

_TRADreadandprocess used (change before synthesize)

CONFIG_USEURAM_FOR_SRCVBUFFER
PROCESS{{context['id']}}_readedgeblockstats. #ifdef CONFIG_PROCESSEDGES_READOFFLINESTATS

// CRITICAL FIXMEs:
maxdramsz = tempstats[0][totalnumpb4llop-1].key + tempstats[0][totalnumpb4llop-1].value; // CRITICAL FIXME.
*/



