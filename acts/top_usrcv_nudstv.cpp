void acts_all::TOPP0_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE 
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete

	sweepparams_t sweepparams;
	sweepparams = UTILP0_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	// globalparamsK.globalvar_dramreadstats[0] = 234l
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	batch_type voffset_kvs = globalposition.source_partition * UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	bool __processit__i_am_first__ = false; 
	bool __processit__i_am_last__ = false;
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ __processit__i_am_first__ = true;; } else { __processit__i_am_first__ = false; }
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ __processit__i_am_last__ = true; } else { __processit__i_am_last__ = false; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(__processit__i_am_first__ == true){ cout<<"-------------------------------- __processit__i_am_first__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	if(__processit__i_am_last__ == true){ cout<<"-------------------------------- __processit__i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	#endif
	
	sweepparams.source_partition = globalposition.source_partition;
	bool_type resetenv = ON; bool_type flush = ON;
	unsigned int _baseoffsetkvs_edgedata = globalparamsE.BASEOFFSETKVS_EDGESDATA;
	#ifdef CONFIG_RELEASE_VERSION_DUPLICATEEDGES
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ _baseoffsetkvs_edgedata = globalparamsE.BASEOFFSETKVS_EDGES0DATA; }
	#endif
	
	ACTSP0_actit(
		ON, ACTSPROCESSMODE,
		kvdram, vbuffer, edgestats, stats, globalcapsule,
		globalparamsE, globalparamsK, globalposition, sweepparams, _baseoffsetkvs_edgedata, globalparamsK.BASEOFFSETKVS_KVDRAM,
		resetenv, flush, edgebankID, collections);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPP0_processit", globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS); }
	#endif
	return;
}

void acts_all::TOPP0_partitionit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	if(globalparamsK.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ return; } ///
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nusrcv_nudstv::UTILP0_getglobalparams:: printing global parameters", globalparamsK);
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE]; 
	globalposition_t globalposition; // DUMMY.
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparamsK.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparamsK.ACTSPARAMS_BEGINLOP + 1 + (globalparamsK.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = UTILP0_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"TOPP0_partitionit: source_partition "<<source_partition<<" ... "<<endl;

			UTILP0_resetkeysandvalues(globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
		
			sweepparams = UTILP0_getsweepparams(globalparamsK, currentLOP, source_partition);
			
			// partition
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparamsK.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### TOPP0_partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, NAp, NAp, NAp, BATCH_RANGE / (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			UTILP0_resetvalues(globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			ACTSP0_actit(config.enablepartition, ACTSPARTITIONMODE,
					kvdram, vbuffer, edgestats, stats, globalcapsule, // CRITICAL FIXME.
					globalparamsE, globalparamsK, globalposition, sweepparams, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp, collections);
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS;
			}
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalcapsule, sweepparams); }
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

void acts_all::TOPP0_reduceit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	globalposition_t globalposition; globalposition.source_partition = NAp; // DUMMY.
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	
	sweepparams = UTILP0_getsweepparams(globalparamsK, currentLOP, source_partition);
	config.enablereduce = ON;
	
	ACTSP0_actit(config.enablereduce, ACTSREDUCEMODE,
			 kvdram, vbuffer, edgestats, stats, globalcapsule, // CRITICAL FIXME.
			globalparamsE, globalparamsK, globalposition, sweepparams, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp, collections);
	return;
}

void acts_all::TOPP0_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], 
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(en_process == ON){ TOPP0_processit(kvdram, vbuffer, edgestats, stats, globalcapsule, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(en_partition == ON){ TOPP0_partitionit(kvdram, vbuffer, edgestats, stats, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(en_reduce == ON){ TOPP0_reduceit(kvdram, vbuffer, edgestats, stats, globalcapsule, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	return;
} 

void acts_all::TOPP0_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	unsigned int sourcestatsmarker = 1;
	unsigned int voffset_kvs = 0;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = globalparamsK.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("TOPP0_dispatch_reduce", globalcapsule, globalparamsK.NUM_REDUCEPARTITIONS);
	#endif
	
	bool_type enablereduce = ON;
	DISPATCHREDUCEP0_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		actsutilityobj->print4("### TOPP0_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "voffset_kvs", source_partition, currentLOP, num_source_partitions, voffset_kvs); 							
		#endif
		
		TOPP0_dispatch(OFF, OFF, enablereduce, kvdram, vbuffer, edgestats, stats, globalcapsule, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);
		
		sourcestatsmarker += 1;
		voffset_kvs += globalparamsK.SIZEKVS2_REDUCEPARTITION;
	}
	return;
} 

void acts_all::TOPP0_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[COLLECTIONS_BUFFERSZ]){									
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	unsigned int printheader1=OFF; // ON, OFF*
	unsigned int printheader2=OFF; 
	if(true && globalposition.source_partition==globalposition.first_source_partition){ printheader1 = ON; } else { printheader1 = OFF; }
	if(false && globalposition.source_partition==globalposition.last_source_partition){ printheader2 = ON; } else { printheader2 = OFF; }
	// printheader1=ON;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> ====================== ACTS Launched @ instance: "<<globalid<<" ====================== "<<endl;
	#endif
		
	#ifdef TESTKERNEL 
	if(UTILP0_isbufferused(globalid) == false){ return; }
	#endif 
	#ifdef SW
	globalvar_actsinstid = globalid;
	#endif 
	
	unsigned int PARTITIONP0_CHKPT[EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
	
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
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	bool __processit__i_am_first__ = true; if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ __processit__i_am_first__ = true;; } else { __processit__i_am_first__ = false; }
	if(__processit__i_am_first__ == true){ UTILP0_resetvalues(globalcapsule, globalparamsK.NUM_REDUCEPARTITIONS, 0); } 
	if(__processit__i_am_first__ == true){ UTILP0_resetkvstatvalues(kvdram, globalparamsK); } // NEWCHANGE
	if(__processit__i_am_first__ == true){ MEMACCESSP0_readglobalstats(ON, kvdram, globalcapsule, globalparamsK.BASEOFFSETKVS_STATSDRAM, globalparamsK); }
	if(__processit__i_am_first__ == true){
		for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
			for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
			#pragma HLS UNROLL
				stats[v][t] = 0;
			}
		}
	}
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP0_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, vbuffer, edgestats, stats, globalcapsule, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT, globalposition.v_chunkid, globalposition.edgebankID, collections); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE  // CRITICAL REMOVEME URGENT.
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP0_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, vbuffer, edgestats, stats, globalcapsule, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP0_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	// reduce & partition
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP0_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP0_dispatch_reduce(kvdram, vbuffer, edgestats, stats, globalcapsule, _globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONP0_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	bool __reduceit__i_am_last__ = false;
	if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ __reduceit__i_am_last__ = true; }
	if(__reduceit__i_am_last__ == true){ UTILP0_increment_graphiteration(kvdram, globalparamsK); } // NB: this should come last.
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(__reduceit__i_am_last__ == true){ actsutilityobj->printglobalvars2(globalparamsK.ACTSPARAMS_INSTID, "instID", "v_chunkid", "LOP", "sourcep", globalparamsK.ACTSPARAMS_INSTID, globalposition.v_chunkid, globalposition.currentLOP, globalposition.source_partition); }
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL1_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL1_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL1_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL1_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL1_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL1_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs1(vdram, kvdram0, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs1(vdram, kvdram0, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP1"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL2_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL2_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL2_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL2_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL2_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL2_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs2(vdram, kvdram0,kvdram1, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs2(vdram, kvdram0,kvdram1, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP2"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL3_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL3_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL3_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL3_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL3_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL3_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs3(vdram, kvdram0,kvdram1,kvdram2, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs3(vdram, kvdram0,kvdram1,kvdram2, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP3"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL4_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL4_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL4_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL4_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL4_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL4_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs4(vdram, kvdram0,kvdram1,kvdram2,kvdram3, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs4(vdram, kvdram0,kvdram1,kvdram2,kvdram3, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP4"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL5_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL5_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL5_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL5_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL5_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL5_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs5(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs5(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram4, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP5"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL6_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL6_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL6_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL6_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL6_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL6_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs6(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs6(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram5, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP6"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL7_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL7_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL7_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL7_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL7_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL7_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs7(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs7(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram6, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP7"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL8_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL8_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL8_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL8_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL8_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL8_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs8(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs8(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram7, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP8"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer8
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer8 type=block factor=2
		// #pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
	workload_t edgestats8[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];
	collection_t collections8[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections8 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL9_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL9_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL9_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL9_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL9_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL9_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs9(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs9(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, edgestats8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram7, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram8, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP9"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer8
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer8 type=block factor=2
		// #pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
	workload_t edgestats8[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];
	collection_t collections8[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections8 complete	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer9
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer9 type=block factor=2
		// #pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
	workload_t edgestats9[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];
	collection_t collections9[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections9 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL10_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL10_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL10_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL10_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL10_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL10_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs10(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs10(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, edgestats8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, edgestats9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram7, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram8, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram9, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP10"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer8
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer8 type=block factor=2
		// #pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
	workload_t edgestats8[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];
	collection_t collections8[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections8 complete	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer9
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer9 type=block factor=2
		// #pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
	workload_t edgestats9[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];
	collection_t collections9[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections9 complete	
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer10
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer10 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer10 type=block factor=2
		// #pragma HLS resource variable=vbuffer10 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats10[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats10
	workload_t edgestats10[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule10[BLOCKRAM_SIZE];
	collection_t collections10[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections10 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  collections10[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL11_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL11_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL11_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL11_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL11_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL11_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs11(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs11(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, edgestats8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, edgestats9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer10, edgestats10, stats10, globalcapsule10, globalposition, globalparamsV, collections10);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram7, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram8, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram9, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram10, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP11"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer8
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer8 type=block factor=2
		// #pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
	workload_t edgestats8[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];
	collection_t collections8[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections8 complete	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer9
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer9 type=block factor=2
		// #pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
	workload_t edgestats9[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];
	collection_t collections9[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections9 complete	
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer10
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer10 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer10 type=block factor=2
		// #pragma HLS resource variable=vbuffer10 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats10[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats10
	workload_t edgestats10[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule10[BLOCKRAM_SIZE];
	collection_t collections10[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections10 complete	
	keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer11
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer11 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer11 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer11 type=block factor=2
		// #pragma HLS resource variable=vbuffer11 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer11 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer11 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats11[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats11
	workload_t edgestats11[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule11[BLOCKRAM_SIZE];
	collection_t collections11[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections11 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  collections10[t].data1 = 0;  collections11[t].data1 = 0;  }

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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	
	UTILP0_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP0_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL12_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL12_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL12_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL12_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP0_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP0_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP0_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL12_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL12_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP0_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						
						// hyrid heuristics
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
							if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || globalposition.num_active_edgeblocks > 0){
								unsigned int depth_i = 0, transfsz_kvs = 0;
								unsigned int buffer_offsets[BLOCKRAM_SIZE];
								workload_t xload_kvs[BLOCKRAM_SIZE];
								unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
								bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
								
								if(sparse_readu == true){
									for(unsigned int n=0; n<num_its; n++){
										unsigned int s = xload_kvs[n].offset_begin / NUM_VERTICESKVS_PER_UPROPBLOCK; // CHECKME.
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (xload_kvs[n].offset_begin % NUM_VERTICESKVS_PER_UPROPBLOCK);
										#ifdef _DEBUGMODE_CHECKS3		
										actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);
										#endif			
										MERGEP0_broadcastVs12(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, xload_kvs[n], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[n].size;
									}
								} else {
									depth_i = 0;
									for(unsigned int s=0; s<NUM_PEs; s++){
										unsigned int offsetSRC = (s * globalparamsV.SIZEKVS2_REDUCEPARTITION) + (source_partition * (globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs));
										MERGEP0_broadcastVs12(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, xload_kvs[0], globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA);
										transfsz_kvs += xload_kvs[0].size;
									}
								}
								
								#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
								collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE;
								collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += (transfsz_kvs + (NUM_PEs*REPORT__DRAM_ACCESS_LATENCY)) * VECTOR2_SIZE; 
								#endif 
								cout<<"----------- top_nusrcv_nudstv-------------------------: transfsz: "<<transfsz_kvs * VECTOR2_SIZE<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl;
								
							}
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, edgestats8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, edgestats9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer10, edgestats10, stats10, globalcapsule10, globalposition, globalparamsV, collections10);		
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer11, edgestats11, stats11, globalcapsule11, globalposition, globalparamsV, collections11);		
	
						#else 
						TOPP0_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILP0_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram7, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram8, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram9, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram10, vdram); 
	
		transfsz_kvs += MERGEP0_mergeVs(kvdram11, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP0_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
	#endif 

	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PRINTGLOBALVARS @ topkernelP12"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
	

