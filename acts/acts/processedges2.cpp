void acts_all::PROCESSP0_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int bufferoffset_kvs, unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams	
		#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
		,collection_t collections[COLLECTIONS_BUFFERSZ]
		#endif 
		){				
	#pragma HLS INLINE
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS
		if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
	// read 
	vmdata_t vmdata;
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS				
		if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(loc, NAp, vbuffer, bufferoffset_kvs); } else { vmdata.vmask = 0; }
			#else 
				if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(loc, vbuffer, 0); } else { vmdata.vmask = 0; }
					#endif 
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", vid: "<<UTILP0_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
			
	// process
	value_t res = PROCESSP0_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	keyvalue_t mykeyvalue; mykeyvalue.key = edata.key; mykeyvalue.value = res;
	
	// write
	if(en == true && vmdata.vmask == 1 && *loadcount < WORKBUFFER_SIZE-2){ buffer[*loadcount] = UTILP0_GETKV(mykeyvalue); }
	if(en == true && vmdata.vmask == 1 && *loadcount < WORKBUFFER_SIZE-2){ *loadcount += 1; } // CRITICAL FIXME. WORKBUFFER_SIZE
	
	#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
	collections[0].data1 += 1;
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1);
	if(en == true && vmdata.vmask == 1 && *loadcount < WORKBUFFER_SIZE-2){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1); } // mask0? FIXME.
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("PROCESSP0_processvector::DEBUG CODE 14::1", *loadcount, SRCBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE, NAp); // SOURCEBLOCKRAM_SIZE
	#endif
	return;
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

void acts_all::PROCESSP0_calculateoffsets(keyvalue_capsule_t * buffer){
	buffer[0].key = 0;
	for(buffer_type i=1; i<NUM_PARTITIONS; i++){ 
	#pragma HLS PIPELINE II=2	
		buffer[i].key = buffer[i-1].key + buffer[i-1].value; 
	}
	return;
}

#ifdef CONFIG_ACTS_MEMORYLAYOUT
fetchmessage_t acts_all::PROCESSP0_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / (NUM_PARTITIONS / 2));
	
	uint32_type E[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	uint32_type E2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E2 complete

	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 2;
	mytravstate.end_kvs = travstate.end_kvs / 2;
	
	loffset_kvs = loffset_kvs / 2; //
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	value_t res[VECTOR2_SIZE];
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
	value_t tempbuffer[VECTOR2_SIZE][SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ loadcount[t] = 0; }
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILP0_GETV(INVALIDDATA);
	unsigned int bufferoffset_kvs = 0;
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	bufferoffset_kvs = (globalposition.source_partition % VDATA_SHRINK_RATIO) * (UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams) / VDATA_SHRINK_RATIO); //  ((BLOCKRAM_VDATA_SIZE / (NUM_PEs)) / 16) * NUM_PEs; // (3 * 24); // 0; BLOCKRAM_VDATA_SIZE
	#endif 
	
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
		tempbuffer[0][i] = edges[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = edges[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = edges[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = edges[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = edges[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = edges[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = edges[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = edges[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = edges[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = edges[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = edges[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = edges[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = edges[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = edges[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = edges[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = edges[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i] = edges[offset_kvs + i].data[0].key; 
		tempbuffer[1][i] = edges[offset_kvs + i].data[0].value;	
		tempbuffer[2][i] = edges[offset_kvs + i].data[1].key; 
		tempbuffer[3][i] = edges[offset_kvs + i].data[1].value;	
		tempbuffer[4][i] = edges[offset_kvs + i].data[2].key; 
		tempbuffer[5][i] = edges[offset_kvs + i].data[2].value;	
		tempbuffer[6][i] = edges[offset_kvs + i].data[3].key; 
		tempbuffer[7][i] = edges[offset_kvs + i].data[3].value;	
		tempbuffer[8][i] = edges[offset_kvs + i].data[4].key; 
		tempbuffer[9][i] = edges[offset_kvs + i].data[4].value;	
		tempbuffer[10][i] = edges[offset_kvs + i].data[5].key; 
		tempbuffer[11][i] = edges[offset_kvs + i].data[5].value;	
		tempbuffer[12][i] = edges[offset_kvs + i].data[6].key; 
		tempbuffer[13][i] = edges[offset_kvs + i].data[6].value;	
		tempbuffer[14][i] = edges[offset_kvs + i].data[7].key; 
		tempbuffer[15][i] = edges[offset_kvs + i].data[7].value;	
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
	// bool statsiscorrect = true;
	#ifdef CONFIG_PROCESSEDGES_READOFFLINESTATS
	unsigned int sum_values = 0;
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ localcapsule[p].key = 0; localcapsule[p].value = tempbuffer[p][0]; if(p<NUM_PARTITIONS-1){ sum_values += tempbuffer[p][0]; }}
	localcapsule[NUM_PARTITIONS-1].value = (chunk_size * VECTOR2_SIZE) - sum_values;
	if(tempbuffer[NUM_PARTITIONS-1][0] != 8888888 || sum_values > chunk_size * VECTOR2_SIZE){
		#if defined(_DEBUGMODE_CHECKS) && defined(CONFIG_INSERTSTATSMETADATAINEDGES) // CRITICAL FIXME.
		if(sum_values > chunk_size * VECTOR2_SIZE && chunk_size > 0){ cout<<"processedges2: ERROR: sum_values("<<sum_values<<") > chunk_size("<<chunk_size<<") * VECTOR2_SIZE. EXITING... "<<endl; actsutilityobj->printkeyvalues("processandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS); exit(EXIT_FAILURE); } 
		if(tempbuffer[NUM_PARTITIONS-1][0] != 8888888){ cout<<"processedges2: ERROR: tempbuffer[NUM_PARTITIONS-1][0] != 8888888. EXITING... "<<endl; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<"tempbuffer["<<v<<"][0]: "<<tempbuffer[v][0]<<endl; } exit(EXIT_FAILURE); }
		#endif 
		unsigned int modelsz = chunk_size / NUM_PARTITIONS; // mock it
		for(unsigned int i=0; i<NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } 
	}
	#else 
	unsigned int modelsz = chunk_size / NUM_PARTITIONS;
	for(unsigned int i=0; i<NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } 
	#endif 
	PROCESSP0_calculateoffsets(localcapsule);
	#ifdef DEBUGME_PROCESSEDGES2
	actsutilityobj->printkeyvalues("processedges2(14).localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS); 
	cout<<"processedges2(15): "<<"chunk_size * VECTOR2_SIZE: "<<chunk_size * VECTOR2_SIZE<<", edgessize_kvs * VECTOR2_SIZE: "<<edgessize_kvs * VECTOR2_SIZE<<", WORKBUFFER_SIZE * VECTOR2_SIZE: "<<(WORKBUFFER_SIZE * VECTOR2_SIZE)<<endl;
	#endif
	
	// process edge block stats 
	maxsize_kvs[0] = 0; maxsize_kvs[1] = 0;
	unsigned int _poff = 0;
	PROCESSBUFFERPARTITIONS_LOOP3: for(unsigned int cid=0; cid<2; cid++){
		PROCESSBUFFERPARTITIONS_LOOP3B: for(partition_type p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=2
			unsigned int ssize_kvs = localcapsule[_poff + p].value / VECTOR2_SIZE;
			if(maxsize_kvs[cid] < ssize_kvs){ maxsize_kvs[cid] = ssize_kvs; }
		}
		_poff += NUM_PARTITIONS/2;
	}
	height_kvs = (localcapsule[NUM_PARTITIONS-1].key + localcapsule[NUM_PARTITIONS-1].value) / VECTOR2_SIZE;

	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR2_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR2_SIZE-1)) / VECTOR2_SIZE;
	}
	
	// process edge block
	unsigned int readoffset = 0; if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ readoffset = 1; }
	PROCESSBUFFERPARTITIONS_LOOP4: for(buffer_type capsule_offset=0; capsule_offset<NUM_PARTITIONS; capsule_offset+=VECTOR_SIZE){ // processing next capsule set
		unsigned int mmaxsz_kvs = maxsize_kvs[capsule_offset / VECTOR_SIZE];
		PROCESSBUFFERPARTITIONS_LOOP4B: for(buffer_type block_offset=0; block_offset<VECTOR2_SIZE; block_offset+=VECTOR_SIZE){ // processing next block set 
			PROCESSBUFFERPARTITIONS_LOOP4C: for(unsigned int r=0; r<VECTOR_SIZE; r++){ // interchanging columns
				PROCESSBUFFERPARTITIONS_LOOP4D: for(buffer_type i=0; i<mmaxsz_kvs; i++){ // processing a column data
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					#endif
					
					// get layout
					PROCESSP0_GetXYLayoutV(r, depths, 0);
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[0], NUM_PARTITIONS, capsule_offset, depths[0], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[1], NUM_PARTITIONS, capsule_offset, depths[1], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[2], NUM_PARTITIONS, capsule_offset, depths[2], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[3], NUM_PARTITIONS, capsule_offset, depths[3], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[4], NUM_PARTITIONS, capsule_offset, depths[4], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[5], NUM_PARTITIONS, capsule_offset, depths[5], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[6], NUM_PARTITIONS, capsule_offset, depths[6], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[7], NUM_PARTITIONS, capsule_offset, depths[7], NAp);
 
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
					if(d_kvs[0] < height_kvs && i < localcapsule_kvs[tdepth0].value){ E[0] = tempbuffer[block_offset + 0][d_kvs[0]]; } else { enx[0] = false; E[0] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[1] < height_kvs && i < localcapsule_kvs[tdepth1].value){ E[1] = tempbuffer[block_offset + 1][d_kvs[1]]; } else { enx[1] = false; E[1] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[2] < height_kvs && i < localcapsule_kvs[tdepth2].value){ E[2] = tempbuffer[block_offset + 2][d_kvs[2]]; } else { enx[2] = false; E[2] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[3] < height_kvs && i < localcapsule_kvs[tdepth3].value){ E[3] = tempbuffer[block_offset + 3][d_kvs[3]]; } else { enx[3] = false; E[3] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[4] < height_kvs && i < localcapsule_kvs[tdepth4].value){ E[4] = tempbuffer[block_offset + 4][d_kvs[4]]; } else { enx[4] = false; E[4] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[5] < height_kvs && i < localcapsule_kvs[tdepth5].value){ E[5] = tempbuffer[block_offset + 5][d_kvs[5]]; } else { enx[5] = false; E[5] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[6] < height_kvs && i < localcapsule_kvs[tdepth6].value){ E[6] = tempbuffer[block_offset + 6][d_kvs[6]]; } else { enx[6] = false; E[6] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[7] < height_kvs && i < localcapsule_kvs[tdepth7].value){ E[7] = tempbuffer[block_offset + 7][d_kvs[7]]; } else { enx[7] = false; E[7] = INVALIDDATA; }	
 	
					
					// re-arrange 
					PROCESSP0_RearrangeLayoutV(r, E, E2);
					PROCESSP0_RearrangeLayoutEn(r, enx, enx2); // NEWCHANGE.
 if(E2[0]==8888888){ E2[0] = 0; }  if(E2[1]==8888888){ E2[1] = 0; }  if(E2[2]==8888888){ E2[2] = 0; }  if(E2[3]==8888888){ E2[3] = 0; }  if(E2[4]==8888888){ E2[4] = 0; }  if(E2[5]==8888888){ E2[5] = 0; }  if(E2[6]==8888888){ E2[6] = 0; }  if(E2[7]==8888888){ E2[7] = 0; } 	
					#ifdef _DEBUGMODE_CHECKS3
					if(E2[0]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[1]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[2]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[3]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[4]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[5]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[6]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[7]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					#endif 
					
					// parse
					parsededge_t parsed_edge0 = PROCESSP0_PARSEEDGE(E2[0]); // FIXME.
					edata[0].value = parsed_edge0.incr; // source info
					edata[0].key = parsed_edge0.dstvid;	
					parsededge_t parsed_edge1 = PROCESSP0_PARSEEDGE(E2[1]); // FIXME.
					edata[1].value = parsed_edge1.incr; // source info
					edata[1].key = parsed_edge1.dstvid;	
					parsededge_t parsed_edge2 = PROCESSP0_PARSEEDGE(E2[2]); // FIXME.
					edata[2].value = parsed_edge2.incr; // source info
					edata[2].key = parsed_edge2.dstvid;	
					parsededge_t parsed_edge3 = PROCESSP0_PARSEEDGE(E2[3]); // FIXME.
					edata[3].value = parsed_edge3.incr; // source info
					edata[3].key = parsed_edge3.dstvid;	
					parsededge_t parsed_edge4 = PROCESSP0_PARSEEDGE(E2[4]); // FIXME.
					edata[4].value = parsed_edge4.incr; // source info
					edata[4].key = parsed_edge4.dstvid;	
					parsededge_t parsed_edge5 = PROCESSP0_PARSEEDGE(E2[5]); // FIXME.
					edata[5].value = parsed_edge5.incr; // source info
					edata[5].key = parsed_edge5.dstvid;	
					parsededge_t parsed_edge6 = PROCESSP0_PARSEEDGE(E2[6]); // FIXME.
					edata[6].value = parsed_edge6.incr; // source info
					edata[6].key = parsed_edge6.dstvid;	
					parsededge_t parsed_edge7 = PROCESSP0_PARSEEDGE(E2[7]); // FIXME.
					edata[7].value = parsed_edge7.incr; // source info
					edata[7].key = parsed_edge7.dstvid;	
					#ifdef _DEBUGMODE_KERNELPRINTS
					if(edata[0].value==42 && enx[0] == true){ cout<<"processedges2: edata[0].value("<<edata[0].value<<")==42. edata[0].key: "<<edata[0].key<<endl; }
					if(edata[1].value==42 && enx[1] == true){ cout<<"processedges2: edata[1].value("<<edata[1].value<<")==42. edata[1].key: "<<edata[1].key<<endl; }
					if(edata[2].value==42 && enx[2] == true){ cout<<"processedges2: edata[2].value("<<edata[2].value<<")==42. edata[2].key: "<<edata[2].key<<endl; }
					if(edata[3].value==42 && enx[3] == true){ cout<<"processedges2: edata[3].value("<<edata[3].value<<")==42. edata[3].key: "<<edata[3].key<<endl; }
					if(edata[4].value==42 && enx[4] == true){ cout<<"processedges2: edata[4].value("<<edata[4].value<<")==42. edata[4].key: "<<edata[4].key<<endl; }
					if(edata[5].value==42 && enx[5] == true){ cout<<"processedges2: edata[5].value("<<edata[5].value<<")==42. edata[5].key: "<<edata[5].key<<endl; }
					if(edata[6].value==42 && enx[6] == true){ cout<<"processedges2: edata[6].value("<<edata[6].value<<")==42. edata[6].key: "<<edata[6].key<<endl; }
					if(edata[7].value==42 && enx[7] == true){ cout<<"processedges2: edata[7].value("<<edata[7].value<<")==42. edata[7].key: "<<edata[7].key<<endl; }
					#endif 
					
					// process
					PROCESSP0_processvector(enx2[0], 0, edata[0].value, edata[0], vbuffer[capsule_offset + 0], buffer[0], bufferoffset_kvs, &loadcount[0], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 0]
						#endif
						);
					PROCESSP0_processvector(enx2[1], 1, edata[1].value, edata[1], vbuffer[capsule_offset + 1], buffer[1], bufferoffset_kvs, &loadcount[1], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 1]
						#endif
						);
					PROCESSP0_processvector(enx2[2], 2, edata[2].value, edata[2], vbuffer[capsule_offset + 2], buffer[2], bufferoffset_kvs, &loadcount[2], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 2]
						#endif
						);
					PROCESSP0_processvector(enx2[3], 3, edata[3].value, edata[3], vbuffer[capsule_offset + 3], buffer[3], bufferoffset_kvs, &loadcount[3], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 3]
						#endif
						);
					PROCESSP0_processvector(enx2[4], 4, edata[4].value, edata[4], vbuffer[capsule_offset + 4], buffer[4], bufferoffset_kvs, &loadcount[4], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 4]
						#endif
						);
					PROCESSP0_processvector(enx2[5], 5, edata[5].value, edata[5], vbuffer[capsule_offset + 5], buffer[5], bufferoffset_kvs, &loadcount[5], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 5]
						#endif
						);
					PROCESSP0_processvector(enx2[6], 6, edata[6].value, edata[6], vbuffer[capsule_offset + 6], buffer[6], bufferoffset_kvs, &loadcount[6], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 6]
						#endif
						);
					PROCESSP0_processvector(enx2[7], 7, edata[7].value, edata[7], vbuffer[capsule_offset + 7], buffer[7], bufferoffset_kvs, &loadcount[7], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 7]
						#endif
						);
	
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[0], SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[1], SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[2], SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[3], SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[4], SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[5], SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[6], SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[7], SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
 
					#endif
				}
			}
		}
	}
	
	// for(unsigned int t=0; t<VECTOR_SIZE; t++){ cout<<"--- loadcount["<<t<<"]: "<<loadcount[t]<<endl; }
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS); ////
	
	unsigned int maxsz_kvs = 0;
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ if(loadcount[t] > maxsz_kvs){ maxsz_kvs = loadcount[t]; }}
	fetchmessage.chunksize_kvs = maxsz_kvs;//chunk_size * 2; // loadcount; // CRITICAL FIXME
	return fetchmessage;
}
#endif 
#ifdef CONFIG_TRADITIONAL_MEMORYLAYOUT
fetchmessage_t acts_all::PROCESSP0_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / (NUM_PARTITIONS / 2));
	
	uint32_type E[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	uint32_type E2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E2 complete

	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 2;
	mytravstate.end_kvs = travstate.end_kvs / 2;
	
	loffset_kvs = loffset_kvs / 2; //
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	unsigned int loadcount;

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	keyvalue_t tempbuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	loadcount = 0;
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILP0_GETV(INVALIDDATA);
	unsigned int bufferoffset_kvs = 0;
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	bufferoffset_kvs = (globalposition.source_partition % VDATA_SHRINK_RATIO) * (UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams) / VDATA_SHRINK_RATIO); //  ((BLOCKRAM_VDATA_SIZE / (NUM_PEs)) / 16) * NUM_PEs; // (3 * 24); // 0; BLOCKRAM_VDATA_SIZE
	#endif
	
	// read edge block
	#ifdef DEBUGME_PROCESSEDGES2
	cout<<"processedges2: FIRST: offset_kvs: "<<offset_kvs<<", loffset_kvs: "<<loffset_kvs<<", goffset_kvs: "<<goffset_kvs<<", edgessize_kvs: "<<edgessize_kvs<<", mytravstate.i_kvs: "<<mytravstate.i_kvs<<", mytravstate.end_kvs: "<<mytravstate.end_kvs<<endl;
	#endif 
	PROCESSBUFFERPARTITIONS_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = edges[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = edges[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = edges[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = edges[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = edges[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = edges[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = edges[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = edges[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = edges[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = edges[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = edges[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = edges[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = edges[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = edges[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = edges[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = edges[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i] = edges[offset_kvs + i].data[0].key; 
		tempbuffer[1][i] = edges[offset_kvs + i].data[0].value;	
		tempbuffer[2][i] = edges[offset_kvs + i].data[1].key; 
		tempbuffer[3][i] = edges[offset_kvs + i].data[1].value;	
		tempbuffer[4][i] = edges[offset_kvs + i].data[2].key; 
		tempbuffer[5][i] = edges[offset_kvs + i].data[2].value;	
		tempbuffer[6][i] = edges[offset_kvs + i].data[3].key; 
		tempbuffer[7][i] = edges[offset_kvs + i].data[3].value;	
		tempbuffer[8][i] = edges[offset_kvs + i].data[4].key; 
		tempbuffer[9][i] = edges[offset_kvs + i].data[4].value;	
		tempbuffer[10][i] = edges[offset_kvs + i].data[5].key; 
		tempbuffer[11][i] = edges[offset_kvs + i].data[5].value;	
		tempbuffer[12][i] = edges[offset_kvs + i].data[6].key; 
		tempbuffer[13][i] = edges[offset_kvs + i].data[6].value;	
		tempbuffer[14][i] = edges[offset_kvs + i].data[7].key; 
		tempbuffer[15][i] = edges[offset_kvs + i].data[7].value;	
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
	// bool statsiscorrect = true;
	#ifdef CONFIG_PROCESSEDGES_READOFFLINESTATS
	unsigned int sum_values = 0;
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ localcapsule[p].key = 0; localcapsule[p].value = tempbuffer[p][0]; if(p<NUM_PARTITIONS-1){ sum_values += tempbuffer[p][0]; }}
	localcapsule[NUM_PARTITIONS-1].value = (chunk_size * VECTOR2_SIZE) - sum_values;
	if(tempbuffer[NUM_PARTITIONS-1][0] != 8888888 || sum_values > chunk_size * VECTOR2_SIZE){
		#if defined(_DEBUGMODE_CHECKS) && defined(CONFIG_INSERTSTATSMETADATAINEDGES) // CRITICAL FIXME.
		if(sum_values > chunk_size * VECTOR2_SIZE && chunk_size > 0){ cout<<"processedges2: ERROR: sum_values("<<sum_values<<") > chunk_size("<<chunk_size<<") * VECTOR2_SIZE. EXITING... "<<endl; actsutilityobj->printkeyvalues("processandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS); exit(EXIT_FAILURE); } 
		if(tempbuffer[NUM_PARTITIONS-1][0] != 8888888){ cout<<"processedges2: ERROR: tempbuffer[NUM_PARTITIONS-1][0] != 8888888. EXITING... "<<endl; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<"tempbuffer["<<v<<"][0]: "<<tempbuffer[v][0]<<endl; } exit(EXIT_FAILURE); }
		#endif 
		unsigned int modelsz = chunk_size / NUM_PARTITIONS; // mock it
		for(unsigned int i=0; i<NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } 
	}
	#else 
	unsigned int modelsz = chunk_size / NUM_PARTITIONS;
	for(unsigned int i=0; i<NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } 
	#endif 
	PROCESSP0_calculateoffsets(localcapsule);
	#ifdef DEBUGME_PROCESSEDGES2
	actsutilityobj->printkeyvalues("processedges2(14).localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS); 
	cout<<"processedges2(15): "<<"chunk_size * VECTOR2_SIZE: "<<chunk_size * VECTOR2_SIZE<<", edgessize_kvs * VECTOR2_SIZE: "<<edgessize_kvs * VECTOR2_SIZE<<", WORKBUFFER_SIZE * VECTOR2_SIZE: "<<(WORKBUFFER_SIZE * VECTOR2_SIZE)<<endl;
	#endif

	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		PROCESSBUFFERPARTITIONS_LOOP1: for(buffer_type i=localcapsule[p].key; i<localcapsule[p].key + localcapsule[p].value; i++){
		#pragma HLS PIPELINE II=1
			value_t E = tempbuffer[i / VECTOR2_SIZE][i % VECTOR2_SIZE];
			
			parsededge_t parsed_edge = PROCESSP0_PARSEEDGE(E);
			keyvalue_t edata; edata.value = parsed_edge.incr; edata.key = parsed_edge.dstvid;	
	
			// read 
			vmdata_t vmdata;
			unsigned int loc = edata.value; //  - sweepparams.upperlimit;
			if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){
				#ifdef _DEBUGMODE_CHECKS
				if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
				#endif 
				loc = 0; }
		
			#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS				
				if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(loc, NAp, vbuffer[p], bufferoffset_kvs); } else { vmdata.vmask = 0; }
					#else 
						if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(loc, vbuffer[p], 0); } else { vmdata.vmask = 0; }
							#endif 
			if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", vid: "<<UTILP0_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
			#endif
					
			// process
			value_t res = PROCESSP0_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			keyvalue_t mykeyvalue; mykeyvalue.key = edata.key; mykeyvalue.value = res;
			
			// write
			if(en == true && vmdata.vmask == 1 && loadcount < WORKBUFFER_SIZE-2){ buffer[loadcount % VECTOR_SIZE][loadcount / VECTOR_SIZE] = UTILP0_GETKV(mykeyvalue); }
			if(en == true && vmdata.vmask == 1 && loadcount < WORKBUFFER_SIZE-2){ loadcount += 1; }
		}
	}
	
	fetchmessage.chunksize_kvs = loadcount / VECTOR_SIZE;
	return fetchmessage;
}
#endif










