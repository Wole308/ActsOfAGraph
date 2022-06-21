// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Vitis/multiple_kernels/vadd_kernel/krnl_vadd.cpp
// https://github.com/Xilinx/SDAccel_Examples/blob/master/getting_started/dataflow/dataflow_stream_c/src/adder.cpp
// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Dataflow/Channels/using_fifos/diamond.cpp  *

#ifdef FPGA_IMPL
void load(uint512_dt *in, hls::stream<uint512_vec_dt >& out, batch_type sourcebaseaddr_kvs, unsigned int offset_kvs, int size){
	Loop_Ld: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt data = UTILP0_GetDataset(in, sourcebaseaddr_kvs + offset_kvs + i);
		out.write(data);
	}
}

void compute_and_store(hls::stream<uint512_vec_dt >& in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
					batch_type destbaseaddr_kvs, unsigned int offset_kvs, int size, unsigned int upperlimit, 
						unsigned int collections_regs[2], globalparams_t globalparamsK, globalposition_t globalposition){
	uint512_vec_dt res;
	#pragma HLS ARRAY_PARTITION variable=res complete
	
	Loop_A: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt data = in.read();
		
		// set flag
		bool enx = true;
		if((globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE) && (collections_regs[0] + VECTOR_SIZE >= globalposition.num_active_edges_in_channel)){ enx = false; } // for BFS implementation
		else { collections_regs[1] += VECTOR_SIZE; }
		
		// process
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS UNROLL
			res.data[v] = PROCESSP0_processvector(enx, v, data.data[v].value - upperlimit, upperlimit, data.data[v], vbuffer_source[v], globalparamsK);
		}
		
		// store
		UTILP0_SetDataset(out, destbaseaddr_kvs + offset_kvs + i, res);
	}
}
#endif 

void ACTSP0_load_compute_and_store(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
			batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, int size, unsigned int upperlimit, 
						unsigned int collections_regs[2], globalparams_t globalparamsK, globalposition_t globalposition){
	uint512_vec_dt res;
	for(int i = 0; i < size; i++){
		// load 
		uint512_vec_dt data = UTILP0_GetDataset(in, sourcebaseaddr_kvs + srcoffset_kvs + i);

		// set flag
		bool enx = true;
		if((globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE) && (collections_regs[0] + VECTOR_SIZE >= globalposition.num_active_edges_in_channel)){ enx = false; } // for BFS implementation
		else { collections_regs[1] += VECTOR_SIZE; }
		
		// process
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS UNROLL
			res.data[v] = PROCESSP0_processvector(enx, v, data.data[v].value - upperlimit, upperlimit, data.data[v], vbuffer_source[v], globalparamsK);
		}
		
		// store
		UTILP0_SetDataset(out, destbaseaddr_kvs + destoffset_kvs + i, res);
	}
}

void ACTSP0_actit3_load_compute_and_store(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	
			unsigned int upperlimit, unsigned int sourcebaseaddr_kvs, unsigned int srcoffset_kvs, unsigned int destbaseaddr_kvs, unsigned int destoffset_kvs, unsigned int size_kvs,
				unsigned int collections_regs[2], globalparams_t globalparamsK, globalposition_t globalposition){

	#ifdef FPGA_IMPL
	hls::stream<uint512_vec_dt> c0; //, c1, c2, c3, c4, c5;
	#pragma HLS STREAM variable = c0 depth = 512

	#pragma HLS dataflow
	load(kvdram, c0, sourcebaseaddr_kvs, srcoffset_kvs, size_kvs);
	compute_and_store(c0, kvdram, vbuffer_source,
					destbaseaddr_kvs, destoffset_kvs, size_kvs, upperlimit, 
						collections_regs, globalparamsK, globalposition);
						
	#else 
	ACTSP0_load_compute_and_store(kvdram, kvdram, vbuffer_source,
		sourcebaseaddr_kvs, srcoffset_kvs, destbaseaddr_kvs, destoffset_kvs, size_kvs, upperlimit, 
					collections_regs, globalparamsK, globalposition);
	#endif
}

void ACTSP0_actit3(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){

	unsigned int memory[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=memory complete
	workload_t workloads_kvs[BLOCKRAM_SIZE];
	
	unsigned int collections_regs[2];
	#pragma HLS ARRAY_PARTITION variable=collections_regs complete
	collections_regs[0] = collections[0][2].data1;
	collections_regs[1] = collections[0][2].data1;
	
	unsigned int num_vPs = globalparamsK.NUM_PROCESSEDGESPARTITIONS;
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; // 1 << (OPT_NUM_PARTITIONS_POW * globalparamsK.ACTSPARAMS_TREEDEPTH);
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; // globalparamsK.ACTSPARAMS_NUM_PARTITIONS    NUM_PARTITIONS;
	unsigned int upperlimit = sweepparams.source_partition * globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
	bool enx = true;
	unsigned int _EN_PROCESS_PARTITION_SAVE = OFF; 
	unsigned int _EN_PROCESS_PARTITION_REDUCE = OFF;
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ _EN_PROCESS_PARTITION_SAVE = OFF; _EN_PROCESS_PARTITION_REDUCE = ON; }
	else { if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){ _EN_PROCESS_PARTITION_SAVE = ON; } else if(mode == ACTSREDUCEMODE){ _EN_PROCESS_PARTITION_REDUCE = ON; }}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit3:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", num_vPs: "<<num_vPs<<"] "<<endl;
	#endif 
	
	ACTIT3_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
		workload_t workload_kvs;
		workload_kvs.offset_begin = MEMACCESSP0_getdata(kvdram, globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK, (sweepparams.source_partition * num_LLPset) + llp_set) / VECTOR_SIZE;
		workload_kvs.offset_end = MEMACCESSP0_getdata(kvdram, globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK, (sweepparams.source_partition * num_LLPset) + llp_set + 1) / VECTOR_SIZE;
		workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
		if(workload_kvs.offset_end < workload_kvs.offset_begin){ workload_kvs.size = 0; }
		#ifdef _DEBUGMODE_CHECKS3
		if(workload_kvs.offset_end < workload_kvs.offset_begin){ cout<<"actit3(process): ERROR 23: workload_kvs.offset_end("<<workload_kvs.offset_end<<") < workload_kvs.offset_begin("<<workload_kvs.offset_begin<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
		#endif		
		
		workloads_kvs[llp_set] = workload_kvs;
	}
	
	if(_EN_PROCESS_PARTITION_SAVE == ON && globalparamsK.ENABLE_PROCESSCOMMAND == ON){
		ACTIT3_MAINLOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
			workload_t workload_kvs = workloads_kvs[llp_set];
			batch_type destoffset_kvs = (globalcapsule[llp_set].key + globalcapsule[llp_set].value) / VECTOR_SIZE;
			
			#ifdef _DEBUGMODE_KERNELPRINTS
			unsigned int num_chunks = workload_kvs.size / BLOCKRAM_SIZE;
			cout<<"actit3(process): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<nextoffset_kvs<<"][size_kvs: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * VECTOR_SIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<num_chunks<<"] ... "<<endl;					
			#endif
			
			ACTSP0_actit3_load_compute_and_store(kvdram, vbuffer_source, vbuffer_dest,	
				upperlimit, sourcebaseaddr_kvs, workload_kvs.offset_begin, destbaseaddr_kvs, destoffset_kvs, workload_kvs.size,
					collections_regs, globalparamsK, globalposition);
			
			globalcapsule[llp_set].value += workload_kvs.size * VECTOR_SIZE; // CRITICAL FIXME. let globalcapsule load all partitions of LLOP-1
			#ifdef _DEBUGMODE_STATS
			if(mode==ACTSPROCESSMODE && enx == true){
				actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, workload_kvs.size * VECTOR_SIZE);
				actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, workload_kvs.size * VECTOR_SIZE); }
			#endif 
		}
		collections[0][2].data1 = collections_regs[1];
	}
	
	if(_EN_PROCESS_PARTITION_REDUCE == ON && globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON){
		unsigned int curroffsetkvs = 0, nextoffsetkvs = 0;
		if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
			curroffsetkvs = MEMACCESSP0_getdata(kvdram, globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK, sweepparams.source_partition) / VECTOR_SIZE;
			nextoffsetkvs = MEMACCESSP0_getdata(kvdram, globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK, sweepparams.source_partition + 1) / VECTOR_SIZE;
		} else { curroffsetkvs = ptravstate.begin_kvs; nextoffsetkvs = ptravstate.end_kvs; }
		unsigned int size_kvs = nextoffsetkvs - curroffsetkvs;
		#ifdef _DEBUGMODE_CHECKS3
		if(nextoffsetkvs < curroffsetkvs){ cout<<"actit3(reduce): ERROR 23: nextoffsetkvs("<<nextoffsetkvs<<") < curroffsetkvs("<<curroffsetkvs<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actit3(reduce): processing all chunks [begin_kvs: "<<curroffsetkvs<<"][end_kvs: "<<nextoffsetkvs<<"][size_kvs: "<<size_kvs<<"][size: "<<size_kvs * VECTOR_SIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<(nextoffsetkvs - curroffsetkvs) / globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"] ... "<<endl;					
		#endif
		// exit(EXIT_SUCCESS);
		ACTIT3_MAINLOOP2A: for(batch_type offset_kvs=curroffsetkvs; offset_kvs<curroffsetkvs + size_kvs; offset_kvs++){
		#pragma HLS PIPELINE II=1
		#pragma HLS dependence variable=vbuffer_dest inter false
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"actit3(reduce): processing chunk [offset_kvs: "<<offset_kvs<<"]: [curroffsetkvs: "<<curroffsetkvs<<"]: [nextoffsetkvs: "<<nextoffsetkvs<<"] ... "<<endl;
			#endif
			
			// get dataset
			uint512_vec_dt data = UTILP0_GetDataset(kvdram, sourcebaseaddr_kvs + offset_kvs);
			
			// reduce 
			bool enx = true;
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP0_reducevector(enx, 0, UTILP0_GETKV(data.data[0]), vbuffer_dest[0 + 0], 0, sweepparams.upperlimit, &memory[0], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP0_reducevector(enx, 1, UTILP0_GETKV(data.data[1]), vbuffer_dest[0 + 1], 0, sweepparams.upperlimit, &memory[1], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP0_reducevector(enx, 2, UTILP0_GETKV(data.data[2]), vbuffer_dest[0 + 2], 0, sweepparams.upperlimit, &memory[2], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP0_reducevector(enx, 3, UTILP0_GETKV(data.data[3]), vbuffer_dest[0 + 3], 0, sweepparams.upperlimit, &memory[3], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP0_reducevector(enx, 4, UTILP0_GETKV(data.data[4]), vbuffer_dest[0 + 4], 0, sweepparams.upperlimit, &memory[4], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP0_reducevector(enx, 5, UTILP0_GETKV(data.data[5]), vbuffer_dest[0 + 5], 0, sweepparams.upperlimit, &memory[5], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP0_reducevector(enx, 6, UTILP0_GETKV(data.data[6]), vbuffer_dest[0 + 6], 0, sweepparams.upperlimit, &memory[6], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP0_reducevector(enx, 7, UTILP0_GETKV(data.data[7]), vbuffer_dest[0 + 7], 0, sweepparams.upperlimit, &memory[7], sweepparams, globalparamsK);
			
			
			#ifdef _DEBUGMODE_STATS
			if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && mode==ACTSPROCESSMODE && enx == true){
				actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, VECTOR_SIZE);
				actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, VECTOR_SIZE); }
			#endif 
		}
	}
}