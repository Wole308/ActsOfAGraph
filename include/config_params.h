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

// #define CONFIG_READVDATA_SLIDE // access format for source vertices 
#define CONFIG_READVDATA_SLIDEANDREARRANGE

#define CONFIG_SEPERATEVMASKFROMVDATA // seperating vmask and vbuffer datas {loadedges_splitdstvxs.cpp, processedges_splitdstvxs.cpp, reduceupdates.cpp, acts_merge_splitdstvxs.cpp}

#define CONFIG_ENABLEPROCESSMODULE
#define CONFIG_ENABLEPARTITIONMODULE
#define CONFIG_ENABLEREDUCEMODULE //
#define CONFIG_ENABLESYNCHRONIZEMODULE //

#define CONFIG_ENABLECLASS_ACTSUTILITY
#define CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
// #define CONFIG_ENABLECLASS_TOPNUSRCV_NUDSTV
#define CONFIG_ENABLECLASS_PROCESSEDGES_SPLITDSTVXS
#define CONFIG_ENABLECLASS_PARTITIONUPDATES
#define CONFIG_ENABLECLASS_REDUCEUPDATES
#define CONFIG_ENABLECLASS_MEM_ACCESS_SPLITDSTVXS
#define CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
#define CONFIG_ENABLECLASS_ACTS_UTIL
#define CONFIG_ENABLECLASS_ACTS
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
context['PE_SETSZ_seq'] = []
for i in range (0,8):
		context['PE_SETSZ_seq'].append(i)
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

void acts_all::MEMACCESS_SPL_readvdatachunks(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t * KV = (value_t *)&kvdram[vbaseoffset_kvs];
	unsigned int mydepth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION * VECTOR2_SIZE;
	for(unsigned int i=0; i<16384; i++){
		for(unsigned int s=0; s<NUM_PEs; s++){
			KV[s*mydepth + i] = index; index+=1;
		}
		if(index > 2*16384){ break; }
		
void {{context['classname__mem_access_splitdstvxs']}}MEMACCESS_SPL_readvdata_slide
void {{context['classname__mem_access_splitdstvxs']}}MEMACCESS_SPL_readvdatachunks(
void {{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS(
READANDPROCESS_SPL_LOOP3C: for (buffer_type i=0; i<5; i++){ // CRITICAL REMOVEME.

******** void {{context['classname__top_nusrcv_nudstv']}}dispatch_reduce({ ...  if(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID==0){
if(globalparams.ALGORITHMINFO_GRAPHITERATIONID==1){ exit(EXIT_SUCCESS); }
algorithm.cpp -> value_t algorithm::vertex_initdata(){ 
	return MAXVDATA; 0xFFFFFFFF;

actscommon-> #ifdef ACTS_PARTITION_AND_REDUCE_STRETEGY
#define NUMPIPELINES_PARTITIONUPDATES 2 // REMOVEME.
}

acts.cpp -> #ifdef ALLVERTEXISACTIVE_ALGORITHM
	if(maxcutoff % 4 != 0){ maxcutoff = 4; } // FIXME.
	#endif 
	
void {{context['classname__top_nusrcv_nudstv']}}dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, {%for n in context['NUM_EDGE_BANKS_seq']%}uint512_dt * edges{{n}},{%endfor%} uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	// if(en_process == ON){ processit_splitdstvxs({%for n in context['NUM_EDGE_BANKS_seq']%}edges{{n}},{%endfor%} kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, globalparamsE, globalparamsK, v_chunkids, v_chunkid, edgebankID); } 
	// if(en_partition == ON){ partitionit({%for n in context['NUM_EDGE_BANKS_seq']%}edges{{n}},{%endfor%} kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE,  vmask_subp, globalparamsK, NAp); } 
	if(en_reduce == ON){ reduceit({%for n in context['NUM_EDGE_BANKS_seq']%}edges{{n}},{%endfor%} kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, sourcestatsmarker, source_partition, globalparamsK, NAp); } 
	return;
}

void {{context['classname__acts_util']}}UTIL_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value)

// CRITICAL REMOVEME.
	// if(GraphIter > 0){ {%if(context['XWARE']=="SW")%}acts_merge_splitdstvxsobj->{%endif%}MERGE_SPLIT_broadcastVs{{n}}({%for i in context['T_seq']%}{%if(i<n)%}kvdram{{i}},{%endif%}{%endfor%} vdram); }
// {%if(context['XWARE']=="SW")%}acts_merge_splitdstvxsobj->{%endif%}MERGE_SPLIT_mergeVs{{n}}({%for i in context['T_seq']%}{%if(i<n)%}kvdram{{i}},{%endif%}{%endfor%} vdram);
	
context['PE_SETSZ_seq'] = []
for i in range (0,8):
		context['PE_SETSZ_seq'].append(i)
		
READANDPROCESS_SPL_LOOP3D: for (buffer_type i=0; i<24; i++){
*/



