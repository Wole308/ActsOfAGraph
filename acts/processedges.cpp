keyvalue_t PROCESSP0_processvector(bool enx, unsigned int v, unsigned int loc, unsigned int upperlimit, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VSRCDATA_SIZE], globalparams_t globalparams){				
	#pragma HLS INLINE
	keyvalue_t vupdate_dummy;
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){
		if(edata.key == INVALIDDATA){ loc = 0; }
		else {
			#ifdef _DEBUGMODE_CHECKS
			if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @@ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<", upperlimit: "<<upperlimit<<". EXITING... "<<endl; return vupdate_dummy; exit(EXIT_FAILURE); }
			// if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; return vupdate_dummy; exit(EXIT_FAILURE); }			
			#endif 
			loc = 0; }
	}
		
	// read 
	vmdata_t vmdata;
	keyvalue_vbuffer_t bits_vector = vbuffer[loc / VDATA_SHRINK_RATIO];
	if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
	if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; } vmdata.vmask = 1; // FIXME. REMOVEME.
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", en: "<<en<<", vid: "<<UTILP0_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
			
	// process
	value_t res = PROCESSP0_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	keyvalue_t vupdate; vupdate.key = edata.key; vupdate.value = res;
	
	return vupdate;
}

fetchmessage_t PROCESSP0_read_and_process(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type sourcebaseaddr_kvs, batch_type offset_kvs, batch_type size_kvs, travstate_t ptravstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	// globalparams.ACTSPARAMS_NUM_PARTITIONS replacedby 16
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	keyvalue_t edges_tup[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edges_tup complete
	keyvalue_t res[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	bool enx;
	
	fetchmessage.chunksize_kvs = size_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	ptravstate.i_kvs = offset_kvs;
	buffer_type chunk_size = UTILP0_getchunksize_kvs(globalparams.ACTSPARAMS_WORKBUFFER_SIZE, ptravstate, 0);
	unsigned int basecollections_0_2_data1 = collections[0][2].data1;
	unsigned int basecollections_0_2_data1b = collections[0][2].data1;
	
	unsigned int upperlimit = sweepparams.source_partition * globalparams.SIZEKVS2_PROCESSEDGESPARTITION;

	ACTIT2_PP0_READ_PROC_ST_MAINLOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		
		// decision flag
		enx = true;
		
		// get dataset
		UTILP0_GetDataset(edges, sourcebaseaddr_kvs + offset_kvs + i, edges_tup);
	
		
		// set flag
		enx = true;
		if((globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE) && (basecollections_0_2_data1 + VECTOR_SIZE >= globalposition.num_active_edges_in_channel)){ enx = false; } // for BFS implementation
		else { basecollections_0_2_data1b += VECTOR_SIZE; }
		
		// process
		res[0] = PROCESSP0_processvector(enx, 0, edges_tup[0].value - upperlimit, upperlimit, edges_tup[0], vbuffer_source[0], globalparams);
		res[1] = PROCESSP0_processvector(enx, 1, edges_tup[1].value - upperlimit, upperlimit, edges_tup[1], vbuffer_source[1], globalparams);
		res[2] = PROCESSP0_processvector(enx, 2, edges_tup[2].value - upperlimit, upperlimit, edges_tup[2], vbuffer_source[2], globalparams);
		res[3] = PROCESSP0_processvector(enx, 3, edges_tup[3].value - upperlimit, upperlimit, edges_tup[3], vbuffer_source[3], globalparams);
		res[4] = PROCESSP0_processvector(enx, 4, edges_tup[4].value - upperlimit, upperlimit, edges_tup[4], vbuffer_source[4], globalparams);
		res[5] = PROCESSP0_processvector(enx, 5, edges_tup[5].value - upperlimit, upperlimit, edges_tup[5], vbuffer_source[5], globalparams);
		res[6] = PROCESSP0_processvector(enx, 6, edges_tup[6].value - upperlimit, upperlimit, edges_tup[6], vbuffer_source[6], globalparams);
		res[7] = PROCESSP0_processvector(enx, 7, edges_tup[7].value - upperlimit, upperlimit, edges_tup[7], vbuffer_source[7], globalparams);
	
		
		// write to buffer 
		if(enx == true){
			destbuffer[0][i] = UTILP0_GETKV(res[0]);
			destbuffer[1][i] = UTILP0_GETKV(res[1]);
			destbuffer[2][i] = UTILP0_GETKV(res[2]);
			destbuffer[3][i] = UTILP0_GETKV(res[3]);
			destbuffer[4][i] = UTILP0_GETKV(res[4]);
			destbuffer[5][i] = UTILP0_GETKV(res[5]);
			destbuffer[6][i] = UTILP0_GETKV(res[6]);
			destbuffer[7][i] = UTILP0_GETKV(res[7]);
		}
		
		#ifdef _DEBUGMODE_STATS
		if(enx == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE); }
		#endif 
	}
	collections[0][2].data1 = basecollections_0_2_data1b;
	fetchmessage.chunksize_kvs = chunk_size; // loadcount; // CRITICAL FIXME
	// cout<<"--- processedges: chunk_size: "<<chunk_size<<", fetchmessage.chunksize_kvs: "<<fetchmessage.chunksize_kvs<<endl;
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS);
	return fetchmessage;
}

fetchmessage_t PROCESSP0_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	return PROCESSP0_read_and_process(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
}







