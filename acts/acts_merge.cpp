void acts_all::MERGEP0_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR_SIZE; v++){	
		unsigned int K = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].key;
		unsigned int V = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].value;
		// if(MEMCAP0_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }
		// if(MEMCAP0_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }	
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
	
	// vertices data
	unsigned int voffset_kvs2 = globalparams.ACTSPARAMS_INSTID * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		// if(globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs2 + globalparams.SIZEKVS2_REDUCEPARTITION < globalparamsv.ACTSPARAMS_MAXHBMCAPACITY_KVS){
		MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){ // globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, globalparams.SIZEKVS2_REDUCEPARTITION
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("mergeVs: ERROR 21", voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, NAp, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
			actsutilityobj->checkoutofbounds("mergeVs: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); 
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
				value_t combo; if(v%2==0){ combo = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k].data[v/2].key; } else { combo = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k].data[v/2].key; }
				value_t vdata = combo >> 1; value_t mask = combo & 0x1;
				unsigned int lvid = (((reduce_partition * globalparams.SIZEKVS2_REDUCEPARTITION) + k) * VDATA_PACKINGSIZE) + v;
				if(mask == 1){ cout<<">>> MERGE:: MERGE VERTICES SEEN @ reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", vid: "<<UTILP0_GETREALVID(lvid, globalparams.ACTSPARAMS_INSTID)<<", combo: "<<combo<<", globalparams.SIZE_DESTVERTICESDATA: "<<globalparams.SIZE_DESTVERTICESDATA<<endl; }		
			}
			#endif
		}
		// }
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"mergeVs:: merge operation finished."<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}

unsigned int acts_all::MERGEP0_copy(uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, unsigned int voffset_kvs, unsigned int firstinstance_id, unsigned int num_compute_units, 
		unsigned int actvvs[VECTOR2_SIZE][BLOCKRAM_SIZE], tuple_t actvvs_size[VECTOR2_SIZE], 
		globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2){
	unsigned int total_sync = 0;
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
				
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
				
				UTILP0_ReadDatas(vdramSRC, globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k, datas);
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
				#pragma HLS UNROLL 
					vdramDST1[globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k].data[v].key = datas[2*v];
					vdramDST1[globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k].data[v].value = datas[2*v+1];
					
					vdramDST2[globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k].data[v].key = datas[2*v];
					vdramDST2[globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k].data[v].value = datas[2*v+1];
				}
				
				for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
				#pragma HLS UNROLL 
					value_t vdata = datas[v] >> 1; value_t mask = datas[v] & 0x1;
					unsigned int lvid = (((reduce_partition * globalparamsDST1.SIZEKVS2_REDUCEPARTITION) + k) * VDATA_PACKINGSIZE) + v;
					unsigned int vid = UTILP0_GETREALVID(lvid, firstinstance_id + i);
					if(mask == 1){ 
						if(actvvs_size[v].A < BLOCKRAM_SIZE){ 
							#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
							cout<<">>> EXCHANGE-COPY:: ACTIVE VERTEX SEEN @ i: "<<i<<", "<<"v: "<<v<<", vdata: "<<vdata<<", lvid: "<<lvid<<", vid: "<<vid<<endl; 
							#endif 
							actvvs[v][actvvs_size[v].A] = vid; 
							actvvs_size[v].A += 1;
						} else { actvvs_size[v].B += 1; } // else { cout<<"merge: EXITING.... 456. "<<endl; exit(EXIT_FAILURE); }
						// actvvs_size[v].B += 1; //////////////
					}
				}
	
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

void acts_all::MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> exchangeVs:: exchanging vertices across different SLRs..." << RESET <<endl; 
	#endif

	unsigned int total_sync[3]; total_sync[0]=0; total_sync[1]=0; total_sync[2]=0; 
	#pragma HLS ARRAY_PARTITION variable=total_sync complete
	
	unsigned int actvvs[VECTOR2_SIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = actvvs
	tuple_t actvvs_size[VECTOR2_SIZE];
	for(int v = 0; v < VECTOR2_SIZE; v++){ actvvs_size[v].A = 0; actvvs_size[v].B = 0; }
	
	// for(int t = 0; t < BLOCKRAM_SIZE; t++){ // REMOVEME.
		// for(int v = 0; v < VECTOR2_SIZE; v++){
		// #pragma HLS UNROLL 
			// actvvs[t][v] = 0;
		// }
	// }

	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR1) * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	
	total_sync[0] = MERGEP0_copy(vdramA, vdramB, vdramC, voffsetA_kvs, 0, NUMCOMPUTEUNITS_SLR2, actvvs, actvvs_size, globalparamsvA, globalparamsvB, globalparamsvC);
	total_sync[1] = MERGEP0_copy(vdramB, vdramA, vdramC, voffsetB_kvs, NUMCOMPUTEUNITS_SLR2, NUMCOMPUTEUNITS_SLR1, actvvs, actvvs_size, globalparamsvB, globalparamsvA, globalparamsvC);
	total_sync[2] = MERGEP0_copy(vdramC, vdramA, vdramB, voffsetC_kvs, NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1, NUMCOMPUTEUNITS_SLR0, actvvs, actvvs_size, globalparamsvC, globalparamsvA, globalparamsvB);
	
	// FIXME. too expensive.
	unsigned int iteration_stats[BLOCKRAM_SIZE]; for(unsigned int t=0; t<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; t++){ iteration_stats[t] = 0; }
	unsigned int edgeblock_stats[MAXNUM_VPs][MAXNUM_EDGEBLOCKS_PER_VPARTITION]; 
	unsigned int sum_A = 0; for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){ sum_A += actvvs_size[v].A; }
	unsigned int sum_B = 0; for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){ sum_B += actvvs_size[v].B; }
	
	if(sum_B == 0){
		for(unsigned int v_p=0; v_p<MAXNUM_VPs; v_p++){ for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){ edgeblock_stats[v_p][t] = 0; }}
		
		for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
			for(unsigned int t=0; t<actvvs_size[v].A; t++){
				unsigned int vid = actvvs[v][t];
				unsigned int v_p = vid / (globalparamsvA.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE);
				unsigned int lvid = vid - (v_p * globalparamsvA.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE);
				
				if(edgeblock_stats[v_p][lvid / MAXVSIZE_ACTVEDGEBLOCK] == 0){ 
					edgeblock_stats[v_p][lvid / MAXVSIZE_ACTVEDGEBLOCK] = 1; 
					iteration_stats[vid / (globalparamsvA.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE)] += 1; 
				}
			}
		}
		
		for(unsigned int v_p=0; v_p<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v_p++){
			unsigned int index = 0;
			for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){ 
				if(edgeblock_stats[v_p][t] > 0){ 
					MEMACCESSP0_setdata(vdramA, globalparamsvA.BASEOFFSETKVS_ACTIVEVERTICES, (v_p * CONFIG_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION) + index, t);
					MEMACCESSP0_setdata(vdramB, globalparamsvB.BASEOFFSETKVS_ACTIVEVERTICES, (v_p * CONFIG_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION) + index, t);
					MEMACCESSP0_setdata(vdramC, globalparamsvC.BASEOFFSETKVS_ACTIVEVERTICES, (v_p * CONFIG_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION) + index, t);
					index += 1;
				}
			}
		}
	
		#ifdef _DEBUGMODE_KERNELPRINTS4
		unsigned int sum=0, sum2=0, sum3=0, sum1 = 0; bool debug = false, debug1 = false;
		if(debug==true){ cout<<">>> exchange:: printing iteration_stats... "<<endl; }
		for(unsigned int v_p=0; v_p<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v_p++){ sum1 += iteration_stats[v_p]; if(debug1==true){ cout<<"--- iteration_stats["<<v_p<<"]: "<<iteration_stats[v_p]<<endl; }}
		if(debug==true){ cout<<"+++::: sum1: "<<sum1<<endl; }
		cout<<">>> exchange:: printing actvvs_size... "<<endl; // VDATA_PACKINGSIZE
		for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){ for(unsigned int t=0; t<actvvs_size[v].A; t++){ sum += actvvs_size[v].A; if(debug1==true){ cout<<"$$$ EXCHANGE:: ACTIVE VERTEX SEEN @ actvvs["<<v<<"]["<<t<<"]: "<<actvvs[v][t]<<endl; }}} // VDATA_PACKINGSIZE
		if(debug==true){ cout<<">>> exchange:: printing edgeblock_stats... "<<endl; }
		for(unsigned int v_p=0; v_p<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v_p++){
			for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){ if(edgeblock_stats[v_p][t] > 0){ sum2 += edgeblock_stats[v_p][t]; if(debug1==true){ cout<<"+++ edgeblock_stats["<<v_p<<"]["<<t<<"]: "<<edgeblock_stats[v_p][t]<<endl; }}}}
		if(debug==true){ cout<<"+++::: sum2: "<<sum2<<endl; }
		if(debug==true){ cout<<">>> exchange:: printing pmask_curr... "<<endl; }
		pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
		MEMACCESSP0_readhelperstats(vdramA, pmask_curr, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
		for(unsigned int t=0; t<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; t++){ sum3 += pmask_curr[t]; if(debug1==true){ cout<<"~~~ pmask_curr["<<t<<"]: "<<pmask_curr[t]<<endl; }}
		if(debug==true){ cout<<"~~~::: sum3: "<<sum3<<endl; }
		#endif 
		#ifdef _DEBUGMODE_HOSTCHECKS3
		if(sum1 != sum2 & sum1 != sum3 || sum2 != sum3){ cout<<"EXCHANGE ERROR with sum3,2,3: sum1: "<<sum1<<", sum2: "<<sum2<<", sum3: "<<sum3<<". EXITING..."<<endl; }//exit(EXIT_FAILURE); }
		#endif
		cout<<"EXCHANGE: sum3,2,3: sum1: "<<sum1<<", sum2: "<<sum2<<", sum3: "<<sum3<<"..."<<endl;
	}
	// exit(EXIT_SUCCESS);
				
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"finished. "<<total_sync[0] + total_sync[1] + total_sync[2]<<" vertices synchronized"<<endl; 
	#endif
	return;
}

extern "C" {
void acts_all::TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
#pragma HLS INTERFACE m_axi port = vdramA offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdramB offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdramC offset = slave bundle = gmem2

#pragma HLS INTERFACE s_axilite port = vdramA bundle = control
#pragma HLS INTERFACE s_axilite port = vdramB bundle = control
#pragma HLS INTERFACE s_axilite port = vdramC bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = vdramA
#pragma HLS DATA_PACK variable = vdramB
#pragma HLS DATA_PACK variable = vdramC

	// return;
	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	unsigned int inACTSMODE = UTILP0_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"~~~ TOPP0_topkernelS:: GraphIter: "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<endl;
	#endif
	
	#ifdef CONFIG_RELEASE_VERSION4
	if(globalparamsvA.ENABLE_EXCHANGECOMMAND == ON){
		MERGEP0_exchange(vdramA, vdramB, vdramC);
	}
	#endif 
	return;
}
}



