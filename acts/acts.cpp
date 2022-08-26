// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Vitis/multiple_kernels/vadd_kernel/krnl_vadd.cpp
// https://github.com/Xilinx/SDAccel_Examples/blob/master/getting_started/dataflow/dataflow_stream_c/src/adder.cpp
// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Dataflow/Channels/using_fifos/diamond.cpp  *

// #define ACTS_ENABLE_COMBINEDLOADS
#define ACTS_ENABLE_BROADCASTUPROPS
#define ACTS_ENABLE_REDUCE
#define ACTS_ENABLE_COLLECTSTATS

void ACTSP0_combinedloads_uprops_and_allstats(bool en, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], 
		workload_t edgestats_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], collection_t collections_tmp[COLLECTIONS_BUFFERSZ], 
			sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsE){			
	#pragma HLS INLINE
	
	workload_t xload_kvs[BLOCKRAM_SIZE];
	workload_t upropload_kvs[BLOCKRAM_SIZE];
	unsigned int edgeblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = edgeblock_ids
	
	bool sparse_process = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = true; } else { sparse_process = false; }
	// if(en == false || globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = false; }
	unsigned int depth = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
	unsigned int num_vPs = globalparamsK.NUM_PROCESSEDGESPARTITIONS;
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	unsigned int num_its_ = 0;
	
	unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, sweepparams.source_partition, kvdram, vbuffer_source, stats, globalposition.num_active_edgeblocks, globalparamsK, upropload_kvs, buffer_offsets, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int c=0; c<3; c++){
		if(c==0){ num_its_ = 1; } 
		else if(c==1){ if(sparse_process==true){ num_its_ = globalposition.num_active_edgeblocks; } else { num_its_ = num_LLPset; }}
		else if(c==2){ num_its_ = num_its; } 
		
		for(int n=0; n<num_its_; n++){
			if(c == 0){
				// load edge-map
				xload_kvs[n] = MEMACCESSP0_get_edgeblock_offset(ON, globalposition.source_partition, vbuffer_source, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE); 
			} else if(c == 1){
				// load edge map / edge ptr offsets
				if(sparse_process == true){ 
					unsigned int edgeblock_id = edgeblock_ids[n % VECTOR2_SIZE][n / VECTOR2_SIZE];
					xload_kvs[n].offset_begin = 0; 
					xload_kvs[n].size = 2; 
					xload_kvs[n].offset_end = xload_kvs[n].offset_begin + xload_kvs[n].size;
					xload_kvs[n].offset_srcbase = globalparamsE.BASEOFFSETKVS_VERTEXPTR + (edgeblock_id / VECTOR2_SIZE);
					xload_kvs[n].offset_dstbase = NAp;
					xload_kvs[n].offset_buffer_begin = 0;
				} else {
					// do nothing
				}
			} else if(c == 2){
				// load source vertex properties
				xload_kvs[n] = upropload_kvs[n];
			}

			workload_t w_ = xload_kvs[n];
			unsigned int voffset_kvs = 0; unsigned int mode_ = READDATAMODE;
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(c==2){ mode_ = READUPROPERTYMODE; xload_kvs[n].offset_srcbase = globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs; xload_kvs[n].offset_buffer_begin = buffer_offsets[n] + s; } 
				else { mode_ = READDATAMODE; if(s>0){ w_.size = 0; } else { w_ = xload_kvs[n]; }}
				MEMACCESSP0_read__reduce(mode_, kvdram, vbuffer_source, stats, xload_kvs[n], collections_tmp, globalparamsK); 
				if(c==2){ voffset_kvs += depth; }
			}
			
			if(c == 0){
				for(unsigned int t=0; t<xload_kvs[n].size; t++){
				#pragma HLS PIPELINE II=1
					for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
					#pragma HLS UNROLL 
						edgeblock_ids[v][t] = vbuffer_source[v][t].data; 
					}
				}
			} else if(c == 1){
				if(sparse_process == true){ 
					unsigned int edgeblock_id = edgeblock_ids[n % VECTOR2_SIZE][n / VECTOR2_SIZE];
					edgestats_kvs[n].offset_begin = vbuffer_source[(edgeblock_id % VECTOR2_SIZE)][0].data / EDGEDATA_PACKINGSIZE; 
					if(edgeblock_id % VECTOR2_SIZE == VECTOR2_SIZE-1){ edgestats_kvs[n].offset_end = vbuffer_source[0][1].data / EDGEDATA_PACKINGSIZE; } else { edgestats_kvs[n].offset_end = vbuffer_source[(edgeblock_id % VECTOR2_SIZE) + 1][0].data / EDGEDATA_PACKINGSIZE; }
					edgestats_kvs[n].size = edgestats_kvs[n].offset_end - edgestats_kvs[n].offset_begin;
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("actit(53)", edgestats_kvs[n].size, ((1 << 28) / 4) / 16, edgestats_kvs[n].offset_begin, edgestats_kvs[n].offset_end, edgeblock_id);
					#endif
				} else {
					edgestats_kvs[n].offset_begin = edgeblock_ids[n][0] / EDGEDATA_PACKINGSIZE;
					edgestats_kvs[n].offset_end = edgeblock_ids[n + 1][0] / EDGEDATA_PACKINGSIZE;
					edgestats_kvs[n].size = edgestats_kvs[n].offset_end - edgestats_kvs[n].offset_begin;
				}
			} else  if(c == 2){
				// do nothing.
			}							
		}	
	}
}

void ACTSP0_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	workload_t edgestats_kvs[BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], 
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){

	// return; // REMOVEME...
	/* for(unsigned int t=0; t<64; t++){
		cout<<"~~~~~~ actit: globalcapsule["<<t<<"].key: "<<globalcapsule[t].key<<", globalcapsule["<<t<<"].value: "<<globalcapsule[t].value<<endl;
	}
	exit(EXIT_SUCCESS); */
	
	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory 
	keyvalue_vbuffer_t vbuffer_sourcebits[VDATA_PACKINGSIZE][BLOCKRAM_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = vbuffer_sourcebits
	value_t datas[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	stats_t temp_stats[BLOCKRAM_UPDATEBLOCK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = temp_stats
	unsigned int metadata_stats[VDATA_PACKINGSIZE];
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	unsigned int xblock_ids[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	// workload_t edgestats_kvs[BLOCKRAM_SIZE];
	#ifndef ACTS_ENABLE_COMBINEDLOADS
	unsigned int edgeblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = edgeblock_ids
	#endif 
	
	collection_t collections_tmp[COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable=collections_tmp complete
	collections_tmp[TRAVERSEDEDGES_COLLECTIONID] = collections[TRAVERSEDEDGES_COLLECTIONID];
	collections_tmp[PROCESSEDGES_COLLECTIONID] = collections[PROCESSEDGES_COLLECTIONID];
	collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections[ACTIVEEDGESPROCESSED_COLLECTIONID];
	collections_tmp[REDUCEUPDATES_COLLECTIONID] = collections[REDUCEUPDATES_COLLECTIONID];
	
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
	
	bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
	bool sparse_process = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = true; } else { sparse_process = false; }
	bool sparse_readv = false; if((stats[17][sweepparams.source_partition + 1] - stats[17][sweepparams.source_partition]) < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readv = true; } else { sparse_readv = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mode == ACTSPROCESSMODE){ cout<<"actit: v_p: "<<sweepparams.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
	#endif 
	
	bool en = true;
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && mode == ACTSPROCESSMODE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active

	if(globalparamsK.ENABLE_RUNKERNELCOMMAND == ON && en == true){
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(mode == ACTSPROCESSMODE){ cout<<"processit->actit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; }
		#endif 
	
		if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){
		
			#ifdef ACTS_ENABLE_BROADCASTUPROPS
			#ifdef ACTS_ENABLE_COMBINEDLOADS
				ACTSP0_combinedloads_uprops_and_allstats(true, kvdram, vbuffer_source, 
					edgestats_kvs, buffer_offsets, stats, collections_tmp, 
						sweepparams, globalposition, globalparamsK, globalparamsE);
			#else 
				MEMACCESSP0_get_edgeblock_ids__or__offsets(ON, globalposition.source_partition, kvdram, vbuffer_dest, edgeblock_ids, stats, collections_tmp, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE);
				
				unsigned int num_its = MEMACCESSP0_get_upropblock_workload(true, sweepparams.source_partition, kvdram, vbuffer_dest, stats, globalposition.num_active_edgeblocks, globalparamsK, xload_kvs, buffer_offsets, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
				for(unsigned int n=0; n<num_its; n++){	
					unsigned int voffset_kvs = 0;
					for(unsigned int s=0; s<NUM_PEs; s++){	
						xload_kvs[n].offset_srcbase = globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs;
						xload_kvs[n].offset_dstbase = NAp;
						xload_kvs[n].offset_buffer_begin = buffer_offsets[n] + s;
						MEMACCESSP0_read__reduce(READUPROPERTYMODE, kvdram, vbuffer_source, stats, xload_kvs[n], collections_tmp, globalparamsK);
						voffset_kvs += depth;
					}	
				}
			#endif 
			#endif 
			
			ACTIT_MAINLOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){ 
			// ACTIT_MAINLOOP1: for(unsigned int llp_set=0; llp_set<1; llp_set++){ // CRITICAL FIXME NOW.
				batch_type destoffset_kvs = (globalcapsule[1 + llp_set].key + globalcapsule[1 + llp_set].value) / EDGEDATA_PACKINGSIZE;
				unsigned int destindex = 0; 
				workload_t last_workload;
				#ifdef ACTS_ENABLE_COMBINEDLOADS
				unsigned int num_its; if(sparse_process == true){ num_its = globalposition.num_active_edgeblocks; } else { num_its = 1; }
				#else 
				unsigned int num_its = MEMACCESSP0_get_edgeblock_workload(true, kvdram, sweepparams.source_partition, llp_set, edgeblock_ids, stats, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE, xload_kvs, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
				#endif 
				
				READ_PROCESS_PARTITION_STORE_LOOP1B: for(unsigned int n=0; n<num_its; n++){
				// READ_PROCESS_PARTITION_STORE_LOOP1B: for(unsigned int n=0; n<1; n++){ // CRITICAL FIXME NOW.
					#ifdef ACTS_ENABLE_COMBINEDLOADS
					workload_t workload_kvs = edgestats_kvs[n];
					#else 
					workload_t workload_kvs = xload_kvs[n];
					#endif 
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.size, (1 << 28) / 8, NAp, NAp, n);
					#endif
					workload_kvs.offset_srcbase = sourcebaseaddr_kvs + workload_kvs.offset_begin;
					workload_kvs.offset_dstbase = destbaseaddr_kvs + destoffset_kvs + destindex;
					workload_kvs.offset_buffer_begin = 0; // NAp;
				
					MEMACCESSP0_write__process_base(mode, llp_set, kvdram, vbuffer_source, stats, workload_kvs, collections_tmp, globalparamsK);
					
					globalcapsule[1 + llp_set].value += workload_kvs.size * EDGEDATA_PACKINGSIZE;	
					destindex += workload_kvs.size;
				}
			}
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
			for(unsigned int n=0; n<num_its; n++){
			// for(unsigned int n=0; n<1; n++){ // CRITICAL FIXME NOW.
				xload_kvs[n].offset_srcbase = globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA;
				xload_kvs[n].offset_dstbase = NAp;
				xload_kvs[n].offset_buffer_begin = buffer_offsets[n];
				MEMACCESSP0_read__reduce(READVPROPERTYMODE, kvdram, vbuffer_dest, stats, xload_kvs[n], collections_tmp, globalparamsK);
			}
		
			workload_kvs.offset_srcbase = sourcebaseaddr_kvs;
			workload_kvs.offset_dstbase = NAp;
			workload_kvs.offset_buffer_begin = NAp;
			MEMACCESSP0_read__reduce(ACTSREDUCEMODE, kvdram, vbuffer_dest, stats, workload_kvs, collections_tmp, globalparamsK);

			for(unsigned int n=0; n<num_its; n++){
			// for(unsigned int n=0; n<1; n++){ // CRITICAL FIXME NOW.
				xload_kvs[n].offset_srcbase = sourcebaseaddr_kvs + xload_kvs[n].offset_begin;
				xload_kvs[n].offset_dstbase = globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + xload_kvs[n].offset_begin, 0;
				xload_kvs[n].offset_buffer_begin = buffer_offsets[n];
				MEMACCESSP0_write__process_base(SAVEVPROPERTYMODE, NAp, kvdram, vbuffer_dest, stats, xload_kvs[n], collections_tmp, globalparamsK);
			}
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
			if(stat > 0){ stats[16][index] = t; }
			// if(stat > 0){ vbuffer_dest[index % VECTOR2_SIZE][1 + (index / VECTOR2_SIZE)].data = t; }
			if(stat > 0){ stats_tmp[llp_set] += 1; }
			if(stat > 0){ index += 1; }
		}
		stats[17][0] = 0; for(unsigned int i=1; i<VECTOR2_SIZE; i++){ stats[17][i] = stats[17][i-1] + stats_tmp[i-1];  } // vbuffer_dest[i][0].data = stats[17][i];// calculate offsets
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
	}
	collections[TRAVERSEDEDGES_COLLECTIONID] = collections_tmp[TRAVERSEDEDGES_COLLECTIONID];
	collections[PROCESSEDGES_COLLECTIONID] = collections_tmp[PROCESSEDGES_COLLECTIONID];
	collections[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID];
	collections[REDUCEUPDATES_COLLECTIONID] = collections_tmp[REDUCEUPDATES_COLLECTIONID];
	// exit(EXIT_SUCCESS);
	#endif 
}

