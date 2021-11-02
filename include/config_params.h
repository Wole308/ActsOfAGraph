#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

/** CRITICAL NOTES
	DEFAULT SETTINGS: ENABLERECURSIVEPARTITIONING, HWIMPLFOR_ACTSPROC, USEHBMMEMORY, NUMSYNCTHREADS(gen.py)=NUMSUBCPUTHREADS
	Num_procs: 16 worked
	
	// ACTS_XbyYbyZbyM: X: num kernels, Y: num compute units, Z: num HBM channels per compute unit, M: num HBM channels paired in one HBM channel
	ACTS_XbyYbyZbyM: X: num kernels, Y: num compute units, Z: num KV HBM channels per compute unit, M: num Edge HBM channels per compute unit
*/

// #define ACTS_1by1 //
// #define ACTSPROC_1by1 //
// #define ACTSPROC_2by1
// #define ACTSPROC_4by1 //
// #define ACTSPROC_8by1

// #define ACTS_1by1by1
// #define ACTS_1by2by1
// #define ACTS_2by1by1

// #define ACTS_1by2by1by0
#define ACTS_1by2by1byN

// #define ENABLE_PERFECTACCURACY //
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

#define HWIMPLFOR_ACTSPROC
#define HWIMPLFOR_ACTSSYNC

#define USEHBMMEMORY
// #define USEDDRAMMEMORY

#endif




