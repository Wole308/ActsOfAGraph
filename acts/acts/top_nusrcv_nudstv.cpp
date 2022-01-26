#include "top_nusrcv_nudstv.h"
using namespace std;

#ifdef SW
top_nusrcv_nudstv::top_nusrcv_nudstv(mydebug * _mydebugobj){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(_mydebugobj); 
	processedgesobj = new processedgesu(_mydebugobj);
	processedges_splitdstvxsobj = new processedges_splitdstvxs(_mydebugobj);
	partitionupdatesobj = new partitionupdates(_mydebugobj);
	reduceupdatesobj = new reduceupdates(_mydebugobj);
	mem_accessobj = new mem_access(_mydebugobj);
	mem_access_splitdstvxsobj = new mem_access_splitdstvxs(_mydebugobj);
	acts_mergeobj = new acts_merge(_mydebugobj);
	acts_merge_splitdstvxsobj = new acts_merge_splitdstvxs(_mydebugobj);
	actsobj = new acts(_mydebugobj);
	mydebugobj = _mydebugobj;
}
top_nusrcv_nudstv::~top_nusrcv_nudstv(){}
#endif

void acts_all::processit_unsplitdstvxs( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,								
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	sweepparams_t sweepparams;
	
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // 32
	
	step_type currentLOP=globalparamsK.ACTSPARAMS_BEGINLOP;
UTIL_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	sweepparams = UTIL_getsweepparams(globalparamsK, currentLOP, 0);
	travstate_t avtravstate;
	batch_type vmaskoffset_kvs = v_chunkids[v_chunkid] * (vmaskbuffersz_kvs); 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type tempnum_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);
	#else 
	batch_type tempnum_source_partitions = NUM_PARTITIONS;	
	#endif 
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
MEMACCESS_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK); 
UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int SKIP_KVS = reducebuffersz * FETFACTOR;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### processit_unsplitdstvxs:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	PROCESSIT_MAINLOOP: for(batch_type source_partition=v_chunkids[v_chunkid]; source_partition<v_chunkids[v_chunkid+1]; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"processit_unsplitdstvxs: processing source partition "<<source_partition<<" ... "<<endl; // REMOVEME.
		#endif 
		
		batch_type voffset_kvs2 = source_partition * reducebuffersz * FETFACTOR;
		
		if(voffset_kvs2 >= avtravstate.end_kvs){ continue; }
		if(GraphAlgo != PAGERANK) { if(vmask_p[source_partition] == 0){ vmaskoffset_kvs += vmaskbuffersz_kvs; continue; }}
		
		sweepparams.source_partition = source_partition;
		avtravstate.i_kvs = voffset_kvs2;
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print5("### processit_unsplitdstvxs:: source_partition", "voffset", "vbegin", "vend", "vskip", source_partition, voffset_kvs2 * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
MEMACCESS_readvmasks(ON, kvdram, vmaskREAD, vbuffer, globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK, 0, vmaskoffset_kvs, 0, vmaskbuffersz_kvs); // NOTE: this should come before loadvdata because vbuffer is used as a temp buffer
MEMACCESS_readvdata(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs2, vbuffer, 0, 0, reducebuffersz, globalparamsK);
MEMACCESS_readvdata(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs2 + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparamsK);
		vmaskoffset_kvs += vmaskbuffersz_kvs;
	
		vertex_t srcvlocaloffset = (voffset_kvs2 * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs2 * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
		tuple_t tup = MEMACCESS_getvptrs_opt( kvdram, vptrbaseoffset_kvs, voffset_kvs2 * VECTOR2_SIZE, (voffset_kvs2 + SKIP_KVS) * VECTOR2_SIZE, edgebankID); // CRITICAL NEWCHANGE.
		keyy_t beginvptr = tup.A;
		keyy_t endvptr = tup.B; 
	
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr;
		keyy_t localendvptr = endvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"processit_unsplitdstvxs::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("processit_unsplitdstvxs", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"[index: "<<source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs2 * VECTOR_SIZE<<"]"<<endl;
		#endif
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->clearallstats();
		#endif
		
		travstate_t etravstate;
		etravstate.begin_kvs = localbeginvptr_kvs;
		etravstate.size_kvs = numedges_kvs;
		etravstate.end_kvs = etravstate.begin_kvs + etravstate.size_kvs;
		etravstate.skip_kvs = SRCBUFFER_SIZE;
		etravstate.i_kvs = etravstate.begin_kvs;
		etravstate.i2 = voffset_kvs2 * VECTOR2_SIZE;
		etravstate.i2_kvs = voffset_kvs2;
		bool_type resetenv = ON;
		bool_type flush = ON;
		if(voffset_kvs2 == avtravstate.begin_kvs){ resetenv = ON; } else { resetenv = OFF; }
		if((voffset_kvs2 + (reducebuffersz * FETFACTOR)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		if(GraphAlgo != PAGERANK){ resetenv = ON; flush = ON; } // CRITICAL NEWCHANGE.
		
		#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
ACTS_actit
		#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
ACTS_priorit	
		#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
ACTS_tradit
		#else 
ACTS_tradit
		#endif 
		(
			ON, PROCESSMODE,
 kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, globalstatsbuffer, 
			globalparamsK, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE,
			resetenv, flush, edgebankID);
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
		
MEMACCESS_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

void acts_all::processit_splitdstvxs( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,								
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	sweepparams_t sweepparams;
	
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	
	step_type currentLOP=globalparamsK.ACTSPARAMS_BEGINLOP;
UTIL_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	sweepparams = UTIL_getsweepparams(globalparamsK, currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
MEMACCESS_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK); 
UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int SKIP_KVS = globalparamsK.SIZEKVS2_REDUCEPARTITION;
	unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
	
	batch_type voffset_kvs2 = 0;
	batch_type vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### processit_splitdstvxs:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	PROCESSIT_MAINLOOP1: for(batch_type source_partition=0; source_partition<globalparamsK.NUM_PROCESSEDGESPARTITIONS; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit_splitdstvxs: processing [source_partition: "<<source_partition<<"]: [voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<"]: [vreadoffset: "<<vreadoffset_kvs2 * NUM_PEs * VECTOR2_SIZE<<"] ... "<<endl;
		#endif
		
		/* ///////////////////////////////////////////////// REMOVEME
		#ifdef _DEBUGMODE_CHECKS2
		if(globalparamsK.ACTSPARAMS_INSTID == 0){
			for(buffer_type k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k+=1){
				for(unsigned int v=0; v<VECTOR2_SIZE; v+=1){
					if(vmaskREAD[v][k]==1){ mydebugobj->increment(1, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, 1); }
				}
			}
		}
		#endif 
		///////////////////////////////////////////////// */
		
		if(voffset_kvs2 >= avtravstate.end_kvs){ voffset_kvs2 += globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION; vreadoffset_kvs2 += vreadskipsz_kvs2; continue; }
		if(GraphAlgo != PAGERANK) { if(vmask_p[source_partition] == 0){ voffset_kvs2 += globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION; vreadoffset_kvs2 += vreadskipsz_kvs2; continue; }}
		#ifdef _DEBUGMODE_KERNELPRINTsS
		if(vmask_p[source_partition] > 0){ cout<<"processit_splitdstvxs:: source partition is active [vmask_p["<<source_partition<<"]: "<<vmask_p[source_partition]<<"] ..."<<endl; }
		#endif 
		
		sweepparams.source_partition = source_partition;
		avtravstate.i_kvs = voffset_kvs2;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print5("### processit_splitdstvxs:: source_partition", "voffset", "vbegin", "vend", "vskip", source_partition, voffset_kvs2 * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
MEMACCESS_SPL_readvmaskschunks(ON, kvdram, vmaskREAD, vbuffer, globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsK);
MEMACCESS_SPL_readvdatachunks(ON, kvdram, vbuffer, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsK);
		
		/* ///////////////////////////////////////////////// REMOVEME
		#ifdef _DEBUGMODE_CHECKS2
		if(globalparamsK.ACTSPARAMS_INSTID == 0){
			for(buffer_type k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k+=1){
				for(unsigned int v=0; v<VECTOR2_SIZE; v+=1){
					if(vmaskREAD[v][k]==1){ mydebugobj->increment(0, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, 1); }
				}
			}
		}
		#endif 
		///////////////////////////////////////////////// */
		
		vertex_t srcvlocaloffset = (voffset_kvs2 * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs2 * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
		tuple_t tup = MEMACCESS_getvptrs_opt( kvdram, vptrbaseoffset_kvs, voffset_kvs2 * VECTOR2_SIZE, (voffset_kvs2 + SKIP_KVS) * VECTOR2_SIZE, edgebankID); // CRITICAL NEWCHANGE.
		keyy_t beginvptr = tup.A;
		keyy_t endvptr = tup.B + VECTOR2_SIZE; // NEWCHANGE 
	
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr;
		keyy_t localendvptr = endvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"processit_splitdstvxs::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("processit_splitdstvxs", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"[index: "<<source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs2 * VECTOR_SIZE<<"]"<<endl;
		#endif
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->clearallstats();
		#endif
		
		travstate_t etravstate;
		etravstate.begin_kvs = localbeginvptr_kvs;
		etravstate.size_kvs = numedges_kvs;
		etravstate.end_kvs = etravstate.begin_kvs + etravstate.size_kvs;
		etravstate.skip_kvs = SRCBUFFER_SIZE;
		etravstate.i_kvs = etravstate.begin_kvs;
		etravstate.i2 = voffset_kvs2 * VECTOR2_SIZE;
		etravstate.i2_kvs = voffset_kvs2;
		bool_type resetenv = ON;
		bool_type flush = ON;
		if(voffset_kvs2 == avtravstate.begin_kvs){ resetenv = ON; } else { resetenv = OFF; }
		if((voffset_kvs2 + globalparamsK.SIZEKVS2_REDUCEPARTITION) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		if(GraphAlgo != PAGERANK){ resetenv = ON; flush = ON; } // CRITICAL NEWCHANGE.
		
		#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
ACTS_actit
		#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
ACTS_priorit	
		#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
ACTS_tradit
		#else 
ACTS_tradit
		#endif 
		(
			ON, PROCESSMODE,
 kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, globalstatsbuffer, 
			globalparamsK, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE,
			resetenv, flush, edgebankID);
			
		voffset_kvs2 += globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
		vreadoffset_kvs2 += vreadskipsz_kvs2;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
		// exit(EXIT_SUCCESS); ////
	}
		
MEMACCESS_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); ////
	return;
}

void acts_all::partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], globalparams_t globalparams, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no partitionit for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nusrcv_nudstv::UTIL_getglobalparams:: printing global parameters", globalparams);
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparams.ACTSPARAMS_BEGINLOP + 1 + (globalparams.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = UTIL_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"partitionit: source_partition "<<source_partition<<" ... "<<endl;

UTIL_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = UTIL_getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t ptravstate = UTIL_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
			
			// collect stats
MEMACCESS_readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
ACTS_actit
			#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
ACTS_priorit	
			#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
ACTS_tradit
			#else 
ACTS_tradit
			#endif
			(config.enablepartition, PARTITIONMODE,
 kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, globalstatsbuffer, // CRITICAL FIXME.
					globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp);
					
MEMACCESS_saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += NUM_PARTITIONS;
			}
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_STATS
			if(config.enablereduce == ON){ edges_count = actsutilityobj->globalstats_getcountvalidkvsreduced(); edgesdstv_sum = actsutilityobj->globalstats_getreducevar1(); }
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printglobalvars();
			actsutilityobj->clearglobalvars();
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		#endif 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
		actsutilityobj->clearglobalvars();
		#endif
	}
	return;
}

void acts_all::reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	config_t config;
	sweepparams_t sweepparams;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	#else 
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH + 1;
	#endif 
	
	sweepparams = UTIL_getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = UTIL_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
ACTS_actit
	#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
ACTS_priorit	
	#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
ACTS_tradit
	#else 
ACTS_tradit
	#endif
	(config.enablereduce, REDUCEMODE,
 kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, globalstatsbuffer, // CRITICAL FIXME.
			globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp); // REMOVEME.
	return;
}

void acts_all::dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#ifdef CONFIG_SPLIT_DESTVTXS // REMOVEME.
	if(en_process == ON){ processit_splitdstvxs( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, globalparamsE, globalparamsK, v_chunkids, v_chunkid, edgebankID); } 
	#else 
	if(en_process == ON){ processit_unsplitdstvxs( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, globalparamsE, globalparamsK, v_chunkids, v_chunkid, edgebankID); } 	
	#endif // CRITICAL ADDMEBACK.
	if(en_partition == ON){ partitionit( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE,  vmask_subp, globalparamsK, NAp); } 
	if(en_reduce == ON){ reduceit( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, sourcestatsmarker, source_partition, globalparamsK, NAp); } 
	return;
}

void acts_all::dispatch_reduce( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no dispatch_reduce for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int vmask_offset_kvs = 0;
	unsigned int vmaskp_offset_kvs = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	#else 
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH + 1;
	#endif 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type num_source_partitions = UTIL_get_num_source_partitions(currentLOP);
	#else 
	batch_type num_source_partitions = NUM_PARTITIONS;	
	#endif

	bool_type enablereduce = ON;
	
	DISPATCHREDUCE_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### dispatch_reduce:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, num_source_partitions); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTIL_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
MEMACCESS_SPL_readvdata(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK);
		#ifdef CONFIG_COLLECTMASKINFOSDURINGREDUCE
UTIL_reset(vmaskWRITE);
		#endif 
		
		// reduce
		dispatch(OFF, OFF, enablereduce,  kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, v_chunkids, v_chunkid, NAp);
		
		// writeback vertices
MEMACCESS_SPL_savevdata(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK); 
		#ifdef CONFIG_COLLECTMASKINFOSDURINGREDUCE
MEMACCESS_SPL_savemasks(enablereduce, kvdram, vmaskWRITE, globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK, vmask_offset_kvs, globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs, globalparamsK); // NEW
		#endif 
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		#ifdef CONFIG_SPLIT_DESTVTXS
		vmaskp_offset_kvs += NUM_PEs;
		#else 
		vmaskp_offset_kvs += 1;	
		#endif
	}
	return;
} 

// top
void acts_all::topkernelproc_embedded(
	
	uint512_dt * kvdram){
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> Light weight ACTS (PR: 1 ACTS IN 1 COMPUTEUNITS) Launched... size: "<<UTIL_GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer
	unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	// #pragma HLS DATA_PACK variable = vmaskREAD
	#pragma HLS array_partition variable = vmaskREAD
	unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE]; // NEWCHANGE.
	// #pragma HLS DATA_PACK variable = vmaskWRITE
	#pragma HLS array_partition variable = vmaskWRITE
	unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	// #pragma HLS DATA_PACK variable = vmask_subp
	#pragma HLS array_partition variable = vmask_subp
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparamsK = UTIL_getglobalparams(kvdram);
	globalparams_t globalparamsE[MAX_NUM_EDGE_BANKS];
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsE[0] = globalparamsK;
	#endif 
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	
	if(GraphAlgo != PAGERANK){ MEMACCESS_readpmask(kvdram, vmask_p, globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(kvdram, globalparamsK); }	
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	#endif 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type num_vchunks = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);
	#else 
	batch_type num_vchunks = NUM_PARTITIONS;	
	#endif 
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int u=0; u<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER+1; u++){ cout<<">>> globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<", PARTITION_CHKPT["<<u<<"]: "<<PARTITION_CHKPT[u]<<endl; }			
	#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(kvdram, vbuffer, globalparamsK);
	#endif
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int it_size = 0;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	it_size = NUM_EDGE_BANKS;
	#else 
	it_size = 1;	
	#endif 
	
	for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalparamsK.VARS_WORKBATCH = 0;
		globalparams_t _globalparamsE = globalparamsE[edgebankID];

		for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			// process & partition
			#ifdef CONFIG_ENABLEPROCESSMODULE
			if(globalparamsK.ENABLE_PROCESSCOMMAND == ON){
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelproc: processing instance ... "<<endl;
				#endif
				dispatch(ON, OFF, OFF,  kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, NAp, NAp, _globalparamsE, globalparamsK, PARTITION_CHKPT[edgebankID], v_chunkid, edgebankID);
			}
			#endif
			
			// partition
			#ifdef CONFIG_ENABLEPARTITIONMODULE
			if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON){
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelproc: partitioning instance ... "<<endl;
				#endif
				dispatch(OFF, ON, OFF,  kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, NAp, NAp, _globalparamsE, globalparamsK, PARTITION_CHKPT[edgebankID], v_chunkid, NAp);
			}
			#endif
			
			// reduce & partition
			#if defined(CONFIG_ENABLEREDUCEMODULE)
			if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON){ 
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelproc: reducing instance ... "<<endl;
				#endif
				dispatch_reduce( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, _globalparamsE, globalparamsK, PARTITION_CHKPT[edgebankID], v_chunkid, NAp);
			}
			#endif
			
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			accumkvstats(kvdram, buffer, globalparamsK);
			globalparamsK.VARS_WORKBATCH += 1;
			#endif 
			#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
			actsutilityobj->clearglobalvars();
			#endif
		}
		
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		globalparamsK.BASEOFFSETKVS_STATSDRAM += (globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS; 
		#endif
	}
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsK.BASEOFFSETKVS_STATSDRAM = FIRST_BASEOFFSETKVS_STATSDRAM;
	commitkvstats(kvdram, buffer, globalparamsK);
	#endif
UTIL_increment_graphiteration(kvdram, globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS3) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void acts_all::topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs1(kvdram0, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP1: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP1: processing instance 0"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs1(kvdram0, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs2(kvdram0,kvdram1, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP2: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP2: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP2: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP2: processing instance 1"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs2(kvdram0,kvdram1, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs3(kvdram0,kvdram1,kvdram2, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
 */
	
MERGE_SPLIT_mergeVs3(kvdram0,kvdram1,kvdram2, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem8
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs4(kvdram0,kvdram1,kvdram2,kvdram3, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 3"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs4(kvdram0,kvdram1,kvdram2,kvdram3, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem10
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem5
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs5(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 4"<<endl;
	#endif 
	topkernelproc_embedded(kvdram4);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 4"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs5(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem12
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs6(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 4"<<endl;
	#endif 
	topkernelproc_embedded(kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 5"<<endl;
	#endif 
	topkernelproc_embedded(kvdram5);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 4"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 5"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs6(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem14
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem7
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs7(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 4"<<endl;
	#endif 
	topkernelproc_embedded(kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 5"<<endl;
	#endif 
	topkernelproc_embedded(kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 6"<<endl;
	#endif 
	topkernelproc_embedded(kvdram6);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 4"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 5"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 6"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs7(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP8(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem16
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem8
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs8(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 4"<<endl;
	#endif 
	topkernelproc_embedded(kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 5"<<endl;
	#endif 
	topkernelproc_embedded(kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 6"<<endl;
	#endif 
	topkernelproc_embedded(kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 7"<<endl;
	#endif 
	topkernelproc_embedded(kvdram7);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 4"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 5"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 6"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 7"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs8(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP9(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = edges8 offset = slave bundle = gmem16
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem18
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem9
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges8 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#pragma HLS DATA_PACK variable = edges8
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs9(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 4"<<endl;
	#endif 
	topkernelproc_embedded(kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 5"<<endl;
	#endif 
	topkernelproc_embedded(kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 6"<<endl;
	#endif 
	topkernelproc_embedded(kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 7"<<endl;
	#endif 
	topkernelproc_embedded(kvdram7);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 8"<<endl;
	#endif 
	topkernelproc_embedded(kvdram8);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 4"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 5"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 6"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 7"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 8"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs9(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP10(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = edges8 offset = slave bundle = gmem16
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
#pragma HLS INTERFACE m_axi port = edges9 offset = slave bundle = gmem18
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem19
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem20
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem10
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges9 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#pragma HLS DATA_PACK variable = edges8
#pragma HLS DATA_PACK variable = edges9
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs10(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 4"<<endl;
	#endif 
	topkernelproc_embedded(kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 5"<<endl;
	#endif 
	topkernelproc_embedded(kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 6"<<endl;
	#endif 
	topkernelproc_embedded(kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 7"<<endl;
	#endif 
	topkernelproc_embedded(kvdram7);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 8"<<endl;
	#endif 
	topkernelproc_embedded(kvdram8);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 9"<<endl;
	#endif 
	topkernelproc_embedded(kvdram9);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 4"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 5"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 6"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 7"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 8"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 9"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs10(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP11(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * kvdram10,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = edges8 offset = slave bundle = gmem16
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
#pragma HLS INTERFACE m_axi port = edges9 offset = slave bundle = gmem18
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem19
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem20
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem21
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem22
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem11
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges9 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#pragma HLS DATA_PACK variable = edges8
#pragma HLS DATA_PACK variable = edges9
#pragma HLS DATA_PACK variable = edges10
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs11(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 4"<<endl;
	#endif 
	topkernelproc_embedded(kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 5"<<endl;
	#endif 
	topkernelproc_embedded(kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 6"<<endl;
	#endif 
	topkernelproc_embedded(kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 7"<<endl;
	#endif 
	topkernelproc_embedded(kvdram7);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 8"<<endl;
	#endif 
	topkernelproc_embedded(kvdram8);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 9"<<endl;
	#endif 
	topkernelproc_embedded(kvdram9);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 10"<<endl;
	#endif 
	topkernelproc_embedded(kvdram10);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 4"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 5"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 6"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 7"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 8"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 9"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 10"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs11(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, vdram);
	return;
}
}
extern "C" {
void acts_all::topkernelP12(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * kvdram10,
	uint512_dt * kvdram11,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = edges8 offset = slave bundle = gmem16
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
#pragma HLS INTERFACE m_axi port = edges9 offset = slave bundle = gmem18
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem19
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem20
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem21
#pragma HLS INTERFACE m_axi port = edges11 offset = slave bundle = gmem22
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem23
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem24
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem12
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges9 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges11 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram11 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#pragma HLS DATA_PACK variable = edges8
#pragma HLS DATA_PACK variable = edges9
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges11
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = kvdram11
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	if(GraphIter > 0){ MERGE_SPLIT_broadcastVs12(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 0"<<endl;
	#endif 
	topkernelproc_embedded(kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 1"<<endl;
	#endif 
	topkernelproc_embedded(kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 3"<<endl;
	#endif 
	topkernelproc_embedded(kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 4"<<endl;
	#endif 
	topkernelproc_embedded(kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 5"<<endl;
	#endif 
	topkernelproc_embedded(kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 6"<<endl;
	#endif 
	topkernelproc_embedded(kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 7"<<endl;
	#endif 
	topkernelproc_embedded(kvdram7);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 8"<<endl;
	#endif 
	topkernelproc_embedded(kvdram8);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 9"<<endl;
	#endif 
	topkernelproc_embedded(kvdram9);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 10"<<endl;
	#endif 
	topkernelproc_embedded(kvdram10);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 11"<<endl;
	#endif 
	topkernelproc_embedded(kvdram11);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 2"<<endl;
	#endif 
	topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 4"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 5"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 6"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 7"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 8"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 9"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 10"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 11"<<endl;
	#endif 
 */
	
MERGE_SPLIT_mergeVs12(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, vdram);
	return;
}
}



