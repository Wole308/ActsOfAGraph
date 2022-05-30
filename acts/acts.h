void ACTSP0_ACTSactspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	if(enable1 == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }
	
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
	// cout<<"------------------- ACTSP0_actspipeline: maxcutoff: "<<maxcutoff<<endl;
	
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

void ACTSP0_TRADactspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){
	if(enable1 == OFF && enable2 == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }

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

void ACTSP0_actspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){
	ACTSP0_ACTSactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);		
	/** #ifdef CONFIG_ALL_EVALUATIONTYPES_IN_ONE_KERNEL
	if((globalparams.EVALUATION_ACTS_PARTITIONINGLOGIC == OFF)
			|| (globalparams.EVALUATION_ACTS_PARTITIONINGLOGIC == ON && globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 2 && currentLOP == 1)){
			ACTSP0_TRADactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, 
				buffer_setof8, capsule_so8,
					currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
	} else {
		ACTSP0_ACTSactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, 
				buffer_setof8, capsule_so8,
					currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
	}	
	#else 
		#ifdef CONFIG_ACTS_PARTITIONINGLOGIC
		ACTSP0_ACTSactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
		#else 
		ACTSP0_TRADactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
		#endif
	#endif */
}
										
fetchmessage_t ACTSP0_fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE],				 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams,
		unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	if(mode == ACTSPROCESSMODE){
		fetchmessage = PROCESSP0_readandprocess(enable, kvdram, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate, sweepparams, globalposition, globalparams, collections);
	} else {
		fetchmessage = MEMACCESSP0_readkeyvalues(enable, kvdram, buffer, goffset_kvs + loffset_kvs, size_kvs, travstate, globalparams); 
	}
	return fetchmessage; 
}

void ACTSP0_commitkeyvalues(bool_type enable, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE],
		keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(mode == ACTSREDUCEMODE || (globalparams.ACTSPARAMS_TREEDEPTH == 1 && globalparams.ENABLE_APPLYUPDATESCOMMAND == ON)){
		REDUCEP0_reduceandbuffer(enable, destbuffer, localcapsule, vbuffer, vbuffer2, sweepparams, globalposition, globalparams, collections);
		if(globalparams.ACTSPARAMS_TREEDEPTH == 1){ for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }} // NEWCHANGE.
	
	
		/* ///////////////// REMOVEME.
		MEMACCESSP0_saveV(ON, kvdram, vbuffer, globalparams.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparams.BUFFERBASEOFFSETKVS_VDATA1, globalparams.SIZEKVS2_REDUCEPARTITION, globalposition, globalparams); 
		// exit();
		//////////////// REMOVEME. */
	
	
	
	} else {
		MEMACCESSP0_savekeyvalues(enable, kvdram, destbuffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalposition, globalparams); 
	}
	return;
} 

void ACTSP0_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(enable == OFF){ return; }
	
keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE]; // REMOVEME.-SWX
	#pragma HLS array_partition variable = buffer_setof1
keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	
keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1

keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS];
	
	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;
	
	bool_type pp0readen = ON;
	bool_type pp1readen = ON;
	bool_type pp0runpipelineen = ON;
	bool_type pp1runpipelineen = ON;
	bool_type pp0partitionen = ON;
	bool_type pp1partitionen = ON;
	bool_type pp0writeen = ON;
	bool_type pp1writeen = ON;
buffer_type pp0cutoffs[VECTOR_SIZE];
buffer_type pp1cutoffs[VECTOR_SIZE];
	batch_type itercount = 0;
	batch_type flushsz = 0;
	
	fetchmessage_t fetchmessagepp0;
	fetchmessage_t fetchmessagepp1;
	fetchmessagepp0.chunksize_kvs = -1; fetchmessagepp0.nextoffset_kvs = -1;
	fetchmessagepp1.chunksize_kvs = -1; fetchmessagepp1.nextoffset_kvs = -1;
	
	if(resetenv == ON){ UTILP0_resetenvbuffers(capsule_so1, capsule_so8); } else { itercount = 64; } // FIXME. '64' is just some number greater than 2
	if(flush == ON){ flushsz = 1*MAX_SRCBUFFER_SIZE; } else { flushsz = 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(resetenv == ON){ cout<<"actit: reset is ON"<<endl; } else { cout<<"actit: reset is OFF"<<endl;  }
	if(flush == ON){ cout<<"actit: flush is ON"<<endl; } else { cout<<"actit: flush is OFF"<<endl;  }
	#endif 
	
	batch_type offset_kvs = ptravstate.begin_kvs;
	unsigned int workbuffer_size = globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ if(mode == ACTSPROCESSMODE){ workbuffer_size += 2; }} // inclusion of metadata else { }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit: "<<((ptravstate.end_kvs + flushsz) - ptravstate.begin_kvs) / workbuffer_size * NUMPIPELINES_PARTITIONUPDATES<<" states of acts pipeline processing..."<<endl; 
	#endif 
	// actsutilityobj->printkeyvalues("actit::globalstatsbuffer 37--", (keyvalue_t *)globalstatsbuffer, NUM_PARTITIONS); 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"actit: processing all chunks [begin_kvs: "<<ptravstate.begin_kvs<<"][end_kvs: "<<ptravstate.end_kvs<<"][flushsz: "<<flushsz<<"][size_kvs: "<<ptravstate.begin_kvs - (ptravstate.end_kvs + flushsz)<<"][workbuffer_size: "<<workbuffer_size<<"][num_chunks: "<<(ptravstate.begin_kvs - (ptravstate.end_kvs + flushsz)) / workbuffer_size<<"] ... "<<endl;
	#endif
		
	ACTIT_MAINLOOP: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs + flushsz; offset_kvs+=workbuffer_size * NUMPIPELINES_PARTITIONUPDATES){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts::ACTSP0_actit: processing chunk [offset_kvs: "<<offset_kvs<<"]: [ptravstate.begin_kvs: "<<ptravstate.begin_kvs<<"]: [ptravstate.end_kvs + flushsz: "<<ptravstate.end_kvs + flushsz<<"] ... "<<endl;
		#endif
		#ifdef PUP1
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 1){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 1){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		ptravstatepp0.i_kvs = offset_kvs;
		fetchmessagepp0 = ACTSP0_fetchkeyvalues(ON, mode,  kvdram, vbuffer_source, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp0.i_kvs, workbuffer_size, ptravstatepp0, sweepparams, globalposition, globalparams, edgebankID, collections);				
		#ifdef PUP1
		ACTSP0_actspipeline(pp1runpipelineen, ON, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, globalposition, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		PARTITIONP0_preparekeyvalues(ON, ON, mode, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, globalposition, fetchmessagepp0.chunksize_kvs, pp0cutoffs, globalparams);
		#ifdef PUP1
		ACTSP0_commitkeyvalues(pp1writeen, ON, mode, kvdram, vbuffer_dest, vbuffer2, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, fetchmessagepp1.chunksize_kvs, sweepparams, globalposition, globalparams, collections); 
		#endif 
		
		ACTSP0_actspipeline(ON, ON, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, globalposition, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		ptravstatepp1.i_kvs = offset_kvs + workbuffer_size;
		fetchmessagepp1 = ACTSP0_fetchkeyvalues(ON, mode,  kvdram, vbuffer_source, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp1.i_kvs, workbuffer_size, ptravstatepp1, sweepparams, globalposition, globalparams, edgebankID, collections);
		#endif
		
		ACTSP0_commitkeyvalues(pp0writeen, ON, mode, kvdram, vbuffer_dest, vbuffer2, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, fetchmessagepp0.chunksize_kvs, sweepparams, globalposition, globalparams, collections); 
		#ifdef PUP1
		PARTITIONP0_preparekeyvalues(pp1partitionen, ON, mode, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, globalposition, fetchmessagepp1.chunksize_kvs, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMPIPELINES_PARTITIONUPDATES;
		// exit(EXIT_SUCCESS);
	}
	// exit(EXIT_SUCCESS); //////////////////////////
	return;
}

