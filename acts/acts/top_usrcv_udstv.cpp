void TOPP0_U_processit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt partition_is_active, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	for(buffer_type t=0; t<globalparamsK.SIZEKVS2_REDUCEPARTITION; t++){
		for(buffer_type v=0; v<VDATA_PACKINGSIZE; v++){
			if(MEMCAP0_READVMASK(vbuffer[v][t]) == 1){ cout<<"TOPP0_U_processit: ACTIVE MASK SEEN: @ t: "<<t<<", v: "<<v<<", vbuffer["<<v<<"]["<<t<<"]: "<<vbuffer[v][t]<<endl; }
		}
	}
	#endif

	sweepparams_t sweepparams;
	
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP0_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0); } 
	sweepparams = UTILP0_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	
	if(globalposition.source_partition == globalposition.first_source_partition){ MEMACCESSP0_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL NEWCHANGE.
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP0_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0); } // CRITICAL NEWCHANGE.

	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print7("### TOPP0_U_processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif

	batch_type voffset_kvs = globalposition.source_partition * UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	bool returncmd = false;
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	if(partition_is_active == 0){ returncmd = true; } // check if vertex partition is active
	else{
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; 
		#endif 
	}
	#endif 
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ returncmd = false; } // for certain algorithms like pagerank
	if(voffset_kvs >= avtravstate.end_kvs){ returncmd = true; } // check if we are at the end of file
	
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && globalposition.stage == 0 && globalposition.source_partition == globalposition.first_source_partition){
		MEMACCESSP0_readV(ON, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, BLOCKRAM_VDATA_SIZE/2, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
	}
	#endif 
	
	if(returncmd == false){ // CRITICAL REMOVEME...............................................................................................
		sweepparams.source_partition = globalposition.source_partition;
		avtravstate.i_kvs = voffset_kvs;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print5("### TOPP0_U_processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + (UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + (UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
			
		unsigned int SKIP_KVS = UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
		
		tuple_t tup = MEMACCESSP0_getvptrs_opt(kvdram, vptrbaseoffset_kvs, voffset_kvs * VECTOR2_SIZE, (voffset_kvs + SKIP_KVS) * VECTOR2_SIZE, edgebankID);
		keyy_t beginvptr = tup.A;
		keyy_t endvptr = tup.B; 
		#ifdef _DEBUGMODE_CHECKS3
		if(globalparamsK.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ if(beginvptr % VECTOR2_SIZE != 0 || endvptr % VECTOR2_SIZE != 0){ cout<<"TOPP0_U_processit::ERROR: beginvptr("<<beginvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0 || endvptr("<<endvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }}
		#endif 
		
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr;
		keyy_t localendvptr = endvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 1*VECTOR_SIZE; // CRITICAL NEWCHANGE.
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN // FIXME. REMOVEME WHEN IN RUNNING MODE.
		if(sweepparams.source_partition < 64){
			UTILP0_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + sweepparams.source_partition, localbeginvptr_kvs);
			UTILP0_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + sweepparams.source_partition, numedges); 
		}
		collections[0][0].data1 += numedges;
		#endif 
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ collections[0][1].data1 += numedges; }
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS3
		if(localendvptr < localbeginvptr){ cout<<"TOPP0_U_processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<", voffset_kvs: "<<voffset_kvs<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("TOPP0_U_processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"[index: "<<globalposition.source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"]"<<endl;
		#endif
		#ifdef _DEBUGMODE_CHECKS3
		if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->clearallstats();
		#endif
		
		travstate_t etravstate;
		etravstate.begin_kvs = localbeginvptr_kvs;
		etravstate.size_kvs = numedges_kvs;
		etravstate.end_kvs = etravstate.begin_kvs + etravstate.size_kvs;
		etravstate.skip_kvs = SRCBUFFER_SIZE;
		etravstate.i_kvs = etravstate.begin_kvs;
		etravstate.i2 = voffset_kvs * VECTOR2_SIZE;
		etravstate.i2_kvs = voffset_kvs;
		bool_type resetenv = ON;
		bool_type flush = ON;
		
		if(voffset_kvs == avtravstate.begin_kvs || globalposition.source_partition == globalposition.first_source_partition){ resetenv = ON; } else { resetenv = OFF; }
		if((voffset_kvs + UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		// if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ 
		resetenv = ON; flush = ON; 
		// } // CRITICAL NEWCHANGE.
		
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
			resetenv, flush, edgebankID, collections);
	}
	
	bool endofrun = false;
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ endofrun = true; }
	
	if(endofrun == true){ MEMACCESSP0_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL OPTIMIZEME. should be called only once
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPP0_U_processit", globalstatsbuffer, NUM_PARTITIONS); }
	#endif	
	
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && endofrun == true){ 
		MEMACCESSP0_saveV(ON, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, BLOCKRAM_VDATA_SIZE/2, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][1].data1);
		#endif
	}
	#endif
	
	#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
	if(endofrun == true){ UTILP0_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][0].data1); }
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

void TOPP0_U_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt partition_is_active, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(en_process == ON){ TOPP0_U_processit(kvdram, sourcebuffer, vbuffer, partition_is_active, globalstatsbuffer, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(en_partition == ON){ TOPP0_NU_partitionit( kvdram, sourcebuffer, vbuffer, globalparamsK, NAp, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(en_reduce == ON){ TOPP0_NU_reduceit(kvdram, sourcebuffer, vbuffer, sourcestatsmarker, source_partition, globalparamsK, NAp, collections); } 
	#endif 
	return;
}

void TOPP0_U_dispatch_reduce(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no TOPP0_U_dispatch_reduce for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //////
	
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int vmask_offset_kvs = 0;
	unsigned int vmaskp_offset_kvs = 0;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = UTILP0_get_num_source_partitions(currentLOP);

	bool_type enablereduce = ON;
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; // NOT USED.
	
	UTILP0_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	DISPATCHREDUCEP0_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### TOPP0_U_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "vreadoffset_kvs2", source_partition, currentLOP, num_source_partitions, vreadoffset_kvs2); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTILP0_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		globalstatsbuffer[source_partition].key = NAp; globalstatsbuffer[source_partition].value = rtravstate.size_kvs;
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		collections[0][1].data1 += rtravstate.size_kvs * VECTOR_SIZE;
		#endif 
		
		// read vertices
		MEMACCESSP0_readV(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		
		// reduce
		TOPP0_U_dispatch(OFF, OFF, enablereduce, kvdram, sourcebuffer, vbuffer, NAp, globalstatsbuffer, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);

		// writeback vertices
		MEMACCESSP0_saveV(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		vmaskp_offset_kvs += NUM_PEs;
	}
	
	#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN // NEWCHANGE
	UTILP0_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][1].data1);
	#endif 
	// exit(EXIT_SUCCESS);
	return;
} 

void TOPP0_U_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt partition_is_active, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){									

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	unsigned int printheader1=OFF; // ON, OFF*
	unsigned int printheader2=OFF; 
	if(true && globalposition.source_partition==globalposition.first_source_partition){ printheader1 = ON; } else { printheader1 = OFF; }
	if(false && globalposition.source_partition==globalposition.last_source_partition){ printheader2 = ON; } else { printheader2 = OFF; }
	// printheader1=ON;
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== ACTS Launched... size: "<<UTILP0_GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl;
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
	globalparams_t globalparamsK;
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	globalparams_t _globalparamsE;
	
	globalparamsK = UTILP0_getglobalparams(kvdram, 0); // CRITICAL OPTIMIZEME. MOVETOBASE?
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){ 
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram, e+1); }} 
	else { globalparamsEs[0] = globalparamsK; }
	_globalparamsE = globalparamsEs[globalposition.edgebankID];
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE. IS THIS OPTIMIZED
	}

	globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = GraphIter; // overrride these with GraphIter
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e].ALGORITHMINFO_GRAPHITERATIONID = GraphIter; }
	_globalparamsE.ALGORITHMINFO_GRAPHITERATIONID = GraphIter;
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	// if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ UTILP0_resetkvstatvalues(kvdram, globalparamsK); } // NEWCHANGE
	#endif 
	// }	// CRITICAL FIXME. NEWCHANGE. DO FOR ALL.
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, sourcebuffer, vbuffer, partition_is_active, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, globalposition.edgebankID, collections); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, sourcebuffer, vbuffer, partition_is_active, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	// reduce & partition
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch_reduce(kvdram, sourcebuffer, vbuffer, _globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	bool endofrun = false;
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }} 
	else { if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }}
	#if not defined(FPGA_IMPL) && defined(CONFIG_COLLECT_DATAS3_DURING_RUN)
	if(endofrun == true){
		// cout<<"------------------ tempsum1: "<<actsutilityobj->globalstats_getcountnumvalidprocessedges(globalparamsK.ACTSPARAMS_INSTID)<<", tempsum2: "<<actsutilityobj->globalstats_getcountvalidkvsreduced(globalparamsK.ACTSPARAMS_INSTID)<<endl;
		UTILP0_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, actsutilityobj->globalstats_getcountnumvalidprocessedges(globalparamsK.ACTSPARAMS_INSTID));
		UTILP0_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, actsutilityobj->globalstats_getcountvalidkvsreduced(globalparamsK.ACTSPARAMS_INSTID));
	}
	#endif 
	if(endofrun == true){
		unsigned int _offset = MERGEP0_actvpstatsoffset(globalparamsK);
		MEMACCESSP0_retreievekvstats(kvdram, globalstatsbuffer, globalparamsK, _offset, globalparamsK.NUM_REDUCEPARTITIONS);
		UTILP0_increment_graphiteration(kvdram, globalparamsK); } // NB: this should come last.
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(endofrun == true){ actsutilityobj->printglobalvars2(globalparamsK.ACTSPARAMS_INSTID, "instID", "v_chunkid", "LOP", "sourcep", globalparamsK.ACTSPARAMS_INSTID, globalposition.v_chunkid, globalposition.currentLOP, globalposition.source_partition); }
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void TOPP0_U_topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[1];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL1_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL1_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL1_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL1_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL1_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks1(ON, vdram, vbuffer0, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks1(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP1"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[2];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL2_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL2_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL2_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL2_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL2_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks2(ON, vdram, vbuffer0,vbuffer1, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks2(ON, vdram, vbuffer0,vbuffer1, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP2"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[3];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL3_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL3_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL3_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL3_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL3_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks3(ON, vdram, vbuffer0,vbuffer1,vbuffer2, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks3(ON, vdram, vbuffer0,vbuffer1,vbuffer2, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP3"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[4];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL4_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL4_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL4_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL4_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL4_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks4(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks4(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP4"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem5

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[5];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
		vbuffer4[0][i] = 0;
		vbuffer4[1][i] = 0;
		vbuffer4[2][i] = 0;
		vbuffer4[3][i] = 0;
		vbuffer4[4][i] = 0;
		vbuffer4[5][i] = 0;
		vbuffer4[6][i] = 0;
		vbuffer4[7][i] = 0;
		vbuffer4[8][i] = 0;
		vbuffer4[9][i] = 0;
		vbuffer4[10][i] = 0;
		vbuffer4[11][i] = 0;
		vbuffer4[12][i] = 0;
		vbuffer4[13][i] = 0;
		vbuffer4[14][i] = 0;
		vbuffer4[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL5_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL5_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL5_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL5_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL5_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks5(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks5(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, partition_is_active, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGEP0_mergeVs(kvdram4, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP5"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[6];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
		vbuffer4[0][i] = 0;
		vbuffer4[1][i] = 0;
		vbuffer4[2][i] = 0;
		vbuffer4[3][i] = 0;
		vbuffer4[4][i] = 0;
		vbuffer4[5][i] = 0;
		vbuffer4[6][i] = 0;
		vbuffer4[7][i] = 0;
		vbuffer4[8][i] = 0;
		vbuffer4[9][i] = 0;
		vbuffer4[10][i] = 0;
		vbuffer4[11][i] = 0;
		vbuffer4[12][i] = 0;
		vbuffer4[13][i] = 0;
		vbuffer4[14][i] = 0;
		vbuffer4[15][i] = 0;
	
		vbuffer5[0][i] = 0;
		vbuffer5[1][i] = 0;
		vbuffer5[2][i] = 0;
		vbuffer5[3][i] = 0;
		vbuffer5[4][i] = 0;
		vbuffer5[5][i] = 0;
		vbuffer5[6][i] = 0;
		vbuffer5[7][i] = 0;
		vbuffer5[8][i] = 0;
		vbuffer5[9][i] = 0;
		vbuffer5[10][i] = 0;
		vbuffer5[11][i] = 0;
		vbuffer5[12][i] = 0;
		vbuffer5[13][i] = 0;
		vbuffer5[14][i] = 0;
		vbuffer5[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL6_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL6_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL6_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL6_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL6_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks6(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks6(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, partition_is_active, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, partition_is_active, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGEP0_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGEP0_mergeVs(kvdram5, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP6"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem7

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5

#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[7];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
		vbuffer4[0][i] = 0;
		vbuffer4[1][i] = 0;
		vbuffer4[2][i] = 0;
		vbuffer4[3][i] = 0;
		vbuffer4[4][i] = 0;
		vbuffer4[5][i] = 0;
		vbuffer4[6][i] = 0;
		vbuffer4[7][i] = 0;
		vbuffer4[8][i] = 0;
		vbuffer4[9][i] = 0;
		vbuffer4[10][i] = 0;
		vbuffer4[11][i] = 0;
		vbuffer4[12][i] = 0;
		vbuffer4[13][i] = 0;
		vbuffer4[14][i] = 0;
		vbuffer4[15][i] = 0;
	
		vbuffer5[0][i] = 0;
		vbuffer5[1][i] = 0;
		vbuffer5[2][i] = 0;
		vbuffer5[3][i] = 0;
		vbuffer5[4][i] = 0;
		vbuffer5[5][i] = 0;
		vbuffer5[6][i] = 0;
		vbuffer5[7][i] = 0;
		vbuffer5[8][i] = 0;
		vbuffer5[9][i] = 0;
		vbuffer5[10][i] = 0;
		vbuffer5[11][i] = 0;
		vbuffer5[12][i] = 0;
		vbuffer5[13][i] = 0;
		vbuffer5[14][i] = 0;
		vbuffer5[15][i] = 0;
	
		vbuffer6[0][i] = 0;
		vbuffer6[1][i] = 0;
		vbuffer6[2][i] = 0;
		vbuffer6[3][i] = 0;
		vbuffer6[4][i] = 0;
		vbuffer6[5][i] = 0;
		vbuffer6[6][i] = 0;
		vbuffer6[7][i] = 0;
		vbuffer6[8][i] = 0;
		vbuffer6[9][i] = 0;
		vbuffer6[10][i] = 0;
		vbuffer6[11][i] = 0;
		vbuffer6[12][i] = 0;
		vbuffer6[13][i] = 0;
		vbuffer6[14][i] = 0;
		vbuffer6[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL7_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL7_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL7_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL7_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL7_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks7(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks7(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, partition_is_active, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, partition_is_active, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, partition_is_active, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGEP0_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGEP0_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGEP0_mergeVs(kvdram6, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP7"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP8(
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
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem8

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


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5

#pragma HLS DATA_PACK variable = kvdram6

#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[8];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
		vbuffer4[0][i] = 0;
		vbuffer4[1][i] = 0;
		vbuffer4[2][i] = 0;
		vbuffer4[3][i] = 0;
		vbuffer4[4][i] = 0;
		vbuffer4[5][i] = 0;
		vbuffer4[6][i] = 0;
		vbuffer4[7][i] = 0;
		vbuffer4[8][i] = 0;
		vbuffer4[9][i] = 0;
		vbuffer4[10][i] = 0;
		vbuffer4[11][i] = 0;
		vbuffer4[12][i] = 0;
		vbuffer4[13][i] = 0;
		vbuffer4[14][i] = 0;
		vbuffer4[15][i] = 0;
	
		vbuffer5[0][i] = 0;
		vbuffer5[1][i] = 0;
		vbuffer5[2][i] = 0;
		vbuffer5[3][i] = 0;
		vbuffer5[4][i] = 0;
		vbuffer5[5][i] = 0;
		vbuffer5[6][i] = 0;
		vbuffer5[7][i] = 0;
		vbuffer5[8][i] = 0;
		vbuffer5[9][i] = 0;
		vbuffer5[10][i] = 0;
		vbuffer5[11][i] = 0;
		vbuffer5[12][i] = 0;
		vbuffer5[13][i] = 0;
		vbuffer5[14][i] = 0;
		vbuffer5[15][i] = 0;
	
		vbuffer6[0][i] = 0;
		vbuffer6[1][i] = 0;
		vbuffer6[2][i] = 0;
		vbuffer6[3][i] = 0;
		vbuffer6[4][i] = 0;
		vbuffer6[5][i] = 0;
		vbuffer6[6][i] = 0;
		vbuffer6[7][i] = 0;
		vbuffer6[8][i] = 0;
		vbuffer6[9][i] = 0;
		vbuffer6[10][i] = 0;
		vbuffer6[11][i] = 0;
		vbuffer6[12][i] = 0;
		vbuffer6[13][i] = 0;
		vbuffer6[14][i] = 0;
		vbuffer6[15][i] = 0;
	
		vbuffer7[0][i] = 0;
		vbuffer7[1][i] = 0;
		vbuffer7[2][i] = 0;
		vbuffer7[3][i] = 0;
		vbuffer7[4][i] = 0;
		vbuffer7[5][i] = 0;
		vbuffer7[6][i] = 0;
		vbuffer7[7][i] = 0;
		vbuffer7[8][i] = 0;
		vbuffer7[9][i] = 0;
		vbuffer7[10][i] = 0;
		vbuffer7[11][i] = 0;
		vbuffer7[12][i] = 0;
		vbuffer7[13][i] = 0;
		vbuffer7[14][i] = 0;
		vbuffer7[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL8_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL8_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL8_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL8_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL8_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks8(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks8(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, partition_is_active, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, partition_is_active, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, partition_is_active, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, partition_is_active, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGEP0_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGEP0_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGEP0_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGEP0_mergeVs(kvdram7, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP8"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP9(
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

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[9];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
		vbuffer4[0][i] = 0;
		vbuffer4[1][i] = 0;
		vbuffer4[2][i] = 0;
		vbuffer4[3][i] = 0;
		vbuffer4[4][i] = 0;
		vbuffer4[5][i] = 0;
		vbuffer4[6][i] = 0;
		vbuffer4[7][i] = 0;
		vbuffer4[8][i] = 0;
		vbuffer4[9][i] = 0;
		vbuffer4[10][i] = 0;
		vbuffer4[11][i] = 0;
		vbuffer4[12][i] = 0;
		vbuffer4[13][i] = 0;
		vbuffer4[14][i] = 0;
		vbuffer4[15][i] = 0;
	
		vbuffer5[0][i] = 0;
		vbuffer5[1][i] = 0;
		vbuffer5[2][i] = 0;
		vbuffer5[3][i] = 0;
		vbuffer5[4][i] = 0;
		vbuffer5[5][i] = 0;
		vbuffer5[6][i] = 0;
		vbuffer5[7][i] = 0;
		vbuffer5[8][i] = 0;
		vbuffer5[9][i] = 0;
		vbuffer5[10][i] = 0;
		vbuffer5[11][i] = 0;
		vbuffer5[12][i] = 0;
		vbuffer5[13][i] = 0;
		vbuffer5[14][i] = 0;
		vbuffer5[15][i] = 0;
	
		vbuffer6[0][i] = 0;
		vbuffer6[1][i] = 0;
		vbuffer6[2][i] = 0;
		vbuffer6[3][i] = 0;
		vbuffer6[4][i] = 0;
		vbuffer6[5][i] = 0;
		vbuffer6[6][i] = 0;
		vbuffer6[7][i] = 0;
		vbuffer6[8][i] = 0;
		vbuffer6[9][i] = 0;
		vbuffer6[10][i] = 0;
		vbuffer6[11][i] = 0;
		vbuffer6[12][i] = 0;
		vbuffer6[13][i] = 0;
		vbuffer6[14][i] = 0;
		vbuffer6[15][i] = 0;
	
		vbuffer7[0][i] = 0;
		vbuffer7[1][i] = 0;
		vbuffer7[2][i] = 0;
		vbuffer7[3][i] = 0;
		vbuffer7[4][i] = 0;
		vbuffer7[5][i] = 0;
		vbuffer7[6][i] = 0;
		vbuffer7[7][i] = 0;
		vbuffer7[8][i] = 0;
		vbuffer7[9][i] = 0;
		vbuffer7[10][i] = 0;
		vbuffer7[11][i] = 0;
		vbuffer7[12][i] = 0;
		vbuffer7[13][i] = 0;
		vbuffer7[14][i] = 0;
		vbuffer7[15][i] = 0;
	
		vbuffer8[0][i] = 0;
		vbuffer8[1][i] = 0;
		vbuffer8[2][i] = 0;
		vbuffer8[3][i] = 0;
		vbuffer8[4][i] = 0;
		vbuffer8[5][i] = 0;
		vbuffer8[6][i] = 0;
		vbuffer8[7][i] = 0;
		vbuffer8[8][i] = 0;
		vbuffer8[9][i] = 0;
		vbuffer8[10][i] = 0;
		vbuffer8[11][i] = 0;
		vbuffer8[12][i] = 0;
		vbuffer8[13][i] = 0;
		vbuffer8[14][i] = 0;
		vbuffer8[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL9_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL9_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL9_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL9_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL9_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks9(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks9(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, partition_is_active, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, partition_is_active, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, partition_is_active, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, partition_is_active, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, partition_is_active, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGEP0_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGEP0_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGEP0_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGEP0_mergeVs(kvdram7, vdram);
	
	globalparams_t globalparamsK8;
	MERGEP0_mergeVs(kvdram8, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP9"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP10(
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

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[10];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
		vbuffer4[0][i] = 0;
		vbuffer4[1][i] = 0;
		vbuffer4[2][i] = 0;
		vbuffer4[3][i] = 0;
		vbuffer4[4][i] = 0;
		vbuffer4[5][i] = 0;
		vbuffer4[6][i] = 0;
		vbuffer4[7][i] = 0;
		vbuffer4[8][i] = 0;
		vbuffer4[9][i] = 0;
		vbuffer4[10][i] = 0;
		vbuffer4[11][i] = 0;
		vbuffer4[12][i] = 0;
		vbuffer4[13][i] = 0;
		vbuffer4[14][i] = 0;
		vbuffer4[15][i] = 0;
	
		vbuffer5[0][i] = 0;
		vbuffer5[1][i] = 0;
		vbuffer5[2][i] = 0;
		vbuffer5[3][i] = 0;
		vbuffer5[4][i] = 0;
		vbuffer5[5][i] = 0;
		vbuffer5[6][i] = 0;
		vbuffer5[7][i] = 0;
		vbuffer5[8][i] = 0;
		vbuffer5[9][i] = 0;
		vbuffer5[10][i] = 0;
		vbuffer5[11][i] = 0;
		vbuffer5[12][i] = 0;
		vbuffer5[13][i] = 0;
		vbuffer5[14][i] = 0;
		vbuffer5[15][i] = 0;
	
		vbuffer6[0][i] = 0;
		vbuffer6[1][i] = 0;
		vbuffer6[2][i] = 0;
		vbuffer6[3][i] = 0;
		vbuffer6[4][i] = 0;
		vbuffer6[5][i] = 0;
		vbuffer6[6][i] = 0;
		vbuffer6[7][i] = 0;
		vbuffer6[8][i] = 0;
		vbuffer6[9][i] = 0;
		vbuffer6[10][i] = 0;
		vbuffer6[11][i] = 0;
		vbuffer6[12][i] = 0;
		vbuffer6[13][i] = 0;
		vbuffer6[14][i] = 0;
		vbuffer6[15][i] = 0;
	
		vbuffer7[0][i] = 0;
		vbuffer7[1][i] = 0;
		vbuffer7[2][i] = 0;
		vbuffer7[3][i] = 0;
		vbuffer7[4][i] = 0;
		vbuffer7[5][i] = 0;
		vbuffer7[6][i] = 0;
		vbuffer7[7][i] = 0;
		vbuffer7[8][i] = 0;
		vbuffer7[9][i] = 0;
		vbuffer7[10][i] = 0;
		vbuffer7[11][i] = 0;
		vbuffer7[12][i] = 0;
		vbuffer7[13][i] = 0;
		vbuffer7[14][i] = 0;
		vbuffer7[15][i] = 0;
	
		vbuffer8[0][i] = 0;
		vbuffer8[1][i] = 0;
		vbuffer8[2][i] = 0;
		vbuffer8[3][i] = 0;
		vbuffer8[4][i] = 0;
		vbuffer8[5][i] = 0;
		vbuffer8[6][i] = 0;
		vbuffer8[7][i] = 0;
		vbuffer8[8][i] = 0;
		vbuffer8[9][i] = 0;
		vbuffer8[10][i] = 0;
		vbuffer8[11][i] = 0;
		vbuffer8[12][i] = 0;
		vbuffer8[13][i] = 0;
		vbuffer8[14][i] = 0;
		vbuffer8[15][i] = 0;
	
		vbuffer9[0][i] = 0;
		vbuffer9[1][i] = 0;
		vbuffer9[2][i] = 0;
		vbuffer9[3][i] = 0;
		vbuffer9[4][i] = 0;
		vbuffer9[5][i] = 0;
		vbuffer9[6][i] = 0;
		vbuffer9[7][i] = 0;
		vbuffer9[8][i] = 0;
		vbuffer9[9][i] = 0;
		vbuffer9[10][i] = 0;
		vbuffer9[11][i] = 0;
		vbuffer9[12][i] = 0;
		vbuffer9[13][i] = 0;
		vbuffer9[14][i] = 0;
		vbuffer9[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL10_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL10_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL10_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL10_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL10_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks10(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks10(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, partition_is_active, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, partition_is_active, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, partition_is_active, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, partition_is_active, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, partition_is_active, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, partition_is_active, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGEP0_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGEP0_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGEP0_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGEP0_mergeVs(kvdram7, vdram);
	
	globalparams_t globalparamsK8;
	MERGEP0_mergeVs(kvdram8, vdram);
	
	globalparams_t globalparamsK9;
	MERGEP0_mergeVs(kvdram9, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP10"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP11(
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

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE];
	collection_t collections10[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[11];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
		vbuffer4[0][i] = 0;
		vbuffer4[1][i] = 0;
		vbuffer4[2][i] = 0;
		vbuffer4[3][i] = 0;
		vbuffer4[4][i] = 0;
		vbuffer4[5][i] = 0;
		vbuffer4[6][i] = 0;
		vbuffer4[7][i] = 0;
		vbuffer4[8][i] = 0;
		vbuffer4[9][i] = 0;
		vbuffer4[10][i] = 0;
		vbuffer4[11][i] = 0;
		vbuffer4[12][i] = 0;
		vbuffer4[13][i] = 0;
		vbuffer4[14][i] = 0;
		vbuffer4[15][i] = 0;
	
		vbuffer5[0][i] = 0;
		vbuffer5[1][i] = 0;
		vbuffer5[2][i] = 0;
		vbuffer5[3][i] = 0;
		vbuffer5[4][i] = 0;
		vbuffer5[5][i] = 0;
		vbuffer5[6][i] = 0;
		vbuffer5[7][i] = 0;
		vbuffer5[8][i] = 0;
		vbuffer5[9][i] = 0;
		vbuffer5[10][i] = 0;
		vbuffer5[11][i] = 0;
		vbuffer5[12][i] = 0;
		vbuffer5[13][i] = 0;
		vbuffer5[14][i] = 0;
		vbuffer5[15][i] = 0;
	
		vbuffer6[0][i] = 0;
		vbuffer6[1][i] = 0;
		vbuffer6[2][i] = 0;
		vbuffer6[3][i] = 0;
		vbuffer6[4][i] = 0;
		vbuffer6[5][i] = 0;
		vbuffer6[6][i] = 0;
		vbuffer6[7][i] = 0;
		vbuffer6[8][i] = 0;
		vbuffer6[9][i] = 0;
		vbuffer6[10][i] = 0;
		vbuffer6[11][i] = 0;
		vbuffer6[12][i] = 0;
		vbuffer6[13][i] = 0;
		vbuffer6[14][i] = 0;
		vbuffer6[15][i] = 0;
	
		vbuffer7[0][i] = 0;
		vbuffer7[1][i] = 0;
		vbuffer7[2][i] = 0;
		vbuffer7[3][i] = 0;
		vbuffer7[4][i] = 0;
		vbuffer7[5][i] = 0;
		vbuffer7[6][i] = 0;
		vbuffer7[7][i] = 0;
		vbuffer7[8][i] = 0;
		vbuffer7[9][i] = 0;
		vbuffer7[10][i] = 0;
		vbuffer7[11][i] = 0;
		vbuffer7[12][i] = 0;
		vbuffer7[13][i] = 0;
		vbuffer7[14][i] = 0;
		vbuffer7[15][i] = 0;
	
		vbuffer8[0][i] = 0;
		vbuffer8[1][i] = 0;
		vbuffer8[2][i] = 0;
		vbuffer8[3][i] = 0;
		vbuffer8[4][i] = 0;
		vbuffer8[5][i] = 0;
		vbuffer8[6][i] = 0;
		vbuffer8[7][i] = 0;
		vbuffer8[8][i] = 0;
		vbuffer8[9][i] = 0;
		vbuffer8[10][i] = 0;
		vbuffer8[11][i] = 0;
		vbuffer8[12][i] = 0;
		vbuffer8[13][i] = 0;
		vbuffer8[14][i] = 0;
		vbuffer8[15][i] = 0;
	
		vbuffer9[0][i] = 0;
		vbuffer9[1][i] = 0;
		vbuffer9[2][i] = 0;
		vbuffer9[3][i] = 0;
		vbuffer9[4][i] = 0;
		vbuffer9[5][i] = 0;
		vbuffer9[6][i] = 0;
		vbuffer9[7][i] = 0;
		vbuffer9[8][i] = 0;
		vbuffer9[9][i] = 0;
		vbuffer9[10][i] = 0;
		vbuffer9[11][i] = 0;
		vbuffer9[12][i] = 0;
		vbuffer9[13][i] = 0;
		vbuffer9[14][i] = 0;
		vbuffer9[15][i] = 0;
	
		vbuffer10[0][i] = 0;
		vbuffer10[1][i] = 0;
		vbuffer10[2][i] = 0;
		vbuffer10[3][i] = 0;
		vbuffer10[4][i] = 0;
		vbuffer10[5][i] = 0;
		vbuffer10[6][i] = 0;
		vbuffer10[7][i] = 0;
		vbuffer10[8][i] = 0;
		vbuffer10[9][i] = 0;
		vbuffer10[10][i] = 0;
		vbuffer10[11][i] = 0;
		vbuffer10[12][i] = 0;
		vbuffer10[13][i] = 0;
		vbuffer10[14][i] = 0;
		vbuffer10[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  collections10[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL11_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL11_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL11_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL11_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL11_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks11(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks11(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, partition_is_active, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, partition_is_active, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, partition_is_active, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, partition_is_active, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, partition_is_active, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, partition_is_active, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer10, partition_is_active, globalstatsbuffer10, globalposition, globalparamsV, collections10);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGEP0_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGEP0_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGEP0_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGEP0_mergeVs(kvdram7, vdram);
	
	globalparams_t globalparamsK8;
	MERGEP0_mergeVs(kvdram8, vdram);
	
	globalparams_t globalparamsK9;
	MERGEP0_mergeVs(kvdram9, vdram);
	
	globalparams_t globalparamsK10;
	MERGEP0_mergeVs(kvdram10, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP11"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPP0_U_topkernelP12(
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

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	// exit(EXIT_SUCCESS);
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE];
	collection_t collections10[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer11
	keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE];
	collection_t collections11[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[12];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	} 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	unsigned int inACTSMODE = UTILP0_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		if(inACTSMODE == ON){ cout<<"################################ ACTSGP::[SWITCH?]:: ACTS CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl; } 
		else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: ACTS NOT CALLED IN ITERATION ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	} else { if(true){cout<<"################################ ACTSGP::[SWITCH?]:: Hybrid Func Not Applicable ("<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<"). ################################"<<endl;} }
	#endif
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){ if(inACTSMODE != ON){ return; }} 
	
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SIZE; i++){ // REMOVEME LATER (NOT NECESSARY).
		vbuffer0[0][i] = 0;
		vbuffer0[1][i] = 0;
		vbuffer0[2][i] = 0;
		vbuffer0[3][i] = 0;
		vbuffer0[4][i] = 0;
		vbuffer0[5][i] = 0;
		vbuffer0[6][i] = 0;
		vbuffer0[7][i] = 0;
		vbuffer0[8][i] = 0;
		vbuffer0[9][i] = 0;
		vbuffer0[10][i] = 0;
		vbuffer0[11][i] = 0;
		vbuffer0[12][i] = 0;
		vbuffer0[13][i] = 0;
		vbuffer0[14][i] = 0;
		vbuffer0[15][i] = 0;
	
		vbuffer1[0][i] = 0;
		vbuffer1[1][i] = 0;
		vbuffer1[2][i] = 0;
		vbuffer1[3][i] = 0;
		vbuffer1[4][i] = 0;
		vbuffer1[5][i] = 0;
		vbuffer1[6][i] = 0;
		vbuffer1[7][i] = 0;
		vbuffer1[8][i] = 0;
		vbuffer1[9][i] = 0;
		vbuffer1[10][i] = 0;
		vbuffer1[11][i] = 0;
		vbuffer1[12][i] = 0;
		vbuffer1[13][i] = 0;
		vbuffer1[14][i] = 0;
		vbuffer1[15][i] = 0;
	
		vbuffer2[0][i] = 0;
		vbuffer2[1][i] = 0;
		vbuffer2[2][i] = 0;
		vbuffer2[3][i] = 0;
		vbuffer2[4][i] = 0;
		vbuffer2[5][i] = 0;
		vbuffer2[6][i] = 0;
		vbuffer2[7][i] = 0;
		vbuffer2[8][i] = 0;
		vbuffer2[9][i] = 0;
		vbuffer2[10][i] = 0;
		vbuffer2[11][i] = 0;
		vbuffer2[12][i] = 0;
		vbuffer2[13][i] = 0;
		vbuffer2[14][i] = 0;
		vbuffer2[15][i] = 0;
	
		vbuffer3[0][i] = 0;
		vbuffer3[1][i] = 0;
		vbuffer3[2][i] = 0;
		vbuffer3[3][i] = 0;
		vbuffer3[4][i] = 0;
		vbuffer3[5][i] = 0;
		vbuffer3[6][i] = 0;
		vbuffer3[7][i] = 0;
		vbuffer3[8][i] = 0;
		vbuffer3[9][i] = 0;
		vbuffer3[10][i] = 0;
		vbuffer3[11][i] = 0;
		vbuffer3[12][i] = 0;
		vbuffer3[13][i] = 0;
		vbuffer3[14][i] = 0;
		vbuffer3[15][i] = 0;
	
		vbuffer4[0][i] = 0;
		vbuffer4[1][i] = 0;
		vbuffer4[2][i] = 0;
		vbuffer4[3][i] = 0;
		vbuffer4[4][i] = 0;
		vbuffer4[5][i] = 0;
		vbuffer4[6][i] = 0;
		vbuffer4[7][i] = 0;
		vbuffer4[8][i] = 0;
		vbuffer4[9][i] = 0;
		vbuffer4[10][i] = 0;
		vbuffer4[11][i] = 0;
		vbuffer4[12][i] = 0;
		vbuffer4[13][i] = 0;
		vbuffer4[14][i] = 0;
		vbuffer4[15][i] = 0;
	
		vbuffer5[0][i] = 0;
		vbuffer5[1][i] = 0;
		vbuffer5[2][i] = 0;
		vbuffer5[3][i] = 0;
		vbuffer5[4][i] = 0;
		vbuffer5[5][i] = 0;
		vbuffer5[6][i] = 0;
		vbuffer5[7][i] = 0;
		vbuffer5[8][i] = 0;
		vbuffer5[9][i] = 0;
		vbuffer5[10][i] = 0;
		vbuffer5[11][i] = 0;
		vbuffer5[12][i] = 0;
		vbuffer5[13][i] = 0;
		vbuffer5[14][i] = 0;
		vbuffer5[15][i] = 0;
	
		vbuffer6[0][i] = 0;
		vbuffer6[1][i] = 0;
		vbuffer6[2][i] = 0;
		vbuffer6[3][i] = 0;
		vbuffer6[4][i] = 0;
		vbuffer6[5][i] = 0;
		vbuffer6[6][i] = 0;
		vbuffer6[7][i] = 0;
		vbuffer6[8][i] = 0;
		vbuffer6[9][i] = 0;
		vbuffer6[10][i] = 0;
		vbuffer6[11][i] = 0;
		vbuffer6[12][i] = 0;
		vbuffer6[13][i] = 0;
		vbuffer6[14][i] = 0;
		vbuffer6[15][i] = 0;
	
		vbuffer7[0][i] = 0;
		vbuffer7[1][i] = 0;
		vbuffer7[2][i] = 0;
		vbuffer7[3][i] = 0;
		vbuffer7[4][i] = 0;
		vbuffer7[5][i] = 0;
		vbuffer7[6][i] = 0;
		vbuffer7[7][i] = 0;
		vbuffer7[8][i] = 0;
		vbuffer7[9][i] = 0;
		vbuffer7[10][i] = 0;
		vbuffer7[11][i] = 0;
		vbuffer7[12][i] = 0;
		vbuffer7[13][i] = 0;
		vbuffer7[14][i] = 0;
		vbuffer7[15][i] = 0;
	
		vbuffer8[0][i] = 0;
		vbuffer8[1][i] = 0;
		vbuffer8[2][i] = 0;
		vbuffer8[3][i] = 0;
		vbuffer8[4][i] = 0;
		vbuffer8[5][i] = 0;
		vbuffer8[6][i] = 0;
		vbuffer8[7][i] = 0;
		vbuffer8[8][i] = 0;
		vbuffer8[9][i] = 0;
		vbuffer8[10][i] = 0;
		vbuffer8[11][i] = 0;
		vbuffer8[12][i] = 0;
		vbuffer8[13][i] = 0;
		vbuffer8[14][i] = 0;
		vbuffer8[15][i] = 0;
	
		vbuffer9[0][i] = 0;
		vbuffer9[1][i] = 0;
		vbuffer9[2][i] = 0;
		vbuffer9[3][i] = 0;
		vbuffer9[4][i] = 0;
		vbuffer9[5][i] = 0;
		vbuffer9[6][i] = 0;
		vbuffer9[7][i] = 0;
		vbuffer9[8][i] = 0;
		vbuffer9[9][i] = 0;
		vbuffer9[10][i] = 0;
		vbuffer9[11][i] = 0;
		vbuffer9[12][i] = 0;
		vbuffer9[13][i] = 0;
		vbuffer9[14][i] = 0;
		vbuffer9[15][i] = 0;
	
		vbuffer10[0][i] = 0;
		vbuffer10[1][i] = 0;
		vbuffer10[2][i] = 0;
		vbuffer10[3][i] = 0;
		vbuffer10[4][i] = 0;
		vbuffer10[5][i] = 0;
		vbuffer10[6][i] = 0;
		vbuffer10[7][i] = 0;
		vbuffer10[8][i] = 0;
		vbuffer10[9][i] = 0;
		vbuffer10[10][i] = 0;
		vbuffer10[11][i] = 0;
		vbuffer10[12][i] = 0;
		vbuffer10[13][i] = 0;
		vbuffer10[14][i] = 0;
		vbuffer10[15][i] = 0;
	
		vbuffer11[0][i] = 0;
		vbuffer11[1][i] = 0;
		vbuffer11[2][i] = 0;
		vbuffer11[3][i] = 0;
		vbuffer11[4][i] = 0;
		vbuffer11[5][i] = 0;
		vbuffer11[6][i] = 0;
		vbuffer11[7][i] = 0;
		vbuffer11[8][i] = 0;
		vbuffer11[9][i] = 0;
		vbuffer11[10][i] = 0;
		vbuffer11[11][i] = 0;
		vbuffer11[12][i] = 0;
		vbuffer11[13][i] = 0;
		vbuffer11[14][i] = 0;
		vbuffer11[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  collections10[v][t].data1 = 0;  collections11[v][t].data1 = 0;  }}

	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<endl; }
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<globalparamsK.NUM_PROCESSEDGESPARTITIONS; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	} 
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	MEMACCESSP0_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, globalparamsK.NUM_PROCESSEDGESPARTITIONS, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID);
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int srcvs_nextit_basekvs = 0;
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) && defined(CONFIG_PRELOADEDVERTICESMASKS)
	unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / MAXNUMGRAPHITERATIONS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ srcvs_nextit_basekvs += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<" (of "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<" v_chunks)"<<endl;
			#endif
			#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
			UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP, globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER);
			#endif 
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL12_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL12_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
					#endif 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL12_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL12_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }
						#endif 						
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL12_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP0_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1;
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF;
						unsigned int partition_is_active = pmask_curr[source_partition];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(partition_is_active == 1){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<pmask_curr[source_partition]<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(pmask_curr[source_partition] > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){
							unsigned int cummtv = 0; for(unsigned int t=0; t<VDATA_SHRINK_RATIO; t++){ cummtv += pmask_curr[source_partition + t]; }
							if(cummtv > 0){ enable_readandreplicatevdata = ON; } else {  enable_readandreplicatevdata = OFF; }
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(cummtv > 0 == 0){ cout<<"topkernelP: reading and replicating vdata for this source partition...."<<endl; } else { cout<<"topkernelP: NOT reading and replicating vdata for this source partition...."<<endl; }
							#endif 
						} else { enable_readandreplicatevdata = OFF; }
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
							MEMACCESSP0_readANDRVchunks12(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, vdatabaseoffset_kvs + srcvs_nextit_basekvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);
							#else 
							MEMACCESSP0_readANDRVchunks12(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
							#endif 
						}
						
						// acts
						#ifdef TESTKERNEL
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, partition_is_active, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, partition_is_active, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, partition_is_active, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, partition_is_active, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, partition_is_active, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, partition_is_active, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, partition_is_active, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, partition_is_active, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, partition_is_active, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, partition_is_active, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer10, partition_is_active, globalstatsbuffer10, globalposition, globalparamsV, collections10);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer11, partition_is_active, globalstatsbuffer11, globalposition, globalparamsV, collections11);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#else 
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						#endif 
						
						#ifdef _DEBUGMODE_CHECKS3
						// cout<<"topkernelP:: vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, srcvs_nextit_basekvs, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID

	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGEP0_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGEP0_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGEP0_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGEP0_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGEP0_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGEP0_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGEP0_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGEP0_mergeVs(kvdram7, vdram);
	
	globalparams_t globalparamsK8;
	MERGEP0_mergeVs(kvdram8, vdram);
	
	globalparams_t globalparamsK9;
	MERGEP0_mergeVs(kvdram9, vdram);
	
	globalparams_t globalparamsK10;
	MERGEP0_mergeVs(kvdram10, vdram);
	
	globalparams_t globalparamsK11;
	MERGEP0_mergeVs(kvdram11, vdram);
	#endif 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP12"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}

