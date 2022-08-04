void acts_all::mergeP0_trace(uint512_dt * kvdram, unsigned int reduce_partition, unsigned int k, unsigned int offset_kvs, globalparams_t globalparams){
	for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
		value_t combo; if(v%2==0){ combo = kvdram[offset_kvs].data[v/2].key; } else { combo = kvdram[offset_kvs].data[v/2].key; }
		value_t vdata = combo >> 1; value_t mask = combo & 0x1;
		unsigned int lvid = (((reduce_partition * globalparams.SIZEKVS2_REDUCEPARTITION) + k) * VDATA_PACKINGSIZE) + v;
		#ifdef _DEBUGMODE_KERNELPRINTS4
		if(mask == 1){ cout<<">>> MERGE:: MERGE VERTICES SEEN @ reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", vid: "<<UTILP0_GETREALVID(lvid, globalparams.ACTSPARAMS_INSTID)<<", combo: "<<combo<<", globalparams.SIZE_DESTVERTICESDATA: "<<globalparams.SIZE_DESTVERTICESDATA<<endl; }	
		#endif 
	}
}

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

unsigned int acts_all::mergeP0_get_updateblock_workload(unsigned int en, unsigned int reduce_partition, unsigned int * stats_offsets, unsigned int * stats_metadata, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE]){
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	workload_t workload_kvs;
	
	bool sparse_v = false; if(stats_metadata[reduce_partition+1] - stats_metadata[reduce_partition] < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION){ sparse_v = true; } else { sparse_v = false; }
	if(en == false || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_v = false; }
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparams.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; // 16
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock; // 512
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(sparse_v == true){ cout<<"merge:: sparse_sync is TRUE. stats[inst:"<<globalparams.ACTSPARAMS_INSTID<<"][1]["<<reduce_partition<<"]: "<<stats_metadata[reduce_partition]<<"->"<<stats_metadata[reduce_partition+1]<<endl; } else { cout<<"merge:: sparse_sync is FALSE. stats[inst:"<<globalparams.ACTSPARAMS_INSTID<<"][1]["<<reduce_partition<<"]: "<<stats_metadata[reduce_partition]<<"->"<<stats_metadata[reduce_partition+1]<<endl; }					
	#endif 
	unsigned int num_its; if(sparse_v == true){ num_its = stats_metadata[reduce_partition+1] - stats_metadata[reduce_partition]; } else { num_its = 1; }
	#ifdef _DEBUGMODE_CHECKS3
	if((stats_metadata[reduce_partition+1] < stats_metadata[reduce_partition]) || (num_its > 600)){ cout<<"_get_updateblock_workload. ERROR 23. error somewhere. EXIITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	for(unsigned int n=0; n<num_its; n++){
		if(sparse_v == true){
			unsigned int updateblock_id = stats_offsets[stats_metadata[reduce_partition] + n]; 
			workload_kvs.offset_begin = updateblock_id * numkvs_vertices_per_updateblock; 
			workload_kvs.size = numkvs_vertices_per_updateblock;  
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			xload_kvs[n] = workload_kvs;
			unsigned int buffer_offset = updateblock_id * numkvs_vertices_per_updateblock;
			buffer_offsets[n] = buffer_offset;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_updateblock_workload(112)::DEBUG CODE 112::1", buffer_offset, MAX_BLOCKRAM_VDESTDATA_SIZE, updateblock_id, numkvs_vertices_per_updateblock, numkvs_updateblocks_per_reducepartition); 
			#endif	
		} else {
			workload_kvs.offset_begin = 0;
			workload_kvs.size = globalparams.SIZEKVS2_REDUCEPARTITION;
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			xload_kvs[n] = workload_kvs;
			unsigned int buffer_offset = 0;
			buffer_offsets[n] = buffer_offset;
		}
	}
	return num_its;
}

unsigned int acts_all::mergeP0_get_upropblock_workload(unsigned int en, unsigned int process_partition, uint512_dt * dram, unsigned int num_active_edgeblocks, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE], unsigned int graphiterationid){
	#pragma HLS INLINE
	
	bool sparse_readu = false; if(num_active_edgeblocks < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
	if(en == false || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = false; }
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	unsigned int edgeblockoffset_vs = (process_partition * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;	
	#else  
	unsigned int edgeblockoffset_vs = ((process_partition * MAXNUMGRAPHITERATIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) + (graphiterationid * MAXSZ_EDGEBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
	#endif 
	unsigned int num_its; if(sparse_readu == true){ num_its = num_active_edgeblocks; } else { num_its = 1; }
	workload_t workload_kvs;
	unsigned int buffer_offset = 0;
	
	for(unsigned int n=0; n<num_its; n++){
		if(sparse_readu == true){
			#ifdef _COLLECT_METADATA_FROM_KERNEL_
			unsigned int edgeblock_id = UTILP0_ReadData(dram, globalparams.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + edgeblockoffset_vs, 1 + n);
			#else  
			unsigned int edgeblock_id = UTILP0_ReadData(dram, globalparams.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + edgeblockoffset_vs, n);
			#endif 
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21", edgeblock_id, MAXSZ_EDGEBLOCKS_PER_VPARTITION, process_partition, n, num_active_edgeblocks);
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21", edgeblock_id, MAXSZ_EDGEBLOCKS_PER_VPARTITION, process_partition, n, globalparams.ALGORITHMINFO_GRAPHITERATIONID);
			#endif
			// cout<<"------------------------ _get_upropblock_workload: edgeblock_id: "<<edgeblock_id<<", globalparams.ALGORITHMINFO_GRAPHITERATIONID: "<<globalparams.ALGORITHMINFO_GRAPHITERATIONID<<", n: "<<n<<endl;
			workload_kvs.offset_begin = 0;
			workload_kvs.size = 1; 
			buffer_offset = edgeblock_id * (NUM_VERTICES_PER_EDGEBLOCK / VDATA_PACKINGSIZE);
		} else {
			workload_kvs.offset_begin = 0;
			workload_kvs.size = globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
			buffer_offset = 0;
		}
		workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
		xload_kvs[n] = workload_kvs;
		buffer_offsets[n] = buffer_offset;
	}
	// exit(EXIT_SUCCESS);
	return num_its;
}

unsigned int acts_all::mergeP0_get_edgeblock_workload(unsigned int en, unsigned int process_partition, unsigned int llp_set, uint512_dt * dram, unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE, workload_t xload_kvs[DOUBLE_BLOCKRAM_SIZE], unsigned int graphiterationid){
	
	keyvalue_t datas[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	
	bool sparse_process = false; if(num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	if(en == false || globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = false; }
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	unsigned int edgeblockoffset_vs = (process_partition * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;	// NEWCHANGE.
	#else  
	unsigned int edgeblockoffset_vs = ((process_partition * MAXNUMGRAPHITERATIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) + (graphiterationid * MAXSZ_EDGEBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
	#endif 
	// unsigned int edgeblockmapoffset_kvs = (process_partition * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR_SIZE;
	unsigned int edgeblockmapoffset_kvs = process_partition * MAXSZ_EDGEBLOCKS_PER_VPARTITION; // NEWCHANGE
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	unsigned int num_its; if(sparse_process == true){ num_its = num_active_edgeblocks; } else { num_its = 1; }
	
	READ_PROCESS_PARTITION_WRITE_GETWORKLOADSTATS_LOOP: for(unsigned int n=0; n<num_its; n++){
		workload_t workload_kvs;					
		if(sparse_process == true){
			#ifdef _COLLECT_METADATA_FROM_KERNEL_
			unsigned int edgeblock_id = UTILP0_ReadData(dram, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + edgeblockoffset_vs, 1 + n);
			#else  
			unsigned int edgeblock_id = UTILP0_ReadData(dram, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + edgeblockoffset_vs, n);
			#endif
			UTILP0_ReadDatas(dram, globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP + edgeblockmapoffset_kvs + edgeblock_id, datas);
			workload_kvs.offset_begin = datas[llp_set].key; workload_kvs.size = datas[llp_set].value; 
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("actit(1)", edgeblock_id, MAXSZ_EDGEBLOCKS_PER_VPARTITION, NAp, NAp, n);
			actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.offset_begin, (1 << 28) / 8, NAp, NAp, n);
			actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.size, (1 << 28) / 8, NAp, NAp, n);
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"actit/sparse_process:: active edge-block seen @ ["<<globalparamsK.ACTSPARAMS_INSTID<<"]["<<process_partition<<"]["<<llp_set<<"]: edgeblock-id: "<<edgeblock_id<<", edgeblock-map: "<<"["<<workload_kvs.offset_begin<<", "<<workload_kvs.size<<"]"<<endl;
			#endif 
		} else {
			unsigned int index = (process_partition * num_LLPset) + llp_set;
			workload_kvs.offset_begin = UTILP0_ReadData(dram, globalparamsE.BASEOFFSETKVS_EDGESMAP, index) / EDGEDATA_PACKINGSIZE;
			workload_kvs.offset_end = UTILP0_ReadData(dram, globalparamsE.BASEOFFSETKVS_EDGESMAP, index + 1) / EDGEDATA_PACKINGSIZE;
			workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"actit/dense_process:: full edge-block seen @ ["<<globalparamsK.ACTSPARAMS_INSTID<<"]["<<process_partition<<"]: edgeblock-id: NAp, edgeblock-map: "<<"["<<workload_kvs.offset_begin<<", "<<workload_kvs.size<<"]"<<endl;
			#endif 
		}
	
		if(workload_kvs.offset_end < workload_kvs.offset_begin){ workload_kvs.size = 0; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"actit:: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<", workload_kvs.offset_end: "<<workload_kvs.offset_end<<", workload_kvs.size: "<<workload_kvs.size<<"] "<<endl;
		#endif
		xload_kvs[n] = workload_kvs;
	}
	return num_its;
}

void acts_all::MERGEP0_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> mergeVs:: merging vertices..."<< RESET <<endl; 
	#endif
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	unsigned int xblock_ids[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	workload_t workload_uprop_kvs;
	unsigned int total_vertices_merged = 0;
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	unsigned int stats[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef TESTKERNEL 
	if(UTILP0_isbufferused(globalparams.ACTSPARAMS_INSTID) == false){ return; }
	#endif 
	
	unsigned int voffset_kvs2 = globalparams.ACTSPARAMS_INSTID * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	
	if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		for(unsigned int t=0; t<16; t++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD 
			stats[1][t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + t].range(63, 32);
			#else 
			stats[1][t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + t].data[0].value;
			#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int i=0; i<4; i++){ cout<<"merge:: stats[0]["<<i<<"]: "<<stats[0][i]<<", "<<endl; }
	for(unsigned int i=0; i<4; i++){ cout<<"merge:: stats[1]["<<i<<"]: "<<stats[1][i]<<", "<<endl; }
	#endif
	
	MERGEP0_MERGEVSLOOP2: for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		
		unsigned int num_its = 1; xload_kvs[0].offset_begin = 0; xload_kvs[0].size = globalparams.SIZEKVS2_REDUCEPARTITION;
		if(globalparams.NUM_REDUCEPARTITIONS > 1){ if(reduce_partition == globalparams.NUM_REDUCEPARTITIONS-1){ xload_kvs[0].size = (globalparams.ACTSPARAMS_BATCH_RANGE / VECTOR2_SIZE) % globalparams.SIZEKVS2_REDUCEPARTITION; } else { xload_kvs[0].size = globalparams.SIZEKVS2_REDUCEPARTITION; }}
			
		if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
			unsigned int num_update_blocks = stats[1][reduce_partition+1] - stats[1][reduce_partition];
			bool sparse_merge = false; if(num_update_blocks < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION){ sparse_merge = true; } else { sparse_merge = false; }
			if(sparse_merge == true){ 
				unsigned int offset_buff = stats[1][reduce_partition];
				for(unsigned int t=0; t<num_update_blocks; t++){ 
				#pragma HLS PIPELINE II=1
					#ifdef _WIDEWORD 
					stats[0][offset_buff + t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + offset_buff + t].range(31, 0)
					#else 
					stats[0][offset_buff + t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + offset_buff + t].data[0].key;
					#endif 
				}
			}
			// check this. =false works, =true not work?
			num_its = mergeP0_get_updateblock_workload(true, reduce_partition, stats[0], stats[1], globalparams, xload_kvs, buffer_offsets); 
		}
		
		MERGEP0_MERGEVSLOOP2B: for(unsigned int n=0; n<num_its; n++){
			MERGEP0_MERGEVSLOOP2C: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].offset_begin + xload_kvs[n].size; k++){ // globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, globalparams.SIZEKVS2_REDUCEPARTITION
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("mergeVs: ERROR 21", voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, NAp, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("mergeVs: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); 
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				mergeP0_trace(kvdram, reduce_partition, k, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k, globalparams); total_vertices_merged += VDATA_PACKINGSIZE;
				#endif
				#ifdef _DEBUGMODE_STATS
				kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[1].key += UPDATEDATA_PACKINGSIZE;
				#endif 
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"mergeVs:: merge operation finished. total_vertices_merged: "<<total_vertices_merged<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}

void acts_all::MERGEP0_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram, unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs){
	MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){
	#pragma HLS PIPELINE II=1
		kvdram[offset2_kvs + k] = vdram[offset1_kvs + k];
	}
	return;
}

void acts_all::MERGEP0_broadcastVs1(uint512_dt * vdram, uint512_dt * kvdram0, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs2(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs3(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs4(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs5(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs6(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs7(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs8(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs9(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram8, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram8, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram8, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs10(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram8, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram9, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram8, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram9, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram8, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram9, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs11(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram8, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram9, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram10, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram8, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram9, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram10, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram8, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram9, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram10, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGEP0_broadcastVs12(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeP0_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGEP0_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILP0_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram8, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram9, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram10, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILP0_WriteDatas(kvdram11, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILP0_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILP0_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram8, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram9, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram10, offset2_kvs + k, datas);
			// 			// UTILP0_WriteDatas(kvdram11, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGEP0_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILP0_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILP0_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram8, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram9, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram10, offset2b_kvs + k, datas);
			UTILP0_WriteDatas(kvdram11, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}

unsigned int acts_all::MERGEP0_copy(unsigned int idx, unsigned int reduce_partition, uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, unsigned int stats[2][BLOCKRAM_SIZE], unsigned int voffset_kvs, unsigned int firstinstance_id, unsigned int num_compute_units, 
		pmask_dt vpartition_stats[BLOCKRAM_CURRPMASK_SIZE], wide_word_bits_3t edgeblock_stats[MAX_RED_SRAMSZ],
		globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2){
	unsigned int total_sync = 0;
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	value_t mask[VDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable=mask complete
	unsigned int vid[VDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable=mask complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	unsigned int xblock_ids[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	workload_t workload_uprop_kvs;
	unsigned int total_vertices_synchd = 0;
		
	unsigned int v2offset_kvs = reduce_partition * globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
	for(unsigned int t=0; t<globalparamsSRC.SIZEKVS2_REDUCEPARTITION; t++){ edgeblock_stats[t].data[idx] = 0; } // FIXME. TOO EXPENSIVE.?
	unsigned int lvid_first = reduce_partition * globalparamsSRC.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE; // (((reduce_partition * globalparamsDST1.SIZEKVS2_REDUCEPARTITION) + k) * VDATA_PACKINGSIZE) + v;
	unsigned int vbegin = reduce_partition * globalparamsSRC.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE * NUM_PEs;

	EXCHANGEP0_COPY_LOOP1: for(unsigned int i=0; i<num_compute_units; i++){
		
		unsigned int reducepsz_kvs = globalparamsSRC.SIZEKVS2_REDUCEPARTITION; 
		if(globalparamsSRC.NUM_REDUCEPARTITIONS > 1){ if(reduce_partition == globalparamsSRC.NUM_REDUCEPARTITIONS-1){ reducepsz_kvs = (globalparamsSRC.ACTSPARAMS_BATCH_RANGE / VECTOR2_SIZE) % globalparamsSRC.SIZEKVS2_REDUCEPARTITION; } else { reducepsz_kvs = globalparamsSRC.SIZEKVS2_REDUCEPARTITION; }}
		// unsigned int num_its = mergeP0_get_updateblock_workload(reduce_partition, stats[0], stats[1], globalparamsSRC, xload_kvs);
		
		unsigned int num_its = 1;
		xload_kvs[0].offset_begin = 0;
		xload_kvs[0].size = globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
		
		for(unsigned int n=0; n<num_its; n++){
			for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].offset_begin + xload_kvs[n].size; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 21", globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 22", globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				total_sync += VECTOR2_SIZE;
				#endif
				
				UTILP0_ReadDatas(vdramSRC, globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas);
				UTILP0_WriteDatas(vdramDST1, globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas); // NEWCHANGE.
				UTILP0_WriteDatas(vdramDST2, globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas);
				
				for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
				#pragma HLS UNROLL 
					value_t vdata = datas[v] >> 1; mask[v] = datas[v] & 0x1;
					unsigned int lvid = lvid_first + (k * VDATA_PACKINGSIZE) + v; vid[0] = UTILP0_GETREALVID(lvid, firstinstance_id + i); // FIXME. MAKE SIMPLER. // vid[0] = vbegin + (i * VDATA_PACKINGSIZE) + k;
					#ifdef _DEBUGMODE_CHECKS3
					unsigned int check_lvid = lvid_first + (k * VDATA_PACKINGSIZE) + v; unsigned int check_v0 = UTILP0_GETREALVID(check_lvid, firstinstance_id + i); // FIXME. MAKE SIMPLER.
					if(vid[0] != check_v0){ cout<<"exchange. seperate calculations of vid[0] don't match. vid[0]("<<vid[0]<<") != check_v0("<<check_v0<<"). EXIITING..."<<endl; exit(EXIT_FAILURE); }
					#endif 
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					if(mask[v] == 1){ cout<<">>> EXCHANGE-COPY:: ACTIVE VERTEX SEEN @ [i: "<<i<<", reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<"] vdata: "<<vdata<<", lvid: "<<lvid<<", vid[0]: "<<vid[0]<<endl; }
					#endif 
				}	
				
				if(mask[0]==1 || mask[1]==1 || mask[2]==1 || mask[3]==1 || mask[4]==1 || mask[5]==1 || mask[6]==1 || mask[7]==1 || mask[8]==1 || mask[9]==1 || mask[10]==1 || mask[11]==1 || mask[12]==1 || mask[13]==1 || mask[14]==1 || mask[15]==1){
					if(edgeblock_stats[k].data[0] == 0 && edgeblock_stats[k].data[1] == 0 && edgeblock_stats[k].data[2] == 0){ 
						edgeblock_stats[k].data[idx] = 1; 
						unsigned int v_p = vid[0] / (globalparamsSRC.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE);
						vpartition_stats[v_p] += 1;
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						cout<<"+++ EXCHANGE-COPY:: ACTIVE EDGE-BLOCK SEEN @ edgeblock_stats["<<firstinstance_id + i<<"]["<<v_p<<"]["<<k<<"]. "<<endl; 
						#endif 
					}
				}
			} // end-of-k
		}
		v2offset_kvs += globalparamsSRC.NUM_REDUCEPARTITIONS * globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
	} // end-of-num_compute_units
	return 0;
}

void acts_all::MERGEP0_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> exchangeVs:: exchanging vertices across different SLRs..." << RESET <<endl; 
	#endif

	unsigned int total_sync[3]; total_sync[0]=0; total_sync[1]=0; total_sync[2]=0; 
	#pragma HLS ARRAY_PARTITION variable=total_sync complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	value_t datas1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas1 complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas2 complete
	
	wide_word_bits_3t edgeblock_stats[MAX_RED_SRAMSZ];
	#pragma HLS DATA_PACK variable = edgeblock_stats
	pmask_dt vpartition_stats[BLOCKRAM_CURRPMASK_SIZE];
	unsigned int statsA[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsA
	unsigned int statsB[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsB
	unsigned int statsC[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsC

	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	
	for(unsigned int t=0; t<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; t++){ vpartition_stats[t] = 0; }
	for(unsigned int t=0; t<globalparamsvA.SIZEKVS2_REDUCEPARTITION; t++){ edgeblock_stats[t].data[0] = 0; edgeblock_stats[t].data[1] = 0; edgeblock_stats[t].data[2] = 0; }
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR1) * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	
	// unsigned int reduce_partition = 0;
	EXCHANGEP0_LOOP1: for(unsigned int reduce_partition=0; reduce_partition<globalparamsvA.NUM_REDUCEPARTITIONS; reduce_partition++){
		total_sync[0] = MERGEP0_copy(0, reduce_partition, vdramA, vdramB, vdramC, statsA, voffsetA_kvs, 0, NUMCOMPUTEUNITS_SLR2, vpartition_stats, edgeblock_stats, globalparamsvA, globalparamsvB, globalparamsvC);
		total_sync[1] = MERGEP0_copy(1, reduce_partition, vdramB, vdramA, vdramC, statsB, voffsetB_kvs, NUMCOMPUTEUNITS_SLR2, NUMCOMPUTEUNITS_SLR1, vpartition_stats, edgeblock_stats, globalparamsvB, globalparamsvA, globalparamsvC);
		total_sync[2] = MERGEP0_copy(2, reduce_partition, vdramC, vdramA, vdramB, statsC, voffsetC_kvs, NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1, NUMCOMPUTEUNITS_SLR0, vpartition_stats, edgeblock_stats, globalparamsvC, globalparamsvA, globalparamsvB);
		
		#ifdef _COLLECT_METADATA_FROM_KERNEL_
		EXCHANGEP0_LOOP1B: for(unsigned int v_p=0; v_p<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v_p++){
			if(vpartition_stats[v_p] < globalparamsvA.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){
				unsigned int offset_vs = (v_p * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VDATA_PACKINGSIZE; // NEWCHANGE.
				unsigned int begin = v_p * MAXSZ_EDGEBLOCKS_PER_VPARTITION; unsigned int sz = MAXSZ_EDGEBLOCKS_PER_VPARTITION; // unsigned int end = begin + sz;
				if(begin + sz >= globalparamsvA.SIZEKVS2_REDUCEPARTITION){ sz = globalparamsvA.SIZEKVS2_REDUCEPARTITION - begin; }
				unsigned int index = 1; // 0;
				for(unsigned int t=begin; t<begin + sz; t++){
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 24", t, globalparamsvA.SIZEKVS2_REDUCEPARTITION, begin, begin + sz, sz);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 25", t, MAX_RED_SRAMSZ, begin, begin + sz, sz);
					#endif
					if(edgeblock_stats[t].data[0] > 0 || edgeblock_stats[t].data[1] > 0 || edgeblock_stats[t].data[2] > 0){
						UTILP0_WriteData(vdramA, globalparamsvA.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						UTILP0_WriteData(vdramB, globalparamsvB.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						UTILP0_WriteData(vdramC, globalparamsvC.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						index += 1; // edgeblock_stats[t] = 0;
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 26", t-begin, MAXSZ_EDGEBLOCKS_PER_VPARTITION, NAp, NAp, NAp);
						#endif
						// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						cout<<"+++ EXCHANGE:: ACTIVE EDGE-BLOCK SEEN @ ["<<v_p<<"]["<<t<<"] "<<endl; 
						// #endif
					}
				}
				UTILP0_WriteData(vdramA, globalparamsvA.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
				UTILP0_WriteData(vdramB, globalparamsvB.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
				UTILP0_WriteData(vdramC, globalparamsvC.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
			}
		}
		#endif 
	}
				
	#ifdef _DEBUGMODE_KERNELPRINTS	
	unsigned int sum=0, sum1=0, sum2=0, sum3=0, sum4 = 0; 
	bool debug = true; bool debug1 = true;
	// bool debug = true; bool debug1 = false;
	
	if(debug==true){ cout<<">>> exchange-copy:: printing vpartition_stats... "<<endl; }
	for(unsigned int v_p=0; v_p<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v_p++){ sum4 += vpartition_stats[v_p]; if(debug1==true){ cout<<"--- vpartition_stats["<<v_p<<"]: "<<vpartition_stats[v_p]<<endl; }}
	if(debug==true){ cout<<"+++::: sum4: "<<sum4<<endl; }
	
	if(debug==true){ cout<<">>> exchange-copy:: printing pmask_curr... "<<endl; }
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	MEMACCESSP0_readhelperstats(vdramA, pmask_curr, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	for(unsigned int t=0; t<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; t++){ sum3 += pmask_curr[t]; if(debug1==true){ cout<<"~~~ pmask_curr["<<t<<"]: "<<pmask_curr[t]<<endl; }}
	if(debug==true){ cout<<"~~~::: sum3: "<<sum3<<endl; }
	cout<<"exchange-copy: sum3,2,3,4: sum1: "<<sum1<<", sum2: "<<sum2<<", sum3: "<<sum3<<", sum4: "<<sum4<<"..."<<endl;
	#endif	
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	MEMACCESSP0_savehelperstats(vdramA, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	MEMACCESSP0_savehelperstats(vdramB, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	MEMACCESSP0_savehelperstats(vdramC, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	#endif 
	
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



