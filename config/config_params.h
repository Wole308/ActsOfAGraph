#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

// #define _GENERATE2DGRAPH //
// #define _GENERATEACTIVEVERTICES
// #define _GENERATESMARTGRAPH

#define RUNALLITERATIONS
#define _EVENWORLOADDISTRIBUTION
#define LAUNCHKERNEL
#define LOADKVDRAMS

#define STREAMEDGESSETUP

// #define FORCEDFINISH_DONTCAREABOUTRETRIEVEBITS // correct this. its making BFS too slow
// #define FORCEDFINISH_DONTCAREABOUTISVERTEXACTIVE
#define FORCEDFINISH_DONTCAREABOUTACTIVEVERTICESGENERATED

// #define ENABLE_LOADKVDRAM
// #define ENABLE_REUSESAVEDCAPSULES
#define ENABLE_APPROXIMATEPARTITIONWRITES // FPGA hangs if this is disabled
// #define ENABLE_PERFECTACCURACY
#define ENABLE_VOICEOUTREDUCEERRORS //
#endif




