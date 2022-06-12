#define ENABLE_PP1
fetchmessage_t acts_all::ACTSP0_fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE],				 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams,
		unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	if(mode == ACTSPROCESSMODE){
		fetchmessage = PROCESSP0_readandprocess(enable, kvdram, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate, sweepparams, globalposition, globalparams, collections);
	} else {
		fetchmessage = MEMACCESSP0_readkeyvalues(enable, kvdram, buffer, goffset_kvs + loffset_kvs, size_kvs, travstate, globalparams); 
	}
	return fetchmessage; 
}

void acts_all::ACTSP0_commitkeyvalues(bool_type enable, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],
		keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(mode == ACTSREDUCEMODE || (globalparams.ACTSPARAMS_TREEDEPTH == 1 && globalparams.ENABLE_APPLYUPDATESCOMMAND == ON)){
		REDUCEP0_reduceandbuffer_base(enable, destbuffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
		if(globalparams.ACTSPARAMS_TREEDEPTH == 1){ for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }} // NEWCHANGE.
	} else {
		MEMACCESSP0_savekeyvalues(enable, kvdram, destbuffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalposition, globalparams); 
	}
	return;
} 

void acts_all::ACTSP0_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(enable == OFF){ return; }
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE]; // REMOVEME.-SWX
	#pragma HLS array_partition variable = buffer_setof1
	keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	
	keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1

	keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS];
	
	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;
	
	bool_type pp0readen = ON;
	bool_type pp1readen = ON;
	bool_type pp0runpipelineen = ON;
	bool_type pp1runpipelineen = ON;
	bool_type pp0partitionen = ON;
	bool_type pp1partitionen = ON;
	bool_type pp0writeen = ON;
	bool_type pp1writeen = ON;
static buffer_type pp0cutoffs[VECTOR_SIZE];
static buffer_type pp1cutoffs[VECTOR_SIZE];
	batch_type itercount = 0;
	batch_type flushsz = 0;
	
	fetchmessage_t fetchmessagepp0;
	fetchmessage_t fetchmessagepp1;
	fetchmessagepp0.chunksize_kvs = -1; fetchmessagepp0.nextoffset_kvs = -1;
	fetchmessagepp1.chunksize_kvs = -1; fetchmessagepp1.nextoffset_kvs = -1;
	
	if(resetenv == ON){ UTILP0_resetenvbuffers(capsule_so1, capsule_so8); } else { itercount = 64; } // FIXME. '64' is just some number greater than 2
	if(flush == ON){ flushsz = 1*MAX_SRCBUFFER_SIZE; } else { flushsz = 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(resetenv == ON){ cout<<"actit: reset is ON"<<endl; } else { cout<<"actit: reset is OFF"<<endl;  }
	if(flush == ON){ cout<<"actit: flush is ON"<<endl; } else { cout<<"actit: flush is OFF"<<endl;  }
	#endif 
	
	batch_type offset_kvs = ptravstate.begin_kvs;
	unsigned int workbuffer_size = globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ if(mode == ACTSPROCESSMODE){ workbuffer_size += 2; }} // inclusion of metadata else { }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit: "<<((ptravstate.end_kvs + flushsz) - ptravstate.begin_kvs) / workbuffer_size * NUMPIPELINES_PARTITIONUPDATES<<" states of acts pipeline processing..."<<endl; 
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit: processing all chunks [begin_kvs: "<<ptravstate.begin_kvs<<"][end_kvs: "<<ptravstate.end_kvs<<"][flushsz: "<<flushsz<<"][size_kvs: "<<(ptravstate.end_kvs + flushsz) - ptravstate.begin_kvs<<"][workbuffer_size: "<<workbuffer_size<<"][num_chunks: "<<((ptravstate.end_kvs + flushsz) - ptravstate.begin_kvs) / workbuffer_size<<"] ... "<<endl;
	#endif

	ACTIT_MAINLOOP: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs + flushsz; offset_kvs+=workbuffer_size * NUMPIPELINES_PARTITIONUPDATES){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts::ACTSP0_actit: processing chunk [offset_kvs: "<<offset_kvs<<"]: [ptravstate.begin_kvs: "<<ptravstate.begin_kvs<<"]: [ptravstate.end_kvs + flushsz: "<<ptravstate.end_kvs + flushsz<<"] ... "<<endl;
		#endif
		#ifdef PUP1
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 1){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 1){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		ptravstatepp0.i_kvs = offset_kvs;
		fetchmessagepp0 = ACTSP0_fetchkeyvalues(ON, mode,  kvdram, vbuffer_source, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp0.i_kvs, workbuffer_size, ptravstatepp0, sweepparams, globalposition, globalparams, edgebankID, collections);				
		#ifdef PUP1
		PARTITIONP0_stage2partitioning(pp1runpipelineen, ON, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, globalposition, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		PARTITIONP0_stage1partitioning(ON, ON, mode, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, globalposition, fetchmessagepp0.chunksize_kvs, pp0cutoffs, globalparams);
		#ifdef PUP1
		ACTSP0_commitkeyvalues(pp1writeen, ON, mode, kvdram, vbuffer_dest, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalposition, globalparams, collections); 
		#endif 
		
		PARTITIONP0_stage2partitioning(ON, ON, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, globalposition, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		ptravstatepp1.i_kvs = offset_kvs + workbuffer_size;
		fetchmessagepp1 = ACTSP0_fetchkeyvalues(ON, mode,  kvdram, vbuffer_source, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp1.i_kvs, workbuffer_size, ptravstatepp1, sweepparams, globalposition, globalparams, edgebankID, collections);
		#endif
		
		ACTSP0_commitkeyvalues(pp0writeen, ON, mode, kvdram, vbuffer_dest, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalposition, globalparams, collections); 
		#ifdef PUP1
		PARTITIONP0_stage1partitioning(pp1partitionen, ON, mode, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, globalposition, fetchmessagepp1.chunksize_kvs, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMPIPELINES_PARTITIONUPDATES;
		// exit(EXIT_SUCCESS);
	}
	// exit(EXIT_SUCCESS);
	return;
}

void acts_all::ACTSP0_actit2(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(enable == OFF){ return; } // NEWCHANGE
	
	keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = destbuffer
	keyvalue_capsule_t capsule_dest[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=capsule_dest complete
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit: processing all chunks [begin_kvs: "<<ptravstate.begin_kvs<<"][end_kvs: "<<ptravstate.end_kvs<<"][size_kvs: "<<ptravstate.end_kvs - ptravstate.begin_kvs<<"][workbuffer_size: "<<globalparams.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<(ptravstate.end_kvs - ptravstate.begin_kvs) / globalparams.ACTSPARAMS_WORKBUFFER_SIZE<<"] ... "<<endl;
	#endif
	ACTIT2_MAINLOOP: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs; offset_kvs+=globalparams.ACTSPARAMS_WORKBUFFER_SIZE){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts::ACTSP0_actit: processing chunk [offset_kvs: "<<offset_kvs<<"]: [ptravstate.begin_kvs: "<<ptravstate.begin_kvs<<"]: [ptravstate.end_kvs: "<<ptravstate.end_kvs<<"] ... "<<endl;
		#endif
		
		PARTITIONP0_read_process_partition(enable, mode, offset_kvs, sourcebaseaddr_kvs, kvdram, vbuffer_source, destbuffer, capsule_dest, ptravstate, sweepparams, globalposition, globalparams, collections);	
		
		ACTSP0_commitkeyvalues(enable, ON, mode, kvdram, vbuffer_dest, destbuffer, globalcapsule, capsule_dest, destbaseaddr_kvs, sweepparams, globalposition, globalparams, collections); 
	}
	// exit(EXIT_SUCCESS); 
	return;
}

void acts_all::ACTSP0_actit_base(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	// ACTSP0_actit(enable, mode, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparams, globalposition, sweepparams, ptravstate, sourcebaseaddr_kvs, destbaseaddr_kvs, resetenv, flush, edgebankID, collections);
	ACTSP0_actit2(enable, mode, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparams, globalposition, sweepparams, ptravstate, sourcebaseaddr_kvs, destbaseaddr_kvs, resetenv, flush, edgebankID, collections);
}

