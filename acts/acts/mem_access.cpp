// -------------------- key values -------------------- //
fetchmessage_t acts_all::MEMACCESSP0_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	buffer_type chunk_size = UTILP0_getchunksize_kvs(size_kvs, travstate, 0);
	
	fetchmessage.chunksize_kvs = chunk_size;
	fetchmessage.nextoffset_kvs = NAp;
	
	READKEYVALUES1_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0;
		keyvalue_t mykeyvalue1;
		keyvalue_t mykeyvalue2;
		keyvalue_t mykeyvalue3;
		keyvalue_t mykeyvalue4;
		keyvalue_t mykeyvalue5;
		keyvalue_t mykeyvalue6;
		keyvalue_t mykeyvalue7;
		
		#ifdef _WIDEWORD
		mykeyvalue0.key = kvdram[offset_kvs + i].range(31, 0);
		mykeyvalue0.value = kvdram[offset_kvs + i].range(63, 32);
		mykeyvalue1.key = kvdram[offset_kvs + i].range(95, 64);
		mykeyvalue1.value = kvdram[offset_kvs + i].range(127, 96);
		mykeyvalue2.key = kvdram[offset_kvs + i].range(159, 128);
		mykeyvalue2.value = kvdram[offset_kvs + i].range(191, 160);
		mykeyvalue3.key = kvdram[offset_kvs + i].range(223, 192);
		mykeyvalue3.value = kvdram[offset_kvs + i].range(255, 224);
		mykeyvalue4.key = kvdram[offset_kvs + i].range(287, 256);
		mykeyvalue4.value = kvdram[offset_kvs + i].range(319, 288);
		mykeyvalue5.key = kvdram[offset_kvs + i].range(351, 320);
		mykeyvalue5.value = kvdram[offset_kvs + i].range(383, 352);
		mykeyvalue6.key = kvdram[offset_kvs + i].range(415, 384);
		mykeyvalue6.value = kvdram[offset_kvs + i].range(447, 416);
		mykeyvalue7.key = kvdram[offset_kvs + i].range(479, 448);
		mykeyvalue7.value = kvdram[offset_kvs + i].range(511, 480);
		#else 
		mykeyvalue0.key = kvdram[offset_kvs + i].data[0].key; 
		mykeyvalue0.value = kvdram[offset_kvs + i].data[0].value; 
		mykeyvalue1.key = kvdram[offset_kvs + i].data[1].key; 
		mykeyvalue1.value = kvdram[offset_kvs + i].data[1].value; 
		mykeyvalue2.key = kvdram[offset_kvs + i].data[2].key; 
		mykeyvalue2.value = kvdram[offset_kvs + i].data[2].value; 
		mykeyvalue3.key = kvdram[offset_kvs + i].data[3].key; 
		mykeyvalue3.value = kvdram[offset_kvs + i].data[3].value; 
		mykeyvalue4.key = kvdram[offset_kvs + i].data[4].key; 
		mykeyvalue4.value = kvdram[offset_kvs + i].data[4].value; 
		mykeyvalue5.key = kvdram[offset_kvs + i].data[5].key; 
		mykeyvalue5.value = kvdram[offset_kvs + i].data[5].value; 
		mykeyvalue6.key = kvdram[offset_kvs + i].data[6].key; 
		mykeyvalue6.value = kvdram[offset_kvs + i].data[6].value; 
		mykeyvalue7.key = kvdram[offset_kvs + i].data[7].key; 
		mykeyvalue7.value = kvdram[offset_kvs + i].data[7].value; 
		#endif 
		
		buffer[0][i] = UTILP0_GETKV(mykeyvalue0);
		buffer[1][i] = UTILP0_GETKV(mykeyvalue1);
		buffer[2][i] = UTILP0_GETKV(mykeyvalue2);
		buffer[3][i] = UTILP0_GETKV(mykeyvalue3);
		buffer[4][i] = UTILP0_GETKV(mykeyvalue4);
		buffer[5][i] = UTILP0_GETKV(mykeyvalue5);
		buffer[6][i] = UTILP0_GETKV(mykeyvalue6);
		buffer[7][i] = UTILP0_GETKV(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues read: offset_kvs: "<<offset_kvs<<", size_kvs: "<<size_kvs<<", chunk_size: "<<chunk_size<<endl;
	#endif
	return fetchmessage;
}

void acts_all::MEMACCESSP0_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_destpartitionsz = DESTBLOCKRAM_SIZE / NUM_PARTITIONS;
	
	// actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 37--", (keyvalue_t *)globalcapsule, NUM_PARTITIONS); 
			// exit(EXIT_SUCCESS); 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("savekeyvalues::localcapsule", (keyvalue_t *)localcapsule, MAX_NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues::localcapsule", (keyvalue_t *)localcapsule, MAX_NUM_PARTITIONS);
	// actsutilityobj->scankeyvalues("savekeyvalues::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.SIZE_BATCHRANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	// exit(EXIT_SUCCESS); 
	
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR_SIZE;
		buffer_type size_kvs = UTILP0_getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("savekeyvalues 23", bramoffset_kvs + size_kvs, DESTBLOCKRAM_SIZE + 1, p, NAp, NAp);
		actsutilityobj->checkoutofbounds("savekeyvalues 25", ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE), globalparams.SIZE_KVDRAM + 1, p, NAp, NAp);
		#endif
		if(realsize_kvs > 0){
			SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
			#pragma HLS PIPELINE II=1
				keyvalue_t mykeyvalue0 = UTILP0_GETKV(buffer[0][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue1 = UTILP0_GETKV(buffer[1][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue2 = UTILP0_GETKV(buffer[2][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue3 = UTILP0_GETKV(buffer[3][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue4 = UTILP0_GETKV(buffer[4][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue5 = UTILP0_GETKV(buffer[5][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue6 = UTILP0_GETKV(buffer[6][bramoffset_kvs + i]);
				keyvalue_t mykeyvalue7 = UTILP0_GETKV(buffer[7][bramoffset_kvs + i]);
				
				#ifdef _WIDEWORD
				kvdram[dramoffset_kvs + i].range(31, 0) = mykeyvalue0.key; 
				kvdram[dramoffset_kvs + i].range(63, 32) = mykeyvalue0.value; 
				kvdram[dramoffset_kvs + i].range(95, 64) = mykeyvalue1.key; 
				kvdram[dramoffset_kvs + i].range(127, 96) = mykeyvalue1.value; 
				kvdram[dramoffset_kvs + i].range(159, 128) = mykeyvalue2.key; 
				kvdram[dramoffset_kvs + i].range(191, 160) = mykeyvalue2.value; 
				kvdram[dramoffset_kvs + i].range(223, 192) = mykeyvalue3.key; 
				kvdram[dramoffset_kvs + i].range(255, 224) = mykeyvalue3.value; 
				kvdram[dramoffset_kvs + i].range(287, 256) = mykeyvalue4.key; 
				kvdram[dramoffset_kvs + i].range(319, 288) = mykeyvalue4.value; 
				kvdram[dramoffset_kvs + i].range(351, 320) = mykeyvalue5.key; 
				kvdram[dramoffset_kvs + i].range(383, 352) = mykeyvalue5.value; 
				kvdram[dramoffset_kvs + i].range(415, 384) = mykeyvalue6.key; 
				kvdram[dramoffset_kvs + i].range(447, 416) = mykeyvalue6.value; 
				kvdram[dramoffset_kvs + i].range(479, 448) = mykeyvalue7.key; 
				kvdram[dramoffset_kvs + i].range(511, 480) = mykeyvalue7.value; 
				#else
				kvdram[dramoffset_kvs + i].data[0].key = mykeyvalue0.key; 
				kvdram[dramoffset_kvs + i].data[0].value = mykeyvalue0.value; 
				kvdram[dramoffset_kvs + i].data[1].key = mykeyvalue1.key; 
				kvdram[dramoffset_kvs + i].data[1].value = mykeyvalue1.value; 
				kvdram[dramoffset_kvs + i].data[2].key = mykeyvalue2.key; 
				kvdram[dramoffset_kvs + i].data[2].value = mykeyvalue2.value; 
				kvdram[dramoffset_kvs + i].data[3].key = mykeyvalue3.key; 
				kvdram[dramoffset_kvs + i].data[3].value = mykeyvalue3.value; 
				kvdram[dramoffset_kvs + i].data[4].key = mykeyvalue4.key; 
				kvdram[dramoffset_kvs + i].data[4].value = mykeyvalue4.value; 
				kvdram[dramoffset_kvs + i].data[5].key = mykeyvalue5.key; 
				kvdram[dramoffset_kvs + i].data[5].value = mykeyvalue5.value; 
				kvdram[dramoffset_kvs + i].data[6].key = mykeyvalue6.key; 
				kvdram[dramoffset_kvs + i].data[6].value = mykeyvalue6.value; 
				kvdram[dramoffset_kvs + i].data[7].key = mykeyvalue7.key; 
				kvdram[dramoffset_kvs + i].data[7].value = mykeyvalue7.value; 
				#endif
			}
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten(realsize_kvs * VECTOR_SIZE);
			#endif
		}
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }
	
	// actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 34--", (keyvalue_t *)globalcapsule, NUM_PARTITIONS); 
	// exit(EXIT_SUCCESS);

	#if defined(ENABLE_PERFECTACCURACY) && defined(_DEBUGMODE_CHECKS2)
	for(unsigned int i=0; i<NUM_PARTITIONS-1; i++){ 
		if(globalcapsule[i].key + globalcapsule[i].value >= globalcapsule[i+1].key && globalcapsule[i].value > 0){ 
			cout<<"savekeyvalues::globalcapsule 33. ERROR. out of bounds. (globalcapsule["<<i<<"].key("<<globalcapsule[i].key<<") + globalcapsule["<<i<<"].value("<<globalcapsule[i].value<<") >= globalcapsule["<<i+1<<"].key("<<globalcapsule[i+1].key<<")) printing and exiting..."<<endl; 
			actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 34", (keyvalue_t *)globalcapsule, NUM_PARTITIONS); 
			exit(EXIT_FAILURE); 
		}
	}
	if(globalcapsule[NUM_PARTITIONS-1].key + globalcapsule[NUM_PARTITIONS-1].value >= globalparams.SIZE_KVDRAM){
		cout<<"savekeyvalues::globalcapsule 36. ERROR. out of bounds. (globalcapsule["<<NUM_PARTITIONS-1<<"].key("<<globalcapsule[NUM_PARTITIONS-1].key<<") + globalcapsule["<<NUM_PARTITIONS-1<<"].value("<<globalcapsule[NUM_PARTITIONS-1].value<<") >= globalparams.SIZE_KVDRAM("<<globalparams.SIZE_KVDRAM<<")) printing and exiting..."<<endl; 
		actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 37", (keyvalue_t *)globalcapsule, NUM_PARTITIONS); 
		exit(EXIT_FAILURE); 
	}
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"savekeyvalues:: keyvalues saved: offset_kvs from: "<<globalbaseaddress_kvs + ((globalcapsule[0].key + globalcapsule[0].value) / VECTOR_SIZE)<<endl;
	actsutilityobj->printkeyvalues("actsutility::savekeyvalues: globalcapsule.", (keyvalue_t *)globalcapsule, NUM_PARTITIONS);
	#endif
	return;
}

// -------------------- vdata -------------------- //
void acts_all::MEMACCESSP0_GetXYLayoutV(unsigned int s, vmdata_t vdata[VECTOR2_SIZE], vmdata_t vdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth){
	unsigned int s_ = s % VECTOR2_SIZE;
	unsigned int depths_tmp[VECTOR2_SIZE];
	
 if(s_==0){ 
		vdata2[0] = vdata[0]; 
		vdata2[1] = vdata[1]; 
		vdata2[2] = vdata[2]; 
		vdata2[3] = vdata[3]; 
		vdata2[4] = vdata[4]; 
		vdata2[5] = vdata[5]; 
		vdata2[6] = vdata[6]; 
		vdata2[7] = vdata[7]; 
		vdata2[8] = vdata[8]; 
		vdata2[9] = vdata[9]; 
		vdata2[10] = vdata[10]; 
		vdata2[11] = vdata[11]; 
		vdata2[12] = vdata[12]; 
		vdata2[13] = vdata[13]; 
		vdata2[14] = vdata[14]; 
		vdata2[15] = vdata[15]; 
		
		depths_tmp[0] = 0; 
		depths_tmp[1] = 1; 
		depths_tmp[2] = 2; 
		depths_tmp[3] = 3; 
		depths_tmp[4] = 4; 
		depths_tmp[5] = 5; 
		depths_tmp[6] = 6; 
		depths_tmp[7] = 7; 
		depths_tmp[8] = 8; 
		depths_tmp[9] = 9; 
		depths_tmp[10] = 10; 
		depths_tmp[11] = 11; 
		depths_tmp[12] = 12; 
		depths_tmp[13] = 13; 
		depths_tmp[14] = 14; 
		depths_tmp[15] = 15; 
	}
else if(s_==1){ 
		vdata2[0] = vdata[15]; 
		vdata2[1] = vdata[0]; 
		vdata2[2] = vdata[1]; 
		vdata2[3] = vdata[2]; 
		vdata2[4] = vdata[3]; 
		vdata2[5] = vdata[4]; 
		vdata2[6] = vdata[5]; 
		vdata2[7] = vdata[6]; 
		vdata2[8] = vdata[7]; 
		vdata2[9] = vdata[8]; 
		vdata2[10] = vdata[9]; 
		vdata2[11] = vdata[10]; 
		vdata2[12] = vdata[11]; 
		vdata2[13] = vdata[12]; 
		vdata2[14] = vdata[13]; 
		vdata2[15] = vdata[14]; 
		
		depths_tmp[0] = 15; 
		depths_tmp[1] = 0; 
		depths_tmp[2] = 1; 
		depths_tmp[3] = 2; 
		depths_tmp[4] = 3; 
		depths_tmp[5] = 4; 
		depths_tmp[6] = 5; 
		depths_tmp[7] = 6; 
		depths_tmp[8] = 7; 
		depths_tmp[9] = 8; 
		depths_tmp[10] = 9; 
		depths_tmp[11] = 10; 
		depths_tmp[12] = 11; 
		depths_tmp[13] = 12; 
		depths_tmp[14] = 13; 
		depths_tmp[15] = 14; 
	}
else if(s_==2){ 
		vdata2[0] = vdata[14]; 
		vdata2[1] = vdata[15]; 
		vdata2[2] = vdata[0]; 
		vdata2[3] = vdata[1]; 
		vdata2[4] = vdata[2]; 
		vdata2[5] = vdata[3]; 
		vdata2[6] = vdata[4]; 
		vdata2[7] = vdata[5]; 
		vdata2[8] = vdata[6]; 
		vdata2[9] = vdata[7]; 
		vdata2[10] = vdata[8]; 
		vdata2[11] = vdata[9]; 
		vdata2[12] = vdata[10]; 
		vdata2[13] = vdata[11]; 
		vdata2[14] = vdata[12]; 
		vdata2[15] = vdata[13]; 
		
		depths_tmp[0] = 14; 
		depths_tmp[1] = 15; 
		depths_tmp[2] = 0; 
		depths_tmp[3] = 1; 
		depths_tmp[4] = 2; 
		depths_tmp[5] = 3; 
		depths_tmp[6] = 4; 
		depths_tmp[7] = 5; 
		depths_tmp[8] = 6; 
		depths_tmp[9] = 7; 
		depths_tmp[10] = 8; 
		depths_tmp[11] = 9; 
		depths_tmp[12] = 10; 
		depths_tmp[13] = 11; 
		depths_tmp[14] = 12; 
		depths_tmp[15] = 13; 
	}
else if(s_==3){ 
		vdata2[0] = vdata[13]; 
		vdata2[1] = vdata[14]; 
		vdata2[2] = vdata[15]; 
		vdata2[3] = vdata[0]; 
		vdata2[4] = vdata[1]; 
		vdata2[5] = vdata[2]; 
		vdata2[6] = vdata[3]; 
		vdata2[7] = vdata[4]; 
		vdata2[8] = vdata[5]; 
		vdata2[9] = vdata[6]; 
		vdata2[10] = vdata[7]; 
		vdata2[11] = vdata[8]; 
		vdata2[12] = vdata[9]; 
		vdata2[13] = vdata[10]; 
		vdata2[14] = vdata[11]; 
		vdata2[15] = vdata[12]; 
		
		depths_tmp[0] = 13; 
		depths_tmp[1] = 14; 
		depths_tmp[2] = 15; 
		depths_tmp[3] = 0; 
		depths_tmp[4] = 1; 
		depths_tmp[5] = 2; 
		depths_tmp[6] = 3; 
		depths_tmp[7] = 4; 
		depths_tmp[8] = 5; 
		depths_tmp[9] = 6; 
		depths_tmp[10] = 7; 
		depths_tmp[11] = 8; 
		depths_tmp[12] = 9; 
		depths_tmp[13] = 10; 
		depths_tmp[14] = 11; 
		depths_tmp[15] = 12; 
	}
else if(s_==4){ 
		vdata2[0] = vdata[12]; 
		vdata2[1] = vdata[13]; 
		vdata2[2] = vdata[14]; 
		vdata2[3] = vdata[15]; 
		vdata2[4] = vdata[0]; 
		vdata2[5] = vdata[1]; 
		vdata2[6] = vdata[2]; 
		vdata2[7] = vdata[3]; 
		vdata2[8] = vdata[4]; 
		vdata2[9] = vdata[5]; 
		vdata2[10] = vdata[6]; 
		vdata2[11] = vdata[7]; 
		vdata2[12] = vdata[8]; 
		vdata2[13] = vdata[9]; 
		vdata2[14] = vdata[10]; 
		vdata2[15] = vdata[11]; 
		
		depths_tmp[0] = 12; 
		depths_tmp[1] = 13; 
		depths_tmp[2] = 14; 
		depths_tmp[3] = 15; 
		depths_tmp[4] = 0; 
		depths_tmp[5] = 1; 
		depths_tmp[6] = 2; 
		depths_tmp[7] = 3; 
		depths_tmp[8] = 4; 
		depths_tmp[9] = 5; 
		depths_tmp[10] = 6; 
		depths_tmp[11] = 7; 
		depths_tmp[12] = 8; 
		depths_tmp[13] = 9; 
		depths_tmp[14] = 10; 
		depths_tmp[15] = 11; 
	}
else if(s_==5){ 
		vdata2[0] = vdata[11]; 
		vdata2[1] = vdata[12]; 
		vdata2[2] = vdata[13]; 
		vdata2[3] = vdata[14]; 
		vdata2[4] = vdata[15]; 
		vdata2[5] = vdata[0]; 
		vdata2[6] = vdata[1]; 
		vdata2[7] = vdata[2]; 
		vdata2[8] = vdata[3]; 
		vdata2[9] = vdata[4]; 
		vdata2[10] = vdata[5]; 
		vdata2[11] = vdata[6]; 
		vdata2[12] = vdata[7]; 
		vdata2[13] = vdata[8]; 
		vdata2[14] = vdata[9]; 
		vdata2[15] = vdata[10]; 
		
		depths_tmp[0] = 11; 
		depths_tmp[1] = 12; 
		depths_tmp[2] = 13; 
		depths_tmp[3] = 14; 
		depths_tmp[4] = 15; 
		depths_tmp[5] = 0; 
		depths_tmp[6] = 1; 
		depths_tmp[7] = 2; 
		depths_tmp[8] = 3; 
		depths_tmp[9] = 4; 
		depths_tmp[10] = 5; 
		depths_tmp[11] = 6; 
		depths_tmp[12] = 7; 
		depths_tmp[13] = 8; 
		depths_tmp[14] = 9; 
		depths_tmp[15] = 10; 
	}
else if(s_==6){ 
		vdata2[0] = vdata[10]; 
		vdata2[1] = vdata[11]; 
		vdata2[2] = vdata[12]; 
		vdata2[3] = vdata[13]; 
		vdata2[4] = vdata[14]; 
		vdata2[5] = vdata[15]; 
		vdata2[6] = vdata[0]; 
		vdata2[7] = vdata[1]; 
		vdata2[8] = vdata[2]; 
		vdata2[9] = vdata[3]; 
		vdata2[10] = vdata[4]; 
		vdata2[11] = vdata[5]; 
		vdata2[12] = vdata[6]; 
		vdata2[13] = vdata[7]; 
		vdata2[14] = vdata[8]; 
		vdata2[15] = vdata[9]; 
		
		depths_tmp[0] = 10; 
		depths_tmp[1] = 11; 
		depths_tmp[2] = 12; 
		depths_tmp[3] = 13; 
		depths_tmp[4] = 14; 
		depths_tmp[5] = 15; 
		depths_tmp[6] = 0; 
		depths_tmp[7] = 1; 
		depths_tmp[8] = 2; 
		depths_tmp[9] = 3; 
		depths_tmp[10] = 4; 
		depths_tmp[11] = 5; 
		depths_tmp[12] = 6; 
		depths_tmp[13] = 7; 
		depths_tmp[14] = 8; 
		depths_tmp[15] = 9; 
	}
else if(s_==7){ 
		vdata2[0] = vdata[9]; 
		vdata2[1] = vdata[10]; 
		vdata2[2] = vdata[11]; 
		vdata2[3] = vdata[12]; 
		vdata2[4] = vdata[13]; 
		vdata2[5] = vdata[14]; 
		vdata2[6] = vdata[15]; 
		vdata2[7] = vdata[0]; 
		vdata2[8] = vdata[1]; 
		vdata2[9] = vdata[2]; 
		vdata2[10] = vdata[3]; 
		vdata2[11] = vdata[4]; 
		vdata2[12] = vdata[5]; 
		vdata2[13] = vdata[6]; 
		vdata2[14] = vdata[7]; 
		vdata2[15] = vdata[8]; 
		
		depths_tmp[0] = 9; 
		depths_tmp[1] = 10; 
		depths_tmp[2] = 11; 
		depths_tmp[3] = 12; 
		depths_tmp[4] = 13; 
		depths_tmp[5] = 14; 
		depths_tmp[6] = 15; 
		depths_tmp[7] = 0; 
		depths_tmp[8] = 1; 
		depths_tmp[9] = 2; 
		depths_tmp[10] = 3; 
		depths_tmp[11] = 4; 
		depths_tmp[12] = 5; 
		depths_tmp[13] = 6; 
		depths_tmp[14] = 7; 
		depths_tmp[15] = 8; 
	}
else if(s_==8){ 
		vdata2[0] = vdata[8]; 
		vdata2[1] = vdata[9]; 
		vdata2[2] = vdata[10]; 
		vdata2[3] = vdata[11]; 
		vdata2[4] = vdata[12]; 
		vdata2[5] = vdata[13]; 
		vdata2[6] = vdata[14]; 
		vdata2[7] = vdata[15]; 
		vdata2[8] = vdata[0]; 
		vdata2[9] = vdata[1]; 
		vdata2[10] = vdata[2]; 
		vdata2[11] = vdata[3]; 
		vdata2[12] = vdata[4]; 
		vdata2[13] = vdata[5]; 
		vdata2[14] = vdata[6]; 
		vdata2[15] = vdata[7]; 
		
		depths_tmp[0] = 8; 
		depths_tmp[1] = 9; 
		depths_tmp[2] = 10; 
		depths_tmp[3] = 11; 
		depths_tmp[4] = 12; 
		depths_tmp[5] = 13; 
		depths_tmp[6] = 14; 
		depths_tmp[7] = 15; 
		depths_tmp[8] = 0; 
		depths_tmp[9] = 1; 
		depths_tmp[10] = 2; 
		depths_tmp[11] = 3; 
		depths_tmp[12] = 4; 
		depths_tmp[13] = 5; 
		depths_tmp[14] = 6; 
		depths_tmp[15] = 7; 
	}
else if(s_==9){ 
		vdata2[0] = vdata[7]; 
		vdata2[1] = vdata[8]; 
		vdata2[2] = vdata[9]; 
		vdata2[3] = vdata[10]; 
		vdata2[4] = vdata[11]; 
		vdata2[5] = vdata[12]; 
		vdata2[6] = vdata[13]; 
		vdata2[7] = vdata[14]; 
		vdata2[8] = vdata[15]; 
		vdata2[9] = vdata[0]; 
		vdata2[10] = vdata[1]; 
		vdata2[11] = vdata[2]; 
		vdata2[12] = vdata[3]; 
		vdata2[13] = vdata[4]; 
		vdata2[14] = vdata[5]; 
		vdata2[15] = vdata[6]; 
		
		depths_tmp[0] = 7; 
		depths_tmp[1] = 8; 
		depths_tmp[2] = 9; 
		depths_tmp[3] = 10; 
		depths_tmp[4] = 11; 
		depths_tmp[5] = 12; 
		depths_tmp[6] = 13; 
		depths_tmp[7] = 14; 
		depths_tmp[8] = 15; 
		depths_tmp[9] = 0; 
		depths_tmp[10] = 1; 
		depths_tmp[11] = 2; 
		depths_tmp[12] = 3; 
		depths_tmp[13] = 4; 
		depths_tmp[14] = 5; 
		depths_tmp[15] = 6; 
	}
else if(s_==10){ 
		vdata2[0] = vdata[6]; 
		vdata2[1] = vdata[7]; 
		vdata2[2] = vdata[8]; 
		vdata2[3] = vdata[9]; 
		vdata2[4] = vdata[10]; 
		vdata2[5] = vdata[11]; 
		vdata2[6] = vdata[12]; 
		vdata2[7] = vdata[13]; 
		vdata2[8] = vdata[14]; 
		vdata2[9] = vdata[15]; 
		vdata2[10] = vdata[0]; 
		vdata2[11] = vdata[1]; 
		vdata2[12] = vdata[2]; 
		vdata2[13] = vdata[3]; 
		vdata2[14] = vdata[4]; 
		vdata2[15] = vdata[5]; 
		
		depths_tmp[0] = 6; 
		depths_tmp[1] = 7; 
		depths_tmp[2] = 8; 
		depths_tmp[3] = 9; 
		depths_tmp[4] = 10; 
		depths_tmp[5] = 11; 
		depths_tmp[6] = 12; 
		depths_tmp[7] = 13; 
		depths_tmp[8] = 14; 
		depths_tmp[9] = 15; 
		depths_tmp[10] = 0; 
		depths_tmp[11] = 1; 
		depths_tmp[12] = 2; 
		depths_tmp[13] = 3; 
		depths_tmp[14] = 4; 
		depths_tmp[15] = 5; 
	}
else if(s_==11){ 
		vdata2[0] = vdata[5]; 
		vdata2[1] = vdata[6]; 
		vdata2[2] = vdata[7]; 
		vdata2[3] = vdata[8]; 
		vdata2[4] = vdata[9]; 
		vdata2[5] = vdata[10]; 
		vdata2[6] = vdata[11]; 
		vdata2[7] = vdata[12]; 
		vdata2[8] = vdata[13]; 
		vdata2[9] = vdata[14]; 
		vdata2[10] = vdata[15]; 
		vdata2[11] = vdata[0]; 
		vdata2[12] = vdata[1]; 
		vdata2[13] = vdata[2]; 
		vdata2[14] = vdata[3]; 
		vdata2[15] = vdata[4]; 
		
		depths_tmp[0] = 5; 
		depths_tmp[1] = 6; 
		depths_tmp[2] = 7; 
		depths_tmp[3] = 8; 
		depths_tmp[4] = 9; 
		depths_tmp[5] = 10; 
		depths_tmp[6] = 11; 
		depths_tmp[7] = 12; 
		depths_tmp[8] = 13; 
		depths_tmp[9] = 14; 
		depths_tmp[10] = 15; 
		depths_tmp[11] = 0; 
		depths_tmp[12] = 1; 
		depths_tmp[13] = 2; 
		depths_tmp[14] = 3; 
		depths_tmp[15] = 4; 
	}
else if(s_==12){ 
		vdata2[0] = vdata[4]; 
		vdata2[1] = vdata[5]; 
		vdata2[2] = vdata[6]; 
		vdata2[3] = vdata[7]; 
		vdata2[4] = vdata[8]; 
		vdata2[5] = vdata[9]; 
		vdata2[6] = vdata[10]; 
		vdata2[7] = vdata[11]; 
		vdata2[8] = vdata[12]; 
		vdata2[9] = vdata[13]; 
		vdata2[10] = vdata[14]; 
		vdata2[11] = vdata[15]; 
		vdata2[12] = vdata[0]; 
		vdata2[13] = vdata[1]; 
		vdata2[14] = vdata[2]; 
		vdata2[15] = vdata[3]; 
		
		depths_tmp[0] = 4; 
		depths_tmp[1] = 5; 
		depths_tmp[2] = 6; 
		depths_tmp[3] = 7; 
		depths_tmp[4] = 8; 
		depths_tmp[5] = 9; 
		depths_tmp[6] = 10; 
		depths_tmp[7] = 11; 
		depths_tmp[8] = 12; 
		depths_tmp[9] = 13; 
		depths_tmp[10] = 14; 
		depths_tmp[11] = 15; 
		depths_tmp[12] = 0; 
		depths_tmp[13] = 1; 
		depths_tmp[14] = 2; 
		depths_tmp[15] = 3; 
	}
else if(s_==13){ 
		vdata2[0] = vdata[3]; 
		vdata2[1] = vdata[4]; 
		vdata2[2] = vdata[5]; 
		vdata2[3] = vdata[6]; 
		vdata2[4] = vdata[7]; 
		vdata2[5] = vdata[8]; 
		vdata2[6] = vdata[9]; 
		vdata2[7] = vdata[10]; 
		vdata2[8] = vdata[11]; 
		vdata2[9] = vdata[12]; 
		vdata2[10] = vdata[13]; 
		vdata2[11] = vdata[14]; 
		vdata2[12] = vdata[15]; 
		vdata2[13] = vdata[0]; 
		vdata2[14] = vdata[1]; 
		vdata2[15] = vdata[2]; 
		
		depths_tmp[0] = 3; 
		depths_tmp[1] = 4; 
		depths_tmp[2] = 5; 
		depths_tmp[3] = 6; 
		depths_tmp[4] = 7; 
		depths_tmp[5] = 8; 
		depths_tmp[6] = 9; 
		depths_tmp[7] = 10; 
		depths_tmp[8] = 11; 
		depths_tmp[9] = 12; 
		depths_tmp[10] = 13; 
		depths_tmp[11] = 14; 
		depths_tmp[12] = 15; 
		depths_tmp[13] = 0; 
		depths_tmp[14] = 1; 
		depths_tmp[15] = 2; 
	}
else if(s_==14){ 
		vdata2[0] = vdata[2]; 
		vdata2[1] = vdata[3]; 
		vdata2[2] = vdata[4]; 
		vdata2[3] = vdata[5]; 
		vdata2[4] = vdata[6]; 
		vdata2[5] = vdata[7]; 
		vdata2[6] = vdata[8]; 
		vdata2[7] = vdata[9]; 
		vdata2[8] = vdata[10]; 
		vdata2[9] = vdata[11]; 
		vdata2[10] = vdata[12]; 
		vdata2[11] = vdata[13]; 
		vdata2[12] = vdata[14]; 
		vdata2[13] = vdata[15]; 
		vdata2[14] = vdata[0]; 
		vdata2[15] = vdata[1]; 
		
		depths_tmp[0] = 2; 
		depths_tmp[1] = 3; 
		depths_tmp[2] = 4; 
		depths_tmp[3] = 5; 
		depths_tmp[4] = 6; 
		depths_tmp[5] = 7; 
		depths_tmp[6] = 8; 
		depths_tmp[7] = 9; 
		depths_tmp[8] = 10; 
		depths_tmp[9] = 11; 
		depths_tmp[10] = 12; 
		depths_tmp[11] = 13; 
		depths_tmp[12] = 14; 
		depths_tmp[13] = 15; 
		depths_tmp[14] = 0; 
		depths_tmp[15] = 1; 
	}
else { 
		vdata2[0] = vdata[1]; 
		vdata2[1] = vdata[2]; 
		vdata2[2] = vdata[3]; 
		vdata2[3] = vdata[4]; 
		vdata2[4] = vdata[5]; 
		vdata2[5] = vdata[6]; 
		vdata2[6] = vdata[7]; 
		vdata2[7] = vdata[8]; 
		vdata2[8] = vdata[9]; 
		vdata2[9] = vdata[10]; 
		vdata2[10] = vdata[11]; 
		vdata2[11] = vdata[12]; 
		vdata2[12] = vdata[13]; 
		vdata2[13] = vdata[14]; 
		vdata2[14] = vdata[15]; 
		vdata2[15] = vdata[0]; 
		
		depths_tmp[0] = 1; 
		depths_tmp[1] = 2; 
		depths_tmp[2] = 3; 
		depths_tmp[3] = 4; 
		depths_tmp[4] = 5; 
		depths_tmp[5] = 6; 
		depths_tmp[6] = 7; 
		depths_tmp[7] = 8; 
		depths_tmp[8] = 9; 
		depths_tmp[9] = 10; 
		depths_tmp[10] = 11; 
		depths_tmp[11] = 12; 
		depths_tmp[12] = 13; 
		depths_tmp[13] = 14; 
		depths_tmp[14] = 15; 
		depths_tmp[15] = 0; 
	}
	
	depths[0] = basedepth + depths_tmp[0]; 
	depths[1] = basedepth + depths_tmp[1]; 
	depths[2] = basedepth + depths_tmp[2]; 
	depths[3] = basedepth + depths_tmp[3]; 
	depths[4] = basedepth + depths_tmp[4]; 
	depths[5] = basedepth + depths_tmp[5]; 
	depths[6] = basedepth + depths_tmp[6]; 
	depths[7] = basedepth + depths_tmp[7]; 
	depths[8] = basedepth + depths_tmp[8]; 
	depths[9] = basedepth + depths_tmp[9]; 
	depths[10] = basedepth + depths_tmp[10]; 
	depths[11] = basedepth + depths_tmp[11]; 
	depths[12] = basedepth + depths_tmp[12]; 
	depths[13] = basedepth + depths_tmp[13]; 
	depths[14] = basedepth + depths_tmp[14]; 
	depths[15] = basedepth + depths_tmp[15]; 
	return;
}

void acts_all::MEMACCESSP0_RearrangeLayoutV(unsigned int s, keyvalue_vbuffer_t vdata[VECTOR2_SIZE], keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]){
	/// NB: This function must be consistent with loadedges_random.cpp->loadedges_random::twist_getpartition
	unsigned int s_ = s % VECTOR2_SIZE;	
	
 if(s_==0){ 
		vdata2[0] = vdata[0]; 
		vdata2[1] = vdata[1]; 
		vdata2[2] = vdata[2]; 
		vdata2[3] = vdata[3]; 
		vdata2[4] = vdata[4]; 
		vdata2[5] = vdata[5]; 
		vdata2[6] = vdata[6]; 
		vdata2[7] = vdata[7]; 
		vdata2[8] = vdata[8]; 
		vdata2[9] = vdata[9]; 
		vdata2[10] = vdata[10]; 
		vdata2[11] = vdata[11]; 
		vdata2[12] = vdata[12]; 
		vdata2[13] = vdata[13]; 
		vdata2[14] = vdata[14]; 
		vdata2[15] = vdata[15]; 
	}
else if(s_==1){ 
		vdata2[1] = vdata[0]; 
		vdata2[2] = vdata[1]; 
		vdata2[3] = vdata[2]; 
		vdata2[4] = vdata[3]; 
		vdata2[5] = vdata[4]; 
		vdata2[6] = vdata[5]; 
		vdata2[7] = vdata[6]; 
		vdata2[8] = vdata[7]; 
		vdata2[9] = vdata[8]; 
		vdata2[10] = vdata[9]; 
		vdata2[11] = vdata[10]; 
		vdata2[12] = vdata[11]; 
		vdata2[13] = vdata[12]; 
		vdata2[14] = vdata[13]; 
		vdata2[15] = vdata[14]; 
		vdata2[0] = vdata[15]; 
	}
else if(s_==2){ 
		vdata2[2] = vdata[0]; 
		vdata2[3] = vdata[1]; 
		vdata2[4] = vdata[2]; 
		vdata2[5] = vdata[3]; 
		vdata2[6] = vdata[4]; 
		vdata2[7] = vdata[5]; 
		vdata2[8] = vdata[6]; 
		vdata2[9] = vdata[7]; 
		vdata2[10] = vdata[8]; 
		vdata2[11] = vdata[9]; 
		vdata2[12] = vdata[10]; 
		vdata2[13] = vdata[11]; 
		vdata2[14] = vdata[12]; 
		vdata2[15] = vdata[13]; 
		vdata2[0] = vdata[14]; 
		vdata2[1] = vdata[15]; 
	}
else if(s_==3){ 
		vdata2[3] = vdata[0]; 
		vdata2[4] = vdata[1]; 
		vdata2[5] = vdata[2]; 
		vdata2[6] = vdata[3]; 
		vdata2[7] = vdata[4]; 
		vdata2[8] = vdata[5]; 
		vdata2[9] = vdata[6]; 
		vdata2[10] = vdata[7]; 
		vdata2[11] = vdata[8]; 
		vdata2[12] = vdata[9]; 
		vdata2[13] = vdata[10]; 
		vdata2[14] = vdata[11]; 
		vdata2[15] = vdata[12]; 
		vdata2[0] = vdata[13]; 
		vdata2[1] = vdata[14]; 
		vdata2[2] = vdata[15]; 
	}
else if(s_==4){ 
		vdata2[4] = vdata[0]; 
		vdata2[5] = vdata[1]; 
		vdata2[6] = vdata[2]; 
		vdata2[7] = vdata[3]; 
		vdata2[8] = vdata[4]; 
		vdata2[9] = vdata[5]; 
		vdata2[10] = vdata[6]; 
		vdata2[11] = vdata[7]; 
		vdata2[12] = vdata[8]; 
		vdata2[13] = vdata[9]; 
		vdata2[14] = vdata[10]; 
		vdata2[15] = vdata[11]; 
		vdata2[0] = vdata[12]; 
		vdata2[1] = vdata[13]; 
		vdata2[2] = vdata[14]; 
		vdata2[3] = vdata[15]; 
	}
else if(s_==5){ 
		vdata2[5] = vdata[0]; 
		vdata2[6] = vdata[1]; 
		vdata2[7] = vdata[2]; 
		vdata2[8] = vdata[3]; 
		vdata2[9] = vdata[4]; 
		vdata2[10] = vdata[5]; 
		vdata2[11] = vdata[6]; 
		vdata2[12] = vdata[7]; 
		vdata2[13] = vdata[8]; 
		vdata2[14] = vdata[9]; 
		vdata2[15] = vdata[10]; 
		vdata2[0] = vdata[11]; 
		vdata2[1] = vdata[12]; 
		vdata2[2] = vdata[13]; 
		vdata2[3] = vdata[14]; 
		vdata2[4] = vdata[15]; 
	}
else if(s_==6){ 
		vdata2[6] = vdata[0]; 
		vdata2[7] = vdata[1]; 
		vdata2[8] = vdata[2]; 
		vdata2[9] = vdata[3]; 
		vdata2[10] = vdata[4]; 
		vdata2[11] = vdata[5]; 
		vdata2[12] = vdata[6]; 
		vdata2[13] = vdata[7]; 
		vdata2[14] = vdata[8]; 
		vdata2[15] = vdata[9]; 
		vdata2[0] = vdata[10]; 
		vdata2[1] = vdata[11]; 
		vdata2[2] = vdata[12]; 
		vdata2[3] = vdata[13]; 
		vdata2[4] = vdata[14]; 
		vdata2[5] = vdata[15]; 
	}
else if(s_==7){ 
		vdata2[7] = vdata[0]; 
		vdata2[8] = vdata[1]; 
		vdata2[9] = vdata[2]; 
		vdata2[10] = vdata[3]; 
		vdata2[11] = vdata[4]; 
		vdata2[12] = vdata[5]; 
		vdata2[13] = vdata[6]; 
		vdata2[14] = vdata[7]; 
		vdata2[15] = vdata[8]; 
		vdata2[0] = vdata[9]; 
		vdata2[1] = vdata[10]; 
		vdata2[2] = vdata[11]; 
		vdata2[3] = vdata[12]; 
		vdata2[4] = vdata[13]; 
		vdata2[5] = vdata[14]; 
		vdata2[6] = vdata[15]; 
	}
else if(s_==8){ 
		vdata2[8] = vdata[0]; 
		vdata2[9] = vdata[1]; 
		vdata2[10] = vdata[2]; 
		vdata2[11] = vdata[3]; 
		vdata2[12] = vdata[4]; 
		vdata2[13] = vdata[5]; 
		vdata2[14] = vdata[6]; 
		vdata2[15] = vdata[7]; 
		vdata2[0] = vdata[8]; 
		vdata2[1] = vdata[9]; 
		vdata2[2] = vdata[10]; 
		vdata2[3] = vdata[11]; 
		vdata2[4] = vdata[12]; 
		vdata2[5] = vdata[13]; 
		vdata2[6] = vdata[14]; 
		vdata2[7] = vdata[15]; 
	}
else if(s_==9){ 
		vdata2[9] = vdata[0]; 
		vdata2[10] = vdata[1]; 
		vdata2[11] = vdata[2]; 
		vdata2[12] = vdata[3]; 
		vdata2[13] = vdata[4]; 
		vdata2[14] = vdata[5]; 
		vdata2[15] = vdata[6]; 
		vdata2[0] = vdata[7]; 
		vdata2[1] = vdata[8]; 
		vdata2[2] = vdata[9]; 
		vdata2[3] = vdata[10]; 
		vdata2[4] = vdata[11]; 
		vdata2[5] = vdata[12]; 
		vdata2[6] = vdata[13]; 
		vdata2[7] = vdata[14]; 
		vdata2[8] = vdata[15]; 
	}
else if(s_==10){ 
		vdata2[10] = vdata[0]; 
		vdata2[11] = vdata[1]; 
		vdata2[12] = vdata[2]; 
		vdata2[13] = vdata[3]; 
		vdata2[14] = vdata[4]; 
		vdata2[15] = vdata[5]; 
		vdata2[0] = vdata[6]; 
		vdata2[1] = vdata[7]; 
		vdata2[2] = vdata[8]; 
		vdata2[3] = vdata[9]; 
		vdata2[4] = vdata[10]; 
		vdata2[5] = vdata[11]; 
		vdata2[6] = vdata[12]; 
		vdata2[7] = vdata[13]; 
		vdata2[8] = vdata[14]; 
		vdata2[9] = vdata[15]; 
	}
else if(s_==11){ 
		vdata2[11] = vdata[0]; 
		vdata2[12] = vdata[1]; 
		vdata2[13] = vdata[2]; 
		vdata2[14] = vdata[3]; 
		vdata2[15] = vdata[4]; 
		vdata2[0] = vdata[5]; 
		vdata2[1] = vdata[6]; 
		vdata2[2] = vdata[7]; 
		vdata2[3] = vdata[8]; 
		vdata2[4] = vdata[9]; 
		vdata2[5] = vdata[10]; 
		vdata2[6] = vdata[11]; 
		vdata2[7] = vdata[12]; 
		vdata2[8] = vdata[13]; 
		vdata2[9] = vdata[14]; 
		vdata2[10] = vdata[15]; 
	}
else if(s_==12){ 
		vdata2[12] = vdata[0]; 
		vdata2[13] = vdata[1]; 
		vdata2[14] = vdata[2]; 
		vdata2[15] = vdata[3]; 
		vdata2[0] = vdata[4]; 
		vdata2[1] = vdata[5]; 
		vdata2[2] = vdata[6]; 
		vdata2[3] = vdata[7]; 
		vdata2[4] = vdata[8]; 
		vdata2[5] = vdata[9]; 
		vdata2[6] = vdata[10]; 
		vdata2[7] = vdata[11]; 
		vdata2[8] = vdata[12]; 
		vdata2[9] = vdata[13]; 
		vdata2[10] = vdata[14]; 
		vdata2[11] = vdata[15]; 
	}
else if(s_==13){ 
		vdata2[13] = vdata[0]; 
		vdata2[14] = vdata[1]; 
		vdata2[15] = vdata[2]; 
		vdata2[0] = vdata[3]; 
		vdata2[1] = vdata[4]; 
		vdata2[2] = vdata[5]; 
		vdata2[3] = vdata[6]; 
		vdata2[4] = vdata[7]; 
		vdata2[5] = vdata[8]; 
		vdata2[6] = vdata[9]; 
		vdata2[7] = vdata[10]; 
		vdata2[8] = vdata[11]; 
		vdata2[9] = vdata[12]; 
		vdata2[10] = vdata[13]; 
		vdata2[11] = vdata[14]; 
		vdata2[12] = vdata[15]; 
	}
else if(s_==14){ 
		vdata2[14] = vdata[0]; 
		vdata2[15] = vdata[1]; 
		vdata2[0] = vdata[2]; 
		vdata2[1] = vdata[3]; 
		vdata2[2] = vdata[4]; 
		vdata2[3] = vdata[5]; 
		vdata2[4] = vdata[6]; 
		vdata2[5] = vdata[7]; 
		vdata2[6] = vdata[8]; 
		vdata2[7] = vdata[9]; 
		vdata2[8] = vdata[10]; 
		vdata2[9] = vdata[11]; 
		vdata2[10] = vdata[12]; 
		vdata2[11] = vdata[13]; 
		vdata2[12] = vdata[14]; 
		vdata2[13] = vdata[15]; 
	}
else { 
		vdata2[15] = vdata[0]; 
		vdata2[0] = vdata[1]; 
		vdata2[1] = vdata[2]; 
		vdata2[2] = vdata[3]; 
		vdata2[3] = vdata[4]; 
		vdata2[4] = vdata[5]; 
		vdata2[5] = vdata[6]; 
		vdata2[6] = vdata[7]; 
		vdata2[7] = vdata[8]; 
		vdata2[8] = vdata[9]; 
		vdata2[9] = vdata[10]; 
		vdata2[10] = vdata[11]; 
		vdata2[11] = vdata[12]; 
		vdata2[12] = vdata[13]; 
		vdata2[13] = vdata[14]; 
		vdata2[14] = vdata[15]; 
	}
	return;
}

void acts_all::MEMACCESSP0_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / 2;
	
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD // CRITICAL FIXME.
		vdata[0] = kvdram[baseoffset_kvs + offset_kvs + i].range(31, 0); 
		vdata[1] = kvdram[baseoffset_kvs + offset_kvs + i].range(63, 32); 
		vdata[2] = kvdram[baseoffset_kvs + offset_kvs + i].range(95, 64); 
		vdata[3] = kvdram[baseoffset_kvs + offset_kvs + i].range(127, 96); 
		vdata[4] = kvdram[baseoffset_kvs + offset_kvs + i].range(159, 128); 
		vdata[5] = kvdram[baseoffset_kvs + offset_kvs + i].range(191, 160); 
		vdata[6] = kvdram[baseoffset_kvs + offset_kvs + i].range(223, 192); 
		vdata[7] = kvdram[baseoffset_kvs + offset_kvs + i].range(255, 224); 
		vdata[8] = kvdram[baseoffset_kvs + offset_kvs + i].range(287, 256); 
		vdata[9] = kvdram[baseoffset_kvs + offset_kvs + i].range(319, 288); 
		vdata[10] = kvdram[baseoffset_kvs + offset_kvs + i].range(351, 320); 
		vdata[11] = kvdram[baseoffset_kvs + offset_kvs + i].range(383, 352); 
		vdata[12] = kvdram[baseoffset_kvs + offset_kvs + i].range(415, 384); 
		vdata[13] = kvdram[baseoffset_kvs + offset_kvs + i].range(447, 416); 
		vdata[14] = kvdram[baseoffset_kvs + offset_kvs + i].range(479, 448); 
		vdata[15] = kvdram[baseoffset_kvs + offset_kvs + i].range(511, 480); 
		#else 
		vdata[0] = kvdram[baseoffset_kvs + offset_kvs + i].data[0].key;
		vdata[1] = kvdram[baseoffset_kvs + offset_kvs + i].data[0].value; 
		vdata[2] = kvdram[baseoffset_kvs + offset_kvs + i].data[1].key;
		vdata[3] = kvdram[baseoffset_kvs + offset_kvs + i].data[1].value; 
		vdata[4] = kvdram[baseoffset_kvs + offset_kvs + i].data[2].key;
		vdata[5] = kvdram[baseoffset_kvs + offset_kvs + i].data[2].value; 
		vdata[6] = kvdram[baseoffset_kvs + offset_kvs + i].data[3].key;
		vdata[7] = kvdram[baseoffset_kvs + offset_kvs + i].data[3].value; 
		vdata[8] = kvdram[baseoffset_kvs + offset_kvs + i].data[4].key;
		vdata[9] = kvdram[baseoffset_kvs + offset_kvs + i].data[4].value; 
		vdata[10] = kvdram[baseoffset_kvs + offset_kvs + i].data[5].key;
		vdata[11] = kvdram[baseoffset_kvs + offset_kvs + i].data[5].value; 
		vdata[12] = kvdram[baseoffset_kvs + offset_kvs + i].data[6].key;
		vdata[13] = kvdram[baseoffset_kvs + offset_kvs + i].data[6].value; 
		vdata[14] = kvdram[baseoffset_kvs + offset_kvs + i].data[7].key;
		vdata[15] = kvdram[baseoffset_kvs + offset_kvs + i].data[7].value; 
		#endif
		
		buffer[0][bufferoffset_kvs + i] = vdata[0];
		buffer[1][bufferoffset_kvs + i] = vdata[1];
		buffer[2][bufferoffset_kvs + i] = vdata[2];
		buffer[3][bufferoffset_kvs + i] = vdata[3];
		buffer[4][bufferoffset_kvs + i] = vdata[4];
		buffer[5][bufferoffset_kvs + i] = vdata[5];
		buffer[6][bufferoffset_kvs + i] = vdata[6];
		buffer[7][bufferoffset_kvs + i] = vdata[7];
		buffer[8][bufferoffset_kvs + i] = vdata[8];
		buffer[9][bufferoffset_kvs + i] = vdata[9];
		buffer[10][bufferoffset_kvs + i] = vdata[10];
		buffer[11][bufferoffset_kvs + i] = vdata[11];
		buffer[12][bufferoffset_kvs + i] = vdata[12];
		buffer[13][bufferoffset_kvs + i] = vdata[13];
		buffer[14][bufferoffset_kvs + i] = vdata[14];
		buffer[15][bufferoffset_kvs + i] = vdata[15];

		#ifdef _DEBUGMODE_STATS 
		actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
		#endif
	}
	return;
}

void acts_all::MEMACCESSP0_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], pmask_dt pmask[BLOCKRAM_PMASK_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, unsigned int source_partition, globalposition_t globalposition, globalparams_t globalparams, globalparams_t globalparamsV){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / 2;
	
	keyvalue_vbuffer_t vdatas[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	
	uint32_type pmaski = 0;
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	unsigned int IND = UTILP0_allignhigher_FACTOR((source_partition * globalparams.SIZEKVS2_REDUCEPARTITION) / VPARTITION_SHRINK_RATIO, 32);
	unsigned int VProw = IND / 32; unsigned int VPcol = IND % 32;
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("MEMACCESSP0_saveV 23", VProw, BLOCKRAM_PMASK_SIZE, NAp, NAp, NAp);
	if(VPcol != 0){ cout<<"MEMACCESSP0_saveV: ERROR SOMEWHERE. VPcol("<<VPcol<<") != 0, source_partition: "<<source_partition<<", globalparams.SIZEKVS2_REDUCEPARTITION: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", VPARTITION_SHRINK_RATIO: "<<VPARTITION_SHRINK_RATIO<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MEMACCESSP0_saveV: source_partition: "<<source_partition<<", IND: "<<IND<<", VProw: "<<VProw<<", VPcol: "<<VPcol<<", SIZEKVS2_REDUCEPARTITION: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", VPARTITION_SHRINK_RATIO: "<<VPARTITION_SHRINK_RATIO<<endl;
	#endif 
	// unsigned int VPcol = 0;
	#endif 
	
	SAVEVDATA_LOOP1: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
	
		vdata[0] = vbuffer[0][bufferoffset_kvs + i];
	
		vdata[1] = vbuffer[1][bufferoffset_kvs + i];
	
		vdata[2] = vbuffer[2][bufferoffset_kvs + i];
	
		vdata[3] = vbuffer[3][bufferoffset_kvs + i];
	
		vdata[4] = vbuffer[4][bufferoffset_kvs + i];
	
		vdata[5] = vbuffer[5][bufferoffset_kvs + i];
	
		vdata[6] = vbuffer[6][bufferoffset_kvs + i];
	
		vdata[7] = vbuffer[7][bufferoffset_kvs + i];
	
		vdata[8] = vbuffer[8][bufferoffset_kvs + i];
	
		vdata[9] = vbuffer[9][bufferoffset_kvs + i];
	
		vdata[10] = vbuffer[10][bufferoffset_kvs + i];
	
		vdata[11] = vbuffer[11][bufferoffset_kvs + i];
	
		vdata[12] = vbuffer[12][bufferoffset_kvs + i];
	
		vdata[13] = vbuffer[13][bufferoffset_kvs + i];
	
		vdata[14] = vbuffer[14][bufferoffset_kvs + i];
	
		vdata[15] = vbuffer[15][bufferoffset_kvs + i];
		
		// collect active partitions for next iterations
		#ifndef ALLVERTEXISACTIVE_ALGORITHM
			#ifdef _WIDEWORD
			pmaski.range(0, 0) = MEMCAP0_READVMASK(vdata[0]); 
			pmaski.range(1, 1) = MEMCAP0_READVMASK(vdata[1]); 
			pmaski.range(2, 2) = MEMCAP0_READVMASK(vdata[2]); 
			pmaski.range(3, 3) = MEMCAP0_READVMASK(vdata[3]); 
			pmaski.range(4, 4) = MEMCAP0_READVMASK(vdata[4]); 
			pmaski.range(5, 5) = MEMCAP0_READVMASK(vdata[5]); 
			pmaski.range(6, 6) = MEMCAP0_READVMASK(vdata[6]); 
			pmaski.range(7, 7) = MEMCAP0_READVMASK(vdata[7]); 
			pmaski.range(8, 8) = MEMCAP0_READVMASK(vdata[8]); 
			pmaski.range(9, 9) = MEMCAP0_READVMASK(vdata[9]); 
			pmaski.range(10, 10) = MEMCAP0_READVMASK(vdata[10]); 
			pmaski.range(11, 11) = MEMCAP0_READVMASK(vdata[11]); 
			pmaski.range(12, 12) = MEMCAP0_READVMASK(vdata[12]); 
			pmaski.range(13, 13) = MEMCAP0_READVMASK(vdata[13]); 
			pmaski.range(14, 14) = MEMCAP0_READVMASK(vdata[14]); 
			pmaski.range(15, 15) = MEMCAP0_READVMASK(vdata[15]); 
			#else
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 0, 1, MEMCAP0_READVMASK(vdata[0]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 1, 1, MEMCAP0_READVMASK(vdata[1]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 2, 1, MEMCAP0_READVMASK(vdata[2]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 3, 1, MEMCAP0_READVMASK(vdata[3]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 4, 1, MEMCAP0_READVMASK(vdata[4]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 5, 1, MEMCAP0_READVMASK(vdata[5]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 6, 1, MEMCAP0_READVMASK(vdata[6]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 7, 1, MEMCAP0_READVMASK(vdata[7]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 8, 1, MEMCAP0_READVMASK(vdata[8]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 9, 1, MEMCAP0_READVMASK(vdata[9]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 10, 1, MEMCAP0_READVMASK(vdata[10]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 11, 1, MEMCAP0_READVMASK(vdata[11]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 12, 1, MEMCAP0_READVMASK(vdata[12]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 13, 1, MEMCAP0_READVMASK(vdata[13]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 14, 1, MEMCAP0_READVMASK(vdata[14]));
			UTILP0_WRITEBITSTO_UINTV(&pmaski, 15, 1, MEMCAP0_READVMASK(vdata[15]));
	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata[0]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 0, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[1]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 1, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[2]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 2, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[3]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 3, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[4]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 4, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[5]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 5, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[6]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 6, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[7]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 7, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[8]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 8, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[9]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 9, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[10]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 10, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[11]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 11, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[12]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 12, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[13]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 13, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[14]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 14, VProw: "<<VProw<<endl; }
			if(MEMCAP0_READVMASK(vdata[15]) > 0){ cout<<"--- MEMACCESSP0_saveV:: NEXT ACTIVE PARTITION ACTIVATED i: "<<i<<", v: 15, VProw: "<<VProw<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_saveV 23", VPcol, 32, NAp, NAp, NAp);
			#endif
			if(pmaski>0){ temppmask[VPcol] = 1; } if(i%VPARTITION_SHRINK_RATIO==0){ VPcol += 1; }	
			if(VPcol % 32 == 31){
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MEMACCESSP0_saveV 23", VProw, BLOCKRAM_PMASK_SIZE, NAp, NAp, NAp);
				#endif
				pmask[VProw].data[0] = temppmask[0];
				pmask[VProw].data[1] = temppmask[1];
				pmask[VProw].data[2] = temppmask[2];
				pmask[VProw].data[3] = temppmask[3];
				pmask[VProw].data[4] = temppmask[4];
				pmask[VProw].data[5] = temppmask[5];
				pmask[VProw].data[6] = temppmask[6];
				pmask[VProw].data[7] = temppmask[7];
				pmask[VProw].data[8] = temppmask[8];
				pmask[VProw].data[9] = temppmask[9];
				pmask[VProw].data[10] = temppmask[10];
				pmask[VProw].data[11] = temppmask[11];
				pmask[VProw].data[12] = temppmask[12];
				pmask[VProw].data[13] = temppmask[13];
				pmask[VProw].data[14] = temppmask[14];
				pmask[VProw].data[15] = temppmask[15];
				pmask[VProw].data[16] = temppmask[16];
				pmask[VProw].data[17] = temppmask[17];
				pmask[VProw].data[18] = temppmask[18];
				pmask[VProw].data[19] = temppmask[19];
				pmask[VProw].data[20] = temppmask[20];
				pmask[VProw].data[21] = temppmask[21];
				pmask[VProw].data[22] = temppmask[22];
				pmask[VProw].data[23] = temppmask[23];
				pmask[VProw].data[24] = temppmask[24];
				pmask[VProw].data[25] = temppmask[25];
				pmask[VProw].data[26] = temppmask[26];
				pmask[VProw].data[27] = temppmask[27];
				pmask[VProw].data[28] = temppmask[28];
				pmask[VProw].data[29] = temppmask[29];
				pmask[VProw].data[30] = temppmask[30];
				pmask[VProw].data[31] = temppmask[31];
				temppmask[0] = 0;
				temppmask[1] = 0;
				temppmask[2] = 0;
				temppmask[3] = 0;
				temppmask[4] = 0;
				temppmask[5] = 0;
				temppmask[6] = 0;
				temppmask[7] = 0;
				temppmask[8] = 0;
				temppmask[9] = 0;
				temppmask[10] = 0;
				temppmask[11] = 0;
				temppmask[12] = 0;
				temppmask[13] = 0;
				temppmask[14] = 0;
				temppmask[15] = 0;
				temppmask[16] = 0;
				temppmask[17] = 0;
				temppmask[18] = 0;
				temppmask[19] = 0;
				temppmask[20] = 0;
				temppmask[21] = 0;
				temppmask[22] = 0;
				temppmask[23] = 0;
				temppmask[24] = 0;
				temppmask[25] = 0;
				temppmask[26] = 0;
				temppmask[27] = 0;
				temppmask[28] = 0;
				temppmask[29] = 0;
				temppmask[30] = 0;
				temppmask[31] = 0;
				VPcol = 0;
				VProw += 1; 
			}
		#endif
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSP0_saveV 23", baseoffset_kvs + offset_kvs + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD // CRITICAL FIXME.
		kvdram[baseoffset_kvs + offset_kvs + i].range(31, 0) = vdata[0]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(63, 32) = vdata[1]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(95, 64) = vdata[2]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(127, 96) = vdata[3]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(159, 128) = vdata[4]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(191, 160) = vdata[5]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(223, 192) = vdata[6]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(255, 224) = vdata[7]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(287, 256) = vdata[8]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(319, 288) = vdata[9]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(351, 320) = vdata[10]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(383, 352) = vdata[11]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(415, 384) = vdata[12]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(447, 416) = vdata[13]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(479, 448) = vdata[14]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(511, 480) = vdata[15]; 
		#else 
		kvdram[baseoffset_kvs + offset_kvs + i].data[0].key = vdata[0];
		kvdram[baseoffset_kvs + offset_kvs + i].data[0].value = vdata[1]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[1].key = vdata[2];
		kvdram[baseoffset_kvs + offset_kvs + i].data[1].value = vdata[3]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[2].key = vdata[4];
		kvdram[baseoffset_kvs + offset_kvs + i].data[2].value = vdata[5]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[3].key = vdata[6];
		kvdram[baseoffset_kvs + offset_kvs + i].data[3].value = vdata[7]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[4].key = vdata[8];
		kvdram[baseoffset_kvs + offset_kvs + i].data[4].value = vdata[9]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[5].key = vdata[10];
		kvdram[baseoffset_kvs + offset_kvs + i].data[5].value = vdata[11]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[6].key = vdata[12];
		kvdram[baseoffset_kvs + offset_kvs + i].data[6].value = vdata[13]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[7].key = vdata[14];
		kvdram[baseoffset_kvs + offset_kvs + i].data[7].value = vdata[15]; 
		#endif

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countvswritten(VECTOR2_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"saveV:: printing pmask... VProw: "<<VProw<<", VPcol: "<<VPcol<<endl;
	for(unsigned int i=0; i<BLOCKRAM_PMASK_SIZE; i++){
		for(unsigned int v=0; v<32; v++){ if(pmask[i].data[v] > 0){ cout<<"$$$ MEMACCESSP0_saveV:: MASK POS [i:"<<i<<", v:"<<v<<"] is ACTIVE ("<<pmask[i].data[v]<<") "<<endl; }}
		// for(unsigned int v=0; v<32; v++){ cout<<pmask[i].data[v]<<", "; } cout<<endl;
	}
	#endif 
	// exit(EXIT_SUCCESS); //
	return;
}

void acts_all::MEMACCESSP0_readANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks1 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks1 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks2 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks2 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks3 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks3 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks4 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks4 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks5 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks5 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks6 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks6 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
	
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks7 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks7 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
	
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
	
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks8 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks8 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
	
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
	
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
	
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	#pragma HLS array_partition variable = buffer8
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
		buffer8[0][i] = 0;
		buffer8[1][i] = 0;
		buffer8[2][i] = 0;
		buffer8[3][i] = 0;
		buffer8[4][i] = 0;
		buffer8[5][i] = 0;
		buffer8[6][i] = 0;
		buffer8[7][i] = 0;
		buffer8[8][i] = 0;
		buffer8[9][i] = 0;
		buffer8[10][i] = 0;
		buffer8[11][i] = 0;
		buffer8[12][i] = 0;
		buffer8[13][i] = 0;
		buffer8[14][i] = 0;
		buffer8[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks9 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks9 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
	
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
	
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
	
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
	
			buffer8[0][bdepth_i + i] = vdata2[0];
			buffer8[1][bdepth_i + i] = vdata2[1];
			buffer8[2][bdepth_i + i] = vdata2[2];
			buffer8[3][bdepth_i + i] = vdata2[3];
			buffer8[4][bdepth_i + i] = vdata2[4];
			buffer8[5][bdepth_i + i] = vdata2[5];
			buffer8[6][bdepth_i + i] = vdata2[6];
			buffer8[7][bdepth_i + i] = vdata2[7];
			buffer8[8][bdepth_i + i] = vdata2[8];
			buffer8[9][bdepth_i + i] = vdata2[9];
			buffer8[10][bdepth_i + i] = vdata2[10];
			buffer8[11][bdepth_i + i] = vdata2[11];
			buffer8[12][bdepth_i + i] = vdata2[12];
			buffer8[13][bdepth_i + i] = vdata2[13];
			buffer8[14][bdepth_i + i] = vdata2[14];
			buffer8[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	#pragma HLS array_partition variable = buffer8
	#pragma HLS array_partition variable = buffer9
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
		buffer8[0][i] = 0;
		buffer8[1][i] = 0;
		buffer8[2][i] = 0;
		buffer8[3][i] = 0;
		buffer8[4][i] = 0;
		buffer8[5][i] = 0;
		buffer8[6][i] = 0;
		buffer8[7][i] = 0;
		buffer8[8][i] = 0;
		buffer8[9][i] = 0;
		buffer8[10][i] = 0;
		buffer8[11][i] = 0;
		buffer8[12][i] = 0;
		buffer8[13][i] = 0;
		buffer8[14][i] = 0;
		buffer8[15][i] = 0;
	
		buffer9[0][i] = 0;
		buffer9[1][i] = 0;
		buffer9[2][i] = 0;
		buffer9[3][i] = 0;
		buffer9[4][i] = 0;
		buffer9[5][i] = 0;
		buffer9[6][i] = 0;
		buffer9[7][i] = 0;
		buffer9[8][i] = 0;
		buffer9[9][i] = 0;
		buffer9[10][i] = 0;
		buffer9[11][i] = 0;
		buffer9[12][i] = 0;
		buffer9[13][i] = 0;
		buffer9[14][i] = 0;
		buffer9[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks10 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks10 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
	
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
	
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
	
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
	
			buffer8[0][bdepth_i + i] = vdata2[0];
			buffer8[1][bdepth_i + i] = vdata2[1];
			buffer8[2][bdepth_i + i] = vdata2[2];
			buffer8[3][bdepth_i + i] = vdata2[3];
			buffer8[4][bdepth_i + i] = vdata2[4];
			buffer8[5][bdepth_i + i] = vdata2[5];
			buffer8[6][bdepth_i + i] = vdata2[6];
			buffer8[7][bdepth_i + i] = vdata2[7];
			buffer8[8][bdepth_i + i] = vdata2[8];
			buffer8[9][bdepth_i + i] = vdata2[9];
			buffer8[10][bdepth_i + i] = vdata2[10];
			buffer8[11][bdepth_i + i] = vdata2[11];
			buffer8[12][bdepth_i + i] = vdata2[12];
			buffer8[13][bdepth_i + i] = vdata2[13];
			buffer8[14][bdepth_i + i] = vdata2[14];
			buffer8[15][bdepth_i + i] = vdata2[15];
	
			buffer9[0][bdepth_i + i] = vdata2[0];
			buffer9[1][bdepth_i + i] = vdata2[1];
			buffer9[2][bdepth_i + i] = vdata2[2];
			buffer9[3][bdepth_i + i] = vdata2[3];
			buffer9[4][bdepth_i + i] = vdata2[4];
			buffer9[5][bdepth_i + i] = vdata2[5];
			buffer9[6][bdepth_i + i] = vdata2[6];
			buffer9[7][bdepth_i + i] = vdata2[7];
			buffer9[8][bdepth_i + i] = vdata2[8];
			buffer9[9][bdepth_i + i] = vdata2[9];
			buffer9[10][bdepth_i + i] = vdata2[10];
			buffer9[11][bdepth_i + i] = vdata2[11];
			buffer9[12][bdepth_i + i] = vdata2[12];
			buffer9[13][bdepth_i + i] = vdata2[13];
			buffer9[14][bdepth_i + i] = vdata2[14];
			buffer9[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	#pragma HLS array_partition variable = buffer8
	#pragma HLS array_partition variable = buffer9
	#pragma HLS array_partition variable = buffer10
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
		buffer8[0][i] = 0;
		buffer8[1][i] = 0;
		buffer8[2][i] = 0;
		buffer8[3][i] = 0;
		buffer8[4][i] = 0;
		buffer8[5][i] = 0;
		buffer8[6][i] = 0;
		buffer8[7][i] = 0;
		buffer8[8][i] = 0;
		buffer8[9][i] = 0;
		buffer8[10][i] = 0;
		buffer8[11][i] = 0;
		buffer8[12][i] = 0;
		buffer8[13][i] = 0;
		buffer8[14][i] = 0;
		buffer8[15][i] = 0;
	
		buffer9[0][i] = 0;
		buffer9[1][i] = 0;
		buffer9[2][i] = 0;
		buffer9[3][i] = 0;
		buffer9[4][i] = 0;
		buffer9[5][i] = 0;
		buffer9[6][i] = 0;
		buffer9[7][i] = 0;
		buffer9[8][i] = 0;
		buffer9[9][i] = 0;
		buffer9[10][i] = 0;
		buffer9[11][i] = 0;
		buffer9[12][i] = 0;
		buffer9[13][i] = 0;
		buffer9[14][i] = 0;
		buffer9[15][i] = 0;
	
		buffer10[0][i] = 0;
		buffer10[1][i] = 0;
		buffer10[2][i] = 0;
		buffer10[3][i] = 0;
		buffer10[4][i] = 0;
		buffer10[5][i] = 0;
		buffer10[6][i] = 0;
		buffer10[7][i] = 0;
		buffer10[8][i] = 0;
		buffer10[9][i] = 0;
		buffer10[10][i] = 0;
		buffer10[11][i] = 0;
		buffer10[12][i] = 0;
		buffer10[13][i] = 0;
		buffer10[14][i] = 0;
		buffer10[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks11 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks11 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
	
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
	
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
	
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
	
			buffer8[0][bdepth_i + i] = vdata2[0];
			buffer8[1][bdepth_i + i] = vdata2[1];
			buffer8[2][bdepth_i + i] = vdata2[2];
			buffer8[3][bdepth_i + i] = vdata2[3];
			buffer8[4][bdepth_i + i] = vdata2[4];
			buffer8[5][bdepth_i + i] = vdata2[5];
			buffer8[6][bdepth_i + i] = vdata2[6];
			buffer8[7][bdepth_i + i] = vdata2[7];
			buffer8[8][bdepth_i + i] = vdata2[8];
			buffer8[9][bdepth_i + i] = vdata2[9];
			buffer8[10][bdepth_i + i] = vdata2[10];
			buffer8[11][bdepth_i + i] = vdata2[11];
			buffer8[12][bdepth_i + i] = vdata2[12];
			buffer8[13][bdepth_i + i] = vdata2[13];
			buffer8[14][bdepth_i + i] = vdata2[14];
			buffer8[15][bdepth_i + i] = vdata2[15];
	
			buffer9[0][bdepth_i + i] = vdata2[0];
			buffer9[1][bdepth_i + i] = vdata2[1];
			buffer9[2][bdepth_i + i] = vdata2[2];
			buffer9[3][bdepth_i + i] = vdata2[3];
			buffer9[4][bdepth_i + i] = vdata2[4];
			buffer9[5][bdepth_i + i] = vdata2[5];
			buffer9[6][bdepth_i + i] = vdata2[6];
			buffer9[7][bdepth_i + i] = vdata2[7];
			buffer9[8][bdepth_i + i] = vdata2[8];
			buffer9[9][bdepth_i + i] = vdata2[9];
			buffer9[10][bdepth_i + i] = vdata2[10];
			buffer9[11][bdepth_i + i] = vdata2[11];
			buffer9[12][bdepth_i + i] = vdata2[12];
			buffer9[13][bdepth_i + i] = vdata2[13];
			buffer9[14][bdepth_i + i] = vdata2[14];
			buffer9[15][bdepth_i + i] = vdata2[15];
	
			buffer10[0][bdepth_i + i] = vdata2[0];
			buffer10[1][bdepth_i + i] = vdata2[1];
			buffer10[2][bdepth_i + i] = vdata2[2];
			buffer10[3][bdepth_i + i] = vdata2[3];
			buffer10[4][bdepth_i + i] = vdata2[4];
			buffer10[5][bdepth_i + i] = vdata2[5];
			buffer10[6][bdepth_i + i] = vdata2[6];
			buffer10[7][bdepth_i + i] = vdata2[7];
			buffer10[8][bdepth_i + i] = vdata2[8];
			buffer10[9][bdepth_i + i] = vdata2[9];
			buffer10[10][bdepth_i + i] = vdata2[10];
			buffer10[11][bdepth_i + i] = vdata2[11];
			buffer10[12][bdepth_i + i] = vdata2[12];
			buffer10[13][bdepth_i + i] = vdata2[13];
			buffer10[14][bdepth_i + i] = vdata2[14];
			buffer10[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}
void acts_all::MEMACCESSP0_readANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	#pragma HLS array_partition variable = buffer8
	#pragma HLS array_partition variable = buffer9
	#pragma HLS array_partition variable = buffer10
	#pragma HLS array_partition variable = buffer11
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for (buffer_type i=0; i<1024; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
		buffer8[0][i] = 0;
		buffer8[1][i] = 0;
		buffer8[2][i] = 0;
		buffer8[3][i] = 0;
		buffer8[4][i] = 0;
		buffer8[5][i] = 0;
		buffer8[6][i] = 0;
		buffer8[7][i] = 0;
		buffer8[8][i] = 0;
		buffer8[9][i] = 0;
		buffer8[10][i] = 0;
		buffer8[11][i] = 0;
		buffer8[12][i] = 0;
		buffer8[13][i] = 0;
		buffer8[14][i] = 0;
		buffer8[15][i] = 0;
	
		buffer9[0][i] = 0;
		buffer9[1][i] = 0;
		buffer9[2][i] = 0;
		buffer9[3][i] = 0;
		buffer9[4][i] = 0;
		buffer9[5][i] = 0;
		buffer9[6][i] = 0;
		buffer9[7][i] = 0;
		buffer9[8][i] = 0;
		buffer9[9][i] = 0;
		buffer9[10][i] = 0;
		buffer9[11][i] = 0;
		buffer9[12][i] = 0;
		buffer9[13][i] = 0;
		buffer9[14][i] = 0;
		buffer9[15][i] = 0;
	
		buffer10[0][i] = 0;
		buffer10[1][i] = 0;
		buffer10[2][i] = 0;
		buffer10[3][i] = 0;
		buffer10[4][i] = 0;
		buffer10[5][i] = 0;
		buffer10[6][i] = 0;
		buffer10[7][i] = 0;
		buffer10[8][i] = 0;
		buffer10[9][i] = 0;
		buffer10[10][i] = 0;
		buffer10[11][i] = 0;
		buffer10[12][i] = 0;
		buffer10[13][i] = 0;
		buffer10[14][i] = 0;
		buffer10[15][i] = 0;
	
		buffer11[0][i] = 0;
		buffer11[1][i] = 0;
		buffer11[2][i] = 0;
		buffer11[3][i] = 0;
		buffer11[4][i] = 0;
		buffer11[5][i] = 0;
		buffer11[6][i] = 0;
		buffer11[7][i] = 0;
		buffer11[8][i] = 0;
		buffer11[9][i] = 0;
		buffer11[10][i] = 0;
		buffer11[11][i] = 0;
		buffer11[12][i] = 0;
		buffer11[13][i] = 0;
		buffer11[14][i] = 0;
		buffer11[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks12 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			MEMACCESSP0_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP0_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<endl; }
			if(MEMCAP0_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks12 25", bdepth_i + i, BLOCKRAM_VDATA_SIZE, NAp, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
	
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
	
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
	
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
	
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
	
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
	
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
	
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
	
			buffer8[0][bdepth_i + i] = vdata2[0];
			buffer8[1][bdepth_i + i] = vdata2[1];
			buffer8[2][bdepth_i + i] = vdata2[2];
			buffer8[3][bdepth_i + i] = vdata2[3];
			buffer8[4][bdepth_i + i] = vdata2[4];
			buffer8[5][bdepth_i + i] = vdata2[5];
			buffer8[6][bdepth_i + i] = vdata2[6];
			buffer8[7][bdepth_i + i] = vdata2[7];
			buffer8[8][bdepth_i + i] = vdata2[8];
			buffer8[9][bdepth_i + i] = vdata2[9];
			buffer8[10][bdepth_i + i] = vdata2[10];
			buffer8[11][bdepth_i + i] = vdata2[11];
			buffer8[12][bdepth_i + i] = vdata2[12];
			buffer8[13][bdepth_i + i] = vdata2[13];
			buffer8[14][bdepth_i + i] = vdata2[14];
			buffer8[15][bdepth_i + i] = vdata2[15];
	
			buffer9[0][bdepth_i + i] = vdata2[0];
			buffer9[1][bdepth_i + i] = vdata2[1];
			buffer9[2][bdepth_i + i] = vdata2[2];
			buffer9[3][bdepth_i + i] = vdata2[3];
			buffer9[4][bdepth_i + i] = vdata2[4];
			buffer9[5][bdepth_i + i] = vdata2[5];
			buffer9[6][bdepth_i + i] = vdata2[6];
			buffer9[7][bdepth_i + i] = vdata2[7];
			buffer9[8][bdepth_i + i] = vdata2[8];
			buffer9[9][bdepth_i + i] = vdata2[9];
			buffer9[10][bdepth_i + i] = vdata2[10];
			buffer9[11][bdepth_i + i] = vdata2[11];
			buffer9[12][bdepth_i + i] = vdata2[12];
			buffer9[13][bdepth_i + i] = vdata2[13];
			buffer9[14][bdepth_i + i] = vdata2[14];
			buffer9[15][bdepth_i + i] = vdata2[15];
	
			buffer10[0][bdepth_i + i] = vdata2[0];
			buffer10[1][bdepth_i + i] = vdata2[1];
			buffer10[2][bdepth_i + i] = vdata2[2];
			buffer10[3][bdepth_i + i] = vdata2[3];
			buffer10[4][bdepth_i + i] = vdata2[4];
			buffer10[5][bdepth_i + i] = vdata2[5];
			buffer10[6][bdepth_i + i] = vdata2[6];
			buffer10[7][bdepth_i + i] = vdata2[7];
			buffer10[8][bdepth_i + i] = vdata2[8];
			buffer10[9][bdepth_i + i] = vdata2[9];
			buffer10[10][bdepth_i + i] = vdata2[10];
			buffer10[11][bdepth_i + i] = vdata2[11];
			buffer10[12][bdepth_i + i] = vdata2[12];
			buffer10[13][bdepth_i + i] = vdata2[13];
			buffer10[14][bdepth_i + i] = vdata2[14];
			buffer10[15][bdepth_i + i] = vdata2[15];
	
			buffer11[0][bdepth_i + i] = vdata2[0];
			buffer11[1][bdepth_i + i] = vdata2[1];
			buffer11[2][bdepth_i + i] = vdata2[2];
			buffer11[3][bdepth_i + i] = vdata2[3];
			buffer11[4][bdepth_i + i] = vdata2[4];
			buffer11[5][bdepth_i + i] = vdata2[5];
			buffer11[6][bdepth_i + i] = vdata2[6];
			buffer11[7][bdepth_i + i] = vdata2[7];
			buffer11[8][bdepth_i + i] = vdata2[8];
			buffer11[9][bdepth_i + i] = vdata2[9];
			buffer11[10][bdepth_i + i] = vdata2[10];
			buffer11[11][bdepth_i + i] = vdata2[11];
			buffer11[12][bdepth_i + i] = vdata2[12];
			buffer11[13][bdepth_i + i] = vdata2[13];
			buffer11[14][bdepth_i + i] = vdata2[14];
			buffer11[15][bdepth_i + i] = vdata2[15];
	

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	// exit(EXIT_SUCCESS);
	return;
}

#ifdef RANDOMVERTEXISACTIVE_ALGORITHM_XXXXXXXXXXXXXXXXXXXXXXXXXXx
void acts_all::MEMACCESSP0_RANDreadANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer4, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer4, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer5, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer4, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer5, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer6, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer4, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer5, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer6, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer7, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer4, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer5, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer6, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer7, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer8, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer4, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer5, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer6, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer7, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer8, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer9, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer4, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer5, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer6, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer7, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer8, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer9, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer10, data, 0);
		}
	}
	return;
}
void acts_all::MEMACCESSP0_RANDreadANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, globalparams_t globalparams){		
	
	unsigned int randvids[BLOCKRAM_SIZE];
	
	unsigned int offset = source_partition * SIZEKVS_ACTIVEVERTICESPERSOURCEPARTITION;
	for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		randvids[i] = vdram[globalparams.BASEOFFSETKVS_ACTIVEVERTICES + i].data[0].key;
	}
	
	RANDREADANDRVCHUNKS_LOOP1: for(unsigned int i=0; i<globalparams.NUM_ACTIVE_VERTICES[source_partition]; i++){
		if(randvids[i] != INVALIDDATA){
			unsigned int gsrcv = randvids[i]; // FIXME. might need to be converted first
			unsigned int lsrcv = randvids[i] - voffset; // FIXME. might need to be converted first
			
			value_t data = MEMCAP0_READDATAFROMDRAM(gsrcv, vdram, vbaseoffset_kvs, 0);
			
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer0, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer1, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer2, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer3, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer4, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer5, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer6, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer7, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer8, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer9, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer10, data, 0);
			MEMCAP0_WRITEDATATOBUFFER(lsrcv, buffer11, data, 0);
		}
	}
	return;
}
#endif 

// -------------------- pmasks -------------------- //
void acts_all::MEMACCESSP0_readpmask(uint512_dt * kvdram, pmask_dt pmask[BLOCKRAM_PMASK1_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		pmask[i].data[0] = kvdram[offset_kvs + i].range(31, 0);
		#else
		pmask[i].data[0] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}

// -------------------- stats -------------------- //
void acts_all::MEMACCESSP0_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + NUM_PARTITIONS, globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt vec;
		#ifdef _WIDEWORD
		vec.data[0].key = kvdram[offset_kvs + i].range(31, 0); 
		vec.data[0].value = kvdram[offset_kvs + i].range(63, 32); 
		vec.data[1].key = kvdram[offset_kvs + i].range(95, 64); 
		vec.data[1].value = kvdram[offset_kvs + i].range(127, 96); 
		vec.data[2].key = kvdram[offset_kvs + i].range(159, 128); 
		vec.data[2].value = kvdram[offset_kvs + i].range(191, 160); 
		vec.data[3].key = kvdram[offset_kvs + i].range(223, 192); 
		vec.data[3].value = kvdram[offset_kvs + i].range(255, 224); 
		vec.data[4].key = kvdram[offset_kvs + i].range(287, 256); 
		vec.data[4].value = kvdram[offset_kvs + i].range(319, 288); 
		#else 
		vec.data[0].key = kvdram[offset_kvs + i].data[0].key; 
		vec.data[0].value = kvdram[offset_kvs + i].data[0].value; 
		vec.data[1].key = kvdram[offset_kvs + i].data[1].key; 
		vec.data[1].value = kvdram[offset_kvs + i].data[1].value; 
		vec.data[2].key = kvdram[offset_kvs + i].data[2].key; 
		vec.data[2].value = kvdram[offset_kvs + i].data[2].value; 
		vec.data[3].key = kvdram[offset_kvs + i].data[3].key; 
		vec.data[3].value = kvdram[offset_kvs + i].data[3].value; 
		vec.data[4].key = kvdram[offset_kvs + i].data[4].key; 
		vec.data[4].value = kvdram[offset_kvs + i].data[4].value; 
		#endif 
		globalstatsbuffer[i] = vec.data[globalparams.VARS_WORKBATCH];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("readglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

void acts_all::MEMACCESSP0_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + NUM_PARTITIONS, globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
	#endif
	
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
 if(globalparams.VARS_WORKBATCH == 0){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(31, 0) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(63, 32) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[0].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[0].value = globalstatsbuffer[i].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 1){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(95, 64) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(127, 96) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[1].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[1].value = globalstatsbuffer[i].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 2){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(159, 128) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(191, 160) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[2].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[2].value = globalstatsbuffer[i].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 3){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(223, 192) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(255, 224) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[3].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[3].value = globalstatsbuffer[i].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 4){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(287, 256) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(319, 288) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[4].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[4].value = globalstatsbuffer[i].value; 
			#endif 
		}
		else {
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"MEMACCESSP0_saveglobalstats: NOT IMPLEMENTED (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printkeyvalues("MEMACCESSP0_saveglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

tuple_t acts_all::MEMACCESSP0_getvptrs(uint512_dt * kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
	#pragma HLS INLINE
	
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;

	#ifdef _WIDEWORD
	beginvptr = kvdram[beginoffset].range(31, 0);
	endvptr = kvdram[endoffset].range(31, 0); 
	#else 
	beginvptr = kvdram[beginoffset].data[0].key;
	endvptr = kvdram[endoffset].data[0].key;
	#endif 

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

unsigned int acts_all::MEMACCESSP0_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
	keyvalue_t vptr_kv;
	
	uint512_dt V = kvdram[baseoffset_kvs + (offset / VECTOR2_SIZE)];
	unsigned int M = (offset % VECTOR2_SIZE) / 2;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP0_getvptr: baseoffset_kvs: "<<baseoffset_kvs<<", offset: "<<offset<<", baseoffset_kvs + (offset / VECTOR2_SIZE): "<<baseoffset_kvs + (offset / VECTOR2_SIZE)<<endl;
	
	#ifdef _WIDEWORD
 if(M == 0){
		vptr_kv.key = V.range(31, 0); 
		vptr_kv.value = V.range(63, 32); 
	}
else if(M == 1){
		vptr_kv.key = V.range(95, 64); 
		vptr_kv.value = V.range(127, 96); 
	}
else if(M == 2){
		vptr_kv.key = V.range(159, 128); 
		vptr_kv.value = V.range(191, 160); 
	}
else if(M == 3){
		vptr_kv.key = V.range(223, 192); 
		vptr_kv.value = V.range(255, 224); 
	}
else if(M == 4){
		vptr_kv.key = V.range(287, 256); 
		vptr_kv.value = V.range(319, 288); 
	}
else if(M == 5){
		vptr_kv.key = V.range(351, 320); 
		vptr_kv.value = V.range(383, 352); 
	}
else if(M == 6){
		vptr_kv.key = V.range(415, 384); 
		vptr_kv.value = V.range(447, 416); 
	}
else {
		vptr_kv.key = V.range(479, 448); 
		vptr_kv.value = V.range(511, 480); 
	}
	#else 
 if(M == 0){
		vptr_kv.key = V.data[0].key; 
		vptr_kv.value = V.data[0].value; 
	}
else if(M == 1){
		vptr_kv.key = V.data[1].key; 
		vptr_kv.value = V.data[1].value; 
	}
else if(M == 2){
		vptr_kv.key = V.data[2].key; 
		vptr_kv.value = V.data[2].value; 
	}
else if(M == 3){
		vptr_kv.key = V.data[3].key; 
		vptr_kv.value = V.data[3].value; 
	}
else if(M == 4){
		vptr_kv.key = V.data[4].key; 
		vptr_kv.value = V.data[4].value; 
	}
else if(M == 5){
		vptr_kv.key = V.data[5].key; 
		vptr_kv.value = V.data[5].value; 
	}
else if(M == 6){
		vptr_kv.key = V.data[6].key; 
		vptr_kv.value = V.data[6].value; 
	}
else {
		vptr_kv.key = V.data[7].key; 
		vptr_kv.value = V.data[7].value; 
	}
	#endif
	
	if(offset % 2 == 0){ return vptr_kv.key; }
	else { return vptr_kv.value; }
}

tuple_t acts_all::MEMACCESSP0_getvptrs_opt( uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
	#pragma HLS INLINE
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	beginoffset = beginoffset / VPTR_SHRINK_RATIO; // convert-to-appropriate-skip-format
	endoffset = endoffset / VPTR_SHRINK_RATIO;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP0_getvptrs_opt: beginoffset: "<<beginoffset<<", endoffset: "<<endoffset<<", VPTR_SHRINK_RATIO: "<<VPTR_SHRINK_RATIO<<endl;
	
	beginvptr = MEMACCESSP0_getvptr(kvdram, baseoffset_kvs, beginoffset);
	endvptr = MEMACCESSP0_getvptr(kvdram, baseoffset_kvs, endoffset);

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

void acts_all::MEMACCESSP0_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset){
	unsigned int totalnumpartitionsb4last = 0;
	RETRIEVEKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (NUM_PARTITIONS_POW * k)); }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset  + k].data[0].value = buffer[k]; 
		#endif
	}
	
	#ifdef _WIDEWORD
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	#else
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}

void acts_all::MEMACCESSP0_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset  + k].data[0].value = buffer[k]; 
		#endif
	}
	return;
}

void acts_all::MEMACCESSP0_commitkvstats2(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k].key; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value = buffer[k].key; 
		#endif
	}
	return;
}

void acts_all::MEMACCESSP0_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[k] = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32); 
		#else
		buffer[k] = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value; 
		#endif
	}
}

void acts_all::MEMACCESSP0_retreievekvstats(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[k].value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32); 
		#else
		buffer[k].value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value; 
		#endif
	}
}

void acts_all::MEMACCESSP0_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	SAVEKVSTATS_LOOP1: for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpartitionsb4last += (1 << (NUM_PARTITIONS_POW * k)); }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
 if(globalparams.VARS_WORKBATCH == 0){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value; 	
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 1){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(127, 96); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[1].value; 	
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 2){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(191, 160); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[2].value; 	
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 3){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(255, 224); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[3].value; 	
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 4){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(319, 288); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[4].value; 	
			#endif 
		}
		else {
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"MEMACCESSP0_accumkvstats: NOT IMPLEMENTED (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}

void acts_all::MEMACCESSP0_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	RETRIEVEKVSTATS_LOOP1: for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpartitionsb4last += (1 << (NUM_PARTITIONS_POW * k)); }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value = buffer[k]; 
		#endif
	}
	
	#ifdef _WIDEWORD
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	#else
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}

// -------------------- multiple accesses -------------------- //
void acts_all::MEMACCESSP0_readmanypmask1(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask2(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask3(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask4(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask5(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE],pmask_dt pmask4[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
		pmask4[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask6(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE],pmask_dt pmask4[BLOCKRAM_PMASK_SIZE],pmask_dt pmask5[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
		pmask4[i] = pmask0[i];
		pmask5[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask7(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE],pmask_dt pmask4[BLOCKRAM_PMASK_SIZE],pmask_dt pmask5[BLOCKRAM_PMASK_SIZE],pmask_dt pmask6[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
		pmask4[i] = pmask0[i];
		pmask5[i] = pmask0[i];
		pmask6[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask8(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE],pmask_dt pmask4[BLOCKRAM_PMASK_SIZE],pmask_dt pmask5[BLOCKRAM_PMASK_SIZE],pmask_dt pmask6[BLOCKRAM_PMASK_SIZE],pmask_dt pmask7[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
		pmask4[i] = pmask0[i];
		pmask5[i] = pmask0[i];
		pmask6[i] = pmask0[i];
		pmask7[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask9(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE],pmask_dt pmask4[BLOCKRAM_PMASK_SIZE],pmask_dt pmask5[BLOCKRAM_PMASK_SIZE],pmask_dt pmask6[BLOCKRAM_PMASK_SIZE],pmask_dt pmask7[BLOCKRAM_PMASK_SIZE],pmask_dt pmask8[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
		pmask4[i] = pmask0[i];
		pmask5[i] = pmask0[i];
		pmask6[i] = pmask0[i];
		pmask7[i] = pmask0[i];
		pmask8[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask10(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE],pmask_dt pmask4[BLOCKRAM_PMASK_SIZE],pmask_dt pmask5[BLOCKRAM_PMASK_SIZE],pmask_dt pmask6[BLOCKRAM_PMASK_SIZE],pmask_dt pmask7[BLOCKRAM_PMASK_SIZE],pmask_dt pmask8[BLOCKRAM_PMASK_SIZE],pmask_dt pmask9[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
		pmask4[i] = pmask0[i];
		pmask5[i] = pmask0[i];
		pmask6[i] = pmask0[i];
		pmask7[i] = pmask0[i];
		pmask8[i] = pmask0[i];
		pmask9[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask11(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE],pmask_dt pmask4[BLOCKRAM_PMASK_SIZE],pmask_dt pmask5[BLOCKRAM_PMASK_SIZE],pmask_dt pmask6[BLOCKRAM_PMASK_SIZE],pmask_dt pmask7[BLOCKRAM_PMASK_SIZE],pmask_dt pmask8[BLOCKRAM_PMASK_SIZE],pmask_dt pmask9[BLOCKRAM_PMASK_SIZE],pmask_dt pmask10[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
		pmask4[i] = pmask0[i];
		pmask5[i] = pmask0[i];
		pmask6[i] = pmask0[i];
		pmask7[i] = pmask0[i];
		pmask8[i] = pmask0[i];
		pmask9[i] = pmask0[i];
		pmask10[i] = pmask0[i];
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask12(uint512_dt * vdram, pmask_dt pmask0[BLOCKRAM_PMASK_SIZE],pmask_dt pmask1[BLOCKRAM_PMASK_SIZE],pmask_dt pmask2[BLOCKRAM_PMASK_SIZE],pmask_dt pmask3[BLOCKRAM_PMASK_SIZE],pmask_dt pmask4[BLOCKRAM_PMASK_SIZE],pmask_dt pmask5[BLOCKRAM_PMASK_SIZE],pmask_dt pmask6[BLOCKRAM_PMASK_SIZE],pmask_dt pmask7[BLOCKRAM_PMASK_SIZE],pmask_dt pmask8[BLOCKRAM_PMASK_SIZE],pmask_dt pmask9[BLOCKRAM_PMASK_SIZE],pmask_dt pmask10[BLOCKRAM_PMASK_SIZE],pmask_dt pmask11[BLOCKRAM_PMASK_SIZE], batch_type offset_kvs, batch_type size_kvs){
	unit1_type temppmask[32];
	#pragma HLS DATA_PACK variable = temppmask
	temppmask[0] = 0;
	temppmask[1] = 0;
	temppmask[2] = 0;
	temppmask[3] = 0;
	temppmask[4] = 0;
	temppmask[5] = 0;
	temppmask[6] = 0;
	temppmask[7] = 0;
	temppmask[8] = 0;
	temppmask[9] = 0;
	temppmask[10] = 0;
	temppmask[11] = 0;
	temppmask[12] = 0;
	temppmask[13] = 0;
	temppmask[14] = 0;
	temppmask[15] = 0;
	temppmask[16] = 0;
	temppmask[17] = 0;
	temppmask[18] = 0;
	temppmask[19] = 0;
	temppmask[20] = 0;
	temppmask[21] = 0;
	temppmask[22] = 0;
	temppmask[23] = 0;
	temppmask[24] = 0;
	temppmask[25] = 0;
	temppmask[26] = 0;
	temppmask[27] = 0;
	temppmask[28] = 0;
	temppmask[29] = 0;
	temppmask[30] = 0;
	temppmask[31] = 0;
	
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		uint32_type temp;
		#ifdef _WIDEWORD
		temp = vdram[offset_kvs + i].range(31, 0);
		#else
		temp = vdram[offset_kvs + i].data[0].key;
		#endif 
		
		pmask0[i].data[0] = 
			#ifdef _WIDEWORD
			temp.range(0, 0);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 0, 1);	
			#endif 
		pmask0[i].data[1] = 
			#ifdef _WIDEWORD
			temp.range(1, 1);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 1, 1);	
			#endif 
		pmask0[i].data[2] = 
			#ifdef _WIDEWORD
			temp.range(2, 2);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 2, 1);	
			#endif 
		pmask0[i].data[3] = 
			#ifdef _WIDEWORD
			temp.range(3, 3);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 3, 1);	
			#endif 
		pmask0[i].data[4] = 
			#ifdef _WIDEWORD
			temp.range(4, 4);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 4, 1);	
			#endif 
		pmask0[i].data[5] = 
			#ifdef _WIDEWORD
			temp.range(5, 5);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 5, 1);	
			#endif 
		pmask0[i].data[6] = 
			#ifdef _WIDEWORD
			temp.range(6, 6);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 6, 1);	
			#endif 
		pmask0[i].data[7] = 
			#ifdef _WIDEWORD
			temp.range(7, 7);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 7, 1);	
			#endif 
		pmask0[i].data[8] = 
			#ifdef _WIDEWORD
			temp.range(8, 8);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 8, 1);	
			#endif 
		pmask0[i].data[9] = 
			#ifdef _WIDEWORD
			temp.range(9, 9);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 9, 1);	
			#endif 
		pmask0[i].data[10] = 
			#ifdef _WIDEWORD
			temp.range(10, 10);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 10, 1);	
			#endif 
		pmask0[i].data[11] = 
			#ifdef _WIDEWORD
			temp.range(11, 11);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 11, 1);	
			#endif 
		pmask0[i].data[12] = 
			#ifdef _WIDEWORD
			temp.range(12, 12);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 12, 1);	
			#endif 
		pmask0[i].data[13] = 
			#ifdef _WIDEWORD
			temp.range(13, 13);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 13, 1);	
			#endif 
		pmask0[i].data[14] = 
			#ifdef _WIDEWORD
			temp.range(14, 14);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 14, 1);	
			#endif 
		pmask0[i].data[15] = 
			#ifdef _WIDEWORD
			temp.range(15, 15);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 15, 1);	
			#endif 
		pmask0[i].data[16] = 
			#ifdef _WIDEWORD
			temp.range(16, 16);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 16, 1);	
			#endif 
		pmask0[i].data[17] = 
			#ifdef _WIDEWORD
			temp.range(17, 17);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 17, 1);	
			#endif 
		pmask0[i].data[18] = 
			#ifdef _WIDEWORD
			temp.range(18, 18);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 18, 1);	
			#endif 
		pmask0[i].data[19] = 
			#ifdef _WIDEWORD
			temp.range(19, 19);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 19, 1);	
			#endif 
		pmask0[i].data[20] = 
			#ifdef _WIDEWORD
			temp.range(20, 20);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 20, 1);	
			#endif 
		pmask0[i].data[21] = 
			#ifdef _WIDEWORD
			temp.range(21, 21);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 21, 1);	
			#endif 
		pmask0[i].data[22] = 
			#ifdef _WIDEWORD
			temp.range(22, 22);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 22, 1);	
			#endif 
		pmask0[i].data[23] = 
			#ifdef _WIDEWORD
			temp.range(23, 23);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 23, 1);	
			#endif 
		pmask0[i].data[24] = 
			#ifdef _WIDEWORD
			temp.range(24, 24);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 24, 1);	
			#endif 
		pmask0[i].data[25] = 
			#ifdef _WIDEWORD
			temp.range(25, 25);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 25, 1);	
			#endif 
		pmask0[i].data[26] = 
			#ifdef _WIDEWORD
			temp.range(26, 26);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 26, 1);	
			#endif 
		pmask0[i].data[27] = 
			#ifdef _WIDEWORD
			temp.range(27, 27);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 27, 1);	
			#endif 
		pmask0[i].data[28] = 
			#ifdef _WIDEWORD
			temp.range(28, 28);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 28, 1);	
			#endif 
		pmask0[i].data[29] = 
			#ifdef _WIDEWORD
			temp.range(29, 29);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 29, 1);	
			#endif 
		pmask0[i].data[30] = 
			#ifdef _WIDEWORD
			temp.range(30, 30);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 30, 1);	
			#endif 
		pmask0[i].data[31] = 
			#ifdef _WIDEWORD
			temp.range(31, 31);
			#else 
			UTILP0_READBITSFROM_UINTV(temp, 31, 1);	
			#endif 
	}
	
	READMANYPMASKS_LOOP2: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		pmask0[i] = pmask0[i];
		pmask1[i] = pmask0[i];
		pmask2[i] = pmask0[i];
		pmask3[i] = pmask0[i];
		pmask4[i] = pmask0[i];
		pmask5[i] = pmask0[i];
		pmask6[i] = pmask0[i];
		pmask7[i] = pmask0[i];
		pmask8[i] = pmask0[i];
		pmask9[i] = pmask0[i];
		pmask10[i] = pmask0[i];
		pmask11[i] = pmask0[i];
	}
	return;
}

// -------------------- others -------------------- //
void acts_all::MEMACCESSP0_copyvs(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], globalparams_t globalparamsK){
	// NOT IMPLEMENTED.
	/* analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;
	
	travstate_t rtravstate;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);
	#else
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH + 1;
	batch_type num_source_partitions = NUM_PARTITIONS;
	#endif
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2; // 512
	
	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	LOADSRCVS_LOOP1: for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	LOADSRCVS_LOOP1: for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	
	unsigned int gmask_buffer[BLOCKRAM_SIZE]; // AUTOMATEME.
	LOADSRCVS_LOOP2: for(unsigned int iterationidx=0; iterationidx<num_source_partitions; iterationidx++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		gmask_buffer[iterationidx] = kvdram[globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + iterationidx].range(31, 0);
		#else 
		gmask_buffer[iterationidx] = kvdram[globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + iterationidx].data[0].key;
		#endif
	}
	
	LOADSRCVS_LOOP3: for(batch_type iterationidx=0; iterationidx<num_source_partitions; iterationidx+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		if(gmask_buffer[iterationidx] > 0){
			readvdata(ON, kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + (iterationidx * reducebuffersz * FETFACTOR), vbuffer, 0, 0, reducebuffersz, globalparamsK);
			readvdata(ON, kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + (iterationidx * reducebuffersz * FETFACTOR) + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparamsK);
			
			savevdata(ON, kvdram, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + (iterationidx * reducebuffersz * FETFACTOR), vbuffer, 0, 0, reducebuffersz, globalparamsK);
			savevdata(ON, kvdram, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + (iterationidx * reducebuffersz * FETFACTOR) + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparamsK);
		}
	} */
	return;
}

