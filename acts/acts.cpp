// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Vitis/multiple_kernels/vadd_kernel/krnl_vadd.cpp
// https://github.com/Xilinx/SDAccel_Examples/blob/master/getting_started/dataflow/dataflow_stream_c/src/adder.cpp
// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Dataflow/Channels/using_fifos/diamond.cpp  *

void acts_all::rearrangeLayoutVx16(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]){
	unsigned int s_ = s;
	if(s_==0){ 
		out[0] = in[0]; 
		out[1] = in[1]; 
		out[2] = in[2]; 
		out[3] = in[3]; 
		out[4] = in[4]; 
		out[5] = in[5]; 
		out[6] = in[6]; 
		out[7] = in[7]; 
		out[8] = in[8]; 
		out[9] = in[9]; 
		out[10] = in[10]; 
		out[11] = in[11]; 
		out[12] = in[12]; 
		out[13] = in[13]; 
		out[14] = in[14]; 
		out[15] = in[15]; 
	} else if(s_==1){ 
		out[15] = in[0]; 
		out[0] = in[1]; 
		out[1] = in[2]; 
		out[2] = in[3]; 
		out[3] = in[4]; 
		out[4] = in[5]; 
		out[5] = in[6]; 
		out[6] = in[7]; 
		out[7] = in[8]; 
		out[8] = in[9]; 
		out[9] = in[10]; 
		out[10] = in[11]; 
		out[11] = in[12]; 
		out[12] = in[13]; 
		out[13] = in[14]; 
		out[14] = in[15]; 
	} else if(s_==2){ 
		out[14] = in[0]; 
		out[15] = in[1]; 
		out[0] = in[2]; 
		out[1] = in[3]; 
		out[2] = in[4]; 
		out[3] = in[5]; 
		out[4] = in[6]; 
		out[5] = in[7]; 
		out[6] = in[8]; 
		out[7] = in[9]; 
		out[8] = in[10]; 
		out[9] = in[11]; 
		out[10] = in[12]; 
		out[11] = in[13]; 
		out[12] = in[14]; 
		out[13] = in[15]; 
	} else if(s_==3){ 
		out[13] = in[0]; 
		out[14] = in[1]; 
		out[15] = in[2]; 
		out[0] = in[3]; 
		out[1] = in[4]; 
		out[2] = in[5]; 
		out[3] = in[6]; 
		out[4] = in[7]; 
		out[5] = in[8]; 
		out[6] = in[9]; 
		out[7] = in[10]; 
		out[8] = in[11]; 
		out[9] = in[12]; 
		out[10] = in[13]; 
		out[11] = in[14]; 
		out[12] = in[15]; 
	} else if(s_==4){ 
		out[12] = in[0]; 
		out[13] = in[1]; 
		out[14] = in[2]; 
		out[15] = in[3]; 
		out[0] = in[4]; 
		out[1] = in[5]; 
		out[2] = in[6]; 
		out[3] = in[7]; 
		out[4] = in[8]; 
		out[5] = in[9]; 
		out[6] = in[10]; 
		out[7] = in[11]; 
		out[8] = in[12]; 
		out[9] = in[13]; 
		out[10] = in[14]; 
		out[11] = in[15]; 
	} else if(s_==5){ 
		out[11] = in[0]; 
		out[12] = in[1]; 
		out[13] = in[2]; 
		out[14] = in[3]; 
		out[15] = in[4]; 
		out[0] = in[5]; 
		out[1] = in[6]; 
		out[2] = in[7]; 
		out[3] = in[8]; 
		out[4] = in[9]; 
		out[5] = in[10]; 
		out[6] = in[11]; 
		out[7] = in[12]; 
		out[8] = in[13]; 
		out[9] = in[14]; 
		out[10] = in[15]; 
	} else if(s_==6){ 
		out[10] = in[0]; 
		out[11] = in[1]; 
		out[12] = in[2]; 
		out[13] = in[3]; 
		out[14] = in[4]; 
		out[15] = in[5]; 
		out[0] = in[6]; 
		out[1] = in[7]; 
		out[2] = in[8]; 
		out[3] = in[9]; 
		out[4] = in[10]; 
		out[5] = in[11]; 
		out[6] = in[12]; 
		out[7] = in[13]; 
		out[8] = in[14]; 
		out[9] = in[15]; 
	} else if(s_==7){ 
		out[9] = in[0]; 
		out[10] = in[1]; 
		out[11] = in[2]; 
		out[12] = in[3]; 
		out[13] = in[4]; 
		out[14] = in[5]; 
		out[15] = in[6]; 
		out[0] = in[7]; 
		out[1] = in[8]; 
		out[2] = in[9]; 
		out[3] = in[10]; 
		out[4] = in[11]; 
		out[5] = in[12]; 
		out[6] = in[13]; 
		out[7] = in[14]; 
		out[8] = in[15]; 
	} else if(s_==8){ 
		out[8] = in[0]; 
		out[9] = in[1]; 
		out[10] = in[2]; 
		out[11] = in[3]; 
		out[12] = in[4]; 
		out[13] = in[5]; 
		out[14] = in[6]; 
		out[15] = in[7]; 
		out[0] = in[8]; 
		out[1] = in[9]; 
		out[2] = in[10]; 
		out[3] = in[11]; 
		out[4] = in[12]; 
		out[5] = in[13]; 
		out[6] = in[14]; 
		out[7] = in[15]; 
	} else if(s_==9){ 
		out[7] = in[0]; 
		out[8] = in[1]; 
		out[9] = in[2]; 
		out[10] = in[3]; 
		out[11] = in[4]; 
		out[12] = in[5]; 
		out[13] = in[6]; 
		out[14] = in[7]; 
		out[15] = in[8]; 
		out[0] = in[9]; 
		out[1] = in[10]; 
		out[2] = in[11]; 
		out[3] = in[12]; 
		out[4] = in[13]; 
		out[5] = in[14]; 
		out[6] = in[15]; 
	} else if(s_==10){ 
		out[6] = in[0]; 
		out[7] = in[1]; 
		out[8] = in[2]; 
		out[9] = in[3]; 
		out[10] = in[4]; 
		out[11] = in[5]; 
		out[12] = in[6]; 
		out[13] = in[7]; 
		out[14] = in[8]; 
		out[15] = in[9]; 
		out[0] = in[10]; 
		out[1] = in[11]; 
		out[2] = in[12]; 
		out[3] = in[13]; 
		out[4] = in[14]; 
		out[5] = in[15]; 
	} else if(s_==11){ 
		out[5] = in[0]; 
		out[6] = in[1]; 
		out[7] = in[2]; 
		out[8] = in[3]; 
		out[9] = in[4]; 
		out[10] = in[5]; 
		out[11] = in[6]; 
		out[12] = in[7]; 
		out[13] = in[8]; 
		out[14] = in[9]; 
		out[15] = in[10]; 
		out[0] = in[11]; 
		out[1] = in[12]; 
		out[2] = in[13]; 
		out[3] = in[14]; 
		out[4] = in[15]; 
	} else if(s_==12){ 
		out[4] = in[0]; 
		out[5] = in[1]; 
		out[6] = in[2]; 
		out[7] = in[3]; 
		out[8] = in[4]; 
		out[9] = in[5]; 
		out[10] = in[6]; 
		out[11] = in[7]; 
		out[12] = in[8]; 
		out[13] = in[9]; 
		out[14] = in[10]; 
		out[15] = in[11]; 
		out[0] = in[12]; 
		out[1] = in[13]; 
		out[2] = in[14]; 
		out[3] = in[15]; 
	} else if(s_==13){ 
		out[3] = in[0]; 
		out[4] = in[1]; 
		out[5] = in[2]; 
		out[6] = in[3]; 
		out[7] = in[4]; 
		out[8] = in[5]; 
		out[9] = in[6]; 
		out[10] = in[7]; 
		out[11] = in[8]; 
		out[12] = in[9]; 
		out[13] = in[10]; 
		out[14] = in[11]; 
		out[15] = in[12]; 
		out[0] = in[13]; 
		out[1] = in[14]; 
		out[2] = in[15]; 
	} else if(s_==14){ 
		out[2] = in[0]; 
		out[3] = in[1]; 
		out[4] = in[2]; 
		out[5] = in[3]; 
		out[6] = in[4]; 
		out[7] = in[5]; 
		out[8] = in[6]; 
		out[9] = in[7]; 
		out[10] = in[8]; 
		out[11] = in[9]; 
		out[12] = in[10]; 
		out[13] = in[11]; 
		out[14] = in[12]; 
		out[15] = in[13]; 
		out[0] = in[14]; 
		out[1] = in[15]; 
	} else { 
		out[1] = in[0]; 
		out[2] = in[1]; 
		out[3] = in[2]; 
		out[4] = in[3]; 
		out[5] = in[4]; 
		out[6] = in[5]; 
		out[7] = in[6]; 
		out[8] = in[7]; 
		out[9] = in[8]; 
		out[10] = in[9]; 
		out[11] = in[10]; 
		out[12] = in[11]; 
		out[13] = in[12]; 
		out[14] = in[13]; 
		out[15] = in[14]; 
		out[0] = in[15]; 
	}
	return;
}	
	
keyvalue_t acts_all::process_edge(bool enx, unsigned int v, unsigned int loc, keyvalue_t edge_data, keyvalue_vbuffer_t * vbuffer, globalparams_t globalparams){				
	#pragma HLS INLINE
	
	// flag 
	bool en = true; // if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){ loc = 0; } 
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true && loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){ cout<<"processvector::ERROR SEEN @@ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<". EXITING... "<<endl; exit(EXIT_FAILURE); }					
	#endif 
	
	// read
	value_t combo= 0;
	#ifdef BIT_TRAVERSAL_ALGORITHM
	combo = vbuffer[loc / 16].data[loc % 16];	
	#else 
	combo = vbuffer[loc].data;	
	#endif 
	value_t mask; if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { mask = combo & 0x1; }
	value_t udata = combo >> 1; 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mask == 1){ cout<<">>> PROCESS VECTOR:: PROCESS EDGE SEEN @ v: "<<v<<", loc: "<<loc<<", edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", udata: "<<udata<<", mask: "<<mask<<", srcvid: "<<UTILP0_GETREALVID((edge_data.value * EDGEDATA_PACKINGSIZE) + v, globalparams.ACTSPARAMS_INSTID)<<", dstvid*: "<<edge_data.key<<endl; }
	#endif
	
	// process
	value_t res = process_func(udata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	keyvalue_t vupdate; if(mask == 1){ vupdate.key = edge_data.key; vupdate.value = res; } else { vupdate.key = INVALIDDATA; vupdate.value = INVALIDDATA; }
	return vupdate;
}

void acts_all::reduce_update(bool enx, unsigned int v, unsigned int loc, keyvalue_t update_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int memory[1], globalparams_t globalparams){
	#pragma HLS INLINE

	// flag 
	bool en = true;

	// check loc
	#ifdef _DEBUGMODE_CHECKS3
	if(true && loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE)){ cout<<"reduce_update::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). update_data.key: "<<update_data.key<<", update_data.key: "<<update_data.key<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	// if(loc >= (globalparams.SIZEKVS2_REDUCEPARTITION / 2) && en == true){ loc = 0; } // loc = loc / 2; // urams pack in 2s
	
	// if(en == true){ if(loc == memory[0]){ loc = (loc + 1) % 8; } memory[0] = loc; } // CRITICAL FIXME.
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 113::1", loc, MAX_BLOCKRAM_VDESTDATA_SIZE, NAp, loc, VDATA_SHRINK_RATIO); }
	#endif
	
	// read
	value_t combo = 0;
	if(en == true){
		#ifdef BIT_TRAVERSAL_ALGORITHM
		combo = vbuffer[loc / 16].data[loc % 16]; 
		#else 
		combo = vbuffer[loc].data; 
		#endif 
	}
	value_t mask; if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { mask = combo & 0x1; }
	value_t vdata_tmp = combo >> 1; 
	
	// reduce 
	value_t new_vprop = reduce_func(vdata_tmp, vdata_tmp, update_data.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	// write-back
	if(en == true && new_vprop != vdata_tmp){
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		cout<<">>> REDUCE VECTOR:: REDUCE UPDATE SEEN @: v: "<<v<<", loc: "<<loc<<", vdata_tmp: "<<vdata_tmp<<", mask: "<<mask<<", update_data.value: "<<update_data.value<<", new_vprop: "<<new_vprop<<", new combo: "<<((new_vprop << 1) | 0x1)<<", dstvid: "<<UTILP0_GETREALVID(update_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; 
		#endif
		
		#ifdef BIT_TRAVERSAL_ALGORITHM
		vbuffer[loc / 16].data[loc % 16] = 1; 
		#else 
		vbuffer[loc].data = (new_vprop << 1) | 0x1; 
		#endif 
	}
	// exit(EXIT_SUCCESS);
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
	keyvalue_t edges[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = edges complete
	uint512_uvec_dt datas2;
	
	Loop_A: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		// read
		uint512_evec_dt data = in.read();
		
		// decode
		for(int v = 0; v < VECTOR2_SIZE; v++){
		#pragma HLS UNROLL 
			if(data.data[v] != INVALIDDATA){ edges[v].key = data.data[v] & MASK_DSTVID; edges[v].value = (data.data[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // srcvid is upper, dstvid is lower 
			else { edges[v].key = INVALIDDATA; edges[v].value = INVALIDDATA; }
		}
		
		// set flag
		bool enx = true;
		collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
		collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE;  // *** used in PR, CF implementations ONLY (not BFS) ***
		
		// process
		for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){
			#pragma HLS UNROLL
			if(datas[v] != INVALIDDATA){ res[v] = process_edge(enx, v, edges[v].value, edges[v], vbuffer_source[v], globalparamsK); }	
			else { res[v].key = INVALIDDATA; res[v].value = INVALIDDATA; }
		}	
		
		// encode 
		for(int v = 0; v < VECTOR2_SIZE; v++){
		#pragma HLS UNROLL
			if(res[v].key != INVALIDDATA){ datas2.data[v] = (res[v].value << DSTVID_BITSZ) | res[v].key; } else { datas2.data[v] = INVALIDDATA; }
		}
		
		// store
		UTILP0_WriteDatas(out, destbaseaddr_kvs + offset_kvs + i, datas2);
	}
}
#endif 

void acts_all::ACTSP0_read_process_partition_and_write(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
			batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, int size, unsigned int upperlimit, 
				collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	
	keyvalue_t res[UPDATEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas2 complete
	keyvalue_t edges[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = edges complete
	
	for(int i = 0; i < size; i++){
		// read 
		UTILP0_ReadEdges(in, sourcebaseaddr_kvs + srcoffset_kvs + i, datas);
		
		// decode
		for(int v = 0; v < VECTOR2_SIZE; v++){
		#pragma HLS UNROLL 
			if(datas[v] != INVALIDDATA){ edges[v].key = datas[v] & MASK_DSTVID; edges[v].value = (datas[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // srcvid is upper, dstvid is lower 
			else { edges[v].key = INVALIDDATA; edges[v].value = INVALIDDATA; }
		}
	
		// set flag
		bool enx = true;
		collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
		collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE; // *** used in PR, CF implementations ONLY (not BFS) ***
		
		// process
		for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){
			#pragma HLS UNROLL
			// if(i<4){ cout<<"---- i; "<<i<<", acts:: edges["<<v<<"].key: "<<edges[v].key<<", edges["<<v<<"].value: "<<edges[v].value<<", i: "<<i<<", upperlimit: "<<upperlimit<<", INVALIDDATA: "<<INVALIDDATA<<endl; }
			if(datas[v] != INVALIDDATA){ res[v] = process_edge(enx, v, edges[v].value, edges[v], vbuffer_source[v], globalparamsK); }	
			else { res[v].key = INVALIDDATA; res[v].value = INVALIDDATA; }
		}	

		// encode
		for(int v = 0; v < VECTOR2_SIZE; v++){
		#pragma HLS UNROLL
			if(res[v].key != INVALIDDATA){ datas2[v] = (res[v].value << DSTVID_BITSZ) | res[v].key; } else { datas2[v] = INVALIDDATA; }
			// datas2[v] = INVALIDDATA; // 0xFFFFFFFE;
		}
		
		// store
		UTILP0_WriteDatas(out, destbaseaddr_kvs + destoffset_kvs + i, datas2);
		
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(enx == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE); }
		#endif 
	}
	// exit(EXIT_SUCCESS);
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
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	keyvalue_t updates_in[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = updates_in complete
	keyvalue_t updates_out[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = updates_out complete
	
	unsigned int tmp = collections[ACTIVEEDGESPROCESSED_COLLECTIONID].data1;
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"actit3(reduce): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][workload_kvs.size: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<(workload_kvs.offset_end - workload_kvs.offset_begin) / globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"] ... "<<endl;					
	#endif
	ACTIT3_MAINLOOP2A: for(batch_type offset_kvs=workload_kvs.offset_begin; offset_kvs<workload_kvs.offset_begin + workload_kvs.size; offset_kvs++){
	#pragma HLS PIPELINE II=1
	#pragma HLS dependence variable=vbuffer_dest inter false
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actit3(reduce): processing chunk [offset_kvs: "<<offset_kvs<<"]: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<"]: [workload_kvs.offset_end: "<<workload_kvs.offset_end<<"] ... "<<endl;
		#endif
		
		// read 
		UTILP0_ReadDatas(kvdram, sourcebaseaddr_kvs + offset_kvs, datas);
		
		// decode
		for(int v = 0; v < VECTOR2_SIZE; v++){
		#pragma HLS UNROLL 
			if(datas[v] != INVALIDDATA){ updates_in[v].key = datas[v] & MASK_DSTVID; updates_in[v].value = (datas[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // 16, 18, srcvid is upper, dstvid is lower 
			else { updates_in[v].key = INVALIDDATA; updates_in[v].value = INVALIDDATA; }
		}
		
		// set flag
		bool enx = true;
		if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){  // used in BFS implementations ONLY (not PR or CF)
			collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
			collections[PROCESSEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
			if(tmp + ((offset_kvs - workload_kvs.offset_begin) * UPDATEDATA_PACKINGSIZE) >= globalposition.num_active_edges_in_channel){ enx = false; } 
			else { enx = true; collections[ACTIVEEDGESPROCESSED_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE; }
		}
		
		// shift 
		unsigned int sample_key = 0; for(unsigned int v=0; v<UPDATEDATA_PACKINGSIZE; v++){ if(updates_in[v].key != INVALIDDATA){ sample_key = updates_in[v].key; }} ///////// CRITICAL FIXME.
		rearrangeLayoutVx16(sample_key % UPDATEDATA_PACKINGSIZE, updates_in, updates_out);
		
		// reduce	
		for(int v = 0; v < UPDATEDATA_PACKINGSIZE; v++){
			#pragma HLS UNROLL
			// if(updates_out[v].key<64){ cout<<"&&&&::reduce:: v; "<<v<<", updates_out["<<v<<"].key: "<<updates_out[v].key<<", updates_out["<<v<<"].value: "<<updates_out[v].value<<", offset_kvs: "<<offset_kvs<<", INVALIDDATA: "<<INVALIDDATA<<", sample_key: "<<sample_key<<endl; }	
			if(updates_out[v].key != INVALIDDATA){ reduce_update(enx, v, updates_out[v].key / UPDATEDATA_PACKINGSIZE, updates_out[v], vbuffer_dest[v], memory[v], globalparamsK); }	
		}
		
		if(enx == true){ collections[REDUCEUPDATES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE; }
		// exit(EXIT_SUCCESS);
	}
}

void acts_all::ACTSP0_read_process_and_reduce(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_sourcebits[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
				collection_t collections[COLLECTIONS_BUFFERSZ], sweepparams_t sweepparams, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition){
	
	keyvalue_t res[EDGEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res complete
	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory 
	keyvalue_t datas[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	unsigned int actvvs[DOUBLE_BLOCKRAM_SIZE];//BLOCKRAM_SIZE];
	workload_t workloads_kvs[DOUBLE_BLOCKRAM_SIZE];
	
	unsigned int num_actvvs = globalposition.num_active_vertices;
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> actit3:: [iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", v_partition: "<<sweepparams.source_partition<<", num_actvvs: "<<num_actvvs<<"] "<<endl;
	#endif 
	
	READ_PROCESS_REDUCE_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
		// get workload stats 
		unsigned int num_its; 
		if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ num_its = num_actvvs; } else { num_its = 1; }
		
		READ_PROCESS_REDUCE_LOOP1A: for(unsigned int n=0; n<num_its; n++){
		// #pragma HLS PIPELINE II=20
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
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"actit3:: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<", workload_kvs.offset_end: "<<workload_kvs.offset_end<<", workload_kvs.size: "<<workload_kvs.size<<"] "<<endl;
			#endif
			workloads_kvs[n] = workload_kvs;
		}
		
		// run bfs
		READ_PROCESS_REDUCE_LOOP1B: for(unsigned int n=0; n<num_its; n++){
			unsigned int sz = workloads_kvs[n].size;
			unsigned int offset = workloads_kvs[n].offset_begin;
			READ_PROCESS_REDUCE_LOOP1C: for(unsigned int i = 0; i < sz; i++){
			#pragma HLS PIPELINE II=1
			#pragma HLS dependence variable=vbuffer_dest inter false
				// get dataset
				UTILP0_ReadEdges(in, globalparamsE.BASEOFFSETKVS_EDGESDATA + offset + i, datas);
				
				// set flag
				bool enx = true;
				if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){  // used in BFS implementations ONLY (not PR or CF)
					collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
					collections[PROCESSEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
				}
				
				// process
	
				res[0] = process_edge(enx, 0, datas[0].value - sweepparams.upperlimit, datas[0], vbuffer_sourcebits[0], globalparamsK);
	
				res[1] = process_edge(enx, 1, datas[1].value - sweepparams.upperlimit, datas[1], vbuffer_sourcebits[1], globalparamsK);
	
				res[2] = process_edge(enx, 2, datas[2].value - sweepparams.upperlimit, datas[2], vbuffer_sourcebits[2], globalparamsK);
	
				res[3] = process_edge(enx, 3, datas[3].value - sweepparams.upperlimit, datas[3], vbuffer_sourcebits[3], globalparamsK);
	
				res[4] = process_edge(enx, 4, datas[4].value - sweepparams.upperlimit, datas[4], vbuffer_sourcebits[4], globalparamsK);
	
				res[5] = process_edge(enx, 5, datas[5].value - sweepparams.upperlimit, datas[5], vbuffer_sourcebits[5], globalparamsK);
	
				res[6] = process_edge(enx, 6, datas[6].value - sweepparams.upperlimit, datas[6], vbuffer_sourcebits[6], globalparamsK);
	
				res[7] = process_edge(enx, 7, datas[7].value - sweepparams.upperlimit, datas[7], vbuffer_sourcebits[7], globalparamsK);
	
				res[8] = process_edge(enx, 8, datas[8].value - sweepparams.upperlimit, datas[8], vbuffer_sourcebits[8], globalparamsK);
	
				res[9] = process_edge(enx, 9, datas[9].value - sweepparams.upperlimit, datas[9], vbuffer_sourcebits[9], globalparamsK);
	
				res[10] = process_edge(enx, 10, datas[10].value - sweepparams.upperlimit, datas[10], vbuffer_sourcebits[10], globalparamsK);
	
				res[11] = process_edge(enx, 11, datas[11].value - sweepparams.upperlimit, datas[11], vbuffer_sourcebits[11], globalparamsK);
	
				res[12] = process_edge(enx, 12, datas[12].value - sweepparams.upperlimit, datas[12], vbuffer_sourcebits[12], globalparamsK);
	
				res[13] = process_edge(enx, 13, datas[13].value - sweepparams.upperlimit, datas[13], vbuffer_sourcebits[13], globalparamsK);
	
				res[14] = process_edge(enx, 14, datas[14].value - sweepparams.upperlimit, datas[14], vbuffer_sourcebits[14], globalparamsK);
	
				res[15] = process_edge(enx, 15, datas[15].value - sweepparams.upperlimit, datas[15], vbuffer_sourcebits[15], globalparamsK);
				
				// reduce
	
				reduce_update(enx, 0, res[0].key, res[0], vbuffer_dest[0], memory[0], globalparamsK);
	
				reduce_update(enx, 1, res[1].key, res[1], vbuffer_dest[1], memory[1], globalparamsK);
	
				reduce_update(enx, 2, res[2].key, res[2], vbuffer_dest[2], memory[2], globalparamsK);
	
				reduce_update(enx, 3, res[3].key, res[3], vbuffer_dest[3], memory[3], globalparamsK);
	
				reduce_update(enx, 4, res[4].key, res[4], vbuffer_dest[4], memory[4], globalparamsK);
	
				reduce_update(enx, 5, res[5].key, res[5], vbuffer_dest[5], memory[5], globalparamsK);
	
				reduce_update(enx, 6, res[6].key, res[6], vbuffer_dest[6], memory[6], globalparamsK);
	
				reduce_update(enx, 7, res[7].key, res[7], vbuffer_dest[7], memory[7], globalparamsK);
	
				reduce_update(enx, 8, res[8].key, res[8], vbuffer_dest[8], memory[8], globalparamsK);
	
				reduce_update(enx, 9, res[9].key, res[9], vbuffer_dest[9], memory[9], globalparamsK);
	
				reduce_update(enx, 10, res[10].key, res[10], vbuffer_dest[10], memory[10], globalparamsK);
	
				reduce_update(enx, 11, res[11].key, res[11], vbuffer_dest[11], memory[11], globalparamsK);
	
				reduce_update(enx, 12, res[12].key, res[12], vbuffer_dest[12], memory[12], globalparamsK);
	
				reduce_update(enx, 13, res[13].key, res[13], vbuffer_dest[13], memory[13], globalparamsK);
	
				reduce_update(enx, 14, res[14].key, res[14], vbuffer_dest[14], memory[14], globalparamsK);
	
				reduce_update(enx, 15, res[15].key, res[15], vbuffer_dest[15], memory[15], globalparamsK);
				
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
	// unsigned int vsize_LLP; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ vsize_LLP = 262144; } else { vsize_LLP = globalparamsK.SIZEKVS2_REDUCEPARTITION; }
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	unsigned int upperlimit = sweepparams.source_partition * globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit3:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", num_vPs: "<<num_vPs<<"] "<<endl;
	cout<<"actit3:: globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_vertices: "<<globalposition.num_active_vertices<<", globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION: "<<globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION<<""<<endl; 
	#endif 
	
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID == BFS){ //  globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		#ifdef BIT_TRAVERSAL_ALGORITHM
		ACTSP0_read_process_and_reduce(kvdram, kvdram, vbuffer_sourcebits, vbuffer_dest, collections_tmp, sweepparams, globalparamsE, globalparamsK, globalposition); 
		#endif 
		collections[TRAVERSEDEDGES_COLLECTIONID] = collections_tmp[TRAVERSEDEDGES_COLLECTIONID];
		collections[PROCESSEDGES_COLLECTIONID] = collections_tmp[PROCESSEDGES_COLLECTIONID];
		collections[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID];
		collections[REDUCEUPDATES_COLLECTIONID] = collections_tmp[REDUCEUPDATES_COLLECTIONID];
	} else {	
		if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){
			ACTIT3_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
			// #pragma HLS PIPELINE II=20
				workload_t workload_kvs;
				workload_kvs.offset_begin = MEMACCESSP0_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, (sweepparams.source_partition * num_LLPset) + llp_set) / EDGEDATA_PACKINGSIZE;
				workload_kvs.offset_end = MEMACCESSP0_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, (sweepparams.source_partition * num_LLPset) + llp_set + 1) / EDGEDATA_PACKINGSIZE;
				workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
				if(workload_kvs.offset_end < workload_kvs.offset_begin){ workload_kvs.size = 0; }
				// cout<<"actit3(process): workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<", workload_kvs.offset_end: "<<workload_kvs.offset_end<<""<<endl;
				#ifdef _DEBUGMODE_CHECKS3
				if(workload_kvs.offset_end < workload_kvs.offset_begin){ cout<<"actit3(process): ERROR 23: workload_kvs.offset_end("<<workload_kvs.offset_end<<") < workload_kvs.offset_begin("<<workload_kvs.offset_begin<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
				#endif		
				
				workloads_kvs[llp_set] = workload_kvs;
			}
		
			ACTIT3_MAINLOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
				workload_t workload_kvs = workloads_kvs[llp_set];
				batch_type destoffset_kvs = (globalcapsule[1 + llp_set].key + globalcapsule[llp_set].value) / EDGEDATA_PACKINGSIZE;
				
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"actit3(process): processing all chunks in llp_set "<<llp_set<<": [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][size_kvs: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"], sweepparams.source_partition: "<<sweepparams.source_partition<<" ... "<<endl;					
				#endif
				
				ACTSP0_read_process_partition_and_write_base(kvdram, vbuffer_source,	
					upperlimit, sourcebaseaddr_kvs, workload_kvs, destbaseaddr_kvs, destoffset_kvs, 
						collections_tmp, globalparamsK, globalposition);
				
				globalcapsule[1 + llp_set].value += workload_kvs.size * EDGEDATA_PACKINGSIZE; 
			}
			// exit(EXIT_SUCCESS);
		}
		
		if(mode == ACTSREDUCEMODE){
			workload_t workload_kvs;
			workload_kvs.offset_begin = globalcapsule[1 + sweepparams.source_partition].key / UPDATEDATA_PACKINGSIZE; 
			workload_kvs.size = globalcapsule[1 + sweepparams.source_partition].value / UPDATEDATA_PACKINGSIZE;
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			#ifdef _DEBUGMODE_CHECKS3
			if(workload_kvs.offset_end < workload_kvs.offset_begin){ cout<<"actit3(reduce): ERROR 23: workload_kvs.offset_end("<<workload_kvs.offset_end<<") < workload_kvs.offset_begin("<<workload_kvs.offset_begin<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"actit3(reduce): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][size_kvs: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"] ... "<<endl;					
			#endif
		
			ACTSP0_read_and_reduce(mode, kvdram, vbuffer_dest,	
				sweepparams, sourcebaseaddr_kvs, workload_kvs,
					collections_tmp, globalparamsK, globalposition);
			// exit(EXIT_SUCCESS);
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