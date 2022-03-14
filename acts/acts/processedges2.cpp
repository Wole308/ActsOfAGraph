#define PE_SETSZ 16
// #define DEBUGME_PROCESSEDGES2
#ifdef FPGA_IMPL
#define PROCESSEDGES_TYPE2
#else 
#define PROCESSEDGES_TYPE1
#endif 

void acts_all::PROCESSP0_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int * loadcount, unsigned int GraphAlgoClass, globalparams_t globalparams){
	#ifdef PROCESSEDGES_TYPE1
	#pragma HLS PIPELINE II=2
	#endif 
	#ifdef PROCESSEDGES_TYPE2
	#pragma HLS INLINE
	#endif 
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS
		if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
	// read 
	vmdata_t vmdata;
	if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(loc, vbuffer, 0); } else { vmdata.vmask = 0; }
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<endl; }
	#endif
			
	// process
	value_t res = PROCESSP0_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	keyvalue_t mykeyvalue; mykeyvalue.key = edata.key; mykeyvalue.value = res;
	
	// write 
	if(en == true && vmdata.vmask == 1){ buffer[*loadcount] = UTILP0_GETKV(mykeyvalue); }
	if(en == true && vmdata.vmask == 1){ *loadcount += 1; }
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsprocessed(1);
	if(en == true && vmdata.vmask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("PROCESSP0_processvector::DEBUG CODE 14::1", *loadcount, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
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
	
	// uint1024_dt W = 0;
	// 	// W.range(31, 0) = vdata[0]; 
	// 	// W.range(63, 32) = vdata[1]; 
	// 	// W.range(95, 64) = vdata[2]; 
	// 	// W.range(127, 96) = vdata[3]; 
	// 	// W.range(159, 128) = vdata[4]; 
	// 	// W.range(191, 160) = vdata[5]; 
	// 	// W.range(223, 192) = vdata[6]; 
	// 	// W.range(255, 224) = vdata[7]; 
	// 	// 	// W.range(543, 512) = vdata[0]; 
	// 	// W.range(575, 544) = vdata[1]; 
	// 	// W.range(607, 576) = vdata[2]; 
	// 	// W.range(639, 608) = vdata[3]; 
	// 	// W.range(671, 640) = vdata[4]; 
	// 	// W.range(703, 672) = vdata[5]; 
	// 	// W.range(735, 704) = vdata[6]; 
	// 	// W.range(767, 736) = vdata[7]; 
	// 	
	// W << s_;
	
	// 	// vdata2[0] = W.range(31, 0); 
	// 	// vdata2[1] = W.range(63, 32); 
	// 	// vdata2[2] = W.range(95, 64); 
	// 	// vdata2[3] = W.range(127, 96); 
	// 	// vdata2[4] = W.range(159, 128); 
	// 	// vdata2[5] = W.range(191, 160); 
	// 	// vdata2[6] = W.range(223, 192); 
	// 	// vdata2[7] = W.range(255, 224); 
	// 	return;
}

parsededge_t acts_all::PROCESSP0_PARSEEDGE(uint32_type data){ 
	parsededge_t parsededge;
	#ifdef _WIDEWORD
	// parsededge.incr = data.range(31, OFFSETOF_SRCV_IN_EDGEDSTVDATA);
	// parsededge.dstvid = data.range(SIZEOF_DSTV_IN_EDGEDSTVDATA, 0);
	parsededge.incr = UTILP0_READFROM_UINT(data, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA);
	parsededge.dstvid = UTILP0_READFROM_UINT(data, OFFSETOF_DSTV_IN_EDGEDSTVDATA, SIZEOF_DSTV_IN_EDGEDSTVDATA);
	#else
	parsededge.incr = UTILP0_READFROM_UINT(data, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA);
	parsededge.dstvid = UTILP0_READFROM_UINT(data, OFFSETOF_DSTV_IN_EDGEDSTVDATA, SIZEOF_DSTV_IN_EDGEDSTVDATA);
	#endif
	return parsededge; 
}

void acts_all::PROCESSP0_calculateoffsets2(keyvalue_capsule_t * buffer){
	buffer[0].key = 0;
	for(buffer_type i=1; i<NUM_PARTITIONS; i++){ 
	#pragma HLS PIPELINE II=2	
		buffer[i].key = UTILP0_allignlower_KV2(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}

void acts_all::PROCESSP0_calculateoffsets1(keyvalue_capsule_t * buffer){
	buffer[0].key = 0;
	for(buffer_type i=1; i<NUM_PARTITIONS; i++){ 
	#pragma HLS PIPELINE II=2	
		buffer[i].key = buffer[i-1].key + buffer[i-1].value; 
	}
	return;
}

#ifdef PROCESSEDGES_TYPE1
fetchmessage_t acts_all::PROCESSP0_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	uint32_type E[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	buffer_type bramoffset_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=bramoffset_kvs complete
	buffer_type lsize_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=lsize_kvs complete

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
	value_t tempbuffer[VECTOR2_SIZE][SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	for(unsigned int t=0; t<VECTOR2_SIZE; t++){ loadcount[t] = 0; }
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILP0_GETV(INVALIDDATA);
	
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
		#if defined(_DEBUGMODE_CHECKS3) && defined(CONFIG_INSERTSTATSMETADATAINEDGES)
		if(sum_values > chunk_size * VECTOR2_SIZE && chunk_size > 0){ cout<<"processedges2: ERROR: sum_values("<<sum_values<<") > chunk_size("<<chunk_size<<") * VECTOR2_SIZE. EXITING... "<<endl; actsutilityobj->printkeyvalues("processandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS); exit(EXIT_FAILURE); } 
		if(tempbuffer[NUM_PARTITIONS-1][0] != 8888888){ cout<<"processedges2: ERROR: tempbuffer[NUM_PARTITIONS-1][0] != 8888888. EXITING... "<<endl; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<"tempbuffer["<<v<<"][0]: "<<tempbuffer[v][0]<<endl; } exit(EXIT_FAILURE); }
		#endif 
		unsigned int modelsz = chunk_size / NUM_PARTITIONS; // mock it
		for(unsigned int i=0; i<NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } 
	}
	PROCESSP0_calculateoffsets1(localcapsule);
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
				unsigned int ind0 = (bramoffset_kvs[0] + i)*VECTOR2_SIZE + v;
	
				unsigned int partition1 = it+1;
				unsigned int ind1 = (bramoffset_kvs[1] + i)*VECTOR2_SIZE + v;
	
				unsigned int partition2 = it+2;
				unsigned int ind2 = (bramoffset_kvs[2] + i)*VECTOR2_SIZE + v;
	
				unsigned int partition3 = it+3;
				unsigned int ind3 = (bramoffset_kvs[3] + i)*VECTOR2_SIZE + v;
	
				unsigned int partition4 = it+4;
				unsigned int ind4 = (bramoffset_kvs[4] + i)*VECTOR2_SIZE + v;
	
				unsigned int partition5 = it+5;
				unsigned int ind5 = (bramoffset_kvs[5] + i)*VECTOR2_SIZE + v;
	
				unsigned int partition6 = it+6;
				unsigned int ind6 = (bramoffset_kvs[6] + i)*VECTOR2_SIZE + v;
	
				unsigned int partition7 = it+7;
				unsigned int ind7 = (bramoffset_kvs[7] + i)*VECTOR2_SIZE + v;
				
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
				
					#ifdef _DEBUGMODE_CHECKS
					unsigned int partition0 = it+0;
					if((partition0 == 1) && (i < lsize_kvs[0]) && (i*VECTOR2_SIZE+v < localcapsule[it+0].value)){
						if(edata[0].value==42 && E[0] != INVALIDDATA){
							cout<<"###### processedges2: it: "<<it;
							cout<<", p: 0";
							cout<<", i: "<<i;
							cout<<", E[0]: "<<E[0];
							cout<<", bramoffset_kvs[0]: "<<bramoffset_kvs[0];
							cout<<", edata[0].key: "<<edata[0].key;
							cout<<", edata[0].value: "<<edata[0].value;
							cout<<", real dstvid: "<<(edata[0].key * NUM_PEs) + globalparams.ACTSPARAMS_INSTID;
							cout<<", bramoffset_kvs[0] + i: "<<bramoffset_kvs[0] + i<<" ["<<(bramoffset_kvs[0] + i) * VECTOR2_SIZE<<"]";
							cout<<endl; }
					}
					unsigned int partition1 = it+1;
					if((partition1 == 1) && (i < lsize_kvs[1]) && (i*VECTOR2_SIZE+v < localcapsule[it+1].value)){
						if(edata[1].value==42 && E[1] != INVALIDDATA){
							cout<<"###### processedges2: it: "<<it;
							cout<<", p: 1";
							cout<<", i: "<<i;
							cout<<", E[1]: "<<E[1];
							cout<<", bramoffset_kvs[1]: "<<bramoffset_kvs[1];
							cout<<", edata[1].key: "<<edata[1].key;
							cout<<", edata[1].value: "<<edata[1].value;
							cout<<", real dstvid: "<<(edata[1].key * NUM_PEs) + globalparams.ACTSPARAMS_INSTID;
							cout<<", bramoffset_kvs[1] + i: "<<bramoffset_kvs[1] + i<<" ["<<(bramoffset_kvs[1] + i) * VECTOR2_SIZE<<"]";
							cout<<endl; }
					}
					unsigned int partition2 = it+2;
					if((partition2 == 1) && (i < lsize_kvs[2]) && (i*VECTOR2_SIZE+v < localcapsule[it+2].value)){
						if(edata[2].value==42 && E[2] != INVALIDDATA){
							cout<<"###### processedges2: it: "<<it;
							cout<<", p: 2";
							cout<<", i: "<<i;
							cout<<", E[2]: "<<E[2];
							cout<<", bramoffset_kvs[2]: "<<bramoffset_kvs[2];
							cout<<", edata[2].key: "<<edata[2].key;
							cout<<", edata[2].value: "<<edata[2].value;
							cout<<", real dstvid: "<<(edata[2].key * NUM_PEs) + globalparams.ACTSPARAMS_INSTID;
							cout<<", bramoffset_kvs[2] + i: "<<bramoffset_kvs[2] + i<<" ["<<(bramoffset_kvs[2] + i) * VECTOR2_SIZE<<"]";
							cout<<endl; }
					}
					unsigned int partition3 = it+3;
					if((partition3 == 1) && (i < lsize_kvs[3]) && (i*VECTOR2_SIZE+v < localcapsule[it+3].value)){
						if(edata[3].value==42 && E[3] != INVALIDDATA){
							cout<<"###### processedges2: it: "<<it;
							cout<<", p: 3";
							cout<<", i: "<<i;
							cout<<", E[3]: "<<E[3];
							cout<<", bramoffset_kvs[3]: "<<bramoffset_kvs[3];
							cout<<", edata[3].key: "<<edata[3].key;
							cout<<", edata[3].value: "<<edata[3].value;
							cout<<", real dstvid: "<<(edata[3].key * NUM_PEs) + globalparams.ACTSPARAMS_INSTID;
							cout<<", bramoffset_kvs[3] + i: "<<bramoffset_kvs[3] + i<<" ["<<(bramoffset_kvs[3] + i) * VECTOR2_SIZE<<"]";
							cout<<endl; }
					}
					unsigned int partition4 = it+4;
					if((partition4 == 1) && (i < lsize_kvs[4]) && (i*VECTOR2_SIZE+v < localcapsule[it+4].value)){
						if(edata[4].value==42 && E[4] != INVALIDDATA){
							cout<<"###### processedges2: it: "<<it;
							cout<<", p: 4";
							cout<<", i: "<<i;
							cout<<", E[4]: "<<E[4];
							cout<<", bramoffset_kvs[4]: "<<bramoffset_kvs[4];
							cout<<", edata[4].key: "<<edata[4].key;
							cout<<", edata[4].value: "<<edata[4].value;
							cout<<", real dstvid: "<<(edata[4].key * NUM_PEs) + globalparams.ACTSPARAMS_INSTID;
							cout<<", bramoffset_kvs[4] + i: "<<bramoffset_kvs[4] + i<<" ["<<(bramoffset_kvs[4] + i) * VECTOR2_SIZE<<"]";
							cout<<endl; }
					}
					unsigned int partition5 = it+5;
					if((partition5 == 1) && (i < lsize_kvs[5]) && (i*VECTOR2_SIZE+v < localcapsule[it+5].value)){
						if(edata[5].value==42 && E[5] != INVALIDDATA){
							cout<<"###### processedges2: it: "<<it;
							cout<<", p: 5";
							cout<<", i: "<<i;
							cout<<", E[5]: "<<E[5];
							cout<<", bramoffset_kvs[5]: "<<bramoffset_kvs[5];
							cout<<", edata[5].key: "<<edata[5].key;
							cout<<", edata[5].value: "<<edata[5].value;
							cout<<", real dstvid: "<<(edata[5].key * NUM_PEs) + globalparams.ACTSPARAMS_INSTID;
							cout<<", bramoffset_kvs[5] + i: "<<bramoffset_kvs[5] + i<<" ["<<(bramoffset_kvs[5] + i) * VECTOR2_SIZE<<"]";
							cout<<endl; }
					}
					unsigned int partition6 = it+6;
					if((partition6 == 1) && (i < lsize_kvs[6]) && (i*VECTOR2_SIZE+v < localcapsule[it+6].value)){
						if(edata[6].value==42 && E[6] != INVALIDDATA){
							cout<<"###### processedges2: it: "<<it;
							cout<<", p: 6";
							cout<<", i: "<<i;
							cout<<", E[6]: "<<E[6];
							cout<<", bramoffset_kvs[6]: "<<bramoffset_kvs[6];
							cout<<", edata[6].key: "<<edata[6].key;
							cout<<", edata[6].value: "<<edata[6].value;
							cout<<", real dstvid: "<<(edata[6].key * NUM_PEs) + globalparams.ACTSPARAMS_INSTID;
							cout<<", bramoffset_kvs[6] + i: "<<bramoffset_kvs[6] + i<<" ["<<(bramoffset_kvs[6] + i) * VECTOR2_SIZE<<"]";
							cout<<endl; }
					}
					unsigned int partition7 = it+7;
					if((partition7 == 1) && (i < lsize_kvs[7]) && (i*VECTOR2_SIZE+v < localcapsule[it+7].value)){
						if(edata[7].value==42 && E[7] != INVALIDDATA){
							cout<<"###### processedges2: it: "<<it;
							cout<<", p: 7";
							cout<<", i: "<<i;
							cout<<", E[7]: "<<E[7];
							cout<<", bramoffset_kvs[7]: "<<bramoffset_kvs[7];
							cout<<", edata[7].key: "<<edata[7].key;
							cout<<", edata[7].value: "<<edata[7].value;
							cout<<", real dstvid: "<<(edata[7].key * NUM_PEs) + globalparams.ACTSPARAMS_INSTID;
							cout<<", bramoffset_kvs[7] + i: "<<bramoffset_kvs[7] + i<<" ["<<(bramoffset_kvs[7] + i) * VECTOR2_SIZE<<"]";
							cout<<endl; }
					}
					#endif 
				
				// process	
				// 	
				// if((i < lsize_kvs[0]) && (i*VECTOR2_SIZE+v < localcapsule[it+0].value)){ PROCESSP0_processvector(true, it+0, edata[0].value, edata[0], vbuffer[it+0], buffer[0], &loadcount[0], GraphAlgoClass, globalparams); }	
				// 	
				// if((i < lsize_kvs[1]) && (i*VECTOR2_SIZE+v < localcapsule[it+1].value)){ PROCESSP0_processvector(true, it+1, edata[1].value, edata[1], vbuffer[it+1], buffer[1], &loadcount[1], GraphAlgoClass, globalparams); }	
				// 	
				// if((i < lsize_kvs[2]) && (i*VECTOR2_SIZE+v < localcapsule[it+2].value)){ PROCESSP0_processvector(true, it+2, edata[2].value, edata[2], vbuffer[it+2], buffer[2], &loadcount[2], GraphAlgoClass, globalparams); }	
				// 	
				// if((i < lsize_kvs[3]) && (i*VECTOR2_SIZE+v < localcapsule[it+3].value)){ PROCESSP0_processvector(true, it+3, edata[3].value, edata[3], vbuffer[it+3], buffer[3], &loadcount[3], GraphAlgoClass, globalparams); }	
				// 	
				// if((i < lsize_kvs[4]) && (i*VECTOR2_SIZE+v < localcapsule[it+4].value)){ PROCESSP0_processvector(true, it+4, edata[4].value, edata[4], vbuffer[it+4], buffer[4], &loadcount[4], GraphAlgoClass, globalparams); }	
				// 	
				// if((i < lsize_kvs[5]) && (i*VECTOR2_SIZE+v < localcapsule[it+5].value)){ PROCESSP0_processvector(true, it+5, edata[5].value, edata[5], vbuffer[it+5], buffer[5], &loadcount[5], GraphAlgoClass, globalparams); }	
				// 	
				// if((i < lsize_kvs[6]) && (i*VECTOR2_SIZE+v < localcapsule[it+6].value)){ PROCESSP0_processvector(true, it+6, edata[6].value, edata[6], vbuffer[it+6], buffer[6], &loadcount[6], GraphAlgoClass, globalparams); }	
				// 	
				// if((i < lsize_kvs[7]) && (i*VECTOR2_SIZE+v < localcapsule[it+7].value)){ PROCESSP0_processvector(true, it+7, edata[7].value, edata[7], vbuffer[it+7], buffer[7], &loadcount[7], GraphAlgoClass, globalparams); }	
				// 	
				/* 	
				unsigned int partition0 = it+0;
				if(
					partition0 > 0
						// && (i*VECTOR2_SIZE+v > localcapsule[it+-1].key + localcapsule[it+-1].value) 
						&& (i*VECTOR2_SIZE+v > localcapsule[it+-1].key + localcapsule[it+-1].value) // CRITICAL CHECKME. CAN BE SOURCE OF CLOCK II DELAY
							&& (i*VECTOR2_SIZE+v < localcapsule[it+0].value))
					{ PROCESSP0_processvector(true, it+0, edata[0].value, edata[0], vbuffer[it+0], buffer[0], &loadcount[0], GraphAlgoClass, globalparams); }	
	
				unsigned int partition1 = it+1;
				if(
					partition1 > 0
						// && (i*VECTOR2_SIZE+v > localcapsule[it+0].key + localcapsule[it+0].value) 
						&& (i*VECTOR2_SIZE+v > localcapsule[it+0].key + localcapsule[it+0].value) // CRITICAL CHECKME. CAN BE SOURCE OF CLOCK II DELAY
							&& (i*VECTOR2_SIZE+v < localcapsule[it+1].value))
					{ PROCESSP0_processvector(true, it+1, edata[1].value, edata[1], vbuffer[it+1], buffer[1], &loadcount[1], GraphAlgoClass, globalparams); }	
	
				unsigned int partition2 = it+2;
				if(
					partition2 > 0
						// && (i*VECTOR2_SIZE+v > localcapsule[it+1].key + localcapsule[it+1].value) 
						&& (i*VECTOR2_SIZE+v > localcapsule[it+1].key + localcapsule[it+1].value) // CRITICAL CHECKME. CAN BE SOURCE OF CLOCK II DELAY
							&& (i*VECTOR2_SIZE+v < localcapsule[it+2].value))
					{ PROCESSP0_processvector(true, it+2, edata[2].value, edata[2], vbuffer[it+2], buffer[2], &loadcount[2], GraphAlgoClass, globalparams); }	
	
				unsigned int partition3 = it+3;
				if(
					partition3 > 0
						// && (i*VECTOR2_SIZE+v > localcapsule[it+2].key + localcapsule[it+2].value) 
						&& (i*VECTOR2_SIZE+v > localcapsule[it+2].key + localcapsule[it+2].value) // CRITICAL CHECKME. CAN BE SOURCE OF CLOCK II DELAY
							&& (i*VECTOR2_SIZE+v < localcapsule[it+3].value))
					{ PROCESSP0_processvector(true, it+3, edata[3].value, edata[3], vbuffer[it+3], buffer[3], &loadcount[3], GraphAlgoClass, globalparams); }	
	
				unsigned int partition4 = it+4;
				if(
					partition4 > 0
						// && (i*VECTOR2_SIZE+v > localcapsule[it+3].key + localcapsule[it+3].value) 
						&& (i*VECTOR2_SIZE+v > localcapsule[it+3].key + localcapsule[it+3].value) // CRITICAL CHECKME. CAN BE SOURCE OF CLOCK II DELAY
							&& (i*VECTOR2_SIZE+v < localcapsule[it+4].value))
					{ PROCESSP0_processvector(true, it+4, edata[4].value, edata[4], vbuffer[it+4], buffer[4], &loadcount[4], GraphAlgoClass, globalparams); }	
	
				unsigned int partition5 = it+5;
				if(
					partition5 > 0
						// && (i*VECTOR2_SIZE+v > localcapsule[it+4].key + localcapsule[it+4].value) 
						&& (i*VECTOR2_SIZE+v > localcapsule[it+4].key + localcapsule[it+4].value) // CRITICAL CHECKME. CAN BE SOURCE OF CLOCK II DELAY
							&& (i*VECTOR2_SIZE+v < localcapsule[it+5].value))
					{ PROCESSP0_processvector(true, it+5, edata[5].value, edata[5], vbuffer[it+5], buffer[5], &loadcount[5], GraphAlgoClass, globalparams); }	
	
				unsigned int partition6 = it+6;
				if(
					partition6 > 0
						// && (i*VECTOR2_SIZE+v > localcapsule[it+5].key + localcapsule[it+5].value) 
						&& (i*VECTOR2_SIZE+v > localcapsule[it+5].key + localcapsule[it+5].value) // CRITICAL CHECKME. CAN BE SOURCE OF CLOCK II DELAY
							&& (i*VECTOR2_SIZE+v < localcapsule[it+6].value))
					{ PROCESSP0_processvector(true, it+6, edata[6].value, edata[6], vbuffer[it+6], buffer[6], &loadcount[6], GraphAlgoClass, globalparams); }	
	
				unsigned int partition7 = it+7;
				if(
					partition7 > 0
						// && (i*VECTOR2_SIZE+v > localcapsule[it+6].key + localcapsule[it+6].value) 
						&& (i*VECTOR2_SIZE+v > localcapsule[it+6].key + localcapsule[it+6].value) // CRITICAL CHECKME. CAN BE SOURCE OF CLOCK II DELAY
							&& (i*VECTOR2_SIZE+v < localcapsule[it+7].value))
					{ PROCESSP0_processvector(true, it+7, edata[7].value, edata[7], vbuffer[it+7], buffer[7], &loadcount[7], GraphAlgoClass, globalparams); }	
	 */		
				
				if((ind0 >= localcapsule[it+0].key) && (ind0 < localcapsule[it+0].key + localcapsule[it+0].value))
					{ PROCESSP0_processvector(true, it+0, edata[0].value, edata[0], vbuffer[it+0], buffer[0], &loadcount[0], GraphAlgoClass, globalparams); }	
				if((ind1 >= localcapsule[it+1].key) && (ind1 < localcapsule[it+1].key + localcapsule[it+1].value))
					{ PROCESSP0_processvector(true, it+1, edata[1].value, edata[1], vbuffer[it+1], buffer[1], &loadcount[1], GraphAlgoClass, globalparams); }	
				if((ind2 >= localcapsule[it+2].key) && (ind2 < localcapsule[it+2].key + localcapsule[it+2].value))
					{ PROCESSP0_processvector(true, it+2, edata[2].value, edata[2], vbuffer[it+2], buffer[2], &loadcount[2], GraphAlgoClass, globalparams); }	
				if((ind3 >= localcapsule[it+3].key) && (ind3 < localcapsule[it+3].key + localcapsule[it+3].value))
					{ PROCESSP0_processvector(true, it+3, edata[3].value, edata[3], vbuffer[it+3], buffer[3], &loadcount[3], GraphAlgoClass, globalparams); }	
				if((ind4 >= localcapsule[it+4].key) && (ind4 < localcapsule[it+4].key + localcapsule[it+4].value))
					{ PROCESSP0_processvector(true, it+4, edata[4].value, edata[4], vbuffer[it+4], buffer[4], &loadcount[4], GraphAlgoClass, globalparams); }	
				if((ind5 >= localcapsule[it+5].key) && (ind5 < localcapsule[it+5].key + localcapsule[it+5].value))
					{ PROCESSP0_processvector(true, it+5, edata[5].value, edata[5], vbuffer[it+5], buffer[5], &loadcount[5], GraphAlgoClass, globalparams); }	
				if((ind6 >= localcapsule[it+6].key) && (ind6 < localcapsule[it+6].key + localcapsule[it+6].value))
					{ PROCESSP0_processvector(true, it+6, edata[6].value, edata[6], vbuffer[it+6], buffer[6], &loadcount[6], GraphAlgoClass, globalparams); }	
				if((ind7 >= localcapsule[it+7].key) && (ind7 < localcapsule[it+7].key + localcapsule[it+7].value))
					{ PROCESSP0_processvector(true, it+7, edata[7].value, edata[7], vbuffer[it+7], buffer[7], &loadcount[7], GraphAlgoClass, globalparams); }	
	
				#ifdef _DEBUGMODE_CHECKS3
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
	
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ cout<<"--- loadcount["<<t<<"]: "<<loadcount[t]<<endl; }
	actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS); ////
	
	unsigned int maxsz_kvs = 0;
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ if(loadcount[t] > maxsz_kvs){ maxsz_kvs = loadcount[t]; }}
	fetchmessage.chunksize_kvs = maxsz_kvs;//chunk_size * 2; // loadcount; // CRITICAL FIXME
	return fetchmessage;
}
#endif 

#ifdef PROCESSEDGES_TYPE2
fetchmessage_t acts_all::PROCESSP0_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
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

	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type voffset_kvs = sweepparams.source_partition * reducebuffersz * FETFACTOR;

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	keyvalue_t edata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edata complete
	value_t tempbuffer[VECTOR2_SIZE][SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	for(unsigned int t=0; t<VECTOR2_SIZE; t++){ loadcount[t] = 0; }
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILP0_GETV(INVALIDDATA);
	
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
		#if defined(_DEBUGMODE_CHECKS3) && defined(CONFIG_INSERTSTATSMETADATAINEDGES)
		if(sum_values > chunk_size * VECTOR2_SIZE && chunk_size > 0){ cout<<"processedges2: ERROR: sum_values("<<sum_values<<") > chunk_size("<<chunk_size<<") * VECTOR2_SIZE. EXITING... "<<endl; actsutilityobj->printkeyvalues("processandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS); exit(EXIT_FAILURE); } 
		if(tempbuffer[NUM_PARTITIONS-1][0] != 8888888){ cout<<"processedges2: ERROR: tempbuffer[NUM_PARTITIONS-1][0] != 8888888. EXITING... "<<endl; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<"tempbuffer["<<v<<"][0]: "<<tempbuffer[v][0]<<endl; } exit(EXIT_FAILURE); }
		#endif 
		unsigned int modelsz = chunk_size / NUM_PARTITIONS; // mock it
		for(unsigned int i=0; i<NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } 
	}
	PROCESSP0_calculateoffsets2(localcapsule);
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
		localcapsule[p].key = localcapsule[p].key / VECTOR2_SIZE;
		localcapsule[p].value = localcapsule[p].value / VECTOR2_SIZE;
	}
	
	// process edge block
	unsigned int readoffset = 0;
	if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ readoffset = 1; }
	buffer_type psetoffset=0;
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
					// read edges . FIXME
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

					unsigned int tdepth0 = capsule_offset + depths[0];
					unsigned int tdepth1 = capsule_offset + depths[1];
					unsigned int tdepth2 = capsule_offset + depths[2];
					unsigned int tdepth3 = capsule_offset + depths[3];
					unsigned int tdepth4 = capsule_offset + depths[4];
					unsigned int tdepth5 = capsule_offset + depths[5];
					unsigned int tdepth6 = capsule_offset + depths[6];
					unsigned int tdepth7 = capsule_offset + depths[7];
 	
					if(i < localcapsule[tdepth0].value){ enx[0] = true; } else { enx[0] = false; }
					if(i < localcapsule[tdepth1].value){ enx[1] = true; } else { enx[1] = false; }
					if(i < localcapsule[tdepth2].value){ enx[2] = true; } else { enx[2] = false; }
					if(i < localcapsule[tdepth3].value){ enx[3] = true; } else { enx[3] = false; }
					if(i < localcapsule[tdepth4].value){ enx[4] = true; } else { enx[4] = false; }
					if(i < localcapsule[tdepth5].value){ enx[5] = true; } else { enx[5] = false; }
					if(i < localcapsule[tdepth6].value){ enx[6] = true; } else { enx[6] = false; }
					if(i < localcapsule[tdepth7].value){ enx[7] = true; } else { enx[7] = false; }
 
					
					d_kvs[0] = readoffset + localcapsule[tdepth0].key + i; 
					d_kvs[1] = readoffset + localcapsule[tdepth1].key + i; 
					d_kvs[2] = readoffset + localcapsule[tdepth2].key + i; 
					d_kvs[3] = readoffset + localcapsule[tdepth3].key + i; 
					d_kvs[4] = readoffset + localcapsule[tdepth4].key + i; 
					d_kvs[5] = readoffset + localcapsule[tdepth5].key + i; 
					d_kvs[6] = readoffset + localcapsule[tdepth6].key + i; 
					d_kvs[7] = readoffset + localcapsule[tdepth7].key + i; 
 	
					if(d_kvs[0] < height_kvs){ E[0] = tempbuffer[block_offset + 0][d_kvs[0]]; } else { E[0] = INVALIDDATA; }	
					if(d_kvs[1] < height_kvs){ E[1] = tempbuffer[block_offset + 1][d_kvs[1]]; } else { E[1] = INVALIDDATA; }	
					if(d_kvs[2] < height_kvs){ E[2] = tempbuffer[block_offset + 2][d_kvs[2]]; } else { E[2] = INVALIDDATA; }	
					if(d_kvs[3] < height_kvs){ E[3] = tempbuffer[block_offset + 3][d_kvs[3]]; } else { E[3] = INVALIDDATA; }	
					if(d_kvs[4] < height_kvs){ E[4] = tempbuffer[block_offset + 4][d_kvs[4]]; } else { E[4] = INVALIDDATA; }	
					if(d_kvs[5] < height_kvs){ E[5] = tempbuffer[block_offset + 5][d_kvs[5]]; } else { E[5] = INVALIDDATA; }	
					if(d_kvs[6] < height_kvs){ E[6] = tempbuffer[block_offset + 6][d_kvs[6]]; } else { E[6] = INVALIDDATA; }	
					if(d_kvs[7] < height_kvs){ E[7] = tempbuffer[block_offset + 7][d_kvs[7]]; } else { E[7] = INVALIDDATA; }	
 	
					
					// re-arrange 
					PROCESSP0_RearrangeLayoutV(r, E, E2);
					
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
					
					if(edata[0].value==42 && E[0] != INVALIDDATA){ cout<<"######################################### processedges2: edata[0].value("<<edata[0].value<<")==42. edata[0].key: "<<edata[0].key<<endl; }
					if(edata[1].value==42 && E[1] != INVALIDDATA){ cout<<"######################################### processedges2: edata[1].value("<<edata[1].value<<")==42. edata[1].key: "<<edata[1].key<<endl; }
					if(edata[2].value==42 && E[2] != INVALIDDATA){ cout<<"######################################### processedges2: edata[2].value("<<edata[2].value<<")==42. edata[2].key: "<<edata[2].key<<endl; }
					if(edata[3].value==42 && E[3] != INVALIDDATA){ cout<<"######################################### processedges2: edata[3].value("<<edata[3].value<<")==42. edata[3].key: "<<edata[3].key<<endl; }
					if(edata[4].value==42 && E[4] != INVALIDDATA){ cout<<"######################################### processedges2: edata[4].value("<<edata[4].value<<")==42. edata[4].key: "<<edata[4].key<<endl; }
					if(edata[5].value==42 && E[5] != INVALIDDATA){ cout<<"######################################### processedges2: edata[5].value("<<edata[5].value<<")==42. edata[5].key: "<<edata[5].key<<endl; }
					if(edata[6].value==42 && E[6] != INVALIDDATA){ cout<<"######################################### processedges2: edata[6].value("<<edata[6].value<<")==42. edata[6].key: "<<edata[6].key<<endl; }
					if(edata[7].value==42 && E[7] != INVALIDDATA){ cout<<"######################################### processedges2: edata[7].value("<<edata[7].value<<")==42. edata[7].key: "<<edata[7].key<<endl; }
					
					
					/* /////
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
					///// */
					
					// process
					if(E[0] == INVALIDDATA){ enx[0] = false; } // edata[0].key = 0; edata[0].value  = INVALIDDATA; }
					PROCESSP0_processvector(enx[0], 0, edata[0].value, edata[0], vbuffer[capsule_offset + 0], buffer[0], &loadcount[0], GraphAlgoClass, globalparams);
					if(E[1] == INVALIDDATA){ enx[1] = false; } // edata[1].key = 0; edata[1].value  = INVALIDDATA; }
					PROCESSP0_processvector(enx[1], 1, edata[1].value, edata[1], vbuffer[capsule_offset + 1], buffer[1], &loadcount[1], GraphAlgoClass, globalparams);
					if(E[2] == INVALIDDATA){ enx[2] = false; } // edata[2].key = 0; edata[2].value  = INVALIDDATA; }
					PROCESSP0_processvector(enx[2], 2, edata[2].value, edata[2], vbuffer[capsule_offset + 2], buffer[2], &loadcount[2], GraphAlgoClass, globalparams);
					if(E[3] == INVALIDDATA){ enx[3] = false; } // edata[3].key = 0; edata[3].value  = INVALIDDATA; }
					PROCESSP0_processvector(enx[3], 3, edata[3].value, edata[3], vbuffer[capsule_offset + 3], buffer[3], &loadcount[3], GraphAlgoClass, globalparams);
					if(E[4] == INVALIDDATA){ enx[4] = false; } // edata[4].key = 0; edata[4].value  = INVALIDDATA; }
					PROCESSP0_processvector(enx[4], 4, edata[4].value, edata[4], vbuffer[capsule_offset + 4], buffer[4], &loadcount[4], GraphAlgoClass, globalparams);
					if(E[5] == INVALIDDATA){ enx[5] = false; } // edata[5].key = 0; edata[5].value  = INVALIDDATA; }
					PROCESSP0_processvector(enx[5], 5, edata[5].value, edata[5], vbuffer[capsule_offset + 5], buffer[5], &loadcount[5], GraphAlgoClass, globalparams);
					if(E[6] == INVALIDDATA){ enx[6] = false; } // edata[6].key = 0; edata[6].value  = INVALIDDATA; }
					PROCESSP0_processvector(enx[6], 6, edata[6].value, edata[6], vbuffer[capsule_offset + 6], buffer[6], &loadcount[6], GraphAlgoClass, globalparams);
					if(E[7] == INVALIDDATA){ enx[7] = false; } // edata[7].key = 0; edata[7].value  = INVALIDDATA; }
					PROCESSP0_processvector(enx[7], 7, edata[7].value, edata[7], vbuffer[capsule_offset + 7], buffer[7], &loadcount[7], GraphAlgoClass, globalparams);
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
	
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ cout<<"--- loadcount["<<t<<"]: "<<loadcount[t]<<endl; }
	actsutilityobj->printglobalvars();
	exit(EXIT_SUCCESS); ////
	
	unsigned int maxsz_kvs = 0;
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ if(loadcount[t] > maxsz_kvs){ maxsz_kvs = loadcount[t]; }}
	fetchmessage.chunksize_kvs = maxsz_kvs;//chunk_size * 2; // loadcount; // CRITICAL FIXME
	return fetchmessage;
}
#endif 









