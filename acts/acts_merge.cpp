void acts_all::MERGEP0_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs){
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

void acts_all::MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> mergeVs:: merging vertices..."<< RESET <<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	unsigned int slrvoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2){ slrvoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ slrvoffset_kvs2 = globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ slrvoffset_kvs2 = (globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }										
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"mergeVs:: merging vertices. [begin offset: "<<slrvoffset_kvs2<<"]"<<endl; 	
	#endif 
	unsigned int i = globalparams.ACTSPARAMS_INSTID % globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2;
	unsigned int voffset_kvs2 = i * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + slrvoffset_kvs2 + voffset_kvs2 + globalparams.SIZEKVS2_REDUCEPARTITION < globalparamsv.ACTSPARAMS_MAXHBMCAPACITY_KVS){
		MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("mergeVs: ERROR 21", slrvoffset_kvs2 + voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, slrvoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
			actsutilityobj->checkoutofbounds("mergeVs: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); 
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + slrvoffset_kvs2 + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			MERGEP0_print_active_masks(vdram, globalparamsv, slrvoffset_kvs2 + voffset_kvs2 + k);	
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

unsigned int acts_all::MERGEP0_copy(uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, unsigned int voffset_kvs, unsigned int num_compute_units, 
		globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2){
	unsigned int total_sync = 0;
	MERGEP0_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<num_compute_units; i++){
		for(unsigned int reduce_partition=0; reduce_partition<globalparamsSRC.NUM_REDUCEPARTITIONS; reduce_partition++){
			if(globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + globalparamsSRC.SIZEKVS2_REDUCEPARTITION < globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS){
			MERGEP0_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsSRC.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 21", globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 22", globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				total_sync += VECTOR2_SIZE;
				#endif
				
				vdramDST1[globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k] = vdramSRC[globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k];
				vdramDST2[globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k] = vdramSRC[globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				MERGEP0_print_active_masks(vdramSRC, globalparamsSRC, voffset_kvs);
				#endif
			}
			}
			voffset_kvs += globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
		}
	}
	return total_sync;
}

void acts_all::MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> exchangeVs:: exchanging vertices across different SLRs..." << RESET <<endl; 
	#endif

	unsigned int total_sync[3]; total_sync[0]=0; total_sync[1]=0; total_sync[2]=0; 
	#pragma HLS ARRAY_PARTITION variable=total_sync complete

	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR1) * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	
	total_sync[0] = MERGEP0_copy(vdramA, vdramB, vdramC, voffsetA_kvs, NUMCOMPUTEUNITS_SLR2, globalparamsvA, globalparamsvB, globalparamsvC);
	total_sync[1] = MERGEP0_copy(vdramB, vdramA, vdramC, voffsetB_kvs, NUMCOMPUTEUNITS_SLR1, globalparamsvB, globalparamsvA, globalparamsvC);
	total_sync[2] = MERGEP0_copy(vdramC, vdramA, vdramB, voffsetC_kvs, NUMCOMPUTEUNITS_SLR0, globalparamsvC, globalparamsvA, globalparamsvB);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"finished. "<<total_sync[0] + total_sync[1] + total_sync[2]<<" vertices synchronized"<<endl; 
	#endif
	return;
}

extern "C" {
void acts_all::TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
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



