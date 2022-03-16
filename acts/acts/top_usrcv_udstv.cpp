#ifdef XXXXXXXXXXXXXX
void TTT(){
	for(buffer_type i=0; i<maxsize_kvs; i++){
	
		unsigned int partition0 = it+0;
		unsigned int ind0 = ((bramoffset_kvs[0] + i)*VECTOR2_SIZE) + v;
	
		unsigned int partition1 = it+1;
		unsigned int ind1 = ((bramoffset_kvs[1] + i)*VECTOR2_SIZE) + v;
	
		unsigned int partition2 = it+2;
		unsigned int ind2 = ((bramoffset_kvs[2] + i)*VECTOR2_SIZE) + v;
	
		unsigned int partition3 = it+3;
		unsigned int ind3 = ((bramoffset_kvs[3] + i)*VECTOR2_SIZE) + v;
	
		unsigned int partition4 = it+4;
		unsigned int ind4 = ((bramoffset_kvs[4] + i)*VECTOR2_SIZE) + v;
	
		unsigned int partition5 = it+5;
		unsigned int ind5 = ((bramoffset_kvs[5] + i)*VECTOR2_SIZE) + v;
	
		unsigned int partition6 = it+6;
		unsigned int ind6 = ((bramoffset_kvs[6] + i)*VECTOR2_SIZE) + v;
	
		unsigned int partition7 = it+7;
		unsigned int ind7 = ((bramoffset_kvs[7] + i)*VECTOR2_SIZE) + v;
		
		// read
		E[0] = tempbuffer[v][readoffset + bramoffset_kvs[0] + i]; 	
		E[1] = tempbuffer[v][readoffset + bramoffset_kvs[1] + i]; 	
		E[2] = tempbuffer[v][readoffset + bramoffset_kvs[2] + i]; 	
		E[3] = tempbuffer[v][readoffset + bramoffset_kvs[3] + i]; 	
		E[4] = tempbuffer[v][readoffset + bramoffset_kvs[4] + i]; 	
		E[5] = tempbuffer[v][readoffset + bramoffset_kvs[5] + i]; 	
		E[6] = tempbuffer[v][readoffset + bramoffset_kvs[6] + i]; 	
		E[7] = tempbuffer[v][readoffset + bramoffset_kvs[7] + i]; 	
		
		// parse
		parsededge_t parsed_edge0 = PROCESSP0_PARSEEDGE(E[0]); // FIXME.
		edata[0].value = parsed_edge0.incr; // source info
		edata[0].key = parsed_edge0.dstvid;	
		parsededge_t parsed_edge1 = PROCESSP0_PARSEEDGE(E[1]); // FIXME.
		edata[1].value = parsed_edge1.incr; // source info
		edata[1].key = parsed_edge1.dstvid;	
		parsededge_t parsed_edge2 = PROCESSP0_PARSEEDGE(E[2]); // FIXME.
		edata[2].value = parsed_edge2.incr; // source info
		edata[2].key = parsed_edge2.dstvid;	
		parsededge_t parsed_edge3 = PROCESSP0_PARSEEDGE(E[3]); // FIXME.
		edata[3].value = parsed_edge3.incr; // source info
		edata[3].key = parsed_edge3.dstvid;	
		parsededge_t parsed_edge4 = PROCESSP0_PARSEEDGE(E[4]); // FIXME.
		edata[4].value = parsed_edge4.incr; // source info
		edata[4].key = parsed_edge4.dstvid;	
		parsededge_t parsed_edge5 = PROCESSP0_PARSEEDGE(E[5]); // FIXME.
		edata[5].value = parsed_edge5.incr; // source info
		edata[5].key = parsed_edge5.dstvid;	
		parsededge_t parsed_edge6 = PROCESSP0_PARSEEDGE(E[6]); // FIXME.
		edata[6].value = parsed_edge6.incr; // source info
		edata[6].key = parsed_edge6.dstvid;	
		parsededge_t parsed_edge7 = PROCESSP0_PARSEEDGE(E[7]); // FIXME.
		edata[7].value = parsed_edge7.incr; // source info
		edata[7].key = parsed_edge7.dstvid;	
		
		// enable flags
		if((ind0 >= localcapsule[it+0].key) && (ind0 < localcapsule[it+0].key + localcapsule[it+0].value)){ enx[0] = true; } else { enx[0] = false; }
		if((ind1 >= localcapsule[it+1].key) && (ind1 < localcapsule[it+1].key + localcapsule[it+1].value)){ enx[1] = true; } else { enx[1] = false; }
		if((ind2 >= localcapsule[it+2].key) && (ind2 < localcapsule[it+2].key + localcapsule[it+2].value)){ enx[2] = true; } else { enx[2] = false; }
		if((ind3 >= localcapsule[it+3].key) && (ind3 < localcapsule[it+3].key + localcapsule[it+3].value)){ enx[3] = true; } else { enx[3] = false; }
		if((ind4 >= localcapsule[it+4].key) && (ind4 < localcapsule[it+4].key + localcapsule[it+4].value)){ enx[4] = true; } else { enx[4] = false; }
		if((ind5 >= localcapsule[it+5].key) && (ind5 < localcapsule[it+5].key + localcapsule[it+5].value)){ enx[5] = true; } else { enx[5] = false; }
		if((ind6 >= localcapsule[it+6].key) && (ind6 < localcapsule[it+6].key + localcapsule[it+6].value)){ enx[6] = true; } else { enx[6] = false; }
		if((ind7 >= localcapsule[it+7].key) && (ind7 < localcapsule[it+7].key + localcapsule[it+7].value)){ enx[7] = true; } else { enx[7] = false; }
		
		// process	
		reskeyvalue[0] = PROCESSP0_processvector(enx[0], it+0, edata[0].value, edata[0], vbuffer[it+0], buffer[0], &loadcount[0], GraphAlgoClass, globalparams);
		reskeyvalue[1] = PROCESSP0_processvector(enx[1], it+1, edata[1].value, edata[1], vbuffer[it+1], buffer[1], &loadcount[1], GraphAlgoClass, globalparams);
		reskeyvalue[2] = PROCESSP0_processvector(enx[2], it+2, edata[2].value, edata[2], vbuffer[it+2], buffer[2], &loadcount[2], GraphAlgoClass, globalparams);
		reskeyvalue[3] = PROCESSP0_processvector(enx[3], it+3, edata[3].value, edata[3], vbuffer[it+3], buffer[3], &loadcount[3], GraphAlgoClass, globalparams);
		reskeyvalue[4] = PROCESSP0_processvector(enx[4], it+4, edata[4].value, edata[4], vbuffer[it+4], buffer[4], &loadcount[4], GraphAlgoClass, globalparams);
		reskeyvalue[5] = PROCESSP0_processvector(enx[5], it+5, edata[5].value, edata[5], vbuffer[it+5], buffer[5], &loadcount[5], GraphAlgoClass, globalparams);
		reskeyvalue[6] = PROCESSP0_processvector(enx[6], it+6, edata[6].value, edata[6], vbuffer[it+6], buffer[6], &loadcount[6], GraphAlgoClass, globalparams);
		reskeyvalue[7] = PROCESSP0_processvector(enx[7], it+7, edata[7].value, edata[7], vbuffer[it+7], buffer[7], &loadcount[7], GraphAlgoClass, globalparams);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[0], WORKBUFFER_SIZE, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[1], WORKBUFFER_SIZE, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[2], WORKBUFFER_SIZE, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[3], WORKBUFFER_SIZE, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[4], WORKBUFFER_SIZE, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[5], WORKBUFFER_SIZE, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[6], WORKBUFFER_SIZE, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[7], WORKBUFFER_SIZE, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE);
 
		#endif
	}
}
#endif 

void acts_all::TOPP0_U_processit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_PMASK1_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
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

	batch_type voffset_kvs = globalposition.source_partition * globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
	
	// check if we are at the end of file
	if(voffset_kvs >= avtravstate.end_kvs){ return; }

	// check if vertex partition is active
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	unsigned int IND = UTILP0_allignhigher_FACTOR((globalposition.source_partition * (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs)) / VPARTITION_SHRINK_RATIO, 32);
	unsigned int span = (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE) / (NUM_PEs * VECTOR2_SIZE * VPARTITION_SHRINK_RATIO);
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("TOPP0_U_processit 23", IND / 32, BLOCKRAM_PMASK_SIZE, NAp, NAp, NAp);
	if(IND % 32 != 0){ cout<<"TOPP0_U_processit: ERROR SOMEWHERE (234). Ycol("<<IND % 32<<") != 0, source_partition: "<<globalposition.source_partition<<", SIZEKVS2_PROCESSEDGESPARTITION: "<<globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION<<", VPARTITION_SHRINK_RATIO: "<<VPARTITION_SHRINK_RATIO<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"TOPP0_U_processit: source_partition: "<<globalposition.source_partition<<", IND: "<<IND<<", span: "<<span<<", SIZEKVS2_PROCESSEDGESPARTITION: "<<globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION<<", VPARTITION_SHRINK_RATIO: "<<VPARTITION_SHRINK_RATIO<<endl;
	#endif
	unsigned int partition_is_active = 0;
	for(unsigned int t=0; t<span; t++){ unsigned int col = (IND+t) % 32; unsigned int row = (IND+t) / 32; if(pmask_curr[row].data[col] > 0){ partition_is_active = 1; break; }}
	if(partition_is_active == 0){ return; }
	#endif 
	
	sweepparams.source_partition = globalposition.source_partition;
	avtravstate.i_kvs = voffset_kvs;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print5("### TOPP0_U_processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
	#endif
	
	vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
	vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
	vertex_t endsrcvid = beginsrcvid + (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE);
	if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
	if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
	unsigned int SKIP_KVS = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
	
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
	
	// if(voffset_kvs == avtravstate.begin_kvs){ resetenv = ON; } else { resetenv = OFF; }
	if(voffset_kvs == avtravstate.begin_kvs || globalposition.source_partition == globalposition.first_source_partition){ resetenv = ON; } else { resetenv = OFF; } // NEWCHANGE
	if((voffset_kvs + globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
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
	
	MEMACCESSP0_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); // CRITICAL OPTIMIZEME. should be called only once
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

void acts_all::TOPP0_U_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_PMASK1_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	if(en_process == ON){ TOPP0_U_processit( kvdram, sourcebuffer, vbuffer, pmask_curr, globalstatsbuffer, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID); } 
	if(en_partition == ON){ TOPP0_NU_partitionit( kvdram, sourcebuffer, vbuffer, globalparamsK, NAp); } 
	if(en_reduce == ON){ TOPP0_NU_reduceit( kvdram, sourcebuffer, vbuffer, sourcestatsmarker, source_partition, globalparamsK, NAp); } 
	return;
}

void acts_all::TOPP0_U_dispatch_reduce(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_PMASK_SIZE], pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no TOPP0_U_dispatch_reduce for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); ///////////////////////
	
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
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = UTILP0_get_num_source_partitions(currentLOP);
	#else 
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH + 1;
	batch_type num_source_partitions = NUM_PARTITIONS;	
	#endif

	bool_type enablereduce = ON;
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE]; // NOT USED.
	
	uint32_type vpmaskVecSum[VECTOR2_SIZE]; for(unsigned int i=0; i<VECTOR2_SIZE; i++){ vpmaskVecSum[i] = 0; }
	
	DISPATCHREDUCEP0_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### TOPP0_U_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "vreadoffset_kvs2", source_partition, currentLOP, num_source_partitions, vreadoffset_kvs2); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTILP0_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		globalstatsbuffer[source_partition].key = NAp; globalstatsbuffer[source_partition].value = rtravstate.size_kvs;
		
		// read vertices
		MEMACCESSP0_readV(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK);
		
		// reduce
		TOPP0_U_dispatch(OFF, OFF, enablereduce,  kvdram, sourcebuffer, vbuffer, pmask_curr, globalstatsbuffer, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp);

		// writeback vertices
		MEMACCESSP0_saveV(enablereduce, kvdram, vbuffer, pmask_next, vpmaskVecSum, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, source_partition, globalposition, globalparamsK, globalparamsV);
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		vmaskp_offset_kvs += NUM_PEs;
	}
	// exit(EXIT_SUCCESS);///
	
	unsigned int sum = 0; for(unsigned int i=0; i<VECTOR2_SIZE; i++){ sum += vpmaskVecSum[i]; }
	#ifdef _DEBUGMODE_STATS
	cout<<"TOPP0_U_dispatch_reduce:: Number of active vertices for next iteration (iteration "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<") [instance: "<<globalparamsK.ACTSPARAMS_INSTID<<"]: "<<sum<<endl;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"TOPP0_U_dispatch_reduce:: globalparamsK.NUM_REDUCEPARTITIONS: "<<globalparamsK.NUM_REDUCEPARTITIONS<<endl;
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ if(pmask_next[i].data[0] > 0){ cout<<""<<pmask_next[i].data[0]<<"("<<i<<"),"<<endl; }} // FIXME.
	#endif
	return;
} 

void acts_all::TOPP0_U_topkernelproc_embedded(unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask_curr[BLOCKRAM_PMASK_SIZE], pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE], globalposition_t globalposition, globalparams_t globalparamsV){				

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
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ UTILP0_resetkvstatvalues(kvdram, globalparamsK); }}	// CRITICAL FIXME. NEWCHANGE. DO FOR ALL.
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, sourcebuffer, vbuffer, pmask_curr, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, globalposition.edgebankID); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, sourcebuffer, vbuffer, pmask_curr, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, NAp);
	}
	#endif
	
	// reduce & partition
	#if defined(CONFIG_ENABLEREDUCEMODULE)
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_U_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_U_dispatch_reduce(kvdram, sourcebuffer, vbuffer, pmask_curr, pmask_next, _globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONP0_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, NAp);
	}
	#endif
	
	if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){
		unsigned int _offset = MERGEP0_actvpstatsoffset(globalparamsK);
		MEMACCESSP0_retreievekvstats(kvdram, globalstatsbuffer, globalparamsK, _offset, globalparamsK.NUM_REDUCEPARTITIONS);
		UTILP0_increment_graphiteration(kvdram, globalparamsK); // NB: this should come last.
	}

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void acts_all::TOPP0_U_topkernelP1(
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask1(vdram, pmask0_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL1_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL1_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL1_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL1_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks1(ON, vdram, vbuffer0, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#endif 
						
						/* 	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs1(kvdram0, vdram, 
globalstatsbuffer0, pmask0_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask2(vdram, pmask0_curr,pmask1_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL2_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL2_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL2_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL2_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks2(ON, vdram, vbuffer0,vbuffer1, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
	
						#endif 
						
						/* 	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs2(kvdram0,kvdram1, vdram, 
globalstatsbuffer0,globalstatsbuffer1, pmask0_next,pmask1_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask3(vdram, pmask0_curr,pmask1_curr,pmask2_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL3_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL3_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL3_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL3_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks3(ON, vdram, vbuffer0,vbuffer1,vbuffer2, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs3(kvdram0,kvdram1,kvdram2, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2, pmask0_next,pmask1_next,pmask2_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask4(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL4_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL4_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL4_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL4_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks4(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs4(kvdram0,kvdram1,kvdram2,kvdram3, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3, pmask0_next,pmask1_next,pmask2_next,pmask3_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	pmask_dt pmask4_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask4_curr
	#pragma HLS DATA_PACK variable = pmask4_next
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
			pmask4_curr[i].data[v] = 0; 
			pmask4_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask5(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr,pmask4_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL5_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL5_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL5_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL5_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks5(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, pmask4_curr, pmask4_next, globalstatsbuffer4, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs5(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3,globalstatsbuffer4, pmask0_next,pmask1_next,pmask2_next,pmask3_next,pmask4_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	pmask_dt pmask4_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask4_curr
	#pragma HLS DATA_PACK variable = pmask4_next
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	pmask_dt pmask5_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask5_curr
	#pragma HLS DATA_PACK variable = pmask5_next
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
			pmask4_curr[i].data[v] = 0; 
			pmask4_next[i].data[v] = 0; 
			pmask5_curr[i].data[v] = 0; 
			pmask5_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask6(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr,pmask4_curr,pmask5_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL6_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL6_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL6_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL6_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks6(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, pmask4_curr, pmask4_next, globalstatsbuffer4, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, pmask5_curr, pmask5_next, globalstatsbuffer5, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs6(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3,globalstatsbuffer4,globalstatsbuffer5, pmask0_next,pmask1_next,pmask2_next,pmask3_next,pmask4_next,pmask5_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	pmask_dt pmask4_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask4_curr
	#pragma HLS DATA_PACK variable = pmask4_next
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	pmask_dt pmask5_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask5_curr
	#pragma HLS DATA_PACK variable = pmask5_next
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	pmask_dt pmask6_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask6_curr
	#pragma HLS DATA_PACK variable = pmask6_next
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
			pmask4_curr[i].data[v] = 0; 
			pmask4_next[i].data[v] = 0; 
			pmask5_curr[i].data[v] = 0; 
			pmask5_next[i].data[v] = 0; 
			pmask6_curr[i].data[v] = 0; 
			pmask6_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask7(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr,pmask4_curr,pmask5_curr,pmask6_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL7_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL7_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL7_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL7_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks7(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, pmask4_curr, pmask4_next, globalstatsbuffer4, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, pmask5_curr, pmask5_next, globalstatsbuffer5, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, pmask6_curr, pmask6_next, globalstatsbuffer6, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs7(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3,globalstatsbuffer4,globalstatsbuffer5,globalstatsbuffer6, pmask0_next,pmask1_next,pmask2_next,pmask3_next,pmask4_next,pmask5_next,pmask6_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	pmask_dt pmask4_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask4_curr
	#pragma HLS DATA_PACK variable = pmask4_next
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	pmask_dt pmask5_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask5_curr
	#pragma HLS DATA_PACK variable = pmask5_next
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	pmask_dt pmask6_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask6_curr
	#pragma HLS DATA_PACK variable = pmask6_next
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	pmask_dt pmask7_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask7_curr
	#pragma HLS DATA_PACK variable = pmask7_next
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
			pmask4_curr[i].data[v] = 0; 
			pmask4_next[i].data[v] = 0; 
			pmask5_curr[i].data[v] = 0; 
			pmask5_next[i].data[v] = 0; 
			pmask6_curr[i].data[v] = 0; 
			pmask6_next[i].data[v] = 0; 
			pmask7_curr[i].data[v] = 0; 
			pmask7_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask8(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr,pmask4_curr,pmask5_curr,pmask6_curr,pmask7_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL8_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL8_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL8_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL8_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks8(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, pmask4_curr, pmask4_next, globalstatsbuffer4, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, pmask5_curr, pmask5_next, globalstatsbuffer5, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, pmask6_curr, pmask6_next, globalstatsbuffer6, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, pmask7_curr, pmask7_next, globalstatsbuffer7, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs8(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3,globalstatsbuffer4,globalstatsbuffer5,globalstatsbuffer6,globalstatsbuffer7, pmask0_next,pmask1_next,pmask2_next,pmask3_next,pmask4_next,pmask5_next,pmask6_next,pmask7_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	pmask_dt pmask4_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask4_curr
	#pragma HLS DATA_PACK variable = pmask4_next
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	pmask_dt pmask5_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask5_curr
	#pragma HLS DATA_PACK variable = pmask5_next
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	pmask_dt pmask6_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask6_curr
	#pragma HLS DATA_PACK variable = pmask6_next
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	pmask_dt pmask7_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask7_curr
	#pragma HLS DATA_PACK variable = pmask7_next
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	pmask_dt pmask8_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask8_curr
	#pragma HLS DATA_PACK variable = pmask8_next
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
			pmask4_curr[i].data[v] = 0; 
			pmask4_next[i].data[v] = 0; 
			pmask5_curr[i].data[v] = 0; 
			pmask5_next[i].data[v] = 0; 
			pmask6_curr[i].data[v] = 0; 
			pmask6_next[i].data[v] = 0; 
			pmask7_curr[i].data[v] = 0; 
			pmask7_next[i].data[v] = 0; 
			pmask8_curr[i].data[v] = 0; 
			pmask8_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask9(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr,pmask4_curr,pmask5_curr,pmask6_curr,pmask7_curr,pmask8_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL9_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL9_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL9_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL9_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks9(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, pmask4_curr, pmask4_next, globalstatsbuffer4, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, pmask5_curr, pmask5_next, globalstatsbuffer5, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, pmask6_curr, pmask6_next, globalstatsbuffer6, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, pmask7_curr, pmask7_next, globalstatsbuffer7, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, pmask8_curr, pmask8_next, globalstatsbuffer8, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs9(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3,globalstatsbuffer4,globalstatsbuffer5,globalstatsbuffer6,globalstatsbuffer7,globalstatsbuffer8, pmask0_next,pmask1_next,pmask2_next,pmask3_next,pmask4_next,pmask5_next,pmask6_next,pmask7_next,pmask8_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	pmask_dt pmask4_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask4_curr
	#pragma HLS DATA_PACK variable = pmask4_next
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	pmask_dt pmask5_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask5_curr
	#pragma HLS DATA_PACK variable = pmask5_next
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	pmask_dt pmask6_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask6_curr
	#pragma HLS DATA_PACK variable = pmask6_next
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	pmask_dt pmask7_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask7_curr
	#pragma HLS DATA_PACK variable = pmask7_next
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	pmask_dt pmask8_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask8_curr
	#pragma HLS DATA_PACK variable = pmask8_next
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	pmask_dt pmask9_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask9_curr
	#pragma HLS DATA_PACK variable = pmask9_next
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
			pmask4_curr[i].data[v] = 0; 
			pmask4_next[i].data[v] = 0; 
			pmask5_curr[i].data[v] = 0; 
			pmask5_next[i].data[v] = 0; 
			pmask6_curr[i].data[v] = 0; 
			pmask6_next[i].data[v] = 0; 
			pmask7_curr[i].data[v] = 0; 
			pmask7_next[i].data[v] = 0; 
			pmask8_curr[i].data[v] = 0; 
			pmask8_next[i].data[v] = 0; 
			pmask9_curr[i].data[v] = 0; 
			pmask9_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask10(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr,pmask4_curr,pmask5_curr,pmask6_curr,pmask7_curr,pmask8_curr,pmask9_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL10_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL10_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL10_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL10_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks10(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, pmask4_curr, pmask4_next, globalstatsbuffer4, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, pmask5_curr, pmask5_next, globalstatsbuffer5, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, pmask6_curr, pmask6_next, globalstatsbuffer6, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, pmask7_curr, pmask7_next, globalstatsbuffer7, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, pmask8_curr, pmask8_next, globalstatsbuffer8, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, pmask9_curr, pmask9_next, globalstatsbuffer9, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs10(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3,globalstatsbuffer4,globalstatsbuffer5,globalstatsbuffer6,globalstatsbuffer7,globalstatsbuffer8,globalstatsbuffer9, pmask0_next,pmask1_next,pmask2_next,pmask3_next,pmask4_next,pmask5_next,pmask6_next,pmask7_next,pmask8_next,pmask9_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	pmask_dt pmask4_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask4_curr
	#pragma HLS DATA_PACK variable = pmask4_next
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	pmask_dt pmask5_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask5_curr
	#pragma HLS DATA_PACK variable = pmask5_next
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	pmask_dt pmask6_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask6_curr
	#pragma HLS DATA_PACK variable = pmask6_next
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	pmask_dt pmask7_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask7_curr
	#pragma HLS DATA_PACK variable = pmask7_next
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	pmask_dt pmask8_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask8_curr
	#pragma HLS DATA_PACK variable = pmask8_next
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	pmask_dt pmask9_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask9_curr
	#pragma HLS DATA_PACK variable = pmask9_next
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	pmask_dt pmask10_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask10_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask10_curr
	#pragma HLS DATA_PACK variable = pmask10_next
	keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
			pmask4_curr[i].data[v] = 0; 
			pmask4_next[i].data[v] = 0; 
			pmask5_curr[i].data[v] = 0; 
			pmask5_next[i].data[v] = 0; 
			pmask6_curr[i].data[v] = 0; 
			pmask6_next[i].data[v] = 0; 
			pmask7_curr[i].data[v] = 0; 
			pmask7_next[i].data[v] = 0; 
			pmask8_curr[i].data[v] = 0; 
			pmask8_next[i].data[v] = 0; 
			pmask9_curr[i].data[v] = 0; 
			pmask9_next[i].data[v] = 0; 
			pmask10_curr[i].data[v] = 0; 
			pmask10_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask11(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr,pmask4_curr,pmask5_curr,pmask6_curr,pmask7_curr,pmask8_curr,pmask9_curr,pmask10_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL11_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL11_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL11_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL11_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks11(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, pmask4_curr, pmask4_next, globalstatsbuffer4, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, pmask5_curr, pmask5_next, globalstatsbuffer5, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, pmask6_curr, pmask6_next, globalstatsbuffer6, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, pmask7_curr, pmask7_next, globalstatsbuffer7, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, pmask8_curr, pmask8_next, globalstatsbuffer8, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, pmask9_curr, pmask9_next, globalstatsbuffer9, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer10, pmask10_curr, pmask10_next, globalstatsbuffer10, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs11(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3,globalstatsbuffer4,globalstatsbuffer5,globalstatsbuffer6,globalstatsbuffer7,globalstatsbuffer8,globalstatsbuffer9,globalstatsbuffer10, pmask0_next,pmask1_next,pmask2_next,pmask3_next,pmask4_next,pmask5_next,pmask6_next,pmask7_next,pmask8_next,pmask9_next,pmask10_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	pmask_dt pmask0_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask0_curr
	#pragma HLS DATA_PACK variable = pmask0_next
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	pmask_dt pmask1_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask1_curr
	#pragma HLS DATA_PACK variable = pmask1_next
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	pmask_dt pmask2_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask2_curr
	#pragma HLS DATA_PACK variable = pmask2_next
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	pmask_dt pmask3_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask3_curr
	#pragma HLS DATA_PACK variable = pmask3_next
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	pmask_dt pmask4_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask4_curr
	#pragma HLS DATA_PACK variable = pmask4_next
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	pmask_dt pmask5_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask5_curr
	#pragma HLS DATA_PACK variable = pmask5_next
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	pmask_dt pmask6_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask6_curr
	#pragma HLS DATA_PACK variable = pmask6_next
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	pmask_dt pmask7_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask7_curr
	#pragma HLS DATA_PACK variable = pmask7_next
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	pmask_dt pmask8_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask8_curr
	#pragma HLS DATA_PACK variable = pmask8_next
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	pmask_dt pmask9_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask9_curr
	#pragma HLS DATA_PACK variable = pmask9_next
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	pmask_dt pmask10_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask10_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask10_curr
	#pragma HLS DATA_PACK variable = pmask10_next
	keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer11
	pmask_dt pmask11_curr[BLOCKRAM_PMASK_SIZE];
	pmask_dt pmask11_next[BLOCKRAM_PMASK1_SIZE];
	#pragma HLS DATA_PACK variable = pmask11_curr
	#pragma HLS DATA_PACK variable = pmask11_next
	keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE];
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
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	LOADPARTITIONCHKPTS_LOOP1: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
	#pragma HLS PIPELINE II=1
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){
		PARTITIONP0_CHKPT[e][u] = kvdram0[globalparamsEs[e].BASEOFFSETKVS_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		cout<<">>> topkernelP: PARTITIONP0_CHKPT[0]["<<u<<"]: "<<PARTITIONP0_CHKPT[0][u]<<", globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM: "<<globalparamsEs[0].BASEOFFSETKVS_EDGESSTATSDRAM<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int vreadoffset_kvs2 = 0;
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [BEFORE]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ // NEWCHANGE.
			pmask0_curr[i].data[v] = 0; 
			pmask0_next[i].data[v] = 0; 
			pmask1_curr[i].data[v] = 0; 
			pmask1_next[i].data[v] = 0; 
			pmask2_curr[i].data[v] = 0; 
			pmask2_next[i].data[v] = 0; 
			pmask3_curr[i].data[v] = 0; 
			pmask3_next[i].data[v] = 0; 
			pmask4_curr[i].data[v] = 0; 
			pmask4_next[i].data[v] = 0; 
			pmask5_curr[i].data[v] = 0; 
			pmask5_next[i].data[v] = 0; 
			pmask6_curr[i].data[v] = 0; 
			pmask6_next[i].data[v] = 0; 
			pmask7_curr[i].data[v] = 0; 
			pmask7_next[i].data[v] = 0; 
			pmask8_curr[i].data[v] = 0; 
			pmask8_next[i].data[v] = 0; 
			pmask9_curr[i].data[v] = 0; 
			pmask9_next[i].data[v] = 0; 
			pmask10_curr[i].data[v] = 0; 
			pmask10_next[i].data[v] = 0; 
			pmask11_curr[i].data[v] = 0; 
			pmask11_next[i].data[v] = 0; 
		}
	} 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ MEMACCESSP0_readmanypmask12(vdram, pmask0_curr,pmask1_curr,pmask2_curr,pmask3_curr,pmask4_curr,pmask5_curr,pmask6_curr,pmask7_curr,pmask8_curr,pmask9_curr,pmask10_curr,pmask11_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"topkernelP: printing pmask current [AFTER]..."<<endl;
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_curr[i].data[v]<<","; } cout<<endl; } 
	for(unsigned int i=0; i<8; i++){ for(unsigned int v=0; v<1; v++){ cout<<""<<pmask0_next[i].data[v]<<","; } cout<<endl; } 
	#endif 
	#endif 
	// exit(EXIT_SUCCESS);
	
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
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
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL12_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL12_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef ENABLERECURSIVEPARTITIONING
					// if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					if(stage==0){ num_source_partitions = globalparamsV.NUM_PROCESSEDGESPARTITIONS; }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL12_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print4("### TOPKERNEL12_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", stage, source_partition, currentLOP, vreadoffset_kvs2); 
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
							if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ if(pmask0_curr[source_partition].data[0] > 0){ enableprocess = ON; } else { enableprocess = OFF; }} else{ enableprocess = ON; }
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
							#ifdef RANDOMVERTEXISACTIVE_ALGORITHM
							// if(ACTSRUNNINGMODE == BASICMODE){ MEMACCESSP0_RANDreadANDRVchunks12(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, vdatabaseoffset_kvs, randvids[BLOCKRAM_SIZE], randvids_size, globalparamsV); }
							#endif 
						}
						
						// acts 
						#ifdef TESTKERNEL	// CRITICAL FIXME.
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
	
						#else 
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, pmask0_curr, pmask0_next, globalstatsbuffer0, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, pmask1_curr, pmask1_next, globalstatsbuffer1, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, pmask3_curr, pmask3_next, globalstatsbuffer3, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, pmask4_curr, pmask4_next, globalstatsbuffer4, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, pmask5_curr, pmask5_next, globalstatsbuffer5, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, pmask6_curr, pmask6_next, globalstatsbuffer6, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, pmask7_curr, pmask7_next, globalstatsbuffer7, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, pmask8_curr, pmask8_next, globalstatsbuffer8, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, pmask9_curr, pmask9_next, globalstatsbuffer9, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer10, pmask10_curr, pmask10_next, globalstatsbuffer10, globalposition, globalparamsV);		
						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer11, pmask11_curr, pmask11_next, globalstatsbuffer11, globalposition, globalparamsV);		
	
						#endif 
						
						/* 						TOPP0_U_topkernelproc_embedded(globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, pmask2_curr, pmask2_next, globalstatsbuffer2, globalposition, globalparamsV);		
	 */
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
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

	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	MERGEP0_mergeVs12(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, vdram, 
globalstatsbuffer0,globalstatsbuffer1,globalstatsbuffer2,globalstatsbuffer3,globalstatsbuffer4,globalstatsbuffer5,globalstatsbuffer6,globalstatsbuffer7,globalstatsbuffer8,globalstatsbuffer9,globalstatsbuffer10,globalstatsbuffer11, pmask0_next,pmask1_next,pmask2_next,pmask3_next,pmask4_next,pmask5_next,pmask6_next,pmask7_next,pmask8_next,pmask9_next,pmask10_next,pmask11_next, 
			globalparamsK, globalparamsV);
	#endif 
	
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
