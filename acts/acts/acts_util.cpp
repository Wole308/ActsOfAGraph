// basic
unsigned int acts_all::UTILP0_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int acts_all::UTILP0_amax(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int acts_all::UTILP0_aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}
uint32_type acts_all::UTILP0_amin2(uint32_type val1, uint32_type val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

// allignment
batch_type acts_all::UTILP0_allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type acts_all::UTILP0_allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type acts_all::UTILP0_allignlower_KV2(batch_type val){
	batch_type fac = val / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type acts_all::UTILP0_allignhigher_KV2(batch_type val){
	batch_type fac = (val + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type acts_all::UTILP0_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR){
	#pragma HLS INLINE
	batch_type fac = (val + (_FACTOR - 1)) / _FACTOR;
	return (fac * _FACTOR);
}
batch_type acts_all::UTILP0_allignlower_FACTOR(batch_type val, unsigned int _FACTOR){
	batch_type fac = val / _FACTOR;
	return (fac * _FACTOR);
}

// bit manipulation
unsigned int acts_all::UTILP0_GETMASK_UINT(uint32_type index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int acts_all::UTILP0_READFROM_UINT(uint32_type data, unsigned int index, unsigned int size){ 
	unsigned int res = 0;
	res = (((data) & UTILP0_GETMASK_UINT((index), (size))) >> (index)); 
	return res;
}
void acts_all::UTILP0_WRITETO_UINT(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~UTILP0_GETMASK_UINT((index), (size)));
	unsigned int C = ((tempdata) & (B));
	unsigned int D = (C) | A;
	*data = D;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"WRITETO_ULONG. index: "<<index<<", size: "<<size<<", value: "<<value<<endl;
	cout<<"WRITETO_ULONG. tempdata"<<endl; actsutilityobj->ULONGTOBINARY(tempdata);
	cout<<"WRITETO_ULONG. A"<<endl; actsutilityobj->ULONGTOBINARY(A);
	cout<<"WRITETO_ULONG. B (~mask)"<<endl; actsutilityobj->ULONGTOBINARY(B);
	cout<<"WRITETO_ULONG. C"<<endl; actsutilityobj->ULONGTOBINARY(C);
	cout<<"WRITETO_ULONG. D (result)"<<endl; actsutilityobj->ULONGTOBINARY(D);
	#endif
	return; 
}
unsigned int acts_all::UTILP0_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	#ifdef _WIDEWORD
	res = data.range(index + size - 1, index); 
	#else
	res = UTILP0_READFROM_UINT(data, index, size);
	#endif
	return res;
}
void acts_all::UTILP0_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	data->range(index + size - 1, index) = value;
	#else
	UTILP0_WRITETO_UINT(data, index, size, value);
	#endif
	return; 
}
unsigned int acts_all::UTILP0_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	res = UTILP0_READFROM_UINT(data, index, size);
	return res;
}
void acts_all::UTILP0_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	UTILP0_WRITETO_UINT(data, index, size, value);
	return; 
}

// converters
keyvalue_t acts_all::UTILP0_GETKV(keyvalue_buffer_t data){
	#pragma HLS INLINE
	keyvalue_t res;
	#ifdef _WIDEWORD
	res.key = data.range(SIZEOF_KEY - 1, 0);
	res.value = data.range(SIZEOF_KEY + SIZEOF_VALUE - 1, SIZEOF_KEY);
	#else 
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyvalue_buffer_t acts_all::UTILP0_GETKV(keyvalue_t data){
	#pragma HLS INLINE
	keyvalue_buffer_t res;
	#ifdef _WIDEWORD
	res.range(SIZEOF_KEY - 1, 0) = data.key;
	res.range(SIZEOF_KEY + SIZEOF_VALUE - 1, SIZEOF_KEY) = data.value;
	#else
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyy_t acts_all::UTILP0_GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_KEY - 1, 0);
	#else
	return data;
	#endif
}
value_t acts_all::UTILP0_GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(SIZEOF_VALUE - 1, 0);
	#else
	return data;
	#endif
}
keyy_t acts_all::UTILP0_GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[v].key;	
	#endif
}		
unsigned int acts_all::UTILP0_GETLOCALVID(unsigned int vid, unsigned int instid){ 
	#pragma HLS INLINE
	return (vid - instid) / NUM_PEs;
}
unsigned int acts_all::UTILP0_GETREALVID(unsigned int lvid, unsigned int instid){ 
	#pragma HLS INLINE
	return (lvid * NUM_PEs) + instid;
}
unsigned int acts_all::UTILP0_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams){ 
	#pragma HLS INLINE
	// return globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	return PROCESSPARTITIONSZ_KVS2;
}

unsigned int acts_all::UTILP0_GetData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index){
	#pragma HLS INLINE
	unsigned int data = 0;
	#ifdef _WIDEWORD
	unsigned int row = index / VECTOR2_SIZE;
	unsigned int col = index % VECTOR2_SIZE;
 if(col == 0){
		data = kvdram[offset_kvs + row].range(31, 0); 
	}
else if(col == 1){
		data = kvdram[offset_kvs + row].range(63, 32); 
	}
else if(col == 2){
		data = kvdram[offset_kvs + row].range(95, 64); 
	}
else if(col == 3){
		data = kvdram[offset_kvs + row].range(127, 96); 
	}
else if(col == 4){
		data = kvdram[offset_kvs + row].range(159, 128); 
	}
else if(col == 5){
		data = kvdram[offset_kvs + row].range(191, 160); 
	}
else if(col == 6){
		data = kvdram[offset_kvs + row].range(223, 192); 
	}
else if(col == 7){
		data = kvdram[offset_kvs + row].range(255, 224); 
	}
else if(col == 8){
		data = kvdram[offset_kvs + row].range(287, 256); 
	}
else if(col == 9){
		data = kvdram[offset_kvs + row].range(319, 288); 
	}
else if(col == 10){
		data = kvdram[offset_kvs + row].range(351, 320); 
	}
else if(col == 11){
		data = kvdram[offset_kvs + row].range(383, 352); 
	}
else if(col == 12){
		data = kvdram[offset_kvs + row].range(415, 384); 
	}
else if(col == 13){
		data = kvdram[offset_kvs + row].range(447, 416); 
	}
else if(col == 14){
		data = kvdram[offset_kvs + row].range(479, 448); 
	}
else if(col == 15){
		data = kvdram[offset_kvs + row].range(511, 480); 
	}
	else {} 
	#else 
	unsigned int loc = index / 2;
	if(index % 2 == 0){ data = kvdram[offset_kvs + (loc / VECTOR_SIZE)].data[loc % VECTOR_SIZE].key; } 
	else { data = kvdram[offset_kvs + (loc / VECTOR_SIZE)].data[loc % VECTOR_SIZE].value; }	
	#endif 
	return data;
}
void acts_all::UTILP0_SetData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	unsigned int row = index / VECTOR2_SIZE;
	unsigned int col = index % VECTOR2_SIZE;
 if(col == 0){
		kvdram[offset_kvs + row].range(31, 0) = data; 
	}
else if(col == 1){
		kvdram[offset_kvs + row].range(63, 32) = data; 
	}
else if(col == 2){
		kvdram[offset_kvs + row].range(95, 64) = data; 
	}
else if(col == 3){
		kvdram[offset_kvs + row].range(127, 96) = data; 
	}
else if(col == 4){
		kvdram[offset_kvs + row].range(159, 128) = data; 
	}
else if(col == 5){
		kvdram[offset_kvs + row].range(191, 160) = data; 
	}
else if(col == 6){
		kvdram[offset_kvs + row].range(223, 192) = data; 
	}
else if(col == 7){
		kvdram[offset_kvs + row].range(255, 224) = data; 
	}
else if(col == 8){
		kvdram[offset_kvs + row].range(287, 256) = data; 
	}
else if(col == 9){
		kvdram[offset_kvs + row].range(319, 288) = data; 
	}
else if(col == 10){
		kvdram[offset_kvs + row].range(351, 320) = data; 
	}
else if(col == 11){
		kvdram[offset_kvs + row].range(383, 352) = data; 
	}
else if(col == 12){
		kvdram[offset_kvs + row].range(415, 384) = data; 
	}
else if(col == 13){
		kvdram[offset_kvs + row].range(447, 416) = data; 
	}
else if(col == 14){
		kvdram[offset_kvs + row].range(479, 448) = data; 
	}
else if(col == 15){
		kvdram[offset_kvs + row].range(511, 480) = data; 
	}
	else {} 
	#else 
	unsigned int loc = index / 2;
	if(index % 2 == 0){ kvdram[offset_kvs + (loc / VECTOR_SIZE)].data[loc % VECTOR_SIZE].key = data; } 
	else { kvdram[offset_kvs + (loc / VECTOR_SIZE)].data[loc % VECTOR_SIZE].value = data; }	
	#endif 
	return;
}
unsigned int acts_all::UTILP0_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(31, 0);
	#else 
	return kvdram[offset_kvs].data[0].key;
	#endif 
}
void acts_all::UTILP0_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(31, 0) = data;
	#else 
	kvdram[offset_kvs].data[0].key = data;
	#endif 
	return;
}
unsigned int acts_all::UTILP0_GetSecondData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(63, 32);
	#else 
	return kvdram[offset_kvs].data[0].value;
	#endif 
}
void acts_all::UTILP0_SetSecondData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(63, 32) = data;
	#else 
	kvdram[offset_kvs].data[0].value = data;
	#endif 
	return;
}
void acts_all::UTILP0_GetDataset(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
	#pragma HLS INLINE
	#ifdef _WIDEWORD 
	datas[0] = kvdram[offset_kvs].range(31, 0); 
	datas[1] = kvdram[offset_kvs].range(63, 32); 
	datas[2] = kvdram[offset_kvs].range(95, 64); 
	datas[3] = kvdram[offset_kvs].range(127, 96); 
	datas[4] = kvdram[offset_kvs].range(159, 128); 
	datas[5] = kvdram[offset_kvs].range(191, 160); 
	datas[6] = kvdram[offset_kvs].range(223, 192); 
	datas[7] = kvdram[offset_kvs].range(255, 224); 
	datas[8] = kvdram[offset_kvs].range(287, 256); 
	datas[9] = kvdram[offset_kvs].range(319, 288); 
	datas[10] = kvdram[offset_kvs].range(351, 320); 
	datas[11] = kvdram[offset_kvs].range(383, 352); 
	datas[12] = kvdram[offset_kvs].range(415, 384); 
	datas[13] = kvdram[offset_kvs].range(447, 416); 
	datas[14] = kvdram[offset_kvs].range(479, 448); 
	datas[15] = kvdram[offset_kvs].range(511, 480); 
	#else 
	datas[0] = kvdram[offset_kvs].data[0].key;
	datas[1] = kvdram[offset_kvs].data[0].value; 
	datas[2] = kvdram[offset_kvs].data[1].key;
	datas[3] = kvdram[offset_kvs].data[1].value; 
	datas[4] = kvdram[offset_kvs].data[2].key;
	datas[5] = kvdram[offset_kvs].data[2].value; 
	datas[6] = kvdram[offset_kvs].data[3].key;
	datas[7] = kvdram[offset_kvs].data[3].value; 
	datas[8] = kvdram[offset_kvs].data[4].key;
	datas[9] = kvdram[offset_kvs].data[4].value; 
	datas[10] = kvdram[offset_kvs].data[5].key;
	datas[11] = kvdram[offset_kvs].data[5].value; 
	datas[12] = kvdram[offset_kvs].data[6].key;
	datas[13] = kvdram[offset_kvs].data[6].value; 
	datas[14] = kvdram[offset_kvs].data[7].key;
	datas[15] = kvdram[offset_kvs].data[7].value; 
	#endif
	return;
}
void acts_all::UTILP0_SetDataset(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
	#pragma HLS INLINE
	#ifdef _WIDEWORD 
	kvdram[offset_kvs].range(31, 0) = datas[0]; 
	kvdram[offset_kvs].range(63, 32) = datas[1]; 
	kvdram[offset_kvs].range(95, 64) = datas[2]; 
	kvdram[offset_kvs].range(127, 96) = datas[3]; 
	kvdram[offset_kvs].range(159, 128) = datas[4]; 
	kvdram[offset_kvs].range(191, 160) = datas[5]; 
	kvdram[offset_kvs].range(223, 192) = datas[6]; 
	kvdram[offset_kvs].range(255, 224) = datas[7]; 
	kvdram[offset_kvs].range(287, 256) = datas[8]; 
	kvdram[offset_kvs].range(319, 288) = datas[9]; 
	kvdram[offset_kvs].range(351, 320) = datas[10]; 
	kvdram[offset_kvs].range(383, 352) = datas[11]; 
	kvdram[offset_kvs].range(415, 384) = datas[12]; 
	kvdram[offset_kvs].range(447, 416) = datas[13]; 
	kvdram[offset_kvs].range(479, 448) = datas[14]; 
	kvdram[offset_kvs].range(511, 480) = datas[15]; 
	#else 
	kvdram[offset_kvs].data[0].key = datas[0];
	kvdram[offset_kvs].data[0].value = datas[1]; 
	kvdram[offset_kvs].data[1].key = datas[2];
	kvdram[offset_kvs].data[1].value = datas[3]; 
	kvdram[offset_kvs].data[2].key = datas[4];
	kvdram[offset_kvs].data[2].value = datas[5]; 
	kvdram[offset_kvs].data[3].key = datas[6];
	kvdram[offset_kvs].data[3].value = datas[7]; 
	kvdram[offset_kvs].data[4].key = datas[8];
	kvdram[offset_kvs].data[4].value = datas[9]; 
	kvdram[offset_kvs].data[5].key = datas[10];
	kvdram[offset_kvs].data[5].value = datas[11]; 
	kvdram[offset_kvs].data[6].key = datas[12];
	kvdram[offset_kvs].data[6].value = datas[13]; 
	kvdram[offset_kvs].data[7].key = datas[14];
	kvdram[offset_kvs].data[7].value = datas[15]; 
	#endif
	return;
}

// utilities
batch_type acts_all::UTILP0_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
	analysis_type analysis_treedepth = TREE_DEPTH;
	batch_type result;
	
	if(currentLOP == 0){ currentLOP = 1; }
	if(sourceORdest == SOURCE){ result = globalparams.ACTSPARAMS_FINALNUMPARTITIONS; }
	else if (sourceORdest == DEST){ result = globalparams.ACTSPARAMS_FINALNUMPARTITIONS / NUM_PARTITIONS; } // FIXME. use TREE_DEPTH for less lut?
	else {}
	for(step_type i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
batch_type acts_all::UTILP0_getrangeforeachllop(globalparams_t globalparams){
	analysis_type analysis_loop1 = TREE_DEPTH;
	unsigned int range = globalparams.SIZE_BATCHRANGE;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1	
		range = range / NUM_PARTITIONS;
	}
	return range;
}
buffer_type acts_all::UTILP0_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
buffer_type acts_all::UTILP0_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	buffer_type size_kvs = 0;
	size_kvs = realsize_kvs;
	return size_kvs;
}
void acts_all::UTILP0_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size){
	unsigned int analysis_size = NUM_PARTITIONS;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = UTILP0_allignhigher_KV(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}
void acts_all::UTILP0_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
	for(buffer_type i=1; i<size; i++){ 
		buffer[0][i].key = buffer[0][i-1].key + buffer[0][i-1].value + skipspacing; 
		buffer[1][i].key = buffer[1][i-1].key + buffer[1][i-1].value + skipspacing; 
		buffer[2][i].key = buffer[2][i-1].key + buffer[2][i-1].value + skipspacing; 
		buffer[3][i].key = buffer[3][i-1].key + buffer[3][i-1].value + skipspacing; 
		buffer[4][i].key = buffer[4][i-1].key + buffer[4][i-1].value + skipspacing; 
		buffer[5][i].key = buffer[5][i-1].key + buffer[5][i-1].value + skipspacing; 
		buffer[6][i].key = buffer[6][i-1].key + buffer[6][i-1].value + skipspacing; 
		buffer[7][i].key = buffer[7][i-1].key + buffer[7][i-1].value + skipspacing; 
	}
	return;
}
batch_type acts_all::UTILP0_get_num_source_partitions(step_type currentLOP){
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	if(currentLOP == 0){ currentLOP = 1; }
	batch_type pow = 1;
	for(step_type i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

globalparams_t acts_all::UTILP0_getglobalparams(uint512_dt * kvdram, unsigned int banksection){
	globalparams_t globalparams;
	
	if(banksection==0){ globalparams.DRAM_BASE_KVS = BASEOFFSET_MESSAGESDATA_KVS; }
	else if (banksection==1) { globalparams.DRAM_BASE_KVS = BASEOFFSET_MESSAGESDATA_KVS + (TOTALDRAMCAPACITY_KVS/2); }
	else {
		#ifdef _DEBUGMODE_CHECKS3
		cout<<"UTILP0_getglobalparams: NOT IMPLEMENTED. EXITING..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	// globalparams.DRAM_BASE_KVS = BASEOFFSET_MESSAGESDATA_KVS;
	
	unsigned int buffer[BLOCKRAM_SIZE];
	GETGLOBALPARAMS_LOOP1: for(unsigned int t=0; t<128; t++){
	#pragma HLS PIPELINE II=1
		buffer[t] = kvdram[globalparams.DRAM_BASE_KVS + t]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	
	globalparams.ENABLE_RUNKERNELCOMMAND = buffer[0];
	globalparams.ENABLE_PROCESSCOMMAND = buffer[1];
	globalparams.ENABLE_PARTITIONCOMMAND = buffer[2];
	globalparams.ENABLE_APPLYUPDATESCOMMAND = buffer[3];
	globalparams.ENABLE_ACTSGP = buffer[4];
	globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES = buffer[5];
	
	globalparams.BASEOFFSETKVS_MESSAGESDATA = globalparams.DRAM_BASE_KVS + buffer[6];
	globalparams.BASEOFFSETKVS_EDGESDATA = globalparams.DRAM_BASE_KVS + buffer[7];
	globalparams.BASEOFFSETKVS_VERTEXPTR = globalparams.DRAM_BASE_KVS + buffer[8];
	globalparams.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.DRAM_BASE_KVS + buffer[9];
	globalparams.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.DRAM_BASE_KVS + buffer[10];
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.DRAM_BASE_KVS + buffer[11];
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.DRAM_BASE_KVS + buffer[12];
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.DRAM_BASE_KVS + buffer[13];
	globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.DRAM_BASE_KVS + buffer[14];
	globalparams.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.DRAM_BASE_KVS + buffer[15]; //
	globalparams.BASEOFFSETKVS_KVDRAM = globalparams.DRAM_BASE_KVS + buffer[16];
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.DRAM_BASE_KVS + buffer[17];
	globalparams.BASEOFFSETKVS_OTHERINFOS = globalparams.DRAM_BASE_KVS + buffer[18];
	
	globalparams.SIZE_MESSAGESDRAM = buffer[19];
	globalparams.SIZE_EDGES = buffer[20];
	globalparams.SIZE_VERTEXPTRS = buffer[21];
	globalparams.SIZE_SRCVERTICESDATA = buffer[22];
	globalparams.SIZE_DESTVERTICESDATA = buffer[23];
	globalparams.SIZE_ACTIVEVERTICES = buffer[24];
	globalparams.SIZE_VERTICESDATAMASK = buffer[25];
	globalparams.SIZE_VERTICESPARTITIONMASK = buffer[26];
	globalparams.SIZE_KVSTATSDRAM = buffer[27];
	globalparams.SIZE_EDGESSTATSDRAM = buffer[28];
	globalparams.SIZE_KVDRAM = buffer[29];
	globalparams.SIZE_KVDRAMWORKSPACE = buffer[30];
	globalparams.SIZE_OTHERINFOS = buffer[31];
	globalparams.SIZEKVS2_PROCESSEDGESPARTITION = buffer[32];
	globalparams.SIZE_REDUCE = buffer[33];
	globalparams.SIZEKVS2_REDUCEPARTITION = buffer[34];
	globalparams.SIZEKVS_VMASKBUFFER = buffer[35];
	globalparams.SIZE_BATCHRANGE = buffer[36];
	globalparams.SIZE_RUN = buffer[37];

	globalparams.POW_MESSAGESDRAM = buffer[38];
	globalparams.POW_EDGES = buffer[39];
	globalparams.POW_VERTEXPTRS = buffer[40];
	globalparams.POW_SRCVERTICESDATA = buffer[41];
	globalparams.POW_DESTVERTICESDATA = buffer[42];
	globalparams.POW_ACTIVEVERTICES = buffer[43];
	globalparams.POW_VERTICESDATAMASK = buffer[44];
	globalparams.POW_KVSTATSDRAM = buffer[45];
	globalparams.POW_EDGESSTATSDRAM = buffer[46];
	globalparams.POW_KVDRAM = buffer[47];
	globalparams.POW_KVDRAMWORKSPACE = buffer[48];
	globalparams.POW_REDUCE = buffer[49];
	globalparams.POW_BATCHRANGE = buffer[50];
	
	globalparams.NUM_PROCESSEDGESPARTITIONS = buffer[51];
	globalparams.NUM_REDUCEPARTITIONS = buffer[52];
	
	globalparams.ALGORITHMINFO_GRAPHITERATIONID = buffer[53];
	globalparams.ALGORITHMINFO_GRAPHALGORITHMID = buffer[54];

	globalparams.ACTSPARAMS_BEGINLOP = buffer[55];
	globalparams.ACTSPARAMS_NUMLOPS = buffer[56];
	globalparams.ACTSPARAMS_TREEDEPTH = buffer[57];
	globalparams.ACTSPARAMS_FINALNUMPARTITIONS = buffer[58];
	globalparams.ACTSPARAMS_SRCVOFFSET = buffer[59];
	globalparams.ACTSPARAMS_SRCVSIZE = buffer[60];
	globalparams.ACTSPARAMS_DESTVOFFSET = buffer[61];
	globalparams.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = buffer[62];
	globalparams.ACTSPARAMS_INSTID = buffer[63];
	
	globalparams.ACTSPARAMS_NUM_EDGE_BANKS = buffer[MESSAGES_ACTSPARAMS_NUM_EDGE_BANKS];
	globalparams.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM = buffer[MESSAGES_ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM];
	
	unsigned int validitytest2 = buffer[MESSAGES_DUMMYCHKPOINT]; // buffer[66];
	for(unsigned int m=0; m<MESSAGES_RETURNVALUES_SIZE; m++){ globalparams.RETURNVALUES[m] = buffer[MESSAGES_RETURNVALUES + m]; }
	for(unsigned int m=0; m<MESSAGES_MAILBOX_SIZE; m++){ globalparams.MAILBOX[m] = buffer[MESSAGES_MAILBOX + m]; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	if(GraphAlgo == BFS || GraphAlgo == SSSP){ globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS = ALGORITHMCLASS_NOTALLVERTEXISACTIVE; }
	else{ globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS = ALGORITHMCLASS_ALLVERTEXISACTIVE; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalparameters("acts_util::getglobalparams:: printing global parameters", globalparams);
	#endif
	
	if(validitytest2 != 70707070){
		#ifdef _DEBUGMODE_CHECKS3
		cout<<"acts_util:: _getglobalparams: ERROR: validitytest2("<<validitytest2<<") != 70707070. MISALLIGNMENT SOMEWHERE. EXITING..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	
	globalparams.VARS_WORKBATCH = 0;
	return globalparams;
}

sweepparams_t acts_all::UTILP0_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
	sweepparams_t sweepparams;
	batch_type sourceskipsize = UTILP0_getskipsize(currentLOP, SOURCE, globalparams);
	
	sweepparams.currentLOP = currentLOP;

	if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; }
	else { sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; }

	unsigned int div = globalparams.SIZE_BATCHRANGE;
	analysis_type analysis_lc = TREE_DEPTH;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_lc avg=analysis_lc	
		div = div / NUM_PARTITIONS;
	}
	
	sweepparams.upperlimit = globalparams.ACTSPARAMS_DESTVOFFSET + (source_partition * sourceskipsize * div); // POW_BATCHRANGE
	sweepparams.upperpartition = (sweepparams.upperlimit - globalparams.ACTSPARAMS_DESTVOFFSET) >> (globalparams.POW_BATCHRANGE - (NUM_PARTITIONS_POW * currentLOP)); //

	sweepparams.source_partition = source_partition;
	return sweepparams;
}
travstate_t acts_all::UTILP0_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
	travstate_t travstate;
	if(enable == OFF){ return travstate; }
	keyvalue_t keyvalue;
	keyvalue_t nextkeyvalue;
	
	if(currentLOP == 0){ keyvalue.key = 0; }
	else if(currentLOP == 1){ keyvalue.key = 0; }
	else {
 if(globalparams.VARS_WORKBATCH == 0){
			#ifdef _WIDEWORD
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(31, 0); 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(63, 32); 
			#else 
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[0].key; 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[0].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 1){
			#ifdef _WIDEWORD
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(95, 64); 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(127, 96); 
			#else 
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[1].key; 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[1].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 2){
			#ifdef _WIDEWORD
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(159, 128); 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(191, 160); 
			#else 
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[2].key; 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[2].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 3){
			#ifdef _WIDEWORD
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(223, 192); 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(255, 224); 
			#else 
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[3].key; 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[3].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 4){
			#ifdef _WIDEWORD
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(287, 256); 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(319, 288); 
			#else 
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[4].key; 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[4].value; 
			#endif 
		}
		else {
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"UTILP0_gettravstate: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	
	if(currentLOP == 0){ nextkeyvalue.key = globalparams.SIZE_RUN; }
	else if(currentLOP == 1){ nextkeyvalue.key = globalparams.SIZE_RUN; }
	else { nextkeyvalue.key = keyvalue.key + keyvalue.value; }
		
	travstate.begin_kvs = keyvalue.key / VECTOR_SIZE; 
	travstate.end_kvs = (nextkeyvalue.key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	
	travstate.size_kvs = travstate.end_kvs - travstate.begin_kvs;
	travstate.skip_kvs = SRCBUFFER_SIZE;
	travstate.i_kvs = travstate.begin_kvs; 
	return travstate;	
}
void acts_all::UTILP0_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value){			
	if(enable == OFF){ return; }
	#ifdef _WIDEWORD
	kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset].range(31, 0) = value; // 64; 
	kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset].range(63, 32) = value; // 64;
	#else
	kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset].data[0].key = value; // 64; 
	kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset].data[0].value = value; // 64; 
	#endif
	return;	
}
partition_type acts_all::UTILP0_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow){
	partition_type partition;
	keyvalue_t thiskeyvalue = UTILP0_GETKV(keyvalue);
	
	#ifdef TREEDEPTHISONE
	if(thiskeyvalue.value == UTILP0_GETV(INVALIDDATA)){ partition = 0; } 
	else { keyy_t lkey = thiskeyvalue.key - upperlimit; partition = (lkey % NUM_PARTITIONS); }
	#else 
	if(thiskeyvalue.value == UTILP0_GETV(INVALIDDATA)){ partition = 0; } 
	else {
		keyy_t lkey = thiskeyvalue.key - upperlimit;
		if(mode == ACTSREDUCEMODE){ partition = (lkey % NUM_PARTITIONS); } else { partition = (lkey >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))); }
	}
	#endif 

	if(partition >= MAX_NUM_PARTITIONS){ 
		#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS3 // ENABLE_VOICEOUTKERNELERRORS
			cout<<"acts_util::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", thiskeyvalue.key: "<<thiskeyvalue.key<<", thiskeyvalue.value: "<<thiskeyvalue.value<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)))<<", INVALIDDATA: "<<INVALIDDATA<<", UTILP0_GETV(INVALIDDATA): "<<UTILP0_GETV(INVALIDDATA)<<endl;			 
			#endif 
		exit(EXIT_FAILURE); 
		#else
		partition = 0;
		// partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1)));
		#endif 
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->globalstats_counterrorsingetpartition(1);
		#endif 
	}
	return partition;
}

// resets 
void acts_all::UTILP0_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void acts_all::UTILP0_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void acts_all::UTILP0_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i] = resetval; 
	}
}
void acts_all::UTILP0_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; 
		buffer[i].value = resetval; 
	}
	return;
}
void acts_all::UTILP0_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"..................... UTILP0_resetkvstatvalues: resetting global stats..."<<endl;
	#endif 
	unsigned int totalnumpartitionsb4last = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ RESETKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (NUM_PARTITIONS_POW * k)); }} else { totalnumpartitionsb4last = 1 + NUM_PARTITIONS; }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(127, 96) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(191, 160) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(255, 224) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(319, 288) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(383, 352) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(447, 416) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(511, 480) = 0; 
		#else 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[1].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[2].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[3].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[4].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[5].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[6].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[7].value = 0; 
		#endif
	}
	return;
}
void acts_all::UTILP0_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams){
	for(unsigned int k=0; k<size_kvs; k++){
	#pragma HLS PIPELINE II=1 // CRITICAL NEWCHANGE.
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(127, 96) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(191, 160) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(255, 224) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(319, 288) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(383, 352) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(447, 416) = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(511, 480) = 0; 
		#else 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[1].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[2].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[3].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[4].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[5].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[6].value = 0; 
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[7].value = 0; 
		#endif
	}
	return;
}
void acts_all::UTILP0_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ SAVEKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (NUM_PARTITIONS_POW * k)); }} else { totalnumpartitionsb4last = 1 + NUM_PARTITIONS; }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
 if(globalparams.VARS_WORKBATCH == 0){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 1){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(127, 96); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[1].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 2){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(191, 160); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[2].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 3){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(255, 224); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[3].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 4){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(319, 288); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[4].value; 
			#endif 
		}
		else {
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"UTILP0_gettravstate: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<") EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}
void acts_all::UTILP0_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _WIDEWORD
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#else
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}
void acts_all::UTILP0_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so1[0][p].key = 0;
		capsule_so1[0][p].value = 0;
		capsule_so1[1][p].key = 0;
		capsule_so1[1][p].value = 0;
		capsule_so1[2][p].key = 0;
		capsule_so1[2][p].value = 0;
		capsule_so1[3][p].key = 0;
		capsule_so1[3][p].value = 0;
		capsule_so1[4][p].key = 0;
		capsule_so1[4][p].value = 0;
		capsule_so1[5][p].key = 0;
		capsule_so1[5][p].value = 0;
		capsule_so1[6][p].key = 0;
		capsule_so1[6][p].value = 0;
		capsule_so1[7][p].key = 0;
		capsule_so1[7][p].value = 0;
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}
void acts_all::UTILP0_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}
