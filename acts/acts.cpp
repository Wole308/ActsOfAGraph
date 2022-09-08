// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Vitis/multiple_kernels/vadd_kernel/krnl_vadd.cpp
// https://github.com/Xilinx/SDAccel_Examples/blob/master/getting_started/dataflow/dataflow_stream_c/src/adder.cpp
// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Dataflow/Channels/using_fifos/diamond.cpp  *

void acts_all::ACTSP0_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats_kvs[BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], 
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,				
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	
	keyvalue_actvbuffer_t actvvbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = actvvbuffer
	unsigned int actvvcounters[VECTOR2_SIZE][BLOCKRAM_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = actvvcounters
	
	unsigned int memory[VECTOR2_SIZE][4];
	#pragma HLS ARRAY_PARTITION variable = memory
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas2 complete
	stats_t temp_stats[BLOCKRAM_UPDATEBLOCK_SIZE];
	unsigned int metadata_stats[VDATA_PACKINGSIZE];
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	unsigned int xblock_ids[BLOCKRAM_SIZE];
	workload_t xload_kvs[MAXNUM_EDGEBLOCKS_PER_VPARTITION]; // BLOCKRAM_SIZE];
	unsigned int vertexblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vertexblock_ids
	
	collection_t collections_tmp[COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections_tmp complete
	collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID]; // category 1
	collections_tmp[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID];
	collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections[REDUCEPHASE_TRANSFSZ_COLLECTIONID];
	collections_tmp[NUMEDGESPROCESSED_COLLECTIONID] = collections[NUMEDGESPROCESSED_COLLECTIONID]; // category 2
	collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID] = collections[NUMVERTICESPROCESSED_COLLECTIONID];
	collections_tmp[NUMREADSFROMDRAM_COLLECTIONID] = collections[NUMREADSFROMDRAM_COLLECTIONID]; // category 3
	collections_tmp[NUMWRITESTODRAM_COLLECTIONID] = collections[NUMWRITESTODRAM_COLLECTIONID];
	
	unsigned int num_vPs = globalparamsK.NUM_PROCESSEDGESPARTITIONS;
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	unsigned int upperlimit = sweepparams.source_partition * globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
	unsigned int depth = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; // 16 
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock; // 512		
	#ifdef _DEBUGMODE_CHECKS3
	if(globalparamsK.POW_BATCHRANGE < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)){ cout<<"actit:: ERROR 234. globalparamsK.POW_BATCHRANGE("<<globalparamsK.POW_BATCHRANGE<<") < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)("<<BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	if(globalparamsK.POW_BATCHRANGE < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)){ cout<<"actit:: ERROR 234. globalparamsK.POW_BATCHRANGE("<<globalparamsK.POW_BATCHRANGE<<") < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)("<<BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	#endif 
	
	if(mode == ACTSREDUCEMODE){	
		for(unsigned int t=0; t<NUM_ACTVVSPARTITIONS_PER_VPARTITION; t++){
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				actvvcounters[v][t] = 0; 
			}
		}
	}
	
	bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
	bool sparse_process = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = true; } else { sparse_process = false; }
	bool sparse_readv = false; if((stats[17][sweepparams.source_partition + 1] - stats[17][sweepparams.source_partition]) < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readv = true; } else { sparse_readv = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS//4
	if(mode == ACTSPROCESSMODE){ cout<<"actit: v_p: "<<sweepparams.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
	#endif 
	
	unsigned int totalnum_its = 0;
	bool en = true;
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && mode == ACTSPROCESSMODE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active

	if(globalparamsK.ENABLE_RUNKERNELCOMMAND == ON && en == true){
		#ifdef _DEBUGMODE_KERNELPRINTS//
		if(mode == ACTSPROCESSMODE){ cout<<"processit->actit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; }
		#endif 
	
		if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){
		
			MEMACCESSP0_get_vertexblock_ids(ON, globalposition.source_partition, kvdram, vbuffer, vertexblock_ids, stats, collections_tmp, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE);
			
			#ifdef ACTS_ENABLE_READUPROPS______________XXX
			unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, sweepparams.source_partition, kvdram, vbuffer, stats, globalposition.num_active_edgeblocks, globalparamsK, xload_kvs, buffer_offsets, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
			#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
			collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += ((num_its / VECTOR2_SIZE) + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
			collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += ((num_its / VECTOR2_SIZE) + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
			collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += ((num_its / VECTOR2_SIZE) + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
			#endif 
			if(sparse_readu == true){
				for(unsigned int n=0; n<num_its; n++){		
					MEMACCESSP0_readV(ON, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA, xload_kvs[n].offset_begin, xload_kvs[n].offset_begin, xload_kvs[n].size);
					#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
					collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
					collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
					collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
					#endif
				}
			} else {
				MEMACCESSP0_readV(ON, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA, 0, 0, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION);
				#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
				collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
				collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				#endif
			}
			#endif 
			
			#ifdef ACTS_ENABLE_READUPROPS//______________XXX
			for(unsigned int s=0; s<NUM_PEs; s++){
				unsigned int offset = s * VDATA_PACKING_PERCHANNEL;
				
				UTILP0_ReadDatas(kvdram, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offset, datas);
				unsigned int max = 0; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v]>max){ max = datas[v]; } }
				if(max > VDATA_PACKING_PERCHANNEL){ max = 0; }
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3 
				// if(max>0){ for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<">>> ACTIT-a:ACTVV BUFFER:: datas["<<v<<"]: "<<datas[v]<<", s: "<<s<<endl; }}
				if(max>0){ cout<<">>> ACTIT-a:ACTVV BUFFER:: max: "<<max<<endl; }
				#endif
				if(max > 0){
					for (buffer_type t=0; t<max; t++){
					#pragma HLS PIPELINE II=1
						UTILP0_ReadDatas(kvdram, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offset + 1 + t, datas2);
						
						for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
							value_t vdata = datas2[v] & 0xFFFF;
							unsigned int lvid = datas2[v] >> ACTVVATA_BITSZ;
							// unsigned int vid = sweepparams.source_partition
							// unsigned int index_offset = 0; // sweepparams.source_partition * NUM_ACTVVSPARTITIONS_PER_VPARTITION * VDATA_PACKING_PERCHANNEL;
							if(t<datas[v]){ vbuffer[v][lvid].data = (vdata << 1) | 0x1; } // combo & 0xFFFF
						}
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3 
						for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
							value_t lvid = datas2[v] >> ACTVVATA_BITSZ; value_t vdata = datas2[v] & 0xFFFF;
							cout<<">>> ACTIT:ACTVV-a:: ACTVV SEEN @: v: "<<v<<", lvid: "<<lvid<<", vdata: "<<vdata<<", num items: "<<datas[v]<<", s: "<<s<<endl; 
						}
						#endif
					}
					#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
					collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += max * VECTOR2_SIZE; 
					collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += max * VECTOR2_SIZE;
					collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += max * VECTOR2_SIZE; 
					#endif
				}
			}
			#endif 
			
			#ifdef ACTS_ENABLE_PROCESSEDGES
			unsigned int num_LLPset_; if(sparse_process == true){ num_LLPset_ = 1; } else { num_LLPset_ = num_LLPset; }
			ACTIT_MAINLOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset_; llp_set++){ 
				batch_type destoffset_kvs = (globalcapsule[1 + llp_set].key + globalcapsule[1 + llp_set].value) / EDGEDATA_PACKINGSIZE;
				unsigned int destindex = 0; 
				unsigned int num_its = MEMACCESSP0_get_edgeblock_workload(true, kvdram, sweepparams.source_partition, llp_set, vertexblock_ids, stats, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE, xload_kvs, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
				totalnum_its += num_its;
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("actit(1.2)", totalnum_its, globalparamsE.SIZE_VERTEXPTRS, NAp, NAp, num_its);
				#endif
			
				READ_PROCESS_PARTITION_STORE_LOOP1B: for(unsigned int n=0; n<num_its; n++){
					workload_t workload_kvs = xload_kvs[n];
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.size, (1 << 28) / 8, NAp, NAp, n);
					#endif
					workload_kvs.offset_srcbase = sourcebaseaddr_kvs + workload_kvs.offset_begin;
					workload_kvs.offset_dstbase = destbaseaddr_kvs + destoffset_kvs + destindex;
					workload_kvs.offset_buffer_begin = 0; // NAp;
				
					unsigned int activetransfsz_kvs = MEMACCESSP0_write__process_base(mode, llp_set, kvdram, vbuffer, stats, workload_kvs, collections_tmp, globalparamsK);
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"actit: n:"<<n<<", workload_kvs.size: "<<workload_kvs.size<<", activetransfsz_kvs: "<<activetransfsz_kvs<<endl;
					#endif 
						
					globalcapsule[1 + llp_set].value += workload_kvs.size * EDGEDATA_PACKINGSIZE;
					// globalcapsule[1 + llp_set].value += activetransfsz_kvs * EDGEDATA_PACKINGSIZE; // workload_kvs.size
					destindex += workload_kvs.size;	
					#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
					/// if(sparse_process == true){ collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += ((workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE) * 2) * REPORT__SPARSEPROCESS_SLOWDOWN_FACTOR; } // '* 2' because both read & write happens in processing phase
					/// else { collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE) * 2) }
					collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += ((workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE) * 2; 
					collections_tmp[NUMEDGESPROCESSED_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
					collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
					collections_tmp[NUMWRITESTODRAM_COLLECTIONID].data1 += (activetransfsz_kvs + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
					#endif 
				}
			}
			#endif 
		}
		
		#ifdef ACTS_ENABLE_REDUCE
		if(mode == ACTSREDUCEMODE){		
			workload_t workload_kvs;
			workload_kvs.offset_begin = globalcapsule[1 + sweepparams.source_partition].key / UPDATEDATA_PACKINGSIZE; 
			workload_kvs.size = globalcapsule[1 + sweepparams.source_partition].value / UPDATEDATA_PACKINGSIZE;
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			#ifdef _DEBUGMODE_CHECKS3
			if(workload_kvs.offset_end < workload_kvs.offset_begin){ cout<<"actit(reduce): ERROR 23: workload_kvs.offset_end("<<workload_kvs.offset_end<<") < workload_kvs.offset_begin("<<workload_kvs.offset_begin<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"actit(reduce): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][size_kvs: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"] ... "<<endl;					
			#endif

			unsigned int num_its = MEMACCESSP0_get_updateblock_workload(true, sweepparams.source_partition, stats[16], stats[17], globalparamsK, xload_kvs, buffer_offsets);
			unsigned int voffset_kvs = sweepparams.source_partition * globalparamsK.SIZEKVS2_REDUCEPARTITION;
			for(unsigned int n=0; n<num_its; n++){
				xload_kvs[n].offset_srcbase = NAp; 
				xload_kvs[n].offset_dstbase = globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA;
				xload_kvs[n].offset_buffer_begin = buffer_offsets[n];
				ACTIT_READVDATA_LOOP1: for (buffer_type i=0; i<xload_kvs[n].size; i++){
				#pragma HLS PIPELINE II=1
					UTILP0_ReadDatas(kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs + xload_kvs[n].offset_begin + i, datas);
					for(unsigned int v=0; v<VECTOR2_SIZE; v++){
					#pragma HLS UNROLL 
						vbuffer[v][xload_kvs[n].offset_buffer_begin + i].data = datas[v];
					}
				}
				#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
				collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
				collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				#endif 
			}
		
			workload_kvs.offset_srcbase = sourcebaseaddr_kvs;
			workload_kvs.offset_dstbase = NAp;
			workload_kvs.offset_buffer_begin = NAp;
			MEMACCESSP0_read__reduce(ACTSREDUCEMODE, kvdram, vbuffer, stats, workload_kvs, collections_tmp, globalparamsK);
			#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
			collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
			collections_tmp[NUMEDGESPROCESSED_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
			collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
			#endif 
			
			for(unsigned int n=0; n<num_its; n++){
				xload_kvs[n].offset_srcbase = NAp; 
				xload_kvs[n].offset_dstbase = globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA;
				xload_kvs[n].offset_buffer_begin = buffer_offsets[n];
				ACTIT_SAVEVDATA_LOOP2: for (buffer_type i=0; i<xload_kvs[n].size; i++){
				#pragma HLS PIPELINE II=1
					for(unsigned int v=0; v<VECTOR2_SIZE; v++){ // collect active frontiers (vertexID-based representation)
					#pragma HLS UNROLL 
						datas[v] = vbuffer[v][xload_kvs[n].offset_buffer_begin + i].data;
					}
					UTILP0_WriteDatas(kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs + xload_kvs[n].offset_begin + i, datas);
				}
				#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
				collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
				collections_tmp[NUMWRITESTODRAM_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				#endif 
			}
			
			#ifdef ACTS_ENABLE_COLLECTACTVVS
			// collect active frontiers (vertexID-based representation)
			unsigned int actvv_p = 0, num_actvv_ps = 0; 
			unsigned int actvvcounters_total[VECTOR2_SIZE]; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ actvvcounters_total[v] = 0; }
			for(unsigned int n=0; n<num_its; n++){
				xload_kvs[n].offset_buffer_begin = buffer_offsets[n];
				ACTIT_SAVEACTVV_LOOP2: for (buffer_type t=0; t<xload_kvs[n].size; t++){
				#pragma HLS PIPELINE II=1
					for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
					#pragma HLS UNROLL 
						unsigned int lvid = xload_kvs[n].offset_buffer_begin + t;
						value_t combo = vbuffer[v][lvid].data; value_t mask = combo & 0x1; value_t vdata = combo >> 1;
						lvid = lvid & 0x0000FFFF; vdata = vdata & 0x0000FFFF; // CRITICAL REMOVEME NOW.
						unsigned int index_offset = 0; // sweepparams.source_partition * NUM_ACTVVSPARTITIONS_PER_VPARTITION * VDATA_PACKING_PERCHANNEL;
						unsigned int actvv_p = lvid / VDATA_PACKING_PERCHANNEL; unsigned int base_ = actvv_p * VDATA_PACKING_PERCHANNEL;
						if(mask == 1){
							actvvbuffer[v][base_ + actvvcounters[v][actvv_p]].data = ((index_offset + lvid) << ACTVVATA_BITSZ) | vdata;	
							actvvcounters[v][actvv_p] += 1;
							actvvcounters_total[v] += 1; 
							#ifdef _DEBUGMODE_CHECKS3
							actsutilityobj->checkoutofbounds("_actit(1215)", actvv_p, globalparamsK.NUM_PROCESSEDGESPARTITIONS, NAp, NAp, NAp);
							actsutilityobj->checkoutofbounds("_actit(1216)", actvvcounters[v][actvv_p], globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, NAp, NAp, NAp);
							#endif
							#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3  // REMOVEME.
							cout<<">>> ACTIT:ACTVV BUFFER:: ACTVV SEEN @: v: "<<v<<", lvid: "<<index_offset + lvid<<", vdata: "<<vdata<<", mask: "<<mask<<", dstvid: "<<UTILP0_GETREALVID((index_offset + lvid), globalparamsK.ACTSPARAMS_INSTID)<<", new combo: "<<(((index_offset + lvid) << ACTVVATA_BITSZ) | vdata)<<endl; 
							#endif
						}
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("_actit(1217)", actvv_p, NUM_ACTVVSPARTITIONS_PER_VPARTITION, NAp, NAp, NAp);
						#endif
					}
				}
				
				// store actvvs
				for(unsigned int p_=0; p_<NUM_ACTVVSPARTITIONS_PER_VPARTITION; p_++){
					unsigned int offset = ((sweepparams.source_partition * NUM_ACTVVSPARTITIONS_PER_VPARTITION) + p_) * VDATA_PACKING_PERCHANNEL;
					
					for(unsigned int v=0; v<VECTOR2_SIZE; v++){ datas[v] = actvvcounters[v][p_]; }
					unsigned int max = 0; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(actvvcounters[v][p_]>max){ max = actvvcounters[v][p_]; } }

					UTILP0_WriteDatas(kvdram, globalparamsK.BASEOFFSETKVS_ACTIVEVERTICESDATA + offset, datas);
					for (buffer_type t=0; t<max; t++){
						for(unsigned int v=0; v<VECTOR2_SIZE; v++){
						#pragma HLS UNROLL 
							datas[v] = actvvbuffer[v][t].data;
						}
						UTILP0_WriteDatas(kvdram, globalparamsK.BASEOFFSETKVS_ACTIVEVERTICESDATA + offset + 1 + t, datas);
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3  // REMOVEME.
						for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
							value_t combo = datas[v]; value_t lvid = combo >> ACTVVATA_BITSZ; value_t vdata = combo & 0xFFFF;
							cout<<">>> ACTIT-c:ACTVV BUFFER:: ACTVV SEEN @: v: "<<v<<", lvid: "<<lvid<<", vdata: "<<vdata<<endl; 
						}
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("_actit(1217)", offset + t, globalparamsK.SIZE_BATCHRANGE, NAp, NAp, NAp);
						#endif
					}
				}
				
				#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
				collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
				collections_tmp[NUMWRITESTODRAM_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				#endif 
			}
			#endif 
		}
		#endif 
	}
	
	// prepare stats
	#ifdef ACTS_ENABLE_COLLECTSTATS
	if(mode == ACTSPROCESSMODE && UTILP0__processit__i_am_last__(globalposition) == true){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"-------------------------------- __processit__i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl;
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit:: numkvs_vertices_per_updateblock: "<<numkvs_vertices_per_updateblock<<", numkvs_updateblocks_per_reducepartition: "<<numkvs_updateblocks_per_reducepartition<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl;
		#endif 
		unsigned int stats_tmp[VECTOR2_SIZE]; 
		#pragma HLS ARRAY_PARTITION variable = stats_tmp complete
		for(int v = 0; v < VECTOR2_SIZE; v++){ stats_tmp[v] = 0; }
		unsigned int index = 0;
		for(int t = 0; t < BLOCKRAM_UPDATEBLOCK_SIZE; t++){
		#pragma HLS PIPELINE II=1
			unsigned int llp_set = t / numkvs_updateblocks_per_reducepartition;
			unsigned int ind = t;
			unsigned int stat = stats[0][ind] | stats[1][ind] | stats[2][ind] | stats[3][ind] | stats[4][ind] | stats[5][ind] | stats[6][ind] | 
				stats[7][ind] | stats[8][ind] | stats[9][ind] | stats[10][ind] | stats[11][ind] | stats[12][ind] | stats[13][ind] | stats[14][ind] | stats[15][ind];
			if(stat > 0){
				stats[16][index] = t; 
				// cout<<">>>>> actit: t: "<<t<<", llp_set: "<<llp_set<<", numkvs_vertices_per_updateblock: "<<numkvs_vertices_per_updateblock<<", numkvs_updateblocks_per_reducepartition: "<<numkvs_updateblocks_per_reducepartition<<endl; 
				// vbuffer[index % VECTOR2_SIZE][1 + (index / VECTOR2_SIZE)].data = t; 
				stats_tmp[llp_set] += 1; 
				index += 1; 
			}
		}
		// for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){ cout<<"--------------- actit: stats_tmp["<<llp_set<<"]: "<<stats_tmp[llp_set]<<endl; }
		stats[17][0] = 0; for(unsigned int i=1; i<VECTOR2_SIZE; i++){ stats[17][i] = stats[17][i-1] + stats_tmp[i-1];  } // vbuffer[i][0].data = stats[17][i];// calculate offsets
		// for(unsigned int llp_set=0; llp_set<num_LLPset+1; llp_set++){ cout<<"&&&&&&&&&&&&&&& actit: stats[17]["<<llp_set<<"]: "<<stats[17][llp_set]<<endl; }
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit:: stats_tmp["<<0<<"]: "<<stats_tmp[0]<<endl;
		for(unsigned int i=0; i<4; i++){ cout<<"actit:: stats[16]["<<i<<"]: "<<stats[16][i]<<", "<<endl; }
		for(unsigned int i=0; i<4; i++){ cout<<"actit:: stats[17]["<<i<<"]: "<<stats[17][i]<<", "<<endl; }
		#endif

		for(unsigned int t=0; t<BLOCKRAM_UPDATEBLOCK_SIZE; t++){
		#pragma HLS PIPELINE II=1	
			datas[0] = stats[16][t];
			datas[1] = stats[17][t];
			UTILP0_WriteDatas(kvdram, globalparamsK.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + t, datas);
		}
		// exit(EXIT_SUCCESS);
	}
	#endif
	
	collections[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID]; // category 1
	collections[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections_tmp[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID];
	collections[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID];
	collections[NUMEDGESPROCESSED_COLLECTIONID] = collections_tmp[NUMEDGESPROCESSED_COLLECTIONID]; // category 2
	collections[NUMVERTICESPROCESSED_COLLECTIONID] = collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID];	
	collections[NUMREADSFROMDRAM_COLLECTIONID] = collections_tmp[NUMREADSFROMDRAM_COLLECTIONID]; // category 3
	collections[NUMWRITESTODRAM_COLLECTIONID] = collections_tmp[NUMWRITESTODRAM_COLLECTIONID];
}

