#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

// #define NACTS_IN_1COMPUTEUNIT
#define NACTS_IN_NCOMPUTEUNITS

// #define ENABLE_PERFECTACCURACY //
// #define ENABLE_VOICEOUTREDUCEERRORS //
// #define ENABLE_VOICEOUTKERNELERRORS //
// #define ENABLE_VOICEOUTPROCESSEDGESERRORS //

// #define ENABLE_APPROXIMATEPARTITIONWRITES // CRITICAL FIXME. FPGA hangs if this is disabled // REMOVEME.

// === parameters for ACTS design ===
// #define ENABLERECURSIVEPARTITIONING // found in: common_h, loadgraph, actscommon_h, actsproc_unopt

// #define ACTS_PARTITION_AND_REDUCE_STRETEGY // found in: actscommon_h
#define BASIC_PARTITION_AND_REDUCE_STRETEGY
#endif




