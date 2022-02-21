#define PE_SETSZ 16
void acts_all::PROCESSP0_processvector(bool enx, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[SOURCEBLOCKRAM_SIZE], unsigned int * loadcount, unsigned int GraphAlgoClass, globalparams_t globalparams){
	#pragma HLS INLINE
	bool en = true; if(edata.key != INVALIDDATA && edata.value != INVALIDDATA && enx == true){ en = true; } else { en = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3 // upperlimit + loc
	if(en == true){ cout<<"PROCESSP0_processvector:: PROCESS SEEN @ vid: "<<UTILP0_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<", loc: "<<loc<<", edata.key(dstvid): "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", upperlimit: "<<upperlimit<<", process size: "<<globalparams.SIZE_REDUCE<<endl; }
	#endif 
	
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS2X
		if(true){ cout<<"PROCESSP0_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
	// read 
	vmdata_t vmdata;
	if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(loc, vbuffer, 0); }
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; } 
			
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
	parsededge.incr = data.range(31, OFFSETOF_SRCV_IN_EDGEDSTVDATA);
	parsededge.dstvid = data.range(SIZEOF_DSTV_IN_EDGEDSTVDATA, 0);
	#else
	parsededge.incr = UTILP0_READFROM_UINT(data, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA);
	parsededge.dstvid = UTILP0_READFROM_UINT(data, OFFSETOF_DSTV_IN_EDGEDSTVDATA, SIZEOF_DSTV_IN_EDGEDSTVDATA);
	#endif
	return parsededge; 
}

fetchmessage_t acts_all::PROCESSP0_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	// NB: localcapsule[x].key & localcapsule[x].key are all in _kvs formats
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
	
	travstate.i_kvs = travstate.i_kvs / 2;
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
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
	#pragma HLS array_partition variable = localcapsule
	
	buffer_type chunk_size = UTILP0_getchunksize_kvs(edgessize_kvs, travstate, 0);
	
	// CRITICAL FIXME. Get this is a metadata in DRAM
	unsigned int modelsz = chunk_size / NUM_PARTITIONS;
	for(unsigned int i=0; i<NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } // FIXME.
	for(unsigned int t=0; t<VECTOR2_SIZE; t++){ loadcount[t] = 0; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("processandbuffer.localcapsule", (keyvalue_t *)localcapsule, NUM_PARTITIONS);
	#endif
	
	PROCESSBUFFERPARTITIONS_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
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
	}
	
	buffer_type maxsize_kvs[2]; maxsize_kvs[0] = 0; maxsize_kvs[1] = 0;
	buffer_type totalsize_kvs = 0;
	unsigned int key_kvs[NUM_PARTITIONS];
	unsigned int _poff = 0;
	PROCESSBUFFERPARTITIONS_LOOP2: for(unsigned int cid=0; cid<2; cid++){
		PROCESSBUFFERPARTITIONS_LOOP2B: for(partition_type p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=2
			unsigned int ssize_kvs = localcapsule[_poff + p].value / VECTOR2_SIZE;
			if(maxsize_kvs[cid] < ssize_kvs){ maxsize_kvs[cid] = ssize_kvs; }
			totalsize_kvs += ssize_kvs;
			key_kvs[_poff + p] = localcapsule[_poff + p].key / VECTOR2_SIZE;
			// cout<<"------------- ssize_kvs: "<<ssize_kvs<<", maxsize_kvs["<<cid<<"]: "<<maxsize_kvs[cid]<<", localcapsule["<<_poff + p<<"].value: "<<localcapsule[_poff + p].value<<", key_kvs["<<_poff + p<<"]: "<<key_kvs[_poff + p]<<endl;
		}
		_poff += NUM_PARTITIONS/2;
	}
	buffer_type height_kvs = (localcapsule[NUM_PARTITIONS-1].key + localcapsule[NUM_PARTITIONS-1].value) / VECTOR2_SIZE;
	// buffer_type height_kvs = chunk_size; // FIXME.
	// cout<<"------------- height_kvs: "<<height_kvs<<", maxsize_kvs[0]: "<<maxsize_kvs[0]<<", maxsize_kvs[1]: "<<maxsize_kvs[1]<<endl;
	// exit(EXIT_SUCCESS); ////////////////
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		localcapsule[p].key = localcapsule[p].key / VECTOR2_SIZE;
		localcapsule[p].value = localcapsule[p].value / VECTOR2_SIZE;
	}
 
	buffer_type psetoffset=0;
	PROCESSBUFFERPARTITIONS_LOOP3: for(buffer_type capsule_offset=0; capsule_offset<NUM_PARTITIONS; capsule_offset+=VECTOR_SIZE){ // processing next capsule set
		unsigned int mmaxsz_kvs = maxsize_kvs[capsule_offset / VECTOR_SIZE];
		PROCESSBUFFERPARTITIONS_LOOP3B: for(buffer_type block_offset=0; block_offset<VECTOR2_SIZE; block_offset+=VECTOR_SIZE){ // processing next block set 
			PROCESSBUFFERPARTITIONS_LOOP3C: for(unsigned int r=0; r<VECTOR_SIZE; r++){ // interchanging columns
				PROCESSBUFFERPARTITIONS_LOOP3D: for(buffer_type i=0; i<mmaxsz_kvs; i++){ // processing a column data
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					#endif
					// read edges . FIXME
					PROCESSP0_GetXYLayoutV(r, depths, 0);
					#ifdef _DEBUGMODE_CHECKS2
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
 
					
					d_kvs[0] = localcapsule[tdepth0].key + i; 	
					d_kvs[1] = localcapsule[tdepth1].key + i; 	
					d_kvs[2] = localcapsule[tdepth2].key + i; 	
					d_kvs[3] = localcapsule[tdepth3].key + i; 	
					d_kvs[4] = localcapsule[tdepth4].key + i; 	
					d_kvs[5] = localcapsule[tdepth5].key + i; 	
					d_kvs[6] = localcapsule[tdepth6].key + i; 	
					d_kvs[7] = localcapsule[tdepth7].key + i; 	
 	
					if(d_kvs[0] < height_kvs){ E[0] = tempbuffer[block_offset + 0][d_kvs[0]]; } else { E[0] = 555; }	
					if(d_kvs[1] < height_kvs){ E[1] = tempbuffer[block_offset + 1][d_kvs[1]]; } else { E[1] = 555; }	
					if(d_kvs[2] < height_kvs){ E[2] = tempbuffer[block_offset + 2][d_kvs[2]]; } else { E[2] = 555; }	
					if(d_kvs[3] < height_kvs){ E[3] = tempbuffer[block_offset + 3][d_kvs[3]]; } else { E[3] = 555; }	
					if(d_kvs[4] < height_kvs){ E[4] = tempbuffer[block_offset + 4][d_kvs[4]]; } else { E[4] = 555; }	
					if(d_kvs[5] < height_kvs){ E[5] = tempbuffer[block_offset + 5][d_kvs[5]]; } else { E[5] = 555; }	
					if(d_kvs[6] < height_kvs){ E[6] = tempbuffer[block_offset + 6][d_kvs[6]]; } else { E[6] = 555; }	
					if(d_kvs[7] < height_kvs){ E[7] = tempbuffer[block_offset + 7][d_kvs[7]]; } else { E[7] = 555; }	
 	
					
					// re-arrange 
					PROCESSP0_RearrangeLayoutV(r, E, E2);
					
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
					
					// process
					// if(block_offset == 0){ enx[0] = false; }
					if(E[0] == INVALIDDATA){ enx[0] = false; }
					PROCESSP0_processvector(enx[0], edata[0].value, edata[0], vbuffer[capsule_offset + 0], buffer[0], &loadcount[0], GraphAlgoClass, globalparams);
					if(E[1] == INVALIDDATA){ enx[1] = false; }
					PROCESSP0_processvector(enx[1], edata[1].value, edata[1], vbuffer[capsule_offset + 1], buffer[1], &loadcount[1], GraphAlgoClass, globalparams);
					if(E[2] == INVALIDDATA){ enx[2] = false; }
					PROCESSP0_processvector(enx[2], edata[2].value, edata[2], vbuffer[capsule_offset + 2], buffer[2], &loadcount[2], GraphAlgoClass, globalparams);
					if(E[3] == INVALIDDATA){ enx[3] = false; }
					PROCESSP0_processvector(enx[3], edata[3].value, edata[3], vbuffer[capsule_offset + 3], buffer[3], &loadcount[3], GraphAlgoClass, globalparams);
					if(E[4] == INVALIDDATA){ enx[4] = false; }
					PROCESSP0_processvector(enx[4], edata[4].value, edata[4], vbuffer[capsule_offset + 4], buffer[4], &loadcount[4], GraphAlgoClass, globalparams);
					if(E[5] == INVALIDDATA){ enx[5] = false; }
					PROCESSP0_processvector(enx[5], edata[5].value, edata[5], vbuffer[capsule_offset + 5], buffer[5], &loadcount[5], GraphAlgoClass, globalparams);
					if(E[6] == INVALIDDATA){ enx[6] = false; }
					PROCESSP0_processvector(enx[6], edata[6].value, edata[6], vbuffer[capsule_offset + 6], buffer[6], &loadcount[6], GraphAlgoClass, globalparams);
					if(E[7] == INVALIDDATA){ enx[7] = false; }
					PROCESSP0_processvector(enx[7], edata[7].value, edata[7], vbuffer[capsule_offset + 7], buffer[7], &loadcount[7], GraphAlgoClass, globalparams);
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
	
	fetchmessage.chunksize_kvs = chunk_size * 2; // loadcount; // CRITICAL FIXME
	// cout<<"classname__processedges2 SUCCESSFULL HERE...."<<endl;
	// exit(EXIT_SUCCESS); ////
	return fetchmessage;
}





