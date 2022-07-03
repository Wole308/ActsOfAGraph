// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Vitis/multiple_kernels/vadd_kernel/krnl_vadd.cpp
// https://github.com/Xilinx/SDAccel_Examples/blob/master/getting_started/dataflow/dataflow_stream_c/src/adder.cpp
// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Dataflow/Channels/using_fifos/diamond.cpp  *

keyvalue_t acts_all::process_edge(bool enx, unsigned int v, unsigned int loc, keyvalue_t edge_data, keyvalue_vbuffer_t * vbufferA, keyvalue_vbuffer_t * vbufferB, globalparams_t globalparams){				
	#pragma HLS INLINE
	
	// flag 
	bool en = true; if(edge_data.key == INVALIDDATA || edge_data.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	// check loc 
	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){ loc = 0; }
	#ifdef _DEBUGMODE_CHECKS
	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && edge_data.key != INVALIDDATA){ cout<<"PROCESSP0_processvector::ERROR SEEN @@ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<". EXITING... "<<endl; exit(EXIT_FAILURE); }					
	#endif 
		
	// read vertex data 
	keyvalue_vbuffer_t bits_vector = vbufferA[loc / VDATA_SHRINK_RATIO];
	
	// read vertex mask
	vmdata_t vmdata; if(en == true){ vmdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
	if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edge_data.key: "<<edge_data.key<<", edge_data.value(srcvid): "<<edge_data.value<<", en: "<<en<<", vid: "<<UTILP0_GETREALVID(edge_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
			
	// process
	value_t res = process_func(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	keyvalue_t vupdate; vupdate.key = edge_data.key; vupdate.value = res;
	return vupdate;
}

void acts_all::reduce_update(bool enx, unsigned int loc, keyvalue_t update_data, keyvalue_vbuffer_t vbufferA[MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbufferB[MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int memory[1], globalparams_t globalparams){
	#pragma HLS INLINE
	
	unsigned int which = loc % 2; // FIXME
	
	// flag 
	bool en = true;
	if(update_data.key == UTILP0_GETK(INVALIDDATA) || update_data.value == UTILP0_GETV(INVALIDDATA) || update_data.key == MAXVDATA || update_data.value == MAXVDATA){ en = false; }
	
	// check loc 
	if(loc >= (globalparams.SIZEKVS2_REDUCEPARTITION / 2) && en == true){ loc = 0; } // loc = loc / 2; // urams pack in 2s
	#ifdef ENABLE_PERFECTACCURACY
	if(true){ cout<<"REDUCEP0reduce_update::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). update_data.key: "<<update_data.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	
	// 
	if(en == true){ if(loc == memory[0]){ loc = (loc + 1) % 8; } memory[0] = loc; } // CRITICAL FIXME.
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 113::1", loc, MAX_BLOCKRAM_VDESTDATA_SIZE, NAp, loc, VDATA_SHRINK_RATIO); }
	#endif
	
	// read vertex data 
	keyvalue_vbuffer_t bits_vectorA = 0; keyvalue_vbuffer_t bits_vectorB = 0;  vmdata_t tmpvdata; tmpvdata.vmask = 0; 
	if(en == true){ bits_vectorA = vbufferA[globalparams.BUFFERBASEOFFSETKVS_VDATA1 + (loc / VDATA_SHRINK_RATIO)]; }
	if(en == true){ bits_vectorB = vbufferB[globalparams.BUFFERBASEOFFSETKVS_VDATA1 + (loc / VDATA_SHRINK_RATIO)]; }
	
	keyvalue_vbuffer_t bits_vector = 0; 
	if(which == 0){ bits_vector = bits_vectorA; } else { bits_vector = bits_vectorB; }
	
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true){ tmpvdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK2(loc, bits_vector); }
			#else 
				if(en == true){ tmpvdata = MEMCAP0_READFROMBUFFER_VDATAWITHVMASK(bits_vector); }
					#endif
	
	// reduce 
	value_t new_vprop = reduce_func(tmpvdata.vdata, tmpvdata.vdata, update_data.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	if(en == true && new_vprop != tmpvdata.vdata){ tmpvdata.vmask = 1; } else { tmpvdata.vmask = 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true && new_vprop != tmpvdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE MASK SEEN AT: loc: "<<loc<<", tmpvdata.vdata: "<<tmpvdata.vdata<<", tmpvdata.vmask: "<<tmpvdata.vmask<<endl; }
	if(en == true && new_vprop != tmpvdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP0_GETREALVID(update_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
	
	// write-back vertx data
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true && which == 0){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK2(loc, vbufferA, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, tmpvdata.vmask); }
		if(en == true && which == 1){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK2(loc, vbufferB, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, tmpvdata.vmask); }
			#else 
				if(en == true && which == 0){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(loc, vbufferA, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, tmpvdata.vmask); }
				if(en == true && which == 1){ MEMCAP0_WRITETOBUFFER_VDATAWITHVMASK(loc, vbufferB, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, tmpvdata.vmask); }
					#endif
	return;
}	

#ifdef FPGA_IMPL
void load(uint512_dt *in, hls::stream<uint512_evec_dt >& out, batch_type sourcebaseaddr_kvs, unsigned int offset_kvs, int size){
	Loop_Ld: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		uint512_evec_dt data = UTILP0_ReadEdges(in, sourcebaseaddr_kvs + offset_kvs + i);
		out.write(data);
	}
}

void compute_and_store(hls::stream<uint512_evec_dt >& in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
					batch_type destbaseaddr_kvs, unsigned int offset_kvs, int size, unsigned int upperlimit, 
						collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	uint512_uvec_dt res;
	
	Loop_A: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		uint512_evec_dt data = in.read();
		
		// set flag
		bool enx = true;
		collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
		collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE;  // *** used in PR, CF implementations ONLY (not BFS) ***
		
		// process
	
		res.data[0] = process_edge(enx, 0, data.data[0].value - upperlimit, data.data[0], vbuffer_source[0], vbuffer_source[1], globalparamsK);
	
		res.data[1] = process_edge(enx, 1, data.data[1].value - upperlimit, data.data[1], vbuffer_source[2], vbuffer_source[3], globalparamsK);
	
		res.data[2] = process_edge(enx, 2, data.data[2].value - upperlimit, data.data[2], vbuffer_source[4], vbuffer_source[5], globalparamsK);
	
		res.data[3] = process_edge(enx, 3, data.data[3].value - upperlimit, data.data[3], vbuffer_source[6], vbuffer_source[7], globalparamsK);
	
		res.data[4] = process_edge(enx, 4, data.data[4].value - upperlimit, data.data[4], vbuffer_source[8], vbuffer_source[9], globalparamsK);
	
		res.data[5] = process_edge(enx, 5, data.data[5].value - upperlimit, data.data[5], vbuffer_source[10], vbuffer_source[11], globalparamsK);
	
		res.data[6] = process_edge(enx, 6, data.data[6].value - upperlimit, data.data[6], vbuffer_source[12], vbuffer_source[13], globalparamsK);
	
		res.data[7] = process_edge(enx, 7, data.data[7].value - upperlimit, data.data[7], vbuffer_source[14], vbuffer_source[15], globalparamsK);
		
		// store
		UTILP0_WriteDataset(out, destbaseaddr_kvs + offset_kvs + i, res);
	}
}
#endif 

void acts_all::ACTSP0_read_process_partition_and_write(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
			batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, int size, unsigned int upperlimit, 
				collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	keyvalue_t res[UPDATEDATA_PACKINGSIZE]; // uint512_uvec_dt
	#pragma HLS ARRAY_PARTITION variable = res complete
	
	for(int i = 0; i < size; i++){
		// load 
		uint512_evec_dt data = UTILP0_ReadEdges(in, sourcebaseaddr_kvs + srcoffset_kvs + i);
	
		// set flag
		bool enx = true;
		collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
		collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE; // *** used in PR, CF implementations ONLY (not BFS) ***
		
		// process
	
		res[0] = process_edge(enx, 0, data.data[0].value - upperlimit, data.data[0], vbuffer_source[0], vbuffer_source[1], globalparamsK);
	
		res[1] = process_edge(enx, 1, data.data[1].value - upperlimit, data.data[1], vbuffer_source[2], vbuffer_source[3], globalparamsK);
	
		res[2] = process_edge(enx, 2, data.data[2].value - upperlimit, data.data[2], vbuffer_source[4], vbuffer_source[5], globalparamsK);
	
		res[3] = process_edge(enx, 3, data.data[3].value - upperlimit, data.data[3], vbuffer_source[6], vbuffer_source[7], globalparamsK);
	
		res[4] = process_edge(enx, 4, data.data[4].value - upperlimit, data.data[4], vbuffer_source[8], vbuffer_source[9], globalparamsK);
	
		res[5] = process_edge(enx, 5, data.data[5].value - upperlimit, data.data[5], vbuffer_source[10], vbuffer_source[11], globalparamsK);
	
		res[6] = process_edge(enx, 6, data.data[6].value - upperlimit, data.data[6], vbuffer_source[12], vbuffer_source[13], globalparamsK);
	
		res[7] = process_edge(enx, 7, data.data[7].value - upperlimit, data.data[7], vbuffer_source[14], vbuffer_source[15], globalparamsK);
		
		// store
		UTILP0_WriteDatas(out, destbaseaddr_kvs + destoffset_kvs + i, res);
		
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(enx == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE); }
		#endif 
	}
}

void acts_all::ACTSP0_read_process_partition_and_write_base(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],	
			unsigned int upperlimit, unsigned int sourcebaseaddr_kvs, workload_t workload_kvs, unsigned int destbaseaddr_kvs, unsigned int destoffset_kvs,
				collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){

	#ifdef FPGA_IMPL
	hls::stream<uint512_evec_dt> c0; //, c1, c2, c3, c4, c5;
	#pragma HLS STREAM variable = c0 depth = 512

	#pragma HLS dataflow
	load(kvdram, c0, sourcebaseaddr_kvs, workload_kvs.offset_begin, workload_kvs.size);
	compute_and_store(c0, kvdram, vbuffer_source,
					destbaseaddr_kvs, destoffset_kvs, workload_kvs.size, upperlimit, 
						collections, globalparamsK, globalposition);
						
	#else 
	ACTSP0_read_process_partition_and_write(kvdram, kvdram, vbuffer_source,
		sourcebaseaddr_kvs, workload_kvs.offset_begin, destbaseaddr_kvs, destoffset_kvs, workload_kvs.size, upperlimit, 
					collections, globalparamsK, globalposition);
	#endif
}

void acts_all::ACTSP0_read_and_reduce(unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	
			sweepparams_t sweepparams, unsigned int sourcebaseaddr_kvs, workload_t workload_kvs, 
				collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	
	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory 
	keyvalue_t datas[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	
	unsigned int tmp = collections[ACTIVEEDGESPROCESSED_COLLECTIONID].data1;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit3(reduce): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][workload_kvs.size: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<(workload_kvs.offset_end - workload_kvs.offset_begin) / globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"] ... "<<endl;					
	#endif
	ACTIT3_MAINLOOP2A: for(batch_type offset_kvs=workload_kvs.offset_begin; offset_kvs<workload_kvs.offset_begin + workload_kvs.size; offset_kvs++){
	#pragma HLS PIPELINE II=1
	#pragma HLS dependence variable=vbuffer_dest inter false
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actit3(reduce): processing chunk [offset_kvs: "<<offset_kvs<<"]: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<"]: [workload_kvs.offset_end: "<<workload_kvs.offset_end<<"] ... "<<endl;
		#endif
		
		// get dataset
		UTILP0_ReadKeyvalues(kvdram, sourcebaseaddr_kvs + offset_kvs, datas);
		
		// set flag
		bool enx = true;
		if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){  // used in BFS implementations ONLY (not PR or CF)
			collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
			collections[PROCESSEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
			if(tmp + ((offset_kvs - workload_kvs.offset_begin) * UPDATEDATA_PACKINGSIZE) >= globalposition.num_active_edges_in_channel){ enx = false; } 
			else { enx = true; collections[ACTIVEEDGESPROCESSED_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE; }
		}
		
		// reduce
	
		reduce_update(enx, datas[0].key, datas[0], vbuffer_dest[0], vbuffer_dest[1], memory[0], globalparamsK);
	
		reduce_update(enx, datas[1].key, datas[1], vbuffer_dest[2], vbuffer_dest[3], memory[1], globalparamsK);
	
		reduce_update(enx, datas[2].key, datas[2], vbuffer_dest[4], vbuffer_dest[5], memory[2], globalparamsK);
	
		reduce_update(enx, datas[3].key, datas[3], vbuffer_dest[6], vbuffer_dest[7], memory[3], globalparamsK);
	
		reduce_update(enx, datas[4].key, datas[4], vbuffer_dest[8], vbuffer_dest[9], memory[4], globalparamsK);
	
		reduce_update(enx, datas[5].key, datas[5], vbuffer_dest[10], vbuffer_dest[11], memory[5], globalparamsK);
	
		reduce_update(enx, datas[6].key, datas[6], vbuffer_dest[12], vbuffer_dest[13], memory[6], globalparamsK);
	
		reduce_update(enx, datas[7].key, datas[7], vbuffer_dest[14], vbuffer_dest[15], memory[7], globalparamsK);
		
		if(enx == true){ collections[REDUCEUPDATES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE; }
	}
}

void acts_all::ACTSP0_read_process_and_reduce(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_sourcebits[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
				collection_t collections[COLLECTIONS_BUFFERSZ], sweepparams_t sweepparams, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition){
	
	keyvalue_t res[EDGEDATA_PACKINGSIZE]; // uint512_uvec_dt
	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory 
	keyvalue_t datas[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	unsigned int actvvs[DOUBLE_BLOCKRAM_SIZE];//BLOCKRAM_SIZE];
	workload_t workloads_kvs[DOUBLE_BLOCKRAM_SIZE];
	
	unsigned int num_actvvs = globalposition.num_active_vertices;
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> actit3:: [iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", v_partition: "<<sweepparams.source_partition<<", num_actvvs: "<<num_actvvs<<"] "<<endl;
	#endif 
	
	for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
		// get workload stats 
		unsigned int num_its; 
		if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ num_its = num_actvvs; } else { num_its = 1; }
		
		for(unsigned int n=0; n<num_its; n++){
			workload_t workload_kvs;
			if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){
				unsigned int loc = n;//n + 5; // REMOVEME. // actvvs[n]
				unsigned int index = (sweepparams.source_partition * MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION) + (2 * loc);
				workload_kvs.offset_begin = MEMACCESSP0_getdata(in, globalparamsE.BASEOFFSETKVS_ACTIVEEDGESMAP, index);
				workload_kvs.size = MEMACCESSP0_getdata(in, globalparamsE.BASEOFFSETKVS_ACTIVEEDGESMAP, index + 1);
				workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			} else {
				unsigned int index = (sweepparams.source_partition * num_LLPset) + llp_set;
				workload_kvs.offset_begin = MEMACCESSP0_getdata(in, globalparamsE.BASEOFFSETKVS_EDGESMAP, index) / UPDATEDATA_PACKINGSIZE;
				workload_kvs.offset_end = MEMACCESSP0_getdata(in, globalparamsE.BASEOFFSETKVS_EDGESMAP, index + 1) / UPDATEDATA_PACKINGSIZE;
				workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
			}
		
			if(workload_kvs.offset_end < workload_kvs.offset_begin){ workload_kvs.size = 0; }
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"actit3:: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<", workload_kvs.offset_end: "<<workload_kvs.offset_end<<", workload_kvs.size: "<<workload_kvs.size<<"] "<<endl;
			#endif
			workloads_kvs[n] = workload_kvs;
		}
		
		// run bfs
		for(unsigned int n=0; n<num_its; n++){
			for(int i = 0; i < workloads_kvs[n].size; i++){
			#pragma HLS PIPELINE II=1
				// get dataset
				UTILP0_ReadKeyvalues(in, globalparamsE.BASEOFFSETKVS_EDGESDATA + workloads_kvs[n].offset_begin + i, datas);
				
				// set flag
				bool enx = true;
				if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){  // used in BFS implementations ONLY (not PR or CF)
					collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
					collections[PROCESSEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
				}
				
				// process	
	
				res[0] = process_edge(enx, 0, datas[0].value - sweepparams.upperlimit, datas[0], vbuffer_sourcebits[0], vbuffer_sourcebits[1], globalparamsK);
	
				res[1] = process_edge(enx, 1, datas[1].value - sweepparams.upperlimit, datas[1], vbuffer_sourcebits[2], vbuffer_sourcebits[3], globalparamsK);
	
				res[2] = process_edge(enx, 2, datas[2].value - sweepparams.upperlimit, datas[2], vbuffer_sourcebits[4], vbuffer_sourcebits[5], globalparamsK);
	
				res[3] = process_edge(enx, 3, datas[3].value - sweepparams.upperlimit, datas[3], vbuffer_sourcebits[6], vbuffer_sourcebits[7], globalparamsK);
	
				res[4] = process_edge(enx, 4, datas[4].value - sweepparams.upperlimit, datas[4], vbuffer_sourcebits[8], vbuffer_sourcebits[9], globalparamsK);
	
				res[5] = process_edge(enx, 5, datas[5].value - sweepparams.upperlimit, datas[5], vbuffer_sourcebits[10], vbuffer_sourcebits[11], globalparamsK);
	
				res[6] = process_edge(enx, 6, datas[6].value - sweepparams.upperlimit, datas[6], vbuffer_sourcebits[12], vbuffer_sourcebits[13], globalparamsK);
	
				res[7] = process_edge(enx, 7, datas[7].value - sweepparams.upperlimit, datas[7], vbuffer_sourcebits[14], vbuffer_sourcebits[15], globalparamsK);
				
				// reduce
	
				reduce_update(enx, res[0].key, res[0], vbuffer_dest[0], vbuffer_dest[1], memory[0], globalparamsK);
	
				reduce_update(enx, res[1].key, res[1], vbuffer_dest[2], vbuffer_dest[3], memory[1], globalparamsK);
	
				reduce_update(enx, res[2].key, res[2], vbuffer_dest[4], vbuffer_dest[5], memory[2], globalparamsK);
	
				reduce_update(enx, res[3].key, res[3], vbuffer_dest[6], vbuffer_dest[7], memory[3], globalparamsK);
	
				reduce_update(enx, res[4].key, res[4], vbuffer_dest[8], vbuffer_dest[9], memory[4], globalparamsK);
	
				reduce_update(enx, res[5].key, res[5], vbuffer_dest[10], vbuffer_dest[11], memory[5], globalparamsK);
	
				reduce_update(enx, res[6].key, res[6], vbuffer_dest[12], vbuffer_dest[13], memory[6], globalparamsK);
	
				reduce_update(enx, res[7].key, res[7], vbuffer_dest[14], vbuffer_dest[15], memory[7], globalparamsK);
				
				if(enx == true){ collections[REDUCEUPDATES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE; }
			}
		}
	}
}

void acts_all::ACTSP0_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){

	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory // complete
	workload_t workloads_kvs[BLOCKRAM_SIZE];
	keyvalue_vbuffer_t vbuffer_sourcebits[VDATA_PACKINGSIZE][BLOCKRAM_SIZE]; /////////////////////
	#pragma HLS ARRAY_PARTITION variable = vbuffer_sourcebits
	
	collection_t collections_tmp[COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable=collections_tmp complete
	collections_tmp[TRAVERSEDEDGES_COLLECTIONID] = collections[TRAVERSEDEDGES_COLLECTIONID];
	collections_tmp[PROCESSEDGES_COLLECTIONID] = collections[PROCESSEDGES_COLLECTIONID];
	collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections[ACTIVEEDGESPROCESSED_COLLECTIONID];
	collections_tmp[REDUCEUPDATES_COLLECTIONID] = collections[REDUCEUPDATES_COLLECTIONID];
	
	unsigned int num_vPs = globalparamsK.NUM_PROCESSEDGESPARTITIONS;
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int vsize_LLP; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ vsize_LLP = 262144; } else { vsize_LLP = globalparamsK.SIZEKVS2_REDUCEPARTITION; }
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	unsigned int upperlimit = sweepparams.source_partition * globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit3:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", num_vPs: "<<num_vPs<<"] "<<endl;
	cout<<"actit3:: globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_vertices: "<<globalposition.num_active_vertices<<", globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION: "<<globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION<<""<<endl; 
	#endif 
	
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		// #ifdef CONFIG_ACTS_HYBRIDLOGIC
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actit3:: TOO FEW VERTICES. using TRADGP instead: [source_partition: "<<globalposition.source_partition<<", num_active_vertices: "<<globalposition.num_active_vertices<<"]"<<endl; 
		#endif 
		ACTSP0_read_process_and_reduce(kvdram, kvdram, vbuffer_sourcebits, vbuffer_dest, collections, sweepparams, globalparamsE, globalparamsK, globalposition);
		// #endif	 
	} else {	
		if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){
			ACTIT3_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
				// retreived workload info 
				workload_t workload_kvs;
				workload_kvs.offset_begin = MEMACCESSP0_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, (sweepparams.source_partition * num_LLPset) + llp_set) / UPDATEDATA_PACKINGSIZE;
				workload_kvs.offset_end = MEMACCESSP0_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, (sweepparams.source_partition * num_LLPset) + llp_set + 1) / UPDATEDATA_PACKINGSIZE;
				workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
				if(workload_kvs.offset_end < workload_kvs.offset_begin){ workload_kvs.size = 0; }
				#ifdef _DEBUGMODE_CHECKS3
				if(workload_kvs.offset_end < workload_kvs.offset_begin){ cout<<"actit3(process): ERROR 23: workload_kvs.offset_end("<<workload_kvs.offset_end<<") < workload_kvs.offset_begin("<<workload_kvs.offset_begin<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
				#endif		
				
				workloads_kvs[llp_set] = workload_kvs;
			}
		
			ACTIT3_MAINLOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
				workload_t workload_kvs = workloads_kvs[llp_set];
				batch_type destoffset_kvs = (globalcapsule[llp_set].key + globalcapsule[llp_set].value) / UPDATEDATA_PACKINGSIZE;
				
				#ifdef _DEBUGMODE_KERNELPRINTS
				unsigned int num_chunks = workload_kvs.size / BLOCKRAM_SIZE;
				cout<<"actit3(process): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][size_kvs: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<num_chunks<<"] ... "<<endl;					
				#endif
				
				ACTSP0_read_process_partition_and_write_base(kvdram, vbuffer_source,	
					upperlimit, sourcebaseaddr_kvs, workload_kvs, destbaseaddr_kvs, destoffset_kvs, 
						collections_tmp, globalparamsK, globalposition);
				
				globalcapsule[llp_set].value += workload_kvs.size * UPDATEDATA_PACKINGSIZE; 
			}
		}
		
		if(mode == ACTSREDUCEMODE){
			// retreived workload info 
			workload_t workload_kvs;
			workload_kvs.offset_begin = ptravstate.begin_kvs; workload_kvs.offset_end = ptravstate.end_kvs;
			workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
			#ifdef _DEBUGMODE_CHECKS3
			if(workload_kvs.offset_end < workload_kvs.offset_begin){ cout<<"actit3(reduce): ERROR 23: workload_kvs.offset_end("<<workload_kvs.offset_end<<") < workload_kvs.offset_begin("<<workload_kvs.offset_begin<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif
		
			ACTSP0_read_and_reduce(mode, kvdram, vbuffer_dest,	
				sweepparams, sourcebaseaddr_kvs, workload_kvs,
					collections_tmp, globalparamsK, globalposition);
		}
		
		collections[TRAVERSEDEDGES_COLLECTIONID] = collections_tmp[TRAVERSEDEDGES_COLLECTIONID];
		collections[PROCESSEDGES_COLLECTIONID] = collections_tmp[PROCESSEDGES_COLLECTIONID];
		collections[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID];
		collections[REDUCEUPDATES_COLLECTIONID] = collections_tmp[REDUCEUPDATES_COLLECTIONID];
	}
}

void acts_all::ACTSP0_actit_base(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	ACTSP0_actit(enable, mode, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sourcebaseaddr_kvs, destbaseaddr_kvs, resetenv, flush, edgebankID, collections);
}