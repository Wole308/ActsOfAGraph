void acts_all::REDUCEP0_reducevector(bool enx, unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], buffer_type destoffset, unsigned int upperlimit, unsigned int * memory, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS INLINE
	// #pragma HLS PIPELINE II=3
	
	keyvalue_t mykeyvalue = UTILP0_GETKV(kvdata);
	vertex_t loc = ((mykeyvalue.key - upperlimit) - col) >> globalparams.ACTSPARAMS_POW_PARTITIONS;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"REDUCEP0_reducevector:: col: "<<col<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", mykeyvalue.value: "<<mykeyvalue.value<<endl;
	#endif
	
	bool en = true;
	if(mykeyvalue.key == UTILP0_GETK(INVALIDDATA) || mykeyvalue.value == UTILP0_GETV(INVALIDDATA) || mykeyvalue.key == MAXVDATA || mykeyvalue.value == MAXVDATA){ en = false; } // REMOVEME?????????????????????????????
	if(mykeyvalue.key == 0 && mykeyvalue.value == 0){ en = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true){ cout<<"$$$ REDUCE VECTOR:: REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<", partition: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID) / (globalparams.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE)<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<endl; }															
	#endif 
	
	if(loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_SHRINK_RATIO) && en == true){
		#ifdef ENABLE_PERFECTACCURACY
		if(true){ cout<<"REDUCEP0_reducevector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }
		
	vmdata_t vmdata; vmdata.vmask = 0;
	vmdata_t vmdata2; vmdata2.vmask = 0;
	unsigned int bufferoffset_kvs = 0; 
	keyvalue_vbuffer_t bits_vector = 0;
	keyvalue_vbuffer_t bits2_vector = 0;
	
	#ifdef CONFIG_USEURAM_PACKBYTWOS
	loc = loc / 2; 
	#endif 
	if(en == true){ if(loc == *memory){ loc = (loc + 1) % 8; } *memory = loc; } // CRITICAL FIXME.
	
	// read 
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 113::1", bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VDESTDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
	#endif
	
	// read-special (cf)
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == CF){ bits2_vector = vbuffer[bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA2 + loc]; } // CUSTOMPROVISION #ifdef CF_ALGORITHM 
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == CF){ vmdata2 = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(bits2_vector); }
	
	// read-special (hits)
	unsigned int src_dest_edge = MEMCAP0_READEDIR(kvdata.key);
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == HITS){ if(src_dest_edge == 0){ loc = loc*2; } else{ loc = loc*2 + 1; }} // CUSTOMPROVISION #ifdef HITS_ALGORITHM 
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 114::1", bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1 + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VDESTDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
	#endif
	
	// read (general)
	if(en == true){ bits_vector = vbuffer[bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1 + (loc / VDATA_SHRINK_RATIO)]; }
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(loc, bits_vector); }
			#else 
				if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(bits_vector); }
					#endif
					
	// reduce 
	value_t new_vprop = REDUCEP0_reducefunc(vmdata.vdata, vmdata2.vdata, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	if(en == true && new_vprop != vmdata.vdata){ vmdata.vmask = 1; } else { vmdata.vmask = 0; } // NEWCHANGE.
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true && new_vprop != vmdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE MASK SEEN AT: loc: "<<loc<<", vmdata.vdata: "<<vmdata.vdata<<", vmdata.vmask: "<<vmdata.vmask<<endl; }
	if(en == true && new_vprop != vmdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP0_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
	
	// write
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK2(loc, vbuffer, bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, vmdata.vmask); }
			#else 
				if(en == true){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(loc, vbuffer, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, vmdata.vmask); }
					#endif
 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(globalparams.ACTSPARAMS_INSTID, 1);
	if(en == true){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(globalparams.ACTSPARAMS_INSTID, 1); }
	#endif
	return;
}

void acts_all::REDUCEP0_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth){
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

void acts_all::REDUCEP0_RearrangeLayoutV(unsigned int s, keyvalue_buffer_t vdata[VECTOR_SIZE], keyvalue_buffer_t vdata2[VECTOR_SIZE]){
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

void acts_all::REDUCEP0_RearrangeLayoutEn(unsigned int s, bool en[VECTOR_SIZE], bool en2[VECTOR_SIZE]){
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

void acts_all::REDUCEP0_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	// globalparams.ACTSPARAMS_NUM_PARTITIONS replacedby 16
	if(enable == OFF){ return; }
	
	keyvalue_buffer_t kvdata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata complete
	keyvalue_buffer_t kvdata2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata2 complete
	unsigned int depths[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	bool enx2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx2 complete
	unsigned int d_kvs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=d_kvs complete
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=localcapsule_kvs complete
	unsigned int memory[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=memory complete
	
	keyvalue_t invalid_dataa; invalid_dataa.key = INVALIDDATA; invalid_dataa.value = INVALIDDATA;
	keyvalue_buffer_t invalid_data = UTILP0_GETKV(invalid_dataa);

	unsigned int chunk_size = 0; for(partition_type p=0; p<16; p++){ chunk_size += localcapsule[p].value / VECTOR_SIZE; } // CRITICAL FIXME.
	unsigned int modelsz = chunk_size / 16;
	for(unsigned int p=0; p<16; p++){ localcapsule[p].key = (p * modelsz) * VECTOR_SIZE; localcapsule[p].value = modelsz * VECTOR_SIZE; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	
	buffer_type maxsize_kvs[2]; maxsize_kvs[0] = 0; maxsize_kvs[1] = 0;
	unsigned int _poff = 0;
	REDUCEBUFFERPARTITIONS_LOOP1: for(unsigned int cid=0; cid<2; cid++){ // NB: is accessed 8kv per time, so we need 2 fetches'
		REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<16/2; p++){ 
		#pragma HLS PIPELINE II=1
			unsigned int ssize_kvs = localcapsule[_poff + p].value / VECTOR_SIZE;
			if(maxsize_kvs[cid] < ssize_kvs){ maxsize_kvs[cid] = ssize_kvs; }
		}
		_poff += 16/2;
	}
	buffer_type height_kvs = (localcapsule[16-1].key + localcapsule[16-1].value) / VECTOR_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"reduceupdates2: maxsize_kvs[0]: "<<maxsize_kvs[0]<<", maxsize_kvs[1]: "<<maxsize_kvs[1]<<", height_kvs: "<<height_kvs<<endl;
	#endif
		
	for(partition_type p=0; p<16; p++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"reduceupdates2: localcapsule["<<p<<"].key: "<<localcapsule[p].key<<", localcapsule["<<p<<"].value: "<<localcapsule[p].value<<endl;
		#endif
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	}
	
	REDUCEBUFFERPARTITIONS_MAINLOOP2: for(buffer_type capsule_offset=0; capsule_offset<16; capsule_offset+=VECTOR_SIZE){
		unsigned int mmaxsz_kvs = maxsize_kvs[capsule_offset / VECTOR_SIZE];	
		REDUCEBUFFERPARTITIONS_MAINLOOP2B: for(unsigned int r=0; r<VECTOR_SIZE; r++){
			REDUCEBUFFERPARTITIONS_MAINLOOP2C: for(buffer_type i=0; i<mmaxsz_kvs; i++){ 
			#pragma HLS PIPELINE II=2
			#pragma HLS dependence variable=vbuffer inter false
				// get layout
				REDUCEP0_GetXYLayoutV(r, depths, 0);
			
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
				d_kvs[0] = localcapsule_kvs[tdepth0].key + i; 	
				unsigned int tdepth1 = capsule_offset + depths[1];
				d_kvs[1] = localcapsule_kvs[tdepth1].key + i; 	
				unsigned int tdepth2 = capsule_offset + depths[2];
				d_kvs[2] = localcapsule_kvs[tdepth2].key + i; 	
				unsigned int tdepth3 = capsule_offset + depths[3];
				d_kvs[3] = localcapsule_kvs[tdepth3].key + i; 	
				unsigned int tdepth4 = capsule_offset + depths[4];
				d_kvs[4] = localcapsule_kvs[tdepth4].key + i; 	
				unsigned int tdepth5 = capsule_offset + depths[5];
				d_kvs[5] = localcapsule_kvs[tdepth5].key + i; 	
				unsigned int tdepth6 = capsule_offset + depths[6];
				d_kvs[6] = localcapsule_kvs[tdepth6].key + i; 	
				unsigned int tdepth7 = capsule_offset + depths[7];
				d_kvs[7] = localcapsule_kvs[tdepth7].key + i; 	
 	
 // FIXME.
				if(i < localcapsule_kvs[tdepth0].value){ enx[0] = true; kvdata[0] = buffer[0][d_kvs[0]]; } else { enx[0] = false; kvdata[0] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth1].value){ enx[1] = true; kvdata[1] = buffer[1][d_kvs[1]]; } else { enx[1] = false; kvdata[1] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth2].value){ enx[2] = true; kvdata[2] = buffer[2][d_kvs[2]]; } else { enx[2] = false; kvdata[2] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth3].value){ enx[3] = true; kvdata[3] = buffer[3][d_kvs[3]]; } else { enx[3] = false; kvdata[3] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth4].value){ enx[4] = true; kvdata[4] = buffer[4][d_kvs[4]]; } else { enx[4] = false; kvdata[4] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth5].value){ enx[5] = true; kvdata[5] = buffer[5][d_kvs[5]]; } else { enx[5] = false; kvdata[5] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth6].value){ enx[6] = true; kvdata[6] = buffer[6][d_kvs[6]]; } else { enx[6] = false; kvdata[6] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth7].value){ enx[7] = true; kvdata[7] = buffer[7][d_kvs[7]]; } else { enx[7] = false; kvdata[7] = invalid_data; }	
				
				// re-arrange
				REDUCEP0_RearrangeLayoutV(r, kvdata, kvdata2);
				REDUCEP0_RearrangeLayoutEn(r, enx, enx2);
			
				// reduce 
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP0_reducevector(enx2[0], 0, kvdata2[0], vbuffer[capsule_offset + 0], 0, sweepparams.upperlimit, &memory[0], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP0_reducevector(enx2[0], 0, kvdata2[0], vbuffer[0 + 0], 0, sweepparams.upperlimit, &memory[0], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP0_reducevector(enx2[1], 1, kvdata2[1], vbuffer[capsule_offset + 1], 0, sweepparams.upperlimit, &memory[1], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP0_reducevector(enx2[1], 1, kvdata2[1], vbuffer[0 + 1], 0, sweepparams.upperlimit, &memory[1], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP0_reducevector(enx2[2], 2, kvdata2[2], vbuffer[capsule_offset + 2], 0, sweepparams.upperlimit, &memory[2], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP0_reducevector(enx2[2], 2, kvdata2[2], vbuffer[0 + 2], 0, sweepparams.upperlimit, &memory[2], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP0_reducevector(enx2[3], 3, kvdata2[3], vbuffer[capsule_offset + 3], 0, sweepparams.upperlimit, &memory[3], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP0_reducevector(enx2[3], 3, kvdata2[3], vbuffer[0 + 3], 0, sweepparams.upperlimit, &memory[3], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP0_reducevector(enx2[4], 4, kvdata2[4], vbuffer[capsule_offset + 4], 0, sweepparams.upperlimit, &memory[4], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP0_reducevector(enx2[4], 4, kvdata2[4], vbuffer[0 + 4], 0, sweepparams.upperlimit, &memory[4], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP0_reducevector(enx2[5], 5, kvdata2[5], vbuffer[capsule_offset + 5], 0, sweepparams.upperlimit, &memory[5], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP0_reducevector(enx2[5], 5, kvdata2[5], vbuffer[0 + 5], 0, sweepparams.upperlimit, &memory[5], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP0_reducevector(enx2[6], 6, kvdata2[6], vbuffer[capsule_offset + 6], 0, sweepparams.upperlimit, &memory[6], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP0_reducevector(enx2[6], 6, kvdata2[6], vbuffer[0 + 6], 0, sweepparams.upperlimit, &memory[6], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP0_reducevector(enx2[7], 7, kvdata2[7], vbuffer[capsule_offset + 7], 0, sweepparams.upperlimit, &memory[7], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP0_reducevector(enx2[7], 7, kvdata2[7], vbuffer[0 + 7], 0, sweepparams.upperlimit, &memory[7], sweepparams, globalparams);
			}
		}
	}
	// exit(EXIT_SUCCESS);
	return;
}

void acts_all::REDUCEP0_reduceandbuffer2(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (MAX_DESTBLOCKRAM_SIZE / (MAX_NUM_PARTITIONS / 2));

	keyvalue_buffer_t kvdata0;
	keyvalue_buffer_t kvdata1;
	keyvalue_buffer_t kvdata2;
	keyvalue_buffer_t kvdata3;
	keyvalue_buffer_t kvdata4;
	keyvalue_buffer_t kvdata5;
	keyvalue_buffer_t kvdata6;
	keyvalue_buffer_t kvdata7;
	keyvalue_buffer_t kvdata8;
	keyvalue_buffer_t kvdata9;
	keyvalue_buffer_t kvdata10;
	keyvalue_buffer_t kvdata11;
	keyvalue_buffer_t kvdata12;
	keyvalue_buffer_t kvdata13;
	keyvalue_buffer_t kvdata14;
	keyvalue_buffer_t kvdata15;
	buffer_type bramoffset_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=bramoffset_kvs complete
	buffer_type size_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=size_kvs complete
	unsigned int upperlimits[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=upperlimits complete
	unsigned int memory[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=memory complete
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	
	REDUCEBUFFERPARTITIONS2_LOOP1: for(unsigned int it=0; it<globalparams.ACTSPARAMS_NUM_PARTITIONS; it+=globalparams.ACTSPARAMS_NUM_PARTITIONS/2){
		buffer_type maxsize_kvs = 0;
		REDUCEBUFFERPARTITIONS2_LOOP1B: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			bramoffset_kvs[p] = localcapsule[it+p].key / VECTOR_SIZE;
			size_kvs[p] = localcapsule[it+p].value / VECTOR_SIZE;
			if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
		}
		
		REDUCEBUFFERPARTITIONS2_MAINLOOP1D: for(buffer_type i=0; i<maxsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#pragma HLS PIPELINE II=48 // 16, 32*, 48, FIXME.
			for(vector_type v=0; v<VECTOR_SIZE; v++){
				kvdata0 = buffer[v][bramoffset_kvs[0] + i]; 	
				kvdata1 = buffer[v][bramoffset_kvs[1] + i]; 	
				kvdata2 = buffer[v][bramoffset_kvs[2] + i]; 	
				kvdata3 = buffer[v][bramoffset_kvs[3] + i]; 	
				kvdata4 = buffer[v][bramoffset_kvs[4] + i]; 	
				kvdata5 = buffer[v][bramoffset_kvs[5] + i]; 	
				kvdata6 = buffer[v][bramoffset_kvs[6] + i]; 	
				kvdata7 = buffer[v][bramoffset_kvs[7] + i]; 	
				
	
				// if(i< size_kvs[0]){ REDUCEP0_reducevector(ON, it+0, kvdata0, vbuffer[it+0], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[0]){ REDUCEP0_reducevector(ON, 0+0, kvdata0, vbuffer[0+0], 0, sweepparams.upperlimit, &memory[0], sweepparams, globalparams); }
	
				// if(i< size_kvs[1]){ REDUCEP0_reducevector(ON, it+1, kvdata1, vbuffer[it+1], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[1]){ REDUCEP0_reducevector(ON, 0+1, kvdata1, vbuffer[0+1], 0, sweepparams.upperlimit, &memory[1], sweepparams, globalparams); }
	
				// if(i< size_kvs[2]){ REDUCEP0_reducevector(ON, it+2, kvdata2, vbuffer[it+2], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[2]){ REDUCEP0_reducevector(ON, 0+2, kvdata2, vbuffer[0+2], 0, sweepparams.upperlimit, &memory[2], sweepparams, globalparams); }
	
				// if(i< size_kvs[3]){ REDUCEP0_reducevector(ON, it+3, kvdata3, vbuffer[it+3], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[3]){ REDUCEP0_reducevector(ON, 0+3, kvdata3, vbuffer[0+3], 0, sweepparams.upperlimit, &memory[3], sweepparams, globalparams); }
	
				// if(i< size_kvs[4]){ REDUCEP0_reducevector(ON, it+4, kvdata4, vbuffer[it+4], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[4]){ REDUCEP0_reducevector(ON, 0+4, kvdata4, vbuffer[0+4], 0, sweepparams.upperlimit, &memory[4], sweepparams, globalparams); }
	
				// if(i< size_kvs[5]){ REDUCEP0_reducevector(ON, it+5, kvdata5, vbuffer[it+5], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[5]){ REDUCEP0_reducevector(ON, 0+5, kvdata5, vbuffer[0+5], 0, sweepparams.upperlimit, &memory[5], sweepparams, globalparams); }
	
				// if(i< size_kvs[6]){ REDUCEP0_reducevector(ON, it+6, kvdata6, vbuffer[it+6], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[6]){ REDUCEP0_reducevector(ON, 0+6, kvdata6, vbuffer[0+6], 0, sweepparams.upperlimit, &memory[6], sweepparams, globalparams); }
	
				// if(i< size_kvs[7]){ REDUCEP0_reducevector(ON, it+7, kvdata7, vbuffer[it+7], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[7]){ REDUCEP0_reducevector(ON, 0+7, kvdata7, vbuffer[0+7], 0, sweepparams.upperlimit, &memory[7], sweepparams, globalparams); }
	
			}
		}
	}
	return;
}

void acts_all::REDUCEP0_reduceandbuffer3(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	if(enable == OFF){ return; }
	
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=localcapsule_kvs complete
	unsigned int totalsum_values_kvs = 0;
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"ACTSreduceandbuffer3: localcapsule["<<p<<"].key: "<<localcapsule[p].key<<", localcapsule["<<p<<"].value: "<<localcapsule[p].value<<endl;
		#endif
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	}
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ totalsum_values_kvs += localcapsule_kvs[p].value; }
	// cout<<"ACTSreduceandbuffer3: totalsum_values_kvs: "<<totalsum_values_kvs<<endl;
	// exit(EXIT_SUCCESS); //
	
	REDUCEBUFFERPARTITIONS3_MAINLOOP2C: for(buffer_type i=0; i<totalsum_values_kvs; i++){
	#pragma HLS PIPELINE II=2
		vbuffer[0][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP0_GETKV(buffer[0][i]).value;
		vbuffer[1][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP0_GETKV(buffer[1][i]).value;
		vbuffer[2][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP0_GETKV(buffer[2][i]).value;
		vbuffer[3][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP0_GETKV(buffer[3][i]).value;
		vbuffer[4][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP0_GETKV(buffer[4][i]).value;
		vbuffer[5][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP0_GETKV(buffer[5][i]).value;
		vbuffer[6][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP0_GETKV(buffer[6][i]).value;
		vbuffer[7][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP0_GETKV(buffer[7][i]).value;
		// cout<<"reduce seen......"<<endl;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsreduced(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE);
		actsutilityobj->globalstats_reduce_countvalidkvsreduced(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE);
		#endif
	}
	return;
}

void acts_all::REDUCEP0_TRADreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	if(enable == OFF){ return; }
	
	keyvalue_buffer_t kvdata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata complete
	keyvalue_buffer_t kvdata2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata2 complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	unsigned int memory[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=memory complete
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	
	REDUCEBUFFERPARTITIONS_LOOP2A: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		REDUCEBUFFERPARTITIONS_LOOP2B: for(buffer_type i=localcapsule[p].key; i<localcapsule[p].key + localcapsule[p].value; i++){
		#pragma HLS PIPELINE II=4
			// read 
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", i / VECTOR_SIZE, MAX_DESTBLOCKRAM_SIZE, i, MAX_DESTBLOCKRAM_SIZE, NAp);
			#endif
			keyvalue_buffer_t kvdata = buffer[i % VECTOR_SIZE][i / VECTOR_SIZE];

			// reduce 
			REDUCEP0_reducevector(ON, p, kvdata, vbuffer[p], 0, sweepparams.upperlimit, &memory[p], sweepparams, globalparams);
		}
	}

	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS); ////
	return;
}

void acts_all::REDUCEP0_reduceandbuffer_base(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	return REDUCEP0_reduceandbuffer(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
	// return REDUCEP0_reduceandbuffer2(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
	// return REDUCEP0_reduceandbuffer3(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
}
