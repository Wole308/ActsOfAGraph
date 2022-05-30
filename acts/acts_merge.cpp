#define CONFIG_MERGE_VDATAS
#ifndef ALLVERTEXISACTIVE_ALGORITHM
// #define CONFIG_MERGE_COLLECTACTIVEVPARTITIONS
#endif
// #define MERGECONFIG_ACTIVATE_HYBRIDMODE

unsigned int MERGEP0_actvpstatsoffset(globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ _offset += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }} else { _offset = 1; }
	return _offset;
}

void MERGEP0_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	// return;
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int offset_kvs = globalparams.ACTSPARAMS_INSTID * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to kvdram."<<endl; 	
	#endif
	MERGEP0_BROADCASTVTXS_LOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){	
		MERGEP0_BROADCASTVTXS_LOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
		#pragma HLS PIPELINE II=1
			kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k];
		}
	}
	return;
}

void MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices (997)..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE];
	unsigned int _offset = MERGEP0_actvpstatsoffset(globalparams);
	MEMACCESSP0_retreievekvstats(kvdram, globalstatsbuffer, globalparams, _offset, globalparams.NUM_REDUCEPARTITIONS);
	
	unsigned int i = globalparams.ACTSPARAMS_INSTID % NUMCOMPUTEUNITS_SLR2;
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"MERGEP0_mergeVs:: globalparams.ACTSPARAMS_INSTID: "<<globalparams.ACTSPARAMS_INSTID<<", i: "<<i<<endl;
	#endif 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ globalstatsbuffer[reduce_partition].value = 0; }
	}
	#endif 	
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer[i].value > 0){ cout<<"MERGE: globalstatsbuffer["<<i<<"].key : "<<globalstatsbuffer[i].key<<", globalstatsbuffer["<<i<<"].value : "<<globalstatsbuffer[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 	
	#endif 
	unsigned int voffset_kvs2 = i * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3 == true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVMASK(K)==1){ cout<<"$$$ MERGE:: MASK=1 SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVMASK(V)==1){ cout<<"$$$ MERGE:: MASK=1 SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = i * globalparamsv.NUM_REDUCEPARTITIONS;
	MERGEP0_MERGEVSLOOP3: for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILP0_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer[reduce_partition].value);
	}
	
	// stats to be returned to host
	#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
	unsigned int vtmp = UTILP0_GetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int vtmp2 = UTILP0_GetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int vtmp3 = UTILP0_GetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int vtmp4 = UTILP0_GetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	
	unsigned int tmp = UTILP0_GetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int tmp2 = UTILP0_GetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int tmp3 = UTILP0_GetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int tmp4 = UTILP0_GetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, vtmp + tmp);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, vtmp2 + tmp2);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, vtmp3 + tmp3);
	UTILP0_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, vtmp4 + tmp4);
		
	#ifdef _DEBUGMODE_STATS	
	unsigned int dat = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	unsigned int dat2 = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	unsigned int dat3 = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	unsigned int dat4 = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	// cout<<">>> MERGE::[COMMENT-ME-OUT]:: number of active vertices for next iteration (iteration "<<globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1<<"): dat: "<<dat<<", dat2: "<<dat2<<", dat3: "<<dat3<<", dat4: "<<dat4<<endl;
	#endif 
	#endif 
	// exit(EXIT_SUCCESS); // 
	return;
}

void MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: ACTS EXCHANGE LAUNCHED. Exchanging vertices across different SLRs..."<<endl; 
	#endif
	
	bool exchangeall = true; // true;//false; // CRITICAL FIXME.
	
	uint32_vec_dt globalstatsbuffer0[BLOCKRAM_GLOBALSTATS_BIGSIZE];
	#pragma HLS DATA_PACK variable = globalstatsbuffer0
	uint32_vec_dt globalstatsbuffer1[BLOCKRAM_GLOBALSTATS_BIGSIZE];
	#pragma HLS DATA_PACK variable = globalstatsbuffer1
	uint32_vec_dt globalstatsbuffer2[BLOCKRAM_GLOBALSTATS_BIGSIZE];
	#pragma HLS DATA_PACK variable = globalstatsbuffer2
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	keyvalue_t actvvbuffer[VECTOR2_SIZE][MAX_ACTVVBUFFER_SIZE];
	#pragma HLS array_partition variable = actvvbuffer
	unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE];
	unsigned int loadcount[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=loadcount complete
	unsigned int numactvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=numactvvs complete
	unsigned int local_actvv[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv complete
	unsigned int local_actvv2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv2 complete
	unsigned int global_actvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=global_actvvs complete
	uint32_type vpmaskVecSum[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vpmaskVecSum complete	
	#endif 	
	value_t vdatas[VECTOR2_SIZE];
	
	unsigned int reduce_partition = 0;
	unsigned int partition_offset = 0;
	
	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	globalparams_t globalparamsm = UTILP0_getglobalparams(mdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsvA);
	
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	unsigned int nextGraphIter = globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID;
	
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR2 = NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR1 = NUMCOMPUTEUNITS_SLR1 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR0 = NUMCOMPUTEUNITS_SLR0 * globalparamsvA.NUM_REDUCEPARTITIONS;
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparamsvA.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	#else 
	unsigned int _NUM_VALIDREDUCEPARTITIONS = globalparamsvA.NUM_REDUCEPARTITIONS;//
	#endif 
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	
	uint32_type pmaski = 0;
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ loadcount[v] = 0; numactvvs[v] = 0; }
	for(unsigned int v=0; v<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v++){ vpmaskbuffer[v] = 0; }
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ vpmaskVecSum[v] = 0; }
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif
	// active reduce partitions
	MERGEP0_EXCHANGEC_LOOP1A: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR2; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_exchange: ERROR 1s20", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif 
		unsigned int tempdata = UTILP0_GetSecondData(vdramA, globalparamsvA.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer0[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] = 1; }
	}
	
	MERGEP0_EXCHANGEC_LOOP1B: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR1; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_exchange: ERROR 1s21", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif
		unsigned int tempdata = UTILP0_GetSecondData(vdramB, globalparamsvB.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer1[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH]; }
	}
	
	MERGEP0_EXCHANGEC_LOOP1C: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR0; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP0_exchange: ERROR 1s22", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif
		unsigned int tempdata = UTILP0_GetSecondData(vdramC, globalparamsvC.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer2[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH]; }
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<_TOTALNUMREDUCEPARTITIONS_SLR0; i++){ if(globalstatsbuffer0[i / 32].data[i % 32] > 0 || globalstatsbuffer1[i / 32].data[i % 32] > 0 || globalstatsbuffer2[i / 32].data[i % 32] > 0){ cout<<"EXCHANGE: globalstatsbuffer0["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer0[i][i / 32].data[i % 32]<<", globalstatsbuffer1["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer1[i / 32].data[i % 32]<<", globalstatsbuffer2["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer2[i / 32].data[i % 32]<<endl; }}					
	#endif 
	
	// vertices
	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 20", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, globalparamsvA.NUM_REDUCEPARTITIONS, NAp, NAp);
			#endif
			if(globalstatsbuffer0[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer0["<<reduce_partition<<"]: "<<globalstatsbuffer0[reduce_partition]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 21", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 22", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					// collect active vertices
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					#else 
					UTILP0_GetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					UTILP0_SetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					UTILP0_SetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					#endif 
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						unsigned int K = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k].data[v].key;
						unsigned int V = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k].data[v].value;
						if(MEMCAP0_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(1):: MASK=1 SEEN. index: "<<globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k<<endl; }
						if(MEMCAP0_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(1):: MASK=1 SEEN. index: "<<globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k<<endl; }	
					}
					#endif
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = i + voffsetA_kvs + k;
					MERGEP0_collects(i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum, partition_offset);
					#endif 
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramA to vdramB & vdramC. [end offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif

	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramB to vdramA & vdramC. [begin offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 21", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(globalstatsbuffer1[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer[0]["<<reduce_partition<<"]: "<<globalstatsbuffer[0][reduce_partition]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 24", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 25", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					// collect active vertices 
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					#else 
					UTILP0_GetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);
					UTILP0_SetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);
					UTILP0_SetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);	
					#endif
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						unsigned int K = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k].data[v].key;
						unsigned int V = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k].data[v].value;
						if(MEMCAP0_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k<<endl; }
						if(MEMCAP0_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k<<endl; }	
					}
					#endif
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = NUMCOMPUTEUNITS_SLR2 + i + voffsetB_kvs + k;
					MERGEP0_collects(NUMCOMPUTEUNITS_SLR2 + i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum, partition_offset);
					#endif 
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramB to vdramA & vdramC. [end offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	
	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramC to vdramA & vdramB. [begin offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 22", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(globalstatsbuffer2[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer0["<<reduce_partition<<"]: "<<globalstatsbuffer0[reduce_partition]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 27", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 28", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					#else 
					UTILP0_GetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);
					UTILP0_SetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);
					UTILP0_SetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);		
					#endif 
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						unsigned int K = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k].data[v].key;
						unsigned int V = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k].data[v].value;
						if(MEMCAP0_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(3):: MASK=1 SEEN. index: "<<globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k<<endl; }
						if(MEMCAP0_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(3):: MASK=1 SEEN. index: "<<globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k<<endl; }	
					}
					#endif
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + i + voffsetC_kvs + k;
					MERGEP0_collects(NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum, partition_offset);	
					#endif 
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;			
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif

	// vertices partition masks
	#if defined(CONFIG_MERGE_COLLECTACTIVEVPARTITIONS)
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"#################################################### MERGE::EXCHANGE:: merging vertices partition masks across vdramA, vdramB & vdramC."<<endl; 
	#endif
	MERGEP0_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; partition++){ // BLOCKRAM_NEXTPMASK_SIZE, globalparamsvA.NUM_PROCESSEDGESPARTITIONS
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 31", globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 32", globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 33", globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		// cout<<"+++ TOPP0_topkernelS: vpmaskbuffer["<<partition<<"]: "<<vpmaskbuffer[partition]<<endl;
		if(vpmaskbuffer[partition] > 0){ cout<<"--- TOPP0_topkernelS: vpmaskbuffer["<<partition<<"]: "<<vpmaskbuffer[partition]<<endl; }
		#endif
		
		UTILP0_SetFirstData(vdramA, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		UTILP0_SetFirstData(vdramB, globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		UTILP0_SetFirstData(vdramC, globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"--- TOPP0_topkernelS: vdramA["<<partition<<"]: "<<p<<endl; 
		#endif
	}
	#endif 
	
	// collect active vertices 
	#if not defined (ALLVERTEXISACTIVE_ALGORITHM) && defined(MERGECONFIG_ACTIVATE_HYBRIDMODE)
	unsigned int maxloadcount_vs = 0;
	unsigned int maxnumactvvs_vs = 0;
	unsigned int sumloadcount = 0;
	unsigned int num_vertices_per_bank = globalparamsvC.SIZEKVS2_REDUCEPARTITION * globalparamsvC.NUM_REDUCEPARTITIONS * VECTOR2_SIZE;
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(maxloadcount_vs < loadcount[v]){ maxloadcount_vs = loadcount[v]; }}
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(maxnumactvvs_vs < numactvvs[v]){ maxnumactvvs_vs = numactvvs[v]; }}
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ sumloadcount += loadcount[v]; }
	
	unsigned int offsetkvs_actvvs;
	if(nextGraphIter % 2 == 0){ offsetkvs_actvvs = 0; } 
	else { offsetkvs_actvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; }
	
	for(unsigned int i=0; i<maxloadcount_vs; i++){
	#pragma HLS PIPELINE II=1
	
		keyvalue_t actvv0 = actvvbuffer[0][i];
		global_actvvs[0] = UTILP0_GETREALVID(actvv0.value, actvv0.key);
	
		keyvalue_t actvv1 = actvvbuffer[1][i];
		global_actvvs[1] = UTILP0_GETREALVID(actvv1.value, actvv1.key);
	
		keyvalue_t actvv2 = actvvbuffer[2][i];
		global_actvvs[2] = UTILP0_GETREALVID(actvv2.value, actvv2.key);
	
		keyvalue_t actvv3 = actvvbuffer[3][i];
		global_actvvs[3] = UTILP0_GETREALVID(actvv3.value, actvv3.key);
	
		keyvalue_t actvv4 = actvvbuffer[4][i];
		global_actvvs[4] = UTILP0_GETREALVID(actvv4.value, actvv4.key);
	
		keyvalue_t actvv5 = actvvbuffer[5][i];
		global_actvvs[5] = UTILP0_GETREALVID(actvv5.value, actvv5.key);
	
		keyvalue_t actvv6 = actvvbuffer[6][i];
		global_actvvs[6] = UTILP0_GETREALVID(actvv6.value, actvv6.key);
	
		keyvalue_t actvv7 = actvvbuffer[7][i];
		global_actvvs[7] = UTILP0_GETREALVID(actvv7.value, actvv7.key);
	
		keyvalue_t actvv8 = actvvbuffer[8][i];
		global_actvvs[8] = UTILP0_GETREALVID(actvv8.value, actvv8.key);
	
		keyvalue_t actvv9 = actvvbuffer[9][i];
		global_actvvs[9] = UTILP0_GETREALVID(actvv9.value, actvv9.key);
	
		keyvalue_t actvv10 = actvvbuffer[10][i];
		global_actvvs[10] = UTILP0_GETREALVID(actvv10.value, actvv10.key);
	
		keyvalue_t actvv11 = actvvbuffer[11][i];
		global_actvvs[11] = UTILP0_GETREALVID(actvv11.value, actvv11.key);
	
		keyvalue_t actvv12 = actvvbuffer[12][i];
		global_actvvs[12] = UTILP0_GETREALVID(actvv12.value, actvv12.key);
	
		keyvalue_t actvv13 = actvvbuffer[13][i];
		global_actvvs[13] = UTILP0_GETREALVID(actvv13.value, actvv13.key);
	
		keyvalue_t actvv14 = actvvbuffer[14][i];
		global_actvvs[14] = UTILP0_GETREALVID(actvv14.value, actvv14.key);
	
		keyvalue_t actvv15 = actvvbuffer[15][i];
		global_actvvs[15] = UTILP0_GETREALVID(actvv15.value, actvv15.key);
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		if(i < loadcount[0] && actvvbuffer[0][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[0]<<", localvid: "<<actvv0.value<<", instance: "<<actvv0.key<<endl; }
		if(i < loadcount[1] && actvvbuffer[1][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[1]<<", localvid: "<<actvv1.value<<", instance: "<<actvv1.key<<endl; }
		if(i < loadcount[2] && actvvbuffer[2][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[2]<<", localvid: "<<actvv2.value<<", instance: "<<actvv2.key<<endl; }
		if(i < loadcount[3] && actvvbuffer[3][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[3]<<", localvid: "<<actvv3.value<<", instance: "<<actvv3.key<<endl; }
		if(i < loadcount[4] && actvvbuffer[4][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[4]<<", localvid: "<<actvv4.value<<", instance: "<<actvv4.key<<endl; }
		if(i < loadcount[5] && actvvbuffer[5][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[5]<<", localvid: "<<actvv5.value<<", instance: "<<actvv5.key<<endl; }
		if(i < loadcount[6] && actvvbuffer[6][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[6]<<", localvid: "<<actvv6.value<<", instance: "<<actvv6.key<<endl; }
		if(i < loadcount[7] && actvvbuffer[7][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[7]<<", localvid: "<<actvv7.value<<", instance: "<<actvv7.key<<endl; }
		if(i < loadcount[8] && actvvbuffer[8][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[8]<<", localvid: "<<actvv8.value<<", instance: "<<actvv8.key<<endl; }
		if(i < loadcount[9] && actvvbuffer[9][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[9]<<", localvid: "<<actvv9.value<<", instance: "<<actvv9.key<<endl; }
		if(i < loadcount[10] && actvvbuffer[10][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[10]<<", localvid: "<<actvv10.value<<", instance: "<<actvv10.key<<endl; }
		if(i < loadcount[11] && actvvbuffer[11][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[11]<<", localvid: "<<actvv11.value<<", instance: "<<actvv11.key<<endl; }
		if(i < loadcount[12] && actvvbuffer[12][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[12]<<", localvid: "<<actvv12.value<<", instance: "<<actvv12.key<<endl; }
		if(i < loadcount[13] && actvvbuffer[13][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[13]<<", localvid: "<<actvv13.value<<", instance: "<<actvv13.key<<endl; }
		if(i < loadcount[14] && actvvbuffer[14][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[14]<<", localvid: "<<actvv14.value<<", instance: "<<actvv14.key<<endl; }
		if(i < loadcount[15] && actvvbuffer[15][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[15]<<", localvid: "<<actvv15.value<<", instance: "<<actvv15.key<<endl; }
		#endif	
		
		UTILP0_SetDataset(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + offsetkvs_actvvs + i, global_actvvs);
	}
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES, maxloadcount_vs * VECTOR2_SIZE);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<"MERGE::EXCHANGE:: loadcount["<<v<<"]: "<<loadcount[v]<<endl; }
	if(maxloadcount_vs < maxnumactvvs_vs){ cout<<"MERGE::EXCHANGE:: too many active vertices. Use ACTS in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	else{ cout<<"MERGE::EXCHANGE:: few active vertices. Use TradGP in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	#endif 
	#endif 
	
	// judging what mode (ACTS or TradGP) to use for next iteration
	#if not defined (ALLVERTEXISACTIVE_ALGORITHM) && defined(MERGECONFIG_ACTIVATE_HYBRIDMODE) && defined(CONFIG_HYBRIDGPMODE_ONLINESWITCHING)
	unsigned int _mode = ON; if(maxloadcount_vs <= maxnumactvvs_vs){ _mode = OFF; } else { _mode = ON; }
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILP0_SetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(_mode == OFF){ cout<<"MERGE::EXCHANGE:: few active vertices. Use TradGP in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }	
	else{ cout<<"MERGE::EXCHANGE:: too many active vertices. Use ACTS in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	#endif 
	#endif 
	
	unsigned int k1; unsigned int k2; unsigned int k3; unsigned int num_acvvs2;
	#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
	k1 = UTILP0_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k2 = UTILP0_GetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k3 = UTILP0_GetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	num_acvvs2 = k1 + k2+ k3;
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1, num_acvvs2);
		#ifdef _DEBUGMODE_STATS
		cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: [@ 0][num processed edges stats @ processedges.cpp] (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs2<< "(k1: "<<k1<<", k2: "<<k2<<", k3: "<<k3<<")" << RESET << endl;
		#endif 
		
	k1=0; k2=0; k3=0;
	k1 = UTILP0_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k2 = UTILP0_GetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k3 = UTILP0_GetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	num_acvvs2 = k1 + k2+ k3;
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1, num_acvvs2);
	#ifdef _DEBUGMODE_STATS
	cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: [@ 32][num reduced vertex-update stats @ processedges.cpp] (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs2<< "(k1: "<<k1<<", k2: "<<k2<<", k3: "<<k3<<")" << RESET << endl;
	#endif
	#endif 
	
	#ifdef _DEBUGMODE_STATS
	k1=0; k2=0; k3=0;
	k1 = UTILP0_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k2 = UTILP0_GetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k3 = UTILP0_GetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	num_acvvs2 = k1 + k2+ k3;
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1, num_acvvs2);
	#ifdef _DEBUGMODE_STATS
	cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: [@ 64][num processed edges stats @ processedges.cpp->actsutility.cpp] (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs2<< "(k1: "<<k1<<", k2: "<<k2<<", k3: "<<k3<<")" << RESET << endl;
	#endif 
		
	k1=0; k2=0; k3=0;
	k1 = UTILP0_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k2 = UTILP0_GetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k3 = UTILP0_GetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	num_acvvs2 = k1 + k2+ k3;
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1, num_acvvs2);
	#ifdef _DEBUGMODE_STATS
	cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: [@ 96][num active vertices for next iteration @ mem_access.cpp] (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs2<< "(k1: "<<k1<<", k2: "<<k2<<", k3: "<<k3<<")" << RESET << endl;
	#endif
	#endif 
	// exit(EXIT_SUCCESS); //
	return;
}

extern "C" {
void TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
#pragma HLS INTERFACE m_axi port = vdramA offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdramB offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdramC offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = mdram offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = vdramA bundle = control
#pragma HLS INTERFACE s_axilite port = vdramB bundle = control
#pragma HLS INTERFACE s_axilite port = vdramC bundle = control
#pragma HLS INTERFACE s_axilite port = mdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = vdramA
#pragma HLS DATA_PACK variable = vdramB
#pragma HLS DATA_PACK variable = vdramC
#pragma HLS DATA_PACK variable = mdram

	// return;
	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	unsigned int inACTSMODE = UTILP0_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"~~~ TOPP0_topkernelS:: GraphIter: "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<endl;
	#endif
	
	#ifdef CONFIG_RELEASE_VERSION4
	MERGEP0_exchange(vdramA, vdramB, vdramC, mdram);
	#endif 
	return;
}
}



