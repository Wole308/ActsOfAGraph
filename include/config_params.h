#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

// #define NACTS_IN_1COMPUTEUNIT
#define NACTS_IN_NCOMPUTEUNITS

#define ACTSFAST
#define MAXPERFORMANCE
#define SIMPLEANDFASTPREPAREFUNC

// #define ENABLE_LOADKVDRAM
// #define ENABLE_REUSESAVEDCAPSULES
#define ENABLE_PERFECTACCURACY //
#define ENABLE_VOICEOUTREDUCEERRORS //
#define ENABLE_VOICEOUTKERNELERRORS //
// #define ENABLE_VOICEOUTPROCESSEDGESERRORS //

#define FPGABUGCOMPLANT
#define ENABLE_APPROXIMATEPARTITIONWRITES // CRITICAL FIXME. FPGA hangs if this is disabled
#define HBMBUGCOMPLANT

#define IGNOREHBMMAXCAPACITY

// #define ACCESSFPGABY_ENQUEUEWRITEBUFFER // FIXME. causing extra latency // FIXME. would cause FPGA inaccuracies now if enabled

#define HARDWIREBY_TREEDEPTH // DEPRECIATED.
#endif




