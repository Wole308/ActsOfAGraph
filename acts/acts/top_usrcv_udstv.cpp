void acts_all::TOPP0_U_processit(uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif

	sweepparams_t sweepparams;
	
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2; // globalparamsK.SIZEKVS2_REDUCEPARTITION
	buffer_type vmaskbuffersz_kvs = (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // globalparamsK.SIZEKVS_VMASKBUFFER
	
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP0_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0); } // CRITICAL NEWCHANGE.
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### TOPP0_U_processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	
	batch_type voffset_kvs = globalposition.source_partition * reducebuffersz * FETFACTOR; // FIXME: globalposition.source_partition * globalparamsK.SIZEKVS2_REDUCEPARTITION
	if(voffset_kvs >= avtravstate.end_kvs){ return; } // continue; }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE) { if(vmask_p[globalposition.source_partition] == 0){ return; }} // vmaskoffset_kvs += vmaskbuffersz_kvs; continue; }} // CRITICAL CHECKME.
	
	sweepparams.source_partition = globalposition.source_partition;
	avtravstate.i_kvs = voffset_kvs;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print5("### TOPP0_U_processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
	#endif
	
	vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
	vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
	vertex_t endsrcvid = beginsrcvid + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR);
	if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
	if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
	unsigned int SKIP_KVS = reducebuffersz * FETFACTOR; // globalparamsK.SIZEKVS2_REDUCEPARTITION
	
	tuple_t tup = MEMACCESSP0_getvptrs_opt(edges0, kvdram, vptrbaseoffset_kvs, voffset_kvs * VECTOR2_SIZE, (voffset_kvs + SKIP_KVS) * VECTOR2_SIZE, edgebankID); // CRITICAL NEWCHANGE.
	keyy_t beginvptr = tup.A;
	keyy_t endvptr = tup.B; 
	
	if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
	
	keyy_t localbeginvptr = beginvptr;
	keyy_t localendvptr = endvptr;
	keyy_t numedges = localendvptr - localbeginvptr + 1*VECTOR_SIZE; // CRITICAL NEWCHANGE.
	if(localbeginvptr == localendvptr){ numedges = 0; }
	keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
	keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
	
	#ifdef _DEBUGMODE_CHECKS2
	if(localendvptr < localbeginvptr){ cout<<"TOPP0_U_processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<", voffset_kvs: "<<voffset_kvs<<endl; exit(EXIT_FAILURE); }
	if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("TOPP0_U_processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"[index: "<<globalposition.source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"]"<<endl;
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
	etravstate.i2 = voffset_kvs * VECTOR2_SIZE;
	etravstate.i2_kvs = voffset_kvs;
	bool_type resetenv = ON;
	bool_type flush = ON;
	
	// if(voffset_kvs == avtravstate.begin_kvs){ resetenv = ON; } else { resetenv = OFF; }
	if(voffset_kvs == avtravstate.begin_kvs || globalposition.source_partition == globalposition.first_source_partition){ resetenv = ON; } else { resetenv = OFF; } // NEWCHANGE
	if((voffset_kvs + (reducebuffersz * FETFACTOR)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ resetenv = ON; flush = ON; } // CRITICAL NEWCHANGE.
	
	#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
ACTSP0_actit
	#elif defined(BASIC_PARTITIONP0_AND_REDUCEP0_STRETEGY)
ACTSP0_priorit	
	#elif defined(TRAD_PARTITIONP0_AND_REDUCEP0_STRETEGY)
ACTSP0_tradit
	#else 
ACTSP0_tradit
	#endif 
	(
		ON, ACTSPROCESSMODE,
edges0, kvdram, sourcebuffer, vbuffer, globalstatsbuffer, 
		globalparamsK, globalposition, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE,
		resetenv, flush, edgebankID);
	
	MEMACCESSP0_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); // CRITICAL OPTIMIZEME. should be called only once
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //	
	return;
}

void acts_all::TOPP0_U_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce, uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	if(en_process == ON){ TOPP0_U_processit(edges0, kvdram, sourcebuffer, vbuffer, vmask_p, globalstatsbuffer, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID); } 
	if(en_partition == ON){ TOPP0_NU_partitionit(edges0, kvdram, sourcebuffer, vbuffer, globalparamsK, NAp); } 
	if(en_reduce == ON){ TOPP0_NU_reduceit(edges0, kvdram, sourcebuffer, vbuffer, sourcestatsmarker, source_partition, globalparamsK, NAp); } 
	return;
}

void acts_all::TOPP0_U_dispatch_reduce(uint512_dt * edges0, uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef TRAD_PARTITIONP0_AND_REDUCEP0_STRETEGY
	return; // no TOPP0_U_dispatch_reduce for TRAD_PARTITIONP0_AND_REDUCEP0_STRETEGY
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
	globalposition_t globalposition; // NOT USED.
	keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS]; // NOT USED.
	
	DISPATCHREDUCEP0_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### TOPP0_U_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "vreadoffset_kvs2", source_partition, currentLOP, num_source_partitions, vreadoffset_kvs2); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTILP0_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		MEMACCESSP0_readV(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK);
		
		// reduce
		TOPP0_U_dispatch(OFF, OFF, enablereduce, edges0, kvdram, sourcebuffer, vbuffer, vmask_p, globalstatsbuffer, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp);

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
#ifdef CONFIG_ENABLECLASS_TOP_USRCV_UDSTV
void acts_all::TOPP0_U_topkernelproc_embedded(unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * edges0, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalposition_t globalposition){

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
	
	globalparamsK = UTILP0_getglobalparams(kvdram); // CRITICAL OPTIMIZEME. MOVETOBASE?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges0); // CRITICAL OPTIMIZEME. MOVETOBASE?
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	_globalparamsE = globalparamsEs[globalposition.edgebankID];
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid;
	for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // OPTIMIZEME? IS THIS OPTIMIZED
	#endif 
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ UTILP0_resetkvstatvalues(kvdram, globalparamsK); }}	// CRITICAL FIXME. NEWCHANGE. DO FOR ALL.
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch(globalposition.EN_PROCESS, OFF, OFF, edges0, kvdram, sourcebuffer, vbuffer, vmask_p, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, globalposition.edgebankID); // PARTITIONP0_CHKPT[0], 0, 0);
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch(OFF, globalposition.EN_PARTITION, OFF, edges0, kvdram, sourcebuffer, vbuffer, vmask_p, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, NAp); // PARTITIONP0_CHKPT[0], 0, NAp);
	}
	#endif
	
	// reduce & partition
	#if defined(CONFIG_ENABLEREDUCEMODULE)
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch_reduce(edges0, kvdram, sourcebuffer, vbuffer, vmask_p, _globalparamsE, globalparamsK, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, NAp);
	}
	#endif
	
	if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){
		UTILP0_increment_graphiteration(kvdram, globalparamsK);
	}

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

extern "C" {
void acts_all::TOPP0_U_topkernelP1(
uint512_dt * edges00,	uint512_dt * kvdram0,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[1];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask1(vdram, vmask0_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL1_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL1_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL1_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL1_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL1_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL1_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks1(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs1(kvdram0, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP1"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP2(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[2];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask2(vdram, vmask0_p,vmask1_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL2_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL2_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL2_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL2_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL2_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL2_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks2(ON, vdram, vbuffer0,vbuffer1, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs2(kvdram0,kvdram1, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP2"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP3(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[3];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask3(vdram, vmask0_p,vmask1_p,vmask2_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL3_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL3_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL3_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL3_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL3_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL3_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks3(ON, vdram, vbuffer0,vbuffer1,vbuffer2, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs3(kvdram0,kvdram1,kvdram2, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP3"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP4(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[4];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask4(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL4_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL4_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL4_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL4_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL4_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL4_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks4(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs4(kvdram0,kvdram1,kvdram2,kvdram3, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP4"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP5(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
	uint512_dt * vdram
	){
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

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[5];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask5(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL5_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL5_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL5_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL5_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL5_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL5_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks5(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, edges40, kvdram4, vbuffer4, vmask4_p, globalstatsbuffer4, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs5(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP5"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP6(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
	uint512_dt * vdram
	){
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

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[6];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask6(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL6_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL6_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL6_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL6_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL6_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL6_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks6(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, edges40, kvdram4, vbuffer4, vmask4_p, globalstatsbuffer4, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, edges50, kvdram5, vbuffer5, vmask5_p, globalstatsbuffer5, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs6(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP6"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP7(
uint512_dt * edges00,	uint512_dt * kvdram0,
uint512_dt * edges10,	uint512_dt * kvdram1,
uint512_dt * edges20,	uint512_dt * kvdram2,
uint512_dt * edges30,	uint512_dt * kvdram3,
uint512_dt * edges40,	uint512_dt * kvdram4,
uint512_dt * edges50,	uint512_dt * kvdram5,
uint512_dt * edges60,	uint512_dt * kvdram6,
	uint512_dt * vdram
	){
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

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[7];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask7(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL7_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL7_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL7_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL7_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL7_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL7_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks7(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, edges40, kvdram4, vbuffer4, vmask4_p, globalstatsbuffer4, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, edges50, kvdram5, vbuffer5, vmask5_p, globalstatsbuffer5, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, edges60, kvdram6, vbuffer6, vmask6_p, globalstatsbuffer6, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs7(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP7"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP8(
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

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[8];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask8(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL8_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL8_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL8_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL8_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL8_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL8_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks8(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, edges40, kvdram4, vbuffer4, vmask4_p, globalstatsbuffer4, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, edges50, kvdram5, vbuffer5, vmask5_p, globalstatsbuffer5, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, edges60, kvdram6, vbuffer6, vmask6_p, globalstatsbuffer6, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, edges70, kvdram7, vbuffer7, vmask7_p, globalstatsbuffer7, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs8(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP8"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP9(
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

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	uint32_type vmask8_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer8[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[9];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask9(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p,vmask8_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL9_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL9_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL9_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL9_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL9_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL9_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks9(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, edges40, kvdram4, vbuffer4, vmask4_p, globalstatsbuffer4, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, edges50, kvdram5, vbuffer5, vmask5_p, globalstatsbuffer5, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, edges60, kvdram6, vbuffer6, vmask6_p, globalstatsbuffer6, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, edges70, kvdram7, vbuffer7, vmask7_p, globalstatsbuffer7, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, edges80, kvdram8, vbuffer8, vmask8_p, globalstatsbuffer8, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs9(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP9"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP10(
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

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = edges90
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	uint32_type vmask8_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer8[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	uint32_type vmask9_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer9[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[10];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask10(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p,vmask8_p,vmask9_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL10_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL10_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL10_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL10_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL10_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL10_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks10(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, edges40, kvdram4, vbuffer4, vmask4_p, globalstatsbuffer4, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, edges50, kvdram5, vbuffer5, vmask5_p, globalstatsbuffer5, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, edges60, kvdram6, vbuffer6, vmask6_p, globalstatsbuffer6, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, edges70, kvdram7, vbuffer7, vmask7_p, globalstatsbuffer7, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, edges80, kvdram8, vbuffer8, vmask8_p, globalstatsbuffer8, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, edges90, kvdram9, vbuffer9, vmask9_p, globalstatsbuffer9, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs10(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP10"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP11(
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

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = edges100 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = edges90
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = edges100
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	uint32_type vmask8_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer8[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	uint32_type vmask9_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer9[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	uint32_type vmask10_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer10[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[11];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask11(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p,vmask8_p,vmask9_p,vmask10_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL11_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL11_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL11_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL11_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL11_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL11_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks11(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, edges40, kvdram4, vbuffer4, vmask4_p, globalstatsbuffer4, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, edges50, kvdram5, vbuffer5, vmask5_p, globalstatsbuffer5, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, edges60, kvdram6, vbuffer6, vmask6_p, globalstatsbuffer6, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, edges70, kvdram7, vbuffer7, vmask7_p, globalstatsbuffer7, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, edges80, kvdram8, vbuffer8, vmask8_p, globalstatsbuffer8, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, edges90, kvdram9, vbuffer9, vmask9_p, globalstatsbuffer9, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, edges100, kvdram10, vbuffer10, vmask10_p, globalstatsbuffer10, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs11(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP11"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP0_U_topkernelP12(
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

#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = edges100 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges110 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram11 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = edges90
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = edges100
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = edges110
#pragma HLS DATA_PACK variable = kvdram11
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	uint32_type vmask8_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer8[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	uint32_type vmask9_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer9[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	uint32_type vmask10_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer10[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer11
	uint32_type vmask11_p[BLOCKRAM_SIZE];
	keyvalue_t globalstatsbuffer11[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[12];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsEs[0] = UTILP0_getglobalparams(edges00);
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram);
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
	
			#else
			PARTITIONP0_CHKPT[0][u] = edges00[globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask12(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p,vmask8_p,vmask9_p,vmask10_p,vmask11_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	MEMACCESSP0_copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0; // NEWCHANGE
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL12_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL12_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL12_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL12_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITIONP0_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL12_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL12_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
						// cout<<"---------------------------- vreadoffset_kvs2: "<<vreadoffset_kvs2<<endl;
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
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
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
						} else { enableprocess = OFF; }
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks12(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);		
						}
						
						// acts 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, edges10, kvdram1, vbuffer1, vmask1_p, globalstatsbuffer1, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, edges20, kvdram2, vbuffer2, vmask2_p, globalstatsbuffer2, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, edges30, kvdram3, vbuffer3, vmask3_p, globalstatsbuffer3, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, edges40, kvdram4, vbuffer4, vmask4_p, globalstatsbuffer4, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, edges50, kvdram5, vbuffer5, vmask5_p, globalstatsbuffer5, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, edges60, kvdram6, vbuffer6, vmask6_p, globalstatsbuffer6, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, edges70, kvdram7, vbuffer7, vmask7_p, globalstatsbuffer7, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, edges80, kvdram8, vbuffer8, vmask8_p, globalstatsbuffer8, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, edges90, kvdram9, vbuffer9, vmask9_p, globalstatsbuffer9, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, edges100, kvdram10, vbuffer10, vmask10_p, globalstatsbuffer10, globalposition);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, edges110, kvdram11, vbuffer11, vmask11_p, globalstatsbuffer11, globalposition);		
	
						// TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, edges00, kvdram0, vbuffer0, vmask0_p, globalstatsbuffer0, globalposition);		
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
	MERGEP0_SPLIT_mergeVs12(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, vdram, globalparamsK, globalparamsV);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	cout<<"PRINTGLOBALVARS @ topkernelP12"<<endl;
	actsutilityobj->printglobalvars();
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
#endif 
