#include "acts_all.h"

#ifdef SW
acts_all::acts_all(mydebug * _mydebugobj){
	actsutilityobj = new actsutility();
	mydebugobj = _mydebugobj;
}
acts_all::~acts_all(){}
#endif
#ifdef SWEMU
// actsutility * actsutilityobj = new actsutility();
#endif 

#ifdef CONFIG_ENABLECLASS_ACTS_UTIL
// basic
unsigned int UTILS_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int UTILS_amax(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int UTILS_aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}
uint32_type UTILS_amin2(uint32_type val1, uint32_type val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

// allignment
batch_type UTILS_allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type UTILS_allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type UTILS_allignlower_KV2(batch_type val){
	batch_type fac = val / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type UTILS_allignhigher_KV2(batch_type val){
	batch_type fac = (val + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type UTILS_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR){
	#pragma HLS INLINE
	batch_type fac = (val + (_FACTOR - 1)) / _FACTOR;
	return (fac * _FACTOR);
}
batch_type UTILS_allignlower_FACTOR(batch_type val, unsigned int _FACTOR){
	batch_type fac = val / _FACTOR;
	return (fac * _FACTOR);
}

unsigned int UTILS_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(unsigned int tree_depth){
	return (1 << (OPT_NUM_PARTITIONS_POW * (tree_depth-1)));
}

// bit manipulation
unsigned int UTILS_GETMASK_UINT(uint32_type index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int UTILS_READFROM_UINT(uint32_type data, unsigned int index, unsigned int size){ 
	unsigned int res = 0;
	res = (((data) & UTILS_GETMASK_UINT((index), (size))) >> (index)); 
	return res;
}
void UTILS_WRITETO_UINT(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~UTILS_GETMASK_UINT((index), (size)));
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
unsigned int UTILS_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	#ifdef _WIDEWORD
	res = data.range(index + size - 1, index); 
	#else
	res = UTILS_READFROM_UINT(data, index, size);
	#endif
	return res;
}
void UTILS_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	data->range(index + size - 1, index) = value;
	#else
	UTILS_WRITETO_UINT(data, index, size, value);
	#endif
	return; 
}
unsigned int UTILS_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	res = UTILS_READFROM_UINT(data, index, size);
	return res;
}
void UTILS_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	UTILS_WRITETO_UINT(data, index, size, value);
	return; 
}

// converters
keyvalue_t UTILS_GETKV(keyvalue_buffer_t data){
	#pragma HLS INLINE
	keyvalue_t res;
	#ifdef _WIDEWORD_FOR_KV
	res.key = data.range(SIZEOF_KEY - 1, 0);
	res.value = data.range(SIZEOF_KEY + SIZEOF_VALUE - 1, SIZEOF_KEY);
	#else 
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyvalue_buffer_t UTILS_GETKV(keyvalue_t data){
	#pragma HLS INLINE
	keyvalue_buffer_t res;
	#ifdef _WIDEWORD_FOR_KV
	res.range(SIZEOF_KEY - 1, 0) = data.key;
	res.range(SIZEOF_KEY + SIZEOF_VALUE - 1, SIZEOF_KEY) = data.value;
	#else
	res.key = data.key;
	res.value = data.value;	
	#endif 
	return res;
}
keyy_t UTILS_GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_KEY - 1, 0);
	#else
	return data;
	#endif
}
value_t UTILS_GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_VALUE - 1, 0);
	#else
	return data;
	#endif
}
keyy_t UTILS_GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[v].key;	
	#endif
}		
unsigned int UTILS_GETLOCALVID(unsigned int vid, unsigned int instid){ 
	#pragma HLS INLINE
	return (vid - instid) / NUM_PEs;
}
unsigned int UTILS_GETREALVID(unsigned int lvid, unsigned int instid){ 
	#pragma HLS INLINE
	return (lvid * NUM_PEs) + instid;
}
unsigned int UTILS_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams){ 
	#pragma HLS INLINE
	return globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	// return PROCESSPARTITIONSZ_KVS2;
}

unsigned int UTILS_GetData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index){
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
void UTILS_SetData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data){
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
unsigned int UTILS_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(31, 0);
	#else 
	return kvdram[offset_kvs].data[0].key;
	#endif 
}
void UTILS_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(31, 0) = data;
	#else 
	kvdram[offset_kvs].data[0].key = data;
	#endif 
	return;
}
void UTILS_SetFirstDatas(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int * data, unsigned int size){
	#pragma HLS INLINE
	for(unsigned int i=0; i<size; i++){
		#ifdef _WIDEWORD
		kvdram[offset_kvs + i].range(31, 0) = data[i];
		#else 
		kvdram[offset_kvs + i].data[0].key = data[i];
		#endif 
	}
	return;
}
unsigned int UTILS_GetSecondData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(63, 32);
	#else 
	return kvdram[offset_kvs].data[0].value;
	#endif 
}
void UTILS_SetSecondData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(63, 32) = data;
	#else 
	kvdram[offset_kvs].data[0].value = data;
	#endif 
	return;
}
void UTILS_GetDataset(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
void UTILS_SetDataset(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
batch_type UTILS_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
	batch_type result;
	
	if(currentLOP == 0){ currentLOP = 1; }
	if(sourceORdest == SOURCE){ result = globalparams.ACTSPARAMS_FINALNUMPARTITIONS; }
	else if (sourceORdest == DEST){ result = globalparams.ACTSPARAMS_FINALNUMPARTITIONS / OPT_NUM_PARTITIONS; } // FIXME. use TREE_DEPTH for less lut?
	else {}
	for(step_type i=0; i<(currentLOP-1); i++){
		result = result / OPT_NUM_PARTITIONS;
	}
	return result;
}
batch_type UTILS_getrangeforeachllop(globalparams_t globalparams){
	unsigned int range = globalparams.SIZE_BATCHRANGE;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
		range = range / OPT_NUM_PARTITIONS;
	}
	return range;
}
buffer_type UTILS_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
buffer_type UTILS_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	buffer_type size_kvs = 0;
	size_kvs = realsize_kvs;
	return size_kvs;
}
void UTILS_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size){
	unsigned int analysis_size = OPT_NUM_PARTITIONS;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = UTILS_allignhigher_KV(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}
void UTILS_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
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
batch_type UTILS_get_num_source_partitions(step_type currentLOP){
	if(currentLOP == 0){ currentLOP = 1; }
	batch_type pow = 1;
	for(step_type i=0; i<(currentLOP-1); i++){
		pow = pow * OPT_NUM_PARTITIONS;
	}
	return pow;
}

globalparams_t UTILS_getglobalparams(uint512_dt * kvdram, unsigned int banksection){
	globalparams_t globalparams;
	
	// cout<<"UTILS_getglobalparams: SEEN 1: globalparams.DRAM_BASE_KVS("<<globalparams.DRAM_BASE_KVS<<") + MESSAGES_ACTSPARAMS_TOTALDRAMCAPACITY_KVS("<<MESSAGES_ACTSPARAMS_TOTALDRAMCAPACITY_KVS<<"): "<<globalparams.DRAM_BASE_KVS + MESSAGES_ACTSPARAMS_TOTALDRAMCAPACITY_KVS<<", (((1 << 28) / 4) / 16): "<<(((1 << 28) / 4) / 16)<<endl;
	
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("UTILS_getglobalparams 23", 0 + MESSAGES_ACTSPARAMS_TOTALDRAMCAPACITY_KVS, (((1 << 28) / 4) / 16), globalparams.DRAM_BASE_KVS, MESSAGES_ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp);
	#endif
	// unsigned int totaldramcapacity_kvs = kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ACTSPARAMS_TOTALDRAMCAPACITY_KVS]
	unsigned int totaldramcapacity_kvs = kvdram[0 + MESSAGES_ACTSPARAMS_TOTALDRAMCAPACITY_KVS]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
							
	// cout<<"UTILS_getglobalparams: SEEN 2..."<<endl;
	
	if(banksection==0){ globalparams.DRAM_BASE_KVS = BASEOFFSET_MESSAGESDATA_KVS; }
	// else if (banksection==1) { globalparams.DRAM_BASE_KVS = BASEOFFSET_MESSAGESDATA_KVS + (globalparams.ACTSPARAMS_TOTALDRAMCAPACITY_KVS / 2); }
	else if (banksection==1) { globalparams.DRAM_BASE_KVS = BASEOFFSET_MESSAGESDATA_KVS + (totaldramcapacity_kvs / 2); }
	else {
		#ifdef _DEBUGMODE_CHECKS3
		cout<<"UTILS_getglobalparams: NOT IMPLEMENTED. EXITING..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	
	// cout<<"UTILS_getglobalparams: SEEN 3..."<<endl;
	
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
	
	// cout<<"UTILS_getglobalparams: SEEN 4..."<<endl;
	
	globalparams.ENABLE_RUNKERNELCOMMAND = buffer[MESSAGES_ENABLE_RUNKERNELCOMMAND];
	globalparams.ENABLE_PROCESSCOMMAND = buffer[MESSAGES_ENABLE_PROCESSCOMMAND];
	globalparams.ENABLE_PARTITIONCOMMAND = buffer[MESSAGES_ENABLE_PARTITIONCOMMAND];
	globalparams.ENABLE_APPLYUPDATESCOMMAND = buffer[MESSAGES_ENABLE_APPLYUPDATESCOMMAND];
	globalparams.GPMODE = buffer[MESSAGES_GPMODE];
	globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES = buffer[MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES];

	globalparams.EVALUATION_ACTS_MEMORYLAYOUT = buffer[MESSAGES_EVALUATION_ACTS_MEMORYLAYOUT];
	globalparams.EVALUATION_ACTS_PARTITIONINGLOGIC = buffer[MESSAGES_EVALUATION_ACTS_PARTITIONINGLOGIC];
	globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC = buffer[MESSAGES_EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC];
	globalparams.EVALUATION_ACTS_HYBRIDLOGIC = buffer[MESSAGES_EVALUATION_ACTS_HYBRIDLOGIC];
	globalparams.EVALUATION_ACTS_VERYBARE = buffer[MESSAGES_EVALUATION_ACTS_VERYBARE];
	
	globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD = buffer[MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD];
	globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION = buffer[MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION];
	
	globalparams.BASEOFFSETKVS_MESSAGESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_MESSAGESDATA];
	globalparams.BASEOFFSETKVS_EDGESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGESDATA];
	globalparams.BASEOFFSETKVS_VERTEXPTR = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_VERTEXPTR];
	globalparams.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA];
	globalparams.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA];
	globalparams.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES];
	globalparams.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK];
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK];
	globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_STATSDRAM];
	globalparams.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGESSTATSDRAM]; //
	globalparams.BASEOFFSETKVS_KVDRAM = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_KVDRAM];
	globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE];
	globalparams.BASEOFFSETKVS_OTHERINFOS = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_ACTSMESSAGE_OTHERINFOS];
	
	globalparams.BUFFERBASEOFFSETKVS_VDATA1 = buffer[MESSAGES_BUFFERBASEOFFSETKVS_VDATA1];
	globalparams.BUFFERBASEOFFSETKVS_VDATA2 = buffer[MESSAGES_BUFFERBASEOFFSETKVS_VDATA2];
	globalparams.BUFFERBASEOFFSETKVS_VDATA3 = buffer[MESSAGES_BUFFERBASEOFFSETKVS_VDATA3];
	globalparams.BUFFERBASEOFFSETKVS_VDATA4 = buffer[MESSAGES_BUFFERBASEOFFSETKVS_VDATA4];
	globalparams.BUFFERBASEOFFSETKVS_VDATA5 = buffer[MESSAGES_BUFFERBASEOFFSETKVS_VDATA5];

	globalparams.SIZE_MESSAGESDRAM = buffer[MESSAGES_SIZE_MESSAGESDATA];
	globalparams.SIZE_EDGES = buffer[MESSAGES_SIZE_EDGES];
	globalparams.SIZE_VERTEXPTRS = buffer[MESSAGES_SIZE_VERTEXPTRS];
	globalparams.SIZE_SRCVERTICESDATA = buffer[MESSAGES_SIZE_SRCVERTICESDATA];
	globalparams.SIZE_DESTVERTICESDATA = buffer[MESSAGES_SIZE_DESTVERTICESDATA];
	globalparams.SIZE_ACTIVEVERTICES = buffer[MESSAGES_SIZE_ACTIVEVERTICES];
	globalparams.SIZE_VERTICESDATAMASK = buffer[MESSAGES_SIZE_VERTICESDATAMASK];
	globalparams.SIZE_VERTICESPARTITIONMASK = buffer[MESSAGES_SIZE_VERTICESPARTITIONMASK];
	globalparams.SIZE_KVSTATSDRAM = buffer[MESSAGES_SIZE_KVSTATSDRAM];
	globalparams.SIZE_EDGESSTATSDRAM = buffer[MESSAGES_SIZE_EDGESSTATSDRAM];
	globalparams.SIZE_KVDRAM = buffer[MESSAGES_SIZE_KVDRAM];
	globalparams.SIZE_KVDRAMWORKSPACE = buffer[MESSAGES_SIZE_KVDRAMWORKSPACE];
	globalparams.SIZE_OTHERINFOS = buffer[MESSAGES_SIZE_OTHERINFOS];
	globalparams.SIZEKVS2_PROCESSEDGESPARTITION = buffer[MESSAGES_SIZEKVS_PROCESSEDGESPARTITION];
	globalparams.SIZE_REDUCE = buffer[MESSAGES_SIZE_REDUCE];
	globalparams.SIZEKVS2_REDUCEPARTITION = buffer[MESSAGES_SIZEKVS_REDUCEPARTITION];
	globalparams.SIZEKVS_VMASKBUFFER = buffer[MESSAGES_SIZEKVS_VMASKBUFFER];
	globalparams.SIZE_BATCHRANGE = buffer[MESSAGES_SIZE_BATCHRANGE];
	globalparams.SIZE_RUN = buffer[MESSAGES_SIZE_RUN];

	globalparams.POW_MESSAGESDRAM = buffer[MESSAGES_POW_MESSAGESDRAM];
	globalparams.POW_EDGES = buffer[MESSAGES_POW_EDGES];
	globalparams.POW_VERTEXPTRS = buffer[MESSAGES_POW_VERTEXPTRS];
	globalparams.POW_SRCVERTICESDATA = buffer[MESSAGES_POW_SRCVERTICESDATA];
	globalparams.POW_DESTVERTICESDATA = buffer[MESSAGES_POW_DESTVERTICESDATA];
	globalparams.POW_ACTIVEVERTICES = buffer[MESSAGES_POW_ACTIVEVERTICES];
	globalparams.POW_VERTICESDATAMASK = buffer[MESSAGES_POW_VERTICESDATAMASK];
	globalparams.POW_KVSTATSDRAM = buffer[MESSAGES_POW_KVSTATSDRAM];
	globalparams.POW_EDGESSTATSDRAM = buffer[MESSAGES_POW_EDGESSTATSDRAM];
	globalparams.POW_KVDRAM = buffer[MESSAGES_POW_KVDRAM];
	globalparams.POW_KVDRAMWORKSPACE = buffer[MESSAGES_POW_KVDRAMWORKSPACE];
	globalparams.POW_REDUCE = buffer[MESSAGES_POW_REDUCE];
	globalparams.POW_BATCHRANGE = buffer[MESSAGES_POW_BATCHRANGE];

	globalparams.NUM_PROCESSEDGESPARTITIONS = buffer[MESSAGES_NUM_PROCESSEDGESPARTITIONS];
	globalparams.NUM_REDUCEPARTITIONS = buffer[MESSAGES_NUM_REDUCEPARTITIONS];
	
	globalparams.ALGORITHMINFO_GRAPHITERATIONID = buffer[MESSAGES_ALGORITHMINFO_GRAPHITERATIONID];
	globalparams.ALGORITHMINFO_GRAPHALGORITHMID = buffer[MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID];

	globalparams.ACTSPARAMS_BEGINLOP = buffer[MESSAGES_ACTSPARAMS_BEGINLOP];
	globalparams.ACTSPARAMS_NUMLOPS = buffer[MESSAGES_ACTSPARAMS_NUMLOPS];
	globalparams.ACTSPARAMS_TREEDEPTH = buffer[MESSAGES_ACTSPARAMS_TREEDEPTH];
	globalparams.ACTSPARAMS_FINALNUMPARTITIONS = buffer[MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS];
	globalparams.ACTSPARAMS_SRCVOFFSET = buffer[MESSAGES_ACTSPARAMS_SRCVOFFSET];
	globalparams.ACTSPARAMS_SRCVSIZE = buffer[MESSAGES_ACTSPARAMS_SRCVSIZE];
	globalparams.ACTSPARAMS_DESTVOFFSET = buffer[MESSAGES_ACTSPARAMS_DESTVOFFSET];
	globalparams.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = buffer[MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER];
	globalparams.ACTSPARAMS_INSTID = buffer[MESSAGES_ACTSPARAMS_INSTID];
	globalparams.ACTSPARAMS_NUM_EDGE_BANKS = buffer[MESSAGES_ACTSPARAMS_NUM_EDGE_BANKS]; 
	globalparams.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM = buffer[MESSAGES_ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM];
	globalparams.ACTSPARAMS_NUM_PARTITIONS = buffer[MESSAGES_ACTSPARAMS_NUM_PARTITIONS];
	globalparams.ACTSPARAMS_NUM_PARTITIONS_LOP1 = buffer[MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP1]; 
	globalparams.ACTSPARAMS_NUM_PARTITIONS_LOP2 = buffer[MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP2]; 
	globalparams.ACTSPARAMS_NUM_PARTITIONS_LOP3 = buffer[MESSAGES_ACTSPARAMS_NUM_PARTITIONS_LOP3]; 
	globalparams.ACTSPARAMS_POW_PARTITIONS = buffer[MESSAGES_ACTSPARAMS_POW_PARTITIONS]; 
	globalparams.ACTSPARAMS_POW_PARTITIONS_LOP1 = buffer[MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP1]; 
	globalparams.ACTSPARAMS_POW_PARTITIONS_LOP2 = buffer[MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP2]; 
	globalparams.ACTSPARAMS_POW_PARTITIONS_LOP3 = buffer[MESSAGES_ACTSPARAMS_POW_PARTITIONS_LOP3]; 
	globalparams.ACTSPARAMS_WORKBUFFER_SIZE = buffer[MESSAGES_ACTSPARAMS_WORKBUFFER_SIZE]; 
	globalparams.ACTSPARAMS_TOTALDRAMCAPACITY_KVS = buffer[MESSAGES_ACTSPARAMS_TOTALDRAMCAPACITY_KVS]; 
	globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO = buffer[MESSAGES_ACTSPARAMS_VPTR_SHRINK_RATIO]; 
	globalparams.ACTSPARAMS_READOFFLINESTATS = buffer[MESSAGES_ACTSPARAMS_READOFFLINESTATS]; 
	globalparams.ACTSPARAMS_UNUSED4 = buffer[MESSAGES_ACTSPARAMS_UNUSED4]; 
	
	globalparams.ACTSPARAMS_NUM_EDGE_BANKS = buffer[MESSAGES_ACTSPARAMS_NUM_EDGE_BANKS];
	globalparams.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM = buffer[MESSAGES_ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM];
	
	globalparams.DUMMYCHKPOINT = buffer[MESSAGES_DUMMYCHKPOINT]; // buffer[66];	

	globalparams.BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM];
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	if(GraphAlgo == BFS || GraphAlgo == SSSP){ globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS = ALGORITHMCLASS_NOTALLVERTEXISACTIVE; }
	else{ globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS = ALGORITHMCLASS_ALLVERTEXISACTIVE; }
	
	// cout<<"UTILS_getglobalparams: SEEN 5..."<<endl;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalparameters("acts_util::getglobalparams:: printing global parameters", globalparams);
	#endif
	
	if(globalparams.DUMMYCHKPOINT != 70707070){
		#ifdef _DEBUGMODE_CHECKS3
		cout<<"acts_util:: _getglobalparams: ERROR: globalparams.DUMMYCHKPOINT("<<globalparams.DUMMYCHKPOINT<<") != 70707070. MISALLIGNMENT SOMEWHERE. EXITING..."<<endl;
		cout<<"getglobalparams:: buffer[MESSAGES_ENABLE_RUNKERNELCOMMAND("<<MESSAGES_ENABLE_RUNKERNELCOMMAND<<")]: "<<buffer[MESSAGES_ENABLE_RUNKERNELCOMMAND]<<endl;
		cout<<"getglobalparams:: buffer[MESSAGES_ENABLE_PARTITIONCOMMAND("<<MESSAGES_ENABLE_PARTITIONCOMMAND<<")]: "<<buffer[MESSAGES_ENABLE_PARTITIONCOMMAND]<<endl;
		cout<<"getglobalparams:: buffer[MESSAGES_ENABLE_APPLYUPDATESCOMMAND("<<MESSAGES_ENABLE_APPLYUPDATESCOMMAND<<")]: "<<buffer[MESSAGES_ENABLE_APPLYUPDATESCOMMAND]<<endl;
		cout<<"getglobalparams:: buffer[MESSAGES_GPMODE("<<MESSAGES_GPMODE<<")]: "<<buffer[MESSAGES_GPMODE]<<endl;
		cout<<"getglobalparams:: buffer[MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES("<<MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES<<")]: "<<buffer[MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES]<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	
	// cout<<"UTILS_getglobalparams: SEEN 6..."<<endl;
	
	globalparams.VARS_WORKBATCH = 0;
	return globalparams;
}

sweepparams_t UTILS_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
	sweepparams_t sweepparams;
	batch_type sourceskipsize = UTILS_getskipsize(currentLOP, SOURCE, globalparams);
	
	sweepparams.currentLOP = currentLOP;

	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC == ON){ 
		if(currentLOP == 0 || (currentLOP % 2) == 1){ sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; }
		else { sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; }
	} else {
		if(currentLOP == 0 || currentLOP == 1){ sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; } 
		else { sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; }
	}
	
	unsigned int div = globalparams.SIZE_BATCHRANGE;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
		div = div / OPT_NUM_PARTITIONS;
	}
	
	sweepparams.upperlimit = globalparams.ACTSPARAMS_DESTVOFFSET + (source_partition * sourceskipsize * div); // POW_BATCHRANGE
	sweepparams.upperpartition = (sweepparams.upperlimit - globalparams.ACTSPARAMS_DESTVOFFSET) >> (globalparams.POW_BATCHRANGE - (OPT_NUM_PARTITIONS_POW * currentLOP)); //

	sweepparams.source_partition = source_partition;
	return sweepparams;
}
travstate_t UTILS_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
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
			cout<<"UTILS_gettravstate: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
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
	travstate.skip_kvs = MAX_SRCBUFFER_SIZE;
	travstate.i_kvs = travstate.begin_kvs; 
	return travstate;	
}
void UTILS_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value){			
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

partition_type UTILS_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, unsigned int tree_depth){
	partition_type partition;
	keyvalue_t thiskeyvalue = UTILS_GETKV(keyvalue);

	if(thiskeyvalue.value == UTILS_GETV(INVALIDDATA)){ partition = 0; } 
	else {
		keyy_t lkey = thiskeyvalue.key - upperlimit;
		if(mode == ACTSREDUCEMODE || tree_depth == 1){ partition = (lkey % OPT_NUM_PARTITIONS); } else { partition = (lkey >> (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP))); }
	}
	
	if(partition >= OPT_NUM_PARTITIONS){ 
		#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"acts_util::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", thiskeyvalue.key: "<<thiskeyvalue.key<<", thiskeyvalue.value: "<<thiskeyvalue.value<<", OPT_NUM_PARTITIONS: "<<OPT_NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP)))<<", INVALIDDATA: "<<INVALIDDATA<<", UTILS_GETV(INVALIDDATA): "<<UTILS_GETV(INVALIDDATA)<<endl;			 
			#endif 
		exit(EXIT_FAILURE); 
		#else
		partition = 0;
		#endif 
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->globalstats_counterrorsingetpartition(1);
		#endif 
	}
	return partition;
}
partition_type UTILS_getpartition2(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, globalparams_t globalparams){
	partition_type partition;
	keyvalue_t thiskeyvalue = UTILS_GETKV(keyvalue);
	
	if(thiskeyvalue.value == UTILS_GETV(INVALIDDATA)){ partition = 0; } 
	else {
		keyy_t lkey = thiskeyvalue.key - upperlimit;
		if(mode == ACTSREDUCEMODE || globalparams.ACTSPARAMS_TREEDEPTH == 1){ partition = (lkey % OPT_NUM_PARTITIONS); } 
		else { 
			if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC == ON){ partition = (lkey >> (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP))); } 
			else { partition = (lkey >> (batch_range_pow - (OPT_NUM_PARTITIONS_POW * (globalparams.ACTSPARAMS_TREEDEPTH-1)))); }
		}
	}
	
	if(partition >= MAX_NUM_PARTITIONS){ 
		#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"acts_util::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", thiskeyvalue.key: "<<thiskeyvalue.key<<", thiskeyvalue.value: "<<thiskeyvalue.value<<", OPT_NUM_PARTITIONS: "<<OPT_NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP)))<<", INVALIDDATA: "<<INVALIDDATA<<", UTILS_GETV(INVALIDDATA): "<<UTILS_GETV(INVALIDDATA)<<endl;			 
			#endif 
		exit(EXIT_FAILURE); 
		#else
		partition = 0;
		#endif 
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->globalstats_counterrorsingetpartition(1);
		#endif 
	}
	return partition;
}

// resets 
void UTILS_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void UTILS_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void UTILS_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i] = resetval; 
	}
}
void UTILS_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; 
		buffer[i].value = resetval; 
	}
	return;
}
void UTILS_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"..................... UTILS_resetkvstatvalues: resetting global stats..."<<endl;
	#endif 
	unsigned int totalnumpartitionsb4last = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ RESETKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (OPT_NUM_PARTITIONS_POW * k)); }} else { totalnumpartitionsb4last = 1 + OPT_NUM_PARTITIONS; }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
	// for(unsigned int k=0; k<globalparamsK.SIZE_KVSTATSDRAM; k++){
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
void UTILS_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams){
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
void UTILS_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ SAVEKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (OPT_NUM_PARTITIONS_POW * k)); }} else { totalnumpartitionsb4last = 1 + OPT_NUM_PARTITIONS; }
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
			cout<<"UTILS_gettravstate: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<") EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}
void UTILS_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _WIDEWORD
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#else
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}
void UTILS_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
	for(partition_type p=0; p<MAX_NUM_PARTITIONS; p++){
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
void UTILS_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
	for(partition_type p=0; p<OPT_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}
	
#endif 
#ifdef CONFIG_ENABLECLASS_ALGO_FUNCS
/* 
All algorithms: 
https://neo4j.com/docs/graph-data-science/current/algorithms/
https://iss.oden.utexas.edu/?p=projects/galois/analytics/betweenness_centrality

*/

value_t PROCESSS_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	value_t res = 0;
	#ifdef CONFIG_GOLDENKERNEL
	// res = udata + edgew;
	if(GraphAlgo == PAGERANK){
		res = udata + edgew;
	} else if(GraphAlgo == CF){
		res = udata;
	} else if(GraphAlgo == SPMV){
		res = udata;
	} else if(GraphAlgo == BFS){
		res = NAp;
	} else if(GraphAlgo == SSSP){
		res = udata + edgew;
	} else {
		res = NAp;
	}
	#else
		#if defined(PR_ALGORITHM)
			res = udata + edgew;
		#elif defined(CF_ALGORITHM)
			// source: https://mrmgroup.cs.princeton.edu/papers/taejun_micro16.pdf (Graphicionado)
			// process edge & reduce combined here (source: graphicionado paper)
			// --- Collaborative Filtering ---:
			// Process Edges: Executed in Reduce. ({Ew, Uprop} from source is sent to destination vertex)
			// Reduce: Function of (uprop, Ew, Vprop) is executed 
			// Apply: Function of (Vprop, Vtemp) is executed 
			// Finish:
			res = udata;
		#elif defined(HITS_ALGORITHM)	
			// https://www.geeksforgeeks.org/hyperlink-induced-topic-search-hits-algorithm-using-networxx-module-python/
			res = udata;
		#elif defined(SPMV_ALGORITHM)
			// source: https://www.baeldung.com/cs/graph-connected-components
			// source: https://cs.usm.maine.edu/~briggs/webPage/c161/projects/graphColoring.htmls
			// source: https://www.usenix.org/system/files/conference/osdi12/osdi12-final-126.pdf (GraphChi)
			// --- Connected Components ---:
			// Process Edges: Each vertex writes its id ("label") to its edges. 
			// Reduce: Vertex chooses the minimum label of its neighbors; 
			// Apply: A neighbor is scheduled only if a label in a connecting edge changes, which we implement by using selective scheduling. 
			// Finish: sets of vertices with equal labels are interpreted as connected components or communities, respectively.
			res = udata;
		#elif defined(BFS_ALGORITHM)
			res = NAp;
		#elif defined(SSSP_ALGORITHM)
			res = udata + edgew;
		#else
			NOT DEFINED.
		#endif
	#endif
	return res;
}

value_t REDUCES_reducefunc(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef CONFIG_GOLDENKERNEL
	// temp = UTILS_amax(vtemp, 1);
	unsigned int lamda = 1;
	unsigned int ew=1;
	if(GraphAlgo == PAGERANK){
		temp = vtemp + res;
	} else if(GraphAlgo == CF){
		temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
	} else if(GraphAlgo == HITS){
		temp = vtemp + res;
	} else if(GraphAlgo == SPMV){
		temp = UTILS_amin(vtemp, res);
	} else if(GraphAlgo == BFS){
		temp = UTILS_amin(vtemp, GraphIter);
	} else if(GraphAlgo == SSSP){
		temp = UTILS_amin(vtemp, res);
	} else {
		// NOT DEFINED
		temp = 0;
	}
	#else
		#if defined(PR_ALGORITHM)
			// Atomic Compute
			unsigned int alpha = 0.5;
			unsigned int vdeg = 1;
			temp = ((alpha + (1-alpha)*vtemp) / vdeg) + res;
			// temp = vtemp + res;
		#elif defined(CF_ALGORITHM)
			// Atomic Compute
			unsigned int ew = 1;
			unsigned int lamda = 0.5;
			unsigned int gamma = 0.5;
			temp = vdata + gamma*(vtemp + ((ew - vtemp*res)*res - lamda*vdata));
		#elif defined(HITS_ALGORITHM)
			// Atomic Add
			temp = vtemp + res;
			// atomicAdd(&hrank_next[src], arank_curr[dest]);
			// atomicAdd(&arank_next[dest], hrank_curr[src]);
		#elif defined(SPMV_ALGORITHM)
			// Atomic Min
			temp = UTILS_amin(vtemp, res);
		#elif defined(BFS_ALGORITHM)
			// Atomic Min
			#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
			temp = UTILS_amax(vtemp, 1);
			#else 
			temp = UTILS_amin(vtemp, GraphIter);
			#endif 
		#elif defined(SSSP_ALGORITHM)
			// Atomic Min
			temp = UTILS_amin(vtemp, res);
		#else 
			NOT DEFINED.
		#endif
	#endif
	return temp;
}

	
#endif 
#ifdef CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
// primitives
unsigned int MEMCAS_READVDATA(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILS_READBITSFROM_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA);
	#endif 
}
unsigned int MEMCAS_READVMASK(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILS_READBITSFROM_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK);
	#endif 
}
unsigned int MEMCAS_READVDATA2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILS_READBITSFROM_UINTV(wideword, offsetof_vdata, SIZEOF_VDATA);
	#endif 
}
unsigned int MEMCAS_READVMASK2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILS_READBITSFROM_UINTV(wideword, offsetof_vmask, SIZEOF_VMASK);
	#endif 
}

void MEMCAS_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILS_WRITEBITSTO_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void MEMCAS_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILS_WRITEBITSTO_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK, vmask);
	#endif 
	return;
}
void MEMCAS_WRITEVDATA2(keyvalue_vbuffer_t * wideword, value_t vdata, unsigned int offsetof_vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILS_WRITEBITSTO_UINTV(wideword, offsetof_vdata, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void MEMCAS_WRITEVMASK2(keyvalue_vbuffer_t * wideword, unit1_type vmask, unsigned int offsetof_vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILS_WRITEBITSTO_UINTV(wideword, offsetof_vmask, SIZEOF_VMASK, vmask);
	#endif 
	return;
}

unsigned int MEMCAS_READEDIR(unsigned int wideword){
	#pragma HLS INLINE
	return UTILS_READBITSFROM_UINTV(wideword, OFFSETOF_EDIR, SIZEOF_EDIR);
}

// non-primitives
vmdata_t MEMCAS_READFROMBUFFER_VDATAWITHVMASK(keyvalue_vbuffer_t bits_vector){
	#pragma HLS INLINE
	vmdata_t vmdata;
	vmdata.vdata = MEMCAS_READVDATA(bits_vector); 
	vmdata.vmask = MEMCAS_READVMASK(bits_vector); 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	if(vmdata.vmask == 1){ cout<<">>> MEMCAS_READFROMBUFFER_VDATAWITHVMASK:: ACTIVE VERTEX READ: @ index: "<<index<<endl; }
	#endif
	return vmdata;
}
void MEMCAS_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){				
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAS_WRITETOBUFFER_VDATAWITHVMASK:", index/2, MAX_BLOCKRAM_VDESTDATA_SIZE, index, NAp, NAp);
	#endif

	MEMCAS_WRITEVDATA(&bits_vector, vdata);
	MEMCAS_WRITEVMASK(&bits_vector, vmask);
	buffer[bufferoffset_kvs + index] = bits_vector;
	return;
}
void MEMCAS_WRITETODRAM_VDATAWITHVMASK(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAS_WRITETODRAM_VDATAWITHVMASK:", index, MAX_TOTALDRAMCAPACITY_KVS, index, NAp, NAp);
	#endif

	MEMCAS_WRITEVDATA(&bits_vector, vdata);
	MEMCAS_WRITEVMASK(&bits_vector, vmask);
	UTILS_SetData(kvdram, dramoffset_kvs, (index / VDATA_SHRINK_RATIO), bits_vector);
	return;
}

vmdata_t MEMCAS_READFROMBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t bits_vector){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAS_READFROMBUFFER_VDATAWITHVMASK2(220):", index % VDATA_SHRINK_RATIO, 32, index, NAp, NAp);
	#endif
	
	vmdata_t vmdata;
	tuple_t tup;
	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	
	#ifdef HWBITACCESSTYPE
	tup.A = MEMCAS_READVDATA2(bits_vector, offsetof_vdata); 
	tup.B = MEMCAS_READVMASK2(bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata); 
	vmdata.vdata = tup.A;
	vmdata.vmask = tup.B;
	#else
	vmdata.vdata = UTILS_SWREADBITSFROM_UINTV(bits_vector, offsetof_vdata, SIZEOF_VDATA);
	vmdata.vmask = UTILS_SWREADBITSFROM_UINTV(bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata, SIZEOF_DEST_VMASK);
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> MEMCAS_VDATAWITHVMASK2(224):: ACTIVE VERTEX READ: @ offsetof_vdata: "<<offsetof_vdata<<", vmdata.vdata: "<<vmdata.vdata<<", vmdata.vmask: "<<vmdata.vmask<<", sub_chunk_height: "<<(index / VDATA_SHRINK_RATIO)<<", index: "<<index<<endl; }						
	#endif
	return vmdata;
}
void MEMCAS_WRITETOBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAS_WRITETOBUFFER_VDATAWITHVMASK2:", index/VDATA_SHRINK_RATIO, MAX_BLOCKRAM_VDESTDATA_SIZE, index, NAp, NAp);
	#endif

	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	// keyvalue_vbuffer_t bits_vector = buffer[bufferoffset_kvs + (index / VDATA_SHRINK_RATIO)];
	
	#ifdef HWBITACCESSTYPE
	MEMCAS_WRITEVDATA2(&bits_vector, vdata, offsetof_vdata);
	MEMCAS_WRITEVMASK2(&bits_vector, vmdata, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata);
	#else
	UTILS_SWWRITEBITSTO_UINTV(&bits_vector, offsetof_vdata, SIZEOF_VDATA, vdata);
	UTILS_SWWRITEBITSTO_UINTV(&bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata, SIZEOF_DEST_VMASK, vmask);	
	#endif 
	
	buffer[bufferoffset_kvs + (index / VDATA_SHRINK_RATIO)] = bits_vector;
	return;
}
void MEMCAS_WRITETODRAM_VDATAWITHVMASK2(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAS_WRITETODRAM_VDATAWITHVMASK2:", index/VDATA_SHRINK_RATIO, MAX_TOTALDRAMCAPACITY_KVS, index, NAp, NAp);
	#endif

	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	
	#ifdef HWBITACCESSTYPE
	MEMCAS_WRITEVDATA2(&bits_vector, vdata, offsetof_vdata);
	MEMCAS_WRITEVMASK2(&bits_vector, vmask, BEGINOFFSETOF_VMASK + offsetof_vdata);
	#else
	UTILS_SWWRITEBITSTO_UINTV(&bits_vector, offsetof_vdata, SIZEOF_VDATA, vdata);
	UTILS_SWWRITEBITSTO_UINTV(&bits_vector, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK, vmask);	
	#endif 
	
	UTILS_SetData(kvdram, dramoffset_kvs, (index / VDATA_SHRINK_RATIO), bits_vector);
	return;
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_MEM_ACCESS
// -------------------- key values -------------------- //
fetchmessage_t MEMACCESSS_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	buffer_type chunk_size = UTILS_getchunksize_kvs(size_kvs, travstate, 0);
	
	fetchmessage.chunksize_kvs = chunk_size;
	fetchmessage.nextoffset_kvs = NAp;
	
	READKEYVALUES1_LOOP: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0;
		keyvalue_t mykeyvalue1;
		keyvalue_t mykeyvalue2;
		keyvalue_t mykeyvalue3;
		keyvalue_t mykeyvalue4;
		keyvalue_t mykeyvalue5;
		keyvalue_t mykeyvalue6;
		keyvalue_t mykeyvalue7;
		
		#ifdef _WIDEWORD
		mykeyvalue0.key = kvdram[offset_kvs + i].range(31, 0);
		mykeyvalue0.value = kvdram[offset_kvs + i].range(63, 32);
		mykeyvalue1.key = kvdram[offset_kvs + i].range(95, 64);
		mykeyvalue1.value = kvdram[offset_kvs + i].range(127, 96);
		mykeyvalue2.key = kvdram[offset_kvs + i].range(159, 128);
		mykeyvalue2.value = kvdram[offset_kvs + i].range(191, 160);
		mykeyvalue3.key = kvdram[offset_kvs + i].range(223, 192);
		mykeyvalue3.value = kvdram[offset_kvs + i].range(255, 224);
		mykeyvalue4.key = kvdram[offset_kvs + i].range(287, 256);
		mykeyvalue4.value = kvdram[offset_kvs + i].range(319, 288);
		mykeyvalue5.key = kvdram[offset_kvs + i].range(351, 320);
		mykeyvalue5.value = kvdram[offset_kvs + i].range(383, 352);
		mykeyvalue6.key = kvdram[offset_kvs + i].range(415, 384);
		mykeyvalue6.value = kvdram[offset_kvs + i].range(447, 416);
		mykeyvalue7.key = kvdram[offset_kvs + i].range(479, 448);
		mykeyvalue7.value = kvdram[offset_kvs + i].range(511, 480);
		#else 
		mykeyvalue0.key = kvdram[offset_kvs + i].data[0].key; 
		mykeyvalue0.value = kvdram[offset_kvs + i].data[0].value; 
		mykeyvalue1.key = kvdram[offset_kvs + i].data[1].key; 
		mykeyvalue1.value = kvdram[offset_kvs + i].data[1].value; 
		mykeyvalue2.key = kvdram[offset_kvs + i].data[2].key; 
		mykeyvalue2.value = kvdram[offset_kvs + i].data[2].value; 
		mykeyvalue3.key = kvdram[offset_kvs + i].data[3].key; 
		mykeyvalue3.value = kvdram[offset_kvs + i].data[3].value; 
		mykeyvalue4.key = kvdram[offset_kvs + i].data[4].key; 
		mykeyvalue4.value = kvdram[offset_kvs + i].data[4].value; 
		mykeyvalue5.key = kvdram[offset_kvs + i].data[5].key; 
		mykeyvalue5.value = kvdram[offset_kvs + i].data[5].value; 
		mykeyvalue6.key = kvdram[offset_kvs + i].data[6].key; 
		mykeyvalue6.value = kvdram[offset_kvs + i].data[6].value; 
		mykeyvalue7.key = kvdram[offset_kvs + i].data[7].key; 
		mykeyvalue7.value = kvdram[offset_kvs + i].data[7].value; 
		#endif 
		
		buffer[0][i] = UTILS_GETKV(mykeyvalue0);
		buffer[1][i] = UTILS_GETKV(mykeyvalue1);
		buffer[2][i] = UTILS_GETKV(mykeyvalue2);
		buffer[3][i] = UTILS_GETKV(mykeyvalue3);
		buffer[4][i] = UTILS_GETKV(mykeyvalue4);
		buffer[5][i] = UTILS_GETKV(mykeyvalue5);
		buffer[6][i] = UTILS_GETKV(mykeyvalue6);
		buffer[7][i] = UTILS_GETKV(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues read: offset_kvs: "<<offset_kvs<<", size_kvs: "<<size_kvs<<", chunk_size: "<<chunk_size<<endl;
	#endif
	return fetchmessage;
}

void MEMACCESSS_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], batch_type globalbaseaddress_kvs, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }
	analysis_type analysis_destpartitionsz = MAX_DESTBLOCKRAM_SIZE / globalparams.ACTSPARAMS_NUM_PARTITIONS;
	
	// actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 37--", (keyvalue_t *)globalcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS); 	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("savekeyvalues::localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	actsutilityobj->printvaluecount("savekeyvalues::localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	
	unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ _NUM_PARTITIONS = UTILS_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
	#endif 
	
	SAVEPARTITIONS_LOOP1: for(partition_type p=0; p<_NUM_PARTITIONS; p++){
		batch_type dramoffset_kvs = globalbaseaddress_kvs + ((globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE);
		buffer_type bramoffset_kvs = localcapsule[p].key / VECTOR_SIZE;
		buffer_type realsize_kvs = localcapsule[p].value / VECTOR_SIZE;
		if(localcapsule[p].key + localcapsule[p].value >= MAX_DESTBLOCKRAM_SIZE * VECTOR2_SIZE){ realsize_kvs = 0; } // ERROR CHECK
		buffer_type size_kvs = UTILS_getpartitionwritesz(realsize_kvs, bramoffset_kvs);
		// if(bramoffset_kvs + size_kvs >= MAX_DESTBLOCKRAM_SIZE){ size_kvs = 0; } // FIXME. STOPPED AT ERROR: orkut
		// if(globalcapsule[p].key + globalcapsule[p].value >= globalparams.SIZE_KVDRAM){ size_kvs = 0; }
		
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"savekeyvalues:: saving key-value partitions... (size_kvs: "<<size_kvs<<")"<<endl;
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS3
		if(globalcapsule[p].key + globalcapsule[p].value > globalparams.SIZE_KVDRAM + 1){ actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 337--", (keyvalue_t *)globalcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);  } 
		actsutilityobj->checkoutofbounds("savekeyvalues 23", bramoffset_kvs + size_kvs, MAX_DESTBLOCKRAM_SIZE + 1, p, bramoffset_kvs, size_kvs);
		actsutilityobj->checkoutofbounds("savekeyvalues 255", globalcapsule[p].key + globalcapsule[p].value, globalparams.SIZE_KVDRAM + 1, p, globalcapsule[p].key, globalcapsule[p].value);
		actsutilityobj->checkoutofbounds("savekeyvalues 256", localcapsule[p].key + localcapsule[p].value, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, p, localcapsule[p].key, localcapsule[p].value);
		#endif
		
		if(realsize_kvs > 0){
			SAVEPARTITIONS_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_destpartitionsz avg=analysis_destpartitionsz
			#pragma HLS PIPELINE II=1
				keyvalue_t mykeyvalue0 = UTILS_GETKV(buffer[0][bramoffset_kvs + i]);	
				keyvalue_t mykeyvalue1 = UTILS_GETKV(buffer[1][bramoffset_kvs + i]);	
				keyvalue_t mykeyvalue2 = UTILS_GETKV(buffer[2][bramoffset_kvs + i]);	
				keyvalue_t mykeyvalue3 = UTILS_GETKV(buffer[3][bramoffset_kvs + i]);	
				keyvalue_t mykeyvalue4 = UTILS_GETKV(buffer[4][bramoffset_kvs + i]);	
				keyvalue_t mykeyvalue5 = UTILS_GETKV(buffer[5][bramoffset_kvs + i]);	
				keyvalue_t mykeyvalue6 = UTILS_GETKV(buffer[6][bramoffset_kvs + i]);	
				keyvalue_t mykeyvalue7 = UTILS_GETKV(buffer[7][bramoffset_kvs + i]);	
				
				#ifdef _WIDEWORD
				kvdram[dramoffset_kvs + i].range(31, 0) = mykeyvalue0.key; 
				kvdram[dramoffset_kvs + i].range(63, 32) = mykeyvalue0.value; 
				kvdram[dramoffset_kvs + i].range(95, 64) = mykeyvalue1.key; 
				kvdram[dramoffset_kvs + i].range(127, 96) = mykeyvalue1.value; 
				kvdram[dramoffset_kvs + i].range(159, 128) = mykeyvalue2.key; 
				kvdram[dramoffset_kvs + i].range(191, 160) = mykeyvalue2.value; 
				kvdram[dramoffset_kvs + i].range(223, 192) = mykeyvalue3.key; 
				kvdram[dramoffset_kvs + i].range(255, 224) = mykeyvalue3.value; 
				kvdram[dramoffset_kvs + i].range(287, 256) = mykeyvalue4.key; 
				kvdram[dramoffset_kvs + i].range(319, 288) = mykeyvalue4.value; 
				kvdram[dramoffset_kvs + i].range(351, 320) = mykeyvalue5.key; 
				kvdram[dramoffset_kvs + i].range(383, 352) = mykeyvalue5.value; 
				kvdram[dramoffset_kvs + i].range(415, 384) = mykeyvalue6.key; 
				kvdram[dramoffset_kvs + i].range(447, 416) = mykeyvalue6.value; 
				kvdram[dramoffset_kvs + i].range(479, 448) = mykeyvalue7.key; 
				kvdram[dramoffset_kvs + i].range(511, 480) = mykeyvalue7.value; 
				#else
				kvdram[dramoffset_kvs + i].data[0].key = mykeyvalue0.key; 
				kvdram[dramoffset_kvs + i].data[0].value = mykeyvalue0.value; 
				kvdram[dramoffset_kvs + i].data[1].key = mykeyvalue1.key; 
				kvdram[dramoffset_kvs + i].data[1].value = mykeyvalue1.value; 
				kvdram[dramoffset_kvs + i].data[2].key = mykeyvalue2.key; 
				kvdram[dramoffset_kvs + i].data[2].value = mykeyvalue2.value; 
				kvdram[dramoffset_kvs + i].data[3].key = mykeyvalue3.key; 
				kvdram[dramoffset_kvs + i].data[3].value = mykeyvalue3.value; 
				kvdram[dramoffset_kvs + i].data[4].key = mykeyvalue4.key; 
				kvdram[dramoffset_kvs + i].data[4].value = mykeyvalue4.value; 
				kvdram[dramoffset_kvs + i].data[5].key = mykeyvalue5.key; 
				kvdram[dramoffset_kvs + i].data[5].value = mykeyvalue5.value; 
				kvdram[dramoffset_kvs + i].data[6].key = mykeyvalue6.key; 
				kvdram[dramoffset_kvs + i].data[6].value = mykeyvalue6.value; 
				kvdram[dramoffset_kvs + i].data[7].key = mykeyvalue7.key; 
				kvdram[dramoffset_kvs + i].data[7].value = mykeyvalue7.value; 
				#endif
			}
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten(globalparams.ACTSPARAMS_INSTID, realsize_kvs * VECTOR_SIZE);
			#endif
		}
	}
	SAVEPARTITIONS_LOOP2: for(partition_type p=0; p<_NUM_PARTITIONS; p++){ if(globalcapsule[p].key + globalcapsule[p].value + localcapsule[p].value < globalparams.SIZE_KVDRAM){ globalcapsule[p].value += localcapsule[p].value; }}
	
	#if defined(ENABLE_PERFECTACCURACY) && defined(_DEBUGMODE_CHECKS2)	
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_NUM_PARTITIONS-1; i++){ 
		if(globalcapsule[i].key + globalcapsule[i].value >= globalcapsule[i+1].key && globalcapsule[i].value > 0){ 
			cout<<"savekeyvalues::globalcapsule 33. ERROR. out of bounds. (globalcapsule["<<i<<"].key("<<globalcapsule[i].key<<") + globalcapsule["<<i<<"].value("<<globalcapsule[i].value<<") >= globalcapsule["<<i+1<<"].key("<<globalcapsule[i+1].key<<")) printing and exiting..."<<endl; 
			actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 34", (keyvalue_t *)globalcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS); 
			exit(EXIT_FAILURE); 
		}
	}
	if(globalcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].key + globalcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].value >= globalparams.SIZE_KVDRAM){
		cout<<"savekeyvalues::globalcapsule 36. ERROR. out of bounds. (globalcapsule["<<globalparams.ACTSPARAMS_NUM_PARTITIONS-1<<"].key("<<globalcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].key<<") + globalcapsule["<<globalparams.ACTSPARAMS_NUM_PARTITIONS-1<<"].value("<<globalcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].value<<") >= globalparams.SIZE_KVDRAM("<<globalparams.SIZE_KVDRAM<<")) printing and exiting..."<<endl; 
		actsutilityobj->printkeyvalues("savekeyvalues::globalcapsule 37", (keyvalue_t *)globalcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS); 
		exit(EXIT_FAILURE); 
	}
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"savekeyvalues:: keyvalues saved: offset_kvs from: "<<globalbaseaddress_kvs + ((globalcapsule[0].key + globalcapsule[0].value) / VECTOR_SIZE)<<endl;
	actsutilityobj->printkeyvalues("actsutility::savekeyvalues: globalcapsule.", (keyvalue_t *)globalcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	exit(EXIT_SUCCESS);
	return;
}

// -------------------- vdata -------------------- //
void MEMACCESSS_GetXYLayoutV(unsigned int s, vmdata_t vdata[VECTOR2_SIZE], vmdata_t vdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth){
	unsigned int s_ = s % VECTOR2_SIZE;
	unsigned int depths_tmp[VECTOR2_SIZE];
	
 if(s_==0){ 
		vdata2[0] = vdata[0]; 
		vdata2[1] = vdata[1]; 
		vdata2[2] = vdata[2]; 
		vdata2[3] = vdata[3]; 
		vdata2[4] = vdata[4]; 
		vdata2[5] = vdata[5]; 
		vdata2[6] = vdata[6]; 
		vdata2[7] = vdata[7]; 
		vdata2[8] = vdata[8]; 
		vdata2[9] = vdata[9]; 
		vdata2[10] = vdata[10]; 
		vdata2[11] = vdata[11]; 
		vdata2[12] = vdata[12]; 
		vdata2[13] = vdata[13]; 
		vdata2[14] = vdata[14]; 
		vdata2[15] = vdata[15]; 
		
		depths_tmp[0] = 0; 
		depths_tmp[1] = 1; 
		depths_tmp[2] = 2; 
		depths_tmp[3] = 3; 
		depths_tmp[4] = 4; 
		depths_tmp[5] = 5; 
		depths_tmp[6] = 6; 
		depths_tmp[7] = 7; 
		depths_tmp[8] = 8; 
		depths_tmp[9] = 9; 
		depths_tmp[10] = 10; 
		depths_tmp[11] = 11; 
		depths_tmp[12] = 12; 
		depths_tmp[13] = 13; 
		depths_tmp[14] = 14; 
		depths_tmp[15] = 15; 
	}
else if(s_==1){ 
		vdata2[0] = vdata[15]; 
		vdata2[1] = vdata[0]; 
		vdata2[2] = vdata[1]; 
		vdata2[3] = vdata[2]; 
		vdata2[4] = vdata[3]; 
		vdata2[5] = vdata[4]; 
		vdata2[6] = vdata[5]; 
		vdata2[7] = vdata[6]; 
		vdata2[8] = vdata[7]; 
		vdata2[9] = vdata[8]; 
		vdata2[10] = vdata[9]; 
		vdata2[11] = vdata[10]; 
		vdata2[12] = vdata[11]; 
		vdata2[13] = vdata[12]; 
		vdata2[14] = vdata[13]; 
		vdata2[15] = vdata[14]; 
		
		depths_tmp[0] = 15; 
		depths_tmp[1] = 0; 
		depths_tmp[2] = 1; 
		depths_tmp[3] = 2; 
		depths_tmp[4] = 3; 
		depths_tmp[5] = 4; 
		depths_tmp[6] = 5; 
		depths_tmp[7] = 6; 
		depths_tmp[8] = 7; 
		depths_tmp[9] = 8; 
		depths_tmp[10] = 9; 
		depths_tmp[11] = 10; 
		depths_tmp[12] = 11; 
		depths_tmp[13] = 12; 
		depths_tmp[14] = 13; 
		depths_tmp[15] = 14; 
	}
else if(s_==2){ 
		vdata2[0] = vdata[14]; 
		vdata2[1] = vdata[15]; 
		vdata2[2] = vdata[0]; 
		vdata2[3] = vdata[1]; 
		vdata2[4] = vdata[2]; 
		vdata2[5] = vdata[3]; 
		vdata2[6] = vdata[4]; 
		vdata2[7] = vdata[5]; 
		vdata2[8] = vdata[6]; 
		vdata2[9] = vdata[7]; 
		vdata2[10] = vdata[8]; 
		vdata2[11] = vdata[9]; 
		vdata2[12] = vdata[10]; 
		vdata2[13] = vdata[11]; 
		vdata2[14] = vdata[12]; 
		vdata2[15] = vdata[13]; 
		
		depths_tmp[0] = 14; 
		depths_tmp[1] = 15; 
		depths_tmp[2] = 0; 
		depths_tmp[3] = 1; 
		depths_tmp[4] = 2; 
		depths_tmp[5] = 3; 
		depths_tmp[6] = 4; 
		depths_tmp[7] = 5; 
		depths_tmp[8] = 6; 
		depths_tmp[9] = 7; 
		depths_tmp[10] = 8; 
		depths_tmp[11] = 9; 
		depths_tmp[12] = 10; 
		depths_tmp[13] = 11; 
		depths_tmp[14] = 12; 
		depths_tmp[15] = 13; 
	}
else if(s_==3){ 
		vdata2[0] = vdata[13]; 
		vdata2[1] = vdata[14]; 
		vdata2[2] = vdata[15]; 
		vdata2[3] = vdata[0]; 
		vdata2[4] = vdata[1]; 
		vdata2[5] = vdata[2]; 
		vdata2[6] = vdata[3]; 
		vdata2[7] = vdata[4]; 
		vdata2[8] = vdata[5]; 
		vdata2[9] = vdata[6]; 
		vdata2[10] = vdata[7]; 
		vdata2[11] = vdata[8]; 
		vdata2[12] = vdata[9]; 
		vdata2[13] = vdata[10]; 
		vdata2[14] = vdata[11]; 
		vdata2[15] = vdata[12]; 
		
		depths_tmp[0] = 13; 
		depths_tmp[1] = 14; 
		depths_tmp[2] = 15; 
		depths_tmp[3] = 0; 
		depths_tmp[4] = 1; 
		depths_tmp[5] = 2; 
		depths_tmp[6] = 3; 
		depths_tmp[7] = 4; 
		depths_tmp[8] = 5; 
		depths_tmp[9] = 6; 
		depths_tmp[10] = 7; 
		depths_tmp[11] = 8; 
		depths_tmp[12] = 9; 
		depths_tmp[13] = 10; 
		depths_tmp[14] = 11; 
		depths_tmp[15] = 12; 
	}
else if(s_==4){ 
		vdata2[0] = vdata[12]; 
		vdata2[1] = vdata[13]; 
		vdata2[2] = vdata[14]; 
		vdata2[3] = vdata[15]; 
		vdata2[4] = vdata[0]; 
		vdata2[5] = vdata[1]; 
		vdata2[6] = vdata[2]; 
		vdata2[7] = vdata[3]; 
		vdata2[8] = vdata[4]; 
		vdata2[9] = vdata[5]; 
		vdata2[10] = vdata[6]; 
		vdata2[11] = vdata[7]; 
		vdata2[12] = vdata[8]; 
		vdata2[13] = vdata[9]; 
		vdata2[14] = vdata[10]; 
		vdata2[15] = vdata[11]; 
		
		depths_tmp[0] = 12; 
		depths_tmp[1] = 13; 
		depths_tmp[2] = 14; 
		depths_tmp[3] = 15; 
		depths_tmp[4] = 0; 
		depths_tmp[5] = 1; 
		depths_tmp[6] = 2; 
		depths_tmp[7] = 3; 
		depths_tmp[8] = 4; 
		depths_tmp[9] = 5; 
		depths_tmp[10] = 6; 
		depths_tmp[11] = 7; 
		depths_tmp[12] = 8; 
		depths_tmp[13] = 9; 
		depths_tmp[14] = 10; 
		depths_tmp[15] = 11; 
	}
else if(s_==5){ 
		vdata2[0] = vdata[11]; 
		vdata2[1] = vdata[12]; 
		vdata2[2] = vdata[13]; 
		vdata2[3] = vdata[14]; 
		vdata2[4] = vdata[15]; 
		vdata2[5] = vdata[0]; 
		vdata2[6] = vdata[1]; 
		vdata2[7] = vdata[2]; 
		vdata2[8] = vdata[3]; 
		vdata2[9] = vdata[4]; 
		vdata2[10] = vdata[5]; 
		vdata2[11] = vdata[6]; 
		vdata2[12] = vdata[7]; 
		vdata2[13] = vdata[8]; 
		vdata2[14] = vdata[9]; 
		vdata2[15] = vdata[10]; 
		
		depths_tmp[0] = 11; 
		depths_tmp[1] = 12; 
		depths_tmp[2] = 13; 
		depths_tmp[3] = 14; 
		depths_tmp[4] = 15; 
		depths_tmp[5] = 0; 
		depths_tmp[6] = 1; 
		depths_tmp[7] = 2; 
		depths_tmp[8] = 3; 
		depths_tmp[9] = 4; 
		depths_tmp[10] = 5; 
		depths_tmp[11] = 6; 
		depths_tmp[12] = 7; 
		depths_tmp[13] = 8; 
		depths_tmp[14] = 9; 
		depths_tmp[15] = 10; 
	}
else if(s_==6){ 
		vdata2[0] = vdata[10]; 
		vdata2[1] = vdata[11]; 
		vdata2[2] = vdata[12]; 
		vdata2[3] = vdata[13]; 
		vdata2[4] = vdata[14]; 
		vdata2[5] = vdata[15]; 
		vdata2[6] = vdata[0]; 
		vdata2[7] = vdata[1]; 
		vdata2[8] = vdata[2]; 
		vdata2[9] = vdata[3]; 
		vdata2[10] = vdata[4]; 
		vdata2[11] = vdata[5]; 
		vdata2[12] = vdata[6]; 
		vdata2[13] = vdata[7]; 
		vdata2[14] = vdata[8]; 
		vdata2[15] = vdata[9]; 
		
		depths_tmp[0] = 10; 
		depths_tmp[1] = 11; 
		depths_tmp[2] = 12; 
		depths_tmp[3] = 13; 
		depths_tmp[4] = 14; 
		depths_tmp[5] = 15; 
		depths_tmp[6] = 0; 
		depths_tmp[7] = 1; 
		depths_tmp[8] = 2; 
		depths_tmp[9] = 3; 
		depths_tmp[10] = 4; 
		depths_tmp[11] = 5; 
		depths_tmp[12] = 6; 
		depths_tmp[13] = 7; 
		depths_tmp[14] = 8; 
		depths_tmp[15] = 9; 
	}
else if(s_==7){ 
		vdata2[0] = vdata[9]; 
		vdata2[1] = vdata[10]; 
		vdata2[2] = vdata[11]; 
		vdata2[3] = vdata[12]; 
		vdata2[4] = vdata[13]; 
		vdata2[5] = vdata[14]; 
		vdata2[6] = vdata[15]; 
		vdata2[7] = vdata[0]; 
		vdata2[8] = vdata[1]; 
		vdata2[9] = vdata[2]; 
		vdata2[10] = vdata[3]; 
		vdata2[11] = vdata[4]; 
		vdata2[12] = vdata[5]; 
		vdata2[13] = vdata[6]; 
		vdata2[14] = vdata[7]; 
		vdata2[15] = vdata[8]; 
		
		depths_tmp[0] = 9; 
		depths_tmp[1] = 10; 
		depths_tmp[2] = 11; 
		depths_tmp[3] = 12; 
		depths_tmp[4] = 13; 
		depths_tmp[5] = 14; 
		depths_tmp[6] = 15; 
		depths_tmp[7] = 0; 
		depths_tmp[8] = 1; 
		depths_tmp[9] = 2; 
		depths_tmp[10] = 3; 
		depths_tmp[11] = 4; 
		depths_tmp[12] = 5; 
		depths_tmp[13] = 6; 
		depths_tmp[14] = 7; 
		depths_tmp[15] = 8; 
	}
else if(s_==8){ 
		vdata2[0] = vdata[8]; 
		vdata2[1] = vdata[9]; 
		vdata2[2] = vdata[10]; 
		vdata2[3] = vdata[11]; 
		vdata2[4] = vdata[12]; 
		vdata2[5] = vdata[13]; 
		vdata2[6] = vdata[14]; 
		vdata2[7] = vdata[15]; 
		vdata2[8] = vdata[0]; 
		vdata2[9] = vdata[1]; 
		vdata2[10] = vdata[2]; 
		vdata2[11] = vdata[3]; 
		vdata2[12] = vdata[4]; 
		vdata2[13] = vdata[5]; 
		vdata2[14] = vdata[6]; 
		vdata2[15] = vdata[7]; 
		
		depths_tmp[0] = 8; 
		depths_tmp[1] = 9; 
		depths_tmp[2] = 10; 
		depths_tmp[3] = 11; 
		depths_tmp[4] = 12; 
		depths_tmp[5] = 13; 
		depths_tmp[6] = 14; 
		depths_tmp[7] = 15; 
		depths_tmp[8] = 0; 
		depths_tmp[9] = 1; 
		depths_tmp[10] = 2; 
		depths_tmp[11] = 3; 
		depths_tmp[12] = 4; 
		depths_tmp[13] = 5; 
		depths_tmp[14] = 6; 
		depths_tmp[15] = 7; 
	}
else if(s_==9){ 
		vdata2[0] = vdata[7]; 
		vdata2[1] = vdata[8]; 
		vdata2[2] = vdata[9]; 
		vdata2[3] = vdata[10]; 
		vdata2[4] = vdata[11]; 
		vdata2[5] = vdata[12]; 
		vdata2[6] = vdata[13]; 
		vdata2[7] = vdata[14]; 
		vdata2[8] = vdata[15]; 
		vdata2[9] = vdata[0]; 
		vdata2[10] = vdata[1]; 
		vdata2[11] = vdata[2]; 
		vdata2[12] = vdata[3]; 
		vdata2[13] = vdata[4]; 
		vdata2[14] = vdata[5]; 
		vdata2[15] = vdata[6]; 
		
		depths_tmp[0] = 7; 
		depths_tmp[1] = 8; 
		depths_tmp[2] = 9; 
		depths_tmp[3] = 10; 
		depths_tmp[4] = 11; 
		depths_tmp[5] = 12; 
		depths_tmp[6] = 13; 
		depths_tmp[7] = 14; 
		depths_tmp[8] = 15; 
		depths_tmp[9] = 0; 
		depths_tmp[10] = 1; 
		depths_tmp[11] = 2; 
		depths_tmp[12] = 3; 
		depths_tmp[13] = 4; 
		depths_tmp[14] = 5; 
		depths_tmp[15] = 6; 
	}
else if(s_==10){ 
		vdata2[0] = vdata[6]; 
		vdata2[1] = vdata[7]; 
		vdata2[2] = vdata[8]; 
		vdata2[3] = vdata[9]; 
		vdata2[4] = vdata[10]; 
		vdata2[5] = vdata[11]; 
		vdata2[6] = vdata[12]; 
		vdata2[7] = vdata[13]; 
		vdata2[8] = vdata[14]; 
		vdata2[9] = vdata[15]; 
		vdata2[10] = vdata[0]; 
		vdata2[11] = vdata[1]; 
		vdata2[12] = vdata[2]; 
		vdata2[13] = vdata[3]; 
		vdata2[14] = vdata[4]; 
		vdata2[15] = vdata[5]; 
		
		depths_tmp[0] = 6; 
		depths_tmp[1] = 7; 
		depths_tmp[2] = 8; 
		depths_tmp[3] = 9; 
		depths_tmp[4] = 10; 
		depths_tmp[5] = 11; 
		depths_tmp[6] = 12; 
		depths_tmp[7] = 13; 
		depths_tmp[8] = 14; 
		depths_tmp[9] = 15; 
		depths_tmp[10] = 0; 
		depths_tmp[11] = 1; 
		depths_tmp[12] = 2; 
		depths_tmp[13] = 3; 
		depths_tmp[14] = 4; 
		depths_tmp[15] = 5; 
	}
else if(s_==11){ 
		vdata2[0] = vdata[5]; 
		vdata2[1] = vdata[6]; 
		vdata2[2] = vdata[7]; 
		vdata2[3] = vdata[8]; 
		vdata2[4] = vdata[9]; 
		vdata2[5] = vdata[10]; 
		vdata2[6] = vdata[11]; 
		vdata2[7] = vdata[12]; 
		vdata2[8] = vdata[13]; 
		vdata2[9] = vdata[14]; 
		vdata2[10] = vdata[15]; 
		vdata2[11] = vdata[0]; 
		vdata2[12] = vdata[1]; 
		vdata2[13] = vdata[2]; 
		vdata2[14] = vdata[3]; 
		vdata2[15] = vdata[4]; 
		
		depths_tmp[0] = 5; 
		depths_tmp[1] = 6; 
		depths_tmp[2] = 7; 
		depths_tmp[3] = 8; 
		depths_tmp[4] = 9; 
		depths_tmp[5] = 10; 
		depths_tmp[6] = 11; 
		depths_tmp[7] = 12; 
		depths_tmp[8] = 13; 
		depths_tmp[9] = 14; 
		depths_tmp[10] = 15; 
		depths_tmp[11] = 0; 
		depths_tmp[12] = 1; 
		depths_tmp[13] = 2; 
		depths_tmp[14] = 3; 
		depths_tmp[15] = 4; 
	}
else if(s_==12){ 
		vdata2[0] = vdata[4]; 
		vdata2[1] = vdata[5]; 
		vdata2[2] = vdata[6]; 
		vdata2[3] = vdata[7]; 
		vdata2[4] = vdata[8]; 
		vdata2[5] = vdata[9]; 
		vdata2[6] = vdata[10]; 
		vdata2[7] = vdata[11]; 
		vdata2[8] = vdata[12]; 
		vdata2[9] = vdata[13]; 
		vdata2[10] = vdata[14]; 
		vdata2[11] = vdata[15]; 
		vdata2[12] = vdata[0]; 
		vdata2[13] = vdata[1]; 
		vdata2[14] = vdata[2]; 
		vdata2[15] = vdata[3]; 
		
		depths_tmp[0] = 4; 
		depths_tmp[1] = 5; 
		depths_tmp[2] = 6; 
		depths_tmp[3] = 7; 
		depths_tmp[4] = 8; 
		depths_tmp[5] = 9; 
		depths_tmp[6] = 10; 
		depths_tmp[7] = 11; 
		depths_tmp[8] = 12; 
		depths_tmp[9] = 13; 
		depths_tmp[10] = 14; 
		depths_tmp[11] = 15; 
		depths_tmp[12] = 0; 
		depths_tmp[13] = 1; 
		depths_tmp[14] = 2; 
		depths_tmp[15] = 3; 
	}
else if(s_==13){ 
		vdata2[0] = vdata[3]; 
		vdata2[1] = vdata[4]; 
		vdata2[2] = vdata[5]; 
		vdata2[3] = vdata[6]; 
		vdata2[4] = vdata[7]; 
		vdata2[5] = vdata[8]; 
		vdata2[6] = vdata[9]; 
		vdata2[7] = vdata[10]; 
		vdata2[8] = vdata[11]; 
		vdata2[9] = vdata[12]; 
		vdata2[10] = vdata[13]; 
		vdata2[11] = vdata[14]; 
		vdata2[12] = vdata[15]; 
		vdata2[13] = vdata[0]; 
		vdata2[14] = vdata[1]; 
		vdata2[15] = vdata[2]; 
		
		depths_tmp[0] = 3; 
		depths_tmp[1] = 4; 
		depths_tmp[2] = 5; 
		depths_tmp[3] = 6; 
		depths_tmp[4] = 7; 
		depths_tmp[5] = 8; 
		depths_tmp[6] = 9; 
		depths_tmp[7] = 10; 
		depths_tmp[8] = 11; 
		depths_tmp[9] = 12; 
		depths_tmp[10] = 13; 
		depths_tmp[11] = 14; 
		depths_tmp[12] = 15; 
		depths_tmp[13] = 0; 
		depths_tmp[14] = 1; 
		depths_tmp[15] = 2; 
	}
else if(s_==14){ 
		vdata2[0] = vdata[2]; 
		vdata2[1] = vdata[3]; 
		vdata2[2] = vdata[4]; 
		vdata2[3] = vdata[5]; 
		vdata2[4] = vdata[6]; 
		vdata2[5] = vdata[7]; 
		vdata2[6] = vdata[8]; 
		vdata2[7] = vdata[9]; 
		vdata2[8] = vdata[10]; 
		vdata2[9] = vdata[11]; 
		vdata2[10] = vdata[12]; 
		vdata2[11] = vdata[13]; 
		vdata2[12] = vdata[14]; 
		vdata2[13] = vdata[15]; 
		vdata2[14] = vdata[0]; 
		vdata2[15] = vdata[1]; 
		
		depths_tmp[0] = 2; 
		depths_tmp[1] = 3; 
		depths_tmp[2] = 4; 
		depths_tmp[3] = 5; 
		depths_tmp[4] = 6; 
		depths_tmp[5] = 7; 
		depths_tmp[6] = 8; 
		depths_tmp[7] = 9; 
		depths_tmp[8] = 10; 
		depths_tmp[9] = 11; 
		depths_tmp[10] = 12; 
		depths_tmp[11] = 13; 
		depths_tmp[12] = 14; 
		depths_tmp[13] = 15; 
		depths_tmp[14] = 0; 
		depths_tmp[15] = 1; 
	}
else { 
		vdata2[0] = vdata[1]; 
		vdata2[1] = vdata[2]; 
		vdata2[2] = vdata[3]; 
		vdata2[3] = vdata[4]; 
		vdata2[4] = vdata[5]; 
		vdata2[5] = vdata[6]; 
		vdata2[6] = vdata[7]; 
		vdata2[7] = vdata[8]; 
		vdata2[8] = vdata[9]; 
		vdata2[9] = vdata[10]; 
		vdata2[10] = vdata[11]; 
		vdata2[11] = vdata[12]; 
		vdata2[12] = vdata[13]; 
		vdata2[13] = vdata[14]; 
		vdata2[14] = vdata[15]; 
		vdata2[15] = vdata[0]; 
		
		depths_tmp[0] = 1; 
		depths_tmp[1] = 2; 
		depths_tmp[2] = 3; 
		depths_tmp[3] = 4; 
		depths_tmp[4] = 5; 
		depths_tmp[5] = 6; 
		depths_tmp[6] = 7; 
		depths_tmp[7] = 8; 
		depths_tmp[8] = 9; 
		depths_tmp[9] = 10; 
		depths_tmp[10] = 11; 
		depths_tmp[11] = 12; 
		depths_tmp[12] = 13; 
		depths_tmp[13] = 14; 
		depths_tmp[14] = 15; 
		depths_tmp[15] = 0; 
	}
	
	depths[0] = basedepth + depths_tmp[0]; 
	depths[1] = basedepth + depths_tmp[1]; 
	depths[2] = basedepth + depths_tmp[2]; 
	depths[3] = basedepth + depths_tmp[3]; 
	depths[4] = basedepth + depths_tmp[4]; 
	depths[5] = basedepth + depths_tmp[5]; 
	depths[6] = basedepth + depths_tmp[6]; 
	depths[7] = basedepth + depths_tmp[7]; 
	depths[8] = basedepth + depths_tmp[8]; 
	depths[9] = basedepth + depths_tmp[9]; 
	depths[10] = basedepth + depths_tmp[10]; 
	depths[11] = basedepth + depths_tmp[11]; 
	depths[12] = basedepth + depths_tmp[12]; 
	depths[13] = basedepth + depths_tmp[13]; 
	depths[14] = basedepth + depths_tmp[14]; 
	depths[15] = basedepth + depths_tmp[15]; 
	return;
}

void MEMACCESSS_RearrangeLayoutV(unsigned int s, keyvalue_vbuffer_t vdata[VECTOR2_SIZE], keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]){
	/// NB: This function must be consistent with loadedges_random.cpp->loadedges_random::twist_getpartition
	unsigned int s_ = s % VECTOR2_SIZE;	
	
 if(s_==0){ 
		vdata2[0] = vdata[0]; 
		vdata2[1] = vdata[1]; 
		vdata2[2] = vdata[2]; 
		vdata2[3] = vdata[3]; 
		vdata2[4] = vdata[4]; 
		vdata2[5] = vdata[5]; 
		vdata2[6] = vdata[6]; 
		vdata2[7] = vdata[7]; 
		vdata2[8] = vdata[8]; 
		vdata2[9] = vdata[9]; 
		vdata2[10] = vdata[10]; 
		vdata2[11] = vdata[11]; 
		vdata2[12] = vdata[12]; 
		vdata2[13] = vdata[13]; 
		vdata2[14] = vdata[14]; 
		vdata2[15] = vdata[15]; 
	}
else if(s_==1){ 
		vdata2[1] = vdata[0]; 
		vdata2[2] = vdata[1]; 
		vdata2[3] = vdata[2]; 
		vdata2[4] = vdata[3]; 
		vdata2[5] = vdata[4]; 
		vdata2[6] = vdata[5]; 
		vdata2[7] = vdata[6]; 
		vdata2[8] = vdata[7]; 
		vdata2[9] = vdata[8]; 
		vdata2[10] = vdata[9]; 
		vdata2[11] = vdata[10]; 
		vdata2[12] = vdata[11]; 
		vdata2[13] = vdata[12]; 
		vdata2[14] = vdata[13]; 
		vdata2[15] = vdata[14]; 
		vdata2[0] = vdata[15]; 
	}
else if(s_==2){ 
		vdata2[2] = vdata[0]; 
		vdata2[3] = vdata[1]; 
		vdata2[4] = vdata[2]; 
		vdata2[5] = vdata[3]; 
		vdata2[6] = vdata[4]; 
		vdata2[7] = vdata[5]; 
		vdata2[8] = vdata[6]; 
		vdata2[9] = vdata[7]; 
		vdata2[10] = vdata[8]; 
		vdata2[11] = vdata[9]; 
		vdata2[12] = vdata[10]; 
		vdata2[13] = vdata[11]; 
		vdata2[14] = vdata[12]; 
		vdata2[15] = vdata[13]; 
		vdata2[0] = vdata[14]; 
		vdata2[1] = vdata[15]; 
	}
else if(s_==3){ 
		vdata2[3] = vdata[0]; 
		vdata2[4] = vdata[1]; 
		vdata2[5] = vdata[2]; 
		vdata2[6] = vdata[3]; 
		vdata2[7] = vdata[4]; 
		vdata2[8] = vdata[5]; 
		vdata2[9] = vdata[6]; 
		vdata2[10] = vdata[7]; 
		vdata2[11] = vdata[8]; 
		vdata2[12] = vdata[9]; 
		vdata2[13] = vdata[10]; 
		vdata2[14] = vdata[11]; 
		vdata2[15] = vdata[12]; 
		vdata2[0] = vdata[13]; 
		vdata2[1] = vdata[14]; 
		vdata2[2] = vdata[15]; 
	}
else if(s_==4){ 
		vdata2[4] = vdata[0]; 
		vdata2[5] = vdata[1]; 
		vdata2[6] = vdata[2]; 
		vdata2[7] = vdata[3]; 
		vdata2[8] = vdata[4]; 
		vdata2[9] = vdata[5]; 
		vdata2[10] = vdata[6]; 
		vdata2[11] = vdata[7]; 
		vdata2[12] = vdata[8]; 
		vdata2[13] = vdata[9]; 
		vdata2[14] = vdata[10]; 
		vdata2[15] = vdata[11]; 
		vdata2[0] = vdata[12]; 
		vdata2[1] = vdata[13]; 
		vdata2[2] = vdata[14]; 
		vdata2[3] = vdata[15]; 
	}
else if(s_==5){ 
		vdata2[5] = vdata[0]; 
		vdata2[6] = vdata[1]; 
		vdata2[7] = vdata[2]; 
		vdata2[8] = vdata[3]; 
		vdata2[9] = vdata[4]; 
		vdata2[10] = vdata[5]; 
		vdata2[11] = vdata[6]; 
		vdata2[12] = vdata[7]; 
		vdata2[13] = vdata[8]; 
		vdata2[14] = vdata[9]; 
		vdata2[15] = vdata[10]; 
		vdata2[0] = vdata[11]; 
		vdata2[1] = vdata[12]; 
		vdata2[2] = vdata[13]; 
		vdata2[3] = vdata[14]; 
		vdata2[4] = vdata[15]; 
	}
else if(s_==6){ 
		vdata2[6] = vdata[0]; 
		vdata2[7] = vdata[1]; 
		vdata2[8] = vdata[2]; 
		vdata2[9] = vdata[3]; 
		vdata2[10] = vdata[4]; 
		vdata2[11] = vdata[5]; 
		vdata2[12] = vdata[6]; 
		vdata2[13] = vdata[7]; 
		vdata2[14] = vdata[8]; 
		vdata2[15] = vdata[9]; 
		vdata2[0] = vdata[10]; 
		vdata2[1] = vdata[11]; 
		vdata2[2] = vdata[12]; 
		vdata2[3] = vdata[13]; 
		vdata2[4] = vdata[14]; 
		vdata2[5] = vdata[15]; 
	}
else if(s_==7){ 
		vdata2[7] = vdata[0]; 
		vdata2[8] = vdata[1]; 
		vdata2[9] = vdata[2]; 
		vdata2[10] = vdata[3]; 
		vdata2[11] = vdata[4]; 
		vdata2[12] = vdata[5]; 
		vdata2[13] = vdata[6]; 
		vdata2[14] = vdata[7]; 
		vdata2[15] = vdata[8]; 
		vdata2[0] = vdata[9]; 
		vdata2[1] = vdata[10]; 
		vdata2[2] = vdata[11]; 
		vdata2[3] = vdata[12]; 
		vdata2[4] = vdata[13]; 
		vdata2[5] = vdata[14]; 
		vdata2[6] = vdata[15]; 
	}
else if(s_==8){ 
		vdata2[8] = vdata[0]; 
		vdata2[9] = vdata[1]; 
		vdata2[10] = vdata[2]; 
		vdata2[11] = vdata[3]; 
		vdata2[12] = vdata[4]; 
		vdata2[13] = vdata[5]; 
		vdata2[14] = vdata[6]; 
		vdata2[15] = vdata[7]; 
		vdata2[0] = vdata[8]; 
		vdata2[1] = vdata[9]; 
		vdata2[2] = vdata[10]; 
		vdata2[3] = vdata[11]; 
		vdata2[4] = vdata[12]; 
		vdata2[5] = vdata[13]; 
		vdata2[6] = vdata[14]; 
		vdata2[7] = vdata[15]; 
	}
else if(s_==9){ 
		vdata2[9] = vdata[0]; 
		vdata2[10] = vdata[1]; 
		vdata2[11] = vdata[2]; 
		vdata2[12] = vdata[3]; 
		vdata2[13] = vdata[4]; 
		vdata2[14] = vdata[5]; 
		vdata2[15] = vdata[6]; 
		vdata2[0] = vdata[7]; 
		vdata2[1] = vdata[8]; 
		vdata2[2] = vdata[9]; 
		vdata2[3] = vdata[10]; 
		vdata2[4] = vdata[11]; 
		vdata2[5] = vdata[12]; 
		vdata2[6] = vdata[13]; 
		vdata2[7] = vdata[14]; 
		vdata2[8] = vdata[15]; 
	}
else if(s_==10){ 
		vdata2[10] = vdata[0]; 
		vdata2[11] = vdata[1]; 
		vdata2[12] = vdata[2]; 
		vdata2[13] = vdata[3]; 
		vdata2[14] = vdata[4]; 
		vdata2[15] = vdata[5]; 
		vdata2[0] = vdata[6]; 
		vdata2[1] = vdata[7]; 
		vdata2[2] = vdata[8]; 
		vdata2[3] = vdata[9]; 
		vdata2[4] = vdata[10]; 
		vdata2[5] = vdata[11]; 
		vdata2[6] = vdata[12]; 
		vdata2[7] = vdata[13]; 
		vdata2[8] = vdata[14]; 
		vdata2[9] = vdata[15]; 
	}
else if(s_==11){ 
		vdata2[11] = vdata[0]; 
		vdata2[12] = vdata[1]; 
		vdata2[13] = vdata[2]; 
		vdata2[14] = vdata[3]; 
		vdata2[15] = vdata[4]; 
		vdata2[0] = vdata[5]; 
		vdata2[1] = vdata[6]; 
		vdata2[2] = vdata[7]; 
		vdata2[3] = vdata[8]; 
		vdata2[4] = vdata[9]; 
		vdata2[5] = vdata[10]; 
		vdata2[6] = vdata[11]; 
		vdata2[7] = vdata[12]; 
		vdata2[8] = vdata[13]; 
		vdata2[9] = vdata[14]; 
		vdata2[10] = vdata[15]; 
	}
else if(s_==12){ 
		vdata2[12] = vdata[0]; 
		vdata2[13] = vdata[1]; 
		vdata2[14] = vdata[2]; 
		vdata2[15] = vdata[3]; 
		vdata2[0] = vdata[4]; 
		vdata2[1] = vdata[5]; 
		vdata2[2] = vdata[6]; 
		vdata2[3] = vdata[7]; 
		vdata2[4] = vdata[8]; 
		vdata2[5] = vdata[9]; 
		vdata2[6] = vdata[10]; 
		vdata2[7] = vdata[11]; 
		vdata2[8] = vdata[12]; 
		vdata2[9] = vdata[13]; 
		vdata2[10] = vdata[14]; 
		vdata2[11] = vdata[15]; 
	}
else if(s_==13){ 
		vdata2[13] = vdata[0]; 
		vdata2[14] = vdata[1]; 
		vdata2[15] = vdata[2]; 
		vdata2[0] = vdata[3]; 
		vdata2[1] = vdata[4]; 
		vdata2[2] = vdata[5]; 
		vdata2[3] = vdata[6]; 
		vdata2[4] = vdata[7]; 
		vdata2[5] = vdata[8]; 
		vdata2[6] = vdata[9]; 
		vdata2[7] = vdata[10]; 
		vdata2[8] = vdata[11]; 
		vdata2[9] = vdata[12]; 
		vdata2[10] = vdata[13]; 
		vdata2[11] = vdata[14]; 
		vdata2[12] = vdata[15]; 
	}
else if(s_==14){ 
		vdata2[14] = vdata[0]; 
		vdata2[15] = vdata[1]; 
		vdata2[0] = vdata[2]; 
		vdata2[1] = vdata[3]; 
		vdata2[2] = vdata[4]; 
		vdata2[3] = vdata[5]; 
		vdata2[4] = vdata[6]; 
		vdata2[5] = vdata[7]; 
		vdata2[6] = vdata[8]; 
		vdata2[7] = vdata[9]; 
		vdata2[8] = vdata[10]; 
		vdata2[9] = vdata[11]; 
		vdata2[10] = vdata[12]; 
		vdata2[11] = vdata[13]; 
		vdata2[12] = vdata[14]; 
		vdata2[13] = vdata[15]; 
	}
else { 
		vdata2[15] = vdata[0]; 
		vdata2[0] = vdata[1]; 
		vdata2[1] = vdata[2]; 
		vdata2[2] = vdata[3]; 
		vdata2[3] = vdata[4]; 
		vdata2[4] = vdata[5]; 
		vdata2[5] = vdata[6]; 
		vdata2[6] = vdata[7]; 
		vdata2[7] = vdata[8]; 
		vdata2[8] = vdata[9]; 
		vdata2[9] = vdata[10]; 
		vdata2[10] = vdata[11]; 
		vdata2[11] = vdata[12]; 
		vdata2[12] = vdata[13]; 
		vdata2[13] = vdata[14]; 
		vdata2[14] = vdata[15]; 
	}
	return;
}

void MEMACCESSS_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){
	if(enable == OFF){ return; }

	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	
	if(globalparams.ALGORITHMINFO_GRAPHALGORITHMID == HITS){ size_kvs = size_kvs * 2; }
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD // CRITICAL FIXME.
		vdata[0] = kvdram[baseoffset_kvs + offset_kvs + i].range(31, 0); 
		vdata[1] = kvdram[baseoffset_kvs + offset_kvs + i].range(63, 32); 
		vdata[2] = kvdram[baseoffset_kvs + offset_kvs + i].range(95, 64); 
		vdata[3] = kvdram[baseoffset_kvs + offset_kvs + i].range(127, 96); 
		vdata[4] = kvdram[baseoffset_kvs + offset_kvs + i].range(159, 128); 
		vdata[5] = kvdram[baseoffset_kvs + offset_kvs + i].range(191, 160); 
		vdata[6] = kvdram[baseoffset_kvs + offset_kvs + i].range(223, 192); 
		vdata[7] = kvdram[baseoffset_kvs + offset_kvs + i].range(255, 224); 
		vdata[8] = kvdram[baseoffset_kvs + offset_kvs + i].range(287, 256); 
		vdata[9] = kvdram[baseoffset_kvs + offset_kvs + i].range(319, 288); 
		vdata[10] = kvdram[baseoffset_kvs + offset_kvs + i].range(351, 320); 
		vdata[11] = kvdram[baseoffset_kvs + offset_kvs + i].range(383, 352); 
		vdata[12] = kvdram[baseoffset_kvs + offset_kvs + i].range(415, 384); 
		vdata[13] = kvdram[baseoffset_kvs + offset_kvs + i].range(447, 416); 
		vdata[14] = kvdram[baseoffset_kvs + offset_kvs + i].range(479, 448); 
		vdata[15] = kvdram[baseoffset_kvs + offset_kvs + i].range(511, 480); 
		#else 
		vdata[0] = kvdram[baseoffset_kvs + offset_kvs + i].data[0].key;
		vdata[1] = kvdram[baseoffset_kvs + offset_kvs + i].data[0].value; 
		vdata[2] = kvdram[baseoffset_kvs + offset_kvs + i].data[1].key;
		vdata[3] = kvdram[baseoffset_kvs + offset_kvs + i].data[1].value; 
		vdata[4] = kvdram[baseoffset_kvs + offset_kvs + i].data[2].key;
		vdata[5] = kvdram[baseoffset_kvs + offset_kvs + i].data[2].value; 
		vdata[6] = kvdram[baseoffset_kvs + offset_kvs + i].data[3].key;
		vdata[7] = kvdram[baseoffset_kvs + offset_kvs + i].data[3].value; 
		vdata[8] = kvdram[baseoffset_kvs + offset_kvs + i].data[4].key;
		vdata[9] = kvdram[baseoffset_kvs + offset_kvs + i].data[4].value; 
		vdata[10] = kvdram[baseoffset_kvs + offset_kvs + i].data[5].key;
		vdata[11] = kvdram[baseoffset_kvs + offset_kvs + i].data[5].value; 
		vdata[12] = kvdram[baseoffset_kvs + offset_kvs + i].data[6].key;
		vdata[13] = kvdram[baseoffset_kvs + offset_kvs + i].data[6].value; 
		vdata[14] = kvdram[baseoffset_kvs + offset_kvs + i].data[7].key;
		vdata[15] = kvdram[baseoffset_kvs + offset_kvs + i].data[7].value; 
		#endif
		
		// reset any masks already present
		#ifndef ALLVERTEXISACTIVE_ALGORITHM // REMOVEME.
		if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.v_chunkid == 0){
			MEMCAS_WRITEVMASK(&vdata[0], 0);
			MEMCAS_WRITEVMASK(&vdata[1], 0);
			MEMCAS_WRITEVMASK(&vdata[2], 0);
			MEMCAS_WRITEVMASK(&vdata[3], 0);
			MEMCAS_WRITEVMASK(&vdata[4], 0);
			MEMCAS_WRITEVMASK(&vdata[5], 0);
			MEMCAS_WRITEVMASK(&vdata[6], 0);
			MEMCAS_WRITEVMASK(&vdata[7], 0);
			MEMCAS_WRITEVMASK(&vdata[8], 0);
			MEMCAS_WRITEVMASK(&vdata[9], 0);
			MEMCAS_WRITEVMASK(&vdata[10], 0);
			MEMCAS_WRITEVMASK(&vdata[11], 0);
			MEMCAS_WRITEVMASK(&vdata[12], 0);
			MEMCAS_WRITEVMASK(&vdata[13], 0);
			MEMCAS_WRITEVMASK(&vdata[14], 0);
			MEMCAS_WRITEVMASK(&vdata[15], 0);
		}
		#endif
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSRRRS_readV 21", bufferoffset_kvs + i, MAX_BLOCKRAM_VDESTDATA_SIZE, size_kvs, bufferoffset_kvs, i);
		#endif
		buffer[0][bufferoffset_kvs + i] = vdata[0];
		buffer[1][bufferoffset_kvs + i] = vdata[1];
		buffer[2][bufferoffset_kvs + i] = vdata[2];
		buffer[3][bufferoffset_kvs + i] = vdata[3];
		buffer[4][bufferoffset_kvs + i] = vdata[4];
		buffer[5][bufferoffset_kvs + i] = vdata[5];
		buffer[6][bufferoffset_kvs + i] = vdata[6];
		buffer[7][bufferoffset_kvs + i] = vdata[7];
		buffer[8][bufferoffset_kvs + i] = vdata[8];
		buffer[9][bufferoffset_kvs + i] = vdata[9];
		buffer[10][bufferoffset_kvs + i] = vdata[10];
		buffer[11][bufferoffset_kvs + i] = vdata[11];
		buffer[12][bufferoffset_kvs + i] = vdata[12];
		buffer[13][bufferoffset_kvs + i] = vdata[13];
		buffer[14][bufferoffset_kvs + i] = vdata[14];
		buffer[15][bufferoffset_kvs + i] = vdata[15];

		#ifdef _DEBUGMODE_STATS 
		actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
		#endif
	}
	return;
}

void MEMACCESSS_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	// cout<<"MEMACCESSS_saveV SEEN. "<<endl;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	
	if(globalparams.ALGORITHMINFO_GRAPHALGORITHMID == HITS){ size_kvs = size_kvs * 2; }
	SAVEVDATA_LOOP1: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	
		vdata[0] = vbuffer[0][bufferoffset_kvs + i];
	
		vdata[1] = vbuffer[1][bufferoffset_kvs + i];
	
		vdata[2] = vbuffer[2][bufferoffset_kvs + i];
	
		vdata[3] = vbuffer[3][bufferoffset_kvs + i];
	
		vdata[4] = vbuffer[4][bufferoffset_kvs + i];
	
		vdata[5] = vbuffer[5][bufferoffset_kvs + i];
	
		vdata[6] = vbuffer[6][bufferoffset_kvs + i];
	
		vdata[7] = vbuffer[7][bufferoffset_kvs + i];
	
		vdata[8] = vbuffer[8][bufferoffset_kvs + i];
	
		vdata[9] = vbuffer[9][bufferoffset_kvs + i];
	
		vdata[10] = vbuffer[10][bufferoffset_kvs + i];
	
		vdata[11] = vbuffer[11][bufferoffset_kvs + i];
	
		vdata[12] = vbuffer[12][bufferoffset_kvs + i];
	
		vdata[13] = vbuffer[13][bufferoffset_kvs + i];
	
		vdata[14] = vbuffer[14][bufferoffset_kvs + i];
	
		vdata[15] = vbuffer[15][bufferoffset_kvs + i];
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSS_saveV 23", baseoffset_kvs + offset_kvs + i, globalparams.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		kvdram[baseoffset_kvs + offset_kvs + i].range(31, 0) = vdata[0]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(63, 32) = vdata[1]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(95, 64) = vdata[2]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(127, 96) = vdata[3]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(159, 128) = vdata[4]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(191, 160) = vdata[5]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(223, 192) = vdata[6]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(255, 224) = vdata[7]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(287, 256) = vdata[8]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(319, 288) = vdata[9]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(351, 320) = vdata[10]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(383, 352) = vdata[11]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(415, 384) = vdata[12]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(447, 416) = vdata[13]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(479, 448) = vdata[14]; 
		kvdram[baseoffset_kvs + offset_kvs + i].range(511, 480) = vdata[15]; 
		#else 
		kvdram[baseoffset_kvs + offset_kvs + i].data[0].key = vdata[0];
		kvdram[baseoffset_kvs + offset_kvs + i].data[0].value = vdata[1]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[1].key = vdata[2];
		kvdram[baseoffset_kvs + offset_kvs + i].data[1].value = vdata[3]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[2].key = vdata[4];
		kvdram[baseoffset_kvs + offset_kvs + i].data[2].value = vdata[5]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[3].key = vdata[6];
		kvdram[baseoffset_kvs + offset_kvs + i].data[3].value = vdata[7]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[4].key = vdata[8];
		kvdram[baseoffset_kvs + offset_kvs + i].data[4].value = vdata[9]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[5].key = vdata[10];
		kvdram[baseoffset_kvs + offset_kvs + i].data[5].value = vdata[11]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[6].key = vdata[12];
		kvdram[baseoffset_kvs + offset_kvs + i].data[6].value = vdata[13]; 
		kvdram[baseoffset_kvs + offset_kvs + i].data[7].key = vdata[14];
		kvdram[baseoffset_kvs + offset_kvs + i].data[7].value = vdata[15]; 
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE // _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			if(MEMCAS_READVMASK(vdata[v])==1){ cout<<"$$$ MEMACCESSS_saveV:: MASK=1 SEEN. index: "<<i<<endl; }
		}
		#endif

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countvswritten(VECTOR2_SIZE);
		#endif
	}
	// exit(EXIT_SUCCESS); //
	return;
}

void MEMACCESSS_copyV(bool_type enable, keyvalue_vbuffer_t buffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t buffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], buffer_type srcoffset_kvs, buffer_type destoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }

	COPYVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		buffer_dest[0][destoffset_kvs + i] = buffer_source[0][srcoffset_kvs + i]; 
		buffer_dest[1][destoffset_kvs + i] = buffer_source[1][srcoffset_kvs + i]; 
		buffer_dest[2][destoffset_kvs + i] = buffer_source[2][srcoffset_kvs + i]; 
		buffer_dest[3][destoffset_kvs + i] = buffer_source[3][srcoffset_kvs + i]; 
		buffer_dest[4][destoffset_kvs + i] = buffer_source[4][srcoffset_kvs + i]; 
		buffer_dest[5][destoffset_kvs + i] = buffer_source[5][srcoffset_kvs + i]; 
		buffer_dest[6][destoffset_kvs + i] = buffer_source[6][srcoffset_kvs + i]; 
		buffer_dest[7][destoffset_kvs + i] = buffer_source[7][srcoffset_kvs + i]; 
		buffer_dest[8][destoffset_kvs + i] = buffer_source[8][srcoffset_kvs + i]; 
		buffer_dest[9][destoffset_kvs + i] = buffer_source[9][srcoffset_kvs + i]; 
		buffer_dest[10][destoffset_kvs + i] = buffer_source[10][srcoffset_kvs + i]; 
		buffer_dest[11][destoffset_kvs + i] = buffer_source[11][srcoffset_kvs + i]; 
		buffer_dest[12][destoffset_kvs + i] = buffer_source[12][srcoffset_kvs + i]; 
		buffer_dest[13][destoffset_kvs + i] = buffer_source[13][srcoffset_kvs + i]; 
		buffer_dest[14][destoffset_kvs + i] = buffer_source[14][srcoffset_kvs + i]; 
		buffer_dest[15][destoffset_kvs + i] = buffer_source[15][srcoffset_kvs + i]; 
	}
	return;
}

void MEMACCESSS_readANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks1 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks1 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks1 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks2 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks2 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks2 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks3 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks3 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks3 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks4 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks4 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks4 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks5 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks5 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks5 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks6 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks6 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks6 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks7 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks7 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks7 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks8 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks8 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks8 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	#pragma HLS array_partition variable = buffer8
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
		buffer8[0][i] = 0;
		buffer8[1][i] = 0;
		buffer8[2][i] = 0;
		buffer8[3][i] = 0;
		buffer8[4][i] = 0;
		buffer8[5][i] = 0;
		buffer8[6][i] = 0;
		buffer8[7][i] = 0;
		buffer8[8][i] = 0;
		buffer8[9][i] = 0;
		buffer8[10][i] = 0;
		buffer8[11][i] = 0;
		buffer8[12][i] = 0;
		buffer8[13][i] = 0;
		buffer8[14][i] = 0;
		buffer8[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks9 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks9 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks9 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
			buffer8[0][bdepth_i + i] = vdata2[0];
			buffer8[1][bdepth_i + i] = vdata2[1];
			buffer8[2][bdepth_i + i] = vdata2[2];
			buffer8[3][bdepth_i + i] = vdata2[3];
			buffer8[4][bdepth_i + i] = vdata2[4];
			buffer8[5][bdepth_i + i] = vdata2[5];
			buffer8[6][bdepth_i + i] = vdata2[6];
			buffer8[7][bdepth_i + i] = vdata2[7];
			buffer8[8][bdepth_i + i] = vdata2[8];
			buffer8[9][bdepth_i + i] = vdata2[9];
			buffer8[10][bdepth_i + i] = vdata2[10];
			buffer8[11][bdepth_i + i] = vdata2[11];
			buffer8[12][bdepth_i + i] = vdata2[12];
			buffer8[13][bdepth_i + i] = vdata2[13];
			buffer8[14][bdepth_i + i] = vdata2[14];
			buffer8[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	#pragma HLS array_partition variable = buffer8
	#pragma HLS array_partition variable = buffer9
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
		buffer8[0][i] = 0;
		buffer8[1][i] = 0;
		buffer8[2][i] = 0;
		buffer8[3][i] = 0;
		buffer8[4][i] = 0;
		buffer8[5][i] = 0;
		buffer8[6][i] = 0;
		buffer8[7][i] = 0;
		buffer8[8][i] = 0;
		buffer8[9][i] = 0;
		buffer8[10][i] = 0;
		buffer8[11][i] = 0;
		buffer8[12][i] = 0;
		buffer8[13][i] = 0;
		buffer8[14][i] = 0;
		buffer8[15][i] = 0;
	
		buffer9[0][i] = 0;
		buffer9[1][i] = 0;
		buffer9[2][i] = 0;
		buffer9[3][i] = 0;
		buffer9[4][i] = 0;
		buffer9[5][i] = 0;
		buffer9[6][i] = 0;
		buffer9[7][i] = 0;
		buffer9[8][i] = 0;
		buffer9[9][i] = 0;
		buffer9[10][i] = 0;
		buffer9[11][i] = 0;
		buffer9[12][i] = 0;
		buffer9[13][i] = 0;
		buffer9[14][i] = 0;
		buffer9[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks10 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks10 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks10 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
			buffer8[0][bdepth_i + i] = vdata2[0];
			buffer8[1][bdepth_i + i] = vdata2[1];
			buffer8[2][bdepth_i + i] = vdata2[2];
			buffer8[3][bdepth_i + i] = vdata2[3];
			buffer8[4][bdepth_i + i] = vdata2[4];
			buffer8[5][bdepth_i + i] = vdata2[5];
			buffer8[6][bdepth_i + i] = vdata2[6];
			buffer8[7][bdepth_i + i] = vdata2[7];
			buffer8[8][bdepth_i + i] = vdata2[8];
			buffer8[9][bdepth_i + i] = vdata2[9];
			buffer8[10][bdepth_i + i] = vdata2[10];
			buffer8[11][bdepth_i + i] = vdata2[11];
			buffer8[12][bdepth_i + i] = vdata2[12];
			buffer8[13][bdepth_i + i] = vdata2[13];
			buffer8[14][bdepth_i + i] = vdata2[14];
			buffer8[15][bdepth_i + i] = vdata2[15];
			buffer9[0][bdepth_i + i] = vdata2[0];
			buffer9[1][bdepth_i + i] = vdata2[1];
			buffer9[2][bdepth_i + i] = vdata2[2];
			buffer9[3][bdepth_i + i] = vdata2[3];
			buffer9[4][bdepth_i + i] = vdata2[4];
			buffer9[5][bdepth_i + i] = vdata2[5];
			buffer9[6][bdepth_i + i] = vdata2[6];
			buffer9[7][bdepth_i + i] = vdata2[7];
			buffer9[8][bdepth_i + i] = vdata2[8];
			buffer9[9][bdepth_i + i] = vdata2[9];
			buffer9[10][bdepth_i + i] = vdata2[10];
			buffer9[11][bdepth_i + i] = vdata2[11];
			buffer9[12][bdepth_i + i] = vdata2[12];
			buffer9[13][bdepth_i + i] = vdata2[13];
			buffer9[14][bdepth_i + i] = vdata2[14];
			buffer9[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	#pragma HLS array_partition variable = buffer8
	#pragma HLS array_partition variable = buffer9
	#pragma HLS array_partition variable = buffer10
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
		buffer8[0][i] = 0;
		buffer8[1][i] = 0;
		buffer8[2][i] = 0;
		buffer8[3][i] = 0;
		buffer8[4][i] = 0;
		buffer8[5][i] = 0;
		buffer8[6][i] = 0;
		buffer8[7][i] = 0;
		buffer8[8][i] = 0;
		buffer8[9][i] = 0;
		buffer8[10][i] = 0;
		buffer8[11][i] = 0;
		buffer8[12][i] = 0;
		buffer8[13][i] = 0;
		buffer8[14][i] = 0;
		buffer8[15][i] = 0;
	
		buffer9[0][i] = 0;
		buffer9[1][i] = 0;
		buffer9[2][i] = 0;
		buffer9[3][i] = 0;
		buffer9[4][i] = 0;
		buffer9[5][i] = 0;
		buffer9[6][i] = 0;
		buffer9[7][i] = 0;
		buffer9[8][i] = 0;
		buffer9[9][i] = 0;
		buffer9[10][i] = 0;
		buffer9[11][i] = 0;
		buffer9[12][i] = 0;
		buffer9[13][i] = 0;
		buffer9[14][i] = 0;
		buffer9[15][i] = 0;
	
		buffer10[0][i] = 0;
		buffer10[1][i] = 0;
		buffer10[2][i] = 0;
		buffer10[3][i] = 0;
		buffer10[4][i] = 0;
		buffer10[5][i] = 0;
		buffer10[6][i] = 0;
		buffer10[7][i] = 0;
		buffer10[8][i] = 0;
		buffer10[9][i] = 0;
		buffer10[10][i] = 0;
		buffer10[11][i] = 0;
		buffer10[12][i] = 0;
		buffer10[13][i] = 0;
		buffer10[14][i] = 0;
		buffer10[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks11 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks11 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks11 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
			buffer8[0][bdepth_i + i] = vdata2[0];
			buffer8[1][bdepth_i + i] = vdata2[1];
			buffer8[2][bdepth_i + i] = vdata2[2];
			buffer8[3][bdepth_i + i] = vdata2[3];
			buffer8[4][bdepth_i + i] = vdata2[4];
			buffer8[5][bdepth_i + i] = vdata2[5];
			buffer8[6][bdepth_i + i] = vdata2[6];
			buffer8[7][bdepth_i + i] = vdata2[7];
			buffer8[8][bdepth_i + i] = vdata2[8];
			buffer8[9][bdepth_i + i] = vdata2[9];
			buffer8[10][bdepth_i + i] = vdata2[10];
			buffer8[11][bdepth_i + i] = vdata2[11];
			buffer8[12][bdepth_i + i] = vdata2[12];
			buffer8[13][bdepth_i + i] = vdata2[13];
			buffer8[14][bdepth_i + i] = vdata2[14];
			buffer8[15][bdepth_i + i] = vdata2[15];
			buffer9[0][bdepth_i + i] = vdata2[0];
			buffer9[1][bdepth_i + i] = vdata2[1];
			buffer9[2][bdepth_i + i] = vdata2[2];
			buffer9[3][bdepth_i + i] = vdata2[3];
			buffer9[4][bdepth_i + i] = vdata2[4];
			buffer9[5][bdepth_i + i] = vdata2[5];
			buffer9[6][bdepth_i + i] = vdata2[6];
			buffer9[7][bdepth_i + i] = vdata2[7];
			buffer9[8][bdepth_i + i] = vdata2[8];
			buffer9[9][bdepth_i + i] = vdata2[9];
			buffer9[10][bdepth_i + i] = vdata2[10];
			buffer9[11][bdepth_i + i] = vdata2[11];
			buffer9[12][bdepth_i + i] = vdata2[12];
			buffer9[13][bdepth_i + i] = vdata2[13];
			buffer9[14][bdepth_i + i] = vdata2[14];
			buffer9[15][bdepth_i + i] = vdata2[15];
			buffer10[0][bdepth_i + i] = vdata2[0];
			buffer10[1][bdepth_i + i] = vdata2[1];
			buffer10[2][bdepth_i + i] = vdata2[2];
			buffer10[3][bdepth_i + i] = vdata2[3];
			buffer10[4][bdepth_i + i] = vdata2[4];
			buffer10[5][bdepth_i + i] = vdata2[5];
			buffer10[6][bdepth_i + i] = vdata2[6];
			buffer10[7][bdepth_i + i] = vdata2[7];
			buffer10[8][bdepth_i + i] = vdata2[8];
			buffer10[9][bdepth_i + i] = vdata2[9];
			buffer10[10][bdepth_i + i] = vdata2[10];
			buffer10[11][bdepth_i + i] = vdata2[11];
			buffer10[12][bdepth_i + i] = vdata2[12];
			buffer10[13][bdepth_i + i] = vdata2[13];
			buffer10[14][bdepth_i + i] = vdata2[14];
			buffer10[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void MEMACCESSS_readANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// unsigned int depth = globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	#pragma HLS array_partition variable = buffer5
	#pragma HLS array_partition variable = buffer6
	#pragma HLS array_partition variable = buffer7
	#pragma HLS array_partition variable = buffer8
	#pragma HLS array_partition variable = buffer9
	#pragma HLS array_partition variable = buffer10
	#pragma HLS array_partition variable = buffer11
	
	#ifndef FPGA_IMPL
	for (buffer_type i=0; i<BLOCKRAM_VDATA_SECTION1; i++){ // REMOVEME.
	#pragma HLS PIPELINE II=1
		buffer0[0][i] = 0;
		buffer0[1][i] = 0;
		buffer0[2][i] = 0;
		buffer0[3][i] = 0;
		buffer0[4][i] = 0;
		buffer0[5][i] = 0;
		buffer0[6][i] = 0;
		buffer0[7][i] = 0;
		buffer0[8][i] = 0;
		buffer0[9][i] = 0;
		buffer0[10][i] = 0;
		buffer0[11][i] = 0;
		buffer0[12][i] = 0;
		buffer0[13][i] = 0;
		buffer0[14][i] = 0;
		buffer0[15][i] = 0;
	
		buffer1[0][i] = 0;
		buffer1[1][i] = 0;
		buffer1[2][i] = 0;
		buffer1[3][i] = 0;
		buffer1[4][i] = 0;
		buffer1[5][i] = 0;
		buffer1[6][i] = 0;
		buffer1[7][i] = 0;
		buffer1[8][i] = 0;
		buffer1[9][i] = 0;
		buffer1[10][i] = 0;
		buffer1[11][i] = 0;
		buffer1[12][i] = 0;
		buffer1[13][i] = 0;
		buffer1[14][i] = 0;
		buffer1[15][i] = 0;
	
		buffer2[0][i] = 0;
		buffer2[1][i] = 0;
		buffer2[2][i] = 0;
		buffer2[3][i] = 0;
		buffer2[4][i] = 0;
		buffer2[5][i] = 0;
		buffer2[6][i] = 0;
		buffer2[7][i] = 0;
		buffer2[8][i] = 0;
		buffer2[9][i] = 0;
		buffer2[10][i] = 0;
		buffer2[11][i] = 0;
		buffer2[12][i] = 0;
		buffer2[13][i] = 0;
		buffer2[14][i] = 0;
		buffer2[15][i] = 0;
	
		buffer3[0][i] = 0;
		buffer3[1][i] = 0;
		buffer3[2][i] = 0;
		buffer3[3][i] = 0;
		buffer3[4][i] = 0;
		buffer3[5][i] = 0;
		buffer3[6][i] = 0;
		buffer3[7][i] = 0;
		buffer3[8][i] = 0;
		buffer3[9][i] = 0;
		buffer3[10][i] = 0;
		buffer3[11][i] = 0;
		buffer3[12][i] = 0;
		buffer3[13][i] = 0;
		buffer3[14][i] = 0;
		buffer3[15][i] = 0;
	
		buffer4[0][i] = 0;
		buffer4[1][i] = 0;
		buffer4[2][i] = 0;
		buffer4[3][i] = 0;
		buffer4[4][i] = 0;
		buffer4[5][i] = 0;
		buffer4[6][i] = 0;
		buffer4[7][i] = 0;
		buffer4[8][i] = 0;
		buffer4[9][i] = 0;
		buffer4[10][i] = 0;
		buffer4[11][i] = 0;
		buffer4[12][i] = 0;
		buffer4[13][i] = 0;
		buffer4[14][i] = 0;
		buffer4[15][i] = 0;
	
		buffer5[0][i] = 0;
		buffer5[1][i] = 0;
		buffer5[2][i] = 0;
		buffer5[3][i] = 0;
		buffer5[4][i] = 0;
		buffer5[5][i] = 0;
		buffer5[6][i] = 0;
		buffer5[7][i] = 0;
		buffer5[8][i] = 0;
		buffer5[9][i] = 0;
		buffer5[10][i] = 0;
		buffer5[11][i] = 0;
		buffer5[12][i] = 0;
		buffer5[13][i] = 0;
		buffer5[14][i] = 0;
		buffer5[15][i] = 0;
	
		buffer6[0][i] = 0;
		buffer6[1][i] = 0;
		buffer6[2][i] = 0;
		buffer6[3][i] = 0;
		buffer6[4][i] = 0;
		buffer6[5][i] = 0;
		buffer6[6][i] = 0;
		buffer6[7][i] = 0;
		buffer6[8][i] = 0;
		buffer6[9][i] = 0;
		buffer6[10][i] = 0;
		buffer6[11][i] = 0;
		buffer6[12][i] = 0;
		buffer6[13][i] = 0;
		buffer6[14][i] = 0;
		buffer6[15][i] = 0;
	
		buffer7[0][i] = 0;
		buffer7[1][i] = 0;
		buffer7[2][i] = 0;
		buffer7[3][i] = 0;
		buffer7[4][i] = 0;
		buffer7[5][i] = 0;
		buffer7[6][i] = 0;
		buffer7[7][i] = 0;
		buffer7[8][i] = 0;
		buffer7[9][i] = 0;
		buffer7[10][i] = 0;
		buffer7[11][i] = 0;
		buffer7[12][i] = 0;
		buffer7[13][i] = 0;
		buffer7[14][i] = 0;
		buffer7[15][i] = 0;
	
		buffer8[0][i] = 0;
		buffer8[1][i] = 0;
		buffer8[2][i] = 0;
		buffer8[3][i] = 0;
		buffer8[4][i] = 0;
		buffer8[5][i] = 0;
		buffer8[6][i] = 0;
		buffer8[7][i] = 0;
		buffer8[8][i] = 0;
		buffer8[9][i] = 0;
		buffer8[10][i] = 0;
		buffer8[11][i] = 0;
		buffer8[12][i] = 0;
		buffer8[13][i] = 0;
		buffer8[14][i] = 0;
		buffer8[15][i] = 0;
	
		buffer9[0][i] = 0;
		buffer9[1][i] = 0;
		buffer9[2][i] = 0;
		buffer9[3][i] = 0;
		buffer9[4][i] = 0;
		buffer9[5][i] = 0;
		buffer9[6][i] = 0;
		buffer9[7][i] = 0;
		buffer9[8][i] = 0;
		buffer9[9][i] = 0;
		buffer9[10][i] = 0;
		buffer9[11][i] = 0;
		buffer9[12][i] = 0;
		buffer9[13][i] = 0;
		buffer9[14][i] = 0;
		buffer9[15][i] = 0;
	
		buffer10[0][i] = 0;
		buffer10[1][i] = 0;
		buffer10[2][i] = 0;
		buffer10[3][i] = 0;
		buffer10[4][i] = 0;
		buffer10[5][i] = 0;
		buffer10[6][i] = 0;
		buffer10[7][i] = 0;
		buffer10[8][i] = 0;
		buffer10[9][i] = 0;
		buffer10[10][i] = 0;
		buffer10[11][i] = 0;
		buffer10[12][i] = 0;
		buffer10[13][i] = 0;
		buffer10[14][i] = 0;
		buffer10[15][i] = 0;
	
		buffer11[0][i] = 0;
		buffer11[1][i] = 0;
		buffer11[2][i] = 0;
		buffer11[3][i] = 0;
		buffer11[4][i] = 0;
		buffer11[5][i] = 0;
		buffer11[6][i] = 0;
		buffer11[7][i] = 0;
		buffer11[8][i] = 0;
		buffer11[9][i] = 0;
		buffer11[10][i] = 0;
		buffer11[11][i] = 0;
		buffer11[12][i] = 0;
		buffer11[13][i] = 0;
		buffer11[14][i] = 0;
		buffer11[15][i] = 0;
	
	}
	#endif
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks12 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks12 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			#ifdef _WIDEWORD
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(31, 0); 
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(63, 32); 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(95, 64); 
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(127, 96); 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(159, 128); 
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(191, 160); 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(223, 192); 
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(255, 224); 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(287, 256); 
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(319, 288); 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(351, 320); 
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(383, 352); 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(415, 384); 
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(447, 416); 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(479, 448); 
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].range(511, 480); 
			#else 
			vdata[0] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].key;
			vdata[1] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[0].value; 
			vdata[2] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].key;
			vdata[3] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[1].value; 
			vdata[4] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].key;
			vdata[5] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[2].value; 
			vdata[6] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].key;
			vdata[7] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[3].value; 
			vdata[8] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].key;
			vdata[9] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[4].value; 
			vdata[10] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].key;
			vdata[11] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[5].value; 
			vdata[12] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].key;
			vdata[13] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[6].value; 
			vdata[14] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].key;
			vdata[15] = vdram[vbaseoffset_kvs + voffset_kvs + depth_i + i].data[7].value; 
			#endif
			
			#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
			MEMACCESSS_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAS_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAS_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks12 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata2[0];
			buffer0[1][bdepth_i + i] = vdata2[1];
			buffer0[2][bdepth_i + i] = vdata2[2];
			buffer0[3][bdepth_i + i] = vdata2[3];
			buffer0[4][bdepth_i + i] = vdata2[4];
			buffer0[5][bdepth_i + i] = vdata2[5];
			buffer0[6][bdepth_i + i] = vdata2[6];
			buffer0[7][bdepth_i + i] = vdata2[7];
			buffer0[8][bdepth_i + i] = vdata2[8];
			buffer0[9][bdepth_i + i] = vdata2[9];
			buffer0[10][bdepth_i + i] = vdata2[10];
			buffer0[11][bdepth_i + i] = vdata2[11];
			buffer0[12][bdepth_i + i] = vdata2[12];
			buffer0[13][bdepth_i + i] = vdata2[13];
			buffer0[14][bdepth_i + i] = vdata2[14];
			buffer0[15][bdepth_i + i] = vdata2[15];
			buffer1[0][bdepth_i + i] = vdata2[0];
			buffer1[1][bdepth_i + i] = vdata2[1];
			buffer1[2][bdepth_i + i] = vdata2[2];
			buffer1[3][bdepth_i + i] = vdata2[3];
			buffer1[4][bdepth_i + i] = vdata2[4];
			buffer1[5][bdepth_i + i] = vdata2[5];
			buffer1[6][bdepth_i + i] = vdata2[6];
			buffer1[7][bdepth_i + i] = vdata2[7];
			buffer1[8][bdepth_i + i] = vdata2[8];
			buffer1[9][bdepth_i + i] = vdata2[9];
			buffer1[10][bdepth_i + i] = vdata2[10];
			buffer1[11][bdepth_i + i] = vdata2[11];
			buffer1[12][bdepth_i + i] = vdata2[12];
			buffer1[13][bdepth_i + i] = vdata2[13];
			buffer1[14][bdepth_i + i] = vdata2[14];
			buffer1[15][bdepth_i + i] = vdata2[15];
			buffer2[0][bdepth_i + i] = vdata2[0];
			buffer2[1][bdepth_i + i] = vdata2[1];
			buffer2[2][bdepth_i + i] = vdata2[2];
			buffer2[3][bdepth_i + i] = vdata2[3];
			buffer2[4][bdepth_i + i] = vdata2[4];
			buffer2[5][bdepth_i + i] = vdata2[5];
			buffer2[6][bdepth_i + i] = vdata2[6];
			buffer2[7][bdepth_i + i] = vdata2[7];
			buffer2[8][bdepth_i + i] = vdata2[8];
			buffer2[9][bdepth_i + i] = vdata2[9];
			buffer2[10][bdepth_i + i] = vdata2[10];
			buffer2[11][bdepth_i + i] = vdata2[11];
			buffer2[12][bdepth_i + i] = vdata2[12];
			buffer2[13][bdepth_i + i] = vdata2[13];
			buffer2[14][bdepth_i + i] = vdata2[14];
			buffer2[15][bdepth_i + i] = vdata2[15];
			buffer3[0][bdepth_i + i] = vdata2[0];
			buffer3[1][bdepth_i + i] = vdata2[1];
			buffer3[2][bdepth_i + i] = vdata2[2];
			buffer3[3][bdepth_i + i] = vdata2[3];
			buffer3[4][bdepth_i + i] = vdata2[4];
			buffer3[5][bdepth_i + i] = vdata2[5];
			buffer3[6][bdepth_i + i] = vdata2[6];
			buffer3[7][bdepth_i + i] = vdata2[7];
			buffer3[8][bdepth_i + i] = vdata2[8];
			buffer3[9][bdepth_i + i] = vdata2[9];
			buffer3[10][bdepth_i + i] = vdata2[10];
			buffer3[11][bdepth_i + i] = vdata2[11];
			buffer3[12][bdepth_i + i] = vdata2[12];
			buffer3[13][bdepth_i + i] = vdata2[13];
			buffer3[14][bdepth_i + i] = vdata2[14];
			buffer3[15][bdepth_i + i] = vdata2[15];
			buffer4[0][bdepth_i + i] = vdata2[0];
			buffer4[1][bdepth_i + i] = vdata2[1];
			buffer4[2][bdepth_i + i] = vdata2[2];
			buffer4[3][bdepth_i + i] = vdata2[3];
			buffer4[4][bdepth_i + i] = vdata2[4];
			buffer4[5][bdepth_i + i] = vdata2[5];
			buffer4[6][bdepth_i + i] = vdata2[6];
			buffer4[7][bdepth_i + i] = vdata2[7];
			buffer4[8][bdepth_i + i] = vdata2[8];
			buffer4[9][bdepth_i + i] = vdata2[9];
			buffer4[10][bdepth_i + i] = vdata2[10];
			buffer4[11][bdepth_i + i] = vdata2[11];
			buffer4[12][bdepth_i + i] = vdata2[12];
			buffer4[13][bdepth_i + i] = vdata2[13];
			buffer4[14][bdepth_i + i] = vdata2[14];
			buffer4[15][bdepth_i + i] = vdata2[15];
			buffer5[0][bdepth_i + i] = vdata2[0];
			buffer5[1][bdepth_i + i] = vdata2[1];
			buffer5[2][bdepth_i + i] = vdata2[2];
			buffer5[3][bdepth_i + i] = vdata2[3];
			buffer5[4][bdepth_i + i] = vdata2[4];
			buffer5[5][bdepth_i + i] = vdata2[5];
			buffer5[6][bdepth_i + i] = vdata2[6];
			buffer5[7][bdepth_i + i] = vdata2[7];
			buffer5[8][bdepth_i + i] = vdata2[8];
			buffer5[9][bdepth_i + i] = vdata2[9];
			buffer5[10][bdepth_i + i] = vdata2[10];
			buffer5[11][bdepth_i + i] = vdata2[11];
			buffer5[12][bdepth_i + i] = vdata2[12];
			buffer5[13][bdepth_i + i] = vdata2[13];
			buffer5[14][bdepth_i + i] = vdata2[14];
			buffer5[15][bdepth_i + i] = vdata2[15];
			buffer6[0][bdepth_i + i] = vdata2[0];
			buffer6[1][bdepth_i + i] = vdata2[1];
			buffer6[2][bdepth_i + i] = vdata2[2];
			buffer6[3][bdepth_i + i] = vdata2[3];
			buffer6[4][bdepth_i + i] = vdata2[4];
			buffer6[5][bdepth_i + i] = vdata2[5];
			buffer6[6][bdepth_i + i] = vdata2[6];
			buffer6[7][bdepth_i + i] = vdata2[7];
			buffer6[8][bdepth_i + i] = vdata2[8];
			buffer6[9][bdepth_i + i] = vdata2[9];
			buffer6[10][bdepth_i + i] = vdata2[10];
			buffer6[11][bdepth_i + i] = vdata2[11];
			buffer6[12][bdepth_i + i] = vdata2[12];
			buffer6[13][bdepth_i + i] = vdata2[13];
			buffer6[14][bdepth_i + i] = vdata2[14];
			buffer6[15][bdepth_i + i] = vdata2[15];
			buffer7[0][bdepth_i + i] = vdata2[0];
			buffer7[1][bdepth_i + i] = vdata2[1];
			buffer7[2][bdepth_i + i] = vdata2[2];
			buffer7[3][bdepth_i + i] = vdata2[3];
			buffer7[4][bdepth_i + i] = vdata2[4];
			buffer7[5][bdepth_i + i] = vdata2[5];
			buffer7[6][bdepth_i + i] = vdata2[6];
			buffer7[7][bdepth_i + i] = vdata2[7];
			buffer7[8][bdepth_i + i] = vdata2[8];
			buffer7[9][bdepth_i + i] = vdata2[9];
			buffer7[10][bdepth_i + i] = vdata2[10];
			buffer7[11][bdepth_i + i] = vdata2[11];
			buffer7[12][bdepth_i + i] = vdata2[12];
			buffer7[13][bdepth_i + i] = vdata2[13];
			buffer7[14][bdepth_i + i] = vdata2[14];
			buffer7[15][bdepth_i + i] = vdata2[15];
			buffer8[0][bdepth_i + i] = vdata2[0];
			buffer8[1][bdepth_i + i] = vdata2[1];
			buffer8[2][bdepth_i + i] = vdata2[2];
			buffer8[3][bdepth_i + i] = vdata2[3];
			buffer8[4][bdepth_i + i] = vdata2[4];
			buffer8[5][bdepth_i + i] = vdata2[5];
			buffer8[6][bdepth_i + i] = vdata2[6];
			buffer8[7][bdepth_i + i] = vdata2[7];
			buffer8[8][bdepth_i + i] = vdata2[8];
			buffer8[9][bdepth_i + i] = vdata2[9];
			buffer8[10][bdepth_i + i] = vdata2[10];
			buffer8[11][bdepth_i + i] = vdata2[11];
			buffer8[12][bdepth_i + i] = vdata2[12];
			buffer8[13][bdepth_i + i] = vdata2[13];
			buffer8[14][bdepth_i + i] = vdata2[14];
			buffer8[15][bdepth_i + i] = vdata2[15];
			buffer9[0][bdepth_i + i] = vdata2[0];
			buffer9[1][bdepth_i + i] = vdata2[1];
			buffer9[2][bdepth_i + i] = vdata2[2];
			buffer9[3][bdepth_i + i] = vdata2[3];
			buffer9[4][bdepth_i + i] = vdata2[4];
			buffer9[5][bdepth_i + i] = vdata2[5];
			buffer9[6][bdepth_i + i] = vdata2[6];
			buffer9[7][bdepth_i + i] = vdata2[7];
			buffer9[8][bdepth_i + i] = vdata2[8];
			buffer9[9][bdepth_i + i] = vdata2[9];
			buffer9[10][bdepth_i + i] = vdata2[10];
			buffer9[11][bdepth_i + i] = vdata2[11];
			buffer9[12][bdepth_i + i] = vdata2[12];
			buffer9[13][bdepth_i + i] = vdata2[13];
			buffer9[14][bdepth_i + i] = vdata2[14];
			buffer9[15][bdepth_i + i] = vdata2[15];
			buffer10[0][bdepth_i + i] = vdata2[0];
			buffer10[1][bdepth_i + i] = vdata2[1];
			buffer10[2][bdepth_i + i] = vdata2[2];
			buffer10[3][bdepth_i + i] = vdata2[3];
			buffer10[4][bdepth_i + i] = vdata2[4];
			buffer10[5][bdepth_i + i] = vdata2[5];
			buffer10[6][bdepth_i + i] = vdata2[6];
			buffer10[7][bdepth_i + i] = vdata2[7];
			buffer10[8][bdepth_i + i] = vdata2[8];
			buffer10[9][bdepth_i + i] = vdata2[9];
			buffer10[10][bdepth_i + i] = vdata2[10];
			buffer10[11][bdepth_i + i] = vdata2[11];
			buffer10[12][bdepth_i + i] = vdata2[12];
			buffer10[13][bdepth_i + i] = vdata2[13];
			buffer10[14][bdepth_i + i] = vdata2[14];
			buffer10[15][bdepth_i + i] = vdata2[15];
			buffer11[0][bdepth_i + i] = vdata2[0];
			buffer11[1][bdepth_i + i] = vdata2[1];
			buffer11[2][bdepth_i + i] = vdata2[2];
			buffer11[3][bdepth_i + i] = vdata2[3];
			buffer11[4][bdepth_i + i] = vdata2[4];
			buffer11[5][bdepth_i + i] = vdata2[5];
			buffer11[6][bdepth_i + i] = vdata2[6];
			buffer11[7][bdepth_i + i] = vdata2[7];
			buffer11[8][bdepth_i + i] = vdata2[8];
			buffer11[9][bdepth_i + i] = vdata2[9];
			buffer11[10][bdepth_i + i] = vdata2[10];
			buffer11[11][bdepth_i + i] = vdata2[11];
			buffer11[12][bdepth_i + i] = vdata2[12];
			buffer11[13][bdepth_i + i] = vdata2[13];
			buffer11[14][bdepth_i + i] = vdata2[14];
			buffer11[15][bdepth_i + i] = vdata2[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
		

// -------------------- stats -------------------- //
void MEMACCESSS_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_TOTALDRAMCAPACITY_KVS + 1, NAp, NAp, NAp);
	#endif
	
	unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ _NUM_PARTITIONS = UTILS_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
	#endif 

	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<_NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt vec;
		#ifdef _WIDEWORD
		vec.data[0].key = kvdram[offset_kvs + i].range(31, 0); 
		vec.data[0].value = kvdram[offset_kvs + i].range(63, 32); 
		vec.data[1].key = kvdram[offset_kvs + i].range(95, 64); 
		vec.data[1].value = kvdram[offset_kvs + i].range(127, 96); 
		vec.data[2].key = kvdram[offset_kvs + i].range(159, 128); 
		vec.data[2].value = kvdram[offset_kvs + i].range(191, 160); 
		vec.data[3].key = kvdram[offset_kvs + i].range(223, 192); 
		vec.data[3].value = kvdram[offset_kvs + i].range(255, 224); 
		vec.data[4].key = kvdram[offset_kvs + i].range(287, 256); 
		vec.data[4].value = kvdram[offset_kvs + i].range(319, 288); 
		#else 
		vec.data[0].key = kvdram[offset_kvs + i].data[0].key; 
		vec.data[0].value = kvdram[offset_kvs + i].data[0].value; 
		vec.data[1].key = kvdram[offset_kvs + i].data[1].key; 
		vec.data[1].value = kvdram[offset_kvs + i].data[1].value; 
		vec.data[2].key = kvdram[offset_kvs + i].data[2].key; 
		vec.data[2].value = kvdram[offset_kvs + i].data[2].value; 
		vec.data[3].key = kvdram[offset_kvs + i].data[3].key; 
		vec.data[3].value = kvdram[offset_kvs + i].data[3].value; 
		vec.data[4].key = kvdram[offset_kvs + i].data[4].key; 
		vec.data[4].value = kvdram[offset_kvs + i].data[4].value; 
		#endif 
		globalstatsbuffer[i] = vec.data[globalparams.VARS_WORKBATCH];
	}
	
	#ifdef _DEBUGMODE_CHECKS3
	for(unsigned int i=0; i<_NUM_PARTITIONS; i++){
		if(globalstatsbuffer[i].key + globalstatsbuffer[i].value >= globalparams.SIZE_KVDRAM){
			cout<<"MEMACCESSS_readglobalstats 36. ERROR. out of bounds. (globalstatsbuffer["<<i<<"].key("<<globalstatsbuffer[i].key<<") + globalstatsbuffer["<<i<<"].value("<<globalstatsbuffer[i].value<<") >= globalparams.SIZE_KVDRAM("<<globalparams.SIZE_KVDRAM<<")). offset_kvs: "<<offset_kvs<<". printing and exiting..."<<endl; 
			actsutilityobj->printkeyvalues("MEMACCESSS_readglobalstats 37", (keyvalue_t *)globalstatsbuffer, _NUM_PARTITIONS); 
			exit(EXIT_FAILURE); 
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("readglobalstats.globalstatsbuffer", globalstatsbuffer, _NUM_PARTITIONS); 
	#endif
	// exit(EXIT_SUCCESS); 
	return;
}

void MEMACCESSS_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, offset_kvs, globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_TOTALDRAMCAPACITY_KVS);
	#endif
	
	// cout<<"--------------------------------------------------------------- MEMACCESSS_saveglobalstats -------------------------------------------------------------"<<endl;
	
	unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ _NUM_PARTITIONS = UTILS_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
	#endif 
	
	SAVEGLOBALSTATS_LOOP: for (buffer_type i=0; i<_NUM_PARTITIONS; i++){
 if(globalparams.VARS_WORKBATCH == 0){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(31, 0) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(63, 32) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[0].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[0].value = globalstatsbuffer[i].value; 
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE // _DEBUGMODE_KERNELPRINTS_TRACE3
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSS_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
			#endif
		}
else if(globalparams.VARS_WORKBATCH == 1){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(95, 64) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(127, 96) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[1].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[1].value = globalstatsbuffer[i].value; 
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE // _DEBUGMODE_KERNELPRINTS_TRACE3
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSS_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
			#endif
		}
else if(globalparams.VARS_WORKBATCH == 2){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(159, 128) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(191, 160) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[2].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[2].value = globalstatsbuffer[i].value; 
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE // _DEBUGMODE_KERNELPRINTS_TRACE3
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSS_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
			#endif
		}
else if(globalparams.VARS_WORKBATCH == 3){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(223, 192) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(255, 224) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[3].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[3].value = globalstatsbuffer[i].value; 
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE // _DEBUGMODE_KERNELPRINTS_TRACE3
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSS_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
			#endif
		}
else if(globalparams.VARS_WORKBATCH == 4){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(287, 256) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(319, 288) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[4].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[4].value = globalstatsbuffer[i].value; 
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE // _DEBUGMODE_KERNELPRINTS_TRACE3
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSS_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
			#endif
		}
		else {
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"MEMACCESSS_saveglobalstats: NOT IMPLEMENTED (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("MEMACCESSS_saveglobalstats.globalstatsbuffer", globalstatsbuffer, _NUM_PARTITIONS); 
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

tuple_t MEMACCESSS_getvptrs(uint512_dt * kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
	#pragma HLS INLINE
	
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;

	#ifdef _WIDEWORD
	beginvptr = kvdram[beginoffset].range(31, 0);
	endvptr = kvdram[endoffset].range(31, 0); 
	#else 
	beginvptr = kvdram[beginoffset].data[0].key;
	endvptr = kvdram[endoffset].data[0].key;
	#endif 

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

unsigned int MEMACCESSS_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
	keyvalue_t vptr_kv;
	
	uint512_dt V = kvdram[baseoffset_kvs + (offset / VECTOR2_SIZE)];
	unsigned int M = (offset % VECTOR2_SIZE) / 2;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSS_getvptr: baseoffset_kvs: "<<baseoffset_kvs<<", offset: "<<offset<<", baseoffset_kvs + (offset / VECTOR2_SIZE): "<<baseoffset_kvs + (offset / VECTOR2_SIZE)<<endl;
	
	#ifdef _WIDEWORD
 if(M == 0){
		vptr_kv.key = V.range(31, 0); 
		vptr_kv.value = V.range(63, 32); 
	}
else if(M == 1){
		vptr_kv.key = V.range(95, 64); 
		vptr_kv.value = V.range(127, 96); 
	}
else if(M == 2){
		vptr_kv.key = V.range(159, 128); 
		vptr_kv.value = V.range(191, 160); 
	}
else if(M == 3){
		vptr_kv.key = V.range(223, 192); 
		vptr_kv.value = V.range(255, 224); 
	}
else if(M == 4){
		vptr_kv.key = V.range(287, 256); 
		vptr_kv.value = V.range(319, 288); 
	}
else if(M == 5){
		vptr_kv.key = V.range(351, 320); 
		vptr_kv.value = V.range(383, 352); 
	}
else if(M == 6){
		vptr_kv.key = V.range(415, 384); 
		vptr_kv.value = V.range(447, 416); 
	}
else {
		vptr_kv.key = V.range(479, 448); 
		vptr_kv.value = V.range(511, 480); 
	}
	#else 
 if(M == 0){
		vptr_kv.key = V.data[0].key; 
		vptr_kv.value = V.data[0].value; 
	}
else if(M == 1){
		vptr_kv.key = V.data[1].key; 
		vptr_kv.value = V.data[1].value; 
	}
else if(M == 2){
		vptr_kv.key = V.data[2].key; 
		vptr_kv.value = V.data[2].value; 
	}
else if(M == 3){
		vptr_kv.key = V.data[3].key; 
		vptr_kv.value = V.data[3].value; 
	}
else if(M == 4){
		vptr_kv.key = V.data[4].key; 
		vptr_kv.value = V.data[4].value; 
	}
else if(M == 5){
		vptr_kv.key = V.data[5].key; 
		vptr_kv.value = V.data[5].value; 
	}
else if(M == 6){
		vptr_kv.key = V.data[6].key; 
		vptr_kv.value = V.data[6].value; 
	}
else {
		vptr_kv.key = V.data[7].key; 
		vptr_kv.value = V.data[7].value; 
	}
	#endif
	
	if(offset % 2 == 0){ return vptr_kv.key; }
	else { return vptr_kv.value; }
}

tuple_t MEMACCESSS_getvptrs_opt(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID, globalparams_t globalparams){
	#pragma HLS INLINE 
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	beginoffset = beginoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO; // convert-to-appropriate-skip-format
	endoffset = endoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSS_getvptrs_opt: beginoffset: "<<beginoffset<<", endoffset: "<<endoffset<<", VPTR_SHRINK_RATIO: "<<VPTR_SHRINK_RATIO<<endl;
	
	beginvptr = MEMACCESSS_getvptr(kvdram, baseoffset_kvs, beginoffset);
	endvptr = MEMACCESSS_getvptr(kvdram, baseoffset_kvs, endoffset);

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

void MEMACCESSS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset){
	unsigned int totalnumpartitionsb4last = 0;
	RETRIEVEKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset  + k].data[0].value = buffer[k]; 
		#endif
	}
	
	#ifdef _WIDEWORD
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	#else
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}

void MEMACCESSS_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset  + k].data[0].value = buffer[k]; 
		#endif
	}
	return;
}

void MEMACCESSS_commitkvstats2(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k].key; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value = buffer[k].key; 
		#endif
	}
	return;
}

void MEMACCESSS_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[k] = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32); 
		#else
		buffer[k] = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value; 
		#endif
	}
}

void MEMACCESSS_retreievekvstats(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[k].value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32); 
		#else
		buffer[k].value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value; 
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		if(buffer[k].value>0){ cout<<"MEMACCESSS_saveglobalstats:: active partition retreieved @ partition "<<k<<", offset: "<<offset + k<<"."<<endl; }	
		#endif
	}
}

void MEMACCESSS_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	SAVEKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }
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
			cout<<"MEMACCESSS_accumkvstats: NOT IMPLEMENTED (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}

void MEMACCESSS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
	unsigned int totalnumpartitionsb4last = 0;
	RETRIEVEKVSTATS_LOOP1: for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ totalnumpartitionsb4last += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }
	for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[0].value = buffer[k]; 
		#endif
	}
	
	#ifdef _WIDEWORD
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	#else
	kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}

// -------------------- multiple accesses -------------------- //
void MEMACCESSS_readpmask(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, unsigned int _ACTSPARAMS_INSTID, globalparams_t globalparams){
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD // CRITICAL FIXME.
		vdata[0] = vdram[offset_kvs + i].range(31, 0); 
		vdata[1] = vdram[offset_kvs + i].range(63, 32); 
		vdata[2] = vdram[offset_kvs + i].range(95, 64); 
		vdata[3] = vdram[offset_kvs + i].range(127, 96); 
		vdata[4] = vdram[offset_kvs + i].range(159, 128); 
		vdata[5] = vdram[offset_kvs + i].range(191, 160); 
		vdata[6] = vdram[offset_kvs + i].range(223, 192); 
		vdata[7] = vdram[offset_kvs + i].range(255, 224); 
		vdata[8] = vdram[offset_kvs + i].range(287, 256); 
		vdata[9] = vdram[offset_kvs + i].range(319, 288); 
		vdata[10] = vdram[offset_kvs + i].range(351, 320); 
		vdata[11] = vdram[offset_kvs + i].range(383, 352); 
		vdata[12] = vdram[offset_kvs + i].range(415, 384); 
		vdata[13] = vdram[offset_kvs + i].range(447, 416); 
		vdata[14] = vdram[offset_kvs + i].range(479, 448); 
		vdata[15] = vdram[offset_kvs + i].range(511, 480); 
		#else 
		vdata[0] = vdram[offset_kvs + i].data[0].key;
		vdata[1] = vdram[offset_kvs + i].data[0].value; 
		vdata[2] = vdram[offset_kvs + i].data[1].key;
		vdata[3] = vdram[offset_kvs + i].data[1].value; 
		vdata[4] = vdram[offset_kvs + i].data[2].key;
		vdata[5] = vdram[offset_kvs + i].data[2].value; 
		vdata[6] = vdram[offset_kvs + i].data[3].key;
		vdata[7] = vdram[offset_kvs + i].data[3].value; 
		vdata[8] = vdram[offset_kvs + i].data[4].key;
		vdata[9] = vdram[offset_kvs + i].data[4].value; 
		vdata[10] = vdram[offset_kvs + i].data[5].key;
		vdata[11] = vdram[offset_kvs + i].data[5].value; 
		vdata[12] = vdram[offset_kvs + i].data[6].key;
		vdata[13] = vdram[offset_kvs + i].data[6].value; 
		vdata[14] = vdram[offset_kvs + i].data[7].key;
		vdata[15] = vdram[offset_kvs + i].data[7].value; 
		#endif
		
		pmask[i] = vdata[GraphIter];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	// if(_ACTSPARAMS_INSTID == 0){
		cout<< TIMINGRESULTSCOLOR << ">>> readpmasks: printing active vertex partitions: GraphIter: "<<GraphIter<<": ";
		unsigned int num_actvps = 0;
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){
			if(pmask[t] > 0  && t < 16){ cout<<t<<", "; }
			if(pmask[t] > 0){ num_actvps += 1; }
		}
		cout<<" ("<<num_actvps<<" active partitions, "<<globalparams.NUM_PROCESSEDGESPARTITIONS<<" total partitions)"<<endl;
		cout<< RESET << endl;
	// }
	#endif 
	return;
}









	
#endif 
#ifdef CONFIG_ENABLECLASS_PROCESSEDGES
void PROCESSS_processvector(bool enx, unsigned int v, unsigned int loc, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[MAX_SOURCEBLOCKRAM_SIZE], unsigned int bufferoffset_kvs, unsigned int * loadcount, unsigned int GraphAlgoClass, globalposition_t globalposition, globalparams_t globalparams	
		#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
		,collection_t collections[COLLECTIONS_BUFFERSZ]
		#endif 
		){				
	#pragma HLS INLINE
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && en == true){
		#ifdef _DEBUGMODE_CHECKS
		if(true){ cout<<"PROCESSS_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#endif 
		loc = 0; }
	
	// read 
	vmdata_t vmdata;
	keyvalue_vbuffer_t bits_vector = vbuffer[bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO)];
	if(en == true){ vmdata = MEMCAS_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
	vmdata.vmask = 1; // FIXME. REMOVEME.
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", en: "<<en<<", vid: "<<UTILS_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
			
	// process
	value_t res = PROCESSS_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	keyvalue_t mykeyvalue; mykeyvalue.key = edata.key; mykeyvalue.value = res;
	
	// write
	if(en == true && vmdata.vmask == 1 && *loadcount < globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2){ buffer[*loadcount] = UTILS_GETKV(mykeyvalue); }
	if(en == true && vmdata.vmask == 1 && *loadcount < globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2){ *loadcount += 1; } // CRITICAL FIXME. globalparams.ACTSPARAMS_WORKBUFFER_SIZE
	
	#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
	collections[0].data1 += 1;
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1);
	if(en == true && vmdata.vmask == 1 && *loadcount < globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1); } // mask0? FIXME.
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("PROCESSS_processvector::DEBUG CODE 14::1", *loadcount, MAX_SRCBUFFER_SIZE, MAX_SOURCEBLOCKRAM_SIZE, MAX_SRCBUFFER_SIZE, NAp); // MAX_SOURCEBLOCKRAM_SIZE
	#endif
	return;
}

void PROCESSS_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth){
	unsigned int s_ = s % VECTOR_SIZE;
	
 if(s_==0){ 
		depths[0] = 0; 
		depths[1] = 1; 
		depths[2] = 2; 
		depths[3] = 3; 
		depths[4] = 4; 
		depths[5] = 5; 
		depths[6] = 6; 
		depths[7] = 7; 
	}
else if(s_==1){ 
		depths[0] = 7; 
		depths[1] = 0; 
		depths[2] = 1; 
		depths[3] = 2; 
		depths[4] = 3; 
		depths[5] = 4; 
		depths[6] = 5; 
		depths[7] = 6; 
	}
else if(s_==2){ 
		depths[0] = 6; 
		depths[1] = 7; 
		depths[2] = 0; 
		depths[3] = 1; 
		depths[4] = 2; 
		depths[5] = 3; 
		depths[6] = 4; 
		depths[7] = 5; 
	}
else if(s_==3){ 
		depths[0] = 5; 
		depths[1] = 6; 
		depths[2] = 7; 
		depths[3] = 0; 
		depths[4] = 1; 
		depths[5] = 2; 
		depths[6] = 3; 
		depths[7] = 4; 
	}
else if(s_==4){ 
		depths[0] = 4; 
		depths[1] = 5; 
		depths[2] = 6; 
		depths[3] = 7; 
		depths[4] = 0; 
		depths[5] = 1; 
		depths[6] = 2; 
		depths[7] = 3; 
	}
else if(s_==5){ 
		depths[0] = 3; 
		depths[1] = 4; 
		depths[2] = 5; 
		depths[3] = 6; 
		depths[4] = 7; 
		depths[5] = 0; 
		depths[6] = 1; 
		depths[7] = 2; 
	}
else if(s_==6){ 
		depths[0] = 2; 
		depths[1] = 3; 
		depths[2] = 4; 
		depths[3] = 5; 
		depths[4] = 6; 
		depths[5] = 7; 
		depths[6] = 0; 
		depths[7] = 1; 
	}
else { 
		depths[0] = 1; 
		depths[1] = 2; 
		depths[2] = 3; 
		depths[3] = 4; 
		depths[4] = 5; 
		depths[5] = 6; 
		depths[6] = 7; 
		depths[7] = 0; 
	}
	return;
}

void PROCESSS_RearrangeLayoutV(unsigned int s, uint32_type vdata[VECTOR_SIZE], uint32_type vdata2[VECTOR_SIZE]){
	unsigned int s_ = s;// % VECTOR_SIZE;
 if(s_==0){ 
		vdata2[0] = vdata[0]; 
		vdata2[1] = vdata[1]; 
		vdata2[2] = vdata[2]; 
		vdata2[3] = vdata[3]; 
		vdata2[4] = vdata[4]; 
		vdata2[5] = vdata[5]; 
		vdata2[6] = vdata[6]; 
		vdata2[7] = vdata[7]; 
	}
else if(s_==1){ 
		vdata2[7] = vdata[0]; 
		vdata2[0] = vdata[1]; 
		vdata2[1] = vdata[2]; 
		vdata2[2] = vdata[3]; 
		vdata2[3] = vdata[4]; 
		vdata2[4] = vdata[5]; 
		vdata2[5] = vdata[6]; 
		vdata2[6] = vdata[7]; 
	}
else if(s_==2){ 
		vdata2[6] = vdata[0]; 
		vdata2[7] = vdata[1]; 
		vdata2[0] = vdata[2]; 
		vdata2[1] = vdata[3]; 
		vdata2[2] = vdata[4]; 
		vdata2[3] = vdata[5]; 
		vdata2[4] = vdata[6]; 
		vdata2[5] = vdata[7]; 
	}
else if(s_==3){ 
		vdata2[5] = vdata[0]; 
		vdata2[6] = vdata[1]; 
		vdata2[7] = vdata[2]; 
		vdata2[0] = vdata[3]; 
		vdata2[1] = vdata[4]; 
		vdata2[2] = vdata[5]; 
		vdata2[3] = vdata[6]; 
		vdata2[4] = vdata[7]; 
	}
else if(s_==4){ 
		vdata2[4] = vdata[0]; 
		vdata2[5] = vdata[1]; 
		vdata2[6] = vdata[2]; 
		vdata2[7] = vdata[3]; 
		vdata2[0] = vdata[4]; 
		vdata2[1] = vdata[5]; 
		vdata2[2] = vdata[6]; 
		vdata2[3] = vdata[7]; 
	}
else if(s_==5){ 
		vdata2[3] = vdata[0]; 
		vdata2[4] = vdata[1]; 
		vdata2[5] = vdata[2]; 
		vdata2[6] = vdata[3]; 
		vdata2[7] = vdata[4]; 
		vdata2[0] = vdata[5]; 
		vdata2[1] = vdata[6]; 
		vdata2[2] = vdata[7]; 
	}
else if(s_==6){ 
		vdata2[2] = vdata[0]; 
		vdata2[3] = vdata[1]; 
		vdata2[4] = vdata[2]; 
		vdata2[5] = vdata[3]; 
		vdata2[6] = vdata[4]; 
		vdata2[7] = vdata[5]; 
		vdata2[0] = vdata[6]; 
		vdata2[1] = vdata[7]; 
	}
else { 
		vdata2[1] = vdata[0]; 
		vdata2[2] = vdata[1]; 
		vdata2[3] = vdata[2]; 
		vdata2[4] = vdata[3]; 
		vdata2[5] = vdata[4]; 
		vdata2[6] = vdata[5]; 
		vdata2[7] = vdata[6]; 
		vdata2[0] = vdata[7]; 
	}
	return;
}

void PROCESSS_RearrangeLayoutEn(unsigned int s, bool en[VECTOR_SIZE], bool en2[VECTOR_SIZE]){
	unsigned int s_ = s;// % VECTOR_SIZE;
 if(s_==0){ 
		en2[0] = en[0]; 
		en2[1] = en[1]; 
		en2[2] = en[2]; 
		en2[3] = en[3]; 
		en2[4] = en[4]; 
		en2[5] = en[5]; 
		en2[6] = en[6]; 
		en2[7] = en[7]; 
	}
else if(s_==1){ 
		en2[7] = en[0]; 
		en2[0] = en[1]; 
		en2[1] = en[2]; 
		en2[2] = en[3]; 
		en2[3] = en[4]; 
		en2[4] = en[5]; 
		en2[5] = en[6]; 
		en2[6] = en[7]; 
	}
else if(s_==2){ 
		en2[6] = en[0]; 
		en2[7] = en[1]; 
		en2[0] = en[2]; 
		en2[1] = en[3]; 
		en2[2] = en[4]; 
		en2[3] = en[5]; 
		en2[4] = en[6]; 
		en2[5] = en[7]; 
	}
else if(s_==3){ 
		en2[5] = en[0]; 
		en2[6] = en[1]; 
		en2[7] = en[2]; 
		en2[0] = en[3]; 
		en2[1] = en[4]; 
		en2[2] = en[5]; 
		en2[3] = en[6]; 
		en2[4] = en[7]; 
	}
else if(s_==4){ 
		en2[4] = en[0]; 
		en2[5] = en[1]; 
		en2[6] = en[2]; 
		en2[7] = en[3]; 
		en2[0] = en[4]; 
		en2[1] = en[5]; 
		en2[2] = en[6]; 
		en2[3] = en[7]; 
	}
else if(s_==5){ 
		en2[3] = en[0]; 
		en2[4] = en[1]; 
		en2[5] = en[2]; 
		en2[6] = en[3]; 
		en2[7] = en[4]; 
		en2[0] = en[5]; 
		en2[1] = en[6]; 
		en2[2] = en[7]; 
	}
else if(s_==6){ 
		en2[2] = en[0]; 
		en2[3] = en[1]; 
		en2[4] = en[2]; 
		en2[5] = en[3]; 
		en2[6] = en[4]; 
		en2[7] = en[5]; 
		en2[0] = en[6]; 
		en2[1] = en[7]; 
	}
else { 
		en2[1] = en[0]; 
		en2[2] = en[1]; 
		en2[3] = en[2]; 
		en2[4] = en[3]; 
		en2[5] = en[4]; 
		en2[6] = en[5]; 
		en2[7] = en[6]; 
		en2[0] = en[7]; 
	}
	return;
}

parsededge_t PROCESSS_PARSEEDGE(uint32_type data){ 
	parsededge_t parsededge;
	parsededge.incr = UTILS_READFROM_UINT(data, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA);
	parsededge.dstvid = UTILS_READFROM_UINT(data, OFFSETOF_DSTV_IN_EDGEDSTVDATA, SIZEOF_DSTV_IN_EDGEDSTVDATA);
	return parsededge; 
}

void PROCESSS_calculateoffsets(keyvalue_capsule_t * buffer, unsigned int size){
	buffer[0].key = 0;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2	
		buffer[i].key = buffer[i-1].key + buffer[i-1].value; 
	}
	return;
}

void PROCESSS_readedgeblockstats(value_t tempbuffer[VECTOR2_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], buffer_type chunk_size, globalparams_t globalparams){
	// read edge block stats  
	// bool statsiscorrect = true;
	// #ifdef CONFIG_PROCESSEDGES_READOFFLINESTATS
	if(globalparams.ACTSPARAMS_READOFFLINESTATS == 1){
		unsigned int sum_values = 0;
		for(unsigned int p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ localcapsule[p].key = 0; localcapsule[p].value = tempbuffer[p][0]; if(p<globalparams.ACTSPARAMS_NUM_PARTITIONS-1){ sum_values += tempbuffer[p][0]; }}
		localcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].value = (chunk_size * VECTOR2_SIZE) - sum_values;
		if(tempbuffer[globalparams.ACTSPARAMS_NUM_PARTITIONS-1][0] != 8888888 || sum_values > chunk_size * VECTOR2_SIZE){
			#if defined(_DEBUGMODE_CHECKS) && defined(CONFIG_INSERTSTATSMETADATAINEDGES) // CRITICAL FIXME.
			if(sum_values > chunk_size * VECTOR2_SIZE && chunk_size > 0){ cout<<"processedges2: ERROR: sum_values("<<sum_values<<") > chunk_size("<<chunk_size<<") * VECTOR2_SIZE. EXITING... "<<endl; actsutilityobj->printkeyvalues("processandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS); exit(EXIT_FAILURE); } 
			if(tempbuffer[globalparams.ACTSPARAMS_NUM_PARTITIONS-1][0] != 8888888){ cout<<"processedges2: ERROR: tempbuffer[globalparams.ACTSPARAMS_NUM_PARTITIONS-1][0] != 8888888. EXITING... "<<endl; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<"tempbuffer["<<v<<"][0]: "<<tempbuffer[v][0]<<endl; } exit(EXIT_FAILURE); }
			#endif 
			unsigned int modelsz = chunk_size / globalparams.ACTSPARAMS_NUM_PARTITIONS; // mock it
			for(unsigned int i=0; i<globalparams.ACTSPARAMS_NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } 
		}
	// #else
	} else {
		unsigned int modelsz = chunk_size / globalparams.ACTSPARAMS_NUM_PARTITIONS;
		for(unsigned int i=0; i<globalparams.ACTSPARAMS_NUM_PARTITIONS; i++){ localcapsule[i].key = (i * modelsz) * VECTOR2_SIZE; localcapsule[i].value = modelsz * VECTOR2_SIZE; } 
	}
	// #endif 
	PROCESSS_calculateoffsets(localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#ifdef _DEBUGMODE_KERNELPRINTS3 // DEBUGME_PROCESSEDGES2
	actsutilityobj->printkeyvalues("processedges2(14).localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS); 
	cout<<"processedges2(15): "<<"chunk_size * VECTOR2_SIZE: "<<chunk_size * VECTOR2_SIZE<<", globalparams.ACTSPARAMS_WORKBUFFER_SIZE * VECTOR2_SIZE: "<<(globalparams.ACTSPARAMS_WORKBUFFER_SIZE * VECTOR2_SIZE)<<endl;
	#endif
}

fetchmessage_t PROCESSS_ACTSreadandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return fetchmessage; }
	
	uint32_type E[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	uint32_type E2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E2 complete

	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 2;
	mytravstate.end_kvs = travstate.end_kvs / 2;
	
	loffset_kvs = loffset_kvs / 2; //
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	value_t res[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	unsigned int loadcount[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=loadcount complete
	unsigned int depths[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int d_kvs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=d_kvs complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	bool enx2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx2 complete

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	keyvalue_t edata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edata complete
	value_t tempbuffer[VECTOR2_SIZE][MAX_SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILS_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ loadcount[t] = 0; }
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILS_GETV(INVALIDDATA);
	unsigned int bufferoffset_kvs = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	for (buffer_type i=0; i<globalparams.SIZEKVS2_REDUCEPARTITION; i++){
		if(MEMCAS_READVMASK(vbuffer[0][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 0"<<", vbuffer[0]["<<i<<"]: "<<vbuffer[0][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[1][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 1"<<", vbuffer[1]["<<i<<"]: "<<vbuffer[1][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[2][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 2"<<", vbuffer[2]["<<i<<"]: "<<vbuffer[2][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[3][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 3"<<", vbuffer[3]["<<i<<"]: "<<vbuffer[3][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[4][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 4"<<", vbuffer[4]["<<i<<"]: "<<vbuffer[4][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[5][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 5"<<", vbuffer[5]["<<i<<"]: "<<vbuffer[5][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[6][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 6"<<", vbuffer[6]["<<i<<"]: "<<vbuffer[6][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[7][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 7"<<", vbuffer[7]["<<i<<"]: "<<vbuffer[7][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[8][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 8"<<", vbuffer[8]["<<i<<"]: "<<vbuffer[8][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[9][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 9"<<", vbuffer[9]["<<i<<"]: "<<vbuffer[9][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[10][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 10"<<", vbuffer[10]["<<i<<"]: "<<vbuffer[10][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[11][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 11"<<", vbuffer[11]["<<i<<"]: "<<vbuffer[11][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[12][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 12"<<", vbuffer[12]["<<i<<"]: "<<vbuffer[12][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[13][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 13"<<", vbuffer[13]["<<i<<"]: "<<vbuffer[13][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[14][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 14"<<", vbuffer[14]["<<i<<"]: "<<vbuffer[14][i]<<endl; }
		if(MEMCAS_READVMASK(vbuffer[15][i]) == 1){ cout<<"READANDPROCESS: ACTIVE MASK SEEN: @ i: "<<i<<", v: 15"<<", vbuffer[15]["<<i<<"]: "<<vbuffer[15][i]<<endl; }
	}
	#endif
	
	// read edge block
	#ifdef DEBUGME_PROCESSEDGES2
	cout<<"processedges2: FIRST: offset_kvs: "<<offset_kvs<<", loffset_kvs: "<<loffset_kvs<<", goffset_kvs: "<<goffset_kvs<<", edgessize_kvs: "<<edgessize_kvs<<", mytravstate.i_kvs: "<<mytravstate.i_kvs<<", mytravstate.end_kvs: "<<mytravstate.end_kvs<<endl;
	#endif 
	PROCESSBUFFERPARTITIONS_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = edges[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = edges[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = edges[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = edges[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = edges[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = edges[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = edges[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = edges[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = edges[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = edges[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = edges[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = edges[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = edges[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = edges[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = edges[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = edges[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i] = edges[offset_kvs + i].data[0].key; 
		tempbuffer[1][i] = edges[offset_kvs + i].data[0].value;	
		tempbuffer[2][i] = edges[offset_kvs + i].data[1].key; 
		tempbuffer[3][i] = edges[offset_kvs + i].data[1].value;	
		tempbuffer[4][i] = edges[offset_kvs + i].data[2].key; 
		tempbuffer[5][i] = edges[offset_kvs + i].data[2].value;	
		tempbuffer[6][i] = edges[offset_kvs + i].data[3].key; 
		tempbuffer[7][i] = edges[offset_kvs + i].data[3].value;	
		tempbuffer[8][i] = edges[offset_kvs + i].data[4].key; 
		tempbuffer[9][i] = edges[offset_kvs + i].data[4].value;	
		tempbuffer[10][i] = edges[offset_kvs + i].data[5].key; 
		tempbuffer[11][i] = edges[offset_kvs + i].data[5].value;	
		tempbuffer[12][i] = edges[offset_kvs + i].data[6].key; 
		tempbuffer[13][i] = edges[offset_kvs + i].data[6].value;	
		tempbuffer[14][i] = edges[offset_kvs + i].data[7].key; 
		tempbuffer[15][i] = edges[offset_kvs + i].data[7].value;	
		#endif
		
		#ifdef DEBUGME_PROCESSEDGES
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[0].key: "<<edges[offset_kvs + i].data[0].key<<", edges["<<offset_kvs + i<<"].data[0].value: "<<edges[offset_kvs + i].data[0].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[1].key: "<<edges[offset_kvs + i].data[1].key<<", edges["<<offset_kvs + i<<"].data[1].value: "<<edges[offset_kvs + i].data[1].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[2].key: "<<edges[offset_kvs + i].data[2].key<<", edges["<<offset_kvs + i<<"].data[2].value: "<<edges[offset_kvs + i].data[2].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[3].key: "<<edges[offset_kvs + i].data[3].key<<", edges["<<offset_kvs + i<<"].data[3].value: "<<edges[offset_kvs + i].data[3].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[4].key: "<<edges[offset_kvs + i].data[4].key<<", edges["<<offset_kvs + i<<"].data[4].value: "<<edges[offset_kvs + i].data[4].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[5].key: "<<edges[offset_kvs + i].data[5].key<<", edges["<<offset_kvs + i<<"].data[5].value: "<<edges[offset_kvs + i].data[5].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[6].key: "<<edges[offset_kvs + i].data[6].key<<", edges["<<offset_kvs + i<<"].data[6].value: "<<edges[offset_kvs + i].data[6].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[7].key: "<<edges[offset_kvs + i].data[7].key<<", edges["<<offset_kvs + i<<"].data[7].value: "<<edges[offset_kvs + i].data[7].value<<endl; }
		#endif 
	}
	
	// read edge block stats  
	PROCESSS_readedgeblockstats(tempbuffer, localcapsule, chunk_size, globalparams);
	
	// process edge block stats 
	maxsize_kvs[0] = 0; maxsize_kvs[1] = 0;
	unsigned int _poff = 0;
	PROCESSBUFFERPARTITIONS_LOOP3: for(unsigned int cid=0; cid<2; cid++){
		PROCESSBUFFERPARTITIONS_LOOP3B: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=2
			unsigned int ssize_kvs = localcapsule[_poff + p].value / VECTOR2_SIZE;
			if(maxsize_kvs[cid] < ssize_kvs){ maxsize_kvs[cid] = ssize_kvs; }
		}
		_poff += globalparams.ACTSPARAMS_NUM_PARTITIONS/2;
	}
	height_kvs = (localcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].key + localcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].value) / VECTOR2_SIZE;

	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR2_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR2_SIZE-1)) / VECTOR2_SIZE;
	}
	
	// process edge block
	unsigned int readoffset = 0; if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ readoffset = 1; }
	PROCESSBUFFERPARTITIONS_LOOP4: for(buffer_type capsule_offset=0; capsule_offset<globalparams.ACTSPARAMS_NUM_PARTITIONS; capsule_offset+=VECTOR_SIZE){ // processing next capsule set
		unsigned int mmaxsz_kvs = maxsize_kvs[capsule_offset / VECTOR_SIZE];
		PROCESSBUFFERPARTITIONS_LOOP4B: for(buffer_type block_offset=0; block_offset<VECTOR2_SIZE; block_offset+=VECTOR_SIZE){ // processing next block set 
			PROCESSBUFFERPARTITIONS_LOOP4C: for(unsigned int r=0; r<VECTOR_SIZE; r++){ // interchanging columns
				PROCESSBUFFERPARTITIONS_LOOP4D: for(buffer_type i=0; i<mmaxsz_kvs; i++){ // processing a column data
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					#endif
					
					// get layout
					PROCESSS_GetXYLayoutV(r, depths, 0);
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[0], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[0], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[1], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[1], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[2], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[2], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[3], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[3], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[4], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[4], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[5], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[5], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[6], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[6], NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(13)::DEBUG CODE 13::1", capsule_offset + depths[7], globalparams.ACTSPARAMS_NUM_PARTITIONS, capsule_offset, depths[7], NAp);
 
					#endif
					
					// calculate enable
					enx[0] = true;	
					enx[1] = true;	
					enx[2] = true;	
					enx[3] = true;	
					enx[4] = true;	
					enx[5] = true;	
					enx[6] = true;	
					enx[7] = true;	
					
					// read 
					unsigned int tdepth0 = capsule_offset + depths[0];	
					d_kvs[0] = readoffset + localcapsule_kvs[tdepth0].key + i; 
					unsigned int tdepth1 = capsule_offset + depths[1];	
					d_kvs[1] = readoffset + localcapsule_kvs[tdepth1].key + i; 
					unsigned int tdepth2 = capsule_offset + depths[2];	
					d_kvs[2] = readoffset + localcapsule_kvs[tdepth2].key + i; 
					unsigned int tdepth3 = capsule_offset + depths[3];	
					d_kvs[3] = readoffset + localcapsule_kvs[tdepth3].key + i; 
					unsigned int tdepth4 = capsule_offset + depths[4];	
					d_kvs[4] = readoffset + localcapsule_kvs[tdepth4].key + i; 
					unsigned int tdepth5 = capsule_offset + depths[5];	
					d_kvs[5] = readoffset + localcapsule_kvs[tdepth5].key + i; 
					unsigned int tdepth6 = capsule_offset + depths[6];	
					d_kvs[6] = readoffset + localcapsule_kvs[tdepth6].key + i; 
					unsigned int tdepth7 = capsule_offset + depths[7];	
					d_kvs[7] = readoffset + localcapsule_kvs[tdepth7].key + i; 
 	
 // FIXME.
					if(d_kvs[0] < height_kvs && i < localcapsule_kvs[tdepth0].value){ E[0] = tempbuffer[block_offset + 0][d_kvs[0]]; } else { enx[0] = false; E[0] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[1] < height_kvs && i < localcapsule_kvs[tdepth1].value){ E[1] = tempbuffer[block_offset + 1][d_kvs[1]]; } else { enx[1] = false; E[1] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[2] < height_kvs && i < localcapsule_kvs[tdepth2].value){ E[2] = tempbuffer[block_offset + 2][d_kvs[2]]; } else { enx[2] = false; E[2] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[3] < height_kvs && i < localcapsule_kvs[tdepth3].value){ E[3] = tempbuffer[block_offset + 3][d_kvs[3]]; } else { enx[3] = false; E[3] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[4] < height_kvs && i < localcapsule_kvs[tdepth4].value){ E[4] = tempbuffer[block_offset + 4][d_kvs[4]]; } else { enx[4] = false; E[4] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[5] < height_kvs && i < localcapsule_kvs[tdepth5].value){ E[5] = tempbuffer[block_offset + 5][d_kvs[5]]; } else { enx[5] = false; E[5] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[6] < height_kvs && i < localcapsule_kvs[tdepth6].value){ E[6] = tempbuffer[block_offset + 6][d_kvs[6]]; } else { enx[6] = false; E[6] = INVALIDDATA; }	
 // FIXME.
					if(d_kvs[7] < height_kvs && i < localcapsule_kvs[tdepth7].value){ E[7] = tempbuffer[block_offset + 7][d_kvs[7]]; } else { enx[7] = false; E[7] = INVALIDDATA; }	
 	
					
					// re-arrange 
					#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
					PROCESSS_RearrangeLayoutV(r, E, E2);
					PROCESSS_RearrangeLayoutEn(r, enx, enx2); // NEWCHANGE.
					#else 
 E2[0] = E[0];  E2[1] = E[1];  E2[2] = E[2];  E2[3] = E[3];  E2[4] = E[4];  E2[5] = E[5];  E2[6] = E[6];  E2[7] = E[7];  E2[8] = E[8];  E2[9] = E[9];  E2[10] = E[10];  E2[11] = E[11];  E2[12] = E[12];  E2[13] = E[13];  E2[14] = E[14];  E2[15] = E[15]; 	
					#endif 
 if(E2[0]==8888888){ E2[0] = 0; } enx2[0] = enx[0];  if(E2[1]==8888888){ E2[1] = 0; } enx2[1] = enx[1];  if(E2[2]==8888888){ E2[2] = 0; } enx2[2] = enx[2];  if(E2[3]==8888888){ E2[3] = 0; } enx2[3] = enx[3];  if(E2[4]==8888888){ E2[4] = 0; } enx2[4] = enx[4];  if(E2[5]==8888888){ E2[5] = 0; } enx2[5] = enx[5];  if(E2[6]==8888888){ E2[6] = 0; } enx2[6] = enx[6];  if(E2[7]==8888888){ E2[7] = 0; } enx2[7] = enx[7]; 	
					#ifdef _DEBUGMODE_CHECKS3
					if(E2[0]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[1]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[2]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[3]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[4]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[5]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[6]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					if(E2[7]==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
					#endif 
					
					// parse
					parsededge_t parsed_edge0 = PROCESSS_PARSEEDGE(E2[0]); // FIXME.
					edata[0].value = parsed_edge0.incr; // source info
					edata[0].key = parsed_edge0.dstvid;	
					parsededge_t parsed_edge1 = PROCESSS_PARSEEDGE(E2[1]); // FIXME.
					edata[1].value = parsed_edge1.incr; // source info
					edata[1].key = parsed_edge1.dstvid;	
					parsededge_t parsed_edge2 = PROCESSS_PARSEEDGE(E2[2]); // FIXME.
					edata[2].value = parsed_edge2.incr; // source info
					edata[2].key = parsed_edge2.dstvid;	
					parsededge_t parsed_edge3 = PROCESSS_PARSEEDGE(E2[3]); // FIXME.
					edata[3].value = parsed_edge3.incr; // source info
					edata[3].key = parsed_edge3.dstvid;	
					parsededge_t parsed_edge4 = PROCESSS_PARSEEDGE(E2[4]); // FIXME.
					edata[4].value = parsed_edge4.incr; // source info
					edata[4].key = parsed_edge4.dstvid;	
					parsededge_t parsed_edge5 = PROCESSS_PARSEEDGE(E2[5]); // FIXME.
					edata[5].value = parsed_edge5.incr; // source info
					edata[5].key = parsed_edge5.dstvid;	
					parsededge_t parsed_edge6 = PROCESSS_PARSEEDGE(E2[6]); // FIXME.
					edata[6].value = parsed_edge6.incr; // source info
					edata[6].key = parsed_edge6.dstvid;	
					parsededge_t parsed_edge7 = PROCESSS_PARSEEDGE(E2[7]); // FIXME.
					edata[7].value = parsed_edge7.incr; // source info
					edata[7].key = parsed_edge7.dstvid;	
					#ifdef _DEBUGMODE_KERNELPRINTS
					if(edata[0].value==42 && enx[0] == true){ cout<<"processedges2: edata[0].value("<<edata[0].value<<")==42. edata[0].key: "<<edata[0].key<<endl; }
					if(edata[1].value==42 && enx[1] == true){ cout<<"processedges2: edata[1].value("<<edata[1].value<<")==42. edata[1].key: "<<edata[1].key<<endl; }
					if(edata[2].value==42 && enx[2] == true){ cout<<"processedges2: edata[2].value("<<edata[2].value<<")==42. edata[2].key: "<<edata[2].key<<endl; }
					if(edata[3].value==42 && enx[3] == true){ cout<<"processedges2: edata[3].value("<<edata[3].value<<")==42. edata[3].key: "<<edata[3].key<<endl; }
					if(edata[4].value==42 && enx[4] == true){ cout<<"processedges2: edata[4].value("<<edata[4].value<<")==42. edata[4].key: "<<edata[4].key<<endl; }
					if(edata[5].value==42 && enx[5] == true){ cout<<"processedges2: edata[5].value("<<edata[5].value<<")==42. edata[5].key: "<<edata[5].key<<endl; }
					if(edata[6].value==42 && enx[6] == true){ cout<<"processedges2: edata[6].value("<<edata[6].value<<")==42. edata[6].key: "<<edata[6].key<<endl; }
					if(edata[7].value==42 && enx[7] == true){ cout<<"processedges2: edata[7].value("<<edata[7].value<<")==42. edata[7].key: "<<edata[7].key<<endl; }
					#endif 
					
					// process
					PROCESSS_processvector(enx2[0], 0, edata[0].value, edata[0], vbuffer[capsule_offset + 0], buffer[0], bufferoffset_kvs, &loadcount[0], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 0]
						#endif
						);
					PROCESSS_processvector(enx2[1], 1, edata[1].value, edata[1], vbuffer[capsule_offset + 1], buffer[1], bufferoffset_kvs, &loadcount[1], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 1]
						#endif
						);
					PROCESSS_processvector(enx2[2], 2, edata[2].value, edata[2], vbuffer[capsule_offset + 2], buffer[2], bufferoffset_kvs, &loadcount[2], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 2]
						#endif
						);
					PROCESSS_processvector(enx2[3], 3, edata[3].value, edata[3], vbuffer[capsule_offset + 3], buffer[3], bufferoffset_kvs, &loadcount[3], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 3]
						#endif
						);
					PROCESSS_processvector(enx2[4], 4, edata[4].value, edata[4], vbuffer[capsule_offset + 4], buffer[4], bufferoffset_kvs, &loadcount[4], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 4]
						#endif
						);
					PROCESSS_processvector(enx2[5], 5, edata[5].value, edata[5], vbuffer[capsule_offset + 5], buffer[5], bufferoffset_kvs, &loadcount[5], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 5]
						#endif
						);
					PROCESSS_processvector(enx2[6], 6, edata[6].value, edata[6], vbuffer[capsule_offset + 6], buffer[6], bufferoffset_kvs, &loadcount[6], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 6]
						#endif
						);
					PROCESSS_processvector(enx2[7], 7, edata[7].value, edata[7], vbuffer[capsule_offset + 7], buffer[7], bufferoffset_kvs, &loadcount[7], GraphAlgoClass, globalposition, globalparams
						#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
						,collections[capsule_offset + 7]
						#endif
						);
	
					#ifdef _DEBUGMODE_CHECKS2
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[0], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[1], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[2], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[3], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[4], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[5], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[6], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", loadcount[7], MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
 
					#endif
				}
			}
		}
	}
	
	// for(unsigned int t=0; t<VECTOR_SIZE; t++){ cout<<"--- loadcount["<<t<<"]: "<<loadcount[t]<<endl; }
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS); ////
	
	unsigned int maxsz_kvs = 0;
	for(unsigned int t=0; t<VECTOR_SIZE; t++){ if(loadcount[t] > maxsz_kvs){ maxsz_kvs = loadcount[t]; }}
	fetchmessage.chunksize_kvs = maxsz_kvs;//chunk_size * 2; // loadcount; // CRITICAL FIXME
	// cout<<"--- processedges: fetchmessage.chunksize_kvs: "<<fetchmessage.chunksize_kvs<<endl;
	return fetchmessage;
}

fetchmessage_t PROCESSS_TRADreadandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ACTSS_actit: Hybrid Check: num_active_vertices("<<globalposition.num_active_vertices<<") < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION("<<globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION<<"). USING TRADITIONAL GP INSTEAD.... !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl; 
		#endif 
		return fetchmessage; } // FIXXXXXXXXMMMMMMMMMMMEEEEEEEEEEEEEEEEEEEEEEE.						
	
	uint32_type E[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	uint32_type E2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E2 complete

	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 2;
	mytravstate.end_kvs = travstate.end_kvs / 2;
	
	loffset_kvs = loffset_kvs / 2; //
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	unsigned int loadcount;

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	value_t tempbuffer[VECTOR2_SIZE][MAX_SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILS_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	loadcount = 0;
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILS_GETV(INVALIDDATA);
	unsigned int bufferoffset_kvs = 0;
	
	// read edge block
	#ifdef DEBUGME_PROCESSEDGES2
	cout<<"processedges2: FIRST: offset_kvs: "<<offset_kvs<<", loffset_kvs: "<<loffset_kvs<<", goffset_kvs: "<<goffset_kvs<<", edgessize_kvs: "<<edgessize_kvs<<", mytravstate.i_kvs: "<<mytravstate.i_kvs<<", mytravstate.end_kvs: "<<mytravstate.end_kvs<<endl;
	#endif 
	PROCESSBUFFERPARTITIONS_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = edges[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = edges[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = edges[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = edges[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = edges[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = edges[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = edges[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = edges[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = edges[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = edges[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = edges[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = edges[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = edges[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = edges[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = edges[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = edges[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i] = edges[offset_kvs + i].data[0].key; 
		tempbuffer[1][i] = edges[offset_kvs + i].data[0].value;	
		tempbuffer[2][i] = edges[offset_kvs + i].data[1].key; 
		tempbuffer[3][i] = edges[offset_kvs + i].data[1].value;	
		tempbuffer[4][i] = edges[offset_kvs + i].data[2].key; 
		tempbuffer[5][i] = edges[offset_kvs + i].data[2].value;	
		tempbuffer[6][i] = edges[offset_kvs + i].data[3].key; 
		tempbuffer[7][i] = edges[offset_kvs + i].data[3].value;	
		tempbuffer[8][i] = edges[offset_kvs + i].data[4].key; 
		tempbuffer[9][i] = edges[offset_kvs + i].data[4].value;	
		tempbuffer[10][i] = edges[offset_kvs + i].data[5].key; 
		tempbuffer[11][i] = edges[offset_kvs + i].data[5].value;	
		tempbuffer[12][i] = edges[offset_kvs + i].data[6].key; 
		tempbuffer[13][i] = edges[offset_kvs + i].data[6].value;	
		tempbuffer[14][i] = edges[offset_kvs + i].data[7].key; 
		tempbuffer[15][i] = edges[offset_kvs + i].data[7].value;	
		#endif
		
		#ifdef DEBUGME_PROCESSEDGES
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[0].key: "<<edges[offset_kvs + i].data[0].key<<", edges["<<offset_kvs + i<<"].data[0].value: "<<edges[offset_kvs + i].data[0].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[1].key: "<<edges[offset_kvs + i].data[1].key<<", edges["<<offset_kvs + i<<"].data[1].value: "<<edges[offset_kvs + i].data[1].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[2].key: "<<edges[offset_kvs + i].data[2].key<<", edges["<<offset_kvs + i<<"].data[2].value: "<<edges[offset_kvs + i].data[2].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[3].key: "<<edges[offset_kvs + i].data[3].key<<", edges["<<offset_kvs + i<<"].data[3].value: "<<edges[offset_kvs + i].data[3].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[4].key: "<<edges[offset_kvs + i].data[4].key<<", edges["<<offset_kvs + i<<"].data[4].value: "<<edges[offset_kvs + i].data[4].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[5].key: "<<edges[offset_kvs + i].data[5].key<<", edges["<<offset_kvs + i<<"].data[5].value: "<<edges[offset_kvs + i].data[5].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[6].key: "<<edges[offset_kvs + i].data[6].key<<", edges["<<offset_kvs + i<<"].data[6].value: "<<edges[offset_kvs + i].data[6].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[7].key: "<<edges[offset_kvs + i].data[7].key<<", edges["<<offset_kvs + i<<"].data[7].value: "<<edges[offset_kvs + i].data[7].value<<endl; }
		#endif 
	}
	
	// read edge block stats  
	PROCESSS_readedgeblockstats(tempbuffer, localcapsule, chunk_size, globalparams);

	unsigned int readoffset = 0; if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ readoffset = 1; }
	PROCESSBUFFERPARTITIONS_LOOP2A: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		PROCESSBUFFERPARTITIONS_LOOP2B: for(buffer_type i=localcapsule[p].key; i<localcapsule[p].key + localcapsule[p].value; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("TRADreadandprocess(14)::DEBUG CODE 12::1", readoffset + (i / VECTOR2_SIZE), MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			value_t E = tempbuffer[i % VECTOR2_SIZE][readoffset + (i / VECTOR2_SIZE)];
			bool en = true; 
			
			#ifdef _DEBUGMODE_CHECKS3
			if(E==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif 
			parsededge_t parsed_edge = PROCESSS_PARSEEDGE(E);
			keyvalue_t edata; edata.value = parsed_edge.incr; edata.key = parsed_edge.dstvid;	
	
			// read 
			vmdata_t vmdata;
			unsigned int loc = edata.value; //  - sweepparams.upperlimit;
			if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){
				#ifdef _DEBUGMODE_CHECKS
				if(true){ cout<<"TRADreadandprocess::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
				#endif 
				loc = 0; }
		
			keyvalue_vbuffer_t bits_vector = 0; 
			#ifdef _DEBUGMODE_CHECKS3
			if(en == true){ actsutilityobj->checkoutofbounds("TRADreadandprocess(14)::DEBUG CODE 113::1", bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VSRCDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
			#endif
			if(en == true){ bits_vector = vbuffer[p][bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO)]; }
			if(en == true){ vmdata = MEMCAS_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
							
			if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", vid: "<<UTILS_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
			#endif
					
			// process
			value_t res = PROCESSS_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			keyvalue_t mykeyvalue; mykeyvalue.key = edata.key; mykeyvalue.value = res;
			
			// write
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("TRADreadandprocess(14)::DEBUG CODE 14::1", loadcount, MAX_SOURCEBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			#endif
			if(en == true && vmdata.vmask == 1 && loadcount < ((globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2) * VECTOR_SIZE)){ buffer[loadcount % VECTOR_SIZE][loadcount / VECTOR_SIZE] = UTILS_GETKV(mykeyvalue); }
			if(en == true && vmdata.vmask == 1 && loadcount < ((globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2) * VECTOR_SIZE)){ loadcount += 1; }
	
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1);
			if(en == true && vmdata.vmask == 1 && loadcount < ((globalparams.ACTSPARAMS_WORKBUFFER_SIZE-2) * VECTOR_SIZE)){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1); } // mask0? FIXME.
			#endif
		}
	}

	fetchmessage.chunksize_kvs = (loadcount + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// exit(EXIT_SUCCESS);
	// cout<<">>> PROCESS VECTOR:: ^^^^^^^^^^ACTIVE VERTEX PROCESSED: SEEN: @ fetchmessage.chunksize_kvs: "<<fetchmessage.chunksize_kvs<<endl;
	// exit(EXIT_SUCCESS);
	return fetchmessage;
}

fetchmessage_t PROCESSS_TRADreadandprocessandreduce(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return fetchmessage; }
	
	uint32_type E[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	uint32_type E2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E2 complete

	travstate_t mytravstate = travstate;
	mytravstate.i_kvs = travstate.i_kvs / 2;
	mytravstate.end_kvs = travstate.end_kvs / 2;
	
	loffset_kvs = loffset_kvs / 2; //
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	unsigned int loadcount;

	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	value_t tempbuffer[VECTOR2_SIZE][MAX_SOURCEBLOCKRAM_SIZE]; // OPTIMIZEME
	#pragma HLS array_partition variable = tempbuffer
	keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS];
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	
	buffer_type chunk_size = UTILS_getchunksize_kvs(edgessize_kvs, mytravstate, 0);
	loadcount = 0;
	buffer_type maxsize_kvs[2]; 
	buffer_type height_kvs = 0;
	
	unsigned int MYINVALIDDATA = UTILS_GETV(INVALIDDATA);
	unsigned int bufferoffset_kvs = 0;
	
	// read edge block
	#ifdef DEBUGME_PROCESSEDGES2
	cout<<"processedges2: FIRST: offset_kvs: "<<offset_kvs<<", loffset_kvs: "<<loffset_kvs<<", goffset_kvs: "<<goffset_kvs<<", edgessize_kvs: "<<edgessize_kvs<<", mytravstate.i_kvs: "<<mytravstate.i_kvs<<", mytravstate.end_kvs: "<<mytravstate.end_kvs<<endl;
	#endif 
	PROCESSBUFFERPARTITIONS_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		tempbuffer[0][i] = edges[offset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = edges[offset_kvs + i].range(63, 32); 
		tempbuffer[2][i] = edges[offset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = edges[offset_kvs + i].range(127, 96); 
		tempbuffer[4][i] = edges[offset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = edges[offset_kvs + i].range(191, 160); 
		tempbuffer[6][i] = edges[offset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = edges[offset_kvs + i].range(255, 224); 
		tempbuffer[8][i] = edges[offset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = edges[offset_kvs + i].range(319, 288); 
		tempbuffer[10][i] = edges[offset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = edges[offset_kvs + i].range(383, 352); 
		tempbuffer[12][i] = edges[offset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = edges[offset_kvs + i].range(447, 416); 
		tempbuffer[14][i] = edges[offset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = edges[offset_kvs + i].range(511, 480); 
		#else 
		tempbuffer[0][i] = edges[offset_kvs + i].data[0].key; 
		tempbuffer[1][i] = edges[offset_kvs + i].data[0].value;	
		tempbuffer[2][i] = edges[offset_kvs + i].data[1].key; 
		tempbuffer[3][i] = edges[offset_kvs + i].data[1].value;	
		tempbuffer[4][i] = edges[offset_kvs + i].data[2].key; 
		tempbuffer[5][i] = edges[offset_kvs + i].data[2].value;	
		tempbuffer[6][i] = edges[offset_kvs + i].data[3].key; 
		tempbuffer[7][i] = edges[offset_kvs + i].data[3].value;	
		tempbuffer[8][i] = edges[offset_kvs + i].data[4].key; 
		tempbuffer[9][i] = edges[offset_kvs + i].data[4].value;	
		tempbuffer[10][i] = edges[offset_kvs + i].data[5].key; 
		tempbuffer[11][i] = edges[offset_kvs + i].data[5].value;	
		tempbuffer[12][i] = edges[offset_kvs + i].data[6].key; 
		tempbuffer[13][i] = edges[offset_kvs + i].data[6].value;	
		tempbuffer[14][i] = edges[offset_kvs + i].data[7].key; 
		tempbuffer[15][i] = edges[offset_kvs + i].data[7].value;	
		#endif
		
		#ifdef DEBUGME_PROCESSEDGES
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[0].key: "<<edges[offset_kvs + i].data[0].key<<", edges["<<offset_kvs + i<<"].data[0].value: "<<edges[offset_kvs + i].data[0].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[1].key: "<<edges[offset_kvs + i].data[1].key<<", edges["<<offset_kvs + i<<"].data[1].value: "<<edges[offset_kvs + i].data[1].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[2].key: "<<edges[offset_kvs + i].data[2].key<<", edges["<<offset_kvs + i<<"].data[2].value: "<<edges[offset_kvs + i].data[2].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[3].key: "<<edges[offset_kvs + i].data[3].key<<", edges["<<offset_kvs + i<<"].data[3].value: "<<edges[offset_kvs + i].data[3].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[4].key: "<<edges[offset_kvs + i].data[4].key<<", edges["<<offset_kvs + i<<"].data[4].value: "<<edges[offset_kvs + i].data[4].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[5].key: "<<edges[offset_kvs + i].data[5].key<<", edges["<<offset_kvs + i<<"].data[5].value: "<<edges[offset_kvs + i].data[5].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[6].key: "<<edges[offset_kvs + i].data[6].key<<", edges["<<offset_kvs + i<<"].data[6].value: "<<edges[offset_kvs + i].data[6].value<<endl; }
		if(i<4){ cout<<"processedges2: +++ sample edge: edges["<<offset_kvs + i<<"].data[7].key: "<<edges[offset_kvs + i].data[7].key<<", edges["<<offset_kvs + i<<"].data[7].value: "<<edges[offset_kvs + i].data[7].value<<endl; }
		#endif 
	}
	
	// read edge block stats  
	PROCESSS_readedgeblockstats(tempbuffer, localcapsule, chunk_size, globalparams);

	unsigned int readoffset = 0; if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ readoffset = 1; }
	PROCESSBUFFERPARTITIONS_LOOP2A: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		PROCESSBUFFERPARTITIONS_LOOP2B: for(buffer_type i=localcapsule[p].key; i<localcapsule[p].key + localcapsule[p].value; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 12::1", readoffset + (i / VECTOR2_SIZE), MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			value_t E = tempbuffer[i % VECTOR2_SIZE][readoffset + (i / VECTOR2_SIZE)];
			bool en = true; 
			
			#ifdef _DEBUGMODE_CHECKS3
			if(E==8888888){ cout<<"processedges2: ERROR 65. E2==8888888. EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif 
			parsededge_t parsed_edge = PROCESSS_PARSEEDGE(E);
			keyvalue_t edata; edata.value = parsed_edge.incr; edata.key = parsed_edge.dstvid;	
	
			// read 
			vmdata_t vmdata;
			unsigned int loc = edata.value; //  - sweepparams.upperlimit;
			if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){
				#ifdef _DEBUGMODE_CHECKS
				if(true){ cout<<"PROCESSS_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
				#endif 
				loc = 0; }
		
			keyvalue_vbuffer_t bits_vector = 0; 
			#ifdef _DEBUGMODE_CHECKS3
			if(en == true){ actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 113::1", bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VSRCDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
			#endif
			if(en == true){ bits_vector = vbuffer[p][bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO)]; }
			if(en == true){ vmdata = MEMCAS_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
						
			if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", vid: "<<UTILS_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
			#endif
					
			// process
			value_t res = PROCESSS_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			keyvalue_t mykeyvalue; mykeyvalue.key = edata.key; mykeyvalue.value = res;
			
			// read  
			vmdata_t vmdata_reduce; vmdata_reduce.vmask = 0;
			keyvalue_vbuffer_t bits_vector2 = 0; 
			if(en == true && vmdata.vmask == 1){ bits_vector2 = UTILS_GetData(kvdram, globalparams.BASEOFFSETKVS_DESTVERTICESDATA, (edata.key / VDATA_SHRINK_RATIO)); }
			if(en == true && vmdata.vmask == 1){ vmdata_reduce = MEMCAS_READFROMBUFFER_VDATAWITHVMASK(bits_vector2); }
						
			// reduce 
			value_t new_vprop = REDUCES_reducefunc(vmdata_reduce.vdata, NAp, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			if(en == true && vmdata.vmask == 1 && new_vprop != vmdata_reduce.vdata){ vmdata_reduce.vmask = 1; } else { vmdata_reduce.vmask = 0; } // NEWCHANGE.
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(en == true && vmdata.vmask == 1 && new_vprop != vmdata_reduce.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE MASK SEEN AT: loc: "<<loc<<", vmdata_reduce.vdata: "<<vmdata_reduce.vdata<<", vmdata_reduce.vmask: "<<vmdata_reduce.vmask<<endl; }
			if(en == true && vmdata.vmask == 1 && new_vprop != vmdata_reduce.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", loc: "<<loc<<", vid: "<<UTILS_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
			#endif
			
			// write
			if(en == true && vmdata.vmask == 1){ MEMCAS_WRITETODRAM_VDATAWITHVMASK(mykeyvalue.key, kvdram, globalparams.BASEOFFSETKVS_DESTVERTICESDATA, bits_vector2, new_vprop, vmdata.vmask); }
				
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1);
			if(vmdata.vmask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, 1); }
			actsutilityobj->globalstats_countkvsreduced(globalparams.ACTSPARAMS_INSTID, 1);
			if(vmdata.vmask == 1){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(globalparams.ACTSPARAMS_INSTID, 1); }
			#endif
		}
	}
	
	fetchmessage.chunksize_kvs = (loadcount + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// exit(EXIT_SUCCESS); //////
	return fetchmessage;
}

fetchmessage_t PROCESSS_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	
	return PROCESSS_ACTSreadandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
	// return PROCESSS_TRADreadandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
	
	/** #ifdef CONFIG_ALL_EVALUATIONTYPES_IN_ONE_KERNEL
	if(globalparams.EVALUATION_ACTS_MEMORYLAYOUT == ON){
		return PROCESSS_ACTSreadandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
	} else {
		if(globalparams.EVALUATION_ACTS_VERYBARE == ON){
			return PROCESSS_TRADreadandprocessandreduce(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
		} else {
			return PROCESSS_TRADreadandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
		}
	}
	#else 
		#ifdef CONFIG_ACTS_MEMORYLAYOUT
		return PROCESSS_ACTSreadandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
		#else 
			#ifdef CONFIG_ACTS_VERYBARE
			return PROCESSS_TRADreadandprocessandreduce(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
			#else
			return PROCESSS_TRADreadandprocess(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
			#endif 
		#endif
	#endif */
}







	
#endif 
#ifdef CONFIG_ENABLECLASS_PARTITIONUPDATES
void PARTITIONS_ACTSpreparekeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){				
	if(enable1 == OFF && enable2 == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }
	analysis_type analysis_dummyfiller = MAX_SRCBUFFER_SIZE - globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	
	value_t emptyslot[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=emptyslot complete
	UTILS_resetvalues(emptyslot, VECTOR_SIZE, 0);
	
	buffer_type chunk_size = size_kvs;
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
		localcapsule[0][p].key = 0;
		localcapsule[0][p].value = 0; 
		localcapsule[1][p].key = 0;
		localcapsule[1][p].value = 0; 
		localcapsule[2][p].key = 0;
		localcapsule[2][p].value = 0; 
		localcapsule[3][p].key = 0;
		localcapsule[3][p].value = 0; 
		localcapsule[4][p].key = 0;
		localcapsule[4][p].value = 0; 
		localcapsule[5][p].key = 0;
		localcapsule[5][p].value = 0; 
		localcapsule[6][p].key = 0;
		localcapsule[6][p].value = 0; 
		localcapsule[7][p].key = 0;
		localcapsule[7][p].value = 0; 
	}

	PREPAREKEYVALUES_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_buffer_t keyvalue0 = sourcebuffer[0][i];
		keyvalue_buffer_t keyvalue1 = sourcebuffer[1][i];
		keyvalue_buffer_t keyvalue2 = sourcebuffer[2][i];
		keyvalue_buffer_t keyvalue3 = sourcebuffer[3][i];
		keyvalue_buffer_t keyvalue4 = sourcebuffer[4][i];
		keyvalue_buffer_t keyvalue5 = sourcebuffer[5][i];
		keyvalue_buffer_t keyvalue6 = sourcebuffer[6][i];
		keyvalue_buffer_t keyvalue7 = sourcebuffer[7][i];
		
		keyvalue_t mykeyvalue0 = UTILS_GETKV(keyvalue0);
		keyvalue_t mykeyvalue1 = UTILS_GETKV(keyvalue1);
		keyvalue_t mykeyvalue2 = UTILS_GETKV(keyvalue2);
		keyvalue_t mykeyvalue3 = UTILS_GETKV(keyvalue3);
		keyvalue_t mykeyvalue4 = UTILS_GETKV(keyvalue4);
		keyvalue_t mykeyvalue5 = UTILS_GETKV(keyvalue5);
		keyvalue_t mykeyvalue6 = UTILS_GETKV(keyvalue6);
		keyvalue_t mykeyvalue7 = UTILS_GETKV(keyvalue7);
		
		bool_type valid0 = ON;
		if(mykeyvalue0.key != UTILS_GETK(INVALIDDATA) && mykeyvalue0.value != UTILS_GETV(INVALIDDATA)){ valid0 = ON; } else { valid0 = OFF; }
		bool_type valid1 = ON;
		if(mykeyvalue1.key != UTILS_GETK(INVALIDDATA) && mykeyvalue1.value != UTILS_GETV(INVALIDDATA)){ valid1 = ON; } else { valid1 = OFF; }
		bool_type valid2 = ON;
		if(mykeyvalue2.key != UTILS_GETK(INVALIDDATA) && mykeyvalue2.value != UTILS_GETV(INVALIDDATA)){ valid2 = ON; } else { valid2 = OFF; }
		bool_type valid3 = ON;
		if(mykeyvalue3.key != UTILS_GETK(INVALIDDATA) && mykeyvalue3.value != UTILS_GETV(INVALIDDATA)){ valid3 = ON; } else { valid3 = OFF; }
		bool_type valid4 = ON;
		if(mykeyvalue4.key != UTILS_GETK(INVALIDDATA) && mykeyvalue4.value != UTILS_GETV(INVALIDDATA)){ valid4 = ON; } else { valid4 = OFF; }
		bool_type valid5 = ON;
		if(mykeyvalue5.key != UTILS_GETK(INVALIDDATA) && mykeyvalue5.value != UTILS_GETV(INVALIDDATA)){ valid5 = ON; } else { valid5 = OFF; }
		bool_type valid6 = ON;
		if(mykeyvalue6.key != UTILS_GETK(INVALIDDATA) && mykeyvalue6.value != UTILS_GETV(INVALIDDATA)){ valid6 = ON; } else { valid6 = OFF; }
		bool_type valid7 = ON;
		if(mykeyvalue7.key != UTILS_GETK(INVALIDDATA) && mykeyvalue7.value != UTILS_GETV(INVALIDDATA)){ valid7 = ON; } else { valid7 = OFF; }
		
		partition_type p0 = 0;
		if(valid0 == ON){ p0 = UTILS_getpartition(ON, mode, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p1 = 0;
		if(valid1 == ON){ p1 = UTILS_getpartition(ON, mode, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p2 = 0;
		if(valid2 == ON){ p2 = UTILS_getpartition(ON, mode, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p3 = 0;
		if(valid3 == ON){ p3 = UTILS_getpartition(ON, mode, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p4 = 0;
		if(valid4 == ON){ p4 = UTILS_getpartition(ON, mode, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p5 = 0;
		if(valid5 == ON){ p5 = UTILS_getpartition(ON, mode, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p6 = 0;
		if(valid6 == ON){ p6 = UTILS_getpartition(ON, mode, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p7 = 0;
		if(valid7 == ON){ p7 = UTILS_getpartition(ON, mode, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		
		if(valid0 == ON){
			if(localcapsule[0][p0].value == 0){ 
				localcapsule[0][p0].key = emptyslot[0]; emptyslot[0] += 4;
			} else if(localcapsule[0][p0].value % 4 == 0){ 
				localcapsule[0][p0].key = emptyslot[0]; emptyslot[0] += 4;
			} else {}
		}
		if(valid1 == ON){
			if(localcapsule[1][p1].value == 0){ 
				localcapsule[1][p1].key = emptyslot[1]; emptyslot[1] += 4;
			} else if(localcapsule[1][p1].value % 4 == 0){ 
				localcapsule[1][p1].key = emptyslot[1]; emptyslot[1] += 4;
			} else {}
		}
		if(valid2 == ON){
			if(localcapsule[2][p2].value == 0){ 
				localcapsule[2][p2].key = emptyslot[2]; emptyslot[2] += 4;
			} else if(localcapsule[2][p2].value % 4 == 0){ 
				localcapsule[2][p2].key = emptyslot[2]; emptyslot[2] += 4;
			} else {}
		}
		if(valid3 == ON){
			if(localcapsule[3][p3].value == 0){ 
				localcapsule[3][p3].key = emptyslot[3]; emptyslot[3] += 4;
			} else if(localcapsule[3][p3].value % 4 == 0){ 
				localcapsule[3][p3].key = emptyslot[3]; emptyslot[3] += 4;
			} else {}
		}
		if(valid4 == ON){
			if(localcapsule[4][p4].value == 0){ 
				localcapsule[4][p4].key = emptyslot[4]; emptyslot[4] += 4;
			} else if(localcapsule[4][p4].value % 4 == 0){ 
				localcapsule[4][p4].key = emptyslot[4]; emptyslot[4] += 4;
			} else {}
		}
		if(valid5 == ON){
			if(localcapsule[5][p5].value == 0){ 
				localcapsule[5][p5].key = emptyslot[5]; emptyslot[5] += 4;
			} else if(localcapsule[5][p5].value % 4 == 0){ 
				localcapsule[5][p5].key = emptyslot[5]; emptyslot[5] += 4;
			} else {}
		}
		if(valid6 == ON){
			if(localcapsule[6][p6].value == 0){ 
				localcapsule[6][p6].key = emptyslot[6]; emptyslot[6] += 4;
			} else if(localcapsule[6][p6].value % 4 == 0){ 
				localcapsule[6][p6].key = emptyslot[6]; emptyslot[6] += 4;
			} else {}
		}
		if(valid7 == ON){
			if(localcapsule[7][p7].value == 0){ 
				localcapsule[7][p7].key = emptyslot[7]; emptyslot[7] += 4;
			} else if(localcapsule[7][p7].value % 4 == 0){ 
				localcapsule[7][p7].key = emptyslot[7]; emptyslot[7] += 4;
			} else {}
		}
		
		buffer_type loc0 = localcapsule[0][p0].key + (localcapsule[0][p0].value % 4);
		buffer_type loc1 = localcapsule[1][p1].key + (localcapsule[1][p1].value % 4);
		buffer_type loc2 = localcapsule[2][p2].key + (localcapsule[2][p2].value % 4);
		buffer_type loc3 = localcapsule[3][p3].key + (localcapsule[3][p3].value % 4);
		buffer_type loc4 = localcapsule[4][p4].key + (localcapsule[4][p4].value % 4);
		buffer_type loc5 = localcapsule[5][p5].key + (localcapsule[5][p5].value % 4);
		buffer_type loc6 = localcapsule[6][p6].key + (localcapsule[6][p6].value % 4);
		buffer_type loc7 = localcapsule[7][p7].key + (localcapsule[7][p7].value % 4);
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[0][p0].value", localcapsule[0][p0].value % 4, 4, localcapsule[0][p0].value, localcapsule[0][p0].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc0", loc0, MAX_SRCBUFFER_SIZE, localcapsule[0][p0].key, localcapsule[0][p0].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[1][p1].value", localcapsule[1][p1].value % 4, 4, localcapsule[1][p1].value, localcapsule[1][p1].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc1", loc1, MAX_SRCBUFFER_SIZE, localcapsule[1][p1].key, localcapsule[1][p1].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[2][p2].value", localcapsule[2][p2].value % 4, 4, localcapsule[2][p2].value, localcapsule[2][p2].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc2", loc2, MAX_SRCBUFFER_SIZE, localcapsule[2][p2].key, localcapsule[2][p2].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[3][p3].value", localcapsule[3][p3].value % 4, 4, localcapsule[3][p3].value, localcapsule[3][p3].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc3", loc3, MAX_SRCBUFFER_SIZE, localcapsule[3][p3].key, localcapsule[3][p3].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[4][p4].value", localcapsule[4][p4].value % 4, 4, localcapsule[4][p4].value, localcapsule[4][p4].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc4", loc4, MAX_SRCBUFFER_SIZE, localcapsule[4][p4].key, localcapsule[4][p4].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[5][p5].value", localcapsule[5][p5].value % 4, 4, localcapsule[5][p5].value, localcapsule[5][p5].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc5", loc5, MAX_SRCBUFFER_SIZE, localcapsule[5][p5].key, localcapsule[5][p5].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[6][p6].value", localcapsule[6][p6].value % 4, 4, localcapsule[6][p6].value, localcapsule[6][p6].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc6", loc6, MAX_SRCBUFFER_SIZE, localcapsule[6][p6].key, localcapsule[6][p6].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 11.localcapsule[7][p7].value", localcapsule[7][p7].value % 4, 4, localcapsule[7][p7].value, localcapsule[7][p7].value, NAp);
		actsutilityobj->checkoutofbounds("preparekeyvalues 12.loc7", loc7, MAX_SRCBUFFER_SIZE, localcapsule[7][p7].key, localcapsule[7][p7].value, NAp);
		#endif
		
		if(valid0 == ON){
			destbuffer[0][loc0] = keyvalue0;
			localcapsule[0][p0].value += 1;
		}
		if(valid1 == ON){
			destbuffer[1][loc1] = keyvalue1;
			localcapsule[1][p1].value += 1;
		}
		if(valid2 == ON){
			destbuffer[2][loc2] = keyvalue2;
			localcapsule[2][p2].value += 1;
		}
		if(valid3 == ON){
			destbuffer[3][loc3] = keyvalue3;
			localcapsule[3][p3].value += 1;
		}
		if(valid4 == ON){
			destbuffer[4][loc4] = keyvalue4;
			localcapsule[4][p4].value += 1;
		}
		if(valid5 == ON){
			destbuffer[5][loc5] = keyvalue5;
			localcapsule[5][p5].value += 1;
		}
		if(valid6 == ON){
			destbuffer[6][loc6] = keyvalue6;
			localcapsule[6][p6].value += 1;
		}
		if(valid7 == ON){
			destbuffer[7][loc7] = keyvalue7;
			localcapsule[7][p7].value += 1;
		}
	}
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = INVALIDDATA;
		keyvalue_buffer_t dummykv = UTILS_GETKV(mydummykv);
		for(vector_type k=0; k<4; k++){
		#pragma HLS PIPELINE II=2
			if(localcapsule[0][p].value > 0 && (localcapsule[0][p].value % 4) > 0){
				if(k >= localcapsule[0][p].value % 4){
					destbuffer[0][localcapsule[0][p].key + k] = dummykv;
					localcapsule[0][p].value += 1;
				}
			}
			if(localcapsule[1][p].value > 0 && (localcapsule[1][p].value % 4) > 0){
				if(k >= localcapsule[1][p].value % 4){
					destbuffer[1][localcapsule[1][p].key + k] = dummykv;
					localcapsule[1][p].value += 1;
				}
			}
			if(localcapsule[2][p].value > 0 && (localcapsule[2][p].value % 4) > 0){
				if(k >= localcapsule[2][p].value % 4){
					destbuffer[2][localcapsule[2][p].key + k] = dummykv;
					localcapsule[2][p].value += 1;
				}
			}
			if(localcapsule[3][p].value > 0 && (localcapsule[3][p].value % 4) > 0){
				if(k >= localcapsule[3][p].value % 4){
					destbuffer[3][localcapsule[3][p].key + k] = dummykv;
					localcapsule[3][p].value += 1;
				}
			}
			if(localcapsule[4][p].value > 0 && (localcapsule[4][p].value % 4) > 0){
				if(k >= localcapsule[4][p].value % 4){
					destbuffer[4][localcapsule[4][p].key + k] = dummykv;
					localcapsule[4][p].value += 1;
				}
			}
			if(localcapsule[5][p].value > 0 && (localcapsule[5][p].value % 4) > 0){
				if(k >= localcapsule[5][p].value % 4){
					destbuffer[5][localcapsule[5][p].key + k] = dummykv;
					localcapsule[5][p].value += 1;
				}
			}
			if(localcapsule[6][p].value > 0 && (localcapsule[6][p].value % 4) > 0){
				if(k >= localcapsule[6][p].value % 4){
					destbuffer[6][localcapsule[6][p].key + k] = dummykv;
					localcapsule[6][p].value += 1;
				}
			}
			if(localcapsule[7][p].value > 0 && (localcapsule[7][p].value % 4) > 0){
				if(k >= localcapsule[7][p].value % 4){
					destbuffer[7][localcapsule[7][p].key + k] = dummykv;
					localcapsule[7][p].value += 1;
				}
			}
		}
	}
	
	for(vector_type v=0; v<VECTOR_SIZE; v++){ cutoffs[v] = emptyslot[v]; }
	
	localcapsule[0][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[0]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[1][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[1]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[2][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[2]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[3][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[3]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[4][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[4]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[5][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[5]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[6][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[6]); //?????????????????????? FIXME. MAKE OBSOLETE?
	localcapsule[7][0].value += (MAX_SRCBUFFER_SIZE - emptyslot[7]); //?????????????????????? FIXME. MAKE OBSOLETE?
	
	localcapsule[0][0].key = 0;
	localcapsule[1][0].key = 0;
	localcapsule[2][0].key = 0;
	localcapsule[3][0].key = 0;
	localcapsule[4][0].key = 0;
	localcapsule[5][0].key = 0;
	localcapsule[6][0].key = 0;
	localcapsule[7][0].key = 0;
	
	UTILS_calculatemanyunallignedoffsets(localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0, 0);

	#ifdef _DEBUGMODE_CHECKS2
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		for(vector_type v=0; v<VECTOR_SIZE; v++){
			actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues.localcapsule[v][p].key", localcapsule[v][p].key, 4);
		}
	}
	buffer_type maxcutoff = 0;
	for(vector_type v=0; v<VECTOR_SIZE; v++){ if(cutoffs[v] > maxcutoff){ maxcutoff = cutoffs[v]; }}
	if(maxcutoff % 4 != 0){ for(int v=0; v<VECTOR_SIZE; v++){ cout<<"------------------- preparekeyvalues:cutoffs["<<v<<"]: "<<cutoffs[v]<<endl; } exit(EXIT_FAILURE); }
	actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:maxcutoff", maxcutoff, 4);
actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[0]", emptyslot[0], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[1]", emptyslot[1], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[2]", emptyslot[2], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[3]", emptyslot[3], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[4]", emptyslot[4], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[5]", emptyslot[5], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[6]", emptyslot[6], 4);actsutilityobj->checkfordivisibleby(ON, "preparekeyvalues:emptyslot[7]", emptyslot[7], 4);	
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("[after]preparekeyvalues.localcapsule[0].2", (keyvalue_t *)localcapsule[0], globalparams.ACTSPARAMS_NUM_PARTITIONS);
	actsutilityobj->printvalues("preparekeyvalues:cutoffs", cutoffs, VECTOR_SIZE);
	#endif
	// cout<<"----------------------------------------------- preparekeyvalues: END ------------------------------------- "<<endl;
	return;
}

void PARTITIONS_TRADpreparekeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){				
	if(enable1 == OFF && enable2 == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }
	analysis_type analysis_loop1 = globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	analysis_type analysis_dummyfiller = MAX_SRCBUFFER_SIZE - globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	
	unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ _NUM_PARTITIONS = UTILS_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
	#endif 
	
	value_t emptyslot[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=emptyslot complete
	UTILS_resetvalues(emptyslot, VECTOR_SIZE, 0);
	
	buffer_type chunk_size = size_kvs;
	for(vector_type v=0; v<VECTOR_SIZE; v++){ cutoffs[v] = size_kvs; }
	
	for(partition_type p=0; p<_NUM_PARTITIONS; p++){ 
		localcapsule[0][p].key = 0;
		localcapsule[0][p].value = 0; 
		localcapsule[1][p].key = 0;
		localcapsule[1][p].value = 0; 
		localcapsule[2][p].key = 0;
		localcapsule[2][p].value = 0; 
		localcapsule[3][p].key = 0;
		localcapsule[3][p].value = 0; 
		localcapsule[4][p].key = 0;
		localcapsule[4][p].value = 0; 
		localcapsule[5][p].key = 0;
		localcapsule[5][p].value = 0; 
		localcapsule[6][p].key = 0;
		localcapsule[6][p].value = 0; 
		localcapsule[7][p].key = 0;
		localcapsule[7][p].value = 0; 
	}
	
	PREPAREKEYVALUES_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1	
	#pragma HLS PIPELINE II=1
		destbuffer[0][i] = sourcebuffer[0][i];
		destbuffer[1][i] = sourcebuffer[1][i];
		destbuffer[2][i] = sourcebuffer[2][i];
		destbuffer[3][i] = sourcebuffer[3][i];
		destbuffer[4][i] = sourcebuffer[4][i];
		destbuffer[5][i] = sourcebuffer[5][i];
		destbuffer[6][i] = sourcebuffer[6][i];
		destbuffer[7][i] = sourcebuffer[7][i];
	}
	return;
}

void PARTITIONS_preparekeyvalues(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){
	#ifdef CONFIG_ALL_EVALUATIONTYPES_IN_ONE_KERNEL
	if(globalparams.EVALUATION_ACTS_PARTITIONINGLOGIC == ON){
		PARTITIONS_ACTSpreparekeyvalues(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
	} else {
		if(globalparams.EVALUATION_ACTS_VERYBARE == ON){
			// NAp
		} else {
			PARTITIONS_TRADpreparekeyvalues(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
		}
	}
	#else 
		#ifdef CONFIG_ACTS_PARTITIONINGLOGIC
		PARTITIONS_ACTSpreparekeyvalues(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
		#else
			#ifdef CONFIG_ACTS_VERYBARE
			// NAp
			#else 
			PARTITIONS_TRADpreparekeyvalues(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
			#endif 
		#endif
	#endif 
	return;
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_REDUCEUPDATES
void REDUCES_reducevector(bool enx, unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2[MAX_BLOCKRAM_VDEST2DATA_SIZE], buffer_type destoffset, unsigned int upperlimit, unsigned int * prevkey, sweepparams_t sweepparams, globalparams_t globalparams
		#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
		,collection_t collections[COLLECTIONS_BUFFERSZ]
		#endif 
		){
	#pragma HLS INLINE
	
	keyvalue_t mykeyvalue = UTILS_GETKV(kvdata);
	vertex_t loc = ((mykeyvalue.key - upperlimit) - col) >> globalparams.ACTSPARAMS_POW_PARTITIONS;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"REDUCES_reducevector:: col: "<<col<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", mykeyvalue.value: "<<mykeyvalue.value<<endl;
	#endif
	
	bool en = true;
	if(mykeyvalue.key == UTILS_GETK(INVALIDDATA) || mykeyvalue.value == UTILS_GETV(INVALIDDATA) || mykeyvalue.key == MAXVDATA || mykeyvalue.value == MAXVDATA){ en = false; } // REMOVEME?????????????????????????????
	if(mykeyvalue.key == 0 && mykeyvalue.value == 0){ en = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true){ cout<<"$$$ REDUCE VECTOR:: REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILS_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<", partition: "<<UTILS_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID) / (globalparams.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE)<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<endl; }															
	#endif 
	
	if(loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_SHRINK_RATIO) && en == true){
		#ifdef ENABLE_PERFECTACCURACY
		if(true){ cout<<"REDUCES_reducevector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }
		
	vmdata_t vmdata; vmdata.vmask = 0;
	vmdata_t vmdata2; vmdata2.vmask = 0;
	unsigned int bufferoffset_kvs = 0; 
	// if(globalparams.ACTSPARAMS_TREEDEPTH == 1){ bufferoffset_kvs = BLOCKRAM_VDATA_SECTION1; } else { bufferoffset_kvs = 0; }
	keyvalue_vbuffer_t bits_vector = 0;
	keyvalue_vbuffer_t bits2_vector = 0;
	
	// if(en == true){ if(loc == *prevkey){ loc = loc + 1; } *prevkey = loc; } // FIXME.????????
	if(en == true){ if(loc == *prevkey){ loc = (loc + 1) % 8; } *prevkey = loc; } // FIXME.????????
	
	// read 
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 113::1", bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VDESTDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
	#endif
	
	// read-special (cf)
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == CF){ bits2_vector = vbuffer[bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA2 + loc]; } // CUSTOMPROVISION #ifdef CF_ALGORITHM 
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == CF){ vmdata2 = MEMCAS_READFROMBUFFER_VDATAWITHVMASK(bits2_vector); }
	
	// read-special (hits)
	unsigned int src_dest_edge = MEMCAS_READEDIR(kvdata.key);
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == HITS){ if(src_dest_edge == 0){ loc = loc*2; } else{ loc = loc*2 + 1; }} // CUSTOMPROVISION #ifdef HITS_ALGORITHM 
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 114::1", bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1 + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VDESTDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
	#endif
	
	// read (general)
	if(en == true){ bits_vector = vbuffer[bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1 + (loc / VDATA_SHRINK_RATIO)]; }
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true){ vmdata = MEMCAS_READFROMBUFFER_VDATAWITHVMASK2(loc, bits_vector); }
			#else 
				if(en == true){ vmdata = MEMCAS_READFROMBUFFER_VDATAWITHVMASK(bits_vector); }
					#endif
					
	// reduce 
	value_t new_vprop = REDUCES_reducefunc(vmdata.vdata, vmdata2.vdata, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	if(en == true && new_vprop != vmdata.vdata){ vmdata.vmask = 1; } else { vmdata.vmask = 0; } // NEWCHANGE.
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true && new_vprop != vmdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE MASK SEEN AT: loc: "<<loc<<", vmdata.vdata: "<<vmdata.vdata<<", vmdata.vmask: "<<vmdata.vmask<<endl; }
	if(en == true && new_vprop != vmdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILS_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
	
	// write
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true){ MEMCAS_WRITETOBUFFER_VDATAWITHVMASK2(loc, vbuffer, bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, vmdata.vmask); }
			#else 
				if(en == true){ MEMCAS_WRITETOBUFFER_VDATAWITHVMASK(loc, vbuffer, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, vmdata.vmask); }
					#endif

	#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
	// collections[1].data1 += 1;
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(globalparams.ACTSPARAMS_INSTID, 1);
	if(en == true){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(globalparams.ACTSPARAMS_INSTID, 1); }
	#endif
	return;
}

void REDUCES_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth){
	unsigned int s_ = s % VECTOR_SIZE;
	
 if(s_==0){ 
		depths[0] = 0; 
		depths[1] = 1; 
		depths[2] = 2; 
		depths[3] = 3; 
		depths[4] = 4; 
		depths[5] = 5; 
		depths[6] = 6; 
		depths[7] = 7; 
	}
else if(s_==1){ 
		depths[0] = 7; 
		depths[1] = 0; 
		depths[2] = 1; 
		depths[3] = 2; 
		depths[4] = 3; 
		depths[5] = 4; 
		depths[6] = 5; 
		depths[7] = 6; 
	}
else if(s_==2){ 
		depths[0] = 6; 
		depths[1] = 7; 
		depths[2] = 0; 
		depths[3] = 1; 
		depths[4] = 2; 
		depths[5] = 3; 
		depths[6] = 4; 
		depths[7] = 5; 
	}
else if(s_==3){ 
		depths[0] = 5; 
		depths[1] = 6; 
		depths[2] = 7; 
		depths[3] = 0; 
		depths[4] = 1; 
		depths[5] = 2; 
		depths[6] = 3; 
		depths[7] = 4; 
	}
else if(s_==4){ 
		depths[0] = 4; 
		depths[1] = 5; 
		depths[2] = 6; 
		depths[3] = 7; 
		depths[4] = 0; 
		depths[5] = 1; 
		depths[6] = 2; 
		depths[7] = 3; 
	}
else if(s_==5){ 
		depths[0] = 3; 
		depths[1] = 4; 
		depths[2] = 5; 
		depths[3] = 6; 
		depths[4] = 7; 
		depths[5] = 0; 
		depths[6] = 1; 
		depths[7] = 2; 
	}
else if(s_==6){ 
		depths[0] = 2; 
		depths[1] = 3; 
		depths[2] = 4; 
		depths[3] = 5; 
		depths[4] = 6; 
		depths[5] = 7; 
		depths[6] = 0; 
		depths[7] = 1; 
	}
else { 
		depths[0] = 1; 
		depths[1] = 2; 
		depths[2] = 3; 
		depths[3] = 4; 
		depths[4] = 5; 
		depths[5] = 6; 
		depths[6] = 7; 
		depths[7] = 0; 
	}
	return;
}

void REDUCES_RearrangeLayoutV(unsigned int s, keyvalue_buffer_t vdata[VECTOR_SIZE], keyvalue_buffer_t vdata2[VECTOR_SIZE]){
	unsigned int s_ = s;// % VECTOR_SIZE;
 if(s_==0){ 
		vdata2[0] = vdata[0]; 
		vdata2[1] = vdata[1]; 
		vdata2[2] = vdata[2]; 
		vdata2[3] = vdata[3]; 
		vdata2[4] = vdata[4]; 
		vdata2[5] = vdata[5]; 
		vdata2[6] = vdata[6]; 
		vdata2[7] = vdata[7]; 
	}
else if(s_==1){ 
		vdata2[7] = vdata[0]; 
		vdata2[0] = vdata[1]; 
		vdata2[1] = vdata[2]; 
		vdata2[2] = vdata[3]; 
		vdata2[3] = vdata[4]; 
		vdata2[4] = vdata[5]; 
		vdata2[5] = vdata[6]; 
		vdata2[6] = vdata[7]; 
	}
else if(s_==2){ 
		vdata2[6] = vdata[0]; 
		vdata2[7] = vdata[1]; 
		vdata2[0] = vdata[2]; 
		vdata2[1] = vdata[3]; 
		vdata2[2] = vdata[4]; 
		vdata2[3] = vdata[5]; 
		vdata2[4] = vdata[6]; 
		vdata2[5] = vdata[7]; 
	}
else if(s_==3){ 
		vdata2[5] = vdata[0]; 
		vdata2[6] = vdata[1]; 
		vdata2[7] = vdata[2]; 
		vdata2[0] = vdata[3]; 
		vdata2[1] = vdata[4]; 
		vdata2[2] = vdata[5]; 
		vdata2[3] = vdata[6]; 
		vdata2[4] = vdata[7]; 
	}
else if(s_==4){ 
		vdata2[4] = vdata[0]; 
		vdata2[5] = vdata[1]; 
		vdata2[6] = vdata[2]; 
		vdata2[7] = vdata[3]; 
		vdata2[0] = vdata[4]; 
		vdata2[1] = vdata[5]; 
		vdata2[2] = vdata[6]; 
		vdata2[3] = vdata[7]; 
	}
else if(s_==5){ 
		vdata2[3] = vdata[0]; 
		vdata2[4] = vdata[1]; 
		vdata2[5] = vdata[2]; 
		vdata2[6] = vdata[3]; 
		vdata2[7] = vdata[4]; 
		vdata2[0] = vdata[5]; 
		vdata2[1] = vdata[6]; 
		vdata2[2] = vdata[7]; 
	}
else if(s_==6){ 
		vdata2[2] = vdata[0]; 
		vdata2[3] = vdata[1]; 
		vdata2[4] = vdata[2]; 
		vdata2[5] = vdata[3]; 
		vdata2[6] = vdata[4]; 
		vdata2[7] = vdata[5]; 
		vdata2[0] = vdata[6]; 
		vdata2[1] = vdata[7]; 
	}
else { 
		vdata2[1] = vdata[0]; 
		vdata2[2] = vdata[1]; 
		vdata2[3] = vdata[2]; 
		vdata2[4] = vdata[3]; 
		vdata2[5] = vdata[4]; 
		vdata2[6] = vdata[5]; 
		vdata2[7] = vdata[6]; 
		vdata2[0] = vdata[7]; 
	}
	return;
}

void REDUCES_RearrangeLayoutEn(unsigned int s, bool en[VECTOR_SIZE], bool en2[VECTOR_SIZE]){
	unsigned int s_ = s;// % VECTOR_SIZE;
 if(s_==0){ 
		en2[0] = en[0]; 
		en2[1] = en[1]; 
		en2[2] = en[2]; 
		en2[3] = en[3]; 
		en2[4] = en[4]; 
		en2[5] = en[5]; 
		en2[6] = en[6]; 
		en2[7] = en[7]; 
	}
else if(s_==1){ 
		en2[7] = en[0]; 
		en2[0] = en[1]; 
		en2[1] = en[2]; 
		en2[2] = en[3]; 
		en2[3] = en[4]; 
		en2[4] = en[5]; 
		en2[5] = en[6]; 
		en2[6] = en[7]; 
	}
else if(s_==2){ 
		en2[6] = en[0]; 
		en2[7] = en[1]; 
		en2[0] = en[2]; 
		en2[1] = en[3]; 
		en2[2] = en[4]; 
		en2[3] = en[5]; 
		en2[4] = en[6]; 
		en2[5] = en[7]; 
	}
else if(s_==3){ 
		en2[5] = en[0]; 
		en2[6] = en[1]; 
		en2[7] = en[2]; 
		en2[0] = en[3]; 
		en2[1] = en[4]; 
		en2[2] = en[5]; 
		en2[3] = en[6]; 
		en2[4] = en[7]; 
	}
else if(s_==4){ 
		en2[4] = en[0]; 
		en2[5] = en[1]; 
		en2[6] = en[2]; 
		en2[7] = en[3]; 
		en2[0] = en[4]; 
		en2[1] = en[5]; 
		en2[2] = en[6]; 
		en2[3] = en[7]; 
	}
else if(s_==5){ 
		en2[3] = en[0]; 
		en2[4] = en[1]; 
		en2[5] = en[2]; 
		en2[6] = en[3]; 
		en2[7] = en[4]; 
		en2[0] = en[5]; 
		en2[1] = en[6]; 
		en2[2] = en[7]; 
	}
else if(s_==6){ 
		en2[2] = en[0]; 
		en2[3] = en[1]; 
		en2[4] = en[2]; 
		en2[5] = en[3]; 
		en2[6] = en[4]; 
		en2[7] = en[5]; 
		en2[0] = en[6]; 
		en2[1] = en[7]; 
	}
else { 
		en2[1] = en[0]; 
		en2[2] = en[1]; 
		en2[3] = en[2]; 
		en2[4] = en[3]; 
		en2[5] = en[4]; 
		en2[6] = en[5]; 
		en2[7] = en[6]; 
		en2[0] = en[7]; 
	}
	return;
}

void REDUCES_ACTSreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	if(enable == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }
	// exit(EXIT_SUCCESS);
	
	keyvalue_buffer_t kvdata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata complete
	keyvalue_buffer_t kvdata2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata2 complete
	unsigned int depths[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	bool enx2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx2 complete
	unsigned int d_kvs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=d_kvs complete
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=localcapsule_kvs complete
	unsigned int prevkey[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=prevkey complete
	
	keyvalue_t invalid_dataa; invalid_dataa.key = INVALIDDATA; invalid_dataa.value = INVALIDDATA;
	keyvalue_buffer_t invalid_data = UTILS_GETKV(invalid_dataa);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	
	buffer_type maxsize_kvs[2]; maxsize_kvs[0] = 0; maxsize_kvs[1] = 0;
	unsigned int _poff = 0;
	REDUCEBUFFERPARTITIONS_LOOP1: for(unsigned int cid=0; cid<2; cid++){ // NB: 'globalparams.ACTSPARAMS_NUM_PARTITIONS is 16, but only 8 can be accessed per time, so we need 2 fetches'
		REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			unsigned int ssize_kvs = localcapsule[_poff + p].value / VECTOR_SIZE;
			if(maxsize_kvs[cid] < ssize_kvs){ maxsize_kvs[cid] = ssize_kvs; }
		}
		_poff += globalparams.ACTSPARAMS_NUM_PARTITIONS/2;
	}
	buffer_type height_kvs = (localcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].key + localcapsule[globalparams.ACTSPARAMS_NUM_PARTITIONS-1].value) / VECTOR_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"reduceupdates2: maxsize_kvs[0]: "<<maxsize_kvs[0]<<", maxsize_kvs[1]: "<<maxsize_kvs[1]<<", height_kvs: "<<height_kvs<<endl;
	#endif
		
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"reduceupdates2: localcapsule["<<p<<"].key: "<<localcapsule[p].key<<", localcapsule["<<p<<"].value: "<<localcapsule[p].value<<endl;
		#endif
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	}
	
	// buffer_type capsule2_offset=0;
	REDUCEBUFFERPARTITIONS_LOOP2: for(buffer_type capsule_offset=0; capsule_offset<globalparams.ACTSPARAMS_NUM_PARTITIONS; capsule_offset+=VECTOR_SIZE){
		unsigned int mmaxsz_kvs = maxsize_kvs[capsule_offset / VECTOR_SIZE];
		// if(globalparams.ALGORITHMINFO_GRAPHALGORITHMID == CF){ capsule2_offset = capsule_offset; } // CUSTOMPROVISION		
		REDUCEBUFFERPARTITIONS_LOOP2B: for(unsigned int r=0; r<VECTOR_SIZE; r++){
			REDUCEBUFFERPARTITIONS_LOOP2C: for(buffer_type i=0; i<mmaxsz_kvs; i++){ 
			#pragma HLS PIPELINE II=2
			#pragma HLS dependence variable=vbuffer inter false // NEWCHANGE. ///////////////////////////////////////////////////////////////////////////
				// get layout
				REDUCES_GetXYLayoutV(r, depths, 0);
			
				// calculate enable
				enx[0] = true;	
				enx[1] = true;	
				enx[2] = true;	
				enx[3] = true;	
				enx[4] = true;	
				enx[5] = true;	
				enx[6] = true;	
				enx[7] = true;	
 
				
				// read 
				unsigned int tdepth0 = capsule_offset + depths[0];
				d_kvs[0] = localcapsule_kvs[tdepth0].key + i; 	
				unsigned int tdepth1 = capsule_offset + depths[1];
				d_kvs[1] = localcapsule_kvs[tdepth1].key + i; 	
				unsigned int tdepth2 = capsule_offset + depths[2];
				d_kvs[2] = localcapsule_kvs[tdepth2].key + i; 	
				unsigned int tdepth3 = capsule_offset + depths[3];
				d_kvs[3] = localcapsule_kvs[tdepth3].key + i; 	
				unsigned int tdepth4 = capsule_offset + depths[4];
				d_kvs[4] = localcapsule_kvs[tdepth4].key + i; 	
				unsigned int tdepth5 = capsule_offset + depths[5];
				d_kvs[5] = localcapsule_kvs[tdepth5].key + i; 	
				unsigned int tdepth6 = capsule_offset + depths[6];
				d_kvs[6] = localcapsule_kvs[tdepth6].key + i; 	
				unsigned int tdepth7 = capsule_offset + depths[7];
				d_kvs[7] = localcapsule_kvs[tdepth7].key + i; 	
 	
 // FIXME.
				if(i < localcapsule_kvs[tdepth0].value){ enx[0] = true; kvdata[0] = buffer[0][d_kvs[0]]; } else { enx[0] = false; kvdata[0] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth1].value){ enx[1] = true; kvdata[1] = buffer[1][d_kvs[1]]; } else { enx[1] = false; kvdata[1] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth2].value){ enx[2] = true; kvdata[2] = buffer[2][d_kvs[2]]; } else { enx[2] = false; kvdata[2] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth3].value){ enx[3] = true; kvdata[3] = buffer[3][d_kvs[3]]; } else { enx[3] = false; kvdata[3] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth4].value){ enx[4] = true; kvdata[4] = buffer[4][d_kvs[4]]; } else { enx[4] = false; kvdata[4] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth5].value){ enx[5] = true; kvdata[5] = buffer[5][d_kvs[5]]; } else { enx[5] = false; kvdata[5] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth6].value){ enx[6] = true; kvdata[6] = buffer[6][d_kvs[6]]; } else { enx[6] = false; kvdata[6] = invalid_data; }	
 // FIXME.
				if(i < localcapsule_kvs[tdepth7].value){ enx[7] = true; kvdata[7] = buffer[7][d_kvs[7]]; } else { enx[7] = false; kvdata[7] = invalid_data; }	
				
				// re-arrange 
				REDUCES_RearrangeLayoutV(r, kvdata, kvdata2);
				REDUCES_RearrangeLayoutEn(r, enx, enx2); // NEWCHANGE.
			
				// reduce 
				REDUCES_reducevector(enx2[0], 0, kvdata2[0], vbuffer[capsule_offset + 0], vbuffer2[capsule_offset + 0], 0, sweepparams.upperlimit, &prevkey[0], sweepparams, globalparams
					#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
					,collections[capsule_offset+0]
					#endif 
					);
				REDUCES_reducevector(enx2[1], 1, kvdata2[1], vbuffer[capsule_offset + 1], vbuffer2[capsule_offset + 1], 0, sweepparams.upperlimit, &prevkey[1], sweepparams, globalparams
					#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
					,collections[capsule_offset+1]
					#endif 
					);
				REDUCES_reducevector(enx2[2], 2, kvdata2[2], vbuffer[capsule_offset + 2], vbuffer2[capsule_offset + 2], 0, sweepparams.upperlimit, &prevkey[2], sweepparams, globalparams
					#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
					,collections[capsule_offset+2]
					#endif 
					);
				REDUCES_reducevector(enx2[3], 3, kvdata2[3], vbuffer[capsule_offset + 3], vbuffer2[capsule_offset + 3], 0, sweepparams.upperlimit, &prevkey[3], sweepparams, globalparams
					#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
					,collections[capsule_offset+3]
					#endif 
					);
				REDUCES_reducevector(enx2[4], 4, kvdata2[4], vbuffer[capsule_offset + 4], vbuffer2[capsule_offset + 4], 0, sweepparams.upperlimit, &prevkey[4], sweepparams, globalparams
					#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
					,collections[capsule_offset+4]
					#endif 
					);
				REDUCES_reducevector(enx2[5], 5, kvdata2[5], vbuffer[capsule_offset + 5], vbuffer2[capsule_offset + 5], 0, sweepparams.upperlimit, &prevkey[5], sweepparams, globalparams
					#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
					,collections[capsule_offset+5]
					#endif 
					);
				REDUCES_reducevector(enx2[6], 6, kvdata2[6], vbuffer[capsule_offset + 6], vbuffer2[capsule_offset + 6], 0, sweepparams.upperlimit, &prevkey[6], sweepparams, globalparams
					#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
					,collections[capsule_offset+6]
					#endif 
					);
				REDUCES_reducevector(enx2[7], 7, kvdata2[7], vbuffer[capsule_offset + 7], vbuffer2[capsule_offset + 7], 0, sweepparams.upperlimit, &prevkey[7], sweepparams, globalparams
					#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
					,collections[capsule_offset+7]
					#endif 
					);
			}
		}
	}
	
	/* ///////////////////////////////////////////////////////////////// REMOVEME.
	cout<<"============================ MEMACCESSS_saveV SEEN. globalparams.SIZEKVS2_REDUCEPARTITION: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<endl;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	SAVEVDATA_LOOP1: for(buffer_type i=0; i<globalparams.SIZEKVS2_REDUCEPARTITION; i++){
	#pragma HLS PIPELINE II=1
	
		vdata[0] = vbuffer[0][0 + i];
	
		vdata[1] = vbuffer[1][0 + i];
	
		vdata[2] = vbuffer[2][0 + i];
	
		vdata[3] = vbuffer[3][0 + i];
	
		vdata[4] = vbuffer[4][0 + i];
	
		vdata[5] = vbuffer[5][0 + i];
	
		vdata[6] = vbuffer[6][0 + i];
	
		vdata[7] = vbuffer[7][0 + i];
	
		vdata[8] = vbuffer[8][0 + i];
	
		vdata[9] = vbuffer[9][0 + i];
	
		vdata[10] = vbuffer[10][0 + i];
	
		vdata[11] = vbuffer[11][0 + i];
	
		vdata[12] = vbuffer[12][0 + i];
	
		vdata[13] = vbuffer[13][0 + i];
	
		vdata[14] = vbuffer[14][0 + i];
	
		vdata[15] = vbuffer[15][0 + i];
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			unsigned int K = vdata[2*v];
			unsigned int V = vdata[2*v+1];
			if(MEMCAS_READVMASK(K)==1){ cout<<"---------------------------- MEMACCESSS_saveV:: MASK=1 SEEN. index: "<<i<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"---------------------------- MEMACCESSS_saveV:: MASK=1 SEEN. index: "<<i<<endl; }	
		}
		#endif
	}
	// exit();
	//////////////////////////////////////////////////////////////// REMOVEME.
	// actsutilityobj->printglobalvars();
	exit(EXIT_SUCCESS); //// */
	return;
}

void REDUCES_TRADreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	if(enable == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }
	// exit(EXIT_SUCCESS);
	
	keyvalue_buffer_t kvdata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata complete
	keyvalue_buffer_t kvdata2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata2 complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	unsigned int prevkey[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=prevkey complete
	
	keyvalue_t invalid_dataa; invalid_dataa.key = INVALIDDATA; invalid_dataa.value = INVALIDDATA;
	keyvalue_buffer_t invalid_data = UTILS_GETKV(invalid_dataa);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	
	REDUCEBUFFERPARTITIONS_LOOP2A: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
		REDUCEBUFFERPARTITIONS_LOOP2B: for(buffer_type i=localcapsule[p].key; i<localcapsule[p].key + localcapsule[p].value; i++){
		#pragma HLS PIPELINE II=4
			// read 
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("readandprocess2(14)::DEBUG CODE 14::1", i / VECTOR_SIZE, MAX_DESTBLOCKRAM_SIZE, i, MAX_DESTBLOCKRAM_SIZE, NAp);
			#endif
			keyvalue_buffer_t kvdata = buffer[i % VECTOR_SIZE][i / VECTOR_SIZE];

			// reduce 
			REDUCES_reducevector(ON, p, kvdata, vbuffer[p], vbuffer2[p], 0, sweepparams.upperlimit, &prevkey[p], sweepparams, globalparams
				#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
				,collections[p]
				#endif 
				);
		}
	}

	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS); ////
	return;
}

void REDUCES_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	
	return REDUCES_ACTSreduceandbuffer(enable, buffer, localcapsule, vbuffer, vbuffer2, sweepparams, globalposition, globalparams, collections);
	
	/** #ifdef CONFIG_ALL_EVALUATIONTYPES_IN_ONE_KERNEL
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC == ON){
		return REDUCES_ACTSreduceandbuffer(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
	} else {
		if(globalparams.EVALUATION_ACTS_VERYBARE == ON){
			// NAp
		} else {
			return REDUCES_TRADreduceandbuffer(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
		}
	#else 
		#ifdef CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
		return REDUCES_ACTSreduceandbuffer(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
		#else 
			#ifdef CONFIG_ACTS_VERYBARE
			// NAp
			#else 
			return REDUCES_TRADreduceandbuffer(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
			#endif 
		#endif
	#endif  */
}	
#endif 
#ifdef CONFIG_ENABLECLASS_ACTS_MERGE
typedef struct {
	keyvalue_vbuffer_t srcp;
	keyvalue_vbuffer_t dstp; // keyvalue_vbuffer_t, uint32_type
	unsigned int srcvid;
	unsigned int dstvid;
	unsigned int status;
	unsigned int tocommit;
} arrow_t;
#define TRADGP_BUFFER_SIZE DOUBLE_BLOCKRAM_SIZE
#define TRADGP_BUFFER_BIGSIZE (DOUBLE_BLOCKRAM_SIZE * VECTOR2_SIZE)
#define TRADGP_LOADCHUNKSZ 64 // 256 // 1024

void TradGPS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"TradGP:: running traditional sssp... "<<endl;
	#endif 
	
	globalparams_t globalparamsm = UTILS_getglobalparams(mdram, 0);
	globalparams_t globalparamsvA = UTILS_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILS_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILS_getglobalparams(vdramC, 0);
	
	unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE];

	unsigned int GraphIter = globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID;
	unsigned int actvvcount_currit = UTILS_GetSecondData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID); // globalparamsm.SIZE_ACTIVEVERTICES;
	unsigned int actvvcount_nextit = 0;
	unsigned int nextGraphIter = GraphIter + 1;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"TradGP:: running traditional sssp... (iteration "<<GraphIter<<", number of active vertices to process: "<<actvvcount_currit<<")"<<endl;
	#endif 
	
	unsigned int offsetkvs_curractvvs = 0;
	unsigned int offsetkvs_nxtactvvs = 0;
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	offsetkvs_curractvvs = 0; offsetkvs_nxtactvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE;
	#else
	if(GraphIter % 2 == 0){ offsetkvs_curractvvs = 0; offsetkvs_nxtactvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; } 
	else { offsetkvs_curractvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; offsetkvs_nxtactvvs = 0; }
	#endif 
	
	unsigned int actvvs_currentit_basekvs = 0;
	unsigned int actvvs_nextit_basekvs = 0;
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	actvvs_currentit_basekvs = GraphIter * (CONFIG_HYBRIDGPMODE_MDRAMSECTIONSZ / VECTOR2_SIZE);
	actvvs_nextit_basekvs = (GraphIter + 1) * (CONFIG_HYBRIDGPMODE_MDRAMSECTIONSZ / VECTOR2_SIZE);
	#endif 
	
	unsigned int total_edges_processed = 0;
	long double edgesprocessedinGraphIter[128]; for(unsigned int i=0; i<128; i++){ edgesprocessedinGraphIter[i] = 0; }
	for(unsigned int v=0; v<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v++){ vpmaskbuffer[v] = 0; }
	
	unsigned int actvvids[TRADGP_BUFFER_SIZE];
	keyvalue_t tempbuffer_vptr[TRADGP_BUFFER_SIZE];
	#pragma HLS DATA_PACK variable = tempbuffer_vptr
	arrow_t independent_edge[TRADGP_BUFFER_BIGSIZE];
	
	TRADGP_LOOP1: for(unsigned int i=0; i<actvvcount_currit; i+=TRADGP_LOADCHUNKSZ){
		unsigned int chunk_size = TRADGP_LOADCHUNKSZ;
		if(i + TRADGP_LOADCHUNKSZ >= actvvcount_currit){ chunk_size = actvvcount_currit - i; }
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		cout<<">>> TRADGP:: processing chunk: i "<<i<<" (of "<<actvvcount_currit<<"), chunk_size: "<<chunk_size<<", TRADGP_LOADCHUNKSZ: "<<TRADGP_LOADCHUNKSZ<<endl;
		#endif 
		
		// read & buffer (actvv ids)
		unsigned int actvvids[VECTOR2_SIZE];
		TRADGP_LOOP2: for(unsigned int i=0; i<chunk_size; i++){
		#pragma HLS PIPELINE
			actvvids[i] = UTILS_GetData(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + actvvs_currentit_basekvs, i); 
		}
		
		// read & buffer (vertex ptrs)
		TRADGP_LOOP3: for(unsigned int t=0; t<chunk_size; t++){
		// #pragma HLS PIPELINE
			unsigned int srcvid = actvvids[t];
			edge_t vptr_begin = UTILS_GetData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, srcvid); 
			edge_t vptr_end = UTILS_GetData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, srcvid + 1); 
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MERGE(2): ERROR 20", t, TRADGP_BUFFER_SIZE, vptr_begin, vptr_end, NAp);
			#endif
			tempbuffer_vptr[t].key = vptr_begin;
			tempbuffer_vptr[t].value = vptr_end;
			if(tempbuffer_vptr[t].value < tempbuffer_vptr[t].key){ tempbuffer_vptr[t].key = 0; tempbuffer_vptr[t].value = 0; }
		}
		
		// read & buffer (dstv properties)
		unsigned int index = 0;
		unsigned int sumEs = 0;
		TRADGP_LOOP4: for(unsigned int i=0; i<chunk_size; i++){
			sumEs += tempbuffer_vptr[i].value - tempbuffer_vptr[i].key;
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"TradGP:: read & buffer (dstv properties): number of edges to read: "<<tempbuffer_vptr[i].value - tempbuffer_vptr[i].key<<", (tempbuffer_vptr["<<i<<"].key: "<<tempbuffer_vptr[i].key<<", tempbuffer_vptr["<<i<<"].value: "<<tempbuffer_vptr[i].value<<", sumEs: "<<sumEs<<")"<<endl;
			#endif 
			TRADGP_LOOP4B: for(unsigned int t=tempbuffer_vptr[i].key; t<tempbuffer_vptr[i].value; t++){
			#pragma HLS PIPELINE
				unsigned int srcvid = actvvids[i];
				unsigned int dstvid = UTILS_GetData(mdram, globalparamsm.BASEOFFSETKVS_EDGESDATA, t);
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				cout<<"TradGP:: EDGE: srcvid: "<<srcvid<<", dstvid: "<<dstvid<<endl;
				#endif 
				
				unsigned int s = dstvid % NUM_PEs;
				unsigned int ldstvid = (dstvid - s) / NUM_PEs; 
				unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
				
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 21", index, TRADGP_BUFFER_BIGSIZE, dstvid, tempbuffer_vptr[i].key, tempbuffer_vptr[i].value);
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 22", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + (ldstvid / VDATA_SHRINK_RATIO), globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, index, dstvid, offset_kvs);
				#endif
				independent_edge[index].dstp = UTILS_GetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO));
				independent_edge[index].srcvid = actvvids[i];
				independent_edge[index].dstvid = dstvid;
				independent_edge[index].status = 0;
				independent_edge[index].tocommit = 0;
				// index += 1;
				#ifdef CONFIG_FORCEDCORRECT_TRADGP
				if(index + 1 < TRADGP_BUFFER_BIGSIZE-2){ index += 1; }
				#endif 
			}
		}
		
		// process 
		TRADGP_LOOP5: for(unsigned int edge=0; edge<index; edge++){
		#pragma HLS PIPELINE
			unsigned int srcvid = independent_edge[edge].srcvid;
			unsigned int dstvid = independent_edge[edge].dstvid;
			
			unsigned int s = dstvid % NUM_PEs;
			unsigned int ldstvid = (dstvid - s) / NUM_PEs; 
			unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
			
			// read 
			vmdata_t vmdata;
			#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) || defined(CONFIG_GOLDENKERNEL)	
			unsigned int offsetof_vdata = (ldstvid % VDATA_SHRINK_RATIO);
			vmdata.vdata = UTILS_SWREADBITSFROM_UINTV(independent_edge[edge].dstp, offsetof_vdata, SIZEOF_VDATA);
			vmdata.vmask = UTILS_SWREADBITSFROM_UINTV(independent_edge[edge].dstp, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK);
			#else
			vmdata.vdata = MEMCAS_READVDATA(independent_edge[edge].dstp); 
			vmdata.vmask = MEMCAS_READVMASK(independent_edge[edge].dstp); 
			#endif 
			
			// reduce 
			value_t new_vprop = REDUCES_reducefunc(vmdata.vdata, NAp, NAp, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA.ALGORITHMINFO_GRAPHALGORITHMID);
			if(new_vprop != vmdata.vdata){ vmdata.vmask = 1; } else { vmdata.vmask = 0; } // NEWCHANGE.
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(new_vprop != vmdata.vdata){ cout<<">>> TRADGP::REDUCE VECTOR(1):: ACTIVE REDUCE SEEN @ srcvid: "<<srcvid<<", dstvid: "<<dstvid<<endl; }
			#endif
			
			// write 
			if(vmdata.vmask == 1){
				#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) || defined(CONFIG_GOLDENKERNEL)
				unsigned int offsetof_vdata = (ldstvid % VDATA_SHRINK_RATIO);
				UTILS_SWWRITEBITSTO_UINTV(&independent_edge[edge].dstp, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK, vmdata.vmask);
				#else 
				MEMCAS_WRITEVMASK(&independent_edge[edge].dstp, vmdata.vmask); 
				#endif
				independent_edge[edge].tocommit = 1;
			}
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(0, 1);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(0, 1); 
			#endif 
		}
		
		// commit 
		TRADGP_LOOP6: for(unsigned int edge=0; edge<index; edge++){
			#pragma HLS PIPELINE
			unsigned int srcvid = independent_edge[edge].srcvid;
			unsigned int dstvid = independent_edge[edge].dstvid;
			
			if(independent_edge[edge].tocommit == 1){ // (vtemp != vprop)
				unsigned int s = independent_edge[edge].dstvid % NUM_PEs;
				unsigned int ldstvid = (independent_edge[edge].dstvid - s) / NUM_PEs; 
				unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
				
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 28", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + (ldstvid / VDATA_SHRINK_RATIO), globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, index, dstvid, offset_kvs);
				#endif
				UTILS_SetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				UTILS_SetData(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				UTILS_SetData(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				
				// set active vertices mask in vdramA, vdramB and vdramC
				// vpmaskbuffer[independent_edge[edge].dstvid / (1 << globalparamsvA.SIZEKVS2_PROCESSEDGESPARTITION)PROCESSPARTITIONSZ] = 1;
				vpmaskbuffer[independent_edge[edge].dstvid / (1 << globalparamsvA.SIZEKVS2_PROCESSEDGESPARTITION)] = 1;
				
				// collect stats 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvsreduced(0, 1);
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(0, 1); 
				#endif 
			}
		}
	}
	
	UTILS_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILS_SetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILS_SetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILS_SetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	
	#if not defined(FPGA_IMPL) && defined(CONFIG_COLLECT_DATAS3_DURING_RUN)
	cout<<">>> TRADGP::[A][Iter: "<<GraphIter<<"]:: num edges processed: "<<actsutilityobj->globalstats_getcountnumvalidprocessedges(0)<<"("<<actsutilityobj->globalstats_getcountnumvalidprocessedges(0)<<"), num vertex updates reduced: "<<actsutilityobj->globalstats_getcountvalidkvsreduced(0)<<"("<<actsutilityobj->globalstats_getcountvalidkvsreduced(0)<<")"<<endl;	
	actsutilityobj->clearglobalvars();
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"TradGP: number of active vertices for iteration "<<nextGraphIter<<": "<<actvvcount_nextit<<""<<endl;
	if(actvvcount_nextit == 0){ cout<<"TradGP: no more active vertices to process. breaking out... "<<endl; }
	#endif 
	// exit(EXIT_SUCCESS); //
	return;
}	
#endif 
#ifdef CONFIG_ENABLECLASS_ACTS_MERGE
#define CONFIG_MERGE_VDATAS
#ifndef ALLVERTEXISACTIVE_ALGORITHM
// #define CONFIG_MERGE_COLLECTACTIVEVPARTITIONS
#endif
// #define MERGECONFIG_ACTIVATE_HYBRIDMODE

unsigned int MERGES_actvpstatsoffset(globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ _offset += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }} else { _offset = 1; }
	return _offset;
}

void MERGES_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	// return;
	
	globalparams_t globalparams = UTILS_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILS_getglobalparams(vdram, 0);
	unsigned int offset_kvs = globalparams.ACTSPARAMS_INSTID * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to kvdram."<<endl; 	
	#endif
	MERGES_BROADCASTVTXS_LOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){	
		MERGES_BROADCASTVTXS_LOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
		#pragma HLS PIPELINE II=1
			kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k];
		}
	}
	return;
}

void MERGES_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices (997)..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILS_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILS_getglobalparams(vdram, 0);
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE];
	unsigned int _offset = MERGES_actvpstatsoffset(globalparams);
	MEMACCESSS_retreievekvstats(kvdram, globalstatsbuffer, globalparams, _offset, globalparams.NUM_REDUCEPARTITIONS);
	
	unsigned int i = globalparams.ACTSPARAMS_INSTID % NUMCOMPUTEUNITS_SLR2;
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"MERGES_mergeVs:: globalparams.ACTSPARAMS_INSTID: "<<globalparams.ACTSPARAMS_INSTID<<", i: "<<i<<endl;
	#endif 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparams.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(reduce_partition >= _NUM_VALIDREDUCEPARTITIONS){ globalstatsbuffer[reduce_partition].value = 0; }
	}
	#endif 	
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer[i].value > 0){ cout<<"MERGE: globalstatsbuffer["<<i<<"].key : "<<globalstatsbuffer[i].key<<", globalstatsbuffer["<<i<<"].value : "<<globalstatsbuffer[i].value<<endl; }}
	#endif 
	
	bool pass3 = false; // false; // true;
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 	
	#endif 
	unsigned int voffset_kvs2 = i * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	MERGES_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGES_merge Vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGES_mergeVs: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3 == true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGES_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGES_mergeVs---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGES_mergeVs: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ MERGE:: MASK=1 SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ MERGE:: MASK=1 SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// active reduce partitions
	unsigned int offset_kvs = i * globalparamsv.NUM_REDUCEPARTITIONS;
	MERGES_MERGEVSLOOP3: for(unsigned int reduce_partition=0; reduce_partition<globalparamsv.NUM_REDUCEPARTITIONS; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGES_mergeVs: ERROR 1s20", offset_kvs + reduce_partition, globalparamsv.SIZE_KVSTATSDRAM, NAp, NAp, NAp);
		#endif
		UTILS_SetSecondData(vdram, globalparamsv.BASEOFFSETKVS_STATSDRAM + offset_kvs + reduce_partition, globalstatsbuffer[reduce_partition].value);
	}
	
	// stats to be returned to host
	#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
	unsigned int vtmp = UTILS_GetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int vtmp2 = UTILS_GetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int vtmp3 = UTILS_GetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int vtmp4 = UTILS_GetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	
	unsigned int tmp = UTILS_GetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int tmp2 = UTILS_GetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int tmp3 = UTILS_GetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	unsigned int tmp4 = UTILS_GetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, vtmp + tmp);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, vtmp2 + tmp2);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, vtmp3 + tmp3);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, vtmp4 + tmp4);
		
	#ifdef _DEBUGMODE_STATS	
	unsigned int dat = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	unsigned int dat2 = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	unsigned int dat3 = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	unsigned int dat4 = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[0].key;
	// cout<<">>> MERGE::[COMMENT-ME-OUT]:: number of active vertices for next iteration (iteration "<<globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1<<"): dat: "<<dat<<", dat2: "<<dat2<<", dat3: "<<dat3<<", dat4: "<<dat4<<endl;
	#endif 
	#endif 
	// exit(EXIT_SUCCESS); // 
	return;
}

void MERGES_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: ACTS EXCHANGE LAUNCHED. Exchanging vertices across different SLRs..."<<endl; 
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
	unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE];
	unsigned int loadcount[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=loadcount complete
	unsigned int numactvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=numactvvs complete
	unsigned int local_actvv[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv complete
	unsigned int local_actvv2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv2 complete
	unsigned int global_actvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=global_actvvs complete
	uint32_type vpmaskVecSum[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vpmaskVecSum complete	
	#endif 	
	value_t vdatas[VECTOR2_SIZE];
	
	unsigned int reduce_partition = 0;
	unsigned int partition_offset = 0;
	
	globalparams_t globalparamsvA = UTILS_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILS_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILS_getglobalparams(vdramC, 0);
	globalparams_t globalparamsm = UTILS_getglobalparams(mdram, 0);
	unsigned int actvpstats_beginoffset = MERGES_actvpstatsoffset(globalparamsvA);
	
	UTILS_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	unsigned int nextGraphIter = globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID;
	
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR2 = NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR1 = NUMCOMPUTEUNITS_SLR1 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR0 = NUMCOMPUTEUNITS_SLR0 * globalparamsvA.NUM_REDUCEPARTITIONS;
	
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	unsigned int _NUM_VALIDREDUCEPARTITIONS = (globalparamsvA.NUM_REDUCEPARTITIONS + (VDATA_SHRINK_RATIO - 1)) / VDATA_SHRINK_RATIO;
	#else 
	unsigned int _NUM_VALIDREDUCEPARTITIONS = globalparamsvA.NUM_REDUCEPARTITIONS;//
	#endif 
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	
	uint32_type pmaski = 0;
	
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ loadcount[v] = 0; numactvvs[v] = 0; }
	for(unsigned int v=0; v<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v++){ vpmaskbuffer[v] = 0; }
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ vpmaskVecSum[v] = 0; }
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif
	// active reduce partitions
	MERGES_EXCHANGEC_LOOP1A: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR2; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGES_exchange: ERROR 1s20", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif 
		unsigned int tempdata = UTILS_GetSecondData(vdramA, globalparamsvA.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer0[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] = 1; }
	}
	
	MERGES_EXCHANGEC_LOOP1B: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR1; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGES_exchange: ERROR 1s21", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif
		unsigned int tempdata = UTILS_GetSecondData(vdramB, globalparamsvB.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer1[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH]; }
	}
	
	MERGES_EXCHANGEC_LOOP1C: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR0; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGES_exchange: ERROR 1s22", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif
		unsigned int tempdata = UTILS_GetSecondData(vdramC, globalparamsvC.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer2[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH]; }
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
	for(unsigned int i=0; i<_TOTALNUMREDUCEPARTITIONS_SLR0; i++){ if(globalstatsbuffer0[i / 32].data[i % 32] > 0 || globalstatsbuffer1[i / 32].data[i % 32] > 0 || globalstatsbuffer2[i / 32].data[i % 32] > 0){ cout<<"EXCHANGE: globalstatsbuffer0["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer0[i][i / 32].data[i % 32]<<", globalstatsbuffer1["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer1[i / 32].data[i % 32]<<", globalstatsbuffer2["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer2[i / 32].data[i % 32]<<endl; }}					
	#endif 
	
	// vertices
	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGES_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPS_topkernelS: ERROR 20", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, globalparamsvA.NUM_REDUCEPARTITIONS, NAp, NAp);
			#endif
			if(globalstatsbuffer0[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer0["<<reduce_partition<<"]: "<<globalstatsbuffer0[reduce_partition]<<"] "<<endl;
				#endif
				MERGES_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 21", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 22", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					// collect active vertices
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					#else 
					UTILS_GetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					UTILS_SetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					UTILS_SetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, vdatas);
					#endif 
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						unsigned int K = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k].data[v].key;
						unsigned int V = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k].data[v].value;
						if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(1):: MASK=1 SEEN. index: "<<globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k<<endl; }
						if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(1):: MASK=1 SEEN. index: "<<globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k<<endl; }	
					}
					#endif
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = i + voffsetA_kvs + k;
					MERGES_collects(i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum, partition_offset);
					#endif 
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramA to vdramB & vdramC. [end offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif

	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramB to vdramA & vdramC. [begin offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	MERGES_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPS_topkernelS: ERROR 21", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(globalstatsbuffer1[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer[0]["<<reduce_partition<<"]: "<<globalstatsbuffer[0][reduce_partition]<<"] "<<endl;
				#endif
				MERGES_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 24", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 25", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					// collect active vertices 
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					#else 
					UTILS_GetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);
					UTILS_SetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);
					UTILS_SetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, vdatas);	
					#endif
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						unsigned int K = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k].data[v].key;
						unsigned int V = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k].data[v].value;
						if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k<<endl; }
						if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k<<endl; }	
					}
					#endif
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = NUMCOMPUTEUNITS_SLR2 + i + voffsetB_kvs + k;
					MERGES_collects(NUMCOMPUTEUNITS_SLR2 + i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum, partition_offset);
					#endif 
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramB to vdramA & vdramC. [end offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	
	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramC to vdramA & vdramB. [begin offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	MERGES_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPS_topkernelS: ERROR 22", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(globalstatsbuffer2[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", globalstatsbuffer0["<<reduce_partition<<"]: "<<globalstatsbuffer0[reduce_partition]<<"] "<<endl;
				#endif
				MERGES_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 27", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 28", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					
					#ifdef ALLVERTEXISACTIVE_ALGORITHM
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					#else 
					UTILS_GetDataset(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);
					UTILS_SetDataset(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);
					UTILS_SetDataset(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, vdatas);		
					#endif 
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						unsigned int K = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k].data[v].key;
						unsigned int V = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k].data[v].value;
						if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(3):: MASK=1 SEEN. index: "<<globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k<<endl; }
						if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(3):: MASK=1 SEEN. index: "<<globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k<<endl; }	
					}
					#endif
					
					#ifndef ALLVERTEXISACTIVE_ALGORITHM
					unsigned int rand = NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + i + voffsetC_kvs + k;
					MERGES_collects(NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + i, voffset_kvs + k, rand, vdatas, actvvbuffer, vpmaskbuffer, loadcount, numactvvs, local_actvv, local_actvv2, global_actvvs, vpmaskVecSum, partition_offset);	
					#endif 
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;			
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif

	// vertices partition masks
	#if defined(CONFIG_MERGE_COLLECTACTIVEVPARTITIONS)
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"#################################################### MERGE::EXCHANGE:: merging vertices partition masks across vdramA, vdramB & vdramC."<<endl; 
	#endif
	MERGES_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; partition++){ // BLOCKRAM_NEXTPMASK_SIZE, globalparamsvA.NUM_PROCESSEDGESPARTITIONS
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 31", globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 32", globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 33", globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, globalparamsvA.ACTSPARAMS_TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		// cout<<"+++ TOPS_topkernelS: vpmaskbuffer["<<partition<<"]: "<<vpmaskbuffer[partition]<<endl;
		if(vpmaskbuffer[partition] > 0){ cout<<"--- TOPS_topkernelS: vpmaskbuffer["<<partition<<"]: "<<vpmaskbuffer[partition]<<endl; }
		#endif
		
		UTILS_SetFirstData(vdramA, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		UTILS_SetFirstData(vdramB, globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		UTILS_SetFirstData(vdramC, globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, vpmaskbuffer[partition]);
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"--- TOPS_topkernelS: vdramA["<<partition<<"]: "<<p<<endl; 
		#endif
	}
	#endif 
	
	// collect active vertices 
	#if not defined (ALLVERTEXISACTIVE_ALGORITHM) && defined(MERGECONFIG_ACTIVATE_HYBRIDMODE)
	unsigned int maxloadcount_vs = 0;
	unsigned int maxnumactvvs_vs = 0;
	unsigned int sumloadcount = 0;
	unsigned int num_vertices_per_bank = globalparamsvC.SIZEKVS2_REDUCEPARTITION * globalparamsvC.NUM_REDUCEPARTITIONS * VECTOR2_SIZE;
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(maxloadcount_vs < loadcount[v]){ maxloadcount_vs = loadcount[v]; }}
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(maxnumactvvs_vs < numactvvs[v]){ maxnumactvvs_vs = numactvvs[v]; }}
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ sumloadcount += loadcount[v]; }
	
	unsigned int offsetkvs_actvvs;
	if(nextGraphIter % 2 == 0){ offsetkvs_actvvs = 0; } 
	else { offsetkvs_actvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; }
	
	for(unsigned int i=0; i<maxloadcount_vs; i++){
	#pragma HLS PIPELINE II=1
	
		keyvalue_t actvv0 = actvvbuffer[0][i];
		global_actvvs[0] = UTILS_GETREALVID(actvv0.value, actvv0.key);
	
		keyvalue_t actvv1 = actvvbuffer[1][i];
		global_actvvs[1] = UTILS_GETREALVID(actvv1.value, actvv1.key);
	
		keyvalue_t actvv2 = actvvbuffer[2][i];
		global_actvvs[2] = UTILS_GETREALVID(actvv2.value, actvv2.key);
	
		keyvalue_t actvv3 = actvvbuffer[3][i];
		global_actvvs[3] = UTILS_GETREALVID(actvv3.value, actvv3.key);
	
		keyvalue_t actvv4 = actvvbuffer[4][i];
		global_actvvs[4] = UTILS_GETREALVID(actvv4.value, actvv4.key);
	
		keyvalue_t actvv5 = actvvbuffer[5][i];
		global_actvvs[5] = UTILS_GETREALVID(actvv5.value, actvv5.key);
	
		keyvalue_t actvv6 = actvvbuffer[6][i];
		global_actvvs[6] = UTILS_GETREALVID(actvv6.value, actvv6.key);
	
		keyvalue_t actvv7 = actvvbuffer[7][i];
		global_actvvs[7] = UTILS_GETREALVID(actvv7.value, actvv7.key);
	
		keyvalue_t actvv8 = actvvbuffer[8][i];
		global_actvvs[8] = UTILS_GETREALVID(actvv8.value, actvv8.key);
	
		keyvalue_t actvv9 = actvvbuffer[9][i];
		global_actvvs[9] = UTILS_GETREALVID(actvv9.value, actvv9.key);
	
		keyvalue_t actvv10 = actvvbuffer[10][i];
		global_actvvs[10] = UTILS_GETREALVID(actvv10.value, actvv10.key);
	
		keyvalue_t actvv11 = actvvbuffer[11][i];
		global_actvvs[11] = UTILS_GETREALVID(actvv11.value, actvv11.key);
	
		keyvalue_t actvv12 = actvvbuffer[12][i];
		global_actvvs[12] = UTILS_GETREALVID(actvv12.value, actvv12.key);
	
		keyvalue_t actvv13 = actvvbuffer[13][i];
		global_actvvs[13] = UTILS_GETREALVID(actvv13.value, actvv13.key);
	
		keyvalue_t actvv14 = actvvbuffer[14][i];
		global_actvvs[14] = UTILS_GETREALVID(actvv14.value, actvv14.key);
	
		keyvalue_t actvv15 = actvvbuffer[15][i];
		global_actvvs[15] = UTILS_GETREALVID(actvv15.value, actvv15.key);
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		if(i < loadcount[0] && actvvbuffer[0][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[0]<<", localvid: "<<actvv0.value<<", instance: "<<actvv0.key<<endl; }
		if(i < loadcount[1] && actvvbuffer[1][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[1]<<", localvid: "<<actvv1.value<<", instance: "<<actvv1.key<<endl; }
		if(i < loadcount[2] && actvvbuffer[2][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[2]<<", localvid: "<<actvv2.value<<", instance: "<<actvv2.key<<endl; }
		if(i < loadcount[3] && actvvbuffer[3][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[3]<<", localvid: "<<actvv3.value<<", instance: "<<actvv3.key<<endl; }
		if(i < loadcount[4] && actvvbuffer[4][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[4]<<", localvid: "<<actvv4.value<<", instance: "<<actvv4.key<<endl; }
		if(i < loadcount[5] && actvvbuffer[5][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[5]<<", localvid: "<<actvv5.value<<", instance: "<<actvv5.key<<endl; }
		if(i < loadcount[6] && actvvbuffer[6][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[6]<<", localvid: "<<actvv6.value<<", instance: "<<actvv6.key<<endl; }
		if(i < loadcount[7] && actvvbuffer[7][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[7]<<", localvid: "<<actvv7.value<<", instance: "<<actvv7.key<<endl; }
		if(i < loadcount[8] && actvvbuffer[8][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[8]<<", localvid: "<<actvv8.value<<", instance: "<<actvv8.key<<endl; }
		if(i < loadcount[9] && actvvbuffer[9][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[9]<<", localvid: "<<actvv9.value<<", instance: "<<actvv9.key<<endl; }
		if(i < loadcount[10] && actvvbuffer[10][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[10]<<", localvid: "<<actvv10.value<<", instance: "<<actvv10.key<<endl; }
		if(i < loadcount[11] && actvvbuffer[11][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[11]<<", localvid: "<<actvv11.value<<", instance: "<<actvv11.key<<endl; }
		if(i < loadcount[12] && actvvbuffer[12][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[12]<<", localvid: "<<actvv12.value<<", instance: "<<actvv12.key<<endl; }
		if(i < loadcount[13] && actvvbuffer[13][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[13]<<", localvid: "<<actvv13.value<<", instance: "<<actvv13.key<<endl; }
		if(i < loadcount[14] && actvvbuffer[14][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[14]<<", localvid: "<<actvv14.value<<", instance: "<<actvv14.key<<endl; }
		if(i < loadcount[15] && actvvbuffer[15][i].value != INVALIDDATA){ cout<<"~~~~~~ MERGE::EXCHANGE(X):: actvv: "<<global_actvvs[15]<<", localvid: "<<actvv15.value<<", instance: "<<actvv15.key<<endl; }
		#endif	
		
		UTILS_SetDataset(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + offsetkvs_actvvs + i, global_actvvs);
	}
	UTILS_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES, maxloadcount_vs * VECTOR2_SIZE);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<"MERGE::EXCHANGE:: loadcount["<<v<<"]: "<<loadcount[v]<<endl; }
	if(maxloadcount_vs < maxnumactvvs_vs){ cout<<"MERGE::EXCHANGE:: too many active vertices. Use ACTS in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	else{ cout<<"MERGE::EXCHANGE:: few active vertices. Use TradGP in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	#endif 
	#endif 
	
	// judging what mode (ACTS or TradGP) to use for next iteration
	#if not defined (ALLVERTEXISACTIVE_ALGORITHM) && defined(MERGECONFIG_ACTIVATE_HYBRIDMODE) && defined(CONFIG_HYBRIDGPMODE_ONLINESWITCHING)
	unsigned int _mode = ON; if(maxloadcount_vs <= maxnumactvvs_vs){ _mode = OFF; } else { _mode = ON; }
	UTILS_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILS_SetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILS_SetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	UTILS_SetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + nextGraphIter, _mode);
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(_mode == OFF){ cout<<"MERGE::EXCHANGE:: few active vertices. Use TradGP in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }	
	else{ cout<<"MERGE::EXCHANGE:: too many active vertices. Use ACTS in next iteration. [maxloadcount_vs: "<<maxloadcount_vs<<", maxnumactvvs_vs: "<<maxnumactvvs_vs<<"]"<<endl; }
	#endif 
	#endif 
	
	unsigned int k1; unsigned int k2; unsigned int k3; unsigned int num_acvvs2;
	#ifdef CONFIG_COLLECT_DATAS1_DURING_RUN
	k1 = UTILS_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k2 = UTILS_GetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k3 = UTILS_GetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	num_acvvs2 = k1 + k2+ k3;
	UTILS_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1, num_acvvs2);
		#ifdef _DEBUGMODE_STATS
		cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: [@ 0][num processed edges stats @ processedges.cpp] (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs2<< "(k1: "<<k1<<", k2: "<<k2<<", k3: "<<k3<<")" << RESET << endl;
		#endif 
		
	k1=0; k2=0; k3=0;
	k1 = UTILS_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k2 = UTILS_GetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k3 = UTILS_GetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	num_acvvs2 = k1 + k2+ k3;
	UTILS_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1, num_acvvs2);
	#ifdef _DEBUGMODE_STATS
	cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: [@ 32][num reduced vertex-update stats @ processedges.cpp] (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs2<< "(k1: "<<k1<<", k2: "<<k2<<", k3: "<<k3<<")" << RESET << endl;
	#endif
	#endif 
	
	#ifdef _DEBUGMODE_STATS
	k1=0; k2=0; k3=0;
	k1 = UTILS_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k2 = UTILS_GetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k3 = UTILS_GetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	num_acvvs2 = k1 + k2+ k3;
	UTILS_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1, num_acvvs2);
	#ifdef _DEBUGMODE_STATS
	cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: [@ 64][num processed edges stats @ processedges.cpp->actsutility.cpp] (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs2<< "(k1: "<<k1<<", k2: "<<k2<<", k3: "<<k3<<")" << RESET << endl;
	#endif 
		
	k1=0; k2=0; k3=0;
	k1 = UTILS_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k2 = UTILS_GetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	k3 = UTILS_GetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1); 
	num_acvvs2 = k1 + k2+ k3;
	UTILS_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID - 1, num_acvvs2);
	#ifdef _DEBUGMODE_STATS
	cout<< TIMINGRESULTSCOLOR << ">>> MERGE::EXCHANGE:: [@ 96][num active vertices for next iteration @ mem_access.cpp] (iteration "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<"): "<<num_acvvs2<< "(k1: "<<k1<<", k2: "<<k2<<", k3: "<<k3<<")" << RESET << endl;
	#endif
	#endif 
	// exit(EXIT_SUCCESS); //
	return;
}

extern "C" {
void TOPS_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
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
	globalparams_t globalparamsvA = UTILS_getglobalparams(vdramA, 0);
	unsigned int inACTSMODE = UTILS_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"~~~ TOPS_topkernelS:: GraphIter: "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<endl;
	#endif
	
	#ifdef CONFIG_RELEASE_VERSION4
	MERGES_exchange(vdramA, vdramB, vdramC, mdram);
	#endif 
	return;
}
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_ACTS
void ACTSS_ACTSactspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	if(enable1 == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }
	
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	
	keyvalue_buffer_t kvA0[4];
	keyvalue_buffer_t kvA2[4];
	keyvalue_buffer_t kvA4[4];
	keyvalue_buffer_t kvA6[4];
	#pragma HLS ARRAY_PARTITION variable=kvA0 complete
	#pragma HLS ARRAY_PARTITION variable=kvA2 complete
	#pragma HLS ARRAY_PARTITION variable=kvA4 complete
	#pragma HLS ARRAY_PARTITION variable=kvA6 complete

	buffer_type tempcutoffs[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=tempcutoffs complete

	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = capsule_so1[0][p].value + capsule_so1[1][p].value 
									+ capsule_so1[2][p].value + capsule_so1[3][p].value 
									+ capsule_so1[4][p].value + capsule_so1[5][p].value 
									+ capsule_so1[6][p].value + capsule_so1[7][p].value;
	}
	UTILS_calculateoffsets(capsule_so8, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	UTILS_resetvalues(capsule_so8, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
	
	buffer_type maxcutoff = 0;
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS PIPELINE II=1
		if(cutoffs[v] > maxcutoff){ maxcutoff = cutoffs[v]; }
	}
	#ifdef _DEBUGMODE_CHECKS2 // CRITICAL FIXME.
	#ifndef ALLVERTEXISACTIVE_ALGORITHM
	if(maxcutoff % 4 != 0){ for(int v=0; v<VECTOR_SIZE; v++){ cout<<"------------------- actspipeline:cutoffs["<<v<<"]: "<<cutoffs[v]<<endl; } exit(EXIT_FAILURE); }
	actsutilityobj->checkfordivisibleby(ON, "actspipeline:maxcutoff", maxcutoff, 4);
	#endif
	#endif
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	if(maxcutoff % 4 != 0){ maxcutoff = 0; } // 4, CRITICAL FIXME.
	#endif 
	// cout<<"------------------- ACTSS_actspipeline: maxcutoff: "<<maxcutoff<<endl;
	
	keyvalue_t mydummykv;
	mydummykv.key = 0;
	mydummykv.value = UTILS_GETV(INVALIDDATA);
	keyvalue_buffer_t dummykv = UTILS_GETKV(mydummykv);
	
	value_t tempbufferDcapsule[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=tempbufferDcapsule complete

	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		tempbufferDcapsule[p] = capsule_so8[p].value; 
	}
	for(vector_type v=0; v<VECTOR_SIZE; v++){ 
	#pragma HLS PIPELINE II=1
		tempcutoffs[v] = cutoffs[v]; 
	}
	
	unsigned int n=0;
	RUNPIPELINE_LOOP1: for(n=0; n<2; n++){
		RUNPIPELINE_LOOP1B: for(buffer_type k=0; k<maxcutoff; k+=4){
		#pragma HLS PIPELINE II=4
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(k % 16 == 0){ cout<<"k: "<<k<<endl; }
			#endif
			
			/// LOADING FROM A
			if(k < tempcutoffs[0+n]){ kvA0[0] = buffer_setof1[0+n][k]; kvA0[1] = buffer_setof1[0+n][k+1]; kvA0[2] = buffer_setof1[0+n][k+2]; kvA0[3] = buffer_setof1[0+n][k+3]; } 
			else { kvA0[0] = dummykv; kvA0[1] = dummykv; kvA0[2] = dummykv; kvA0[3] = dummykv; }
			if(k < tempcutoffs[2+n]){ kvA2[0] = buffer_setof1[2+n][k]; kvA2[1] = buffer_setof1[2+n][k+1]; kvA2[2] = buffer_setof1[2+n][k+2]; kvA2[3] = buffer_setof1[2+n][k+3]; } 
			else { kvA2[0] = dummykv; kvA2[1] = dummykv; kvA2[2] = dummykv; kvA2[3] = dummykv; }
			if(k < tempcutoffs[4+n]){ kvA4[0] = buffer_setof1[4+n][k]; kvA4[1] = buffer_setof1[4+n][k+1]; kvA4[2] = buffer_setof1[4+n][k+2]; kvA4[3] = buffer_setof1[4+n][k+3]; } 
			else { kvA4[0] = dummykv; kvA4[1] = dummykv; kvA4[2] = dummykv; kvA4[3] = dummykv; }
			if(k < tempcutoffs[6+n]){ kvA6[0] = buffer_setof1[6+n][k]; kvA6[1] = buffer_setof1[6+n][k+1]; kvA6[2] = buffer_setof1[6+n][k+2]; kvA6[3] = buffer_setof1[6+n][k+3]; } 
			else { kvA6[0] = dummykv; kvA6[1] = dummykv; kvA6[2] = dummykv; kvA6[3] = dummykv; }
		
			#ifdef _DEBUGMODE_KERNELPRINTS
			for(unsigned int m=0; m<4; m++){ cout<<"kvA0["<<m<<"].key: "<<UTILS_GETKV(kvA0[m]).key<<", kvA0["<<m<<"].value: "<<UTILS_GETKV(kvA0[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA2["<<m<<"].key: "<<UTILS_GETKV(kvA2[m]).key<<", kvA2["<<m<<"].value: "<<UTILS_GETKV(kvA2[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA4["<<m<<"].key: "<<UTILS_GETKV(kvA4[m]).key<<", kvA4["<<m<<"].value: "<<UTILS_GETKV(kvA4[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA6["<<m<<"].key: "<<UTILS_GETKV(kvA6[m]).key<<", kvA6["<<m<<"].value: "<<UTILS_GETKV(kvA6[m]).value<<endl; }
			#endif
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkn(enablebufferA, "kvA0", kvA0, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA2", kvA2, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA4", kvA4, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA6", kvA6, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			#endif
			
			partition_type pA0 = UTILS_getpartition(ON, mode, kvA0[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA2 = UTILS_getpartition(ON, mode, kvA2[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA4 = UTILS_getpartition(ON, mode, kvA4[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA6 = UTILS_getpartition(ON, mode, kvA6[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			
			/// LOADING FROM AND INTO D
			buffer_type posD0 = capsule_so8[pA0].key + tempbufferDcapsule[pA0];
			unsigned int yoffset0 = posD0 / 8;
			unsigned int xoffset0 = posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "posD0", posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "posD0", posD0, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset0", yoffset0, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset0+4", xoffset0+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset0 == 0){
				buffer_setof8[0][yoffset0] = kvA0[0]; buffer_setof8[1][yoffset0] = kvA0[1]; buffer_setof8[2][yoffset0] = kvA0[2]; buffer_setof8[3][yoffset0] = kvA0[3]; 
			} else {
				buffer_setof8[4][yoffset0] = kvA0[0]; buffer_setof8[5][yoffset0] = kvA0[1]; buffer_setof8[6][yoffset0] = kvA0[2]; buffer_setof8[7][yoffset0] = kvA0[3]; 
			}
			if(UTILS_GETKV(kvA0[0]).value != UTILS_GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; }
			// if(UTILS_GETKV(kvA0[0]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA0[1]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA0[2]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA0[3]).value != UTILS_GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; }		
			
			buffer_type _posD0 = capsule_so8[pA2].key + tempbufferDcapsule[pA2];
			unsigned int yoffset1 = _posD0 / 8;
			unsigned int xoffset1 = _posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "_posD0", _posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "_posD0", _posD0, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset1", yoffset1, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset1+4", xoffset1+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset1 == 0){
				buffer_setof8[0][yoffset1] = kvA2[0]; buffer_setof8[1][yoffset1] = kvA2[1]; buffer_setof8[2][yoffset1] = kvA2[2]; buffer_setof8[3][yoffset1] = kvA2[3]; 
			} else {
				buffer_setof8[4][yoffset1] = kvA2[0]; buffer_setof8[5][yoffset1] = kvA2[1]; buffer_setof8[6][yoffset1] = kvA2[2]; buffer_setof8[7][yoffset1] = kvA2[3]; 
			}
			if(UTILS_GETKV(kvA2[0]).value != UTILS_GETV(INVALIDDATA)){ tempbufferDcapsule[pA2] += 4; }
			// if(UTILS_GETKV(kvA2[0]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA2[1]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA2[2]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA2[3]).value != UTILS_GETV(INVALIDDATA)){ tempbufferDcapsule[pA2] += 4; }
			
			buffer_type __posD0 = capsule_so8[pA4].key + tempbufferDcapsule[pA4];
			unsigned int yoffset2 = __posD0 / 8;
			unsigned int xoffset2 = __posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "__posD0", __posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "__posD0", __posD0, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset2", yoffset2, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset2+4", xoffset2+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset2 == 0){
				buffer_setof8[0][yoffset2] = kvA4[0]; buffer_setof8[1][yoffset2] = kvA4[1]; buffer_setof8[2][yoffset2] = kvA4[2]; buffer_setof8[3][yoffset2] = kvA4[3]; 
			} else {
				buffer_setof8[4][yoffset2] = kvA4[0]; buffer_setof8[5][yoffset2] = kvA4[1]; buffer_setof8[6][yoffset2] = kvA4[2]; buffer_setof8[7][yoffset2] = kvA4[3]; 
			}
			if(UTILS_GETKV(kvA4[0]).value != UTILS_GETV(INVALIDDATA)){ tempbufferDcapsule[pA4] += 4; }
			// if(UTILS_GETKV(kvA4[1]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA4[1]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA4[2]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA4[3]).value != UTILS_GETV(INVALIDDATA)){ tempbufferDcapsule[pA4] += 4; }
			
			buffer_type ___posD0 = capsule_so8[pA6].key + tempbufferDcapsule[pA6];
			unsigned int yoffset3 = ___posD0 / 8;
			unsigned int xoffset3 = ___posD0 % 8;
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkfordivisibleby(enablebufferD, "___posD0", ___posD0, 4);
			actsutilityobj->checkoutofbounds(enablebufferD, "___posD0", ___posD0, MAX_DESTBLOCKRAM_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "yoffset3", yoffset3, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds(enablebufferD, "xoffset3+4", xoffset3+4, VECTOR_SIZE+1, NAp, NAp, NAp);
			#endif 
			if(xoffset3 == 0){
				buffer_setof8[0][yoffset3] = kvA6[0]; buffer_setof8[1][yoffset3] = kvA6[1]; buffer_setof8[2][yoffset3] = kvA6[2]; buffer_setof8[3][yoffset3] = kvA6[3]; 
			} else {
				buffer_setof8[4][yoffset3] = kvA6[0]; buffer_setof8[5][yoffset3] = kvA6[1]; buffer_setof8[6][yoffset3] = kvA6[2]; buffer_setof8[7][yoffset3] = kvA6[3]; 
			}
			if(UTILS_GETKV(kvA6[0]).value != UTILS_GETV(INVALIDDATA)){ tempbufferDcapsule[pA6] += 4; }
			// if(UTILS_GETKV(kvA6[0]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA6[1]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA6[2]).value != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kvA6[3]).value != UTILS_GETV(INVALIDDATA)){ tempbufferDcapsule[pA6] += 4; }
		}
	}
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		capsule_so8[p].value = tempbufferDcapsule[p]; 
	}
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = UTILS_GETV(INVALIDDATA);
		keyvalue_buffer_t dummykv = UTILS_GETKV(mydummykv);
		if(capsule_so8[p].value % 8 == 4){
			unsigned int yoffset = (capsule_so8[p].key + capsule_so8[p].value) / VECTOR_SIZE;
			buffer_setof8[4][yoffset] = dummykv; buffer_setof8[5][yoffset] = dummykv; buffer_setof8[6][yoffset] = dummykv; buffer_setof8[7][yoffset] = dummykv; 
			capsule_so8[p].value += 4;
		}
		#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
		actsutilityobj->checkfordivisibleby(enablebufferD, "capsule_so8[p].value", capsule_so8[p].value, 8);
		#endif
	}
	return;
}

void ACTSS_TRADactspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){
	if(enable1 == OFF && enable2 == OFF){ return; }
	if(globalposition.num_active_vertices < globalparams.THRESHOLD_HYBRIDGPMODE_MAXVTHRESHOLD_PER_VPARTITION){ return; }

	buffer_type chunk_size = cutoffs[0];
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	unsigned int num_partitions = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	
	if(globalparams.EVALUATION_ACTS_VERYBARE == ON){ return; }
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ num_partitions = UTILS_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
	#endif 
	
	for(partition_type p=0; p<num_partitions; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0; 
	}
	
	if(chunk_size >= BLOCKRAM_SIZE){ chunk_size = globalparams.ACTSPARAMS_WORKBUFFER_SIZE; } // BLOCKRAM_SIZE, globalparams.ACTSPARAMS_WORKBUFFER_SIZE
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("TRADactspipeline (22)", chunk_size, BLOCKRAM_SIZE, cutoffs[0], NAp, NAp);
	#endif
	
	TRADITIONALPARTITIONKEYVALUES_LOOP: for(buffer_type i=0; i<chunk_size; i++){
		TRADITIONALPARTITIONKEYVALUES_LOOPB: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=2
			keyvalue_buffer_t kv = buffer_setof1[v][i];
			
			partition_type p = UTILS_getpartition2(ON, mode, kv, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams);
			if(UTILS_GETKV(kv).key != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kv).value != UTILS_GETV(INVALIDDATA)){ capsule_so8[p].value += 1; }
		}
	}
	
	UTILS_calculateoffsets(capsule_so8, num_partitions);
	UTILS_resetvalues(capsule_so8, num_partitions, 0);
	
	TRADITIONALPARTITIONKEYVALUES_LOOP2: for(buffer_type i=0; i<chunk_size; i++){
		TRADITIONALPARTITIONKEYVALUES_LOOP2B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=2
			keyvalue_buffer_t kv = buffer_setof1[v][i];
			// cout<<"--- priorpartitionkeyvalues: kv.key: "<<kv.key<<endl; // REMOVEME.
			partition_type p = UTILS_getpartition2(ON, mode, kv, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams);
			buffer_type pos = capsule_so8[p].key + capsule_so8[p].value;
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("TRADactspipeline (23)", pos / VECTOR_SIZE, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(UTILS_GETKV(kv).key != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kv).value != UTILS_GETV(INVALIDDATA)){ buffer_setof8[pos % VECTOR_SIZE][pos / VECTOR_SIZE] = kv; } // NOTE: could this be the cause of slight imperfection in results?
			if(UTILS_GETKV(kv).key != UTILS_GETV(INVALIDDATA) && UTILS_GETKV(kv).value != UTILS_GETV(INVALIDDATA)){ capsule_so8[p].value += 1; }
		}
	}
	
	TRADITIONALPARTITIONKEYVALUES_LOOP3: for(partition_type p=0; p<num_partitions; p++){
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = UTILS_GETV(INVALIDDATA);
		keyvalue_buffer_t dummykv = UTILS_GETKV(mydummykv);
	
		unsigned int endoffset = capsule_so8[p].key + capsule_so8[p].value;
		unsigned int xpos = endoffset % VECTOR_SIZE;
		unsigned int ypos = endoffset / VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("TRADactspipeline (24)", ypos, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(capsule_so8[p].value > 0){
			TRADITIONALPARTITIONKEYVALUES_LOOP3B: for(vector_type v=xpos; v<VECTOR_SIZE; v++){
			#pragma HLS PIPELINE II=2
				buffer_setof8[v][ypos] = dummykv;
				capsule_so8[p].value += 1;
			}
		}
	}
	
	// cout<<"actit:: MAX_NUM_PARTITIONS: "<<MAX_NUM_PARTITIONS<<", num_partitions: "<<num_partitions<<", globalparams.ACTSPARAMS_TREEDEPTH: "<<globalparams.ACTSPARAMS_TREEDEPTH<<endl;
	// actsutilityobj->printkeyvalues("------------- actit::capsule_so8 37--", (keyvalue_t *)capsule_so8, num_partitions); 
	// exit(EXIT_SUCCESS);
	return;
}

void ACTSS_actspipeline(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){
	ACTSS_ACTSactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);		
	/** #ifdef CONFIG_ALL_EVALUATIONTYPES_IN_ONE_KERNEL
	if((globalparams.EVALUATION_ACTS_PARTITIONINGLOGIC == OFF)
			|| (globalparams.EVALUATION_ACTS_PARTITIONINGLOGIC == ON && globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 2 && currentLOP == 1)){
			ACTSS_TRADactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, 
				buffer_setof8, capsule_so8,
					currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
	} else {
		ACTSS_ACTSactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, 
				buffer_setof8, capsule_so8,
					currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
	}	
	#else 
		#ifdef CONFIG_ACTS_PARTITIONINGLOGIC
		ACTSS_ACTSactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
		#else 
		ACTSS_TRADactspipeline(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
		#endif
	#endif */
}
										
fetchmessage_t ACTSS_fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE],				 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams,
		unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	if(mode == ACTSPROCESSMODE){
		fetchmessage = PROCESSS_readandprocess(enable, kvdram, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate, sweepparams, globalposition, globalparams, collections);
	} else {
		fetchmessage = MEMACCESSS_readkeyvalues(enable, kvdram, buffer, goffset_kvs + loffset_kvs, size_kvs, travstate, globalparams); 
	}
	return fetchmessage; 
}

void ACTSS_commitkeyvalues(bool_type enable, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE],
		keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, buffer_type chunk_size, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(mode == ACTSREDUCEMODE || (globalparams.ACTSPARAMS_TREEDEPTH == 1 && globalparams.ENABLE_APPLYUPDATESCOMMAND == ON)){
		REDUCES_reduceandbuffer(enable, destbuffer, localcapsule, vbuffer, vbuffer2, sweepparams, globalposition, globalparams, collections);
		if(globalparams.ACTSPARAMS_TREEDEPTH == 1){ for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }} // NEWCHANGE.
	
	
		/* ///////////////// REMOVEME.
		MEMACCESSS_saveV(ON, kvdram, vbuffer, globalparams.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparams.BUFFERBASEOFFSETKVS_VDATA1, globalparams.SIZEKVS2_REDUCEPARTITION, globalposition, globalparams); 
		// exit();
		//////////////// REMOVEME. */
	
	
	
	} else {
		MEMACCESSS_savekeyvalues(enable, kvdram, destbuffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalposition, globalparams); 
	}
	return;
} 

void ACTSS_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(enable == OFF){ return; }
	
keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE]; // REMOVEME.-SWX
	#pragma HLS array_partition variable = buffer_setof1
keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = buffer_setof8
	
keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS];
	#pragma HLS array_partition variable = capsule_so1

keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS];
	
	travstate_t ptravstatepp0 = ptravstate;
	travstate_t ptravstatepp1 = ptravstate;
	
	bool_type pp0readen = ON;
	bool_type pp1readen = ON;
	bool_type pp0runpipelineen = ON;
	bool_type pp1runpipelineen = ON;
	bool_type pp0partitionen = ON;
	bool_type pp1partitionen = ON;
	bool_type pp0writeen = ON;
	bool_type pp1writeen = ON;
buffer_type pp0cutoffs[VECTOR_SIZE];
buffer_type pp1cutoffs[VECTOR_SIZE];
	batch_type itercount = 0;
	batch_type flushsz = 0;
	
	fetchmessage_t fetchmessagepp0;
	fetchmessage_t fetchmessagepp1;
	fetchmessagepp0.chunksize_kvs = -1; fetchmessagepp0.nextoffset_kvs = -1;
	fetchmessagepp1.chunksize_kvs = -1; fetchmessagepp1.nextoffset_kvs = -1;
	
	if(resetenv == ON){ UTILS_resetenvbuffers(capsule_so1, capsule_so8); } else { itercount = 64; } // FIXME. '64' is just some number greater than 2
	if(flush == ON){ flushsz = 1*MAX_SRCBUFFER_SIZE; } else { flushsz = 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(resetenv == ON){ cout<<"actit: reset is ON"<<endl; } else { cout<<"actit: reset is OFF"<<endl;  }
	if(flush == ON){ cout<<"actit: flush is ON"<<endl; } else { cout<<"actit: flush is OFF"<<endl;  }
	#endif 
	
	batch_type offset_kvs = ptravstate.begin_kvs;
	unsigned int workbuffer_size = globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	if(globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ if(mode == ACTSPROCESSMODE){ workbuffer_size += 2; }} // inclusion of metadata else { }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit: "<<((ptravstate.end_kvs + flushsz) - ptravstate.begin_kvs) / workbuffer_size * NUMPIPELINES_PARTITIONUPDATES<<" states of acts pipeline processing..."<<endl; 
	#endif 
	// actsutilityobj->printkeyvalues("actit::globalstatsbuffer 37--", (keyvalue_t *)globalstatsbuffer, NUM_PARTITIONS); 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"actit: processing all chunks [begin_kvs: "<<ptravstate.begin_kvs<<"][end_kvs: "<<ptravstate.end_kvs<<"][flushsz: "<<flushsz<<"][size_kvs: "<<ptravstate.begin_kvs - (ptravstate.end_kvs + flushsz)<<"][workbuffer_size: "<<workbuffer_size<<"][num_chunks: "<<(ptravstate.begin_kvs - (ptravstate.end_kvs + flushsz)) / workbuffer_size<<"] ... "<<endl;
	#endif
		
	ACTIT_MAINLOOP: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs + flushsz; offset_kvs+=workbuffer_size * NUMPIPELINES_PARTITIONUPDATES){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts::ACTSS_actit: processing chunk [offset_kvs: "<<offset_kvs<<"]: [ptravstate.begin_kvs: "<<ptravstate.begin_kvs<<"]: [ptravstate.end_kvs + flushsz: "<<ptravstate.end_kvs + flushsz<<"] ... "<<endl;
		#endif
		#ifdef PUP1
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 1){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 1){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		ptravstatepp0.i_kvs = offset_kvs;
		fetchmessagepp0 = ACTSS_fetchkeyvalues(ON, mode,  kvdram, vbuffer_source, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp0.i_kvs, workbuffer_size, ptravstatepp0, sweepparams, globalposition, globalparams, edgebankID, collections);				
		#ifdef PUP1
		ACTSS_actspipeline(pp1runpipelineen, ON, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, globalposition, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		PARTITIONS_preparekeyvalues(ON, ON, mode, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, globalposition, fetchmessagepp0.chunksize_kvs, pp0cutoffs, globalparams);
		#ifdef PUP1
		ACTSS_commitkeyvalues(pp1writeen, ON, mode, kvdram, vbuffer_dest, vbuffer2, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, fetchmessagepp1.chunksize_kvs, sweepparams, globalposition, globalparams, collections); 
		#endif 
		
		ACTSS_actspipeline(ON, ON, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, globalposition, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		ptravstatepp1.i_kvs = offset_kvs + workbuffer_size;
		fetchmessagepp1 = ACTSS_fetchkeyvalues(ON, mode,  kvdram, vbuffer_source, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp1.i_kvs, workbuffer_size, ptravstatepp1, sweepparams, globalposition, globalparams, edgebankID, collections);
		#endif
		
		ACTSS_commitkeyvalues(pp0writeen, ON, mode, kvdram, vbuffer_dest, vbuffer2, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, fetchmessagepp0.chunksize_kvs, sweepparams, globalposition, globalparams, collections); 
		#ifdef PUP1
		PARTITIONS_preparekeyvalues(pp1partitionen, ON, mode, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, globalposition, fetchmessagepp1.chunksize_kvs, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMPIPELINES_PARTITIONUPDATES;
		// exit(EXIT_SUCCESS);
	}
	// exit(EXIT_SUCCESS); //////////////////////////
	return;
}

	
#endif 			
#if defined(CONFIG_ACTSONLYMODE) || defined(CONFIG_HYBRIDGPMODE)
void TOPS_processit(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	for(buffer_type t=0; t<globalparamsK.SIZEKVS2_REDUCEPARTITION; t++){
		for(buffer_type v=0; v<VDATA_PACKINGSIZE; v++){
			if(MEMCAS_READVMASK(vbuffer_source[v][t]) == 1){ cout<<"TOPS_processit: ACTIVE MASK SEEN: @ t: "<<t<<", v: "<<v<<", vbuffer_source["<<v<<"]["<<t<<"]: "<<vbuffer_source[v][t]<<endl; }
		}
	}
	#endif

	sweepparams_t sweepparams;
	if(globalparamsK.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ globalposition.deststatsmarker = MERGES_actvpstatsoffset(globalparamsK); }
	
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILS_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0); } 
	sweepparams = UTILS_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	
	if(globalposition.source_partition == globalposition.first_source_partition){ MEMACCESSS_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL NEWCHANGE.
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILS_resetvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0); } // CRITICAL NEWCHANGE.

	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); //. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print7("### TOPS_processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif

	batch_type voffset_kvs = globalposition.source_partition * UTILS_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	bool returncmd = false;
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	if(globalposition.num_active_vertices == 0){ returncmd = true; } // check if vertex partition is active
	else{
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; 
		#endif 
	}
	#endif 
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ returncmd = false; } // for certain algorithms like pagerank
	if(voffset_kvs >= avtravstate.end_kvs){ returncmd = true; } // check if we are at the end of file
	
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && globalposition.stage == 0 && globalposition.source_partition == globalposition.first_source_partition){
		// MEMACCESSS_readV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, globalposition.source_partition * globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
		MEMACCESSS_readV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
	}
	
	if(returncmd == false){
		sweepparams.source_partition = globalposition.source_partition;
		avtravstate.i_kvs = voffset_kvs;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print5("### TOPS_processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, MAX_SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + (UTILS_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + (UTILS_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
			
		unsigned int SKIP_KVS = UTILS_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
		
		tuple_t tup = MEMACCESSS_getvptrs_opt(kvdram, vptrbaseoffset_kvs, voffset_kvs * VECTOR2_SIZE, (voffset_kvs + SKIP_KVS) * VECTOR2_SIZE, edgebankID, globalparamsK);
		keyy_t beginvptr = tup.A;
		keyy_t endvptr = tup.B; 
		#ifdef _DEBUGMODE_CHECKS3
		if(globalparamsK.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ if(beginvptr % VECTOR2_SIZE != 0 || endvptr % VECTOR2_SIZE != 0){ cout<<"TOPS_processit::ERROR: beginvptr("<<beginvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0 || endvptr("<<endvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }}
		#endif 
		
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr;
		keyy_t localendvptr = endvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 1*VECTOR_SIZE; // CRITICAL NEWCHANGE.
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN // FIXME. REMOVEME WHEN IN RUNNING MODE.
		if(sweepparams.source_partition < 64){
			UTILS_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + sweepparams.source_partition, localbeginvptr_kvs);
			UTILS_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + sweepparams.source_partition, numedges); 
		}
		collections[0][0].data1 += numedges;
		#endif 
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ collections[0][1].data1 += numedges; }
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS3
		if(localendvptr < localbeginvptr){ cout<<"TOPS_processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<", voffset_kvs: "<<voffset_kvs<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("TOPS_processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"[index: "<<globalposition.source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"]"<<endl;
		#endif
		#ifdef _DEBUGMODE_CHECKS3
		if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->clearallstats();
		#endif
		
		travstate_t etravstate;
		etravstate.begin_kvs = localbeginvptr_kvs;
		etravstate.size_kvs = numedges_kvs;
		etravstate.end_kvs = etravstate.begin_kvs + etravstate.size_kvs;
		etravstate.skip_kvs = MAX_SRCBUFFER_SIZE;
		etravstate.i_kvs = etravstate.begin_kvs;
		etravstate.i2 = voffset_kvs * VECTOR2_SIZE;
		etravstate.i2_kvs = voffset_kvs;
		bool_type resetenv = ON;
		bool_type flush = ON;
		
		if(voffset_kvs == avtravstate.begin_kvs || globalposition.source_partition == globalposition.first_source_partition){ resetenv = ON; } else { resetenv = OFF; }
		if((voffset_kvs + UTILS_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		// if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ 
		resetenv = ON; flush = ON; 
		// } // CRITICAL NEWCHANGE.
		
		ACTSS_actit(
			ON, ACTSPROCESSMODE,
			kvdram, sourcebuffer, vbuffer_source, vbuffer_dest, vbuffer2_dest, globalstatsbuffer,
			globalparamsK, globalposition, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE,
			resetenv, flush, edgebankID, collections);
			
		/* ///////////////// REMOVEME.
		MEMACCESSS_saveV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, globalposition.source_partition * globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
		//////////////// REMOVEME. */
	}
	
	bool endofrun = false;
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ endofrun = true; }
	
	if(endofrun == true){ MEMACCESSS_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL OPTIMIZEME. should be called only once
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPS_processit", globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS); }
	#endif	
	
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && endofrun == true){ 
		// MEMACCESSS_saveV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, globalposition.source_partition * globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
		MEMACCESSS_saveV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][1].data1);
		#endif
	}
	
	#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
	if(endofrun == true){ UTILS_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][0].data1); }
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

void TOPS_partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], globalparams_t globalparams, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ return; } ///
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nusrcv_nudstv::UTILS_getglobalparams:: printing global parameters", globalparams);
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; 
	globalposition_t globalposition; // DUMMY.
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + globalparams.ACTSPARAMS_NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparams.ACTSPARAMS_BEGINLOP + 1 + (globalparams.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = UTILS_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"TOPS_partitionit: source_partition "<<source_partition<<" ... "<<endl;

			UTILS_resetkeysandvalues(globalstatsbuffer, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
		
			sweepparams = UTILS_getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t ptravstate = UTILS_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
			
			// collect stats
			MEMACCESSS_readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
			UTILS_resetvalues(globalstatsbuffer, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### TOPS_partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			UTILS_resetvalues(globalstatsbuffer, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			ACTSS_actit(config.enablepartition, ACTSPARTITIONMODE,
					kvdram, sourcebuffer, vbuffer_source, vbuffer_dest, vbuffer2_dest, globalstatsbuffer, // CRITICAL FIXME.
					globalparams, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp, collections);
					
			MEMACCESSS_saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += globalparams.ACTSPARAMS_NUM_PARTITIONS;
			}
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_STATS
			if(config.enablereduce == ON){ edges_count = actsutilityobj->globalstats_getcountvalidkvsreduced(globalparams.ACTSPARAMS_INSTID); edgesdstv_sum = actsutilityobj->globalstats_getreducevar1(); }
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printglobalvars();
			#endif
			#ifdef _DEBUGMODE_STATS_XXX
			actsutilityobj->clearglobalvars();
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		#endif 
		#ifdef _DEBUGMODE_STATS_XXX
		actsutilityobj->clearglobalvars();
		#endif
	}
	return;
}

void TOPS_reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; 
	globalposition_t globalposition; // DUMMY.
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	
	sweepparams = UTILS_getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = UTILS_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
	UTILS_resetkeysandvalues(globalstatsbuffer, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### TOPS_reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	ACTSS_actit(config.enablereduce, ACTSREDUCEMODE,
			 kvdram, sourcebuffer, vbuffer_source, vbuffer_dest, vbuffer2_dest, globalstatsbuffer, // CRITICAL FIXME.
			globalparams, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp, collections);
	return;
}

void TOPS_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(en_process == ON){ TOPS_processit(kvdram, sourcebuffer, vbuffer_source, vbuffer_dest, vbuffer2_dest, globalstatsbuffer, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(en_partition == ON){ TOPS_partitionit( kvdram, sourcebuffer, vbuffer_source, vbuffer_dest, vbuffer2_dest, globalparamsK, NAp, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(en_reduce == ON){ TOPS_reduceit(kvdram, sourcebuffer, vbuffer_source, vbuffer_dest, vbuffer2_dest, sourcestatsmarker, source_partition, globalparamsK, NAp, collections); } 
	#endif 
	return;
} 

void TOPS_dispatch_reduce(uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t vbuffer2_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); }
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int vmask_offset_kvs = 0;
	unsigned int vmaskp_offset_kvs = 0;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = UTILS_get_num_source_partitions(currentLOP);

	bool_type enablereduce = ON;
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; // NOT USED.
	
	UTILS_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
	
	DISPATCHREDUCES_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### TOPS_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "vreadoffset_kvs2", source_partition, currentLOP, num_source_partitions, vreadoffset_kvs2); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTILS_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		globalstatsbuffer[source_partition].key = NAp; globalstatsbuffer[source_partition].value = rtravstate.size_kvs;
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		collections[0][1].data1 += rtravstate.size_kvs * VECTOR_SIZE;
		#endif 
		
		// read vertices
		MEMACCESSS_readV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID == CF){ MEMACCESSS_copyV(enablereduce, vbuffer_dest, vbuffer2_dest, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.BUFFERBASEOFFSETKVS_VDATA2, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK); } // CUSTOMPROVISION
		
		// reduce
		TOPS_dispatch(OFF, OFF, enablereduce, kvdram, sourcebuffer, vbuffer_source, vbuffer_dest, vbuffer2_dest, globalstatsbuffer, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);
		
		// writeback vertices
		MEMACCESSS_saveV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		vmaskp_offset_kvs += NUM_PEs;
	}
	
	#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN // NEWCHANGE
	UTILS_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][1].data1);
	#endif 
	// exit(EXIT_SUCCESS);
	return;
} 

void TOPS_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){									

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	unsigned int printheader1=OFF; // ON, OFF*
	unsigned int printheader2=OFF; 
	if(true && globalposition.source_partition==globalposition.first_source_partition){ printheader1 = ON; } else { printheader1 = OFF; }
	if(false && globalposition.source_partition==globalposition.last_source_partition){ printheader2 = ON; } else { printheader2 = OFF; }
	// printheader1=ON;
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== ACTS Launched... size: "<<UTILS_GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl;
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	unsigned int PARTITIONS_CHKPT[EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
	keyvalue_vbuffer_t vbuffer2_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDEST2DATA_SIZE]; // CUSTOMPROVISION.
	#pragma HLS array_partition variable = vbuffer2_dest	
	#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
	keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_dest	
	#ifdef CONFIG_USEURAM_FOR_DESTVBUFFER
		#pragma HLS resource variable=vbuffer_dest core=XPM_MEMORY uram
		// #pragma HLS bind_storage variable=vbuffer_dest type=RAM_T2P impl=uram // REMOVEME?
		// #pragma HLS array_reshape variable=vbuffer_dest type=block factor=2
	#endif 
	#endif 
	
	globalparams_t globalparamsK;
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	globalparams_t _globalparamsE;
	
	globalparamsK = UTILS_getglobalparams(kvdram, 0); // CRITICAL OPTIMIZEME. MOVETOBASE?
	
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){ 
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram, e+1); }} 
	else { globalparamsEs[0] = globalparamsK; }
	_globalparamsE = globalparamsEs[globalposition.edgebankID];
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE. IS THIS OPTIMIZED
	}
	
	globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = GraphIter; // overrride these with GraphIter
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e].ALGORITHMINFO_GRAPHITERATIONID = GraphIter; }
	_globalparamsE.ALGORITHMINFO_GRAPHITERATIONID = GraphIter;
	
	#ifdef CONFIG_SEPERATESRCVDESTVBUFFER // reset vbuffer
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		for (buffer_type i=0; i<MAX_BLOCKRAM_VDESTDATA_SIZE; i++){ // reset vbuffer_source 
			vbuffer_dest[0][i] = 0;
			vbuffer_dest[1][i] = 0;
			vbuffer_dest[2][i] = 0;
			vbuffer_dest[3][i] = 0;
			vbuffer_dest[4][i] = 0;
			vbuffer_dest[5][i] = 0;
			vbuffer_dest[6][i] = 0;
			vbuffer_dest[7][i] = 0;
			vbuffer_dest[8][i] = 0;
			vbuffer_dest[9][i] = 0;
			vbuffer_dest[10][i] = 0;
			vbuffer_dest[11][i] = 0;
			vbuffer_dest[12][i] = 0;
			vbuffer_dest[13][i] = 0;
			vbuffer_dest[14][i] = 0;
			vbuffer_dest[15][i] = 0;
		}
	}
	#endif 
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ UTILS_resetkvstatvalues(kvdram, globalparamsK); } // NEWCHANGE
	#endif
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPS_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPS_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, sourcebuffer, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
				vbuffer_dest, 
					#else 
						vbuffer_source,	
							#endif 
								vbuffer2_dest, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONS_CHKPT, globalposition.v_chunkid, globalposition.edgebankID, collections); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPS_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPS_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, sourcebuffer, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
			vbuffer_dest, 
				#else 
					vbuffer_source,	
						#endif 
							vbuffer2_dest, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONS_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	// reduce & partition
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPS_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPS_dispatch_reduce(kvdram, sourcebuffer, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
			vbuffer_dest, 
				#else 
					vbuffer_source,	
						#endif 
							vbuffer2_dest, _globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONS_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	bool endofrun = false;
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }} 
	else { if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }}
	#if not defined(FPGA_IMPL) && defined(CONFIG_COLLECT_DATAS3_DURING_RUN)
	if(endofrun == true){
		// cout<<"------------------ top_nusrcv_nudstv: actsutilityobj->globalstats_getcountnumvalidprocessedges("<<globalparamsK.ACTSPARAMS_INSTID<<"): "<<actsutilityobj->globalstats_getcountnumvalidprocessedges(globalparamsK.ACTSPARAMS_INSTID)<<endl;
		// cout<<"++++++++++++++++++ top_nusrcv_nudstv: actsutilityobj->globalstats_getcountvalidkvsreduced("<<globalparamsK.ACTSPARAMS_INSTID<<"): "<<actsutilityobj->globalstats_getcountvalidkvsreduced(globalparamsK.ACTSPARAMS_INSTID)<<endl;
		UTILS_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, actsutilityobj->globalstats_getcountnumvalidprocessedges(globalparamsK.ACTSPARAMS_INSTID));
		UTILS_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, actsutilityobj->globalstats_getcountvalidkvsreduced(globalparamsK.ACTSPARAMS_INSTID));
	}
	#endif 
	if(endofrun == true){
		unsigned int _offset = MERGES_actvpstatsoffset(globalparamsK);
		MEMACCESSS_retreievekvstats(kvdram, globalstatsbuffer, globalparamsK, _offset, globalparamsK.NUM_REDUCEPARTITIONS);
		UTILS_increment_graphiteration(kvdram, globalparamsK); } // NB: this should come last.
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(endofrun == true){ actsutilityobj->printglobalvars2(globalparamsK.ACTSPARAMS_INSTID, "instID", "v_chunkid", "LOP", "sourcep", globalparamsK.ACTSPARAMS_INSTID, globalposition.v_chunkid, globalposition.currentLOP, globalposition.source_partition); }
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void TOPS_topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[1];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL1_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL1_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL1_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL1_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL1_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL1_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks1(ON, vdram, vbuffer_source0, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP1"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[2];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL2_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL2_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL2_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL2_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL2_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL2_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks2(ON, vdram, vbuffer_source0,vbuffer_source1, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP2"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[3];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL3_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL3_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL3_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL3_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL3_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL3_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks3(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP3"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[4];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL4_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL4_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL4_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL4_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL4_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL4_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks4(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP4"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem5

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[5];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL5_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL5_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL5_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL5_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL5_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL5_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks5(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv4 = kvdram4[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
			K = vv4.data[v].key;
			V = vv4.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGES_mergeVs(kvdram4, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP5"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[6];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL6_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL6_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL6_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL6_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL6_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL6_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks6(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv4 = kvdram4[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv5 = kvdram5[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
			K = vv4.data[v].key;
			V = vv4.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }	
			K = vv5.data[v].key;
			V = vv5.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGES_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGES_mergeVs(kvdram5, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP6"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem7

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5

#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[7];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL7_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL7_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL7_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL7_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL7_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL7_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks7(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv4 = kvdram4[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv5 = kvdram5[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv6 = kvdram6[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
			K = vv4.data[v].key;
			V = vv4.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }	
			K = vv5.data[v].key;
			V = vv5.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }	
			K = vv6.data[v].key;
			V = vv6.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGES_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGES_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGES_mergeVs(kvdram6, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP7"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP8(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem8

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5

#pragma HLS DATA_PACK variable = kvdram6

#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[8];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL8_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL8_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL8_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL8_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL8_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL8_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks8(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv4 = kvdram4[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv5 = kvdram5[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv6 = kvdram6[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv7 = kvdram7[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
			K = vv4.data[v].key;
			V = vv4.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }	
			K = vv5.data[v].key;
			V = vv5.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }	
			K = vv6.data[v].key;
			V = vv6.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }	
			K = vv7.data[v].key;
			V = vv7.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGES_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGES_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGES_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGES_mergeVs(kvdram7, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP8"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP9(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem9

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5

#pragma HLS DATA_PACK variable = kvdram6

#pragma HLS DATA_PACK variable = kvdram7

#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source8
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source8 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];	
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[9];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
		vbuffer_source8[0][i] = 0;
		vbuffer_source8[1][i] = 0;
		vbuffer_source8[2][i] = 0;
		vbuffer_source8[3][i] = 0;
		vbuffer_source8[4][i] = 0;
		vbuffer_source8[5][i] = 0;
		vbuffer_source8[6][i] = 0;
		vbuffer_source8[7][i] = 0;
		vbuffer_source8[8][i] = 0;
		vbuffer_source8[9][i] = 0;
		vbuffer_source8[10][i] = 0;
		vbuffer_source8[11][i] = 0;
		vbuffer_source8[12][i] = 0;
		vbuffer_source8[13][i] = 0;
		vbuffer_source8[14][i] = 0;
		vbuffer_source8[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL9_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL9_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL9_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL9_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL9_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL9_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks9(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv4 = kvdram4[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv5 = kvdram5[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv6 = kvdram6[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv7 = kvdram7[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv8 = kvdram8[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
			K = vv4.data[v].key;
			V = vv4.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }	
			K = vv5.data[v].key;
			V = vv5.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }	
			K = vv6.data[v].key;
			V = vv6.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }	
			K = vv7.data[v].key;
			V = vv7.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }	
			K = vv8.data[v].key;
			V = vv8.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK8=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK8=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGES_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGES_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGES_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGES_mergeVs(kvdram7, vdram);
	
	globalparams_t globalparamsK8;
	MERGES_mergeVs(kvdram8, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP9"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP10(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem10

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5

#pragma HLS DATA_PACK variable = kvdram6

#pragma HLS DATA_PACK variable = kvdram7

#pragma HLS DATA_PACK variable = kvdram8

#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source8
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source8 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];	
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source9
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source9 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];	
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[10];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
		vbuffer_source8[0][i] = 0;
		vbuffer_source8[1][i] = 0;
		vbuffer_source8[2][i] = 0;
		vbuffer_source8[3][i] = 0;
		vbuffer_source8[4][i] = 0;
		vbuffer_source8[5][i] = 0;
		vbuffer_source8[6][i] = 0;
		vbuffer_source8[7][i] = 0;
		vbuffer_source8[8][i] = 0;
		vbuffer_source8[9][i] = 0;
		vbuffer_source8[10][i] = 0;
		vbuffer_source8[11][i] = 0;
		vbuffer_source8[12][i] = 0;
		vbuffer_source8[13][i] = 0;
		vbuffer_source8[14][i] = 0;
		vbuffer_source8[15][i] = 0;
	
		vbuffer_source9[0][i] = 0;
		vbuffer_source9[1][i] = 0;
		vbuffer_source9[2][i] = 0;
		vbuffer_source9[3][i] = 0;
		vbuffer_source9[4][i] = 0;
		vbuffer_source9[5][i] = 0;
		vbuffer_source9[6][i] = 0;
		vbuffer_source9[7][i] = 0;
		vbuffer_source9[8][i] = 0;
		vbuffer_source9[9][i] = 0;
		vbuffer_source9[10][i] = 0;
		vbuffer_source9[11][i] = 0;
		vbuffer_source9[12][i] = 0;
		vbuffer_source9[13][i] = 0;
		vbuffer_source9[14][i] = 0;
		vbuffer_source9[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL10_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL10_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL10_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL10_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL10_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL10_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks10(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv4 = kvdram4[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv5 = kvdram5[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv6 = kvdram6[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv7 = kvdram7[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv8 = kvdram8[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv9 = kvdram9[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
			K = vv4.data[v].key;
			V = vv4.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }	
			K = vv5.data[v].key;
			V = vv5.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }	
			K = vv6.data[v].key;
			V = vv6.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }	
			K = vv7.data[v].key;
			V = vv7.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }	
			K = vv8.data[v].key;
			V = vv8.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK8=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK8=1 SEEN. index: "<<k<<endl; }	
			K = vv9.data[v].key;
			V = vv9.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK9=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK9=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGES_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGES_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGES_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGES_mergeVs(kvdram7, vdram);
	
	globalparams_t globalparamsK8;
	MERGES_mergeVs(kvdram8, vdram);
	
	globalparams_t globalparamsK9;
	MERGES_mergeVs(kvdram9, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP10"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP11(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * kvdram10,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem11

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5

#pragma HLS DATA_PACK variable = kvdram6

#pragma HLS DATA_PACK variable = kvdram7

#pragma HLS DATA_PACK variable = kvdram8

#pragma HLS DATA_PACK variable = kvdram9

#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source8
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source8 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];	
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source9
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source9 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];	
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source10
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source10 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source10 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source10 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE];	
	collection_t collections10[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[11];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
		vbuffer_source8[0][i] = 0;
		vbuffer_source8[1][i] = 0;
		vbuffer_source8[2][i] = 0;
		vbuffer_source8[3][i] = 0;
		vbuffer_source8[4][i] = 0;
		vbuffer_source8[5][i] = 0;
		vbuffer_source8[6][i] = 0;
		vbuffer_source8[7][i] = 0;
		vbuffer_source8[8][i] = 0;
		vbuffer_source8[9][i] = 0;
		vbuffer_source8[10][i] = 0;
		vbuffer_source8[11][i] = 0;
		vbuffer_source8[12][i] = 0;
		vbuffer_source8[13][i] = 0;
		vbuffer_source8[14][i] = 0;
		vbuffer_source8[15][i] = 0;
	
		vbuffer_source9[0][i] = 0;
		vbuffer_source9[1][i] = 0;
		vbuffer_source9[2][i] = 0;
		vbuffer_source9[3][i] = 0;
		vbuffer_source9[4][i] = 0;
		vbuffer_source9[5][i] = 0;
		vbuffer_source9[6][i] = 0;
		vbuffer_source9[7][i] = 0;
		vbuffer_source9[8][i] = 0;
		vbuffer_source9[9][i] = 0;
		vbuffer_source9[10][i] = 0;
		vbuffer_source9[11][i] = 0;
		vbuffer_source9[12][i] = 0;
		vbuffer_source9[13][i] = 0;
		vbuffer_source9[14][i] = 0;
		vbuffer_source9[15][i] = 0;
	
		vbuffer_source10[0][i] = 0;
		vbuffer_source10[1][i] = 0;
		vbuffer_source10[2][i] = 0;
		vbuffer_source10[3][i] = 0;
		vbuffer_source10[4][i] = 0;
		vbuffer_source10[5][i] = 0;
		vbuffer_source10[6][i] = 0;
		vbuffer_source10[7][i] = 0;
		vbuffer_source10[8][i] = 0;
		vbuffer_source10[9][i] = 0;
		vbuffer_source10[10][i] = 0;
		vbuffer_source10[11][i] = 0;
		vbuffer_source10[12][i] = 0;
		vbuffer_source10[13][i] = 0;
		vbuffer_source10[14][i] = 0;
		vbuffer_source10[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  collections10[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL11_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL11_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL11_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL11_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL11_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL11_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks11(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, globalstatsbuffer10, globalposition, globalparamsV, collections10);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv4 = kvdram4[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv5 = kvdram5[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv6 = kvdram6[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv7 = kvdram7[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv8 = kvdram8[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv9 = kvdram9[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv10 = kvdram10[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
			K = vv4.data[v].key;
			V = vv4.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }	
			K = vv5.data[v].key;
			V = vv5.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }	
			K = vv6.data[v].key;
			V = vv6.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }	
			K = vv7.data[v].key;
			V = vv7.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }	
			K = vv8.data[v].key;
			V = vv8.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK8=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK8=1 SEEN. index: "<<k<<endl; }	
			K = vv9.data[v].key;
			V = vv9.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK9=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK9=1 SEEN. index: "<<k<<endl; }	
			K = vv10.data[v].key;
			V = vv10.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK10=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK10=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGES_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGES_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGES_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGES_mergeVs(kvdram7, vdram);
	
	globalparams_t globalparamsK8;
	MERGES_mergeVs(kvdram8, vdram);
	
	globalparams_t globalparamsK9;
	MERGES_mergeVs(kvdram9, vdram);
	
	globalparams_t globalparamsK10;
	MERGES_mergeVs(kvdram10, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP11"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
extern "C" {
void TOPS_topkernelP12(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * kvdram10,
	uint512_dt * kvdram11,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem12

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram11 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0

#pragma HLS DATA_PACK variable = kvdram1

#pragma HLS DATA_PACK variable = kvdram2

#pragma HLS DATA_PACK variable = kvdram3

#pragma HLS DATA_PACK variable = kvdram4

#pragma HLS DATA_PACK variable = kvdram5

#pragma HLS DATA_PACK variable = kvdram6

#pragma HLS DATA_PACK variable = kvdram7

#pragma HLS DATA_PACK variable = kvdram8

#pragma HLS DATA_PACK variable = kvdram9

#pragma HLS DATA_PACK variable = kvdram10

#pragma HLS DATA_PACK variable = kvdram11
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source0
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source0 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source0 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE];	
	collection_t collections0[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source1
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source1 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source1 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE];	
	collection_t collections1[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source2
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source2 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source2 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE];	
	collection_t collections2[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source3
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source3 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source3 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE];	
	collection_t collections3[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source4
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source4 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source4 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE];	
	collection_t collections4[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source5
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source5 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source5 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE];	
	collection_t collections5[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source6
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source6 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source6 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE];	
	collection_t collections6[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source7
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source7 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source7 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE];	
	collection_t collections7[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source8
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source8 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source8 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE];	
	collection_t collections8[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source9
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source9 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source9 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE];	
	collection_t collections9[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source10
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source10 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source10 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source10 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source10 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE];	
	collection_t collections10[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	keyvalue_vbuffer_t vbuffer_source11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_source11
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER
		// #pragma HLS bind_storage variable=vbuffer_source11 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer_source11 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer_source11 type=block factor=2
		// #pragma HLS resource variable=vbuffer_source11 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer_source11 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer_source11 type=RAM_S2P impl=uram
	#endif
	
	keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE];	
	collection_t collections11[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ];
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[12];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	if(globalparamsK.ACTSPARAMS_EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	} else {
		for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = globalparamsK; }
	}
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	// Hybrid GP
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON){
		unsigned int called_in_current_iteration = OFF; 
		unsigned int inACTSMODE = UTILS_GetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
		if(inACTSMODE == ON){ called_in_current_iteration = ON; } else { called_in_current_iteration = OFF; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(globalparamsK.ACTSPARAMS_INSTID == 0){ cout<<"################ ACTSGP::[SWITCH?][GraphIter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"][inACTSMODE == "<<inACTSMODE<<"][ACTS_HYBRIDLOGIC == "<<globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC<<"][GPMODE: "<<globalparamsK.GPMODE<<"][ACTSGP called in this iteration: "<<called_in_current_iteration<<"]. ################"<<endl; }
		#endif
		if(called_in_current_iteration == OFF){ return; }
	}
	
	for (buffer_type i=0; i<MAX_BLOCKRAM_VSRCDATA_SIZE; i++){ // reset vbuffer_source 
		vbuffer_source0[0][i] = 0;
		vbuffer_source0[1][i] = 0;
		vbuffer_source0[2][i] = 0;
		vbuffer_source0[3][i] = 0;
		vbuffer_source0[4][i] = 0;
		vbuffer_source0[5][i] = 0;
		vbuffer_source0[6][i] = 0;
		vbuffer_source0[7][i] = 0;
		vbuffer_source0[8][i] = 0;
		vbuffer_source0[9][i] = 0;
		vbuffer_source0[10][i] = 0;
		vbuffer_source0[11][i] = 0;
		vbuffer_source0[12][i] = 0;
		vbuffer_source0[13][i] = 0;
		vbuffer_source0[14][i] = 0;
		vbuffer_source0[15][i] = 0;
	
		vbuffer_source1[0][i] = 0;
		vbuffer_source1[1][i] = 0;
		vbuffer_source1[2][i] = 0;
		vbuffer_source1[3][i] = 0;
		vbuffer_source1[4][i] = 0;
		vbuffer_source1[5][i] = 0;
		vbuffer_source1[6][i] = 0;
		vbuffer_source1[7][i] = 0;
		vbuffer_source1[8][i] = 0;
		vbuffer_source1[9][i] = 0;
		vbuffer_source1[10][i] = 0;
		vbuffer_source1[11][i] = 0;
		vbuffer_source1[12][i] = 0;
		vbuffer_source1[13][i] = 0;
		vbuffer_source1[14][i] = 0;
		vbuffer_source1[15][i] = 0;
	
		vbuffer_source2[0][i] = 0;
		vbuffer_source2[1][i] = 0;
		vbuffer_source2[2][i] = 0;
		vbuffer_source2[3][i] = 0;
		vbuffer_source2[4][i] = 0;
		vbuffer_source2[5][i] = 0;
		vbuffer_source2[6][i] = 0;
		vbuffer_source2[7][i] = 0;
		vbuffer_source2[8][i] = 0;
		vbuffer_source2[9][i] = 0;
		vbuffer_source2[10][i] = 0;
		vbuffer_source2[11][i] = 0;
		vbuffer_source2[12][i] = 0;
		vbuffer_source2[13][i] = 0;
		vbuffer_source2[14][i] = 0;
		vbuffer_source2[15][i] = 0;
	
		vbuffer_source3[0][i] = 0;
		vbuffer_source3[1][i] = 0;
		vbuffer_source3[2][i] = 0;
		vbuffer_source3[3][i] = 0;
		vbuffer_source3[4][i] = 0;
		vbuffer_source3[5][i] = 0;
		vbuffer_source3[6][i] = 0;
		vbuffer_source3[7][i] = 0;
		vbuffer_source3[8][i] = 0;
		vbuffer_source3[9][i] = 0;
		vbuffer_source3[10][i] = 0;
		vbuffer_source3[11][i] = 0;
		vbuffer_source3[12][i] = 0;
		vbuffer_source3[13][i] = 0;
		vbuffer_source3[14][i] = 0;
		vbuffer_source3[15][i] = 0;
	
		vbuffer_source4[0][i] = 0;
		vbuffer_source4[1][i] = 0;
		vbuffer_source4[2][i] = 0;
		vbuffer_source4[3][i] = 0;
		vbuffer_source4[4][i] = 0;
		vbuffer_source4[5][i] = 0;
		vbuffer_source4[6][i] = 0;
		vbuffer_source4[7][i] = 0;
		vbuffer_source4[8][i] = 0;
		vbuffer_source4[9][i] = 0;
		vbuffer_source4[10][i] = 0;
		vbuffer_source4[11][i] = 0;
		vbuffer_source4[12][i] = 0;
		vbuffer_source4[13][i] = 0;
		vbuffer_source4[14][i] = 0;
		vbuffer_source4[15][i] = 0;
	
		vbuffer_source5[0][i] = 0;
		vbuffer_source5[1][i] = 0;
		vbuffer_source5[2][i] = 0;
		vbuffer_source5[3][i] = 0;
		vbuffer_source5[4][i] = 0;
		vbuffer_source5[5][i] = 0;
		vbuffer_source5[6][i] = 0;
		vbuffer_source5[7][i] = 0;
		vbuffer_source5[8][i] = 0;
		vbuffer_source5[9][i] = 0;
		vbuffer_source5[10][i] = 0;
		vbuffer_source5[11][i] = 0;
		vbuffer_source5[12][i] = 0;
		vbuffer_source5[13][i] = 0;
		vbuffer_source5[14][i] = 0;
		vbuffer_source5[15][i] = 0;
	
		vbuffer_source6[0][i] = 0;
		vbuffer_source6[1][i] = 0;
		vbuffer_source6[2][i] = 0;
		vbuffer_source6[3][i] = 0;
		vbuffer_source6[4][i] = 0;
		vbuffer_source6[5][i] = 0;
		vbuffer_source6[6][i] = 0;
		vbuffer_source6[7][i] = 0;
		vbuffer_source6[8][i] = 0;
		vbuffer_source6[9][i] = 0;
		vbuffer_source6[10][i] = 0;
		vbuffer_source6[11][i] = 0;
		vbuffer_source6[12][i] = 0;
		vbuffer_source6[13][i] = 0;
		vbuffer_source6[14][i] = 0;
		vbuffer_source6[15][i] = 0;
	
		vbuffer_source7[0][i] = 0;
		vbuffer_source7[1][i] = 0;
		vbuffer_source7[2][i] = 0;
		vbuffer_source7[3][i] = 0;
		vbuffer_source7[4][i] = 0;
		vbuffer_source7[5][i] = 0;
		vbuffer_source7[6][i] = 0;
		vbuffer_source7[7][i] = 0;
		vbuffer_source7[8][i] = 0;
		vbuffer_source7[9][i] = 0;
		vbuffer_source7[10][i] = 0;
		vbuffer_source7[11][i] = 0;
		vbuffer_source7[12][i] = 0;
		vbuffer_source7[13][i] = 0;
		vbuffer_source7[14][i] = 0;
		vbuffer_source7[15][i] = 0;
	
		vbuffer_source8[0][i] = 0;
		vbuffer_source8[1][i] = 0;
		vbuffer_source8[2][i] = 0;
		vbuffer_source8[3][i] = 0;
		vbuffer_source8[4][i] = 0;
		vbuffer_source8[5][i] = 0;
		vbuffer_source8[6][i] = 0;
		vbuffer_source8[7][i] = 0;
		vbuffer_source8[8][i] = 0;
		vbuffer_source8[9][i] = 0;
		vbuffer_source8[10][i] = 0;
		vbuffer_source8[11][i] = 0;
		vbuffer_source8[12][i] = 0;
		vbuffer_source8[13][i] = 0;
		vbuffer_source8[14][i] = 0;
		vbuffer_source8[15][i] = 0;
	
		vbuffer_source9[0][i] = 0;
		vbuffer_source9[1][i] = 0;
		vbuffer_source9[2][i] = 0;
		vbuffer_source9[3][i] = 0;
		vbuffer_source9[4][i] = 0;
		vbuffer_source9[5][i] = 0;
		vbuffer_source9[6][i] = 0;
		vbuffer_source9[7][i] = 0;
		vbuffer_source9[8][i] = 0;
		vbuffer_source9[9][i] = 0;
		vbuffer_source9[10][i] = 0;
		vbuffer_source9[11][i] = 0;
		vbuffer_source9[12][i] = 0;
		vbuffer_source9[13][i] = 0;
		vbuffer_source9[14][i] = 0;
		vbuffer_source9[15][i] = 0;
	
		vbuffer_source10[0][i] = 0;
		vbuffer_source10[1][i] = 0;
		vbuffer_source10[2][i] = 0;
		vbuffer_source10[3][i] = 0;
		vbuffer_source10[4][i] = 0;
		vbuffer_source10[5][i] = 0;
		vbuffer_source10[6][i] = 0;
		vbuffer_source10[7][i] = 0;
		vbuffer_source10[8][i] = 0;
		vbuffer_source10[9][i] = 0;
		vbuffer_source10[10][i] = 0;
		vbuffer_source10[11][i] = 0;
		vbuffer_source10[12][i] = 0;
		vbuffer_source10[13][i] = 0;
		vbuffer_source10[14][i] = 0;
		vbuffer_source10[15][i] = 0;
	
		vbuffer_source11[0][i] = 0;
		vbuffer_source11[1][i] = 0;
		vbuffer_source11[2][i] = 0;
		vbuffer_source11[3][i] = 0;
		vbuffer_source11[4][i] = 0;
		vbuffer_source11[5][i] = 0;
		vbuffer_source11[6][i] = 0;
		vbuffer_source11[7][i] = 0;
		vbuffer_source11[8][i] = 0;
		vbuffer_source11[9][i] = 0;
		vbuffer_source11[10][i] = 0;
		vbuffer_source11[11][i] = 0;
		vbuffer_source11[12][i] = 0;
		vbuffer_source11[13][i] = 0;
		vbuffer_source11[14][i] = 0;
		vbuffer_source11[15][i] = 0;
	
	}
	for(unsigned int v=0; v<NUM_COLLECTIONS; v++){ for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[v][t].data1 = 0;  collections1[v][t].data1 = 0;  collections2[v][t].data1 = 0;  collections3[v][t].data1 = 0;  collections4[v][t].data1 = 0;  collections5[v][t].data1 = 0;  collections6[v][t].data1 = 0;  collections7[v][t].data1 = 0;  collections8[v][t].data1 = 0;  collections9[v][t].data1 = 0;  collections10[v][t].data1 = 0;  collections11[v][t].data1 = 0;  }}
	
	unsigned int PARTITIONS_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONS_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONS_CHKPT["<<u<<"]: "<<PARTITIONS_CHKPT[u]<<endl; }
	#endif
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){
		reducesourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	//cout<<"--------------------------))))))))))))))))))))))))))))))))))--------- top_nusrcv_nudstv: it_size: "<<it_size<<endl;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	unsigned int vdram_SRC4NextIt_BASE_KVS = 0; // NEWCHANGE.
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	// unsigned int srcvs_skip = (globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE) / VDATA_SHRINK_RATIO;
	unsigned int srcvs_skip = SRC_VOLUME_SIZE_KVS;
	for(unsigned int t=0; t<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID; t++){ vdram_SRC4NextIt_BASE_KVS += srcvs_skip; } // NOTE: must correspond with what is defined in utility.cpp
	#endif 
	// cout<<"-------------------------------------------- top_nusrcv_nudstv: vdram_SRC4NextIt_BASE_KVS: "<<vdram_SRC4NextIt_BASE_KVS<<" ----------------------------------------------------------- "<<endl;
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL12_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL12_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				#endif 
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // IMPORTANT.
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = 0;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 0; } 
				} else {
					if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
					else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
					else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				}
				
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
				
				TOPKERNEL12_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL12_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					}
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL12_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "source_partition", "currentLOP", "vreadoffset_kvs2", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, vreadoffset_kvs2, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL12_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readpmask(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; // NEWCHANGE.
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = OFF;
						if(globalposition.EN_PROCESS == ON){ enableprocess = ON; } else { enableprocess = OFF; }
						
						bool_type enable_readandreplicatevdata = ON;
						unsigned int num_active_vertices = 0;
						enable_readandreplicatevdata = OFF; // NEWCHANGE
						num_active_vertices = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<")...."<<endl; } // else { cout<<"-------------------------topkernelP: partition is NOT active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices<<") ...."<<endl; }
						#endif 
						if(globalposition.EN_PROCESS == ON){ 
							if(num_active_vertices > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; } // NEW CHANGE.	
						} else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices;
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks12(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10,vbuffer_source11, vdatabaseoffset_kvs + vdram_SRC4NextIt_BASE_KVS, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts 
						#ifdef TESTKERNEL
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, globalstatsbuffer10, globalposition, globalparamsV, collections10);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer_source11, globalstatsbuffer11, globalposition, globalparamsV, collections11);		
	
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, vdram_SRC4NextIt_BASE_KVS, NAp);
						#endif
						// cout<<"----- exit(EXIT_SUCCESS 22); -----"<<endl;
						// exit(EXIT_SUCCESS); //
					}
					// cout<<"----- exit(EXIT_SUCCESS 23); -----"<<endl;
					// exit(EXIT_SUCCESS); //
				}
				// cout<<"----- exit(EXIT_SUCCESS 24); -----"<<endl;
				// exit(EXIT_SUCCESS); //
			} // stage
			// cout<<"----- exit(EXIT_SUCCESS 25); -----"<<endl;
			// exit(EXIT_SUCCESS); //
		} // v_chunkid
		// cout<<"----- exit(EXIT_SUCCESS 26); -----"<<endl;
		// exit(EXIT_SUCCESS); //
	} // edgebankID
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILS_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"$$$****************************8 top_nusrcv_nudstv:: SEEN 1"<<endl;
	for(unsigned int k=0; k<globalparamsK.SIZEKVS2_REDUCEPARTITION; k++){
	
		uint512_dt vv0 = kvdram0[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv1 = kvdram1[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv2 = kvdram2[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv3 = kvdram3[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv4 = kvdram4[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv5 = kvdram5[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv6 = kvdram6[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv7 = kvdram7[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv8 = kvdram8[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv9 = kvdram9[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv10 = kvdram10[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
	
		uint512_dt vv11 = kvdram11[globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + k];
		
		unsigned int K = 0;
		unsigned int V = 0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			K = vv0.data[v].key;
			V = vv0.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK0=1 SEEN. index: "<<k<<endl; }	
			K = vv1.data[v].key;
			V = vv1.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK1=1 SEEN. index: "<<k<<endl; }	
			K = vv2.data[v].key;
			V = vv2.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK2=1 SEEN. index: "<<k<<endl; }	
			K = vv3.data[v].key;
			V = vv3.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK3=1 SEEN. index: "<<k<<endl; }	
			K = vv4.data[v].key;
			V = vv4.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK4=1 SEEN. index: "<<k<<endl; }	
			K = vv5.data[v].key;
			V = vv5.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK5=1 SEEN. index: "<<k<<endl; }	
			K = vv6.data[v].key;
			V = vv6.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK6=1 SEEN. index: "<<k<<endl; }	
			K = vv7.data[v].key;
			V = vv7.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK7=1 SEEN. index: "<<k<<endl; }	
			K = vv8.data[v].key;
			V = vv8.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK8=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK8=1 SEEN. index: "<<k<<endl; }	
			K = vv9.data[v].key;
			V = vv9.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK9=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK9=1 SEEN. index: "<<k<<endl; }	
			K = vv10.data[v].key;
			V = vv10.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK10=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK10=1 SEEN. index: "<<k<<endl; }	
			K = vv11.data[v].key;
			V = vv11.data[v].value;
			// if(MEMCAS_READVDATA(K)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
			// if(MEMCAS_READVDATA(V)==0){ cout<<"$$$ MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
			
			if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK11=1 SEEN. index: "<<k<<endl; }
			if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ top_nusrcv_nudstv:: MASK11=1 SEEN. index: "<<k<<endl; }	
		}
		#endif
	}
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////// */
	
	
	
	#ifdef CONFIG_RELEASE_VERSION2
	#ifdef TESTKERNEL
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	#else
	
	globalparams_t globalparamsK0;
	MERGES_mergeVs(kvdram0, vdram);
	
	globalparams_t globalparamsK1;
	MERGES_mergeVs(kvdram1, vdram);
	
	globalparams_t globalparamsK2;
	MERGES_mergeVs(kvdram2, vdram);
	
	globalparams_t globalparamsK3;
	MERGES_mergeVs(kvdram3, vdram);
	
	globalparams_t globalparamsK4;
	MERGES_mergeVs(kvdram4, vdram);
	
	globalparams_t globalparamsK5;
	MERGES_mergeVs(kvdram5, vdram);
	
	globalparams_t globalparamsK6;
	MERGES_mergeVs(kvdram6, vdram);
	
	globalparams_t globalparamsK7;
	MERGES_mergeVs(kvdram7, vdram);
	
	globalparams_t globalparamsK8;
	MERGES_mergeVs(kvdram8, vdram);
	
	globalparams_t globalparamsK9;
	MERGES_mergeVs(kvdram9, vdram);
	
	globalparams_t globalparamsK10;
	MERGES_mergeVs(kvdram10, vdram);
	
	globalparams_t globalparamsK11;
	MERGES_mergeVs(kvdram11, vdram);
	#endif 
	#endif
	// exit(EXIT_SUCCESS);///////////////////////////////
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP12"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	return;
}
}
	
	
#endif 

