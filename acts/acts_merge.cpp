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

void MERGEP0_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR_SIZE; v++){	
		unsigned int K = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].key;
		unsigned int V = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].value;
		if(MEMCAP0_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }
		if(MEMCAP0_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }	
	}
	#endif 
	return;
}

void MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << "mergeVs:: merging vertices (1)..."<< RESET <<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	bool pass3 = false; // false; // true;
	
	// retreive active partitions  (B)
	pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE];
	MEMACCESSP0_readhelperstats(kvdram, pmask, globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, NAp, globalparams);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"mergeVs:: printing active partitions..."<<endl;
	for(unsigned int p=0; p<globalparams.NUM_REDUCEPARTITIONS; p++){ if(pmask[p] > 0){ cout<<"pmask["<<p<<"] : "<<pmask[p]<<endl; }}
	#endif 

	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 	
	#endif 
	unsigned int i = globalparams.ACTSPARAMS_INSTID % NUMCOMPUTEUNITS_SLR2;
	unsigned int voffset_kvs2 = i * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"mergeVs:: vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<", num partitions: "<<globalparams.NUM_REDUCEPARTITIONS<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(pmask[partition] > 0 || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3 == true){ // REMOVEME 'true'
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"mergeVs:: Vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("mergeVs---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA); // globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE
				actsutilityobj->checkoutofbounds("mergeVs^^^: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); // globalparamsv.SIZE_DESTVERTICESDATA / VECTOR2_SIZE
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				MERGEP0_print_active_masks(vdram, globalparamsv, lbasevoffset_kvs2 + voffset_kvs2 + k);	
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"mergeVs:: merge operation finished."<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}

void MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << "exchangeVs:: exchanging vertices across different SLRs..." << RESET <<endl; 
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
	unsigned int numactvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=numactvvs complete
	unsigned int local_actvv[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv complete
	unsigned int local_actvv2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv2 complete
	unsigned int global_actvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=global_actvvs complete
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

	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR2 = NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR1 = NUMCOMPUTEUNITS_SLR1 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR0 = NUMCOMPUTEUNITS_SLR0 * globalparamsvA.NUM_REDUCEPARTITIONS;
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	
	uint32_type pmaski = 0;
	unsigned int totalvsynchronized = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
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
	cout<<"exchangeVs:: printing active global stats..."<<endl;
	for(unsigned int i=0; i<_TOTALNUMREDUCEPARTITIONS_SLR0; i++){ if(globalstatsbuffer0[i / 32].data[i % 32] > 0 || globalstatsbuffer1[i / 32].data[i % 32] > 0 || globalstatsbuffer2[i / 32].data[i % 32] > 0){ cout<<"EXCHANGE: globalstatsbuffer0["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer0[i][i / 32].data[i % 32]<<", globalstatsbuffer1["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer1[i / 32].data[i % 32]<<", globalstatsbuffer2["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer2[i / 32].data[i % 32]<<endl; }}					
	#endif 
	
	// vertices
	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 20", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, globalparamsvA.NUM_REDUCEPARTITIONS, NAp, NAp);
			#endif
			if(globalstatsbuffer0[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"exchangeVs vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 21", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 22", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					totalvsynchronized += VECTOR2_SIZE;
					#endif
					
					// collect active vertices
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					MERGEP0_print_active_masks(vdramA, globalparamsvA, voffsetA_kvs);
					#endif
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: FINISHED transferring vertices from vdramA to vdramB & vdramC. [end offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif

	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: transferring vertices from vdramB to vdramA & vdramC. [begin offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 21", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(globalstatsbuffer1[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"exchangeVs vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 24", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 25", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					totalvsynchronized += VECTOR2_SIZE;
					#endif
					
					// collect active vertices 
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					MERGEP0_print_active_masks(vdramB, globalparamsvB, voffsetB_kvs);
					#endif
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: finished transferring vertices from vdramB to vdramA & vdramC. [end offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	
	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: transferring vertices from vdramC to vdramA & vdramB. [begin offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 22", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			bool exchange_partition = true; if((globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + globalparamsvC.SIZEKVS2_REDUCEPARTITION > globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS)){ exchange_partition = false; }
			if((globalstatsbuffer2[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true) & (exchange_partition == true)){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"exchangeVs vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 27", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 28", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					totalvsynchronized += VECTOR2_SIZE;
					#endif
					
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k]; 
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					MERGEP0_print_active_masks(vdramC, globalparamsvC, voffsetC_kvs);
					#endif
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;			
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: finished transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"exchangeVs:: exchange operation finished. "<<totalvsynchronized<<" vertices synchronized."<<endl; 
	cout<<"exchangeVs:: exchange operation finished."<<endl; 
	#endif
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
	if(globalparamsvA.ENABLE_EXCHANGECOMMAND == ON){
		MERGEP0_exchange(vdramA, vdramB, vdramC, mdram);
	}
	#endif 
	return;
}
}



