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
	
	#ifdef _DEBUGMODE_CHECKS
	actsutilityobj->printkeyvalues("savekeyvalues::localcapsule", localcapsule, MAX_NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues::localcapsule", localcapsule, MAX_NUM_PARTITIONS);
	actsutilityobj->scankeyvalues("savekeyvalues::buffer", (keyvalue_t *)buffer, localcapsule, NUM_PARTITIONS, globalparams.SIZE_BATCHRANGE / NUM_PARTITIONS, actsutilityobj->getsweepparams().upperlimit);
	#endif
	
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

void acts_all::MEMACCESSP0_readkeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].data[0].key;
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].data[1].key;
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].data[2].key;
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].data[3].key;
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].data[4].key;
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].data[5].key;
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].data[6].key;
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].data[7].key;
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].data[7].value; 
		
		buffer[0][bufferoffset_kvs + i] = UTILP0_GETKV(mykeyvalue0);
		buffer[1][bufferoffset_kvs + i] = UTILP0_GETKV(mykeyvalue1);
		buffer[2][bufferoffset_kvs + i] = UTILP0_GETKV(mykeyvalue2);
		buffer[3][bufferoffset_kvs + i] = UTILP0_GETKV(mykeyvalue3);
		buffer[4][bufferoffset_kvs + i] = UTILP0_GETKV(mykeyvalue4);
		buffer[5][bufferoffset_kvs + i] = UTILP0_GETKV(mykeyvalue5);
		buffer[6][bufferoffset_kvs + i] = UTILP0_GETKV(mykeyvalue6);
		buffer[7][bufferoffset_kvs + i] = UTILP0_GETKV(mykeyvalue7);
		
		#endif 
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readkeyvalues:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	#endif
	return;
}

void acts_all::MEMACCESSP0_savekeyvalues(bool_type enable, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_buffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  BLOCKRAM_SIZE;
	
	SAVEKEYVALUES2_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0 = UTILP0_GETKV(buffer[0][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue1 = UTILP0_GETKV(buffer[1][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue2 = UTILP0_GETKV(buffer[2][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue3 = UTILP0_GETKV(buffer[3][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue4 = UTILP0_GETKV(buffer[4][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue5 = UTILP0_GETKV(buffer[5][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue6 = UTILP0_GETKV(buffer[6][bufferoffset_kvs + i]);
		keyvalue_t mykeyvalue7 = UTILP0_GETKV(buffer[7][bufferoffset_kvs + i]);
	
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
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<dramoffset_kvs-globalparams.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE<<"-"<<((dramoffset_kvs + size_kvs)-globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_DESTVERTICESDATA && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"savekeyvalues:: vertices saved: offset: "<<(dramoffset_kvs-globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<((dramoffset_kvs-globalparams.BASEOFFSETKVS_DESTVERTICESDATA) + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }					
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

void acts_all::MEMACCESSP0_RearrangeLayoutV(unsigned int s, vmdata_t vdata[VECTOR2_SIZE], vmdata_t vdata2[VECTOR2_SIZE]){
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
else if(s_==2){ 
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
else if(s_==3){ 
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
else if(s_==4){ 
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
else if(s_==5){ 
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
else if(s_==6){ 
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
else if(s_==7){ 
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
else if(s_==10){ 
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
else if(s_==11){ 
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
else if(s_==12){ 
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
else if(s_==13){ 
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
else if(s_==14){ 
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
else { 
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
	
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		unsigned int index = i; unsigned int j = i%2;
		MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, baseoffset_kvs, offset_kvs);
		if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
			
		if(j==1){ 
			vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
			vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
			vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
			vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
			vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
			vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
			vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
			vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
			vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
			vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
			vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
			vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
			vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
			vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
			vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
			vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
			MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS(index, buffer, vsandmsdata, 0);
		}
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
		#endif
	}
	return;
}

void acts_all::MEMACCESSP0_SLreadV(bool_type enable, unsigned int s, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / NUM_PEs;

	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	
	vmdata_t vandmdata1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	
	unsigned int index2 = 0;
	
	SLREADVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		unsigned int index = i; unsigned int j = i%2;
		MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, baseoffset_kvs, offset_kvs);
		unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
		MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
		if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
		if(j==1){ 
			vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
			// if(vsandmsdata[0].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[0].vmdata0.vmask: "<<vsandmsdata[0].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[0].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[0].vmdata1.vmask: "<<vsandmsdata[0].vmdata1.vmask<<endl; }	
			vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
			// if(vsandmsdata[1].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[1].vmdata0.vmask: "<<vsandmsdata[1].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[1].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[1].vmdata1.vmask: "<<vsandmsdata[1].vmdata1.vmask<<endl; }	
			vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
			// if(vsandmsdata[2].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[2].vmdata0.vmask: "<<vsandmsdata[2].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[2].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[2].vmdata1.vmask: "<<vsandmsdata[2].vmdata1.vmask<<endl; }	
			vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
			// if(vsandmsdata[3].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[3].vmdata0.vmask: "<<vsandmsdata[3].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[3].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[3].vmdata1.vmask: "<<vsandmsdata[3].vmdata1.vmask<<endl; }	
			vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
			// if(vsandmsdata[4].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[4].vmdata0.vmask: "<<vsandmsdata[4].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[4].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[4].vmdata1.vmask: "<<vsandmsdata[4].vmdata1.vmask<<endl; }	
			vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
			// if(vsandmsdata[5].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[5].vmdata0.vmask: "<<vsandmsdata[5].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[5].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[5].vmdata1.vmask: "<<vsandmsdata[5].vmdata1.vmask<<endl; }	
			vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
			// if(vsandmsdata[6].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[6].vmdata0.vmask: "<<vsandmsdata[6].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[6].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[6].vmdata1.vmask: "<<vsandmsdata[6].vmdata1.vmask<<endl; }	
			vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
			// if(vsandmsdata[7].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[7].vmdata0.vmask: "<<vsandmsdata[7].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[7].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[7].vmdata1.vmask: "<<vsandmsdata[7].vmdata1.vmask<<endl; }	
			vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
			// if(vsandmsdata[8].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[8].vmdata0.vmask: "<<vsandmsdata[8].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[8].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[8].vmdata1.vmask: "<<vsandmsdata[8].vmdata1.vmask<<endl; }	
			vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
			// if(vsandmsdata[9].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[9].vmdata0.vmask: "<<vsandmsdata[9].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[9].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[9].vmdata1.vmask: "<<vsandmsdata[9].vmdata1.vmask<<endl; }	
			vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
			// if(vsandmsdata[10].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[10].vmdata0.vmask: "<<vsandmsdata[10].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[10].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[10].vmdata1.vmask: "<<vsandmsdata[10].vmdata1.vmask<<endl; }	
			vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
			// if(vsandmsdata[11].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[11].vmdata0.vmask: "<<vsandmsdata[11].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[11].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[11].vmdata1.vmask: "<<vsandmsdata[11].vmdata1.vmask<<endl; }	
			vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
			// if(vsandmsdata[12].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[12].vmdata0.vmask: "<<vsandmsdata[12].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[12].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[12].vmdata1.vmask: "<<vsandmsdata[12].vmdata1.vmask<<endl; }	
			vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
			// if(vsandmsdata[13].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[13].vmdata0.vmask: "<<vsandmsdata[13].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[13].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[13].vmdata1.vmask: "<<vsandmsdata[13].vmdata1.vmask<<endl; }	
			vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
			// if(vsandmsdata[14].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[14].vmdata0.vmask: "<<vsandmsdata[14].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[14].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[14].vmdata1.vmask: "<<vsandmsdata[14].vmdata1.vmask<<endl; }	
			vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
			// if(vsandmsdata[15].vmdata0.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[15].vmdata0.vmask: "<<vsandmsdata[15].vmdata0.vmask<<endl; }	
			// if(vsandmsdata[15].vmdata1.vmask > 0){ cout<<"------------- MEMACCESSP0_SLreadV:: vsandmsdata[15].vmdata1.vmask: "<<vsandmsdata[15].vmdata1.vmask<<endl; }	
		
			MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, buffer, vsandmsdata, 0);
		}
		index2 += NUM_PEs;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savevdata:: vertices saved: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	return;
}

void acts_all::MEMACCESSP0_readVchunks(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	// read vertices data
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME?
	#pragma HLS array_partition variable = tempbuffer
	READVDATACHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		MEMACCESSP0_SLreadV(enable, s, kvdram, tempbuffer, vbaseoffset_kvs, depth_i + voffset_kvs, bdepth_i, vsz_kvs, globalparams);
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS(index, buffer, vsandmsdata, 0);	
			}
		}
	}
	return;	
}

void acts_all::MEMACCESSP0_saveVandMs(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / 2;
	
	keyvalue_vbuffer_t vdatas[VECTOR2_SIZE];
	vmdata_t vandmdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata complete
	
	SAVEVDATAANDMASKS_LOOP: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		// CRITICAL FIXME.
		MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(i, buffer, vandmdata, 0);
		MEMCAP0_WRITETOKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata, baseoffset_kvs, offset_kvs);

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countvswritten(VECTOR2_SIZE);
		#endif
	}
	return;
}

void acts_all::MEMACCESSP0_readANDRVchunks1(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS1_ANDREPLICATE(depths, buffer0, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS1_ANDREPLICATE(index, buffer0, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks2(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS2_ANDREPLICATE(depths, buffer0,buffer1, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS2_ANDREPLICATE(index, buffer0,buffer1, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks3(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS3_ANDREPLICATE(depths, buffer0,buffer1,buffer2, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS3_ANDREPLICATE(index, buffer0,buffer1,buffer2, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks4(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS4_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS4_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks5(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS5_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3,buffer4, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS5_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3,buffer4, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks6(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS6_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS6_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks7(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS7_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS7_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks8(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS8_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS8_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks9(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS9_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7,buffer8, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS9_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7,buffer8, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks10(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS10_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7,buffer8,buffer9, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS10_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7,buffer8,buffer9, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks11(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS11_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7,buffer8,buffer9,buffer10, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS11_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7,buffer8,buffer9,buffer10, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 
void acts_all::MEMACCESSP0_readANDRVchunks12(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams)
#ifdef CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer
	
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
	
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			if(j==0){ vandmdataM[0] = vandmdata1[0];vandmdataM[1] = vandmdata1[1];vandmdataM[2] = vandmdata1[2];vandmdataM[3] = vandmdata1[3];vandmdataM[4] = vandmdata1[4];vandmdataM[5] = vandmdata1[5];vandmdataM[6] = vandmdata1[6];vandmdataM[7] = vandmdata1[7];vandmdataM[8] = vandmdata1[8];vandmdataM[9] = vandmdata1[9];vandmdataM[10] = vandmdata1[10];vandmdataM[11] = vandmdata1[11];vandmdataM[12] = vandmdata1[12];vandmdataM[13] = vandmdata1[13];vandmdataM[14] = vandmdata1[14];vandmdataM[15] = vandmdata1[15];} else { vandmdataN[0] = vandmdata1[0];vandmdataN[1] = vandmdata1[1];vandmdataN[2] = vandmdata1[2];vandmdataN[3] = vandmdata1[3];vandmdataN[4] = vandmdata1[4];vandmdataN[5] = vandmdata1[5];vandmdataN[6] = vandmdata1[6];vandmdataN[7] = vandmdata1[7];vandmdataN[8] = vandmdata1[8];vandmdataN[9] = vandmdata1[9];vandmdataN[10] = vandmdata1[10];vandmdataN[11] = vandmdata1[11];vandmdataN[12] = vandmdata1[12];vandmdataN[13] = vandmdata1[13];vandmdataN[14] = vandmdata1[14];vandmdataN[15] = vandmdata1[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
		
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS12_ANDREPLICATE(depths, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7,buffer8,buffer9,buffer10,buffer11, vsandmsdata, 0);	
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
#endif 
#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
{
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_VDATA_SIZE]; // CRITICAL REMOVEME.
	#pragma HLS array_partition variable = tempbuffer

	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			unsigned int index = i; unsigned int j = i%2;
			MEMCAP0_READFROMKVDRAM_VDATASANDVMASKS(i, kvdram, vandmdata1, vbaseoffset_kvs, depth_i + voffset_kvs);
			unsigned int x = index2 + s; basedepth = (x / VECTOR2_SIZE) * VECTOR2_SIZE;
			MEMACCESSP0_GetXYLayoutV(x, vandmdata1, vandmdata2, depths, basedepth);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
				
			if(j==1){ 
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);	
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);	
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);	
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);	
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);	
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);	
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);	
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);	
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);	
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);	
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);	
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);	
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);	
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);	
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);	
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);	
				MEMCAP0_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(depths, tempbuffer, vsandmsdata, 0);
			}
			index2 += NUM_PEs;
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	// re-arrange vertices data 
	vmdata_t vandmdata1[VECTOR2_SIZE];
	vmdata_t vandmdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vandmdata2 complete
	vmdata_t vandmdataM[VECTOR2_SIZE]; 
	vmdata_t vandmdataN[VECTOR2_SIZE]; 
	keyvalue_vbuffer_t vsandmsdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vandmdataM complete
	#pragma HLS ARRAY_PARTITION variable=vandmdataN complete
	READVDATACHUNKS_REARRANGE_LOOP1: for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i+=2){
	#pragma HLS PIPELINE II=1 
	// #pragma HLS PIPELINE II=8 // CRITICAL REMOVEME.//////////////////////////////////////
		READVDATACHUNKS_REARRANGE_LOOP1B: for(unsigned int j=0; j<2; j++){
			unsigned int index = i+j;
			MEMCAP0_READFROMBUFFER_VDATASANDVMASKS(index, tempbuffer, vandmdata1, 0);	
			MEMACCESSP0_RearrangeLayoutV(voffset_kvs + index, vandmdata1, vandmdata2);
			if(j==0){ vandmdataM[0] = vandmdata2[0];vandmdataM[1] = vandmdata2[1];vandmdataM[2] = vandmdata2[2];vandmdataM[3] = vandmdata2[3];vandmdataM[4] = vandmdata2[4];vandmdataM[5] = vandmdata2[5];vandmdataM[6] = vandmdata2[6];vandmdataM[7] = vandmdata2[7];vandmdataM[8] = vandmdata2[8];vandmdataM[9] = vandmdata2[9];vandmdataM[10] = vandmdata2[10];vandmdataM[11] = vandmdata2[11];vandmdataM[12] = vandmdata2[12];vandmdataM[13] = vandmdata2[13];vandmdataM[14] = vandmdata2[14];vandmdataM[15] = vandmdata2[15];} else { vandmdataN[0] = vandmdata2[0];vandmdataN[1] = vandmdata2[1];vandmdataN[2] = vandmdata2[2];vandmdataN[3] = vandmdata2[3];vandmdataN[4] = vandmdata2[4];vandmdataN[5] = vandmdata2[5];vandmdataN[6] = vandmdata2[6];vandmdataN[7] = vandmdata2[7];vandmdataN[8] = vandmdata2[8];vandmdataN[9] = vandmdata2[9];vandmdataN[10] = vandmdata2[10];vandmdataN[11] = vandmdata2[11];vandmdataN[12] = vandmdata2[12];vandmdataN[13] = vandmdata2[13];vandmdataN[14] = vandmdata2[14];vandmdataN[15] = vandmdata2[15];	  }
			
			if(j==1){
				vsandmsdata[0] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[0], vandmdataN[0]);
				vsandmsdata[1] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[1], vandmdataN[1]);
				vsandmsdata[2] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[2], vandmdataN[2]);
				vsandmsdata[3] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[3], vandmdataN[3]);
				vsandmsdata[4] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[4], vandmdataN[4]);
				vsandmsdata[5] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[5], vandmdataN[5]);
				vsandmsdata[6] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[6], vandmdataN[6]);
				vsandmsdata[7] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[7], vandmdataN[7]);
				vsandmsdata[8] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[8], vandmdataN[8]);
				vsandmsdata[9] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[9], vandmdataN[9]);
				vsandmsdata[10] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[10], vandmdataN[10]);
				vsandmsdata[11] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[11], vandmdataN[11]);
				vsandmsdata[12] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[12], vandmdataN[12]);
				vsandmsdata[13] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[13], vandmdataN[13]);
				vsandmsdata[14] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[14], vandmdataN[14]);
				vsandmsdata[15] = MEMCAP0_CREATEVBUFFERSTRUCT(vandmdataM[15], vandmdataN[15]);
	
				MEMCAP0_WRITETOBUFFER_VDATASANDVMASKS12_ANDREPLICATE(index, buffer0,buffer1,buffer2,buffer3,buffer4,buffer5,buffer6,buffer7,buffer8,buffer9,buffer10,buffer11, vsandmsdata, 0);	
			}
		}
	}
	return;	
}
#endif 

// -------------------- pmasks -------------------- //
void acts_all::MEMACCESSP0_readpmask(uint512_dt * kvdram, uint32_type vmask_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}

// -------------------- stats -------------------- //
void acts_all::MEMACCESSP0_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + NUM_PARTITIONS, globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
		#if NUM_EDGE_BANKS==0
			keyvalue_t kv;
			#ifdef _WIDEWORD
			kv.key = kvdram[offset_kvs + i].range(31, 0); 
			kv.value = kvdram[offset_kvs + i].range(63, 32); 
			#else 
			kv.key = kvdram[offset_kvs + i].data[0].key;
			kv.value = kvdram[offset_kvs + i].data[0].value;
			#endif 
			globalstatsbuffer[i] = kv;
		#else 
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
			vec.data[5].key = kvdram[offset_kvs + i].range(351, 320); 
			vec.data[5].value = kvdram[offset_kvs + i].range(383, 352); 
			vec.data[6].key = kvdram[offset_kvs + i].range(415, 384); 
			vec.data[6].value = kvdram[offset_kvs + i].range(447, 416); 
			vec.data[7].key = kvdram[offset_kvs + i].range(479, 448); 
			vec.data[7].value = kvdram[offset_kvs + i].range(511, 480); 
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
			vec.data[5].key = kvdram[offset_kvs + i].data[5].key; 
			vec.data[5].value = kvdram[offset_kvs + i].data[5].value; 
			vec.data[6].key = kvdram[offset_kvs + i].data[6].key; 
			vec.data[6].value = kvdram[offset_kvs + i].data[6].value; 
			vec.data[7].key = kvdram[offset_kvs + i].data[7].key; 
			vec.data[7].value = kvdram[offset_kvs + i].data[7].value; 
			#endif 
			globalstatsbuffer[i] = vec.data[globalparams.VARS_WORKBATCH];
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("readglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

void acts_all::MEMACCESSP0_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + NUM_PARTITIONS, globalparams.BASEOFFSETKVS_STATSDRAM + KVSTATSDRAMSZ_KVS + 1, offset_kvs, NUM_PARTITIONS, KVSTATSDRAMSZ_KVS);
	#endif
	
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<NUM_PARTITIONS; i++){
		#if NUM_EDGE_BANKS==0
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(31, 0) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(63, 32) = globalstatsbuffer[i].value; 
			#else
			kvdram[offset_kvs + i].data[0].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[0].value = globalstatsbuffer[i].value; 
			#endif
		#else
			#ifdef _WIDEWORD
 if(globalparams.VARS_WORKBATCH == 0){
				kvdram[offset_kvs + i].range(31, 0) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(63, 32) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 1){
				kvdram[offset_kvs + i].range(95, 64) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(127, 96) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 2){
				kvdram[offset_kvs + i].range(159, 128) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(191, 160) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 3){
				kvdram[offset_kvs + i].range(223, 192) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(255, 224) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 4){
				kvdram[offset_kvs + i].range(287, 256) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(319, 288) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 5){
				kvdram[offset_kvs + i].range(351, 320) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(383, 352) = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 6){
				kvdram[offset_kvs + i].range(415, 384) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(447, 416) = globalstatsbuffer[i].value; 
			}
else {
				kvdram[offset_kvs + i].range(479, 448) = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].range(511, 480) = globalstatsbuffer[i].value; 
			}
			#else 
 if(globalparams.VARS_WORKBATCH == 0){
				kvdram[offset_kvs + i].data[0].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[0].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 1){
				kvdram[offset_kvs + i].data[1].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[1].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 2){
				kvdram[offset_kvs + i].data[2].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[2].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 3){
				kvdram[offset_kvs + i].data[3].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[3].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 4){
				kvdram[offset_kvs + i].data[4].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[4].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 5){
				kvdram[offset_kvs + i].data[5].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[5].value = globalstatsbuffer[i].value; 
			}
else if(globalparams.VARS_WORKBATCH == 6){
				kvdram[offset_kvs + i].data[6].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[6].value = globalstatsbuffer[i].value; 
			}
else {
				kvdram[offset_kvs + i].data[7].key = globalstatsbuffer[i].key; 
				kvdram[offset_kvs + i].data[7].value = globalstatsbuffer[i].value; 
			}
			#endif
		#endif 

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printkeyvalues("MEMACCESSP0_saveglobalstats.globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS); 
	#endif
	return;
}

tuple_t acts_all::MEMACCESSP0_getvptrs( uint512_dt * kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
	#pragma HLS INLINE
	
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	#if NUM_EDGE_BANKS==0
		#ifdef _WIDEWORD
		beginvptr = kvdram[beginoffset].range(31, 0);
		endvptr = kvdram[endoffset].range(31, 0); 
		#else 
		beginvptr = kvdram[beginoffset].data[0].key;
		endvptr = kvdram[endoffset].data[0].key;
		#endif 
	#else
		if(edgebankID == 0){
			#ifdef _WIDEWORD
			beginvptr = edges0[beginoffset].range(31, 0);
			endvptr = edges0[endoffset].range(31, 0); 
			#else 
			beginvptr = edges0[beginoffset].data[0].key;
			endvptr = edges0[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>1
		else if(edgebankID == 1){
			#ifdef _WIDEWORD
			beginvptr = edges1[beginoffset].range(31, 0);
			endvptr = edges1[endoffset].range(31, 0); 
			#else 
			beginvptr = edges1[beginoffset].data[0].key;
			endvptr = edges1[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>2
		else if(edgebankID == 2){
			#ifdef _WIDEWORD
			beginvptr = edges2[beginoffset].range(31, 0);
			endvptr = edges2[endoffset].range(31, 0); 
			#else 
			beginvptr = edges2[beginoffset].data[0].key;
			endvptr = edges2[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>3
		else if(edgebankID == 3){
			#ifdef _WIDEWORD
			beginvptr = edges3[beginoffset].range(31, 0);
			endvptr = edges3[endoffset].range(31, 0); 
			#else 
			beginvptr = edges3[beginoffset].data[0].key;
			endvptr = edges3[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>4
		else if(edgebankID == 4){
			#ifdef _WIDEWORD
			beginvptr = edges4[beginoffset].range(31, 0);
			endvptr = edges4[endoffset].range(31, 0); 
			#else 
			beginvptr = edges4[beginoffset].data[0].key;
			endvptr = edges4[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>5
		else if(edgebankID == 5){
			#ifdef _WIDEWORD
			beginvptr = edges5[beginoffset].range(31, 0);
			endvptr = edges5[endoffset].range(31, 0); 
			#else 
			beginvptr = edges5[beginoffset].data[0].key;
			endvptr = edges5[endoffset].data[0].key;
			#endif 
		} 
		#if NUM_EDGE_BANKS>6
		else if(edgebankID == 6){
			#ifdef _WIDEWORD
			beginvptr = edges6[beginoffset].range(31, 0);
			endvptr = edges6[endoffset].range(31, 0); 
			#else 
			beginvptr = edges6[beginoffset].data[0].key;
			endvptr = edges6[endoffset].data[0].key;
			#endif 
		}
		#if NUM_EDGE_BANKS>7
		else if(edgebankID == 7){
			#ifdef _WIDEWORD
			beginvptr = edges7[beginoffset].range(31, 0);
			endvptr = edges7[endoffset].range(31, 0); 
			#else 
			beginvptr = edges7[beginoffset].data[0].key;
			endvptr = edges7[endoffset].data[0].key;
			#endif 
		}
	#endif
	#endif 
	#endif 
	#endif 
	#endif
	#endif 
	#endif
	else {
		#ifdef _WIDEWORD 
		beginvptr = NAp; 
		endvptr = NAp; 
		#else 
		beginvptr = NAp;
		endvptr = NAp;
		#endif 
	}
	#endif 

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

unsigned int acts_all::MEMACCESSP0_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
	keyvalue_t vptr_kv;
	
	uint512_dt V = kvdram[baseoffset_kvs + (offset / 16)];
	unsigned int M = (offset % 16) / 2;
	
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
	
	#if NUM_EDGE_BANKS==0
		beginvptr = MEMACCESSP0_getvptr(kvdram, baseoffset_kvs, beginoffset);
		endvptr = MEMACCESSP0_getvptr(kvdram, baseoffset_kvs, endoffset);
	#else
		if(edgebankID == 0){
			beginvptr = MEMACCESSP0_getvptr(edges0, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESSP0_getvptr(edges0, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>1
		else if(edgebankID == 1){
			beginvptr = MEMACCESSP0_getvptr(edges1, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESSP0_getvptr(edges1, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>2
		else if(edgebankID == 2){
			beginvptr = MEMACCESSP0_getvptr(edges2, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESSP0_getvptr(edges2, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>3
		else if(edgebankID == 3){
			beginvptr = MEMACCESSP0_getvptr(edges3, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESSP0_getvptr(edges3, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>4
		else if(edgebankID == 4){
			beginvptr = MEMACCESSP0_getvptr(edges4, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESSP0_getvptr(edges4, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>5
		else if(edgebankID == 5){
			beginvptr = MEMACCESSP0_getvptr(edges5, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESSP0_getvptr(edges5, baseoffset_kvs, endoffset);
		} 
		#if NUM_EDGE_BANKS>6
		else if(edgebankID == 6){
			beginvptr = MEMACCESSP0_getvptr(edges6, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESSP0_getvptr(edges6, baseoffset_kvs, endoffset);
		}
		#if NUM_EDGE_BANKS>7
		else if(edgebankID == 7){
			beginvptr = MEMACCESSP0_getvptr(edges7, baseoffset_kvs, beginoffset);
			endvptr = MEMACCESSP0_getvptr(edges7, baseoffset_kvs, endoffset);
		}
	#endif
	#endif 
	#endif 
	#endif 
	#endif
	#endif 
	#endif
	else {
		#ifdef _WIDEWORD 
		beginvptr = NAp; 
		endvptr = NAp; 
		#else 
		beginvptr = NAp;
		endvptr = NAp;
		#endif 
	}
	#endif 

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

void acts_all::MEMACCESSP0_copystats(uint512_dt * edges, uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;
	
	COPYSTATS_LOOP1: for(unsigned int k=0; k<globalparamsK.SIZE_KVSTATSDRAM; k++){
	#pragma HLS PIPELINE II=1
		kvdram[globalparamsK.BASEOFFSETKVS_STATSDRAM + k] = edges[globalparamsE.BASEOFFSETKVS_STATSDRAM + k];
	}
	return;
}

void acts_all::MEMACCESSP0_copyallstats( uint512_dt * kvdram, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID){
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;

	return;
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

void acts_all::MEMACCESSP0_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	SAVEKVSTATS_LOOP1: for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpartitionsb4last += (1 << (NUM_PARTITIONS_POW * k)); }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
		#ifdef _WIDEWORD
 if(globalparams.VARS_WORKBATCH == 0){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32); 
		}
else if(globalparams.VARS_WORKBATCH == 1){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(127, 96); 
		}
else if(globalparams.VARS_WORKBATCH == 2){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(191, 160); 
		}
else if(globalparams.VARS_WORKBATCH == 3){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(255, 224); 
		}
else if(globalparams.VARS_WORKBATCH == 4){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(319, 288); 
		}
else if(globalparams.VARS_WORKBATCH == 5){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(383, 352); 
		}
else if(globalparams.VARS_WORKBATCH == 6){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(447, 416); 
		}
else {
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(511, 480); 
		}
		#else 
 if(globalparams.VARS_WORKBATCH == 0){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value; 
		}
else if(globalparams.VARS_WORKBATCH == 1){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[1].value; 
		}
else if(globalparams.VARS_WORKBATCH == 2){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[2].value; 
		}
else if(globalparams.VARS_WORKBATCH == 3){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[3].value; 
		}
else if(globalparams.VARS_WORKBATCH == 4){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[4].value; 
		}
else if(globalparams.VARS_WORKBATCH == 5){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[5].value; 
		}
else if(globalparams.VARS_WORKBATCH == 6){
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[6].value; 
		}
else {
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[7].value; 
		}
		#endif
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
void acts_all::MEMACCESSP0_readmanypmask1(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask2(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask3(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask4(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask5(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask6(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask7(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask8(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask9(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask8_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask8_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask10(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask8_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask9_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask8_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask9_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask11(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask8_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask9_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask10_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask8_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask9_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask10_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
	}
	return;
}
void acts_all::MEMACCESSP0_readmanypmask12(uint512_dt * kvdram, uint32_type vmask0_p[BLOCKRAM_SIZE],uint32_type vmask1_p[BLOCKRAM_SIZE],uint32_type vmask2_p[BLOCKRAM_SIZE],uint32_type vmask3_p[BLOCKRAM_SIZE],uint32_type vmask4_p[BLOCKRAM_SIZE],uint32_type vmask5_p[BLOCKRAM_SIZE],uint32_type vmask6_p[BLOCKRAM_SIZE],uint32_type vmask7_p[BLOCKRAM_SIZE],uint32_type vmask8_p[BLOCKRAM_SIZE],uint32_type vmask9_p[BLOCKRAM_SIZE],uint32_type vmask10_p[BLOCKRAM_SIZE],uint32_type vmask11_p[BLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs){
	LOADACTIVEPARTITIONS_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#ifdef _WIDEWORD
		vmask0_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask1_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask2_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask3_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask4_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask5_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask6_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask7_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask8_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask9_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask10_p[i] = kvdram[offset_kvs + i].range(31, 0);
		vmask11_p[i] = kvdram[offset_kvs + i].range(31, 0);
		#else
		vmask0_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask1_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask2_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask3_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask4_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask5_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask6_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask7_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask8_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask9_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask10_p[i] = kvdram[offset_kvs + i].data[0].key;
		vmask11_p[i] = kvdram[offset_kvs + i].data[0].key;
		#endif 
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

