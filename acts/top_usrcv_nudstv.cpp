void acts_all::TOPP0_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	for(buffer_type t=0; t<globalparamsK.SIZEKVS2_REDUCEPARTITION; t++){
		for(buffer_type v=0; v<VDATA_PACKINGSIZE; v++){
			if(MEMCAP0_READVMASK(vbuffer_source[v][t]) == 1){ cout<<"TOPP0_processit: ACTIVE MASK SEEN: @ t: "<<t<<", v: "<<v<<", vbuffer_source["<<v<<"]["<<t<<"]: "<<vbuffer_source[v][t]<<endl; }
		}
	}
	#endif

	sweepparams_t sweepparams;
	if(globalparamsK.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ globalposition.deststatsmarker = MERGEP0_actvpstatsoffset(globalparamsK); }
	
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP0_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0); } 
	sweepparams = UTILP0_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	
	if(globalposition.source_partition == globalposition.first_source_partition){ MEMACCESSP0_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL NEWCHANGE.
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP0_resetvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0); } // CRITICAL NEWCHANGE.
	
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); //. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print7("### TOPP0_processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif

	batch_type voffset_kvs = globalposition.source_partition * UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	bool returncmd = false;
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	if(globalposition.num_active_vertices == 0){ returncmd = true; } // check if vertex partition is active
	else{
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; 
		#endif 
	}
	#endif 
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ returncmd = false; } // for certain algorithms like pagerank
	if(voffset_kvs >= avtravstate.end_kvs){ returncmd = true; } // check if we are at the end of file
	
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && globalposition.stage == 0 && globalposition.source_partition == globalposition.first_source_partition){
		MEMACCESSP0_readV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
	}
	
	if(returncmd == false){
		sweepparams.source_partition = globalposition.source_partition;
		avtravstate.i_kvs = voffset_kvs;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print5("### TOPP0_processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, MAX_SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + (UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + (UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
			
		unsigned int SKIP_KVS = UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
		
		tuple_t tup = MEMACCESSP0_getvptrs_opt(kvdram, vptrbaseoffset_kvs, voffset_kvs * VECTOR2_SIZE, (voffset_kvs + SKIP_KVS) * VECTOR2_SIZE, edgebankID, globalparamsK);
		keyy_t beginvptr = tup.A;
		keyy_t endvptr = tup.B; 
		
		#ifdef _DEBUGMODE_CHECKS3
		if(globalparamsK.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ if(beginvptr % VECTOR2_SIZE != 0 || endvptr % VECTOR2_SIZE != 0){ cout<<"TOPP0_processit::ERROR: beginvptr("<<beginvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0 || endvptr("<<endvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }}
		#endif 
		
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr;
		keyy_t localendvptr = endvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 1*VECTOR_SIZE; // CRITICAL NEWCHANGE.
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN____UNUSED // FIXME. REMOVEME WHEN IN RUNNING MODE.
		if(sweepparams.source_partition < 64){
			UTILP0_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + sweepparams.source_partition, localbeginvptr_kvs);
			UTILP0_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + sweepparams.source_partition, numedges); 
		}
		#endif 
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		collections[0][0].data1 += numedges;
		#endif
		
		#ifdef _DEBUGMODE_CHECKS3
		if(localendvptr < localbeginvptr){ cout<<"TOPP0_processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<", voffset_kvs: "<<voffset_kvs<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("TOPP0_processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS//3
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
		etravstate.skip_kvs = MAX_SRCBUFFER_SIZE;
		etravstate.i_kvs = etravstate.begin_kvs;
		etravstate.i2 = voffset_kvs * VECTOR2_SIZE;
		etravstate.i2_kvs = voffset_kvs;
		bool_type resetenv = ON;
		bool_type flush = ON;
		
		if(voffset_kvs == avtravstate.begin_kvs || globalposition.source_partition == globalposition.first_source_partition){ resetenv = ON; } else { resetenv = OFF; }
		if((voffset_kvs + UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		resetenv = ON; flush = ON; 
		
		ACTSP0_actit_base(
			ON, ACTSPROCESSMODE,
			kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer,
			globalparamsK, globalposition, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAM,
			resetenv, flush, edgebankID, collections);
			
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
			if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ collections[0][1].data1 = collections[0][2].data1; } else { collections[0][1].data1 += numedges; }}
		#endif 
	}
	
	bool endofrun = false;
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ endofrun = true; }
	
	if(endofrun == true){ MEMACCESSP0_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL OPTIMIZEME. should be called only once
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPP0_processit", globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS); }
	#endif	
	
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && endofrun == true){ 
		MEMACCESSP0_saveV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP0_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][1].data1);
		#endif
	}
	
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

void acts_all::TOPP0_partitionit( uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparams, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ return; } ///
	
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
	batch_type deststatsmarker = 1 + globalparams.ACTSPARAMS_NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparams.ACTSPARAMS_BEGINLOP + 1 + (globalparams.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = UTILP0_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"TOPP0_partitionit: source_partition "<<source_partition<<" ... "<<endl;

			UTILP0_resetkeysandvalues(globalstatsbuffer, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
		
			sweepparams = UTILP0_getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t ptravstate = UTILP0_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
			
			// collect stats
			MEMACCESSP0_readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
			UTILP0_resetvalues(globalstatsbuffer, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### TOPP0_partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			UTILP0_resetvalues(globalstatsbuffer, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			ACTSP0_actit_base(config.enablepartition, ACTSPARTITIONMODE,
					kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, // CRITICAL FIXME.
					globalparams, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp, collections);
					
			MEMACCESSP0_saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += globalparams.ACTSPARAMS_NUM_PARTITIONS;
			}
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_STATS
			if(config.enablereduce == ON){ edges_count = actsutilityobj->globalstats_getcountvalidkvsreduced(globalparams.ACTSPARAMS_INSTID); edgesdstv_sum = actsutilityobj->globalstats_getreducevar1(); }
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

void acts_all::TOPP0_reduceit( uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; 
	globalposition_t globalposition; // DUMMY.
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	
	sweepparams = UTILP0_getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = UTILP0_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
	UTILP0_resetkeysandvalues(globalstatsbuffer, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### TOPP0_reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	ACTSP0_actit_base(config.enablereduce, ACTSREDUCEMODE,
			 kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, // CRITICAL FIXME.
			globalparams, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp, collections);
	return;
}

void acts_all::TOPP0_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(en_process == ON){ TOPP0_processit(kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(en_partition == ON){ TOPP0_partitionit(kvdram, vbuffer_source, vbuffer_dest, globalparamsK, NAp, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(en_reduce == ON){ TOPP0_reduceit(kvdram, vbuffer_source, vbuffer_dest, sourcestatsmarker, source_partition, globalparamsK, NAp, collections); } 
	#endif 
	return;
} 

void acts_all::TOPP0_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); }
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int vmask_offset_kvs = 0;
	unsigned int vmaskp_offset_kvs = 0;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = globalparamsK.NUM_REDUCEPARTITIONS;
	
	bool_type enablereduce = ON;
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; // NOT USED.
	
	UTILP0_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
	
	DISPATCHREDUCEP0_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### TOPP0_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "vreadoffset_kvs2", source_partition, currentLOP, num_source_partitions, vreadoffset_kvs2); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTILP0_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		globalstatsbuffer[source_partition].key = NAp; globalstatsbuffer[source_partition].value = rtravstate.size_kvs;
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		collections[0][1].data1 += rtravstate.size_kvs * VECTOR_SIZE;
		#endif 
		
		// read vertices
		MEMACCESSP0_readV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		// if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID == CF){ MEMACCESSP0_copyV(enablereduce, vbuffer_dest, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.BUFFERBASEOFFSETKVS_VDATA2, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK); } // CUSTOMPROVISION
		
		// reduce
		TOPP0_dispatch(OFF, OFF, enablereduce, kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);
		
		// writeback vertices
		MEMACCESSP0_saveV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		
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

void acts_all::TOPP0_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){									

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
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	unsigned int PARTITIONP0_CHKPT[EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
	#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
	keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_dest	
	#ifdef CONFIG_USEURAM_FOR_DESTVBUFFER
		#pragma HLS resource variable=vbuffer_dest core=XPM_MEMORY uram
		// #pragma HLS bind_storage variable=vbuffer_dest type=RAM_T2P impl=uram // REMOVEME?
		// #pragma HLS array_reshape variable=vbuffer_dest type=block factor=2
	#endif 
	#endif 
	
	globalparams_t globalparamsK;
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	globalparams_t _globalparamsE;
	
	globalparamsK = UTILP0_getglobalparams(kvdram, 0); // CRITICAL OPTIMIZEME. MOVETOBASE?
	
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram, e+1); }
	_globalparamsE = globalparamsEs[globalposition.edgebankID];
	globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid;
	for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE. IS THIS OPTIMIZED
	
	globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = GraphIter; // overrride these with GraphIter
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e].ALGORITHMINFO_GRAPHITERATIONID = GraphIter; }
	_globalparamsE.ALGORITHMINFO_GRAPHITERATIONID = GraphIter;
	
	#ifdef CONFIG_SEPERATESRCVDESTVBUFFER // reset vbuffer
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		for (buffer_type i=0; i<MAX_BLOCKRAM_VDESTDATA_SIZE; i++){ // reset vbuffer_source 
			vbuffer_dest[0][i] = 0;
			vbuffer_dest[1][i] = 0;
			vbuffer_dest[2][i] = 0;
			vbuffer_dest[3][i] = 0;
			vbuffer_dest[4][i] = 0;
			vbuffer_dest[5][i] = 0;
			vbuffer_dest[6][i] = 0;
			vbuffer_dest[7][i] = 0;
			vbuffer_dest[8][i] = 0;
			vbuffer_dest[9][i] = 0;
			vbuffer_dest[10][i] = 0;
			vbuffer_dest[11][i] = 0;
			vbuffer_dest[12][i] = 0;
			vbuffer_dest[13][i] = 0;
			vbuffer_dest[14][i] = 0;
			vbuffer_dest[15][i] = 0;
		}
	}
	#endif 
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ UTILP0_resetkvstatvalues(kvdram, globalparamsK); } // NEWCHANGE
	#endif
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
				vbuffer_dest, 
					#else 
						vbuffer_source,	
							#endif 
								globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT, globalposition.v_chunkid, globalposition.edgebankID, collections); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
			vbuffer_dest, 
				#else 
					vbuffer_source,		
						#endif 
							globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	// reduce & partition
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP0_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_dispatch_reduce(kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
			vbuffer_dest, 
				#else 
					vbuffer_source,		
						#endif 
							_globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONP0_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	bool endofrun = false;
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }} 
	else { if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }}
	#if not defined(FPGA_IMPL) && defined(CONFIG_COLLECT_DATAS3_DURING_RUN)
	if(endofrun == true){
		UTILP0_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, actsutilityobj->globalstats_getcountnumvalidprocessedges(globalparamsK.ACTSPARAMS_INSTID)); // REMOVEME.
		UTILP0_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, actsutilityobj->globalstats_getcountkvsreduced(globalparamsK.ACTSPARAMS_INSTID)); // globalstats_getcountvalidkvsreduced, *globalstats_getcountkvsreduced, globalstats_getcountkvspartitionswritten
	}
	#endif 
	if(endofrun == true){
		unsigned int _offset = MERGEP0_actvpstatsoffset(globalparamsK);
		MEMACCESSP0_retreievekvstats(kvdram, globalstatsbuffer, globalparamsK, _offset, globalparamsK.NUM_REDUCEPARTITIONS);
		UTILP0_increment_graphiteration(kvdram, globalparamsK); } // NB: this should come last.
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(endofrun == true){ actsutilityobj->printglobalvars2(globalparamsK.ACTSPARAMS_INSTID, "instID", "v_chunkid", "LOP", "sourcep", globalparamsK.ACTSPARAMS_INSTID, globalposition.v_chunkid, globalposition.currentLOP, globalposition.source_partition); }
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void acts_all::TOPP0_topkernelP1(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[1];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL1_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks1(ON, vdram, vbuffer_source0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP1"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP2(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[2];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL2_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks2(ON, vdram, vbuffer_source0,vbuffer_source1, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP0_mergeVs(kvdram1, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP2"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP3(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[3];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL3_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks3(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP0_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP0_mergeVs(kvdram2, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP3"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP4(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[4];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL4_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks4(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP0_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP0_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP0_mergeVs(kvdram3, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP4"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP5(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[5];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL5_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks5(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
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
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP5"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP6(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[6];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL6_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks6(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
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
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP6"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP7(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[7];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL7_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks7(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
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
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP7"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP8(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[8];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL8_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks8(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
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
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP8"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP9(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source8
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source8 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];	
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections8 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[9];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
		vbuffer_source8[0][i] = 0;
		vbuffer_source8[1][i] = 0;
		vbuffer_source8[2][i] = 0;
		vbuffer_source8[3][i] = 0;
		vbuffer_source8[4][i] = 0;
		vbuffer_source8[5][i] = 0;
		vbuffer_source8[6][i] = 0;
		vbuffer_source8[7][i] = 0;
		vbuffer_source8[8][i] = 0;
		vbuffer_source8[9][i] = 0;
		vbuffer_source8[10][i] = 0;
		vbuffer_source8[11][i] = 0;
		vbuffer_source8[12][i] = 0;
		vbuffer_source8[13][i] = 0;
		vbuffer_source8[14][i] = 0;
		vbuffer_source8[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL9_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks9(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
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
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP9"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP10(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source8
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source8 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];	
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections8 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source9
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source9 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];	
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections9 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[10];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
		vbuffer_source8[0][i] = 0;
		vbuffer_source8[1][i] = 0;
		vbuffer_source8[2][i] = 0;
		vbuffer_source8[3][i] = 0;
		vbuffer_source8[4][i] = 0;
		vbuffer_source8[5][i] = 0;
		vbuffer_source8[6][i] = 0;
		vbuffer_source8[7][i] = 0;
		vbuffer_source8[8][i] = 0;
		vbuffer_source8[9][i] = 0;
		vbuffer_source8[10][i] = 0;
		vbuffer_source8[11][i] = 0;
		vbuffer_source8[12][i] = 0;
		vbuffer_source8[13][i] = 0;
		vbuffer_source8[14][i] = 0;
		vbuffer_source8[15][i] = 0;
	
		vbuffer_source9[0][i] = 0;
		vbuffer_source9[1][i] = 0;
		vbuffer_source9[2][i] = 0;
		vbuffer_source9[3][i] = 0;
		vbuffer_source9[4][i] = 0;
		vbuffer_source9[5][i] = 0;
		vbuffer_source9[6][i] = 0;
		vbuffer_source9[7][i] = 0;
		vbuffer_source9[8][i] = 0;
		vbuffer_source9[9][i] = 0;
		vbuffer_source9[10][i] = 0;
		vbuffer_source9[11][i] = 0;
		vbuffer_source9[12][i] = 0;
		vbuffer_source9[13][i] = 0;
		vbuffer_source9[14][i] = 0;
		vbuffer_source9[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL10_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks10(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
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
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP10"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP11(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source8
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source8 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];	
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections8 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source9
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source9 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];	
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections9 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source10
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source10 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source10 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source10 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE];	
	collection_t collections10[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections10 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[11];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
		vbuffer_source8[0][i] = 0;
		vbuffer_source8[1][i] = 0;
		vbuffer_source8[2][i] = 0;
		vbuffer_source8[3][i] = 0;
		vbuffer_source8[4][i] = 0;
		vbuffer_source8[5][i] = 0;
		vbuffer_source8[6][i] = 0;
		vbuffer_source8[7][i] = 0;
		vbuffer_source8[8][i] = 0;
		vbuffer_source8[9][i] = 0;
		vbuffer_source8[10][i] = 0;
		vbuffer_source8[11][i] = 0;
		vbuffer_source8[12][i] = 0;
		vbuffer_source8[13][i] = 0;
		vbuffer_source8[14][i] = 0;
		vbuffer_source8[15][i] = 0;
	
		vbuffer_source9[0][i] = 0;
		vbuffer_source9[1][i] = 0;
		vbuffer_source9[2][i] = 0;
		vbuffer_source9[3][i] = 0;
		vbuffer_source9[4][i] = 0;
		vbuffer_source9[5][i] = 0;
		vbuffer_source9[6][i] = 0;
		vbuffer_source9[7][i] = 0;
		vbuffer_source9[8][i] = 0;
		vbuffer_source9[9][i] = 0;
		vbuffer_source9[10][i] = 0;
		vbuffer_source9[11][i] = 0;
		vbuffer_source9[12][i] = 0;
		vbuffer_source9[13][i] = 0;
		vbuffer_source9[14][i] = 0;
		vbuffer_source9[15][i] = 0;
	
		vbuffer_source10[0][i] = 0;
		vbuffer_source10[1][i] = 0;
		vbuffer_source10[2][i] = 0;
		vbuffer_source10[3][i] = 0;
		vbuffer_source10[4][i] = 0;
		vbuffer_source10[5][i] = 0;
		vbuffer_source10[6][i] = 0;
		vbuffer_source10[7][i] = 0;
		vbuffer_source10[8][i] = 0;
		vbuffer_source10[9][i] = 0;
		vbuffer_source10[10][i] = 0;
		vbuffer_source10[11][i] = 0;
		vbuffer_source10[12][i] = 0;
		vbuffer_source10[13][i] = 0;
		vbuffer_source10[14][i] = 0;
		vbuffer_source10[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  collections10[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL11_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks11(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, globalstatsbuffer10, globalposition, globalparamsV, collections10);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
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
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP11"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void acts_all::TOPP0_topkernelP12(
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
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source8
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source8 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];	
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections8 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source9
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source9 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];	
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections9 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source10
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source10 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source10 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source10 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE];	
	collection_t collections10[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections10 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source11
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source11 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source11 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source11 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source11 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source11 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source11 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE];	
	collection_t collections11[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections11 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[12];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP0_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP0_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP0_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
		vbuffer_source8[0][i] = 0;
		vbuffer_source8[1][i] = 0;
		vbuffer_source8[2][i] = 0;
		vbuffer_source8[3][i] = 0;
		vbuffer_source8[4][i] = 0;
		vbuffer_source8[5][i] = 0;
		vbuffer_source8[6][i] = 0;
		vbuffer_source8[7][i] = 0;
		vbuffer_source8[8][i] = 0;
		vbuffer_source8[9][i] = 0;
		vbuffer_source8[10][i] = 0;
		vbuffer_source8[11][i] = 0;
		vbuffer_source8[12][i] = 0;
		vbuffer_source8[13][i] = 0;
		vbuffer_source8[14][i] = 0;
		vbuffer_source8[15][i] = 0;
	
		vbuffer_source9[0][i] = 0;
		vbuffer_source9[1][i] = 0;
		vbuffer_source9[2][i] = 0;
		vbuffer_source9[3][i] = 0;
		vbuffer_source9[4][i] = 0;
		vbuffer_source9[5][i] = 0;
		vbuffer_source9[6][i] = 0;
		vbuffer_source9[7][i] = 0;
		vbuffer_source9[8][i] = 0;
		vbuffer_source9[9][i] = 0;
		vbuffer_source9[10][i] = 0;
		vbuffer_source9[11][i] = 0;
		vbuffer_source9[12][i] = 0;
		vbuffer_source9[13][i] = 0;
		vbuffer_source9[14][i] = 0;
		vbuffer_source9[15][i] = 0;
	
		vbuffer_source10[0][i] = 0;
		vbuffer_source10[1][i] = 0;
		vbuffer_source10[2][i] = 0;
		vbuffer_source10[3][i] = 0;
		vbuffer_source10[4][i] = 0;
		vbuffer_source10[5][i] = 0;
		vbuffer_source10[6][i] = 0;
		vbuffer_source10[7][i] = 0;
		vbuffer_source10[8][i] = 0;
		vbuffer_source10[9][i] = 0;
		vbuffer_source10[10][i] = 0;
		vbuffer_source10[11][i] = 0;
		vbuffer_source10[12][i] = 0;
		vbuffer_source10[13][i] = 0;
		vbuffer_source10[14][i] = 0;
		vbuffer_source10[15][i] = 0;
	
		vbuffer_source11[0][i] = 0;
		vbuffer_source11[1][i] = 0;
		vbuffer_source11[2][i] = 0;
		vbuffer_source11[3][i] = 0;
		vbuffer_source11[4][i] = 0;
		vbuffer_source11[5][i] = 0;
		vbuffer_source11[6][i] = 0;
		vbuffer_source11[7][i] = 0;
		vbuffer_source11[8][i] = 0;
		vbuffer_source11[9][i] = 0;
		vbuffer_source11[10][i] = 0;
		vbuffer_source11[11][i] = 0;
		vbuffer_source11[12][i] = 0;
		vbuffer_source11[13][i] = 0;
		vbuffer_source11[14][i] = 0;
		vbuffer_source11[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  collections10[v][t].data1 = 0;  collections11[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONP0_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP0_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP0_CHKPT["<<u<<"]: "<<PARTITIONP0_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
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
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
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
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
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
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
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
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL12_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP0_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
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
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices_in_partition, num_active_edges_in_channel = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ globalposition.num_active_vertices = 0; globalposition.num_active_edges_in_channel = 0; enable_readandreplicatevdata = OFF; } // force a no-activity
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP0_readANDRVchunks12(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10,vbuffer_source11, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#ifdef NOT_USED
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, globalstatsbuffer10, globalposition, globalparamsV, collections10);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer_source11, globalstatsbuffer11, globalposition, globalparamsV, collections11);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD){
	
		globalparams_t globalparamsK0;
		MERGEP0_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
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
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP12"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
	
