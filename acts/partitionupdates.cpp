// single stage partitioning 
void acts_all::PARTITIONP0_read_process_partition(bool_type enable, unsigned int mode, unsigned int offset_kvs, unsigned int sourcebaseaddr_kvs, 
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_dest[MAX_NUM_PARTITIONS], 
			travstate_t ptravstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	// globalparams.ACTSPARAMS_NUM_PARTITIONS replacedby 16
	analysis_type analysis_loopcount1 = 512;
	keyvalue_t edges_tup[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edges_tup complete
	keyvalue_t res[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	keyvalue_capsule_t capsule_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=capsule_kvs complete
	bool enx;
	
	ptravstate.i_kvs = offset_kvs;
	buffer_type chunk_size = UTILP0_getchunksize_kvs(globalparams.ACTSPARAMS_WORKBUFFER_SIZE, ptravstate, 0);
	unsigned int basecollections_0_2_data1 = collections[0][2].data1;
	unsigned int basecollections_0_2_data1b = collections[0][2].data1;
	
	unsigned int modelsz = chunk_size / VECTOR_SIZE; // CRITICAL FIXME.
	for(unsigned int p=0; p<16; p++){ capsule_kvs[p].key = 0; capsule_kvs[p].value = 0; }

	ACTIT2_PP0_READ_PROC_PART_ST_MAINLOOP1: for(buffer_type i=0; i<chunk_size; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		
		// decision flag
		enx = true;
		
		// get dataset
		UTILP0_GetDataset(kvdram, sourcebaseaddr_kvs + offset_kvs + i, edges_tup);
	
		
		// set flag
		enx = true;
		if((globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE) && (basecollections_0_2_data1 + VECTOR_SIZE >= globalposition.num_active_edges_in_channel)){ enx = false; } // for BFS implementation
		else { basecollections_0_2_data1b += VECTOR_SIZE; }
		
		// process
		res[0] = PROCESSP0_processvector(enx, 0, edges_tup[0].value, edges_tup[0], vbuffer_source[0], globalparams);
		res[1] = PROCESSP0_processvector(enx, 1, edges_tup[1].value, edges_tup[1], vbuffer_source[1], globalparams);
		res[2] = PROCESSP0_processvector(enx, 2, edges_tup[2].value, edges_tup[2], vbuffer_source[2], globalparams);
		res[3] = PROCESSP0_processvector(enx, 3, edges_tup[3].value, edges_tup[3], vbuffer_source[3], globalparams);
		res[4] = PROCESSP0_processvector(enx, 4, edges_tup[4].value, edges_tup[4], vbuffer_source[4], globalparams);
		res[5] = PROCESSP0_processvector(enx, 5, edges_tup[5].value, edges_tup[5], vbuffer_source[5], globalparams);
		res[6] = PROCESSP0_processvector(enx, 6, edges_tup[6].value, edges_tup[6], vbuffer_source[6], globalparams);
		res[7] = PROCESSP0_processvector(enx, 7, edges_tup[7].value, edges_tup[7], vbuffer_source[7], globalparams);
	
		
		// partition
		partition_type p = UTILP0_getpartition(ON, mode, UTILP0_GETKV(res[0]), sweepparams.currentLOP, sweepparams.upperlimit, sweepparams.upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
		
		// write to buffer 
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("read_process_partition 23", capsule_kvs[p].key + capsule_kvs[p].value, BLOCKRAM_SIZE + 1, p, capsule_kvs[p].key, capsule_kvs[p].value);
		#endif
		if(enx == true){
			destbuffer[0][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP0_GETKV(res[0]);
			destbuffer[1][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP0_GETKV(res[1]);
			destbuffer[2][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP0_GETKV(res[2]);
			destbuffer[3][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP0_GETKV(res[3]);
			destbuffer[4][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP0_GETKV(res[4]);
			destbuffer[5][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP0_GETKV(res[5]);
			destbuffer[6][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP0_GETKV(res[6]);
			destbuffer[7][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP0_GETKV(res[7]);
			capsule_kvs[p].value += 1;
		}
		
		#ifdef _DEBUGMODE_STATS
		if(mode==ACTSPROCESSMODE && enx == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE); }
		#endif 
	}
	for(unsigned int p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		capsule_dest[p].key = capsule_kvs[p].key * VECTOR_SIZE; capsule_dest[p].value = capsule_kvs[p].value * VECTOR_SIZE; 
	} // required in key-value mode in reduce
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("read_process_partition.capsule_kvs", (keyvalue_t *)capsule_kvs, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("read_process_partition.capsule_dest", (keyvalue_t *)capsule_dest, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	collections[0][2].data1 = basecollections_0_2_data1b;
	return;
}

// stage 1 partitioning
void acts_all::PARTITIONP0_ACTSstage1partitioning(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){				
	if(enable1 == OFF && enable2 == OFF){ return; }
	analysis_type analysis_dummyfiller = MAX_SRCBUFFER_SIZE - globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	
	value_t emptyslot[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=emptyslot complete
	UTILP0_resetvalues(emptyslot, VECTOR_SIZE, 0);
	
	buffer_type chunk_size = size_kvs;
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
		localcapsule[0][p].key = 0;
		localcapsule[0][p].value = 0; 
		localcapsule[1][p].key = 0;
		localcapsule[1][p].value = 0; 
		localcapsule[2][p].key = 0;
		localcapsule[2][p].value = 0; 
		localcapsule[3][p].key = 0;
		localcapsule[3][p].value = 0; 
		localcapsule[4][p].key = 0;
		localcapsule[4][p].value = 0; 
		localcapsule[5][p].key = 0;
		localcapsule[5][p].value = 0; 
		localcapsule[6][p].key = 0;
		localcapsule[6][p].value = 0; 
		localcapsule[7][p].key = 0;
		localcapsule[7][p].value = 0; 
	}

	PREPAREKEYVALUES_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_buffer_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_buffer_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_buffer_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_buffer_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_buffer_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_buffer_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_buffer_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_buffer_t keyvalue7 = sourcebuffer[7][i];
		
		keyvalue_t mykeyvalue0 = UTILP0_GETKV(keyvalue0);
		keyvalue_t mykeyvalue1 = UTILP0_GETKV(keyvalue1);
		keyvalue_t mykeyvalue2 = UTILP0_GETKV(keyvalue2);
		keyvalue_t mykeyvalue3 = UTILP0_GETKV(keyvalue3);
		keyvalue_t mykeyvalue4 = UTILP0_GETKV(keyvalue4);
		keyvalue_t mykeyvalue5 = UTILP0_GETKV(keyvalue5);
		keyvalue_t mykeyvalue6 = UTILP0_GETKV(keyvalue6);
		keyvalue_t mykeyvalue7 = UTILP0_GETKV(keyvalue7);
		
		bool_type valid0 = ON;
		if(mykeyvalue0.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue0.value != UTILP0_GETV(INVALIDDATA)){ valid0 = ON; } else { valid0 = OFF; }
		bool_type valid1 = ON;
		if(mykeyvalue1.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue1.value != UTILP0_GETV(INVALIDDATA)){ valid1 = ON; } else { valid1 = OFF; }
		bool_type valid2 = ON;
		if(mykeyvalue2.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue2.value != UTILP0_GETV(INVALIDDATA)){ valid2 = ON; } else { valid2 = OFF; }
		bool_type valid3 = ON;
		if(mykeyvalue3.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue3.value != UTILP0_GETV(INVALIDDATA)){ valid3 = ON; } else { valid3 = OFF; }
		bool_type valid4 = ON;
		if(mykeyvalue4.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue4.value != UTILP0_GETV(INVALIDDATA)){ valid4 = ON; } else { valid4 = OFF; }
		bool_type valid5 = ON;
		if(mykeyvalue5.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue5.value != UTILP0_GETV(INVALIDDATA)){ valid5 = ON; } else { valid5 = OFF; }
		bool_type valid6 = ON;
		if(mykeyvalue6.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue6.value != UTILP0_GETV(INVALIDDATA)){ valid6 = ON; } else { valid6 = OFF; }
		bool_type valid7 = ON;
		if(mykeyvalue7.key != UTILP0_GETK(INVALIDDATA) && mykeyvalue7.value != UTILP0_GETV(INVALIDDATA)){ valid7 = ON; } else { valid7 = OFF; }
		
		partition_type p0 = 0;
		if(valid0 == ON){ p0 = UTILP0_getpartition(ON, mode, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p1 = 0;
		if(valid1 == ON){ p1 = UTILP0_getpartition(ON, mode, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p2 = 0;
		if(valid2 == ON){ p2 = UTILP0_getpartition(ON, mode, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p3 = 0;
		if(valid3 == ON){ p3 = UTILP0_getpartition(ON, mode, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p4 = 0;
		if(valid4 == ON){ p4 = UTILP0_getpartition(ON, mode, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p5 = 0;
		if(valid5 == ON){ p5 = UTILP0_getpartition(ON, mode, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p6 = 0;
		if(valid6 == ON){ p6 = UTILP0_getpartition(ON, mode, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p7 = 0;
		if(valid7 == ON){ p7 = UTILP0_getpartition(ON, mode, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		
		if(valid0 == ON){
			if(localcapsule[0][p0].value == 0){ 
				localcapsule[0][p0].key = emptyslot[0]; emptyslot[0] += 4;
			} else if(localcapsule[0][p0].value % 4 == 0){ 
				localcapsule[0][p0].key = emptyslot[0]; emptyslot[0] += 4;
			} else {}
		}
		if(valid1 == ON){
			if(localcapsule[1][p1].value == 0){ 
				localcapsule[1][p1].key = emptyslot[1]; emptyslot[1] += 4;
			} else if(localcapsule[1][p1].value % 4 == 0){ 
				localcapsule[1][p1].key = emptyslot[1]; emptyslot[1] += 4;
			} else {}
		}
		if(valid2 == ON){
			if(localcapsule[2][p2].value == 0){ 
				localcapsule[2][p2].key = emptyslot[2]; emptyslot[2] += 4;
			} else if(localcapsule[2][p2].value % 4 == 0){ 
				localcapsule[2][p2].key = emptyslot[2]; emptyslot[2] += 4;
			} else {}
		}
		if(valid3 == ON){
			if(localcapsule[3][p3].value == 0){ 
				localcapsule[3][p3].key = emptyslot[3]; emptyslot[3] += 4;
			} else if(localcapsule[3][p3].value % 4 == 0){ 
				localcapsule[3][p3].key = emptyslot[3]; emptyslot[3] += 4;
			} else {}
		}
		if(valid4 == ON){
			if(localcapsule[4][p4].value == 0){ 
				localcapsule[4][p4].key = emptyslot[4]; emptyslot[4] += 4;
			} else if(localcapsule[4][p4].value % 4 == 0){ 
				localcapsule[4][p4].key = emptyslot[4]; emptyslot[4] += 4;
			} else {}
		}
		if(valid5 == ON){
			if(localcapsule[5][p5].value == 0){ 
				localcapsule[5][p5].key = emptyslot[5]; emptyslot[5] += 4;
			} else if(localcapsule[5][p5].value % 4 == 0){ 
				localcapsule[5][p5].key = emptyslot[5]; emptyslot[5] += 4;
			} else {}
		}
		if(valid6 == ON){
			if(localcapsule[6][p6].value == 0){ 
				localcapsule[6][p6].key = emptyslot[6]; emptyslot[6] += 4;
			} else if(localcapsule[6][p6].value % 4 == 0){ 
				localcapsule[6][p6].key = emptyslot[6]; emptyslot[6] += 4;
			} else {}
		}
		if(valid7 == ON){
			if(localcapsule[7][p7].value == 0){ 
				localcapsule[7][p7].key = emptyslot[7]; emptyslot[7] += 4;
			} else if(localcapsule[7][p7].value % 4 == 0){ 
				localcapsule[7][p7].key = emptyslot[7]; emptyslot[7] += 4;
			} else {}
		}
		
		buffer_type loc0 = localcapsule[0][p0].key + (localcapsule[0][p0].value % 4);
		buffer_type loc1 = localcapsule[1][p1].key + (localcapsule[1][p1].value % 4);
		buffer_type loc2 = localcapsule[2][p2].key + (localcapsule[2][p2].value % 4);
		buffer_type loc3 = localcapsule[3][p3].key + (localcapsule[3][p3].value % 4);
		buffer_type loc4 = localcapsule[4][p4].key + (localcapsule[4][p4].value % 4);
		buffer_type loc5 = localcapsule[5][p5].key + (localcapsule[5][p5].value % 4);
		buffer_type loc6 = localcapsule[6][p6].key + (localcapsule[6][p6].value % 4);
		buffer_type loc7 = localcapsule[7][p7].key + (localcapsule[7][p7].value % 4);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[0][p0].value", localcapsule[0][p0].value % 4, 4, localcapsule[0][p0].value, localcapsule[0][p0].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc0", loc0, MAX_SRCBUFFER_SIZE, localcapsule[0][p0].key, localcapsule[0][p0].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[1][p1].value", localcapsule[1][p1].value % 4, 4, localcapsule[1][p1].value, localcapsule[1][p1].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc1", loc1, MAX_SRCBUFFER_SIZE, localcapsule[1][p1].key, localcapsule[1][p1].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[2][p2].value", localcapsule[2][p2].value % 4, 4, localcapsule[2][p2].value, localcapsule[2][p2].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc2", loc2, MAX_SRCBUFFER_SIZE, localcapsule[2][p2].key, localcapsule[2][p2].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[3][p3].value", localcapsule[3][p3].value % 4, 4, localcapsule[3][p3].value, localcapsule[3][p3].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc3", loc3, MAX_SRCBUFFER_SIZE, localcapsule[3][p3].key, localcapsule[3][p3].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[4][p4].value", localcapsule[4][p4].value % 4, 4, localcapsule[4][p4].value, localcapsule[4][p4].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc4", loc4, MAX_SRCBUFFER_SIZE, localcapsule[4][p4].key, localcapsule[4][p4].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[5][p5].value", localcapsule[5][p5].value % 4, 4, localcapsule[5][p5].value, localcapsule[5][p5].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc5", loc5, MAX_SRCBUFFER_SIZE, localcapsule[5][p5].key, localcapsule[5][p5].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[6][p6].value", localcapsule[6][p6].value % 4, 4, localcapsule[6][p6].value, localcapsule[6][p6].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc6", loc6, MAX_SRCBUFFER_SIZE, localcapsule[6][p6].key, localcapsule[6][p6].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[7][p7].value", localcapsule[7][p7].value % 4, 4, localcapsule[7][p7].value, localcapsule[7][p7].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc7", loc7, MAX_SRCBUFFER_SIZE, localcapsule[7][p7].key, localcapsule[7][p7].value, NAp);
		#endif
		
		if(valid0 == ON){
			destbuffer[0][loc0] = keyvalue0;
			localcapsule[0][p0].value += 1;
		}
		if(valid1 == ON){
			destbuffer[1][loc1] = keyvalue1;
			localcapsule[1][p1].value += 1;
		}
		if(valid2 == ON){
			destbuffer[2][loc2] = keyvalue2;
			localcapsule[2][p2].value += 1;
		}
		if(valid3 == ON){
			destbuffer[3][loc3] = keyvalue3;
			localcapsule[3][p3].value += 1;
		}
		if(valid4 == ON){
			destbuffer[4][loc4] = keyvalue4;
			localcapsule[4][p4].value += 1;
		}
		if(valid5 == ON){
			destbuffer[5][loc5] = keyvalue5;
			localcapsule[5][p5].value += 1;
		}
		if(valid6 == ON){
			destbuffer[6][loc6] = keyvalue6;
			localcapsule[6][p6].value += 1;
		}
		if(valid7 == ON){
			destbuffer[7][loc7] = keyvalue7;
			localcapsule[7][p7].value += 1;
		}
	}
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = INVALIDDATA;
		keyvalue_buffer_t dummykv = UTILP0_GETKV(mydummykv);
		for(vector_type k=0; k<4; k++){
		#pragma HLS PIPELINE II=2
			if(localcapsule[0][p].value > 0 && (localcapsule[0][p].value % 4) > 0){
				if(k >= localcapsule[0][p].value % 4){
					destbuffer[0][localcapsule[0][p].key + k] = dummykv;
					localcapsule[0][p].value += 1;
				}
			}
			if(localcapsule[1][p].value > 0 && (localcapsule[1][p].value % 4) > 0){
				if(k >= localcapsule[1][p].value % 4){
					destbuffer[1][localcapsule[1][p].key + k] = dummykv;
					localcapsule[1][p].value += 1;
				}
			}
			if(localcapsule[2][p].value > 0 && (localcapsule[2][p].value % 4) > 0){
				if(k >= localcapsule[2][p].value % 4){
					destbuffer[2][localcapsule[2][p].key + k] = dummykv;
					localcapsule[2][p].value += 1;
				}
			}
			if(localcapsule[3][p].value > 0 && (localcapsule[3][p].value % 4) > 0){
				if(k >= localcapsule[3][p].value % 4){
					destbuffer[3][localcapsule[3][p].key + k] = dummykv;
					localcapsule[3][p].value += 1;
				}
			}
			if(localcapsule[4][p].value > 0 && (localcapsule[4][p].value % 4) > 0){
				if(k >= localcapsule[4][p].value % 4){
					destbuffer[4][localcapsule[4][p].key + k] = dummykv;
					localcapsule[4][p].value += 1;
				}
			}
			if(localcapsule[5][p].value > 0 && (localcapsule[5][p].value % 4) > 0){
				if(k >= localcapsule[5][p].value % 4){
					destbuffer[5][localcapsule[5][p].key + k] = dummykv;
					localcapsule[5][p].value += 1;
				}
			}
			if(localcapsule[6][p].value > 0 && (localcapsule[6][p].value % 4) > 0){
				if(k >= localcapsule[6][p].value % 4){
					destbuffer[6][localcapsule[6][p].key + k] = dummykv;
					localcapsule[6][p].value += 1;
				}
			}
			if(localcapsule[7][p].value > 0 && (localcapsule[7][p].value % 4) > 0){
				if(k >= localcapsule[7][p].value % 4){
					destbuffer[7][localcapsule[7][p].key + k] = dummykv;
					localcapsule[7][p].value += 1;
				}
			}
		}
	}
	
	for(vector_type v=0; v<VECTOR_SIZE; v++){ cutoffs[v] = emptyslot[v]; }
	
	localcapsule[0][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[0]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[1][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[1]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[2][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[2]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[3][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[3]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[4][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[4]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[5][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[5]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[6][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[6]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[7][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[7]); //?????????????????????? FIXME. MAKE OBSOLETE?
	
	localcapsule[0][0].key = 0;
	localcapsule[1][0].key = 0;
	localcapsule[2][0].key = 0;
	localcapsule[3][0].key = 0;
	localcapsule[4][0].key = 0;
	localcapsule[5][0].key = 0;
	localcapsule[6][0].key = 0;
	localcapsule[7][0].key = 0;
	
	UTILP0_calculatemanyunallignedoffsets(localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0, 0);

	#ifdef _DEBUGMODE_CHECKS2
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		for(vector_type v=0; v<VECTOR_SIZE; v++){
			actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues.localcapsule[v][p].key", localcapsule[v][p].key, 4);
		}
	}
	buffer_type maxcutoff = 0;
	for(vector_type v=0; v<VECTOR_SIZE; v++){ if(cutoffs[v] > maxcutoff){ maxcutoff = cutoffs[v]; }}
	if(maxcutoff % 4 != 0){ for(int v=0; v<VECTOR_SIZE; v++){ cout<<"------------------- preparekeyvalues:cutoffs["<<v<<"]: "<<cutoffs[v]<<endl; } exit(EXIT_FAILURE); }
	actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:maxcutoff", maxcutoff, 4);
actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[0]", emptyslot[0], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[1]", emptyslot[1], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[2]", emptyslot[2], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[3]", emptyslot[3], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[4]", emptyslot[4], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[5]", emptyslot[5], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[6]", emptyslot[6], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[7]", emptyslot[7], 4);	
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("[after]preparekeyvalues.localcapsule[0].2", (keyvalue_t *)localcapsule[0], globalparams.ACTSPARAMS_NUM_PARTITIONS);
	actsutilityobj->printvalues("preparekeyvalues:cutoffs", cutoffs, VECTOR_SIZE);
	#endif
	// cout<<"----------------------------------------------- preparekeyvalues: END ------------------------------------- "<<endl;
	return;
}

void acts_all::PARTITIONP0_TRADstage1partitioning(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){				
	if(enable1 == OFF && enable2 == OFF){ return; }
	analysis_type analysis_loop1 = globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	analysis_type analysis_dummyfiller = MAX_SRCBUFFER_SIZE - globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	
	unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ _NUM_PARTITIONS = UTILP0_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
	#endif 
	
	value_t emptyslot[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=emptyslot complete
	UTILP0_resetvalues(emptyslot, VECTOR_SIZE, 0);
	
	buffer_type chunk_size = size_kvs;
	for(vector_type v=0; v<VECTOR_SIZE; v++){ cutoffs[v] = size_kvs; }
	
	for(partition_type p=0; p<_NUM_PARTITIONS; p++){ 
		localcapsule[0][p].key = 0;
		localcapsule[0][p].value = 0; 
		localcapsule[1][p].key = 0;
		localcapsule[1][p].value = 0; 
		localcapsule[2][p].key = 0;
		localcapsule[2][p].value = 0; 
		localcapsule[3][p].key = 0;
		localcapsule[3][p].value = 0; 
		localcapsule[4][p].key = 0;
		localcapsule[4][p].value = 0; 
		localcapsule[5][p].key = 0;
		localcapsule[5][p].value = 0; 
		localcapsule[6][p].key = 0;
		localcapsule[6][p].value = 0; 
		localcapsule[7][p].key = 0;
		localcapsule[7][p].value = 0; 
	}
	
	PREPAREKEYVALUES_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1	
	#pragma HLS PIPELINE II=1
		destbuffer[0][i] = sourcebuffer[0][i];
		destbuffer[1][i] = sourcebuffer[1][i];
		destbuffer[2][i] = sourcebuffer[2][i];
		destbuffer[3][i] = sourcebuffer[3][i];
		destbuffer[4][i] = sourcebuffer[4][i];
		destbuffer[5][i] = sourcebuffer[5][i];
		destbuffer[6][i] = sourcebuffer[6][i];
		destbuffer[7][i] = sourcebuffer[7][i];
	}
	return;
}

void acts_all::PARTITIONP0_stage1partitioning(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){
	PARTITIONP0_ACTSstage1partitioning(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
	// PARTITIONP0_TRADstage1partitioning(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
	// PARTITIONP0_ACTS2stage1partitioning(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
	return;
}

// stage 2 partitioning
void acts_all::PARTITIONP0_ACTSstage2partitioning(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	if(enable1 == OFF){ return; }
	
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
	keyvalue_buffer_t kvA0[4];
	keyvalue_buffer_t kvA2[4];
	keyvalue_buffer_t kvA4[4];
	keyvalue_buffer_t kvA6[4];
	#pragma HLS ARRAY_PARTITION variable=kvA0 complete
	#pragma HLS ARRAY_PARTITION variable=kvA2 complete
	#pragma HLS ARRAY_PARTITION variable=kvA4 complete
	#pragma HLS ARRAY_PARTITION variable=kvA6 complete

	buffer_type tempcutoffs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=tempcutoffs complete

	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = capsule_so1[0][p].value + capsule_so1[1][p].value 
									+ capsule_so1[2][p].value + capsule_so1[3][p].value 
									+ capsule_so1[4][p].value + capsule_so1[5][p].value 
									+ capsule_so1[6][p].value + capsule_so1[7][p].value;
	}
	UTILP0_calculateoffsets(capsule_so8, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	UTILP0_resetvalues(capsule_so8, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
	
	buffer_type maxcutoff = 0;
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS PIPELINE II=1
		if(cutoffs[v] > maxcutoff){ maxcutoff = cutoffs[v]; }
	}
	#ifdef _DEBUGMODE_CHECKS2 // CRITICAL FIXME.
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	if(maxcutoff % 4 != 0){ for(int v=0; v<VECTOR_SIZE; v++){ cout<<"------------------- actspipeline:cutoffs["<<v<<"]: "<<cutoffs[v]<<endl; } exit(EXIT_FAILURE); }
	actsutilityobj->checkfordivisibleby(ON, "actspipeline:maxcutoff", maxcutoff, 4);
	#endif
	#endif
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	if(maxcutoff % 4 != 0){ maxcutoff = 0; } // 4, CRITICAL FIXME.
	#endif 
	// cout<<"------------------- acts_all::ACTSP0_actspipeline: maxcutoff: "<<maxcutoff<<endl;
	
	keyvalue_t mydummykv;
	mydummykv.key = 0;
	mydummykv.value = UTILP0_GETV(INVALIDDATA);
	keyvalue_buffer_t dummykv = UTILP0_GETKV(mydummykv);
	
	value_t tempbufferDcapsule[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=tempbufferDcapsule complete

	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		tempbufferDcapsule[p] = capsule_so8[p].value; 
	}
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS PIPELINE II=1
		tempcutoffs[v] = cutoffs[v]; 
	}
	
	unsigned int n=0;
	RUNPIPELINE_LOOP1: for(n=0; n<2; n++){
		RUNPIPELINE_LOOP1B: for(buffer_type k=0; k<maxcutoff; k+=4){
		#pragma HLS PIPELINE II=4
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(k % 16 == 0){ cout<<"k: "<<k<<endl; }
			#endif
			
			/// LOADING FROM A
			if(k < tempcutoffs[0+n]){ kvA0[0] = buffer_setof1[0+n][k]; kvA0[1] = buffer_setof1[0+n][k+1]; kvA0[2] = buffer_setof1[0+n][k+2]; kvA0[3] = buffer_setof1[0+n][k+3]; } 
			else { kvA0[0] = dummykv; kvA0[1] = dummykv; kvA0[2] = dummykv; kvA0[3] = dummykv; }
			if(k < tempcutoffs[2+n]){ kvA2[0] = buffer_setof1[2+n][k]; kvA2[1] = buffer_setof1[2+n][k+1]; kvA2[2] = buffer_setof1[2+n][k+2]; kvA2[3] = buffer_setof1[2+n][k+3]; } 
			else { kvA2[0] = dummykv; kvA2[1] = dummykv; kvA2[2] = dummykv; kvA2[3] = dummykv; }
			if(k < tempcutoffs[4+n]){ kvA4[0] = buffer_setof1[4+n][k]; kvA4[1] = buffer_setof1[4+n][k+1]; kvA4[2] = buffer_setof1[4+n][k+2]; kvA4[3] = buffer_setof1[4+n][k+3]; } 
			else { kvA4[0] = dummykv; kvA4[1] = dummykv; kvA4[2] = dummykv; kvA4[3] = dummykv; }
			if(k < tempcutoffs[6+n]){ kvA6[0] = buffer_setof1[6+n][k]; kvA6[1] = buffer_setof1[6+n][k+1]; kvA6[2] = buffer_setof1[6+n][k+2]; kvA6[3] = buffer_setof1[6+n][k+3]; } 
			else { kvA6[0] = dummykv; kvA6[1] = dummykv; kvA6[2] = dummykv; kvA6[3] = dummykv; }
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			for(unsigned int m=0; m<4; m++){ cout<<"kvA0["<<m<<"].key: "<<UTILP0_GETKV(kvA0[m]).key<<", kvA0["<<m<<"].value: "<<UTILP0_GETKV(kvA0[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA2["<<m<<"].key: "<<UTILP0_GETKV(kvA2[m]).key<<", kvA2["<<m<<"].value: "<<UTILP0_GETKV(kvA2[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA4["<<m<<"].key: "<<UTILP0_GETKV(kvA4[m]).key<<", kvA4["<<m<<"].value: "<<UTILP0_GETKV(kvA4[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA6["<<m<<"].key: "<<UTILP0_GETKV(kvA6[m]).key<<", kvA6["<<m<<"].value: "<<UTILP0_GETKV(kvA6[m]).value<<endl; }
			#endif
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkn(enablebufferA, "kvA0", kvA0, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA2", kvA2, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA4", kvA4, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA6", kvA6, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			#endif
			
			partition_type pA0 = UTILP0_getpartition(ON, mode, kvA0[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA2 = UTILP0_getpartition(ON, mode, kvA2[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA4 = UTILP0_getpartition(ON, mode, kvA4[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA6 = UTILP0_getpartition(ON, mode, kvA6[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			
			/// LOADING FROM AND INTO D
			buffer_type posD0 = capsule_so8[pA0].key + tempbufferDcapsule[pA0];
			unsigned int yoffset0 = posD0 / 8;
			unsigned int xoffset0 = posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "posD0", posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "posD0", posD0, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset0", yoffset0, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset0+4", xoffset0+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset0 == 0){
				buffer_setof8[0][yoffset0] = kvA0[0]; buffer_setof8[1][yoffset0] = kvA0[1]; buffer_setof8[2][yoffset0] = kvA0[2]; buffer_setof8[3][yoffset0] = kvA0[3]; 
			} else {
				buffer_setof8[4][yoffset0] = kvA0[0]; buffer_setof8[5][yoffset0] = kvA0[1]; buffer_setof8[6][yoffset0] = kvA0[2]; buffer_setof8[7][yoffset0] = kvA0[3]; 
			}
			if(UTILP0_GETKV(kvA0[0]).value != UTILP0_GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; }
			// if(UTILP0_GETKV(kvA0[0]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA0[1]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA0[2]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA0[3]).value != UTILP0_GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; }		
			
			buffer_type _posD0 = capsule_so8[pA2].key + tempbufferDcapsule[pA2];
			unsigned int yoffset1 = _posD0 / 8;
			unsigned int xoffset1 = _posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "_posD0", _posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "_posD0", _posD0, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset1", yoffset1, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset1+4", xoffset1+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset1 == 0){
				buffer_setof8[0][yoffset1] = kvA2[0]; buffer_setof8[1][yoffset1] = kvA2[1]; buffer_setof8[2][yoffset1] = kvA2[2]; buffer_setof8[3][yoffset1] = kvA2[3]; 
			} else {
				buffer_setof8[4][yoffset1] = kvA2[0]; buffer_setof8[5][yoffset1] = kvA2[1]; buffer_setof8[6][yoffset1] = kvA2[2]; buffer_setof8[7][yoffset1] = kvA2[3]; 
			}
			if(UTILP0_GETKV(kvA2[0]).value != UTILP0_GETV(INVALIDDATA)){ tempbufferDcapsule[pA2] += 4; }
			// if(UTILP0_GETKV(kvA2[0]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA2[1]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA2[2]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA2[3]).value != UTILP0_GETV(INVALIDDATA)){ tempbufferDcapsule[pA2] += 4; }
			
			buffer_type __posD0 = capsule_so8[pA4].key + tempbufferDcapsule[pA4];
			unsigned int yoffset2 = __posD0 / 8;
			unsigned int xoffset2 = __posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "__posD0", __posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "__posD0", __posD0, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset2", yoffset2, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset2+4", xoffset2+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset2 == 0){
				buffer_setof8[0][yoffset2] = kvA4[0]; buffer_setof8[1][yoffset2] = kvA4[1]; buffer_setof8[2][yoffset2] = kvA4[2]; buffer_setof8[3][yoffset2] = kvA4[3]; 
			} else {
				buffer_setof8[4][yoffset2] = kvA4[0]; buffer_setof8[5][yoffset2] = kvA4[1]; buffer_setof8[6][yoffset2] = kvA4[2]; buffer_setof8[7][yoffset2] = kvA4[3]; 
			}
			if(UTILP0_GETKV(kvA4[0]).value != UTILP0_GETV(INVALIDDATA)){ tempbufferDcapsule[pA4] += 4; }
			// if(UTILP0_GETKV(kvA4[1]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA4[1]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA4[2]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA4[3]).value != UTILP0_GETV(INVALIDDATA)){ tempbufferDcapsule[pA4] += 4; }
			
			buffer_type ___posD0 = capsule_so8[pA6].key + tempbufferDcapsule[pA6];
			unsigned int yoffset3 = ___posD0 / 8;
			unsigned int xoffset3 = ___posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "___posD0", ___posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "___posD0", ___posD0, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset3", yoffset3, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset3+4", xoffset3+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset3 == 0){
				buffer_setof8[0][yoffset3] = kvA6[0]; buffer_setof8[1][yoffset3] = kvA6[1]; buffer_setof8[2][yoffset3] = kvA6[2]; buffer_setof8[3][yoffset3] = kvA6[3]; 
			} else {
				buffer_setof8[4][yoffset3] = kvA6[0]; buffer_setof8[5][yoffset3] = kvA6[1]; buffer_setof8[6][yoffset3] = kvA6[2]; buffer_setof8[7][yoffset3] = kvA6[3]; 
			}
			if(UTILP0_GETKV(kvA6[0]).value != UTILP0_GETV(INVALIDDATA)){ tempbufferDcapsule[pA6] += 4; }
			// if(UTILP0_GETKV(kvA6[0]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA6[1]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA6[2]).value != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kvA6[3]).value != UTILP0_GETV(INVALIDDATA)){ tempbufferDcapsule[pA6] += 4; }
		}
	}
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		capsule_so8[p].value = tempbufferDcapsule[p]; 
	}
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = UTILP0_GETV(INVALIDDATA);
		keyvalue_buffer_t dummykv = UTILP0_GETKV(mydummykv);
		if(capsule_so8[p].value % 8 == 4){
			unsigned int yoffset = (capsule_so8[p].key + capsule_so8[p].value) / VECTOR_SIZE;
			buffer_setof8[4][yoffset] = dummykv; buffer_setof8[5][yoffset] = dummykv; buffer_setof8[6][yoffset] = dummykv; buffer_setof8[7][yoffset] = dummykv; 
			capsule_so8[p].value += 4;
		}
		#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
		actsutilityobj->checkfordivisibleby(enablebufferD, "capsule_so8[p].value", capsule_so8[p].value, 8);
		#endif
	}
	return;
}

void acts_all::PARTITIONP0_TRADstage2partitioning(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){
	if(enable1 == OFF && enable2 == OFF){ return; }
	
	buffer_type chunk_size = cutoffs[0];
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	unsigned int num_partitions = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	
	if(globalparams.EVALUATION_ACTS_VERYBARE == ON){ return; }
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ num_partitions = UTILP0_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
	#endif 
	
	for(partition_type p=0; p<num_partitions; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0; 
	}
	
	if(chunk_size >= BLOCKRAM_SIZE){ chunk_size = globalparams.ACTSPARAMS_WORKBUFFER_SIZE; } // BLOCKRAM_SIZE, globalparams.ACTSPARAMS_WORKBUFFER_SIZE
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("TRADactspipeline (22)", chunk_size, BLOCKRAM_SIZE, cutoffs[0], NAp, NAp);
	#endif
	
	TRADITIONALPARTITIONKEYVALUES_LOOP: for(buffer_type i=0; i<chunk_size; i++){
		TRADITIONALPARTITIONKEYVALUES_LOOPB: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=2
			keyvalue_buffer_t kv = buffer_setof1[v][i];
			
			partition_type p = UTILP0_getpartition2(ON, mode, kv, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams);
			if(UTILP0_GETKV(kv).key != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kv).value != UTILP0_GETV(INVALIDDATA)){ capsule_so8[p].value += 1; }
		}
	}
	
	UTILP0_calculateoffsets(capsule_so8, num_partitions);
	UTILP0_resetvalues(capsule_so8, num_partitions, 0);
	
	TRADITIONALPARTITIONKEYVALUES_LOOP2: for(buffer_type i=0; i<chunk_size; i++){
		TRADITIONALPARTITIONKEYVALUES_LOOP2B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=2
			keyvalue_buffer_t kv = buffer_setof1[v][i];
			// cout<<"--- priorpartitionkeyvalues: kv.key: "<<kv.key<<endl; // REMOVEME.
			partition_type p = UTILP0_getpartition2(ON, mode, kv, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams);
			buffer_type pos = capsule_so8[p].key + capsule_so8[p].value;
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("TRADactspipeline (23)", pos / VECTOR_SIZE, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(UTILP0_GETKV(kv).key != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kv).value != UTILP0_GETV(INVALIDDATA)){ buffer_setof8[pos % VECTOR_SIZE][pos / VECTOR_SIZE] = kv; } // NOTE: could this be the cause of slight imperfection in results?
			if(UTILP0_GETKV(kv).key != UTILP0_GETV(INVALIDDATA) && UTILP0_GETKV(kv).value != UTILP0_GETV(INVALIDDATA)){ capsule_so8[p].value += 1; }
		}
	}
	
	TRADITIONALPARTITIONKEYVALUES_LOOP3: for(partition_type p=0; p<num_partitions; p++){
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = UTILP0_GETV(INVALIDDATA);
		keyvalue_buffer_t dummykv = UTILP0_GETKV(mydummykv);
	
		unsigned int endoffset = capsule_so8[p].key + capsule_so8[p].value;
		unsigned int xpos = endoffset % VECTOR_SIZE;
		unsigned int ypos = endoffset / VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("TRADactspipeline (24)", ypos, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(capsule_so8[p].value > 0){
			TRADITIONALPARTITIONKEYVALUES_LOOP3B: for(vector_type v=xpos; v<VECTOR_SIZE; v++){
			#pragma HLS PIPELINE II=2
				buffer_setof8[v][ypos] = dummykv;
				capsule_so8[p].value += 1;
			}
		}
	}
	
	// cout<<"actit:: MAX_NUM_PARTITIONS: "<<MAX_NUM_PARTITIONS<<", num_partitions: "<<num_partitions<<", globalparams.ACTSPARAMS_TREEDEPTH: "<<globalparams.ACTSPARAMS_TREEDEPTH<<endl;
	// actsutilityobj->printkeyvalues("------------- actit::capsule_so8 37--", (keyvalue_t *)capsule_so8, num_partitions); 
	// exit(EXIT_SUCCESS);
	return;
}

void acts_all::PARTITIONP0_stage2partitioning(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){
	PARTITIONP0_ACTSstage2partitioning(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);		
	// PARTITIONP0_TRADstage2partitioning(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
}


