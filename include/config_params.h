#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

/** CRITICAL NOTES
	DEFAULT SETTINGS: ENABLERECURSIVEPARTITIONING, HWIMPLFOR_ACTSPROC, USEHBMMEMORY, NUMSYNCTHREADS(gen.py)=NUMSUBCPUTHREADS
	Num_procs: 16 worked
	
	ACTS_XbyYbyZbyM: X: num kernels, Y: num compute units (1: everything packed in one, 2: 2 CUs in one kernel), Z: num KV HBM channels per compute unit, M: num Edge HBM channels per compute unit
*/

// #define ACTS_1by2by1byN
#define ACTS_1by1by1byN // (everything merged in one compute unit)

#define ENABLE_PERFECTACCURACY //
// #define ENABLE_VOICEOUTREDUCEERRORS //
// #define ENABLE_VOICEOUTKERNELERRORS //
// #define ENABLE_VOICEOUTPROCESSEDGESERRORS //

// #define ENABLE_APPROXIMATEPARTITIONWRITES // CRITICAL FIXME. FPGA hangs if this is disabled // REMOVEME.

// === parameters for ACTS design (actscommon_h, goclkernel.cpp, app.cpp, actsproc.cpp, common_h, loadgraph) ===
#define ENABLERECURSIVEPARTITIONING
#ifdef ENABLERECURSIVEPARTITIONING
#define ACTS_PARTITION_AND_REDUCE_STRETEGY
#else
// #define BASIC_PARTITION_AND_REDUCE_STRETEGY
#define TRAD_PARTITION_AND_REDUCE_STRETEGY
#endif

// #define DATA_SENSITIVE_ACTS //

#define HWIMPLFOR_ACTSPROC
#define HWIMPLFOR_ACTSSYNC

#define USEHBMMEMORY
// #define USEDDRAMMEMORY

#define CONFIG_SPLIT_DESTVTXS // {reduceupdates.cpp, acts_util.cpp, app.cpp, loadgraph.cpp, top_nusrcv_nudstv.cpp, reduceupdates.cpp, actscommon.h}
#define CONFIG_UNIFYSRCV // {app.cpp}

// #define CONFIG_READVDATA_SLIDE // access format for source vertices 
#define CONFIG_READVDATA_SLIDEANDREARRANGE

// #define CONFIG_SEPERATEVMASKFROMVDATA // seperating vmask and vbuffer datas {loadedges_splitdstvxs.cpp, processedges_splitdstvxs.cpp, reduceupdates.cpp, acts_merge_splitdstvxs.cpp, actscommon.h, common.h}

#define CUSTOMLOGICFOREACHALGORITHM // {processedges_splitdstvxs.cpp, reduceupdates.cpp}

#define CONFIG_ENABLEPROCESSMODULE
#define CONFIG_ENABLEPARTITIONMODULE
#define CONFIG_ENABLEREDUCEMODULE //
#define CONFIG_ENABLESYNCHRONIZEMODULE //

#define CONFIG_ENABLECLASS_ACTSUTILITY
#ifdef CONFIG_UNIFYSRCV
#define CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
#else 
#define CONFIG_ENABLECLASS_TOPNUSRCV_NUDSTV
#endif
#define CONFIG_ENABLECLASS_PROCESSEDGES_SPLITDSTVXS
#define CONFIG_ENABLECLASS_PARTITIONUPDATES
#define CONFIG_ENABLECLASS_REDUCEUPDATES
#define CONFIG_ENABLECLASS_MEM_ACCESS_SPLITDSTVXS
#define CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
#define CONFIG_ENABLECLASS_ACTS_UTIL
#define CONFIG_ENABLECLASS_ACTS
#define CONFIG_ENABLECLASS_ACTS_MERGE_SPLITDSTVXS
#define CONFIG_ENABLECLASS_MYDEBUG

#define TESTRUN /////////////////// CRITICAL REMOVEME.

#endif

/**
- AUTOMATE FOR LARGER GRAPHS: mem_access.cpp, acts_merge.cpp, mem_access.cpp->MEMACCESS_readvmasks
- CRITICAL FIXME. mem_access.cpp
- CRITICAL FIXME. TOO EXPENSIVE. mem_access.cpp, processedges_splitdstvxs.cpp MEMACCESS_SPL_GetXYLayoutV((i*NUM_PEs)+s, vdata, vdata2, depths); // FIXME. TOO EXPENSIVE.
- CRITICAL OPTIMIZEME: acts_merge.cpp
- TOO EXPENSIVE. top_nusrcv_nudstv.cpp, processedges_slicedgraph.cpp, acts_merge.cpp
- FIXME FOR DIFFERENT PARAMETERS: loadedges_splitdstvxs.cpp
- NOT IMPLEMENTED. mem_access.cpp

- // CRITICAL FIXME.
- acts.cpp : #ifdef _DEBUGMODE_CHECKS2 // CRITICAL CHECKME.
	if(maxcutoff % 4 != 0){ for(int v=0; v<VECTOR_SIZE; v++){ cout<<"------------------- actspipeline:cutoffs["<<v<<"]: "<<cutoffs[v]<<endl; } exit(EXIT_FAILURE); }
	actsutilityobj->checkfordivisibleby(ON, "actspipeline:maxcutoff", maxcutoff, 4);
	#endif
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	if(maxcutoff % 4 != 0){ maxcutoff = 4; } // FIXME.
	#endif 
- #ifdef _WIDEWORD // CRITICAL FIXME. @ {{context['classname__mem_access_splitdstvxs']}}MEMACCESS_SPL_readVchunks
- #ifndef _WIDEWORD  // CRITICAL FIXME. @ {{context['classname__mem_access_splitdstvxs']}}MEMACCESS_SPL_SLreadV
- // CRITICAL FIXME @ {{context['classname__mem_convert_and_access']}}MEMCA_CREATEVBUFFERSTRUCT
- reduceupdates.cpp: if(en == true){ curr_vprop = loc + destoffset; } // CRITICAL REMOVEME.	
- NEWCHANGE: laodedges_splitdsxtvs: if(index%VECTOR2_SIZE == 0 && edge.dstvid == 888888){ numskippededges += 1; tempe_index += 1; continue; }
- config.cpp: #define TESTRUN /////////////////// CRITICAL REMOVEME.
- CRITICAL REMOVEME: /* cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
- CRITICAL REMOVEME: void {{context['classname__top_usrcv_udstv']}}TOP{{context['id']}}_U_topkernelP{{n}}: // num_stages = 1; // REMOVEME.
- NEWCHANGE: processit: if(voffset_kvs == avtravstate.begin_kvs || globalposition.source_partition == globalposition.first_source_partition){ resetenv = ON; } else { resetenv = OFF; } // NEWCHANGE
- CRITICAL REMOVEME: actscommon.h: #define NUMPIPELINES_PARTITIONUPDATES 1//2 
- NEWCHANGE: top: batch_type vreadoffset_kvs2 = 0; // NEWCHANGE.
*/


/* *** IMPORTANT CHANGES ***
#define CONFIG_SEPERATEVMASKFROMVDATA 
evaluate.sh: for num_pes in $NUM_PEs_EQ24
context['PE_SETSZ'] = 8 #8, 16 # for classname__processedges_splitdstvxs.cpp
#define MAX_NUM_UNIQ_EDGES_PER_VEC 16
// topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID  (REMOVEME)
// if(GraphIter > 0){ {%if(context['XWARE']=="SW")%}acts_merge_splitdstvxsobj->{%e
void {{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATA0ANDVMASK0(k
void {{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATA1ANDVMASK1(ke

*/



