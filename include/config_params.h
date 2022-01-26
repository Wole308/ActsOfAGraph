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

// #define CONFIG_UNIFIED_VDRAM //
// #define CONFIG_UNIFIED_SRCVDRAM
// #define CONFIG_UNIFIED_DESTVDRAM
#define CONFIG_SPLIT_DESTVTXS // {reduceupdates.cpp, acts_util.cpp, app.cpp, loadgraph.cpp, top_nusrcv_nudstv.cpp, reduceupdates.h, actscommon.h}

#define CONFIG_ACTSPROCESSEDGES_SPREADVTXREAD
// #define CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II1
#define CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II2
#define CONFIG_ACTSPROCESSEDGES_REARRANGEINREADTREAD
#define CONFIG_READVDATA_SLIDEANDREARRANGE

#define CONFIG_ENABLEPROCESSMODULE
#define CONFIG_ENABLEPARTITIONMODULE
#define CONFIG_ENABLEREDUCEMODULE //
#define CONFIG_ENABLESYNCHRONIZEMODULE //

#ifdef CONFIG_SPLIT_DESTVTXS
#define CONFIG_COLLECTMASKINFOSDURINGREDUCE
#endif 

#define CONFIG_ENABLECLASS_ACTSUTILITY
// #define CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
#define CONFIG_ENABLECLASS_TOPNUSRCV_NUDSTV
#define CONFIG_ENABLECLASS_PROCESSEDGESU
#define CONFIG_ENABLECLASS_PROCESSEDGES_SPLITDSTVXS
#define CONFIG_ENABLECLASS_PARTITIONUPDATES
#define CONFIG_ENABLECLASS_REDUCEUPDATES
#define CONFIG_ENABLECLASS_MEM_ACCESS
#define CONFIG_ENABLECLASS_MEM_ACCESS_SPLITDSTVXS
#define CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
#define CONFIG_ENABLECLASS_ACTS_UTIL
#define CONFIG_ENABLECLASS_ACTS
// #define CONFIG_ENABLECLASS_ACTS_MERGE
#define CONFIG_ENABLECLASS_ACTS_MERGE_SPLITDSTVXS
#define CONFIG_ENABLECLASS_MYDEBUG

#endif

/*
- AUTOMATE FOR LARGER GRAPHS: mem_access.cpp, acts_merge.cpp, mem_access.cpp->MEMACCESS_readvmasks
- CRITICAL FIXME. mem_access.cpp
- CRITICAL FIXME. TOO EXPENSIVE. mem_access.cpp, processedges_splitdstvxs.cpp MEMACCESS_SPL_GetXYLayoutV((i*NUM_PEs)+s, vdata, vdata2, depths); // FIXME. TOO EXPENSIVE.
- CRITICAL OPTIMIZEME: acts_merge.cpp
- TOO EXPENSIVE. top_nusrcv_nudstv.cpp, processedges_slicedgraph.cpp, acts_merge.cpp
- FIXME FOR DIFFERENT PARAMETERS: loadedges_splitdstvxs.cpp
- NOT IMPLEMENTED. mem_access.cpp

*/


/*
classname__top_nusrcv_nudstv.unit1_type: use data packing throughout instead to conserve BRAM

fetchmessage_t {{context['classname__acts']}}ACTS_fetchkeyvalues -> {%if(context['XWARE']=="SW")%}processedgesobj->{%endif%}PROCESS_SPL2_readandprocess
void {{context['classname__mem_access_splitdstvxs']}}MEMACCESS_SPL_readvdata_slide
context['MQ_seq'] = []
for i in range (0,8):
		context['MQ_seq'].append(i)
// if(GraphAlgo == PAGERANK){
		// res = udata;
	// } else if(GraphAlgo == BFS){
		// res = NAp;
	// } else if(GraphAlgo == SSSP){
		// res = udata + edgew;
	// } else {
		// res = NAp;
	// } 
#define CONFIG_READVDATA_SLIDEANDREARRANGE
MEMACCESS_SPL_readvdata_slide(enable, s, kvdram, buffer, vbaseoffset_kvs, depth_i + voffset_kvs, bdepth_i, vsz_kvs, globalparams); // CRITICAL FIXME.
READANDPROCESS_SPL_LOOP3C: for (buffer_type i=0; i<4; i++){ // CRITICAL REMOVEME.
*/



