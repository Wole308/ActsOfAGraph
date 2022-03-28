keyvalue_t acts_all::PROCESSP0_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int bufferoffset_kvs, unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams){									
	#pragma HLS PIPELINE II=2
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS
		if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
	// read 
	vmdata_t vmdata;
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS	
	// if((bufferoffset_kvs + loc/VDATA_SHRINK_RATIO) >= BLOCKRAM_VDATA_SIZE){ cout<<"PROCESS VECTOR:: ERROR: (bufferoffset_kvs("<<bufferoffset_kvs<<") + loc("<<loc<<")/2) >= BLOCKRAM_VDATA_SIZE("<<BLOCKRAM_VDATA_SIZE<<") ||| globalposition.source_partition("<<globalposition.source_partition<<") * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<") / VDATA_SHRINK_RATIO("<<VDATA_SHRINK_RATIO<<")). EXITING..."<<endl; exit(EXIT_FAILURE); }				
		if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(loc, (globalposition.source_partition % VDATA_SHRINK_RATIO), vbuffer, bufferoffset_kvs); } else { vmdata.vmask = 0; }
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
	
	if(en == true && vmdata.vmask == 1){ } else { mykeyvalue.key = INVALIDDATA; }
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsprocessed(1);
	if(en == true && vmdata.vmask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
	#endif
	return mykeyvalue;
}

void acts_all::PROCESSP0_processvectorB(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams){
	#pragma HLS INLINE
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS
		if(true){ cout<<"PROCESSP0_processvectorB::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
	// read 
	vmdata_t vmdata;
	if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(loc, vbuffer, 0); } else { vmdata.vmask = 0; }
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR(B):: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<endl; }
	#endif
			
	// process
	value_t res = PROCESSP0_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	keyvalue_t mykeyvalue; mykeyvalue.key = edata.key; mykeyvalue.value = res;
	
	// write 
	// if(en == true && vmdata.vmask == 1){ buffer[*loadcount] = UTILP0_GETKV(mykeyvalue); }
	// if(en == true && vmdata.vmask == 1){ *loadcount += 1; }
	if(en == true && vmdata.vmask == 1 && *loadcount < WORKBUFFER_SIZE-2){ buffer[*loadcount] = UTILP0_GETKV(mykeyvalue); *loadcount += 1; }
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsprocessed(1);
	if(en == true && vmdata.vmask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	if(en == true && vmdata.vmask == 1){ actsutilityobj->checkoutofbounds("PROCESSP0_processvectorB::DEBUG CODE 14::1", *loadcount, WORKBUFFER_SIZE, SOURCEBLOCKRAM_SIZE, SRCBUFFER_SIZE, WORKBUFFER_SIZE); }
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

void acts_all::PROCESSP0_RearrangeLayoutV(unsigned int s, keyvalue_t vdata[VECTOR_SIZE], keyvalue_t vdata2[VECTOR_SIZE]){
	unsigned int s_ = s % VECTOR_SIZE;
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

fetchmessage_t acts_all::PROCESSP0_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	uint32_type E[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	uint32_type E2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E2 complete
	buffer_type bramoffset_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=bramoffset_kvs complete
	buffer_type lsize_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=lsize_kvs complete
	keyvalue_t reskeyvalue[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=reskeyvalue complete
	keyvalue_t res2keyvalue[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res2keyvalue complete

	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 2;
	mytravstate.end_kvs = travstate.end_kvs / 2;
	
	loffset_kvs = loffset_kvs / 2; //
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	unsigned int loadcount[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=loadcount complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete

	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type voffset_kvs = sweepparams.source_partition * reducebuffersz * FETFACTOR;

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	keyvalue_t edata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edata complete
	keyvalue_t edata2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edata2 complete
	value_t tempbuffer[VECTOR2_SIZE][SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	
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
	PROCESSP0_calculateoffsets(localcapsule);
	#ifdef DEBUGME_PROCESSEDGES2
	actsutilityobj->printkeyvalues("processedges2(14).localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS); 
	cout<<"processedges2(15): "<<"chunk_size * VECTOR2_SIZE: "<<chunk_size * VECTOR2_SIZE<<", edgessize_kvs * VECTOR2_SIZE: "<<edgessize_kvs * VECTOR2_SIZE<<", WORKBUFFER_SIZE * VECTOR2_SIZE: "<<(WORKBUFFER_SIZE * VECTOR2_SIZE)<<endl;
	#endif
	
	// process edges 
	unsigned int readoffset = 0; if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ readoffset = 1; }
	PROCESSBUFFERPARTITIONS_LOOP2: for(unsigned int it=0; it<NUM_PARTITIONS; it+=NUM_PARTITIONS/2){
		buffer_type maxsize_kvs = 0;
		REDUCEBUFFERPARTITIONS_LOOP2B: for(partition_type p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			bramoffset_kvs[p] = localcapsule[it+p].key / VECTOR2_SIZE;
			lsize_kvs[p] = localcapsule[it+p].value / VECTOR2_SIZE;
			// lsize_kvs[p] = (localcapsule[it+p].value + (VECTOR2_SIZE-1)) / VECTOR2_SIZE;
			if(maxsize_kvs < lsize_kvs[p]){ maxsize_kvs = lsize_kvs[p]; }
		}
		
		PROCESSBUFFERPARTITIONS_LOOP2C: for(buffer_type i=0; i<maxsize_kvs; i++){
		#pragma HLS PIPELINE II=16
			for(vector_type v=0; v<VECTOR2_SIZE; v++){
	
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
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("readandprocess2(11)::DEBUG CODE 11::1", readoffset + bramoffset_kvs[0], SOURCEBLOCKRAM_SIZE, readoffset, bramoffset_kvs[0], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(11)::DEBUG CODE 11::1", readoffset + bramoffset_kvs[1], SOURCEBLOCKRAM_SIZE, readoffset, bramoffset_kvs[1], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(11)::DEBUG CODE 11::1", readoffset + bramoffset_kvs[2], SOURCEBLOCKRAM_SIZE, readoffset, bramoffset_kvs[2], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(11)::DEBUG CODE 11::1", readoffset + bramoffset_kvs[3], SOURCEBLOCKRAM_SIZE, readoffset, bramoffset_kvs[3], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(11)::DEBUG CODE 11::1", readoffset + bramoffset_kvs[4], SOURCEBLOCKRAM_SIZE, readoffset, bramoffset_kvs[4], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(11)::DEBUG CODE 11::1", readoffset + bramoffset_kvs[5], SOURCEBLOCKRAM_SIZE, readoffset, bramoffset_kvs[5], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(11)::DEBUG CODE 11::1", readoffset + bramoffset_kvs[6], SOURCEBLOCKRAM_SIZE, readoffset, bramoffset_kvs[6], NAp);
				actsutilityobj->checkoutofbounds("readandprocess2(11)::DEBUG CODE 11::1", readoffset + bramoffset_kvs[7], SOURCEBLOCKRAM_SIZE, readoffset, bramoffset_kvs[7], NAp);
 
				#endif
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
				reskeyvalue[0] = PROCESSP0_processvector(enx[0], it+0, edata[0].value, edata[0], vbuffer[it+0], buffer[0], bufferoffset_kvs, &loadcount[0], GraphAlgoClass, globalposition, globalparams);
				reskeyvalue[1] = PROCESSP0_processvector(enx[1], it+1, edata[1].value, edata[1], vbuffer[it+1], buffer[1], bufferoffset_kvs, &loadcount[1], GraphAlgoClass, globalposition, globalparams);
				reskeyvalue[2] = PROCESSP0_processvector(enx[2], it+2, edata[2].value, edata[2], vbuffer[it+2], buffer[2], bufferoffset_kvs, &loadcount[2], GraphAlgoClass, globalposition, globalparams);
				reskeyvalue[3] = PROCESSP0_processvector(enx[3], it+3, edata[3].value, edata[3], vbuffer[it+3], buffer[3], bufferoffset_kvs, &loadcount[3], GraphAlgoClass, globalposition, globalparams);
				reskeyvalue[4] = PROCESSP0_processvector(enx[4], it+4, edata[4].value, edata[4], vbuffer[it+4], buffer[4], bufferoffset_kvs, &loadcount[4], GraphAlgoClass, globalposition, globalparams);
				reskeyvalue[5] = PROCESSP0_processvector(enx[5], it+5, edata[5].value, edata[5], vbuffer[it+5], buffer[5], bufferoffset_kvs, &loadcount[5], GraphAlgoClass, globalposition, globalparams);
				reskeyvalue[6] = PROCESSP0_processvector(enx[6], it+6, edata[6].value, edata[6], vbuffer[it+6], buffer[6], bufferoffset_kvs, &loadcount[6], GraphAlgoClass, globalposition, globalparams);
				reskeyvalue[7] = PROCESSP0_processvector(enx[7], it+7, edata[7].value, edata[7], vbuffer[it+7], buffer[7], bufferoffset_kvs, &loadcount[7], GraphAlgoClass, globalposition, globalparams);
	
				
				// re-arrange 
				PROCESSP0_RearrangeLayoutV(i, reskeyvalue, res2keyvalue);
				
				if(res2keyvalue[0].key != INVALIDDATA && loadcount[0] < WORKBUFFER_SIZE-2){ buffer[0][loadcount[0]] = UTILP0_GETKV(res2keyvalue[0]); loadcount[0] += 1; }
				if(res2keyvalue[1].key != INVALIDDATA && loadcount[1] < WORKBUFFER_SIZE-2){ buffer[1][loadcount[1]] = UTILP0_GETKV(res2keyvalue[1]); loadcount[1] += 1; }
				if(res2keyvalue[2].key != INVALIDDATA && loadcount[2] < WORKBUFFER_SIZE-2){ buffer[2][loadcount[2]] = UTILP0_GETKV(res2keyvalue[2]); loadcount[2] += 1; }
				if(res2keyvalue[3].key != INVALIDDATA && loadcount[3] < WORKBUFFER_SIZE-2){ buffer[3][loadcount[3]] = UTILP0_GETKV(res2keyvalue[3]); loadcount[3] += 1; }
				if(res2keyvalue[4].key != INVALIDDATA && loadcount[4] < WORKBUFFER_SIZE-2){ buffer[4][loadcount[4]] = UTILP0_GETKV(res2keyvalue[4]); loadcount[4] += 1; }
				if(res2keyvalue[5].key != INVALIDDATA && loadcount[5] < WORKBUFFER_SIZE-2){ buffer[5][loadcount[5]] = UTILP0_GETKV(res2keyvalue[5]); loadcount[5] += 1; }
				if(res2keyvalue[6].key != INVALIDDATA && loadcount[6] < WORKBUFFER_SIZE-2){ buffer[6][loadcount[6]] = UTILP0_GETKV(res2keyvalue[6]); loadcount[6] += 1; }
				if(res2keyvalue[7].key != INVALIDDATA && loadcount[7] < WORKBUFFER_SIZE-2){ buffer[7][loadcount[7]] = UTILP0_GETKV(res2keyvalue[7]); loadcount[7] += 1; }
				
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
	}
	
	// for(unsigned int t=0; t<VECTOR_SIZE; t++){ cout<<"--- loadcount["<<t<<"]: "<<loadcount[t]<<endl; }
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS); ////
	
	unsigned int maxsz_kvs = 0;
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ if(loadcount[t] > maxsz_kvs){ maxsz_kvs = loadcount[t]; }}
	fetchmessage.chunksize_kvs = maxsz_kvs;
	
	// unsigned int minsz_kvs = INFINITI; // CRITICAL REMOVEME.
	// for(unsigned int t=0; t<VECTOR_SIZE; t++){ if(loadcount[t] < minsz_kvs){ minsz_kvs = loadcount[t]; }}
	// fetchmessage.chunksize_kvs = minsz_kvs;
	
	// fetchmessage.chunksize_kvs = 64; // CRITICAL REMOVEME.
	
	// cout<<"--- processedges:: fetchmessage.chunksize_kvs: "<<fetchmessage.chunksize_kvs<<", edgessize_kvs*2: "<<edgessize_kvs*2<<endl;
	
	// exit(EXIT_SUCCESS); ////
	return fetchmessage;
}











