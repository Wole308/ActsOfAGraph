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

#endif

/*
- AUTOMATE FOR LARGER GRAPHS: MERGE_mergeVs, mem_access.cpp
- CRITICAL FIXME. mem_access.cpp
- CRITICAL FIXME. TOO EXPENSIVE. mem_access.cpp
- CRITICAL OPTIMIZEME: acts_merge.cpp
- TOO EXPENSIVE. top_nusrcv_nudstv.cpp


*/



