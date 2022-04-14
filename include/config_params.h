#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

/** CRITICAL NOTES
	DEFAULT SETTINGS: CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC, HWIMPLFOR_ACTSPROC, USEHBMMEMORY, NUMSYNCTHREADS(gen.py)=NUMSUBCPUTHREADS
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
#define ENABLERECURSIVEPARTITIONING // OBSOLETE.
#define ACTS_PARTITION_AND_REDUCE_STRETEGY

#define HWIMPLFOR_ACTSPROC
#define HWIMPLFOR_ACTSSYNC

#define USEHBMMEMORY
// #define USEDDRAMMEMORY

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

#define CONFIG_INSERTSTATSMETADATAINEDGES // {acts.cpp, loadedges_random.cpp, classname__top_usrcv_udstv.cpp, processedges2.cpp}

#define CONFIG_VDATAIS32BITSWIDE // NEW, FIXME. IMPLEMENT FOR ALL CASES.

#define CONFIG_ENABLEPROCESSMODULE
#define CONFIG_ENABLEPARTITIONMODULE
#define CONFIG_ENABLEREDUCEMODULE //
#define CONFIG_ENABLESYNCHRONIZEMODULE //

#define CONFIG_ENABLECLASS_ALGO_FUNCS
#define CONFIG_ENABLECLASS_ACTSUTILITY
#define CONFIG_ENABLECLASS_TOP_USRCV_UDSTV

// #define CONFIG_ENABLECLASS_PROCESSEDGES	// More LUTs, Faster?
// #define CONFIG_ENABLECLASS_REDUCEUPDATES	
#define CONFIG_ENABLECLASS_PROCESSEDGES2 // Fewer LUTs, Slower (use this for FPGA_IMPL)***
#define CONFIG_ENABLECLASS_REDUCEUPDATES2

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

#define CONFIG_SKELETAL_ALGORITHM // {loadgraph.cpp, swkernel.cpp, goclkernel.cpp, classname__top_usrcv_udstv.cpp, mem_acces.cpp} // FIXME.
// #define CONFIG_COLLECT_DATAS1_DURING_RUN // { data collected in processedges.cpp & reduceupdates.cpp }
#define CONFIG_COLLECT_DATAS2_DURING_RUN // {loops entered, number of vptrs across source partitions, number of edges across source partitions}
#define CONFIG_COLLECT_DATAS3_DURING_RUN // { data collected in actsutility.cpp }

// #define CONFIG_PROCESSEDGES_READOFFLINESTATS // FIXME.

#define CONFIG_FORBFS_REPRESENTVDATASASBITS // { common.h }

#define CONFIG_SELECTIVEVERTEXPARTITIONS // OBSOLETE.
#define CONFIG_PRELOADEDVERTEXPARTITIONMASKS
#define CONFIG_PRELOADEDVERTICESMASKS // { utility.cpp classname__top_usrcv_udstv.cpp, acts_merge.cpp}

#define CONFIG_FORCEDCORRECT_LOADEDGES
#define CONFIG_FORCEDCORRECT_TRADGP

#define CONFIG_INCLUDE_IMPL_WITH_TREEDEPTH_OF_1 // {actscommon.h, classname__top_usrcv_udstv.cpp, acts_util.cpp, mem_access.cpp, config_params.h}

#endif

/**
CHANGES:
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// #define CONFIG_PROCESSEDGES_READOFFLINESTATS
typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int vector_type;
typedef unsigned int step_type;
typedef unsigned int bool_type;
typedef unsigned int analysis_type;
typedef unsigned int visitstate_type; // actscommon.h
#ifndef ALLVERTEXISACTIVE_ALGORITHM
	if(pmask_curr[globalposition.source_partition / BRAM_BIT_WIDTH].data[globalposition.source_partition % BRAM_BIT_WIDTH] == 0){ returncmd = true; } // check if vertex partition is active
	else{
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; 
		#endif 
	}
	#endif 
#define CONFIG_COLLECT_DATAS2_DURING_RUN
// if(returncmd == false){ // CRITICAL REMOVEME...............................................................................................
// if(en_partition == ON){ TOP{{context['id']}}_NU_partitionit( kvdram, sourcebuffer, vbuffer, globalparamsK, NAp, collections); } 
	// if(en_reduce == ON){ TOP{{context['id']}}_NU_reduceit( kvdram, sourcebuffer, vbuffer, sourcestatsmarker, source_partition, globalparamsK, NAp, collections); } 
OCL_CHECK( // CRITICAL RESTOREME.
        err,
        cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
OCL_CHECK(err, err = kernel_events[s].wait()); // NEWCHANGE.
// if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; } // NEWCHANGE.
SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ // NEWCHANGE.
		if(globalcapsule[p].key + globalcapsule[p].value < globalparams.SIZE_KVDRAM){ globalcapsule[p].value += localcapsule[p].value; }
	}
if(localcapsule[p].key + localcapsule[p].value >= DESTBLOCKRAM_SIZE){ realsize_kvs = 0; } // ERROR CHECK
// if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ 
		resetenv = ON; flush = ON; 
		// } // CRITICAL NEWCHANGE.
Algo = BFS; // FIXME. app.cpp
// #if defined(BFS_ALGORITHM) || defined(SSSP_ALGORITHM)
// #define RANDOMVERTEXISACTIVE_ALGORITHM
// #else 
// #define ALLVERTEXISACTIVE_ALGORITHM
// #endif	// common.h
// #if not defined(ALLVERTEXISACTIVE_ALGORITHM) && defined(CONFIG_ACTS_HYBRIDLOGIC)
	// {%for i in context['T_seq']%}{%if(i<n)%}	
	// MERGE{{context['id']}}_broadcastVs(vdram, kvdram{{i}});	
	// {%endif%}{%endfor%}
	// #endif  // top_usrcv_udstv.cpp
	
#ifdef BFS_ALGORITHM 
#define ALGORITHMTYPE_REPRESENTVDATASASBITS
#endif 

// #if TREE_DEPTH==2 && defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) //
// #define TREEDEPTHISONE // {actscommon.h, classname__top_usrcv_udstv.cpp, acts_util.cpp, mem_access.cpp, config_params.h}
// #endif 

// UTIL{{context['id']}}_SetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				// UTIL{{context['id']}}_SetData(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				// UTIL{{context['id']}}_SetData(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				
				
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

*** UTIL{{context['id']}}_WRITEBITSTO_UINTV(&vdata[{{v}}], BEGINOFFSETOF_VMASK, 32, zero{{v}}); // mem_acces.cpp
*** #define MESSAGES_RETURNVALUES_SIZE 32//16 // 8//16

--- {config_params.h}
#define CONFIG_INSERTSTATSMETADATAINEDGES

// #define CONFIG_ENABLECLASS_REDUCEUPDATES
#define CONFIG_ENABLECLASS_REDUCEUPDATES2

--- {common.h}
// #define TREEDEPTHISONE // FIXME.

--- {evaluate.sh}
TESTKERNEL="RK"
# TESTKERNEL="TESTKERNEL"
TESTKERNELARG=1

--- {top_usrcv_udstv.cpp}
for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		{%for i in context['T_seq']%}{%if(i<n)%}
		{%for v in context['VECTOR2_SIZE_seq']%}
		vbuffer{{i}}[{{v}}][i] = 0;
		{%endfor%}	
		{%endif%}{%endfor%}
	}
// if(globalposition.source_partition==globalposition.last_source_partition){ // ADDME.

--- {acts.cpp}
if(globalparams.ACTSPARAMS_TREEDEPTH == 1){ for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }} // NEWCHANGE.
	
--- {classname__mem_convert_and_access.cpp}
tup = MEMCA{{context['id']}}_READVDATAWITHVMASK2(bits_vector, offsetof_vdata, offsetof_vmask);  // MEMCA:FIXME. USE SIMPLE SHIFTING OP INSTEAD.

--- {swkernel.cpp}

--- {acts_merge.cpp}
if(globalstatsbuffer{{i}}[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // {acts_merge.cpp} REMOVEME 'true'
#ifdef CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC // {common.h}
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
for(unsigned int i=3; i<4; i++){ // NUM_PEs // CRIICAL FIXME.

#ifdef FPGA_IMPL
#define LOADSLICEDEDGES_INCORRECT
#else 
#define LOADSLICEDEDGES_CORRECT	
#endif 

--- {actscommon.h}
#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS //
*/



