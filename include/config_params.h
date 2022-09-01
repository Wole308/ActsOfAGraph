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

#define CONFIG_SETUP_SUITABLE_FOR_LARGE_GRAPHS // actit2 is used instead of actit

#define HWIMPLFOR_ACTSPROC
#define HWIMPLFOR_ACTSSYNC

#define USEHBMMEMORY

#define CONFIG_UNIFYSRCV // {app.cpp}
#define CONFIG_SPLIT_DESTVTXS

#define CONFIG_UPDATEPACKINGx16
#define CONFIG_SEND_LOCAL_VERTEXIDS_ONLY

#define SEQUENTIALIZE_VIDS_IN_A_DRAM_ROW // uses MAX_NUM_UNIQ_EDGES_PER_VEC in {common.h}

// ********************************************** POWERFUL CONFIGS **********************************************
#define CONFIG_RELEASE_VERSION1
#define CONFIG_RELEASE_VERSION4 // { (1) exchangeVs fixed. }
#define CONFIG_RELEASE_FULLKERNEL // { (1) full kernel run fixed. }
#define CONFIG_RELEASE_VERSION6 // readV, saveV
#define CONFIG_RELEASE_VERSION7 // mask included
// #define CONFIG_RELEASE_VERSION_DUPLICATEEDGES 





// #ifdef RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR //////////////////////////////////////////////////
// >>> top_usrcv_nudstv.cpp <<<
// #define TOP_ENABLE_SHRINKVBUFFER // { loadgraph.cpp }
#define TOP_ENABLE_MERGEVS // CRITICAL FIXME NOW. *
#define TOP_ENABLE_BROADCASTUPROPS // CRITICAL FIXME NOW ***
// #define TOP_ENABLE_USEURAMS // CRITICAL FIXME NOW.

// >>> mem_acces.cpp <<< 
#define MEMACCESS_ENABLE_COLLECTSTATSFORHOST
#define MEMACCESS_ENABLE_ATOMICREDUCE // CRITICAL FIXME NOW.
#define MEMACCESS_ENABLE_CYCLICSHIFTS 
// #define MEMACCESS_ENABLE_USEHLSSTREAM
#define MEMACCESS_ENABLE_SPARSEEDGEBLOCKS // CRITICAL FIXME NOW.
#define MEMACCESS_ENABLE_SEPERATEINTERFACEFORDRAMREADS
#define MEMACCESS_ENABLE_SEPERATEINTERFACEFORDRAMWRITES
#define MEMACCESS_ENABLE_SEPERATEINTERFACEFORMISCREADSANDWRITES

// >>> acts.cpp <<<
#define ACTS_ENABLE_READUPROPS // CRITICAL FIXME NOW ***.
#define ACTS_ENABLE_PROCESSEDGES
#define ACTS_ENABLE_REDUCE
#define ACTS_ENABLE_COLLECTSTATS // CRITICAL FIXME NOW.
#define ACTS_ENABLE_COLLECTHOSTSTATS

// >>> algo_funcs.cpp <<<
#define ALGOFUNCS_ENABLE_ALLALGORITHMS 
// #endif ////////////////////////////////////////////////// //////////////////////////////////////////////////





// #define CONFIG_ENABLE___SPARSEUPDATEBUFFER___ 

#define CONFIG_GOLDENKERNEL // { processedges.cpp, reduceupdates.cpp, algorithm.cpp, classname__top_usrcv_nudstv.cpp, mem_acces.cpp, trad_gp.cpp }

#define CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS // NEW {classname__top_usrcv_nudstv.cpp}

#define CONFIG_ENABLEPROCESSMODULE
// #define CONFIG_ENABLEPARTITIONMODULE
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

#define CONFIG_COLLECT_DATAS2_DURING_RUN // {loops entered, number of vptrs across source partitions, number of edges across source partitions}
#define CONFIG_COLLECT_DATAS3_DURING_RUN // { data collected in actsutility.cpp }

#define CONFIG_PROCESSEDGES_READOFFLINESTATS // NEWCHANGE.

#define CONFIG_SELECTIVEVERTEXPARTITIONS // OBSOLETE.
#define CONFIG_PRELOADEDVERTEXPARTITIONMASKS
// #define CONFIG_PRELOADEDVERTEXMASKS /////////////////////////////////// FIXME?

#define CONFIG_FORCEDCORRECT_LOADEDGES
#define CONFIG_FORCEDCORRECT_TRADGP

#define CONFIG_ENABLE_PROFILING

#define CONFIG_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION 0 // 2*,8
#define CONFIG_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION 0 // 1, 2*, 16, 32, 64
#define CONFIG_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION 0 // 1, 2* // CONFIG_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION

#define CONFIG_USEURAM_FOR_SRCVBUFFER // CRITICAL REMOVEME!
#define CONFIG_USEURAM_FOR_DESTVBUFFER // { classname__mem_access.cpp, classname__reduceupdates.cpp }

#define CONFIG_EDGEHASHSCHEME_SETVIDS // { acts_util.cpp, load_edges.cpp, utility.cpp }

#endif

/*
NEWCHANGES:
MEMACCESS{{context['id']}}_write__process_base: removed 'if(mode == ACTSPROCESSMODE){ load(kvdram, c0, workload_kvs); }'

*/





