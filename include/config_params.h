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

////////////////////////////////////////////////////////////////////////////////////
#define CONFIG_SPLIT_DESTVTXS // [OBSOLETE] // {reduceupdates.cpp, acts_util.cpp, app.cpp, loadgraph.cpp, top_nusrcv_nudstv.cpp, reduceupdates.cpp, actscommon.h}
// #define CONFIG_SEPERATEVMASKFROMVDATA // [OBSOLETE] // seperating vmask and vbuffer datas {loadedges_sequential.cpp, processedges.cpp, reduceupdates.cpp, acts_merge.cpp, actscommon.h, common.h}
////////////////////////////////////////////////////////////////////////////////////

#define CONFIG_UNIFYSRCV // {app.cpp}

// #define CONFIG_READVDATA_SLIDE // access format for source vertices 
#define CONFIG_READVDATA_SLIDEANDREARRANGE

#define SEQUENTIALIZE_VIDS_IN_A_DRAM_ROW // uses MAX_NUM_UNIQ_EDGES_PER_VEC in {common.h}

#define CUSTOMLOGICFOREACHALGORITHM // {processedges.cpp, reduceupdates.cpp}

// #define CONFIG_PREPROCESS_EDGESREARRANGEDINDRAM // {classname__mem_access.template}
// #define CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS // {classname__mem_access.template, utility.cpp}
#define CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS // NEW {classname__top_usrcv_udstv.cpp}

#define CONFIG_INSERTSTATSMETADATAINEDGES // {acts.cpp, loadedges_random.cpp, classname__top_usrcv_udstv.cpp, processedges2.cpp}

#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
#define CONFIG_VDATAIS32BITSWIDE // NEW, FIXME. IMPLEMENT FOR ALL CASES.
#endif 

#define CONFIG_ENABLEPROCESSMODULE
#define CONFIG_ENABLEPARTITIONMODULE
#define CONFIG_ENABLEREDUCEMODULE //
#define CONFIG_ENABLESYNCHRONIZEMODULE //

#define CONFIG_ENABLECLASS_ALGO_FUNCS
#define CONFIG_ENABLECLASS_ACTSUTILITY
#ifdef CONFIG_UNIFYSRCV
	#define CONFIG_ENABLECLASS_TOP_USRCV_UDSTV // FIXME. should be 'CONFIG_ENABLECLASS_TOP_USRCV_NUDSTV'
		#else 
			#define CONFIG_ENABLECLASS_TOPNUSRCV_NUDSTV
				#endif
#if defined(CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS)
	#define CONFIG_ENABLECLASS_PROCESSEDGES	
		#elif defined(CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS)
			#define CONFIG_ENABLECLASS_PROCESSEDGES2
				#else
				NOT DEFINED.	
					#endif	
#define CONFIG_ENABLECLASS_REDUCEUPDATES
// #define CONFIG_ENABLECLASS_REDUCEUPDATES2
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
{config_params.h}
#define CONFIG_INSERTSTATSMETADATAINEDGES // {congig_params.h}
// #define CONFIG_ENABLECLASS_REDUCEUPDATES2 // {congig_params.h}

{evaluate.sh}
TESTKERNEL="RK"
# TESTKERNEL="TESTKERNEL"
TESTKERNELARG=1

{acts_merge.cpp}
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

{acts_merge.cpp}
#ifndef FPGA_IMPL
	MERGE{{context['id']}}_mergeVs{{n}}({%for i in context['T_seq']%}{%if(i<n)%}kvdram{{i}},{%endif%}{%endfor%} vdram, 
		{%for i in context['T_seq']%}{%if(i<n)%}globalstatsbuffer{{i}},{%endif%}{%endfor%} {%for i in context['T_seq']%}{%if(i<n)%}pmask{{i}}_next,{%endif%}{%endfor%} 
			globalparamsK, globalparamsV);
	#endif 
	
{common.h}	
#define PROCESSPARTITIONSZ (((SRAMSZ * VDATA_PACKINGSIZE) / NUM_PEs) * NUM_PEs) // 16128

// #define PROCESSEDGES_TYPEA
#define PROCESSEDGES_TYPEB

{acts_merge.cpp}
if(statssizes_kvs[0][partitionoffseti] > 0 || true){ // CRITICAL REMOVEME. true. 

{processedges2.cpp}
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
*/

/**
- AUTOMATE FOR LARGER GRAPHS: mem_access.cpp, acts_merge.cpp, mem_access.cpp->MEMACCESS_readvmasks
- CRITICAL FIXME. mem_access.cpp
- CRITICAL FIXME. TOO EXPENSIVE. mem_access.cpp, processedges.cpp MEMACCESS_SPL_GetXYLayoutV((i*NUM_PEs)+s, vdata, vdata2, depths); // FIXME. TOO EXPENSIVE.
- CRITICAL OPTIMIZEME: acts_merge.cpp
- TOO EXPENSIVE. top_nusrcv_nudstv.cpp, processedges_slicedgraph.cpp, acts_merge.cpp
- FIXME FOR DIFFERENT PARAMETERS: loadedges_sequential.cpp
- NOT IMPLEMENTED. mem_access.cpp

- // CRITICAL FIXME.
- acts.cpp : #ifdef _DEBUGMODE_CHECKS2 // CRITICAL CHECKME.
	if(maxcutoff % 4 != 0){ for(int v=0; v<VECTOR_SIZE; v++){ cout<<"------------------- actspipeline:cutoffs["<<v<<"]: "<<cutoffs[v]<<endl; } exit(EXIT_FAILURE); }
	actsutilityobj->checkfordivisibleby(ON, "actspipeline:maxcutoff", maxcutoff, 4);
	#endif
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	if(maxcutoff % 4 != 0){ maxcutoff = 4; } // FIXME.
	#endif 
- #ifdef _WIDEWORD // CRITICAL FIXME. @ {{context['classname__mem_access']}}MEMACCESS_SPL_readVchunks
- #ifndef _WIDEWORD  // CRITICAL FIXME. @ {{context['classname__mem_access']}}MEMACCESS_SPL_SLreadV
- // CRITICAL FIXME @ {{context['classname__mem_convert_and_access']}}MEMCA_CREATEVBUFFERSTRUCT
- reduceupdates.cpp: if(en == true){ curr_vprop = loc + destoffset; } // CRITICAL REMOVEME.	
- NEWCHANGE: laodedges_splitdsxtvs: if(index%VECTOR2_SIZE == 0 && edge.dstvid == 888888){ numskippededges += 1; tempe_index += 1; continue; }
- CRITICAL REMOVEME: cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
- CRITICAL REMOVEME: void {{context['classname__top_usrcv_udstv']}}TOP{{context['id']}}_U_topkernelP{{n}}: // num_stages = 1; // REMOVEME.
- NEWCHANGE: processit: if(voffset_kvs == avtravstate.begin_kvs || globalposition.source_partition == globalposition.first_source_partition){ resetenv = ON; } else { resetenv = OFF; } // NEWCHANGE
- CRITICAL REMOVEME: actscommon.h: #define NUMPIPELINES_PARTITIONUPDATES 1//2 
- REMOVEME: context['PE_SETSZ'] = 16 #8, 16 # for classname__processedges.cpp
- NEWCHANGE: top: batch_type vreadoffset_kvs2 = 0; // NEWCHANGE.
- NEWCHANGE: acts.cpp: // if(UTIL{{context['id']}}_GETKV(kvA0[0]).value != UTIL{{context['id']}}_GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; }
			if(UTIL{{context['id']}}_GETKV(kvA0[0]).value != UTIL{{context['id']}}_GETV(INVALIDDATA) && UTIL{{context['id']}}_GETKV(kvA0[1]).value != UTIL{{context['id']}}_GETV(INVALIDDATA) && UTIL{{context['id']}}_GETKV(kvA0[2]).value != UTIL{{context['id']}}_GETV(INVALIDDATA) && UTIL{{context['id']}}_GETKV(kvA0[3]).value != UTIL{{context['id']}}_GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; }		
- CRITICAL REMOVEME: MEMACCESS{{context['id']}}_readANDRVchunks{{n}}: #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.
- CRITICAL FIXME: unsigned int incr = 5; // edges_temp[i][j+v].srcvid; // - srcvid_head; // CRITICAL FIXME.
- CRITICAL REMOVEME: keyvalue_t mykeyvalue; 0 = edata.key; mykeyvalue.value = 0; // CRITICAL REMOVEME.
- CRITICAL REMOVEME: #define KVSOURCEDRAMSZ (1 << 26) // max HBM capacity (512MB) // CRITICAL REMOVEME. actscommon.h
- CRITICAL FIXME: top_nusrcv_nudstv.cpp: #ifndef ALLVERTEXISACTIVE_ALGORITHM // CRITICAL FIXME.
	MERGE{{context['id']}}_mergeVs{{n}}({%for i in context['T_seq']%}{%if(i<n)%}kvdram{{i}},{%endif%}{%endfor%} vdram, globalparamsK, globalparamsV);
	#endif 
- FIXME: config_params.h // #define CUSTOMLOGICFOREACHALGORITHM // {processedges.cpp, reduceupdates.cpp}
- FIXME: acts_util.cpp: // partition_type {{context['classname__acts_util']}}UTIL{{context['id']}}_getpartition(
- CRITICAL OPTIMIZEME: top_u: for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparams.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE. IS THIS OPTIMIZED
- CRITICAL OPTIMIZEME: top_u: for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparams.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.

- CRITICAL DEBUGME: loadedges_random.cpp: // WRITETO_UINT((unsigned int *)&edges2_temp[i][j+v].dstvid, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA, incr); // NEWCHANGE.
- CRITICAL DEBUGME: processedges2.cpp: if(_processedgeblock == true){
- FIXME: creategraphs.cpp: if(srcv >= KVDATA_RANGE){ srcv = KVDATA_RANGE-1; } // CRITICAL FIXME.
- FIXME: if(srcv >= KVDATA_RANGE){ srcv = KVDATA_RANGE-1; } // FIXME.
- FIXME: createundirectedgraph.cpp: if(srcv == 0 && dstv == 0 && num_zeros++ > 1000){ continue; } // FIXME.
- FIXME?: graph.cpp: void graph::loadedgesfromfile(int col, 
- CRITICAL NEWCHANGE: loadgraph.cpp: // CRITICAL NEWCHANGE. since kvdram and edges dram are now merged into one
fetchmessage.chunksize_kvs = 0;//chunk_size * 2; // loadcount; // CRITICAL FIXME
*/

/** *** IMPORTANT CHANGES ***
#define CONFIG_SEPERATEVMASKFROMVDATA 
evaluate.sh: for num_pes in $NUM_PEs_EQ24
context['PE_SETSZ'] = 8 #8, 16 # for classname__processedges.cpp
#define MAX_NUM_UNIQ_EDGES_PER_VEC 16
// topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID  (REMOVEME)
// if(GraphIter > 0){ {%if(context['XWARE']=="SW")%}acts_mergeobj->{%e
void {{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATA0ANDVMASK0(k
void {{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATA1ANDVMASK1(ke

*/



