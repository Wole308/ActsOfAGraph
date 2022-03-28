void acts_all::TOPP0_NU_processit_splitdstvxs(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_NEXTPMASK_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,								
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; 
	
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
		
		globalposition.source_partition = source_partition;
		globalposition.first_source_partition = 0;
		
		if(voffset_kvs2 >= avtravstate.end_kvs){ voffset_kvs2 += globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION; vreadoffset_kvs2 += vreadskipsz_kvs2; continue; }
		if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE) { if(pmask_curr[source_partition] == 0){ voffset_kvs2 += globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION; vreadoffset_kvs2 += vreadskipsz_kvs2; continue; }}
		#ifdef _DEBUGMODE_KERNELPRINTsS
		if(pmask_curr[source_partition] > 0){ cout<<"TOPP0_NU_processit_splitdstvxs:: source partition is active [pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<"] ..."<<endl; }
		#endif 
		
		sweepparams.source_partition = source_partition;
		avtravstate.i_kvs = voffset_kvs2;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print5("### TOPP0_NU_processit_splitdstvxs:: source_partition", "voffset", "vbegin", "vend", "vskip", source_partition, voffset_kvs2 * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		MEMACCESSP0_readANDRVchunks1(ON, kvdram, vbuffer, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsK);
	
		vertex_t srcvlocaloffset = (voffset_kvs2 * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs2 * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
		tuple_t tup = MEMACCESSP0_getvptrs_opt( kvdram, vptrbaseoffset_kvs, voffset_kvs2 * VECTOR2_SIZE, (voffset_kvs2 + SKIP_KVS) * VECTOR2_SIZE, edgebankID); // CRITICAL NEWCHANGE.
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
			 kvdram, sourcebuffer, vbuffer, globalstatsbuffer, 
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

void acts_all::TOPP0_NU_partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], globalparams_t globalparams, unsigned int edgebankID){
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; 
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
					kvdram, sourcebuffer, vbuffer, globalstatsbuffer, // CRITICAL FIXME.
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

void acts_all::TOPP0_NU_reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; 
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
	UTILP0_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);

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
			 kvdram, sourcebuffer, vbuffer, globalstatsbuffer, // CRITICAL FIXME.
			globalparams, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp); // REMOVEME.
	return;
}

void acts_all::TOPP0_NU_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_NEXTPMASK_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	if(en_process == ON){ TOPP0_NU_processit_splitdstvxs( kvdram, sourcebuffer, vbuffer, pmask_curr, globalparamsE, globalparamsK, v_chunkids, v_chunkid, edgebankID); } 
	if(en_partition == ON){ TOPP0_NU_partitionit( kvdram, sourcebuffer, vbuffer, globalparamsK, NAp); } 
	if(en_reduce == ON){ TOPP0_NU_reduceit( kvdram, sourcebuffer, vbuffer, sourcestatsmarker, source_partition, globalparamsK, NAp); } 
	return;
}




