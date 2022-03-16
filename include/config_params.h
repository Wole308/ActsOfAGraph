#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

/** CRITICAL NOTES
	DEFAULT SETTINGS: ENABLERECURSIVEPARTITIONING, HWIMPLFOR_ACTSPROC, USEHBMMEMORY, NUMSYNCTHREADS(gen.py)=NUMSUBCPUTHREADS
	Num_procs: 16 worked
	
	ACTS_XbyYbyZbyM: X: num kernels, Y: num compute units (1: everything packed in one, 2: 2 CUs in one kernel), Z: num KV HBM channels per compute unit, M: num Edge HBM channels per compute unit
*/

// #define ACTS_1by2by1byN
#define ACTS_1by1by1byN // (everything merged in one compute unit)

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
// #define BASIC_PARTITION_AND_REDUCE_STRETEGY // {acts.cpp}
#define TRAD_PARTITION_AND_REDUCE_STRETEGY
#endif

#define HWIMPLFOR_ACTSPROC
#define HWIMPLFOR_ACTSSYNC

#define USEHBMMEMORY
// #define USEDDRAMMEMORY

// #define CONFIG_LASTWORKINGVERSION /////////////////////////////// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ // USe this when synthesizing and running FPGA_IMPL mode (FIXME.)

////////////////////////////////////////////////////////////////////////////////////
#define CONFIG_SPLIT_DESTVTXS // [OBSOLETE] // {reduceupdates.cpp, acts_util.cpp, app.cpp, loadgraph.cpp, top_nusrcv_nudstv.cpp, reduceupdates.cpp, actscommon.h}
// #define CONFIG_SEPERATEVMASKFROMVDATA // [OBSOLETE] // seperating vmask and vbuffer datas {loadedges_sequential.cpp, processedges.cpp, reduceupdates.cpp, acts_merge.cpp, actscommon.h, common.h}
////////////////////////////////////////////////////////////////////////////////////

#define CONFIG_UNIFYSRCV // {app.cpp}

// #define CONFIG_READVDATA_SLIDE // access format for source vertices 
#define CONFIG_READVDATA_SLIDEANDREARRANGE

#define SEQUENTIALIZE_VIDS_IN_A_DRAM_ROW // uses MAX_NUM_UNIQ_EDGES_PER_VEC in {common.h}

#define CUSTOMLOGICFOREACHALGORITHM // {processedges.cpp, reduceupdates.cpp}

#define CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS // NEW {classname__top_usrcv_udstv.cpp}

#ifndef CONFIG_LASTWORKINGVERSION
#define CONFIG_INSERTSTATSMETADATAINEDGES /////////////////////////////////////////// {acts.cpp, loadedges_random.cpp, classname__top_usrcv_udstv.cpp, processedges2.cpp}
#endif 

#define CONFIG_VDATAIS32BITSWIDE // NEW, FIXME. IMPLEMENT FOR ALL CASES.

#define CONFIG_ENABLEPROCESSMODULE
#define CONFIG_ENABLEPARTITIONMODULE
#define CONFIG_ENABLEREDUCEMODULE //
#define CONFIG_ENABLESYNCHRONIZEMODULE //

#define CONFIG_ENABLECLASS_ALGO_FUNCS
#define CONFIG_ENABLECLASS_ACTSUTILITY
#define CONFIG_ENABLECLASS_TOP_USRCV_UDSTV

#ifdef CONFIG_LASTWORKINGVERSION
#define CONFIG_ENABLECLASS_PROCESSEDGES2 /////////////////////////////////////////// FPGA IMPL
#define CONFIG_ENABLECLASS_REDUCEUPDATES2
#else 
#define CONFIG_ENABLECLASS_PROCESSEDGES	// SW
#define CONFIG_ENABLECLASS_REDUCEUPDATES	
#endif 

#define CONFIG_ENABLECLASS_PARTITIONUPDATES
#define CONFIG_ENABLECLASS_MEM_ACCESS
#define CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
#define CONFIG_ENABLECLASS_ACTS_UTIL
#define CONFIG_ENABLECLASS_ACTS
#define CONFIG_ENABLECLASS_ACTS_MERGE
#define CONFIG_ENABLECLASS_MYDEBUG

// creating graphs 
// #define CONFIG_CREATEGRAPHS_CHECK_FOR_NONINCREASINGEDGES

#endif

/**
CHANGES:
--- {config_params.h}
#define CONFIG_INSERTSTATSMETADATAINEDGES

// #define CONFIG_ENABLECLASS_REDUCEUPDATES
#define CONFIG_ENABLECLASS_REDUCEUPDATES2

--- {evaluate.sh}
TESTKERNEL="RK"
# TESTKERNEL="TESTKERNEL"
TESTKERNELARG=1

--- {acts_merge.cpp}
if(globalstatsbuffer{{i}}[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // {acts_merge.cpp} REMOVEME 'true'
#ifdef ENABLERECURSIVEPARTITIONING // {common.h}
	#ifdef FPGA_IMPL
	#define SRAMSZ_POW 9//10 // 1024
	#else 
	#define SRAMSZ_POW 10
	#endif 
#else
	#define SRAMSZ_POW 14 // 16384
#endif

--- {acts_merge.cpp}
#ifndef FPGA_IMPL
	MERGE{{context['id']}}_mergeVs{{n}}({%for i in context['T_seq']%}{%if(i<n)%}kvdram{{i}},{%endif%}{%endfor%} vdram, 
		{%for i in context['T_seq']%}{%if(i<n)%}globalstatsbuffer{{i}},{%endif%}{%endfor%} {%for i in context['T_seq']%}{%if(i<n)%}pmask{{i}}_next,{%endif%}{%endfor%} 
			globalparamsK, globalparamsV);
	#endif 
	
--- {common.h}	
#ifdef FPGA_IMPL
#define PROCESSPARTITIONSZ (SRAMSZ * VDATA_PACKINGSIZE) // 16384 // 16384
#else 
#define PROCESSPARTITIONSZ ((SRAMSZ / NUM_PEs) * NUM_PEs * VDATA_PACKINGSIZE) // 16128
#endif 

--- {acts_merge.cpp}
if(statssizes_kvs[0][partitionoffseti] > 0 || true){ // CRITICAL REMOVEME. true. 
#ifdef FPGA_IMPL // CRITICAL FIXME.
	bool exchangeall = false;
	#else 
	bool exchangeall = true;
	#endif 

--- {processedges2.cpp}
#ifdef FPGA_IMPL
#define PROCESSEDGES_TYPE2
#else 
#define PROCESSEDGES_TYPE1
#endif 
#ifdef PROCESSEDGES_TYPE1
	#pragma HLS PIPELINE II=2
	#endif 
	#ifdef PROCESSEDGES_TYPE2
	#pragma HLS INLINE
	#endif 
	
if(tempbuffer[NUM_PARTITIONS-1][0] != 8888888 || sum_values > chunk_size * VECTOR2_SIZE){
		#if defined(_DEBUGMODE_CHECKS) && defined(CONFIG_INSERTSTATSMETADATAINEDGES)
	
if(en == true && vmdata.vmask == 1 && *loadcount < SOURCEBLOCKRAM_SIZE){ *loadcount += 1; } // CRITICAL FIXME.
if(en == true && vmdata.vmask == 1 && *loadcount < WORKBUFFER_SIZE-2){ *loadcount += 1; } // CRITICAL FIXME.

--- {loadedges_random.cpp}
if(edge.dstvid >= KVDATA_RANGE){ edge.dstvid = edge.dstvid % KVDATA_RANGE; } // CRIICAL FIXME.

#ifdef FPGA_IMPL
#define LOADSLICEDEDGES_INCORRECT
#else 
#define LOADSLICEDEDGES_CORRECT	
#endif 
*/



