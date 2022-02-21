void acts_all::TOPP0_NU_processit_splitdstvxs(uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,								
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	sweepparams_t sweepparams;
	
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	
	step_type currentLOP=globalparamsK.ACTSPARAMS_BEGINLOP;
	UTILP0_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	sweepparams = UTILP0_getsweepparams(globalparamsK, currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	globalposition_t globalposition; // DUMMY.
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	MEMACCESSP0_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK); 
	UTILP0_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int SKIP_KVS = globalparamsK.SIZEKVS2_REDUCEPARTITION;
	unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
	
	batch_type voffset_kvs2 = 0;
	batch_type vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### TOPP0_NU_processit_splitdstvxs:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	PROCESSIT_MAINLOOP1: for(batch_type source_partition=0; source_partition<globalparamsK.NUM_PROCESSEDGESPARTITIONS; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"TOPP0_NU_processit_splitdstvxs: processing [source_partition: "<<source_partition<<"]: [voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<"]: [vreadoffset: "<<vreadoffset_kvs2 * NUM_PEs * VECTOR2_SIZE<<"] ... "<<endl;
		#endif
		
		if(voffset_kvs2 >= avtravstate.end_kvs){ voffset_kvs2 += globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION; vreadoffset_kvs2 += vreadskipsz_kvs2; continue; }
		if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE) { if(vmask_p[source_partition] == 0){ voffset_kvs2 += globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION; vreadoffset_kvs2 += vreadskipsz_kvs2; continue; }}
		#ifdef _DEBUGMODE_KERNELPRINTsS
		if(vmask_p[source_partition] > 0){ cout<<"TOPP0_NU_processit_splitdstvxs:: source partition is active [vmask_p["<<source_partition<<"]: "<<vmask_p[source_partition]<<"] ..."<<endl; }
		#endif 
		
		sweepparams.source_partition = source_partition;
		avtravstate.i_kvs = voffset_kvs2;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print5("### TOPP0_NU_processit_splitdstvxs:: source_partition", "voffset", "vbegin", "vend", "vskip", source_partition, voffset_kvs2 * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
MEMACCESSP0_readVchunks(ON, kvdram, vbuffer, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsK);
	
		vertex_t srcvlocaloffset = (voffset_kvs2 * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs2 * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
		tuple_t tup = MEMACCESSP0_getvptrs_opt(edges0, kvdram, vptrbaseoffset_kvs, voffset_kvs2 * VECTOR2_SIZE, (voffset_kvs2 + SKIP_KVS) * VECTOR2_SIZE, edgebankID); // CRITICAL NEWCHANGE.
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
		if(localendvptr < localbeginvptr){ cout<<"TOPP0_NU_processit_splitdstvxs::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("TOPP0_NU_processit_splitdstvxs", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
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
		if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ resetenv = ON; flush = ON; } // CRITICAL NEWCHANGE.
		
		#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
ACTSP0_actit
		#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
		ACTSP0_priorit	
		#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
		ACTSP0_tradit
		#else 
		ACTSP0_tradit
		#endif 
		(
			ON, ACTSPROCESSMODE,
edges0, kvdram, sourcebuffer, vbuffer, globalstatsbuffer, 
			globalparamsK, globalposition, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE,
			resetenv, flush, edgebankID);
			
		voffset_kvs2 += globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
		vreadoffset_kvs2 += vreadskipsz_kvs2;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		#endif 
		#ifdef _DEBUGMODE_STATS_XXX
		actsutilityobj->clearglobalvars();
		#endif
		// exit(EXIT_SUCCESS); ////
	}
	
	MEMACCESSP0_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); ////
	return;
}

void acts_all::TOPP0_NU_partitionit(uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], globalparams_t globalparams, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no TOPP0_NU_partitionit for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nusrcv_nudstv::UTILP0_getglobalparams:: printing global parameters", globalparams);
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	globalposition_t globalposition; // DUMMY.
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparams.ACTSPARAMS_BEGINLOP + 1 + (globalparams.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = UTILP0_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"TOPP0_NU_partitionit: source_partition "<<source_partition<<" ... "<<endl;

			UTILP0_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = UTILP0_getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t ptravstate = UTILP0_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
			
			// collect stats
			MEMACCESSP0_readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
			UTILP0_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### TOPP0_NU_partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			UTILP0_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY) // CRITICAL REMOVEME.
			ACTSP0_actit
			#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
			ACTSP0_priorit	
			#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
			ACTSP0_tradit
			#else 
			ACTSP0_tradit
			#endif
			(config.enablepartition, ACTSPARTITIONMODE,
edges0, kvdram, sourcebuffer, vbuffer, globalstatsbuffer, // CRITICAL FIXME.
					globalparams, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp);
					
			MEMACCESSP0_saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
			
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
			#endif
			#ifdef _DEBUGMODE_STATS_XXX
			actsutilityobj->clearglobalvars();
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		#endif 
		#ifdef _DEBUGMODE_STATS_XXX
		actsutilityobj->clearglobalvars();
		#endif
	}
	return;
}

void acts_all::TOPP0_NU_reduceit(uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	globalposition_t globalposition; // DUMMY.
	
	config_t config;
	sweepparams_t sweepparams;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	#else 
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH + 1;
	#endif 
	
	sweepparams = UTILP0_getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = UTILP0_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### TOPP0_NU_reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
	ACTSP0_actit
	#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
	ACTSP0_priorit	
	#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
	ACTSP0_tradit
	#else 
	ACTSP0_tradit
	#endif
	(config.enablereduce, ACTSREDUCEMODE,
edges0, kvdram, sourcebuffer, vbuffer, globalstatsbuffer, // CRITICAL FIXME.
			globalparams, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp); // REMOVEME.
	return;
}

void acts_all::TOPP0_NU_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	if(en_process == ON){ TOPP0_NU_processit_splitdstvxs(edges0, kvdram, sourcebuffer, vbuffer, vmask_p, globalparamsE, globalparamsK, v_chunkids, v_chunkid, edgebankID); } 
	if(en_partition == ON){ TOPP0_NU_partitionit(edges0, kvdram, sourcebuffer, vbuffer, globalparamsK, NAp); } 
	if(en_reduce == ON){ TOPP0_NU_reduceit(edges0, kvdram, sourcebuffer, vbuffer, sourcestatsmarker, source_partition, globalparamsK, NAp); } 
	return;
}

void acts_all::TOPP0_NU_dispatch_reduce(uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no TOPP0_NU_dispatch_reduce for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
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
	batch_type num_source_partitions = UTILP0_get_num_source_partitions(currentLOP);
	#else 
	batch_type num_source_partitions = NUM_PARTITIONS;	
	#endif

	bool_type enablereduce = ON;
	
	DISPATCHREDUCEP0_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPP0_NU_dispatch_reduce:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, num_source_partitions); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTILP0_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
MEMACCESSP0_readV(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK);
		
		// reduce
		TOPP0_NU_dispatch(OFF, OFF, enablereduce, edges0, kvdram, sourcebuffer, vbuffer, vmask_p, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, v_chunkids, v_chunkid, NAp);
		
		// writeback vertices
MEMACCESSP0_saveVandMs(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK);
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		vmaskp_offset_kvs += NUM_PEs;
	}
	return;
} 

// top
#ifdef CONFIG_ENABLECLASS_TOPNUSRCV_NUDSTV
void acts_all::TOPP0_NU_topkernelproc_embedded(uint512_dt * edges0, uint512_dt * kvdram){
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> Light weight ACTS (PR: 1 ACTS IN 1 COMPUTEUNITS) Launched... size: "<<UTILP0_GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram);
	globalparams_t globalparamsE[MAX_NUM_EDGE_BANKS];
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsE[0] = UTILP0_getglobalparams(edges0);
	
	#else 
	globalparamsE[0] = globalparamsK;
	#endif 
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readpmask(kvdram, vmask_p, globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(kvdram, globalparamsK); }	
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	#endif 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type num_vchunks = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);
	#else 
	batch_type num_vchunks = NUM_PARTITIONS;	
	#endif 
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges0[globalparamsE[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges0[globalparamsE[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int u=0; u<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER+1; u++){ cout<<">>> globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<", PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }			
	#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(kvdram, vbuffer, globalparamsK);
	#endif
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int it_size = 0;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	it_size = NUM_EDGE_BANKS;
	#else 
	it_size = 1;	
	#endif 
	
	for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalparamsK.VARS_WORKBATCH = 0;
		globalparams_t _globalparamsE = globalparamsE[edgebankID];

		for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			// process & partition
			#ifdef CONFIG_ENABLEPROCESSMODULE
			if(globalparamsK.ENABLE_PROCESSCOMMAND == ON){
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelP: processing instance ... "<<endl;
				#endif
				TOPP0_NU_dispatch(ON, OFF, OFF, edges0, kvdram, sourcebuffer, vbuffer, vmask_p, NAp, NAp, _globalparamsE, globalparamsK, PARTITIONP0_CHKPT[edgebankID], v_chunkid, edgebankID);
			}
			#endif
			
			// partition
			#ifdef CONFIG_ENABLEPARTITIONMODULE
			if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON){
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelP: partitioning instance ... "<<endl;
				#endif
				TOPP0_NU_dispatch(OFF, ON, OFF, edges0, kvdram, sourcebuffer, vbuffer, vmask_p, NAp, NAp, _globalparamsE, globalparamsK, PARTITIONP0_CHKPT[edgebankID], v_chunkid, NAp);
			}
			#endif
			
			// reduce & partition
			#if defined(CONFIG_ENABLEREDUCEMODULE)
			if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON){ 
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelP: reducing instance ... "<<endl;
				#endif
				TOPP0_NU_dispatch_reduce(edges0, kvdram, sourcebuffer, vbuffer, vmask_p, _globalparamsE, globalparamsK, PARTITIONP0_CHKPT[edgebankID], v_chunkid, NAp);
			}
			#endif
			
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			MEMACCESSP0_accumkvstats(kvdram, buffer, globalparamsK);
			globalparamsK.VARS_WORKBATCH += 1;
			#endif 
			#ifdef _DEBUGMODE_STATS_XXX
			actsutilityobj->clearglobalvars();
			#endif
		}
		
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		globalparamsK.BASEOFFSETKVS_STATSDRAM += (globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS; 
		#endif
	}
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsK.BASEOFFSETKVS_STATSDRAM = FIRST_BASEOFFSETKVS_STATSDRAM;
	MEMACCESSP0_commitkvstats(kvdram, buffer, globalparamsK);
	#endif
	UTILP0_increment_graphiteration(kvdram, globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void acts_all::TOPP0_NU_topkernelP1(
uint512_dt * edges00,	uint512_dt * kvdram0,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs1(kvdram0, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP1: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP1: processing instance 0"<<endl;
	#endif 
 */

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs1(kvdram0, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP2(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs2(kvdram0,kvdram1, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP2: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP2: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP2: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP2: processing instance 1"<<endl;
	#endif 
 */

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs2(kvdram0,kvdram1, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP3(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs3(kvdram0,kvdram1,kvdram2, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP3: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
 */

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs3(kvdram0,kvdram1,kvdram2, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP4(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs4(kvdram0,kvdram1,kvdram2,kvdram3, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP4: processing instance 3"<<endl;
	#endif 
 */

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs4(kvdram0,kvdram1,kvdram2,kvdram3, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP5(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem5
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem5
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
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
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs5(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 4"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges40, kvdram4);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
	exit(EXIT_SUCCESS); //	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 3"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP5: processing instance 4"<<endl;
	#endif 
 */

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs5(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP6(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6
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
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
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
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
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
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs6(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 4"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges40, kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 5"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges50, kvdram5);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP6: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
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

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs6(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP7(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem7
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
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
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
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
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
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs7(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 4"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges40, kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 5"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges50, kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 6"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges60, kvdram6);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP7: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
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

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs7(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP8(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem8
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
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
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
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
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
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs8(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 4"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges40, kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 5"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges50, kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 6"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges60, kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 7"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges70, kvdram7);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP8: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
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

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs8(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP9(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
uint512_dt * edges80,	uint512_dt * kvdram8,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges80 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem9
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
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
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
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = edges80
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
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs9(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 4"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges40, kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 5"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges50, kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 6"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges60, kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 7"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges70, kvdram7);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 8"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges80, kvdram8);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP9: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
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

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs9(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP10(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
uint512_dt * edges80,	uint512_dt * kvdram8,
uint512_dt * edges90,	uint512_dt * kvdram9,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges80 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = edges90 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem10
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
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
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
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = edges90
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
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs10(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 4"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges40, kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 5"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges50, kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 6"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges60, kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 7"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges70, kvdram7);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 8"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges80, kvdram8);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 9"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges90, kvdram9);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP10: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
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

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs10(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP11(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
uint512_dt * edges80,	uint512_dt * kvdram8,
uint512_dt * edges90,	uint512_dt * kvdram9,
uint512_dt * edges100,	uint512_dt * kvdram10,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges80 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = edges90 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges100 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem11
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
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
#pragma HLS INTERFACE s_axilite port = edges100 bundle = control
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
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = edges90
#pragma HLS DATA_PACK variable = edges100
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
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs11(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 4"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges40, kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 5"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges50, kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 6"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges60, kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 7"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges70, kvdram7);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 8"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges80, kvdram8);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 9"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges90, kvdram9);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 10"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges100, kvdram10);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP11: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
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

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs11(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, vdram, globalparams, globalparamsV);
	return;
}
}
extern "C" {
void acts_all::TOPP0_NU_topkernelP12(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
uint512_dt * edges70,	uint512_dt * kvdram7,
uint512_dt * edges80,	uint512_dt * kvdram8,
uint512_dt * edges90,	uint512_dt * kvdram9,
uint512_dt * edges100,	uint512_dt * kvdram10,
uint512_dt * edges110,	uint512_dt * kvdram11,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges80 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = edges90 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges100 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = edges110 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem12
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
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
#pragma HLS INTERFACE s_axilite port = edges100 bundle = control
#pragma HLS INTERFACE s_axilite port = edges110 bundle = control
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
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = edges90
#pragma HLS DATA_PACK variable = edges100
#pragma HLS DATA_PACK variable = edges110
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
	
	unsigned int GraphIter = UTILP0_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	globalparams_t globalparams;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> top_nusrcv_nudstv::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// CRITICAL REMOVEME.
	if(GraphIter > 0){ MERGEP0_SPLIT_broadcastVs12(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, vdram); }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 0"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges00, kvdram0);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 1"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges10, kvdram1);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges20, kvdram2);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 3"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges30, kvdram3);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 4"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges40, kvdram4);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 5"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges50, kvdram5);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 6"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges60, kvdram6);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 7"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges70, kvdram7);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 8"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges80, kvdram8);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 9"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges90, kvdram9);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 10"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges100, kvdram10);	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 11"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(edges110, kvdram11);	
	
	/* 	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 0"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 1"<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> topkernelP12: processing instance 2"<<endl;
	#endif 
	TOPP0_NU_topkernelproc_embedded(kvdram2);	
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

	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
MERGEP0_SPLIT_mergeVs12(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, vdram, globalparams, globalparamsV);
	return;
}
}
#endif



