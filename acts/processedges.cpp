void acts_all::PROCESSP0_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[MAX_SOURCEBLOCKRAM_SIZE], unsigned int bufferoffset_kvs, unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams){				
	#pragma HLS INLINE
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS
		if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
	// read 
	vmdata_t vmdata;
	keyvalue_vbuffer_t bits_vector = vbuffer[bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO)];
	if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
	vmdata.vmask = 1; // FIXME. REMOVEME.
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", en: "<<en<<", vid: "<<UTILP0_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
			
	// process
	value_t res = PROCESSP0_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	keyvalue_t mykeyvalue; mykeyvalue.key = edata.key; mykeyvalue.value = res;
	
	// write
	if(en == true && vmdata.vmask == 1 && *loadcount < globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2){ buffer[*loadcount] = UTILP0_GETKV(mykeyvalue); }
	if(en == true && vmdata.vmask == 1 && *loadcount < globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2){ *loadcount += 1; } // CRITICAL FIXME. globalparams.ACTSPARAMS_WORKBUFFER_SIZE
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1);
	if(en == true && vmdata.vmask == 1 && *loadcount < globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1); } // mask0? FIXME.
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("PROCESSP0_processvector::DEBUG CODE 14::1", *loadcount, MAX_SRCBUFFER_SIZE, MAX_SOURCEBLOCKRAM_SIZE, MAX_SRCBUFFER_SIZE, NAp); // MAX_SOURCEBLOCKRAM_SIZE
	#endif
	return;
}

keyvalue_t acts_all::PROCESSP0_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VSRCDATA_SIZE], globalparams_t globalparams){				
	#pragma HLS INLINE
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS
		if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
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

void acts_all::PROCESSP0_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth){
	unsigned int s_ = s % VECTOR_SIZE;
	
 if(s_==0){ 
		depths[0] = 0; 
		depths[1] = 1; 
		depths[2] = 2; 
		depths[3] = 3; 
		depths[4] = 4; 
		depths[5] = 5; 
		depths[6] = 6; 
		depths[7] = 7; 
	}
else if(s_==1){ 
		depths[0] = 7; 
		depths[1] = 0; 
		depths[2] = 1; 
		depths[3] = 2; 
		depths[4] = 3; 
		depths[5] = 4; 
		depths[6] = 5; 
		depths[7] = 6; 
	}
else if(s_==2){ 
		depths[0] = 6; 
		depths[1] = 7; 
		depths[2] = 0; 
		depths[3] = 1; 
		depths[4] = 2; 
		depths[5] = 3; 
		depths[6] = 4; 
		depths[7] = 5; 
	}
else if(s_==3){ 
		depths[0] = 5; 
		depths[1] = 6; 
		depths[2] = 7; 
		depths[3] = 0; 
		depths[4] = 1; 
		depths[5] = 2; 
		depths[6] = 3; 
		depths[7] = 4; 
	}
else if(s_==4){ 
		depths[0] = 4; 
		depths[1] = 5; 
		depths[2] = 6; 
		depths[3] = 7; 
		depths[4] = 0; 
		depths[5] = 1; 
		depths[6] = 2; 
		depths[7] = 3; 
	}
else if(s_==5){ 
		depths[0] = 3; 
		depths[1] = 4; 
		depths[2] = 5; 
		depths[3] = 6; 
		depths[4] = 7; 
		depths[5] = 0; 
		depths[6] = 1; 
		depths[7] = 2; 
	}
else if(s_==6){ 
		depths[0] = 2; 
		depths[1] = 3; 
		depths[2] = 4; 
		depths[3] = 5; 
		depths[4] = 6; 
		depths[5] = 7; 
		depths[6] = 0; 
		depths[7] = 1; 
	}
else { 
		depths[0] = 1; 
		depths[1] = 2; 
		depths[2] = 3; 
		depths[3] = 4; 
		depths[4] = 5; 
		depths[5] = 6; 
		depths[6] = 7; 
		depths[7] = 0; 
	}
	return;
}

void acts_all::PROCESSP0_RearrangeLayoutV(unsigned int s, uint32_type vdata[VECTOR_SIZE], uint32_type vdata2[VECTOR_SIZE]){
	unsigned int s_ = s;// % VECTOR_SIZE;
 if(s_==0){ 
		vdata2[0] = vdata[0]; 
		vdata2[1] = vdata[1]; 
		vdata2[2] = vdata[2]; 
		vdata2[3] = vdata[3]; 
		vdata2[4] = vdata[4]; 
		vdata2[5] = vdata[5]; 
		vdata2[6] = vdata[6]; 
		vdata2[7] = vdata[7]; 
	}
else if(s_==1){ 
		vdata2[7] = vdata[0]; 
		vdata2[0] = vdata[1]; 
		vdata2[1] = vdata[2]; 
		vdata2[2] = vdata[3]; 
		vdata2[3] = vdata[4]; 
		vdata2[4] = vdata[5]; 
		vdata2[5] = vdata[6]; 
		vdata2[6] = vdata[7]; 
	}
else if(s_==2){ 
		vdata2[6] = vdata[0]; 
		vdata2[7] = vdata[1]; 
		vdata2[0] = vdata[2]; 
		vdata2[1] = vdata[3]; 
		vdata2[2] = vdata[4]; 
		vdata2[3] = vdata[5]; 
		vdata2[4] = vdata[6]; 
		vdata2[5] = vdata[7]; 
	}
else if(s_==3){ 
		vdata2[5] = vdata[0]; 
		vdata2[6] = vdata[1]; 
		vdata2[7] = vdata[2]; 
		vdata2[0] = vdata[3]; 
		vdata2[1] = vdata[4]; 
		vdata2[2] = vdata[5]; 
		vdata2[3] = vdata[6]; 
		vdata2[4] = vdata[7]; 
	}
else if(s_==4){ 
		vdata2[4] = vdata[0]; 
		vdata2[5] = vdata[1]; 
		vdata2[6] = vdata[2]; 
		vdata2[7] = vdata[3]; 
		vdata2[0] = vdata[4]; 
		vdata2[1] = vdata[5]; 
		vdata2[2] = vdata[6]; 
		vdata2[3] = vdata[7]; 
	}
else if(s_==5){ 
		vdata2[3] = vdata[0]; 
		vdata2[4] = vdata[1]; 
		vdata2[5] = vdata[2]; 
		vdata2[6] = vdata[3]; 
		vdata2[7] = vdata[4]; 
		vdata2[0] = vdata[5]; 
		vdata2[1] = vdata[6]; 
		vdata2[2] = vdata[7]; 
	}
else if(s_==6){ 
		vdata2[2] = vdata[0]; 
		vdata2[3] = vdata[1]; 
		vdata2[4] = vdata[2]; 
		vdata2[5] = vdata[3]; 
		vdata2[6] = vdata[4]; 
		vdata2[7] = vdata[5]; 
		vdata2[0] = vdata[6]; 
		vdata2[1] = vdata[7]; 
	}
else { 
		vdata2[1] = vdata[0]; 
		vdata2[2] = vdata[1]; 
		vdata2[3] = vdata[2]; 
		vdata2[4] = vdata[3]; 
		vdata2[5] = vdata[4]; 
		vdata2[6] = vdata[5]; 
		vdata2[7] = vdata[6]; 
		vdata2[0] = vdata[7]; 
	}
	return;
}

void acts_all::PROCESSP0_RearrangeLayoutEn(unsigned int s, bool en[VECTOR_SIZE], bool en2[VECTOR_SIZE]){
	unsigned int s_ = s;// % VECTOR_SIZE;
 if(s_==0){ 
		en2[0] = en[0]; 
		en2[1] = en[1]; 
		en2[2] = en[2]; 
		en2[3] = en[3]; 
		en2[4] = en[4]; 
		en2[5] = en[5]; 
		en2[6] = en[6]; 
		en2[7] = en[7]; 
	}
else if(s_==1){ 
		en2[7] = en[0]; 
		en2[0] = en[1]; 
		en2[1] = en[2]; 
		en2[2] = en[3]; 
		en2[3] = en[4]; 
		en2[4] = en[5]; 
		en2[5] = en[6]; 
		en2[6] = en[7]; 
	}
else if(s_==2){ 
		en2[6] = en[0]; 
		en2[7] = en[1]; 
		en2[0] = en[2]; 
		en2[1] = en[3]; 
		en2[2] = en[4]; 
		en2[3] = en[5]; 
		en2[4] = en[6]; 
		en2[5] = en[7]; 
	}
else if(s_==3){ 
		en2[5] = en[0]; 
		en2[6] = en[1]; 
		en2[7] = en[2]; 
		en2[0] = en[3]; 
		en2[1] = en[4]; 
		en2[2] = en[5]; 
		en2[3] = en[6]; 
		en2[4] = en[7]; 
	}
else if(s_==4){ 
		en2[4] = en[0]; 
		en2[5] = en[1]; 
		en2[6] = en[2]; 
		en2[7] = en[3]; 
		en2[0] = en[4]; 
		en2[1] = en[5]; 
		en2[2] = en[6]; 
		en2[3] = en[7]; 
	}
else if(s_==5){ 
		en2[3] = en[0]; 
		en2[4] = en[1]; 
		en2[5] = en[2]; 
		en2[6] = en[3]; 
		en2[7] = en[4]; 
		en2[0] = en[5]; 
		en2[1] = en[6]; 
		en2[2] = en[7]; 
	}
else if(s_==6){ 
		en2[2] = en[0]; 
		en2[3] = en[1]; 
		en2[4] = en[2]; 
		en2[5] = en[3]; 
		en2[6] = en[4]; 
		en2[7] = en[5]; 
		en2[0] = en[6]; 
		en2[1] = en[7]; 
	}
else { 
		en2[1] = en[0]; 
		en2[2] = en[1]; 
		en2[3] = en[2]; 
		en2[4] = en[3]; 
		en2[5] = en[4]; 
		en2[6] = en[5]; 
		en2[7] = en[6]; 
		en2[0] = en[7]; 
	}
	return;
}

parsededge_t acts_all::PROCESSP0_PARSEEDGE(uint32_type data){ 
	parsededge_t parsededge;
	parsededge.incr = UTILP0_READFROM_UINT(data, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA);
	parsededge.dstvid = UTILP0_READFROM_UINT(data, OFFSETOF_DSTV_IN_EDGEDSTVDATA, SIZEOF_DSTV_IN_EDGEDSTVDATA);
	return parsededge; 
}

void acts_all::PROCESSP0_calculateoffsets(keyvalue_capsule_t * buffer, unsigned int size){
	buffer[0].key = 0;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2	
		buffer[i].key = buffer[i-1].key + buffer[i-1].value; 
	}
	return;
}

void acts_all::PROCESSP0_readedgeblockstats(keyvalue_t tempbuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], buffer_type chunk_size, globalparams_t globalparams){
	if(globalparams.ACTSPARAMS_READOFFLINESTATS == 1){
		unsigned int sum_values = 0;
		for(unsigned int p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
			localcapsule[p].key = 0; 
			if(p % 2 == 0){ localcapsule[p].value = tempbuffer[p/2][0].key; } else { localcapsule[p].value = tempbuffer[p/2][0].value; }
			sum_values += localcapsule[p].value;
		} sum_values -= 8888888;
		localcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].value = (chunk_size * VECTOR_SIZE) - sum_values;
		if(tempbuffer[7][0].value != 8888888 || sum_values > chunk_size * VECTOR_SIZE){
			#if defined(_DEBUGMODE_CHECKS) && defined(CONFIG_INSERTSTATSMETADATAINEDGES) // CRITICAL FIXME.
			if(sum_values > chunk_size * VECTOR_SIZE && chunk_size > 0){ cout<<"processedges2: ERROR: sum_values("<<sum_values<<") > chunk_size("<<chunk_size<<") * VECTOR_SIZE. EXITING... "<<endl; actsutilityobj->printkeyvalues("processandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS); exit(EXIT_FAILURE); } 
			if(tempbuffer[7][0].value != 8888888){ cout<<"processedges2: ERROR: tempbuffer[7][0].value != 8888888. EXITING... "<<endl; for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"tempbuffer["<<v<<"][0]: "<<tempbuffer[v][0]<<endl; } exit(EXIT_FAILURE); }
			#endif 
			unsigned int modelsz = chunk_size / globalparams.ACTSPARAMS_NUM_PARTITIONS; // mock it
			for(unsigned int i=0; i<globalparams.ACTSPARAMS_NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR_SIZE; localcapsule[i].value = modelsz * VECTOR_SIZE; } 
		}
	} else {
		unsigned int modelsz = chunk_size / globalparams.ACTSPARAMS_NUM_PARTITIONS;
		for(unsigned int i=0; i<globalparams.ACTSPARAMS_NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR_SIZE; localcapsule[i].value = modelsz * VECTOR_SIZE; } 
	}
	PROCESSP0_calculateoffsets(localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#ifdef _DEBUGMODE_KERNELPRINTS // DEBUGME_PROCESSEDGES2
	actsutilityobj->printkeyvalues("processedges2(14).localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS); 
	cout<<"processedges2(15): "<<"chunk_size * VECTOR_SIZE: "<<chunk_size * VECTOR_SIZE<<", globalparams.ACTSPARAMS_WORKBUFFER_SIZE * VECTOR_SIZE: "<<(globalparams.ACTSPARAMS_WORKBUFFER_SIZE * VECTOR_SIZE)<<endl;
	#endif
}

fetchmessage_t acts_all::PROCESSP0_ACTSreadandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return fetchmessage; }
	
	keyvalue_t E[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	keyvalue_t E2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E2 complete

	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 1;
	mytravstate.end_kvs = travstate.end_kvs / 1;
	
	loffset_kvs = loffset_kvs / 1; //
	buffer_type edgessize_kvs = size_kvs / 1;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	value_t res[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	unsigned int loadcount[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=loadcount complete
	unsigned int depths[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int d_kvs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=d_kvs complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	bool enx2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx2 complete

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	keyvalue_t edata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edata complete
	keyvalue_t tempbuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ loadcount[t] = 0; }
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILP0_GETV(INVALIDDATA);
	unsigned int bufferoffset_kvs = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	for (buffer_type i=0; i<globalparams.SIZEKVS2_REDUCEPARTITION; i++){
		if(MEMCAP0_READVMASK(vbuffer[0][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 0"<<", vbuffer[0]["<<i<<"]: "<<vbuffer[0][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[1][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 1"<<", vbuffer[1]["<<i<<"]: "<<vbuffer[1][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[2][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 2"<<", vbuffer[2]["<<i<<"]: "<<vbuffer[2][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[3][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 3"<<", vbuffer[3]["<<i<<"]: "<<vbuffer[3][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[4][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 4"<<", vbuffer[4]["<<i<<"]: "<<vbuffer[4][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[5][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 5"<<", vbuffer[5]["<<i<<"]: "<<vbuffer[5][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[6][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 6"<<", vbuffer[6]["<<i<<"]: "<<vbuffer[6][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[7][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 7"<<", vbuffer[7]["<<i<<"]: "<<vbuffer[7][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[8][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 8"<<", vbuffer[8]["<<i<<"]: "<<vbuffer[8][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[9][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 9"<<", vbuffer[9]["<<i<<"]: "<<vbuffer[9][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[10][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 10"<<", vbuffer[10]["<<i<<"]: "<<vbuffer[10][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[11][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 11"<<", vbuffer[11]["<<i<<"]: "<<vbuffer[11][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[12][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 12"<<", vbuffer[12]["<<i<<"]: "<<vbuffer[12][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[13][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 13"<<", vbuffer[13]["<<i<<"]: "<<vbuffer[13][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[14][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 14"<<", vbuffer[14]["<<i<<"]: "<<vbuffer[14][i]<<endl; }
		if(MEMCAP0_READVMASK(vbuffer[15][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 15"<<", vbuffer[15]["<<i<<"]: "<<vbuffer[15][i]<<endl; }
	}
	#endif
	
	// read edge block
	#ifdef DEBUGME_PROCESSEDGES2
	cout<<"processedges2: FIRST: offset_kvs: "<<offset_kvs<<", loffset_kvs: "<<loffset_kvs<<", goffset_kvs: "<<goffset_kvs<<", edgessize_kvs: "<<edgessize_kvs<<", mytravstate.i_kvs: "<<mytravstate.i_kvs<<", mytravstate.end_kvs: "<<mytravstate.end_kvs<<endl;
	#endif 
	PROCESSBUFFERPARTITIONS_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=1	
		#ifdef _WIDEWORD
		tempbuffer[0][i].key = edges[offset_kvs + i].range(31, 0);
		tempbuffer[0][i].value = edges[offset_kvs + i].range(63, 32);
		tempbuffer[1][i].key = edges[offset_kvs + i].range(95, 64);
		tempbuffer[1][i].value = edges[offset_kvs + i].range(127, 96);
		tempbuffer[2][i].key = edges[offset_kvs + i].range(159, 128);
		tempbuffer[2][i].value = edges[offset_kvs + i].range(191, 160);
		tempbuffer[3][i].key = edges[offset_kvs + i].range(223, 192);
		tempbuffer[3][i].value = edges[offset_kvs + i].range(255, 224);
		tempbuffer[4][i].key = edges[offset_kvs + i].range(287, 256);
		tempbuffer[4][i].value = edges[offset_kvs + i].range(319, 288);
		tempbuffer[5][i].key = edges[offset_kvs + i].range(351, 320);
		tempbuffer[5][i].value = edges[offset_kvs + i].range(383, 352);
		tempbuffer[6][i].key = edges[offset_kvs + i].range(415, 384);
		tempbuffer[6][i].value = edges[offset_kvs + i].range(447, 416);
		tempbuffer[7][i].key = edges[offset_kvs + i].range(479, 448);
		tempbuffer[7][i].value = edges[offset_kvs + i].range(511, 480);
		#else 
		tempbuffer[0][i].key = edges[offset_kvs + i].data[0].key; 
		tempbuffer[0][i].value = edges[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = edges[offset_kvs + i].data[1].key; 
		tempbuffer[1][i].value = edges[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = edges[offset_kvs + i].data[2].key; 
		tempbuffer[2][i].value = edges[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = edges[offset_kvs + i].data[3].key; 
		tempbuffer[3][i].value = edges[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = edges[offset_kvs + i].data[4].key; 
		tempbuffer[4][i].value = edges[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = edges[offset_kvs + i].data[5].key; 
		tempbuffer[5][i].value = edges[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = edges[offset_kvs + i].data[6].key; 
		tempbuffer[6][i].value = edges[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = edges[offset_kvs + i].data[7].key; 
		tempbuffer[7][i].value = edges[offset_kvs + i].data[7].value; 
		#endif 
		
		#ifdef DEBUGME_PROCESSEDGES
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[0].key: "<<edges[offset_kvs + i].data[0].key<<", edges["<<offset_kvs + i<<"].data[0].value: "<<edges[offset_kvs + i].data[0].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[1].key: "<<edges[offset_kvs + i].data[1].key<<", edges["<<offset_kvs + i<<"].data[1].value: "<<edges[offset_kvs + i].data[1].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[2].key: "<<edges[offset_kvs + i].data[2].key<<", edges["<<offset_kvs + i<<"].data[2].value: "<<edges[offset_kvs + i].data[2].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[3].key: "<<edges[offset_kvs + i].data[3].key<<", edges["<<offset_kvs + i<<"].data[3].value: "<<edges[offset_kvs + i].data[3].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[4].key: "<<edges[offset_kvs + i].data[4].key<<", edges["<<offset_kvs + i<<"].data[4].value: "<<edges[offset_kvs + i].data[4].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[5].key: "<<edges[offset_kvs + i].data[5].key<<", edges["<<offset_kvs + i<<"].data[5].value: "<<edges[offset_kvs + i].data[5].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[6].key: "<<edges[offset_kvs + i].data[6].key<<", edges["<<offset_kvs + i<<"].data[6].value: "<<edges[offset_kvs + i].data[6].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[7].key: "<<edges[offset_kvs + i].data[7].key<<", edges["<<offset_kvs + i<<"].data[7].value: "<<edges[offset_kvs + i].data[7].value<<endl; }
		#endif 
	}
	
	// read edge block stats  
	PROCESSP0_readedgeblockstats(tempbuffer, localcapsule, chunk_size, globalparams);
	
	// process edge block stats 
	maxsize_kvs[0] = 0; maxsize_kvs[1] = 0;
	unsigned int _poff = 0;
	PROCESSBUFFERPARTITIONS_LOOP3: for(unsigned int cid=0; cid<2; cid++){
		PROCESSBUFFERPARTITIONS_LOOP3B: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=2
			unsigned int ssize_kvs = localcapsule[_poff + p].value / VECTOR_SIZE;
			if(maxsize_kvs[cid] < ssize_kvs){ maxsize_kvs[cid] = ssize_kvs; }
		}
		_poff += globalparams.ACTSPARAMS_NUM_PARTITIONS/2;
	}
	height_kvs = (localcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].key + localcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].value) / VECTOR_SIZE;

	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE;
	}
	
	// process edge block
	unsigned int readoffset = 0; if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ readoffset = 1; }
	PROCESSBUFFERPARTITIONS_MAINLOOP4: for(buffer_type capsule_offset=0; capsule_offset<globalparams.ACTSPARAMS_NUM_PARTITIONS; capsule_offset+=VECTOR_SIZE){ // processing next capsule set
		unsigned int mmaxsz_kvs = maxsize_kvs[capsule_offset / VECTOR_SIZE];
		PROCESSBUFFERPARTITIONS_MAINLOOP4C: for(unsigned int r=0; r<VECTOR_SIZE; r++){ // interchanging columns
			PROCESSBUFFERPARTITIONS_MAINLOOP4D: for(buffer_type i=0; i<mmaxsz_kvs; i++){ // processing a column data
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				
				// get layout
				PROCESSP0_GetXYLayoutV(r, depths, 0);
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[0], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[0], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[1], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[1], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[2], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[2], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[3], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[3], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[4], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[4], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[5], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[5], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[6], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[6], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[7], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[7], NAp);
 
				#endif
				
				// calculate enable
				enx[0] = true;	
				enx[1] = true;	
				enx[2] = true;	
				enx[3] = true;	
				enx[4] = true;	
				enx[5] = true;	
				enx[6] = true;	
				enx[7] = true;	
				
				// read 
				unsigned int tdepth0 = capsule_offset + depths[0];	
				d_kvs[0] = readoffset + localcapsule_kvs[tdepth0].key + i; 
				unsigned int tdepth1 = capsule_offset + depths[1];	
				d_kvs[1] = readoffset + localcapsule_kvs[tdepth1].key + i; 
				unsigned int tdepth2 = capsule_offset + depths[2];	
				d_kvs[2] = readoffset + localcapsule_kvs[tdepth2].key + i; 
				unsigned int tdepth3 = capsule_offset + depths[3];	
				d_kvs[3] = readoffset + localcapsule_kvs[tdepth3].key + i; 
				unsigned int tdepth4 = capsule_offset + depths[4];	
				d_kvs[4] = readoffset + localcapsule_kvs[tdepth4].key + i; 
				unsigned int tdepth5 = capsule_offset + depths[5];	
				d_kvs[5] = readoffset + localcapsule_kvs[tdepth5].key + i; 
				unsigned int tdepth6 = capsule_offset + depths[6];	
				d_kvs[6] = readoffset + localcapsule_kvs[tdepth6].key + i; 
				unsigned int tdepth7 = capsule_offset + depths[7];	
				d_kvs[7] = readoffset + localcapsule_kvs[tdepth7].key + i; 
 	
 // FIXME.
				if(d_kvs[0] < height_kvs && i < localcapsule_kvs[tdepth0].value){ E[0] = tempbuffer[0][d_kvs[0]]; } else { enx[0] = false; E[0].key = INVALIDDATA; }	
 // FIXME.
				if(d_kvs[1] < height_kvs && i < localcapsule_kvs[tdepth1].value){ E[1] = tempbuffer[1][d_kvs[1]]; } else { enx[1] = false; E[1].key = INVALIDDATA; }	
 // FIXME.
				if(d_kvs[2] < height_kvs && i < localcapsule_kvs[tdepth2].value){ E[2] = tempbuffer[2][d_kvs[2]]; } else { enx[2] = false; E[2].key = INVALIDDATA; }	
 // FIXME.
				if(d_kvs[3] < height_kvs && i < localcapsule_kvs[tdepth3].value){ E[3] = tempbuffer[3][d_kvs[3]]; } else { enx[3] = false; E[3].key = INVALIDDATA; }	
 // FIXME.
				if(d_kvs[4] < height_kvs && i < localcapsule_kvs[tdepth4].value){ E[4] = tempbuffer[4][d_kvs[4]]; } else { enx[4] = false; E[4].key = INVALIDDATA; }	
 // FIXME.
				if(d_kvs[5] < height_kvs && i < localcapsule_kvs[tdepth5].value){ E[5] = tempbuffer[5][d_kvs[5]]; } else { enx[5] = false; E[5].key = INVALIDDATA; }	
 // FIXME.
				if(d_kvs[6] < height_kvs && i < localcapsule_kvs[tdepth6].value){ E[6] = tempbuffer[6][d_kvs[6]]; } else { enx[6] = false; E[6].key = INVALIDDATA; }	
 // FIXME.
				if(d_kvs[7] < height_kvs && i < localcapsule_kvs[tdepth7].value){ E[7] = tempbuffer[7][d_kvs[7]]; } else { enx[7] = false; E[7].key = INVALIDDATA; }	
 	
				
				// for BFS implementation
				collections[0][2].data1 += VECTOR_SIZE;
				if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){  
					if(collections[0][2].data1 >= globalposition.num_active_edges_in_channel){  enx[0] = false;  enx[1] = false;  enx[2] = false;  enx[3] = false;  enx[4] = false;  enx[5] = false;  enx[6] = false;  enx[7] = false;  }
				}
				
				// process
				PROCESSP0_processvector(enx[0], 0, E[0].value, E[0], vbuffer[capsule_offset + 0], buffer[0], bufferoffset_kvs, &loadcount[0], GraphAlgoClass, globalposition, globalparams);
				PROCESSP0_processvector(enx[1], 1, E[1].value, E[1], vbuffer[capsule_offset + 1], buffer[1], bufferoffset_kvs, &loadcount[1], GraphAlgoClass, globalposition, globalparams);
				PROCESSP0_processvector(enx[2], 2, E[2].value, E[2], vbuffer[capsule_offset + 2], buffer[2], bufferoffset_kvs, &loadcount[2], GraphAlgoClass, globalposition, globalparams);
				PROCESSP0_processvector(enx[3], 3, E[3].value, E[3], vbuffer[capsule_offset + 3], buffer[3], bufferoffset_kvs, &loadcount[3], GraphAlgoClass, globalposition, globalparams);
				PROCESSP0_processvector(enx[4], 4, E[4].value, E[4], vbuffer[capsule_offset + 4], buffer[4], bufferoffset_kvs, &loadcount[4], GraphAlgoClass, globalposition, globalparams);
				PROCESSP0_processvector(enx[5], 5, E[5].value, E[5], vbuffer[capsule_offset + 5], buffer[5], bufferoffset_kvs, &loadcount[5], GraphAlgoClass, globalposition, globalparams);
				PROCESSP0_processvector(enx[6], 6, E[6].value, E[6], vbuffer[capsule_offset + 6], buffer[6], bufferoffset_kvs, &loadcount[6], GraphAlgoClass, globalposition, globalparams);
				PROCESSP0_processvector(enx[7], 7, E[7].value, E[7], vbuffer[capsule_offset + 7], buffer[7], bufferoffset_kvs, &loadcount[7], GraphAlgoClass, globalposition, globalparams);
	
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[0], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[1], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[2], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[3], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[4], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[5], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[6], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[7], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
 
				#endif
			}
		}
	}
	
	// actsutilityobj->printkeyvalues("ACTSreadandprocess.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS); 
	// for(unsigned int t=0; t<VECTOR_SIZE; t++){ cout<<"ACTSreadandprocess: loadcount["<<t<<"]: "<<loadcount[t]<<endl; }
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ for(unsigned int t=0; t<4; t++){ cout<<"processedges:: buffer["<<v<<"]["<<t<<"].key: "<<UTILP0_GETKV(buffer[v][t]).key<<", buffer["<<v<<"]["<<t<<"].value: "<<UTILP0_GETKV(buffer[v][t]).value<<endl; }}
	unsigned int maxsz_kvs = 0; for(unsigned int t=0; t<VECTOR_SIZE; t++){ if(loadcount[t] > maxsz_kvs){ maxsz_kvs = loadcount[t]; }}
	fetchmessage.chunksize_kvs = maxsz_kvs;//chunk_size * 2; // loadcount; // CRITICAL FIXME
	// cout<<"--- processedges: chunk_size: "<<chunk_size<<", fetchmessage.chunksize_kvs: "<<fetchmessage.chunksize_kvs<<endl;
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS);
	return fetchmessage;
}

fetchmessage_t acts_all::PROCESSP0_ACTS2readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return fetchmessage; }
	
	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 1;
	mytravstate.end_kvs = travstate.end_kvs / 1;
	
	loffset_kvs = loffset_kvs / 1; //
	buffer_type edgessize_kvs = size_kvs / 1;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	unsigned int loadcount[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=loadcount complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	keyvalue_t edges_reg[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edges_reg complete

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	buffer_type chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ loadcount[t] = 0; }

	PROCESSBUFFERPARTITIONS_MAINLOOP1: for(buffer_type i=0; i<chunk_size; i++){ // processing a column data
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		
		// decision flag
 enx[0] = true;  enx[1] = true;  enx[2] = true;  enx[3] = true;  enx[4] = true;  enx[5] = true;  enx[6] = true;  enx[7] = true; 		
		// get dataset
		UTILP0_GetDataset(edges, offset_kvs + i, edges_reg);
		
		// for BFS implementation
		if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && collections[0][2].data1 >= globalposition.num_active_edges_in_channel){  enx[0] = false;  enx[1] = false;  enx[2] = false;  enx[3] = false;  enx[4] = false;  enx[5] = false;  enx[6] = false;  enx[7] = false;  }
		else { collections[0][2].data1 += VECTOR_SIZE; }
		
		// process
		PROCESSP0_processvector(enx[0], 0, edges_reg[0].value, edges_reg[0], vbuffer[0], buffer[0], 0, &loadcount[0], GraphAlgoClass, globalposition, globalparams);
		PROCESSP0_processvector(enx[1], 1, edges_reg[1].value, edges_reg[1], vbuffer[1], buffer[1], 0, &loadcount[1], GraphAlgoClass, globalposition, globalparams);
		PROCESSP0_processvector(enx[2], 2, edges_reg[2].value, edges_reg[2], vbuffer[2], buffer[2], 0, &loadcount[2], GraphAlgoClass, globalposition, globalparams);
		PROCESSP0_processvector(enx[3], 3, edges_reg[3].value, edges_reg[3], vbuffer[3], buffer[3], 0, &loadcount[3], GraphAlgoClass, globalposition, globalparams);
		PROCESSP0_processvector(enx[4], 4, edges_reg[4].value, edges_reg[4], vbuffer[4], buffer[4], 0, &loadcount[4], GraphAlgoClass, globalposition, globalparams);
		PROCESSP0_processvector(enx[5], 5, edges_reg[5].value, edges_reg[5], vbuffer[5], buffer[5], 0, &loadcount[5], GraphAlgoClass, globalposition, globalparams);
		PROCESSP0_processvector(enx[6], 6, edges_reg[6].value, edges_reg[6], vbuffer[6], buffer[6], 0, &loadcount[6], GraphAlgoClass, globalposition, globalparams);
		PROCESSP0_processvector(enx[7], 7, edges_reg[7].value, edges_reg[7], vbuffer[7], buffer[7], 0, &loadcount[7], GraphAlgoClass, globalposition, globalparams);
	
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[0], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[1], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[2], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[3], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[4], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[5], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[6], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[7], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
 
		#endif
	}
	
	// cout<<"_ACTSreadandprocess_presorted: chunk_size: "<<chunk_size<<", collections[0][2].data1: "<<collections[0][2].data1<<", globalposition.num_active_edges_in_channel: "<<globalposition.num_active_edges_in_channel<<endl;
	// for(unsigned int t=0; t<VECTOR_SIZE; t++){ cout<<"_ACTSreadandprocess_presorted: loadcount["<<t<<"]: "<<loadcount[t]<<endl; }
	// for(unsigned int v=0; v<VECTOR_SIZE; v++){ for(unsigned int t=0; t<4; t++){ cout<<"processedges:: buffer["<<v<<"]["<<t<<"].key: "<<UTILP0_GETKV(buffer[v][t]).key<<", buffer["<<v<<"]["<<t<<"].value: "<<UTILP0_GETKV(buffer[v][t]).value<<endl; }}
	unsigned int maxsz_kvs = 0; for(unsigned int t=0; t<VECTOR_SIZE; t++){ if(loadcount[t] > maxsz_kvs){ maxsz_kvs = loadcount[t]; }}
	fetchmessage.chunksize_kvs = maxsz_kvs;//chunk_size * 2; // loadcount; // CRITICAL FIXME
	// cout<<"_ACTSreadandprocess_presorted: chunk_size: "<<chunk_size<<", fetchmessage.chunksize_kvs: "<<fetchmessage.chunksize_kvs<<endl;
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS);
	return fetchmessage;
}

fetchmessage_t acts_all::PROCESSP0_TRADreadandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ACTSP0_actit: Hybrid Check: num_active_vertices("<<globalposition.num_active_vertices<<") < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION("<<globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION<<"). USING TRADITIONAL GP INSTEAD.... !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl; 
		#endif 
		return fetchmessage; } // FIXXXXXXXXMMMMMMMMMMMEEEEEEEEEEEEEEEEEEEEEEE.						
	
	uint32_type E[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	uint32_type E2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E2 complete

	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 1;
	mytravstate.end_kvs = travstate.end_kvs / 1;
	
	loffset_kvs = loffset_kvs / 1; //
	buffer_type edgessize_kvs = size_kvs / 1;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	unsigned int loadcount;

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	keyvalue_t tempbuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	loadcount = 0;
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILP0_GETV(INVALIDDATA);
	unsigned int bufferoffset_kvs = 0;
	
	// read edge block
	#ifdef DEBUGME_PROCESSEDGES2
	cout<<"processedges2: FIRST: offset_kvs: "<<offset_kvs<<", loffset_kvs: "<<loffset_kvs<<", goffset_kvs: "<<goffset_kvs<<", edgessize_kvs: "<<edgessize_kvs<<", mytravstate.i_kvs: "<<mytravstate.i_kvs<<", mytravstate.end_kvs: "<<mytravstate.end_kvs<<endl;
	#endif 
	PROCESSBUFFERPARTITIONS_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=1	
		#ifdef _WIDEWORD
		tempbuffer[0][i].key = edges[offset_kvs + i].range(31, 0);
		tempbuffer[0][i].value = edges[offset_kvs + i].range(63, 32);
		tempbuffer[1][i].key = edges[offset_kvs + i].range(95, 64);
		tempbuffer[1][i].value = edges[offset_kvs + i].range(127, 96);
		tempbuffer[2][i].key = edges[offset_kvs + i].range(159, 128);
		tempbuffer[2][i].value = edges[offset_kvs + i].range(191, 160);
		tempbuffer[3][i].key = edges[offset_kvs + i].range(223, 192);
		tempbuffer[3][i].value = edges[offset_kvs + i].range(255, 224);
		tempbuffer[4][i].key = edges[offset_kvs + i].range(287, 256);
		tempbuffer[4][i].value = edges[offset_kvs + i].range(319, 288);
		tempbuffer[5][i].key = edges[offset_kvs + i].range(351, 320);
		tempbuffer[5][i].value = edges[offset_kvs + i].range(383, 352);
		tempbuffer[6][i].key = edges[offset_kvs + i].range(415, 384);
		tempbuffer[6][i].value = edges[offset_kvs + i].range(447, 416);
		tempbuffer[7][i].key = edges[offset_kvs + i].range(479, 448);
		tempbuffer[7][i].value = edges[offset_kvs + i].range(511, 480);
		#else 
		tempbuffer[0][i].key = edges[offset_kvs + i].data[0].key; 
		tempbuffer[0][i].value = edges[offset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = edges[offset_kvs + i].data[1].key; 
		tempbuffer[1][i].value = edges[offset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = edges[offset_kvs + i].data[2].key; 
		tempbuffer[2][i].value = edges[offset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = edges[offset_kvs + i].data[3].key; 
		tempbuffer[3][i].value = edges[offset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = edges[offset_kvs + i].data[4].key; 
		tempbuffer[4][i].value = edges[offset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = edges[offset_kvs + i].data[5].key; 
		tempbuffer[5][i].value = edges[offset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = edges[offset_kvs + i].data[6].key; 
		tempbuffer[6][i].value = edges[offset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = edges[offset_kvs + i].data[7].key; 
		tempbuffer[7][i].value = edges[offset_kvs + i].data[7].value; 
		#endif 
		
		#ifdef DEBUGME_PROCESSEDGES
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[0].key: "<<edges[offset_kvs + i].data[0].key<<", edges["<<offset_kvs + i<<"].data[0].value: "<<edges[offset_kvs + i].data[0].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[1].key: "<<edges[offset_kvs + i].data[1].key<<", edges["<<offset_kvs + i<<"].data[1].value: "<<edges[offset_kvs + i].data[1].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[2].key: "<<edges[offset_kvs + i].data[2].key<<", edges["<<offset_kvs + i<<"].data[2].value: "<<edges[offset_kvs + i].data[2].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[3].key: "<<edges[offset_kvs + i].data[3].key<<", edges["<<offset_kvs + i<<"].data[3].value: "<<edges[offset_kvs + i].data[3].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[4].key: "<<edges[offset_kvs + i].data[4].key<<", edges["<<offset_kvs + i<<"].data[4].value: "<<edges[offset_kvs + i].data[4].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[5].key: "<<edges[offset_kvs + i].data[5].key<<", edges["<<offset_kvs + i<<"].data[5].value: "<<edges[offset_kvs + i].data[5].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[6].key: "<<edges[offset_kvs + i].data[6].key<<", edges["<<offset_kvs + i<<"].data[6].value: "<<edges[offset_kvs + i].data[6].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[7].key: "<<edges[offset_kvs + i].data[7].key<<", edges["<<offset_kvs + i<<"].data[7].value: "<<edges[offset_kvs + i].data[7].value<<endl; }
		#endif 
	}
	
	// read edge block stats  
	PROCESSP0_readedgeblockstats(tempbuffer, localcapsule, chunk_size, globalparams);

	unsigned int readoffset = 0; if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ readoffset = 1; }
	PROCESSBUFFERPARTITIONS_LOOP2A: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		PROCESSBUFFERPARTITIONS_LOOP2B: for(buffer_type i=localcapsule[p].key; i<localcapsule[p].key + localcapsule[p].value; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("TRADreadandprocess(14)::DEBUG CODE 12::1", readoffset + (i / VECTOR_SIZE), MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			keyvalue_t E = tempbuffer[i % VECTOR_SIZE][readoffset + (i / VECTOR_SIZE)];
			#ifdef _DEBUGMODE_CHECKS//3
			if(E.key==8888888 || E.value==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif 
			bool en = true; 
			
			// read 
			vmdata_t vmdata;
			unsigned int loc = E.value; //  - sweepparams.upperlimit;
			if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){
				#ifdef _DEBUGMODE_CHECKS
				if(true){ cout<<"TRADreadandprocess::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). E.key: "<<E.key<<", E.value: "<<E.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
				#endif 
				loc = 0; }
		
			keyvalue_vbuffer_t bits_vector = 0; 
			#ifdef _DEBUGMODE_CHECKS3
			if(en == true){ actsutilityobj->checkoutofbounds("TRADreadandprocess(14)::DEBUG CODE 113::1", bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VSRCDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
			#endif
			if(en == true){ bits_vector = vbuffer[p][bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO)]; }
			if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
							
			if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ loc: "<<loc<<", E.key: "<<E.key<<", E.value(srcvid): "<<E.value<<", vid: "<<UTILP0_GETREALVID(E.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
			#endif
					
			// process
			value_t res = PROCESSP0_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			keyvalue_t mykeyvalue; mykeyvalue.key = E.key; mykeyvalue.value = res;
			
			// write
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("TRADreadandprocess(14)::DEBUG CODE 14::1", loadcount, MAX_SOURCEBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			#endif
			if(en == true && vmdata.vmask == 1 && loadcount < ((globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2) * VECTOR_SIZE)){ buffer[loadcount % VECTOR_SIZE][loadcount / VECTOR_SIZE] = UTILP0_GETKV(mykeyvalue); }
			if(en == true && vmdata.vmask == 1 && loadcount < ((globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2) * VECTOR_SIZE)){ loadcount += 1; }
	
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1);
			if(en == true && vmdata.vmask == 1 && loadcount < ((globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2) * VECTOR_SIZE)){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1); } // mask0? FIXME.
			#endif
		}
	}

	fetchmessage.chunksize_kvs = (loadcount + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// exit(EXIT_SUCCESS);
	// cout<<">>> PROCESS VECTOR:: ^^^^^^^^^^ACTIVE VERTEX PROCESSED: SEEN: @ fetchmessage.chunksize_kvs: "<<fetchmessage.chunksize_kvs<<endl;
	// exit(EXIT_SUCCESS);
	return fetchmessage;
}

fetchmessage_t acts_all::PROCESSP0_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	// return PROCESSP0_ACTSreadandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
	return PROCESSP0_ACTS2readandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
	// return PROCESSP0_TRADreadandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
}







