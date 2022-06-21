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
unsigned int UTILP1_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int UTILP1_amax(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int UTILP1_aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}
uint32_type UTILP1_amin2(uint32_type val1, uint32_type val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

// allignment
batch_type UTILP1_allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type UTILP1_allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type UTILP1_allignlower_KV2(batch_type val){
	batch_type fac = val / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type UTILP1_allignhigher_KV2(batch_type val){
	batch_type fac = (val + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type UTILP1_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR){
	#pragma HLS INLINE
	batch_type fac = (val + (_FACTOR - 1)) / _FACTOR;
	return (fac * _FACTOR);
}
batch_type UTILP1_allignlower_FACTOR(batch_type val, unsigned int _FACTOR){
	batch_type fac = val / _FACTOR;
	return (fac * _FACTOR);
}

unsigned int UTILP1_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(unsigned int tree_depth){
	return (1 << (OPT_NUM_PARTITIONS_POW * (tree_depth-1)));
}

// bit manipulation
unsigned int UTILP1_GETMASK_UINT(uint32_type index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int UTILP1_READFROM_UINT(uint32_type data, unsigned int index, unsigned int size){ 
	unsigned int res = 0;
	res = (((data) & UTILP1_GETMASK_UINT((index), (size))) >> (index)); 
	return res;
}
void UTILP1_WRITETO_UINT(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~UTILP1_GETMASK_UINT((index), (size)));
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
unsigned int UTILP1_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	#ifdef _WIDEWORD
	res = data.range(index + size - 1, index); 
	#else
	res = UTILP1_READFROM_UINT(data, index, size);
	#endif
	return res;
}
void UTILP1_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	data->range(index + size - 1, index) = value;
	#else
	UTILP1_WRITETO_UINT(data, index, size, value);
	#endif
	return; 
}
unsigned int UTILP1_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	res = UTILP1_READFROM_UINT(data, index, size);
	return res;
}
void UTILP1_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	UTILP1_WRITETO_UINT(data, index, size, value);
	return; 
}

// converters
keyvalue_t UTILP1_GETKV(keyvalue_buffer_t data){
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
keyvalue_buffer_t UTILP1_GETKV(keyvalue_t data){
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
keyy_t UTILP1_GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_KEY - 1, 0);
	#else
	return data;
	#endif
}
value_t UTILP1_GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_VALUE - 1, 0);
	#else
	return data;
	#endif
}
keyy_t UTILP1_GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[v].key;	
	#endif
}		
unsigned int UTILP1_GETLOCALVID(unsigned int vid, unsigned int instid){ 
	#pragma HLS INLINE
	return (vid - instid) / NUM_PEs;
}
unsigned int UTILP1_GETREALVID(unsigned int lvid, unsigned int instid){ 
	#pragma HLS INLINE
	return (lvid * NUM_PEs) + instid;
}
unsigned int UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams){ 
	#pragma HLS INLINE
	return globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	// return PROCESSPARTITIONSZ_KVS2;
}

unsigned int UTILP1_GetData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index){
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
void UTILP1_SetData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data){
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
unsigned int UTILP1_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(31, 0);
	#else 
	return kvdram[offset_kvs].data[0].key;
	#endif 
}
void UTILP1_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(31, 0) = data;
	#else 
	kvdram[offset_kvs].data[0].key = data;
	#endif 
	return;
}
void UTILP1_SetFirstDatas(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int * data, unsigned int size){
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
unsigned int UTILP1_GetSecondData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(63, 32);
	#else 
	return kvdram[offset_kvs].data[0].value;
	#endif 
}
void UTILP1_SetSecondData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(63, 32) = data;
	#else 
	kvdram[offset_kvs].data[0].value = data;
	#endif 
	return;
}
void UTILP1_GetDataset(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
void UTILP1_GetDataset(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
	#pragma HLS INLINE
	#ifdef _WIDEWORD 
	datas[0].key = kvdram[offset_kvs].range(31, 0); 
	datas[0].value = kvdram[offset_kvs].range(63, 32); 
	datas[1].key = kvdram[offset_kvs].range(95, 64); 
	datas[1].value = kvdram[offset_kvs].range(127, 96); 
	datas[2].key = kvdram[offset_kvs].range(159, 128); 
	datas[2].value = kvdram[offset_kvs].range(191, 160); 
	datas[3].key = kvdram[offset_kvs].range(223, 192); 
	datas[3].value = kvdram[offset_kvs].range(255, 224); 
	datas[4].key = kvdram[offset_kvs].range(287, 256); 
	datas[4].value = kvdram[offset_kvs].range(319, 288); 
	datas[5].key = kvdram[offset_kvs].range(351, 320); 
	datas[5].value = kvdram[offset_kvs].range(383, 352); 
	datas[6].key = kvdram[offset_kvs].range(415, 384); 
	datas[6].value = kvdram[offset_kvs].range(447, 416); 
	datas[7].key = kvdram[offset_kvs].range(479, 448); 
	datas[7].value = kvdram[offset_kvs].range(511, 480); 
	#else 
	datas[0].key = kvdram[offset_kvs].data[0].key;
	datas[0].value = kvdram[offset_kvs].data[0].value; 
	datas[1].key = kvdram[offset_kvs].data[1].key;
	datas[1].value = kvdram[offset_kvs].data[1].value; 
	datas[2].key = kvdram[offset_kvs].data[2].key;
	datas[2].value = kvdram[offset_kvs].data[2].value; 
	datas[3].key = kvdram[offset_kvs].data[3].key;
	datas[3].value = kvdram[offset_kvs].data[3].value; 
	datas[4].key = kvdram[offset_kvs].data[4].key;
	datas[4].value = kvdram[offset_kvs].data[4].value; 
	datas[5].key = kvdram[offset_kvs].data[5].key;
	datas[5].value = kvdram[offset_kvs].data[5].value; 
	datas[6].key = kvdram[offset_kvs].data[6].key;
	datas[6].value = kvdram[offset_kvs].data[6].value; 
	datas[7].key = kvdram[offset_kvs].data[7].key;
	datas[7].value = kvdram[offset_kvs].data[7].value; 
	#endif
	return;
}
uint512_vec_dt UTILP1_GetDataset(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	uint512_vec_dt dataset;
	
	#ifdef _WIDEWORD 
	dataset.data[0].key = kvdram[offset_kvs].range(31, 0); 
	dataset.data[0].value = kvdram[offset_kvs].range(63, 32); 
	dataset.data[1].key = kvdram[offset_kvs].range(95, 64); 
	dataset.data[1].value = kvdram[offset_kvs].range(127, 96); 
	dataset.data[2].key = kvdram[offset_kvs].range(159, 128); 
	dataset.data[2].value = kvdram[offset_kvs].range(191, 160); 
	dataset.data[3].key = kvdram[offset_kvs].range(223, 192); 
	dataset.data[3].value = kvdram[offset_kvs].range(255, 224); 
	dataset.data[4].key = kvdram[offset_kvs].range(287, 256); 
	dataset.data[4].value = kvdram[offset_kvs].range(319, 288); 
	dataset.data[5].key = kvdram[offset_kvs].range(351, 320); 
	dataset.data[5].value = kvdram[offset_kvs].range(383, 352); 
	dataset.data[6].key = kvdram[offset_kvs].range(415, 384); 
	dataset.data[6].value = kvdram[offset_kvs].range(447, 416); 
	dataset.data[7].key = kvdram[offset_kvs].range(479, 448); 
	dataset.data[7].value = kvdram[offset_kvs].range(511, 480); 
	#else 
	dataset.data[0].key = kvdram[offset_kvs].data[0].key;
	dataset.data[0].value = kvdram[offset_kvs].data[0].value; 
	dataset.data[1].key = kvdram[offset_kvs].data[1].key;
	dataset.data[1].value = kvdram[offset_kvs].data[1].value; 
	dataset.data[2].key = kvdram[offset_kvs].data[2].key;
	dataset.data[2].value = kvdram[offset_kvs].data[2].value; 
	dataset.data[3].key = kvdram[offset_kvs].data[3].key;
	dataset.data[3].value = kvdram[offset_kvs].data[3].value; 
	dataset.data[4].key = kvdram[offset_kvs].data[4].key;
	dataset.data[4].value = kvdram[offset_kvs].data[4].value; 
	dataset.data[5].key = kvdram[offset_kvs].data[5].key;
	dataset.data[5].value = kvdram[offset_kvs].data[5].value; 
	dataset.data[6].key = kvdram[offset_kvs].data[6].key;
	dataset.data[6].value = kvdram[offset_kvs].data[6].value; 
	dataset.data[7].key = kvdram[offset_kvs].data[7].key;
	dataset.data[7].value = kvdram[offset_kvs].data[7].value; 
	#endif
	
	return dataset;
}
void UTILP1_SetDataset(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
void UTILP1_SetDataset(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
	#pragma HLS INLINE
	#ifdef _WIDEWORD 
	kvdram[offset_kvs].range(31, 0) = datas[0].key; 
	kvdram[offset_kvs].range(63, 32) = datas[0].value; 
	kvdram[offset_kvs].range(95, 64) = datas[1].key; 
	kvdram[offset_kvs].range(127, 96) = datas[1].value; 
	kvdram[offset_kvs].range(159, 128) = datas[2].key; 
	kvdram[offset_kvs].range(191, 160) = datas[2].value; 
	kvdram[offset_kvs].range(223, 192) = datas[3].key; 
	kvdram[offset_kvs].range(255, 224) = datas[3].value; 
	kvdram[offset_kvs].range(287, 256) = datas[4].key; 
	kvdram[offset_kvs].range(319, 288) = datas[4].value; 
	kvdram[offset_kvs].range(351, 320) = datas[5].key; 
	kvdram[offset_kvs].range(383, 352) = datas[5].value; 
	kvdram[offset_kvs].range(415, 384) = datas[6].key; 
	kvdram[offset_kvs].range(447, 416) = datas[6].value; 
	kvdram[offset_kvs].range(479, 448) = datas[7].key; 
	kvdram[offset_kvs].range(511, 480) = datas[7].value; 
	#else 
	kvdram[offset_kvs].data[0].key = datas[0].key;
	kvdram[offset_kvs].data[0].value = datas[0].value; 
	kvdram[offset_kvs].data[1].key = datas[1].key;
	kvdram[offset_kvs].data[1].value = datas[1].value; 
	kvdram[offset_kvs].data[2].key = datas[2].key;
	kvdram[offset_kvs].data[2].value = datas[2].value; 
	kvdram[offset_kvs].data[3].key = datas[3].key;
	kvdram[offset_kvs].data[3].value = datas[3].value; 
	kvdram[offset_kvs].data[4].key = datas[4].key;
	kvdram[offset_kvs].data[4].value = datas[4].value; 
	kvdram[offset_kvs].data[5].key = datas[5].key;
	kvdram[offset_kvs].data[5].value = datas[5].value; 
	kvdram[offset_kvs].data[6].key = datas[6].key;
	kvdram[offset_kvs].data[6].value = datas[6].value; 
	kvdram[offset_kvs].data[7].key = datas[7].key;
	kvdram[offset_kvs].data[7].value = datas[7].value; 
	#endif
	return;
}
void UTILP1_SetDataset(uint512_dt * kvdram, unsigned int offset_kvs, uint512_vec_dt dataset){
	#pragma HLS INLINE
	#ifdef _WIDEWORD 
	kvdram[offset_kvs].range(31, 0) = dataset.data[0].key; 
	kvdram[offset_kvs].range(63, 32) = dataset.data[0].value; 
	kvdram[offset_kvs].range(95, 64) = dataset.data[1].key; 
	kvdram[offset_kvs].range(127, 96) = dataset.data[1].value; 
	kvdram[offset_kvs].range(159, 128) = dataset.data[2].key; 
	kvdram[offset_kvs].range(191, 160) = dataset.data[2].value; 
	kvdram[offset_kvs].range(223, 192) = dataset.data[3].key; 
	kvdram[offset_kvs].range(255, 224) = dataset.data[3].value; 
	kvdram[offset_kvs].range(287, 256) = dataset.data[4].key; 
	kvdram[offset_kvs].range(319, 288) = dataset.data[4].value; 
	kvdram[offset_kvs].range(351, 320) = dataset.data[5].key; 
	kvdram[offset_kvs].range(383, 352) = dataset.data[5].value; 
	kvdram[offset_kvs].range(415, 384) = dataset.data[6].key; 
	kvdram[offset_kvs].range(447, 416) = dataset.data[6].value; 
	kvdram[offset_kvs].range(479, 448) = dataset.data[7].key; 
	kvdram[offset_kvs].range(511, 480) = dataset.data[7].value; 
	#else 
	kvdram[offset_kvs].data[0].key = dataset.data[0].key;
	kvdram[offset_kvs].data[0].value = dataset.data[0].value; 
	kvdram[offset_kvs].data[1].key = dataset.data[1].key;
	kvdram[offset_kvs].data[1].value = dataset.data[1].value; 
	kvdram[offset_kvs].data[2].key = dataset.data[2].key;
	kvdram[offset_kvs].data[2].value = dataset.data[2].value; 
	kvdram[offset_kvs].data[3].key = dataset.data[3].key;
	kvdram[offset_kvs].data[3].value = dataset.data[3].value; 
	kvdram[offset_kvs].data[4].key = dataset.data[4].key;
	kvdram[offset_kvs].data[4].value = dataset.data[4].value; 
	kvdram[offset_kvs].data[5].key = dataset.data[5].key;
	kvdram[offset_kvs].data[5].value = dataset.data[5].value; 
	kvdram[offset_kvs].data[6].key = dataset.data[6].key;
	kvdram[offset_kvs].data[6].value = dataset.data[6].value; 
	kvdram[offset_kvs].data[7].key = dataset.data[7].key;
	kvdram[offset_kvs].data[7].value = dataset.data[7].value; 
	#endif
	return;
}
void UTILP1_SetDataset(keyvalue_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
	#pragma HLS INLINE
	buffer[0][offset_kvs].key = datas[0].key;
	buffer[0][offset_kvs].value = datas[0].value; 
	buffer[1][offset_kvs].key = datas[1].key;
	buffer[1][offset_kvs].value = datas[1].value; 
	buffer[2][offset_kvs].key = datas[2].key;
	buffer[2][offset_kvs].value = datas[2].value; 
	buffer[3][offset_kvs].key = datas[3].key;
	buffer[3][offset_kvs].value = datas[3].value; 
	buffer[4][offset_kvs].key = datas[4].key;
	buffer[4][offset_kvs].value = datas[4].value; 
	buffer[5][offset_kvs].key = datas[5].key;
	buffer[5][offset_kvs].value = datas[5].value; 
	buffer[6][offset_kvs].key = datas[6].key;
	buffer[6][offset_kvs].value = datas[6].value; 
	buffer[7][offset_kvs].key = datas[7].key;
	buffer[7][offset_kvs].value = datas[7].value; 
	return;
}

// utilities
batch_type UTILP1_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
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
batch_type UTILP1_getrangeforeachllop(globalparams_t globalparams){
	unsigned int range = globalparams.SIZE_BATCHRANGE;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
		range = range / OPT_NUM_PARTITIONS;
	}
	return range;
}
buffer_type UTILP1_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
buffer_type UTILP1_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	buffer_type size_kvs = 0;
	size_kvs = realsize_kvs;
	return size_kvs;
}
void UTILP1_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size){
	unsigned int analysis_size = OPT_NUM_PARTITIONS;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = UTILP1_allignhigher_KV(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}
void UTILP1_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
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
batch_type UTILP1_get_num_source_partitions(step_type currentLOP){
	if(currentLOP == 0){ currentLOP = 1; }
	batch_type pow = 1;
	for(step_type i=0; i<(currentLOP-1); i++){
		pow = pow * OPT_NUM_PARTITIONS;
	}
	return pow;
}

globalparams_t UTILP1_getglobalparams(uint512_dt * kvdram, unsigned int banksection){
	globalparams_t globalparams;
	
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("UTILP1_getglobalparams 23", 0 + MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS, (((1 << 28) / 4) / 16), globalparams.DRAM_BASE_KVS, MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp);
	#endif
	unsigned int edges_data_kvs = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_VOLUMEOFFSETKVS_WORKDATA]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	unsigned int workspace_data_kvs = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_VOLUMEOFFSETKVS_WORKSPACEDATA] // NOT USED.
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif
		
	if(banksection==0){ globalparams.DRAM_BASE_KVS = BASEOFFSET_MESSAGESDATA_KVS; }
	else if (banksection==1) { globalparams.DRAM_BASE_KVS = BASEOFFSET_MESSAGESDATA_KVS + edges_data_kvs; }
	else {
		#ifdef _DEBUGMODE_CHECKS3
		cout<<"UTILP1_getglobalparams: NOT IMPLEMENTED. EXITING..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	
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
	
	globalparams.ENABLE_RUNKERNELCOMMAND = buffer[MESSAGES_ENABLE_RUNKERNELCOMMAND];
	globalparams.ENABLE_PROCESSCOMMAND = buffer[MESSAGES_ENABLE_PROCESSCOMMAND];
	globalparams.ENABLE_PARTITIONCOMMAND = buffer[MESSAGES_ENABLE_PARTITIONCOMMAND];
	globalparams.ENABLE_APPLYUPDATESCOMMAND = buffer[MESSAGES_ENABLE_APPLYUPDATESCOMMAND];
	globalparams.ENABLE_MERGECOMMAND = buffer[MESSAGES_ENABLE_MERGECOMMAND];
	globalparams.ENABLE_EXCHANGECOMMAND = buffer[MESSAGES_ENABLE_EXCHANGECOMMAND];
	globalparams.GPMODE = buffer[MESSAGES_GPMODE];
	globalparams.ACTSCONFIG_INSERTSTATSMETADATAINEDGES = buffer[MESSAGES_ACTSCONFIG_INSERTSTATSMETADATAINEDGES];

	globalparams.EVALUATION_ACTS_MEMORYLAYOUT = buffer[MESSAGES_EVALUATION_ACTS_MEMORYLAYOUT];
	globalparams.EVALUATION_ACTS_PARTITIONINGLOGIC = buffer[MESSAGES_EVALUATION_ACTS_PARTITIONINGLOGIC];
	globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC = buffer[MESSAGES_EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC];
	globalparams.EVALUATION_ACTS_HYBRIDLOGIC = buffer[MESSAGES_EVALUATION_ACTS_HYBRIDLOGIC];
	globalparams.EVALUATION_ACTS_VERYBARE = buffer[MESSAGES_EVALUATION_ACTS_VERYBARE];
	
	globalparams.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD = buffer[MESSAGES_THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD];
	globalparams.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION = buffer[MESSAGES_THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION];
	
	globalparams.BASEOFFSETKVS_MESSAGESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_MESSAGESDATA];
	globalparams.VOLUMEOFFSETKVS_WORKDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_VOLUMEOFFSETKVS_WORKDATA];
	globalparams.VOLUMEOFFSETKVS_WORKSPACEDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_VOLUMEOFFSETKVS_WORKSPACEDATA];
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
	globalparams.VOLUMESIZEU32_WORKDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_VOLUMESIZEU32_WORKDATA];
	globalparams.VOLUMESIZEU32_WORKSPACEDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_VOLUMESIZEU32_WORKSPACEDATA];
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
	globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS = buffer[MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS]; 
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
		cout<<"getglobalparams:: banksection: "<<banksection<<endl;
		cout<<"getglobalparams:: edges_data_kvs: "<<edges_data_kvs<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	
	globalparams.VARS_WORKBATCH = 0;
	return globalparams;
}

sweepparams_t UTILP1_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
	sweepparams_t sweepparams;
	batch_type sourceskipsize = UTILP1_getskipsize(currentLOP, SOURCE, globalparams);
	
	sweepparams.currentLOP = currentLOP;
	
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC == ON){ 
		if(currentLOP == 1){ sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; }
		else {
			if(currentLOP % 2 == 0){ sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; }
			else { sweepparams.worksourcebaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAMWORKSPACE; sweepparams.workdestbaseaddress_kvs = globalparams.BASEOFFSETKVS_KVDRAM; }
		}
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
travstate_t UTILP1_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
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
			cout<<"UTILP1_gettravstate: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
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
void UTILP1_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value){			
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

partition_type UTILP1_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, unsigned int tree_depth){
	partition_type partition;
	keyvalue_t thiskeyvalue = UTILP1_GETKV(keyvalue);

	if(thiskeyvalue.value == UTILP1_GETV(INVALIDDATA)){ partition = 0; } 
	else {
		keyy_t lkey = thiskeyvalue.key - upperlimit;
		if(mode == ACTSREDUCEMODE || tree_depth == 1){ partition = (lkey % OPT_NUM_PARTITIONS); } else { partition = (lkey >> (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP))); }
	}
	
	if(partition >= OPT_NUM_PARTITIONS){ 
		#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"acts_util::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", thiskeyvalue.key: "<<thiskeyvalue.key<<", thiskeyvalue.value: "<<thiskeyvalue.value<<", OPT_NUM_PARTITIONS: "<<OPT_NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP)))<<", INVALIDDATA: "<<INVALIDDATA<<", UTILP1_GETV(INVALIDDATA): "<<UTILP1_GETV(INVALIDDATA)<<endl;			 
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
partition_type UTILP1_getpartition2(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, globalparams_t globalparams){
	partition_type partition;
	keyvalue_t thiskeyvalue = UTILP1_GETKV(keyvalue);
	
	if(thiskeyvalue.value == UTILP1_GETV(INVALIDDATA)){ partition = 0; } 
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
			cout<<"acts_util::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", thiskeyvalue.key: "<<thiskeyvalue.key<<", thiskeyvalue.value: "<<thiskeyvalue.value<<", OPT_NUM_PARTITIONS: "<<OPT_NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP)))<<", INVALIDDATA: "<<INVALIDDATA<<", UTILP1_GETV(INVALIDDATA): "<<UTILP1_GETV(INVALIDDATA)<<endl;			 
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
void UTILP1_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void UTILP1_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void UTILP1_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i] = resetval; 
	}
}
void UTILP1_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; 
		buffer[i].value = resetval; 
	}
	return;
}
void UTILP1_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"..................... UTILP1_resetkvstatvalues: resetting global stats..."<<endl;
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
void UTILP1_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams){
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
void UTILP1_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
			cout<<"UTILP1_gettravstate: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<") EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}
void UTILP1_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _WIDEWORD
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#else
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}
void UTILP1_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
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
void UTILP1_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
	for(partition_type p=0; p<OPT_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}

// checks 
void UTILP1_check_capsule(keyvalue_t capsule[MAX_NUM_PARTITIONS], unsigned int num_partitions, unsigned int max_value){
	#if defined(ENABLE_PERFECTACCURACY) && defined(_DEBUGMODE_CHECKS2)	
	for(unsigned int i=0; i<num_partitions-1; i++){ 
		if(capsule[i].key + capsule[i].value >= capsule[i+1].key && capsule[i].value > 0){ 
			cout<<"savekeyvalues::capsule 33. ERROR. out of bounds. (capsule["<<i<<"].key("<<capsule[i].key<<") + capsule["<<i<<"].value("<<capsule[i].value<<") >= capsule["<<i+1<<"].key("<<capsule[i+1].key<<")) printing and exiting..."<<endl; 
			actsutilityobj->printkeyvalues("savekeyvalues::capsule 34", (keyvalue_t *)capsule, num_partitions); 
			exit(EXIT_FAILURE); 
		}
	}
	if(capsule[num_partitions-1].key + capsule[num_partitions-1].value >= max_value){
		cout<<"savekeyvalues::capsule 36. ERROR. out of bounds. (capsule["<<num_partitions-1<<"].key("<<capsule[num_partitions-1].key<<") + capsule["<<num_partitions-1<<"].value("<<capsule[num_partitions-1].value<<") >= globalparams.SIZE_KVDRAM("<<globalparams.SIZE_KVDRAM<<")) printing and exiting..."<<endl; 
		actsutilityobj->printkeyvalues("savekeyvalues::capsule 37", (keyvalue_t *)capsule, num_partitions); 
		exit(EXIT_FAILURE); 
	}
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printkeyvalues("actsutility::savekeyvalues: capsule.", (keyvalue_t *)capsule, num_partitions);
	#endif
	return;
}	
#endif 
#ifdef CONFIG_ENABLECLASS_ALGO_FUNCS
/* 
All algorithms: 
https://neo4j.com/docs/graph-data-science/current/algorithms/
https://iss.oden.utexas.edu/?p=projects/galois/analytics/betweenness_centrality

*/

value_t PROCESSP1_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	#pragma HLS INLINE // NEWCHANGE.
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

value_t REDUCEP1_reducefunc(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	#pragma HLS INLINE // NEWCHANGE.
	value_t temp = 0;
	#ifdef CONFIG_GOLDENKERNEL
	// temp = UTILP1_amax(vtemp, 1);
	unsigned int lamda = 1;
	unsigned int ew=1;
	if(GraphAlgo == PAGERANK){
		temp = vtemp + res;
	} else if(GraphAlgo == CF){
		temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
	} else if(GraphAlgo == HITS){
		temp = vtemp + res;
	} else if(GraphAlgo == SPMV){
		temp = UTILP1_amin(vtemp, res);
	} else if(GraphAlgo == BFS){
		temp = UTILP1_amin(vtemp, GraphIter);
	} else if(GraphAlgo == SSSP){
		temp = UTILP1_amin(vtemp, res);
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
			temp = UTILP1_amin(vtemp, res);
		#elif defined(BFS_ALGORITHM)
			// Atomic Min
			#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
			temp = UTILP1_amax(vtemp, 1);
			#else 
			temp = UTILP1_amin(vtemp, GraphIter);
			#endif 
		#elif defined(SSSP_ALGORITHM)
			// Atomic Min
			temp = UTILP1_amin(vtemp, res);
		#else 
			NOT DEFINED.
		#endif
	#endif
	return temp;
}

	
#endif 
#ifdef CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
// primitives
unsigned int MEMCAP1_READVDATA(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILP1_READBITSFROM_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA);
	#endif 
}
unsigned int MEMCAP1_READVMASK(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILP1_READBITSFROM_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK);
	#endif 
}
unsigned int MEMCAP1_READVDATA2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILP1_READBITSFROM_UINTV(wideword, offsetof_vdata, SIZEOF_VDATA);
	#endif 
}
unsigned int MEMCAP1_READVMASK2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILP1_READBITSFROM_UINTV(wideword, offsetof_vmask, SIZEOF_VMASK);
	#endif 
}

void MEMCAP1_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILP1_WRITEBITSTO_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void MEMCAP1_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILP1_WRITEBITSTO_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK, vmask);
	#endif 
	return;
}
void MEMCAP1_WRITEVDATA2(keyvalue_vbuffer_t * wideword, value_t vdata, unsigned int offsetof_vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILP1_WRITEBITSTO_UINTV(wideword, offsetof_vdata, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void MEMCAP1_WRITEVMASK2(keyvalue_vbuffer_t * wideword, unit1_type vmask, unsigned int offsetof_vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILP1_WRITEBITSTO_UINTV(wideword, offsetof_vmask, SIZEOF_VMASK, vmask);
	#endif 
	return;
}

unsigned int MEMCAP1_READEDIR(unsigned int wideword){
	#pragma HLS INLINE
	return UTILP1_READBITSFROM_UINTV(wideword, OFFSETOF_EDIR, SIZEOF_EDIR);
}

// non-primitives
vmdata_t MEMCAP1_READFROMBUFFER_VDATAWITHVMASK(keyvalue_vbuffer_t bits_vector){
	#pragma HLS INLINE
	vmdata_t vmdata;
	vmdata.vdata = MEMCAP1_READVDATA(bits_vector); 
	vmdata.vmask = MEMCAP1_READVMASK(bits_vector); 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	if(vmdata.vmask == 1){ cout<<">>> MEMCAP1_READFROMBUFFER_VDATAWITHVMASK:: ACTIVE VERTEX READ: @ index: "<<index<<endl; }
	#endif
	return vmdata;
}
void MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){				
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK:", index/2, MAX_BLOCKRAM_VDESTDATA_SIZE, index, NAp, NAp);
	#endif

	MEMCAP1_WRITEVDATA(&bits_vector, vdata);
	MEMCAP1_WRITEVMASK(&bits_vector, vmask);
	buffer[bufferoffset_kvs + index] = bits_vector;
	return;
}
void MEMCAP1_WRITETODRAM_VDATAWITHVMASK(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP1_WRITETODRAM_VDATAWITHVMASK:", index, MAX_MAXHBMCAPACITY_KVS, index, NAp, NAp);
	#endif

	MEMCAP1_WRITEVDATA(&bits_vector, vdata);
	MEMCAP1_WRITEVMASK(&bits_vector, vmask);
	UTILP1_SetData(kvdram, dramoffset_kvs, (index / VDATA_SHRINK_RATIO), bits_vector);
	return;
}

vmdata_t MEMCAP1_READFROMBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t bits_vector){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP1_READFROMBUFFER_VDATAWITHVMASK2(220):", index % VDATA_SHRINK_RATIO, 32, index, NAp, NAp);
	#endif
	
	vmdata_t vmdata;
	tuple_t tup;
	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	
	#ifdef HWBITACCESSTYPE
	tup.A = MEMCAP1_READVDATA2(bits_vector, offsetof_vdata); 
	tup.B = MEMCAP1_READVMASK2(bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata); 
	vmdata.vdata = tup.A;
	vmdata.vmask = tup.B;
	#else
	vmdata.vdata = UTILP1_SWREADBITSFROM_UINTV(bits_vector, offsetof_vdata, SIZEOF_VDATA);
	vmdata.vmask = UTILP1_SWREADBITSFROM_UINTV(bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata, SIZEOF_DEST_VMASK);
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> MEMCAP1_VDATAWITHVMASK2(224):: ACTIVE VERTEX READ: @ offsetof_vdata: "<<offsetof_vdata<<", vmdata.vdata: "<<vmdata.vdata<<", vmdata.vmask: "<<vmdata.vmask<<", sub_chunk_height: "<<(index / VDATA_SHRINK_RATIO)<<", index: "<<index<<endl; }						
	#endif
	return vmdata;
}
void MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK2:", index/VDATA_SHRINK_RATIO, MAX_BLOCKRAM_VDESTDATA_SIZE, index, NAp, NAp);
	#endif

	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	// keyvalue_vbuffer_t bits_vector = buffer[bufferoffset_kvs + (index / VDATA_SHRINK_RATIO)];
	
	#ifdef HWBITACCESSTYPE
	MEMCAP1_WRITEVDATA2(&bits_vector, vdata, offsetof_vdata);
	MEMCAP1_WRITEVMASK2(&bits_vector, vmdata, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata);
	#else
	UTILP1_SWWRITEBITSTO_UINTV(&bits_vector, offsetof_vdata, SIZEOF_VDATA, vdata);
	UTILP1_SWWRITEBITSTO_UINTV(&bits_vector, BEGINOFFSETOF_DEST_VMASK + offsetof_vdata, SIZEOF_DEST_VMASK, vmask);	
	#endif 
	
	buffer[bufferoffset_kvs + (index / VDATA_SHRINK_RATIO)] = bits_vector;
	return;
}
void MEMCAP1_WRITETODRAM_VDATAWITHVMASK2(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP1_WRITETODRAM_VDATAWITHVMASK2:", index/VDATA_SHRINK_RATIO, MAX_MAXHBMCAPACITY_KVS, index, NAp, NAp);
	#endif

	unsigned int offsetof_vdata = (index % VDATA_SHRINK_RATIO);
	
	#ifdef HWBITACCESSTYPE
	MEMCAP1_WRITEVDATA2(&bits_vector, vdata, offsetof_vdata);
	MEMCAP1_WRITEVMASK2(&bits_vector, vmask, BEGINOFFSETOF_VMASK + offsetof_vdata);
	#else
	UTILP1_SWWRITEBITSTO_UINTV(&bits_vector, offsetof_vdata, SIZEOF_VDATA, vdata);
	UTILP1_SWWRITEBITSTO_UINTV(&bits_vector, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK, vmask);	
	#endif 
	
	UTILP1_SetData(kvdram, dramoffset_kvs, (index / VDATA_SHRINK_RATIO), bits_vector);
	return;
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_MEM_ACCESS
// -------------------- key values -------------------- //
fetchmessage_t MEMACCESSP1_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	buffer_type chunk_size = UTILP1_getchunksize_kvs(size_kvs, travstate, 0);
	
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
		
		buffer[0][i] = UTILP1_GETKV(mykeyvalue0);
		buffer[1][i] = UTILP1_GETKV(mykeyvalue1);
		buffer[2][i] = UTILP1_GETKV(mykeyvalue2);
		buffer[3][i] = UTILP1_GETKV(mykeyvalue3);
		buffer[4][i] = UTILP1_GETKV(mykeyvalue4);
		buffer[5][i] = UTILP1_GETKV(mykeyvalue5);
		buffer[6][i] = UTILP1_GETKV(mykeyvalue6);
		buffer[7][i] = UTILP1_GETKV(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readkeyvalues:: keyvalues read: offset_kvs: "<<offset_kvs<<", size_kvs: "<<size_kvs<<", chunk_size: "<<chunk_size<<endl;
	#endif
	return fetchmessage;
}

void MEMACCESSP1_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_dest[MAX_NUM_PARTITIONS], batch_type destbaseaddr_kvs, globalposition_t globalposition, globalparams_t globalparams){
	// globalparams.ACTSPARAMS_NUM_PARTITIONS replacedby 16
	analysis_type analysis_loopcount1a = 16;
	analysis_type analysis_loopcount1b = 32;
	keyvalue_t updates[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=updates complete
	
	#ifdef _DEBUGMODE_CHECKS2
	UTILP1_check_capsule((keyvalue_t*)capsule_dest, globalparams.ACTSPARAMS_NUM_PARTITIONS, MAX_DESTBLOCKRAM_SIZE);
	#endif
	
	ACTIT2_PP0_COMMIT_MAINLOOP1: for(partition_type p=0; p<16; p++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1a avg=analysis_loopcount1a
		batch_type dramoffset_kvs = (globalcapsule[p].key + globalcapsule[p].value) / VECTOR_SIZE;
		buffer_type bramoffset_kvs = capsule_dest[p].key / VECTOR2_SIZE;
		buffer_type size_kvs = capsule_dest[p].value / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("actit2::savekeyvalues 23", bramoffset_kvs + size_kvs, MAX_DESTBLOCKRAM_SIZE + 1, p, bramoffset_kvs, size_kvs);
		#endif
		
		ACTIT2_PP0_COMMIT_MAINLOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1b avg=analysis_loopcount1b
		#pragma HLS PIPELINE II=1
			updates[0] = UTILP1_GETKV(destbuffer[0][bramoffset_kvs + i]);	
			updates[1] = UTILP1_GETKV(destbuffer[1][bramoffset_kvs + i]);	
			updates[2] = UTILP1_GETKV(destbuffer[2][bramoffset_kvs + i]);	
			updates[3] = UTILP1_GETKV(destbuffer[3][bramoffset_kvs + i]);	
			updates[4] = UTILP1_GETKV(destbuffer[4][bramoffset_kvs + i]);	
			updates[5] = UTILP1_GETKV(destbuffer[5][bramoffset_kvs + i]);	
			updates[6] = UTILP1_GETKV(destbuffer[6][bramoffset_kvs + i]);	
			updates[7] = UTILP1_GETKV(destbuffer[7][bramoffset_kvs + i]);	
			
			UTILP1_SetDataset(kvdram, destbaseaddr_kvs + dramoffset_kvs, updates);
		}
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvspartitionswritten(globalparams.ACTSPARAMS_INSTID, size_kvs * VECTOR_SIZE);
		#endif
	}
	ACTIT2_PP0_COMMIT_MAINLOOP2: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ if(globalcapsule[p].key + globalcapsule[p].value + (capsule_dest[p].value) < globalparams.SIZE_KVDRAM){ globalcapsule[p].value += capsule_dest[p].value; }}
	
	#ifdef _DEBUGMODE_CHECKS2
	UTILP1_check_capsule(globalcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.SIZE_KVDRAM);
	#endif
	return;
}

// -------------------- vdata -------------------- //
void MEMACCESSP1_GetXYLayoutV(unsigned int s, vmdata_t vdata[VECTOR2_SIZE], vmdata_t vdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth){
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

void MEMACCESSP1_RearrangeLayoutV(unsigned int s, keyvalue_vbuffer_t vdata[VECTOR2_SIZE], keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]){
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

void MEMACCESSP1_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){
	if(enable == OFF){ return; }

	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	size_kvs = size_kvs / 2; // NEWCHANGE.
	#endif 
	
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
			MEMCAP1_WRITEVMASK(&vdata[0], 0);
			MEMCAP1_WRITEVMASK(&vdata[1], 0);
			MEMCAP1_WRITEVMASK(&vdata[2], 0);
			MEMCAP1_WRITEVMASK(&vdata[3], 0);
			MEMCAP1_WRITEVMASK(&vdata[4], 0);
			MEMCAP1_WRITEVMASK(&vdata[5], 0);
			MEMCAP1_WRITEVMASK(&vdata[6], 0);
			MEMCAP1_WRITEVMASK(&vdata[7], 0);
			MEMCAP1_WRITEVMASK(&vdata[8], 0);
			MEMCAP1_WRITEVMASK(&vdata[9], 0);
			MEMCAP1_WRITEVMASK(&vdata[10], 0);
			MEMCAP1_WRITEVMASK(&vdata[11], 0);
			MEMCAP1_WRITEVMASK(&vdata[12], 0);
			MEMCAP1_WRITEVMASK(&vdata[13], 0);
			MEMCAP1_WRITEVMASK(&vdata[14], 0);
			MEMCAP1_WRITEVMASK(&vdata[15], 0);
		}
		#endif
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSRRRP1_readV 21", bufferoffset_kvs + i, MAX_BLOCKRAM_VDESTDATA_SIZE, size_kvs, bufferoffset_kvs, i);
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

void MEMACCESSP1_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	// cout<<"MEMACCESSP1_saveV SEEN. "<<endl;
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	size_kvs = size_kvs / 2; // NEWCHANGE.
	#endif 
	
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
		actsutilityobj->checkoutofbounds("MEMACCESSP1_saveV 23", baseoffset_kvs + offset_kvs + i, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
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
			if(MEMCAP1_READVMASK(vdata[v])==1){ cout<<"$$$ MEMACCESSP1_saveV:: MASK=1 SEEN. index: "<<i<<endl; }
		}
		#endif

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countvswritten(VECTOR2_SIZE);
		#endif
	}
	// exit(EXIT_SUCCESS); //
	return;
}

void MEMACCESSP1_copyV(bool_type enable, keyvalue_vbuffer_t buffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_vbuffer_t buffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], buffer_type srcoffset_kvs, buffer_type destoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	size_kvs = size_kvs / 2; // NEWCHANGE.
	#endif 

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

void MEMACCESSP1_readANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks1 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks1 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks1 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks2 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks2 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks2 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks3 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks3 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks3 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks4 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks4 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks4 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks5 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks5 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks5 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks6 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks6 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks6 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks7 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks7 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks7 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks8 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks8 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks8 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks9 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks9 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks9 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks10 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks10 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks10 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks11 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks11 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks11 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
	
	#if defined(CONFIG_USEURAM_FOR_DESTVBUFFER) && defined(CONFIG_USEURAM_PACKBYTWOS)
	vsz_kvs = vsz_kvs / 2; // NEWCHANGE.
	depth = depth / 2; // NEWCHANGE.
	#endif 
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP1: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif
		READANDRVCHUNKS_LOOP1B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks12 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks12 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
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
			MEMACCESSP1_RearrangeLayoutV(s, vdata, vdata2); // re-arrange to fit vdata provided in loadedges_random.cpp
			#else 
 vdata2[0] = vdata[0];  vdata2[1] = vdata[1];  vdata2[2] = vdata[2];  vdata2[3] = vdata[3];  vdata2[4] = vdata[4];  vdata2[5] = vdata[5];  vdata2[6] = vdata[6];  vdata2[7] = vdata[7];  vdata2[8] = vdata[8];  vdata2[9] = vdata[9];  vdata2[10] = vdata[10];  vdata2[11] = vdata[11];  vdata2[12] = vdata[12];  vdata2[13] = vdata[13];  vdata2[14] = vdata[14];  vdata2[15] = vdata[15]; 	
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata2[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata2[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks12 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
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
void MEMACCESSP1_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS + 1, NAp, NAp, NAp);
	#endif
	
	unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ _NUM_PARTITIONS = UTILP1_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
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
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){
		for(unsigned int i=0; i<_NUM_PARTITIONS; i++){
			if(globalstatsbuffer[i].key + globalstatsbuffer[i].value >= globalparams.SIZE_KVDRAM){
				cout<<"MEMACCESSP1_readglobalstats 36. ERROR. out of bounds. (globalstatsbuffer["<<i<<"].key("<<globalstatsbuffer[i].key<<") + globalstatsbuffer["<<i<<"].value("<<globalstatsbuffer[i].value<<") >= globalparams.SIZE_KVDRAM("<<globalparams.SIZE_KVDRAM<<")). offset_kvs: "<<offset_kvs<<". printing and exiting..."<<endl; 
				actsutilityobj->printkeyvalues("MEMACCESSP1_readglobalstats 37", (keyvalue_t *)globalstatsbuffer, _NUM_PARTITIONS); 
				exit(EXIT_FAILURE); 
			}
		}
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("readglobalstats.globalstatsbuffer", globalstatsbuffer, _NUM_PARTITIONS); 
	#endif
	// exit(EXIT_SUCCESS); 
	return;
}

void MEMACCESSP1_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS, offset_kvs, globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS);
	#endif
	
	// cout<<"--------------------------------------------------------------- MEMACCESSP1_saveglobalstats -------------------------------------------------------------"<<endl;
	
	unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ _NUM_PARTITIONS = UTILP1_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
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
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSP1_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
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
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSP1_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
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
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSP1_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
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
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSP1_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
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
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSP1_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
			#endif
		}
		else {
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"MEMACCESSP1_saveglobalstats: NOT IMPLEMENTED (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}

		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(VECTOR_SIZE);
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("MEMACCESSP1_saveglobalstats.globalstatsbuffer", globalstatsbuffer, _NUM_PARTITIONS); 
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

tuple_t MEMACCESSP1_getvptrs(uint512_dt * kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
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

unsigned int MEMACCESSP1_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
	keyvalue_t vptr_kv;
	
	uint512_dt V = kvdram[baseoffset_kvs + (offset / VECTOR2_SIZE)];
	unsigned int M = (offset % VECTOR2_SIZE) / 2;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP1_getvptr: baseoffset_kvs: "<<baseoffset_kvs<<", offset: "<<offset<<", baseoffset_kvs + (offset / VECTOR2_SIZE): "<<baseoffset_kvs + (offset / VECTOR2_SIZE)<<endl;
	
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

tuple_t MEMACCESSP1_getvptrs_opt(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID, globalparams_t globalparams){
	#pragma HLS INLINE 
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	beginoffset = beginoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO; // convert-to-appropriate-skip-format
	endoffset = endoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP1_getvptrs_opt: beginoffset: "<<beginoffset<<", endoffset: "<<endoffset<<", VPTR_SHRINK_RATIO: "<<VPTR_SHRINK_RATIO<<endl;
	
	beginvptr = MEMACCESSP1_getvptr(kvdram, baseoffset_kvs, beginoffset);
	endvptr = MEMACCESSP1_getvptr(kvdram, baseoffset_kvs, endoffset);

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

unsigned int MEMACCESSP1_getdata(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int loc){
	keyvalue_t data;
	
	uint512_dt V = kvdram[baseoffset_kvs + loc / VECTOR2_SIZE];
	unsigned int index = (loc % VECTOR2_SIZE) / 2;
	
	#ifdef _WIDEWORD
 if(index == 0){
		data.key = V.range(31, 0); 
		data.value = V.range(63, 32); 
	}
else if(index == 1){
		data.key = V.range(95, 64); 
		data.value = V.range(127, 96); 
	}
else if(index == 2){
		data.key = V.range(159, 128); 
		data.value = V.range(191, 160); 
	}
else if(index == 3){
		data.key = V.range(223, 192); 
		data.value = V.range(255, 224); 
	}
else if(index == 4){
		data.key = V.range(287, 256); 
		data.value = V.range(319, 288); 
	}
else if(index == 5){
		data.key = V.range(351, 320); 
		data.value = V.range(383, 352); 
	}
else if(index == 6){
		data.key = V.range(415, 384); 
		data.value = V.range(447, 416); 
	}
else {
		data.key = V.range(479, 448); 
		data.value = V.range(511, 480); 
	}
	#else 
 if(index == 0){
		data.key = V.data[0].key; 
		data.value = V.data[0].value; 
	}
else if(index == 1){
		data.key = V.data[1].key; 
		data.value = V.data[1].value; 
	}
else if(index == 2){
		data.key = V.data[2].key; 
		data.value = V.data[2].value; 
	}
else if(index == 3){
		data.key = V.data[3].key; 
		data.value = V.data[3].value; 
	}
else if(index == 4){
		data.key = V.data[4].key; 
		data.value = V.data[4].value; 
	}
else if(index == 5){
		data.key = V.data[5].key; 
		data.value = V.data[5].value; 
	}
else if(index == 6){
		data.key = V.data[6].key; 
		data.value = V.data[6].value; 
	}
else {
		data.key = V.data[7].key; 
		data.value = V.data[7].value; 
	}
	#endif
	
	if(loc % 2 == 0){ return data.key; }
	else { return data.value; }
}

void MEMACCESSP1_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset){
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

void MEMACCESSP1_commitkvstats2(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k]; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset  + k].data[0].value = buffer[k]; 
		#endif
	}
	return;
}

void MEMACCESSP1_commitkvstats2(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
		#ifdef _WIDEWORD
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32) = buffer[k].key; 
		#else
		kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value = buffer[k].key; 
		#endif
	}
	return;
}

void MEMACCESSP1_retreievekvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[k] = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32); 
		#else
		buffer[k] = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value; 
		#endif
	}
}

void MEMACCESSP1_retreievekvstats(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
	for(unsigned int k=0; k<size; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		buffer[k].value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].range(63, 32); 
		#else
		buffer[k].value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + offset + k].data[0].value; 
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		if(buffer[k].value>0){ cout<<"MEMACCESSP1_saveglobalstats:: active partition retreieved @ partition "<<k<<", offset: "<<offset + k<<"."<<endl; }	
		#endif
	}
}

void MEMACCESSP1_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
			cout<<"MEMACCESSP1_accumkvstats: NOT IMPLEMENTED (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}

void MEMACCESSP1_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
void MEMACCESSP1_readhelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, unsigned int actsinstance, globalparams_t globalparams){
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
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
		
		pmask[i] = vdata[GraphIter];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	// if(actsinstance == 0){
		cout<< TIMINGRESULTSCOLOR << ">>> readpmasks: printing active vertex partitions: GraphIter: "<<GraphIter<<": ";
		unsigned int num_actvps = 0;
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){
			if(pmask[t] > 0  && t < 16){ cout<<t<<", "; }
			if(pmask[t] > 0){ num_actvps += 1; }
		}
		cout<<" ("<<num_actvps<<" active partitions, "<<size_kvs<<" total partitions)"<< RESET << endl;
	// }
	#endif 
	return;
}









	
#endif 
#ifdef CONFIG_ENABLECLASS_PROCESSEDGES
keyvalue_t PROCESSP1_processvector(bool enx, unsigned int v, unsigned int loc, unsigned int upperlimit, keyvalue_t edata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VSRCDATA_SIZE], globalparams_t globalparams){				
	#pragma HLS INLINE
	keyvalue_t vupdate_dummy;
	bool en = true; if(edata.key == INVALIDDATA || edata.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){
		if(edata.key == INVALIDDATA){ loc = 0; }
		else {
			#ifdef _DEBUGMODE_CHECKS
			if(true){ cout<<"PROCESSP1_processvector::ERROR SEEN @@ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<", upperlimit: "<<upperlimit<<". EXITING... "<<endl; return vupdate_dummy; exit(EXIT_FAILURE); }
			// if(true){ cout<<"PROCESSP1_processvector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edata.key: "<<edata.key<<", edata.value: "<<edata.value<<", v: "<<v<<". EXITING... "<<endl; return vupdate_dummy; exit(EXIT_FAILURE); }			
			#endif 
			loc = 0; }
	}
		
	// read 
	vmdata_t vmdata;
	keyvalue_vbuffer_t bits_vector = vbuffer[loc / VDATA_SHRINK_RATIO];
	if(en == true){ vmdata = MEMCAP1_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
	if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; } vmdata.vmask = 1; // FIXME. REMOVEME.
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edata.key: "<<edata.key<<", edata.value(srcvid): "<<edata.value<<", en: "<<en<<", vid: "<<UTILP1_GETREALVID(edata.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
			
	// process
	value_t res = PROCESSP1_processfunc(vmdata.vdata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	keyvalue_t vupdate; vupdate.key = edata.key; vupdate.value = res;
	
	return vupdate;
}

fetchmessage_t PROCESSP1_read_and_process(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type sourcebaseaddr_kvs, batch_type offset_kvs, batch_type size_kvs, travstate_t ptravstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	// globalparams.ACTSPARAMS_NUM_PARTITIONS replacedby 16
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	keyvalue_t edges_tup[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edges_tup complete
	keyvalue_t res[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	bool enx;
	
	fetchmessage.chunksize_kvs = size_kvs;
	fetchmessage.nextoffset_kvs = -1;
	
	ptravstate.i_kvs = offset_kvs;
	buffer_type chunk_size = UTILP1_getchunksize_kvs(globalparams.ACTSPARAMS_WORKBUFFER_SIZE, ptravstate, 0);
	unsigned int basecollections_0_2_data1 = collections[0][2].data1;
	unsigned int basecollections_0_2_data1b = collections[0][2].data1;
	
	unsigned int upperlimit = sweepparams.source_partition * globalparams.SIZEKVS2_PROCESSEDGESPARTITION;

	ACTIT2_PP0_READ_PROC_ST_MAINLOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		
		// decision flag
		enx = true;
		
		// get dataset
		UTILP1_GetDataset(edges, sourcebaseaddr_kvs + offset_kvs + i, edges_tup);
	
		
		// set flag
		enx = true;
		if((globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE) && (basecollections_0_2_data1 + VECTOR_SIZE >= globalposition.num_active_edges_in_channel)){ enx = false; } // for BFS implementation
		else { basecollections_0_2_data1b += VECTOR_SIZE; }
		
		// process
		res[0] = PROCESSP1_processvector(enx, 0, edges_tup[0].value - upperlimit, upperlimit, edges_tup[0], vbuffer_source[0], globalparams);
		res[1] = PROCESSP1_processvector(enx, 1, edges_tup[1].value - upperlimit, upperlimit, edges_tup[1], vbuffer_source[1], globalparams);
		res[2] = PROCESSP1_processvector(enx, 2, edges_tup[2].value - upperlimit, upperlimit, edges_tup[2], vbuffer_source[2], globalparams);
		res[3] = PROCESSP1_processvector(enx, 3, edges_tup[3].value - upperlimit, upperlimit, edges_tup[3], vbuffer_source[3], globalparams);
		res[4] = PROCESSP1_processvector(enx, 4, edges_tup[4].value - upperlimit, upperlimit, edges_tup[4], vbuffer_source[4], globalparams);
		res[5] = PROCESSP1_processvector(enx, 5, edges_tup[5].value - upperlimit, upperlimit, edges_tup[5], vbuffer_source[5], globalparams);
		res[6] = PROCESSP1_processvector(enx, 6, edges_tup[6].value - upperlimit, upperlimit, edges_tup[6], vbuffer_source[6], globalparams);
		res[7] = PROCESSP1_processvector(enx, 7, edges_tup[7].value - upperlimit, upperlimit, edges_tup[7], vbuffer_source[7], globalparams);
	
		
		// write to buffer 
		if(enx == true){
			destbuffer[0][i] = UTILP1_GETKV(res[0]);
			destbuffer[1][i] = UTILP1_GETKV(res[1]);
			destbuffer[2][i] = UTILP1_GETKV(res[2]);
			destbuffer[3][i] = UTILP1_GETKV(res[3]);
			destbuffer[4][i] = UTILP1_GETKV(res[4]);
			destbuffer[5][i] = UTILP1_GETKV(res[5]);
			destbuffer[6][i] = UTILP1_GETKV(res[6]);
			destbuffer[7][i] = UTILP1_GETKV(res[7]);
		}
		
		#ifdef _DEBUGMODE_STATS
		if(enx == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE); }
		#endif 
	}
	collections[0][2].data1 = basecollections_0_2_data1b;
	fetchmessage.chunksize_kvs = chunk_size; // loadcount; // CRITICAL FIXME
	// cout<<"--- processedges: chunk_size: "<<chunk_size<<", fetchmessage.chunksize_kvs: "<<fetchmessage.chunksize_kvs<<endl;
	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS);
	return fetchmessage;
}

fetchmessage_t PROCESSP1_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	return PROCESSP1_read_and_process(enable, edges, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate,  sweepparams, globalposition, globalparams, collections);
}







	
#endif 
#ifdef CONFIG_ENABLECLASS_PARTITIONUPDATES
// single stage partitioning 
void PARTITIONP1_read_process_partition(bool_type enable, unsigned int mode, unsigned int offset_kvs, unsigned int sourcebaseaddr_kvs, 
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_dest[MAX_NUM_PARTITIONS], 
			travstate_t ptravstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	// globalparams.ACTSPARAMS_NUM_PARTITIONS replacedby 16
	analysis_type analysis_loopcount1 = 512;
	keyvalue_t edges_tup[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=edges_tup complete
	keyvalue_t res[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	keyvalue_capsule_t capsule_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=capsule_kvs complete
	bool enx;
	
	ptravstate.i_kvs = offset_kvs;
	buffer_type chunk_size = UTILP1_getchunksize_kvs(globalparams.ACTSPARAMS_WORKBUFFER_SIZE, ptravstate, 0);
	unsigned int basecollections_0_2_data1 = collections[0][2].data1;
	unsigned int basecollections_0_2_data1b = collections[0][2].data1;
	
	unsigned int modelsz = chunk_size / VECTOR_SIZE; // CRITICAL FIXME.
	for(unsigned int p=0; p<16; p++){ capsule_kvs[p].key = 0; capsule_kvs[p].value = 0; }
	
	unsigned int upperlimit = sweepparams.source_partition * globalparams.SIZEKVS2_PROCESSEDGESPARTITION;

	ACTIT2_PP0_READ_PROC_PART_ST_MAINLOOP1: for(buffer_type i=0; i<chunk_size; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess2(12)::DEBUG CODE 12::1", i, MAX_SOURCEBLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		
		// decision flag
		enx = true;
		
		// get dataset
		UTILP1_GetDataset(kvdram, sourcebaseaddr_kvs + offset_kvs + i, edges_tup);
	
		
		// set flag
		enx = true;
		if((globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE) && (basecollections_0_2_data1 + VECTOR_SIZE >= globalposition.num_active_edges_in_channel)){ enx = false; } // for BFS implementation
		else { basecollections_0_2_data1b += VECTOR_SIZE; }
		
		// process
		res[0] = PROCESSP1_processvector(enx, 0, edges_tup[0].value - upperlimit, upperlimit, edges_tup[0], vbuffer_source[0], globalparams);
		res[1] = PROCESSP1_processvector(enx, 1, edges_tup[1].value - upperlimit, upperlimit, edges_tup[1], vbuffer_source[1], globalparams);
		res[2] = PROCESSP1_processvector(enx, 2, edges_tup[2].value - upperlimit, upperlimit, edges_tup[2], vbuffer_source[2], globalparams);
		res[3] = PROCESSP1_processvector(enx, 3, edges_tup[3].value - upperlimit, upperlimit, edges_tup[3], vbuffer_source[3], globalparams);
		res[4] = PROCESSP1_processvector(enx, 4, edges_tup[4].value - upperlimit, upperlimit, edges_tup[4], vbuffer_source[4], globalparams);
		res[5] = PROCESSP1_processvector(enx, 5, edges_tup[5].value - upperlimit, upperlimit, edges_tup[5], vbuffer_source[5], globalparams);
		res[6] = PROCESSP1_processvector(enx, 6, edges_tup[6].value - upperlimit, upperlimit, edges_tup[6], vbuffer_source[6], globalparams);
		res[7] = PROCESSP1_processvector(enx, 7, edges_tup[7].value - upperlimit, upperlimit, edges_tup[7], vbuffer_source[7], globalparams);
	
		
		// partition
		partition_type p = UTILP1_getpartition(ON, mode, UTILP1_GETKV(res[0]), sweepparams.currentLOP, sweepparams.upperlimit, sweepparams.upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
		
		// write to buffer 
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("read_process_partition 23", capsule_kvs[p].key + capsule_kvs[p].value, BLOCKRAM_SIZE + 1, p, capsule_kvs[p].key, capsule_kvs[p].value);
		#endif
		if(enx == true){
			destbuffer[0][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP1_GETKV(res[0]);
			destbuffer[1][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP1_GETKV(res[1]);
			destbuffer[2][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP1_GETKV(res[2]);
			destbuffer[3][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP1_GETKV(res[3]);
			destbuffer[4][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP1_GETKV(res[4]);
			destbuffer[5][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP1_GETKV(res[5]);
			destbuffer[6][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP1_GETKV(res[6]);
			destbuffer[7][capsule_kvs[p].key + capsule_kvs[p].value] = UTILP1_GETKV(res[7]);
			capsule_kvs[p].value += 1;
		}
		
		#ifdef _DEBUGMODE_STATS
		if(mode==ACTSPROCESSMODE && enx == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE); }
		#endif 
	}
	for(unsigned int p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ 
	#pragma HLS PIPELINE II=1
		capsule_dest[p].key = capsule_kvs[p].key * VECTOR_SIZE; capsule_dest[p].value = capsule_kvs[p].value * VECTOR_SIZE; 
	} // required in key-value mode in reduce
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("read_process_partition.capsule_kvs", (keyvalue_t *)capsule_kvs, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	actsutilityobj->printkeyvalues("read_process_partition.capsule_dest", (keyvalue_t *)capsule_dest, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	collections[0][2].data1 = basecollections_0_2_data1b;
	return;
}

// stage 1 partitioning
void PARTITIONP1_partition_stage1(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){				
	if(enable1 == OFF && enable2 == OFF){ return; }
	analysis_type analysis_dummyfiller = MAX_SRCBUFFER_SIZE - globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	
	value_t emptyslot[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=emptyslot complete
	UTILP1_resetvalues(emptyslot, VECTOR_SIZE, 0);
	
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
		
		keyvalue_t mykeyvalue0 = UTILP1_GETKV(keyvalue0);
		keyvalue_t mykeyvalue1 = UTILP1_GETKV(keyvalue1);
		keyvalue_t mykeyvalue2 = UTILP1_GETKV(keyvalue2);
		keyvalue_t mykeyvalue3 = UTILP1_GETKV(keyvalue3);
		keyvalue_t mykeyvalue4 = UTILP1_GETKV(keyvalue4);
		keyvalue_t mykeyvalue5 = UTILP1_GETKV(keyvalue5);
		keyvalue_t mykeyvalue6 = UTILP1_GETKV(keyvalue6);
		keyvalue_t mykeyvalue7 = UTILP1_GETKV(keyvalue7);
		
		bool_type valid0 = ON;
		if(mykeyvalue0.key != UTILP1_GETK(INVALIDDATA) && mykeyvalue0.value != UTILP1_GETV(INVALIDDATA)){ valid0 = ON; } else { valid0 = OFF; }
		bool_type valid1 = ON;
		if(mykeyvalue1.key != UTILP1_GETK(INVALIDDATA) && mykeyvalue1.value != UTILP1_GETV(INVALIDDATA)){ valid1 = ON; } else { valid1 = OFF; }
		bool_type valid2 = ON;
		if(mykeyvalue2.key != UTILP1_GETK(INVALIDDATA) && mykeyvalue2.value != UTILP1_GETV(INVALIDDATA)){ valid2 = ON; } else { valid2 = OFF; }
		bool_type valid3 = ON;
		if(mykeyvalue3.key != UTILP1_GETK(INVALIDDATA) && mykeyvalue3.value != UTILP1_GETV(INVALIDDATA)){ valid3 = ON; } else { valid3 = OFF; }
		bool_type valid4 = ON;
		if(mykeyvalue4.key != UTILP1_GETK(INVALIDDATA) && mykeyvalue4.value != UTILP1_GETV(INVALIDDATA)){ valid4 = ON; } else { valid4 = OFF; }
		bool_type valid5 = ON;
		if(mykeyvalue5.key != UTILP1_GETK(INVALIDDATA) && mykeyvalue5.value != UTILP1_GETV(INVALIDDATA)){ valid5 = ON; } else { valid5 = OFF; }
		bool_type valid6 = ON;
		if(mykeyvalue6.key != UTILP1_GETK(INVALIDDATA) && mykeyvalue6.value != UTILP1_GETV(INVALIDDATA)){ valid6 = ON; } else { valid6 = OFF; }
		bool_type valid7 = ON;
		if(mykeyvalue7.key != UTILP1_GETK(INVALIDDATA) && mykeyvalue7.value != UTILP1_GETV(INVALIDDATA)){ valid7 = ON; } else { valid7 = OFF; }
		
		partition_type p0 = 0;
		if(valid0 == ON){ p0 = UTILP1_getpartition(ON, mode, keyvalue0, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p1 = 0;
		if(valid1 == ON){ p1 = UTILP1_getpartition(ON, mode, keyvalue1, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p2 = 0;
		if(valid2 == ON){ p2 = UTILP1_getpartition(ON, mode, keyvalue2, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p3 = 0;
		if(valid3 == ON){ p3 = UTILP1_getpartition(ON, mode, keyvalue3, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p4 = 0;
		if(valid4 == ON){ p4 = UTILP1_getpartition(ON, mode, keyvalue4, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p5 = 0;
		if(valid5 == ON){ p5 = UTILP1_getpartition(ON, mode, keyvalue5, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p6 = 0;
		if(valid6 == ON){ p6 = UTILP1_getpartition(ON, mode, keyvalue6, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		partition_type p7 = 0;
		if(valid7 == ON){ p7 = UTILP1_getpartition(ON, mode, keyvalue7, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH); }
		
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
		keyvalue_buffer_t dummykv = UTILP1_GETKV(mydummykv);
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
	
	UTILP1_calculatemanyunallignedoffsets(localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0, 0);

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

void PARTITIONP1_partition_stage1_trad(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){				
	if(enable1 == OFF && enable2 == OFF){ return; }
	analysis_type analysis_loop1 = globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	analysis_type analysis_dummyfiller = MAX_SRCBUFFER_SIZE - globalparams.ACTSPARAMS_WORKBUFFER_SIZE;
	
	unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ _NUM_PARTITIONS = UTILP1_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
	#endif 
	
	value_t emptyslot[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=emptyslot complete
	UTILP1_resetvalues(emptyslot, VECTOR_SIZE, 0);
	
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

void PARTITIONP1_partitionbase_stage1(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], keyvalue_buffer_t destbuffer[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[VECTOR_SIZE][MAX_NUM_PARTITIONS], step_type currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type size_kvs, buffer_type cutoffs[VECTOR_SIZE], globalparams_t globalparams){
	PARTITIONP1_partition_stage1(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
	// PARTITIONP1_partition_stage1_trad(enable1, enable2, mode, sourcebuffer, destbuffer, localcapsule, currentLOP, sweepparams, globalposition, size_kvs, cutoffs, globalparams);
	return;
}

// stage 2 partitioning
void PARTITIONP1_partition_stage2(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){		
	if(enable1 == OFF){ return; }
	
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
	UTILP1_calculateoffsets(capsule_so8, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	UTILP1_resetvalues(capsule_so8, globalparams.ACTSPARAMS_NUM_PARTITIONS, 0);
	
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
	// cout<<"------------------- ACTSP1_actspipeline: maxcutoff: "<<maxcutoff<<endl;
	
	keyvalue_t mydummykv;
	mydummykv.key = 0;
	mydummykv.value = UTILP1_GETV(INVALIDDATA);
	keyvalue_buffer_t dummykv = UTILP1_GETKV(mydummykv);
	
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
			for(unsigned int m=0; m<4; m++){ cout<<"kvA0["<<m<<"].key: "<<UTILP1_GETKV(kvA0[m]).key<<", kvA0["<<m<<"].value: "<<UTILP1_GETKV(kvA0[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA2["<<m<<"].key: "<<UTILP1_GETKV(kvA2[m]).key<<", kvA2["<<m<<"].value: "<<UTILP1_GETKV(kvA2[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA4["<<m<<"].key: "<<UTILP1_GETKV(kvA4[m]).key<<", kvA4["<<m<<"].value: "<<UTILP1_GETKV(kvA4[m]).value<<endl; }
			for(unsigned int m=0; m<4; m++){ cout<<"kvA6["<<m<<"].key: "<<UTILP1_GETKV(kvA6[m]).key<<", kvA6["<<m<<"].value: "<<UTILP1_GETKV(kvA6[m]).value<<endl; }
			#endif
			
			#if defined(_DEBUGMODE_CHECKS2) && defined(_DEBUGMODE_PARTITIONCHECKS)
			actsutilityobj->checkn(enablebufferA, "kvA0", kvA0, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA2", kvA2, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA4", kvA4, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			actsutilityobj->checkn(enablebufferA, "kvA6", kvA6, currentLOP, upperlimit, globalparams.POW_BATCHRANGE, 4);
			#endif
			
			partition_type pA0 = UTILP1_getpartition(ON, mode, kvA0[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA2 = UTILP1_getpartition(ON, mode, kvA2[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA4 = UTILP1_getpartition(ON, mode, kvA4[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			partition_type pA6 = UTILP1_getpartition(ON, mode, kvA6[0], currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams.ACTSPARAMS_TREEDEPTH);
			
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
			if(UTILP1_GETKV(kvA0[0]).value != UTILP1_GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; }
			// if(UTILP1_GETKV(kvA0[0]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA0[1]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA0[2]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA0[3]).value != UTILP1_GETV(INVALIDDATA)){ tempbufferDcapsule[pA0] += 4; }		
			
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
			if(UTILP1_GETKV(kvA2[0]).value != UTILP1_GETV(INVALIDDATA)){ tempbufferDcapsule[pA2] += 4; }
			// if(UTILP1_GETKV(kvA2[0]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA2[1]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA2[2]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA2[3]).value != UTILP1_GETV(INVALIDDATA)){ tempbufferDcapsule[pA2] += 4; }
			
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
			if(UTILP1_GETKV(kvA4[0]).value != UTILP1_GETV(INVALIDDATA)){ tempbufferDcapsule[pA4] += 4; }
			// if(UTILP1_GETKV(kvA4[1]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA4[1]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA4[2]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA4[3]).value != UTILP1_GETV(INVALIDDATA)){ tempbufferDcapsule[pA4] += 4; }
			
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
			if(UTILP1_GETKV(kvA6[0]).value != UTILP1_GETV(INVALIDDATA)){ tempbufferDcapsule[pA6] += 4; }
			// if(UTILP1_GETKV(kvA6[0]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA6[1]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA6[2]).value != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kvA6[3]).value != UTILP1_GETV(INVALIDDATA)){ tempbufferDcapsule[pA6] += 4; }
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
		mydummykv.value = UTILP1_GETV(INVALIDDATA);
		keyvalue_buffer_t dummykv = UTILP1_GETKV(mydummykv);
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

void PARTITIONP1_partition_stage2_trad(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){
	if(enable1 == OFF && enable2 == OFF){ return; }
	
	buffer_type chunk_size = cutoffs[0];
	unsigned int upperlimit = sweepparams.upperlimit;
	unsigned int upperpartition = sweepparams.upperpartition;
	unsigned int num_partitions = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	
	if(globalparams.EVALUATION_ACTS_VERYBARE == ON){ return; }
	#ifndef CONFIG_ACTS_PERFORMANCEOFALGORITHM
	if(globalparams.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON && globalparams.ACTSPARAMS_TREEDEPTH > 1){ num_partitions = UTILP1_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(globalparams.ACTSPARAMS_TREEDEPTH); }
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
			
			partition_type p = UTILP1_getpartition2(ON, mode, kv, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams);
			if(UTILP1_GETKV(kv).key != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kv).value != UTILP1_GETV(INVALIDDATA)){ capsule_so8[p].value += 1; }
		}
	}
	
	UTILP1_calculateoffsets(capsule_so8, num_partitions);
	UTILP1_resetvalues(capsule_so8, num_partitions, 0);
	
	TRADITIONALPARTITIONKEYVALUES_LOOP2: for(buffer_type i=0; i<chunk_size; i++){
		TRADITIONALPARTITIONKEYVALUES_LOOP2B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=2
			keyvalue_buffer_t kv = buffer_setof1[v][i];
			// cout<<"--- priorpartitionkeyvalues: kv.key: "<<kv.key<<endl; // REMOVEME.
			partition_type p = UTILP1_getpartition2(ON, mode, kv, currentLOP, upperlimit, upperpartition, globalparams.POW_BATCHRANGE, globalparams);
			buffer_type pos = capsule_so8[p].key + capsule_so8[p].value;
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("TRADactspipeline (23)", pos / VECTOR_SIZE, MAX_DESTBLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(UTILP1_GETKV(kv).key != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kv).value != UTILP1_GETV(INVALIDDATA)){ buffer_setof8[pos % VECTOR_SIZE][pos / VECTOR_SIZE] = kv; } // NOTE: could this be the cause of slight imperfection in results?
			if(UTILP1_GETKV(kv).key != UTILP1_GETV(INVALIDDATA) && UTILP1_GETKV(kv).value != UTILP1_GETV(INVALIDDATA)){ capsule_so8[p].value += 1; }
		}
	}
	
	TRADITIONALPARTITIONKEYVALUES_LOOP3: for(partition_type p=0; p<num_partitions; p++){
		keyvalue_t mydummykv;
		mydummykv.key = p;
		mydummykv.value = UTILP1_GETV(INVALIDDATA);
		keyvalue_buffer_t dummykv = UTILP1_GETKV(mydummykv);
	
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

void PARTITIONP1_partitionbase_stage2(bool_type enable1, bool_type enable2, unsigned int mode, keyvalue_buffer_t buffer_setof1[VECTOR_SIZE][BLOCKRAM_SIZE], keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], 
						keyvalue_buffer_t buffer_setof8[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS],
							unsigned int currentLOP, sweepparams_t sweepparams, globalposition_t globalposition, buffer_type cutoffs[VECTOR_SIZE], batch_type shiftcount, globalparams_t globalparams){
	PARTITIONP1_partition_stage2(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);		
	// PARTITIONP1_partition_stage2_trad(enable1, enable2, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, currentLOP, sweepparams, globalposition, cutoffs, shiftcount, globalparams);
}


	
#endif 
#ifdef CONFIG_ENABLECLASS_REDUCEUPDATES
void REDUCEP1_reducevector(bool enx, unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], buffer_type destoffset, unsigned int upperlimit, unsigned int * memory, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS INLINE
	// #pragma HLS PIPELINE II=3
	
	keyvalue_t mykeyvalue = UTILP1_GETKV(kvdata);
	vertex_t loc = ((mykeyvalue.key - upperlimit) - col) >> globalparams.ACTSPARAMS_POW_PARTITIONS;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"REDUCEP1_reducevector:: col: "<<col<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", mykeyvalue.value: "<<mykeyvalue.value<<endl;
	#endif
	
	bool en = true;
	if(mykeyvalue.key == UTILP1_GETK(INVALIDDATA) || mykeyvalue.value == UTILP1_GETV(INVALIDDATA) || mykeyvalue.key == MAXVDATA || mykeyvalue.value == MAXVDATA){ en = false; }
	if(mykeyvalue.key == 0 && mykeyvalue.value == 0){ en = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true){ cout<<"$$$ REDUCE VECTOR:: REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP1_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<", partition: "<<UTILP1_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID) / (globalparams.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE)<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<endl; }															
	#endif 
	
	if(loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_SHRINK_RATIO) && en == true){
		#ifdef ENABLE_PERFECTACCURACY
		if(true){ cout<<"REDUCEP1_reducevector::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }
		
	vmdata_t vmdata; vmdata.vmask = 0;
	vmdata_t vmdata2; vmdata2.vmask = 0;
	unsigned int bufferoffset_kvs = 0; 
	keyvalue_vbuffer_t bits_vector = 0;
	keyvalue_vbuffer_t bits2_vector = 0;
	
	#ifdef CONFIG_USEURAM_PACKBYTWOS
	loc = loc / 2; 
	#endif 
	if(en == true){ if(loc == *memory){ loc = (loc + 1) % 8; } *memory = loc; } // CRITICAL FIXME.
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 113::1", bufferoffset_kvs + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VDESTDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
	#endif
	
	/** // read-special (cf)
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == CF){ bits2_vector = vbuffer[bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA2 + loc]; } // CUSTOMPROVISION #ifdef CF_ALGORITHM 
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == CF){ vmdata2 = MEMCAP1_READFROMBUFFER_VDATAWITHVMASK(bits2_vector); }
	
	// read-special (hits)
	unsigned int src_dest_edge = MEMCAP1_READEDIR(kvdata.key);
	if(en == true && globalparams.ALGORITHMINFO_GRAPHALGORITHMID == HITS){ if(src_dest_edge == 0){ loc = loc*2; } else{ loc = loc*2 + 1; }} // CUSTOMPROVISION #ifdef HITS_ALGORITHM 
	#ifdef _DEBUGMODE_CHECKS3
	if(en == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 114::1", bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1 + (loc / VDATA_SHRINK_RATIO), MAX_BLOCKRAM_VDESTDATA_SIZE, bufferoffset_kvs, loc, VDATA_SHRINK_RATIO); }
	#endif */
	
	// read (general)
	if(en == true){ bits_vector = vbuffer[bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1 + (loc / VDATA_SHRINK_RATIO)]; }
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true){ vmdata = MEMCAP1_READFROMBUFFER_VDATAWITHVMASK2(loc, bits_vector); }
			#else 
				if(en == true){ vmdata = MEMCAP1_READFROMBUFFER_VDATAWITHVMASK(bits_vector); }
					#endif
					
	// reduce 
	value_t new_vprop = REDUCEP1_reducefunc(vmdata.vdata, vmdata2.vdata, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	if(en == true && new_vprop != vmdata.vdata){ vmdata.vmask = 1; } else { vmdata.vmask = 0; } // NEWCHANGE.
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true && new_vprop != vmdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE MASK SEEN AT: loc: "<<loc<<", vmdata.vdata: "<<vmdata.vdata<<", vmdata.vmask: "<<vmdata.vmask<<endl; }
	if(en == true && new_vprop != vmdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP1_GETREALVID(mykeyvalue.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
	
	// write
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true){ MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK2(loc, vbuffer, bufferoffset_kvs + globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, vmdata.vmask); }
			#else 
				if(en == true){ MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK(loc, vbuffer, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, vmdata.vmask); }
					#endif
 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(globalparams.ACTSPARAMS_INSTID, 1);
	if(en == true){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(globalparams.ACTSPARAMS_INSTID, 1); }
	#endif
	return;
}

void REDUCEP1_GetXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE], unsigned int basedepth){
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

void REDUCEP1_RearrangeLayoutV(unsigned int s, keyvalue_buffer_t vdata[VECTOR_SIZE], keyvalue_buffer_t vdata2[VECTOR_SIZE]){
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

void REDUCEP1_RearrangeLayoutEn(unsigned int s, bool en[VECTOR_SIZE], bool en2[VECTOR_SIZE]){
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

void REDUCEP1_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	// globalparams.ACTSPARAMS_NUM_PARTITIONS replacedby 16
	if(enable == OFF){ return; }
	
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
	unsigned int memory[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=memory complete
	
	keyvalue_t invalid_dataa; invalid_dataa.key = INVALIDDATA; invalid_dataa.value = INVALIDDATA;
	keyvalue_buffer_t invalid_data = UTILP1_GETKV(invalid_dataa);

	unsigned int chunk_size = 0; for(partition_type p=0; p<16; p++){ chunk_size += localcapsule[p].value / VECTOR_SIZE; } // CRITICAL FIXME.
	unsigned int modelsz = chunk_size / 16;
	for(unsigned int p=0; p<16; p++){ localcapsule[p].key = (p * modelsz) * VECTOR_SIZE; localcapsule[p].value = modelsz * VECTOR_SIZE; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	
	buffer_type maxsize_kvs[2]; maxsize_kvs[0] = 0; maxsize_kvs[1] = 0;
	unsigned int _poff = 0;
	REDUCEBUFFERPARTITIONS_LOOP1: for(unsigned int cid=0; cid<2; cid++){ // NB: is accessed 8kv per time, so we need 2 fetches'
		REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<16/2; p++){ 
		#pragma HLS PIPELINE II=1
			unsigned int ssize_kvs = localcapsule[_poff + p].value / VECTOR_SIZE;
			if(maxsize_kvs[cid] < ssize_kvs){ maxsize_kvs[cid] = ssize_kvs; }
		}
		_poff += 16/2;
	}
	buffer_type height_kvs = (localcapsule[16-1].key + localcapsule[16-1].value) / VECTOR_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"reduceupdates2: maxsize_kvs[0]: "<<maxsize_kvs[0]<<", maxsize_kvs[1]: "<<maxsize_kvs[1]<<", height_kvs: "<<height_kvs<<endl;
	#endif
		
	for(partition_type p=0; p<16; p++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"reduceupdates2: localcapsule["<<p<<"].key: "<<localcapsule[p].key<<", localcapsule["<<p<<"].value: "<<localcapsule[p].value<<endl;
		#endif
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	}
	
	REDUCEBUFFERPARTITIONS_MAINLOOP2: for(buffer_type capsule_offset=0; capsule_offset<16; capsule_offset+=VECTOR_SIZE){
		unsigned int mmaxsz_kvs = maxsize_kvs[capsule_offset / VECTOR_SIZE];	
		REDUCEBUFFERPARTITIONS_MAINLOOP2B: for(unsigned int r=0; r<VECTOR_SIZE; r++){
			REDUCEBUFFERPARTITIONS_MAINLOOP2C: for(buffer_type i=0; i<mmaxsz_kvs; i++){ 
			#pragma HLS PIPELINE II=2
			#pragma HLS dependence variable=vbuffer inter false
				// get layout
				REDUCEP1_GetXYLayoutV(r, depths, 0);
			
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
				REDUCEP1_RearrangeLayoutV(r, kvdata, kvdata2);
				REDUCEP1_RearrangeLayoutEn(r, enx, enx2);
			
				// reduce 
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP1_reducevector(enx2[0], 0, kvdata2[0], vbuffer[capsule_offset + 0], 0, sweepparams.upperlimit, &memory[0], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP1_reducevector(enx2[0], 0, kvdata2[0], vbuffer[0 + 0], 0, sweepparams.upperlimit, &memory[0], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP1_reducevector(enx2[1], 1, kvdata2[1], vbuffer[capsule_offset + 1], 0, sweepparams.upperlimit, &memory[1], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP1_reducevector(enx2[1], 1, kvdata2[1], vbuffer[0 + 1], 0, sweepparams.upperlimit, &memory[1], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP1_reducevector(enx2[2], 2, kvdata2[2], vbuffer[capsule_offset + 2], 0, sweepparams.upperlimit, &memory[2], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP1_reducevector(enx2[2], 2, kvdata2[2], vbuffer[0 + 2], 0, sweepparams.upperlimit, &memory[2], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP1_reducevector(enx2[3], 3, kvdata2[3], vbuffer[capsule_offset + 3], 0, sweepparams.upperlimit, &memory[3], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP1_reducevector(enx2[3], 3, kvdata2[3], vbuffer[0 + 3], 0, sweepparams.upperlimit, &memory[3], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP1_reducevector(enx2[4], 4, kvdata2[4], vbuffer[capsule_offset + 4], 0, sweepparams.upperlimit, &memory[4], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP1_reducevector(enx2[4], 4, kvdata2[4], vbuffer[0 + 4], 0, sweepparams.upperlimit, &memory[4], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP1_reducevector(enx2[5], 5, kvdata2[5], vbuffer[capsule_offset + 5], 0, sweepparams.upperlimit, &memory[5], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP1_reducevector(enx2[5], 5, kvdata2[5], vbuffer[0 + 5], 0, sweepparams.upperlimit, &memory[5], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP1_reducevector(enx2[6], 6, kvdata2[6], vbuffer[capsule_offset + 6], 0, sweepparams.upperlimit, &memory[6], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP1_reducevector(enx2[6], 6, kvdata2[6], vbuffer[0 + 6], 0, sweepparams.upperlimit, &memory[6], sweepparams, globalparams);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
				// REDUCEP1_reducevector(enx2[7], 7, kvdata2[7], vbuffer[capsule_offset + 7], 0, sweepparams.upperlimit, &memory[7], sweepparams, globalparams); // CRITICAL FIXME.		
				REDUCEP1_reducevector(enx2[7], 7, kvdata2[7], vbuffer[0 + 7], 0, sweepparams.upperlimit, &memory[7], sweepparams, globalparams);
			}
		}
	}
	// exit(EXIT_SUCCESS);
	return;
}

void REDUCEP1_reduceandbuffer2(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = (MAX_DESTBLOCKRAM_SIZE / (MAX_NUM_PARTITIONS / 2));

	keyvalue_buffer_t kvdata0;
	keyvalue_buffer_t kvdata1;
	keyvalue_buffer_t kvdata2;
	keyvalue_buffer_t kvdata3;
	keyvalue_buffer_t kvdata4;
	keyvalue_buffer_t kvdata5;
	keyvalue_buffer_t kvdata6;
	keyvalue_buffer_t kvdata7;
	keyvalue_buffer_t kvdata8;
	keyvalue_buffer_t kvdata9;
	keyvalue_buffer_t kvdata10;
	keyvalue_buffer_t kvdata11;
	keyvalue_buffer_t kvdata12;
	keyvalue_buffer_t kvdata13;
	keyvalue_buffer_t kvdata14;
	keyvalue_buffer_t kvdata15;
	buffer_type bramoffset_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=bramoffset_kvs complete
	buffer_type size_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=size_kvs complete
	unsigned int upperlimits[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=upperlimits complete
	unsigned int memory[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=memory complete
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("reduceandbuffer.localcapsule", (keyvalue_t *)localcapsule, globalparams.ACTSPARAMS_NUM_PARTITIONS);
	#endif
	
	REDUCEBUFFERPARTITIONS2_LOOP1: for(unsigned int it=0; it<globalparams.ACTSPARAMS_NUM_PARTITIONS; it+=globalparams.ACTSPARAMS_NUM_PARTITIONS/2){
		buffer_type maxsize_kvs = 0;
		REDUCEBUFFERPARTITIONS2_LOOP1B: for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			bramoffset_kvs[p] = localcapsule[it+p].key / VECTOR_SIZE;
			size_kvs[p] = localcapsule[it+p].value / VECTOR_SIZE;
			if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
		}
		
		REDUCEBUFFERPARTITIONS2_MAINLOOP1D: for(buffer_type i=0; i<maxsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#pragma HLS PIPELINE II=48 // 16, 32*, 48, FIXME.
			for(vector_type v=0; v<VECTOR_SIZE; v++){
				kvdata0 = buffer[v][bramoffset_kvs[0] + i]; 	
				kvdata1 = buffer[v][bramoffset_kvs[1] + i]; 	
				kvdata2 = buffer[v][bramoffset_kvs[2] + i]; 	
				kvdata3 = buffer[v][bramoffset_kvs[3] + i]; 	
				kvdata4 = buffer[v][bramoffset_kvs[4] + i]; 	
				kvdata5 = buffer[v][bramoffset_kvs[5] + i]; 	
				kvdata6 = buffer[v][bramoffset_kvs[6] + i]; 	
				kvdata7 = buffer[v][bramoffset_kvs[7] + i]; 	
				
	
				// if(i< size_kvs[0]){ REDUCEP1_reducevector(ON, it+0, kvdata0, vbuffer[it+0], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[0]){ REDUCEP1_reducevector(ON, 0+0, kvdata0, vbuffer[0+0], 0, sweepparams.upperlimit, &memory[0], sweepparams, globalparams); }
	
				// if(i< size_kvs[1]){ REDUCEP1_reducevector(ON, it+1, kvdata1, vbuffer[it+1], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[1]){ REDUCEP1_reducevector(ON, 0+1, kvdata1, vbuffer[0+1], 0, sweepparams.upperlimit, &memory[1], sweepparams, globalparams); }
	
				// if(i< size_kvs[2]){ REDUCEP1_reducevector(ON, it+2, kvdata2, vbuffer[it+2], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[2]){ REDUCEP1_reducevector(ON, 0+2, kvdata2, vbuffer[0+2], 0, sweepparams.upperlimit, &memory[2], sweepparams, globalparams); }
	
				// if(i< size_kvs[3]){ REDUCEP1_reducevector(ON, it+3, kvdata3, vbuffer[it+3], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[3]){ REDUCEP1_reducevector(ON, 0+3, kvdata3, vbuffer[0+3], 0, sweepparams.upperlimit, &memory[3], sweepparams, globalparams); }
	
				// if(i< size_kvs[4]){ REDUCEP1_reducevector(ON, it+4, kvdata4, vbuffer[it+4], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[4]){ REDUCEP1_reducevector(ON, 0+4, kvdata4, vbuffer[0+4], 0, sweepparams.upperlimit, &memory[4], sweepparams, globalparams); }
	
				// if(i< size_kvs[5]){ REDUCEP1_reducevector(ON, it+5, kvdata5, vbuffer[it+5], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[5]){ REDUCEP1_reducevector(ON, 0+5, kvdata5, vbuffer[0+5], 0, sweepparams.upperlimit, &memory[5], sweepparams, globalparams); }
	
				// if(i< size_kvs[6]){ REDUCEP1_reducevector(ON, it+6, kvdata6, vbuffer[it+6], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[6]){ REDUCEP1_reducevector(ON, 0+6, kvdata6, vbuffer[0+6], 0, sweepparams.upperlimit, &memory[6], sweepparams, globalparams); }
	
				// if(i< size_kvs[7]){ REDUCEP1_reducevector(ON, it+7, kvdata7, vbuffer[it+7], 0, sweepparams.upperlimit, sweepparams, globalparams); } // CRITICAL FIXME.
				if(i< size_kvs[7]){ REDUCEP1_reducevector(ON, 0+7, kvdata7, vbuffer[0+7], 0, sweepparams.upperlimit, &memory[7], sweepparams, globalparams); }
	
			}
		}
	}
	return;
}

void REDUCEP1_reduceandbuffer3(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	if(enable == OFF){ return; }
	
	keyvalue_capsule_t localcapsule_kvs[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=localcapsule_kvs complete
	unsigned int totalsum_values_kvs = 0;
	
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"ACTSreduceandbuffer3: localcapsule["<<p<<"].key: "<<localcapsule[p].key<<", localcapsule["<<p<<"].value: "<<localcapsule[p].value<<endl;
		#endif
		localcapsule_kvs[p].key = localcapsule[p].key / VECTOR_SIZE;
		localcapsule_kvs[p].value = (localcapsule[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	}
	for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ totalsum_values_kvs += localcapsule_kvs[p].value; }
	// cout<<"ACTSreduceandbuffer3: totalsum_values_kvs: "<<totalsum_values_kvs<<endl;
	// exit(EXIT_SUCCESS); //
	
	REDUCEBUFFERPARTITIONS3_MAINLOOP2C: for(buffer_type i=0; i<totalsum_values_kvs; i++){
	#pragma HLS PIPELINE II=2
		vbuffer[0][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP1_GETKV(buffer[0][i]).value;
		vbuffer[1][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP1_GETKV(buffer[1][i]).value;
		vbuffer[2][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP1_GETKV(buffer[2][i]).value;
		vbuffer[3][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP1_GETKV(buffer[3][i]).value;
		vbuffer[4][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP1_GETKV(buffer[4][i]).value;
		vbuffer[5][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP1_GETKV(buffer[5][i]).value;
		vbuffer[6][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP1_GETKV(buffer[6][i]).value;
		vbuffer[7][i % MAX_BLOCKRAM_VDESTDATA_SIZE] = UTILP1_GETKV(buffer[7][i]).value;
		// cout<<"reduce seen......"<<endl;
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsreduced(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE);
		actsutilityobj->globalstats_reduce_countvalidkvsreduced(globalparams.ACTSPARAMS_INSTID, VECTOR_SIZE);
		#endif
	}
	return;
}

void REDUCEP1_TRADreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){				
	if(enable == OFF){ return; }
	
	keyvalue_buffer_t kvdata[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata complete
	keyvalue_buffer_t kvdata2[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=kvdata2 complete
	bool enx[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enx complete
	unsigned int memory[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=memory complete
	
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
			REDUCEP1_reducevector(ON, p, kvdata, vbuffer[p], 0, sweepparams.upperlimit, &memory[p], sweepparams, globalparams);
		}
	}

	// actsutilityobj->printglobalvars();
	// exit(EXIT_SUCCESS); ////
	return;
}

void REDUCEP1_reduceandbuffer_base(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	// return REDUCEP1_reduceandbuffer(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
	// return REDUCEP1_reduceandbuffer2(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
	return REDUCEP1_reduceandbuffer3(enable, buffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
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

void TradGPP1(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"TradGP:: running traditional sssp... "<<endl;
	#endif 
	
	globalparams_t globalparamsm = UTILP1_getglobalparams(mdram, 0);
	globalparams_t globalparamsvA = UTILP1_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP1_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP1_getglobalparams(vdramC, 0);
	
	unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE];

	unsigned int GraphIter = globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID;
	unsigned int actvvcount_currit = UTILP1_GetSecondData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID); // globalparamsm.SIZE_ACTIVEVERTICES;
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
			actvvids[i] = UTILP1_GetData(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + actvvs_currentit_basekvs, i); 
		}
		
		// read & buffer (vertex ptrs)
		TRADGP_LOOP3: for(unsigned int t=0; t<chunk_size; t++){
		// #pragma HLS PIPELINE
			unsigned int srcvid = actvvids[t];
			edge_t vptr_begin = UTILP1_GetData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, srcvid); 
			edge_t vptr_end = UTILP1_GetData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, srcvid + 1); 
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
				unsigned int dstvid = UTILP1_GetData(mdram, globalparamsm.BASEOFFSETKVS_EDGESDATA, t);
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				cout<<"TradGP:: EDGE: srcvid: "<<srcvid<<", dstvid: "<<dstvid<<endl;
				#endif 
				
				unsigned int s = dstvid % NUM_PEs;
				unsigned int ldstvid = (dstvid - s) / NUM_PEs; 
				unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
				
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 21", index, TRADGP_BUFFER_BIGSIZE, dstvid, tempbuffer_vptr[i].key, tempbuffer_vptr[i].value);
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 22", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + (ldstvid / VDATA_SHRINK_RATIO), globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, index, dstvid, offset_kvs);
				#endif
				independent_edge[index].dstp = UTILP1_GetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO));
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
			vmdata.vdata = UTILP1_SWREADBITSFROM_UINTV(independent_edge[edge].dstp, offsetof_vdata, SIZEOF_VDATA);
			vmdata.vmask = UTILP1_SWREADBITSFROM_UINTV(independent_edge[edge].dstp, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK);
			#else
			vmdata.vdata = MEMCAP1_READVDATA(independent_edge[edge].dstp); 
			vmdata.vmask = MEMCAP1_READVMASK(independent_edge[edge].dstp); 
			#endif 
			
			// reduce 
			value_t new_vprop = REDUCEP1_reducefunc(vmdata.vdata, NAp, NAp, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA.ALGORITHMINFO_GRAPHALGORITHMID);
			if(new_vprop != vmdata.vdata){ vmdata.vmask = 1; } else { vmdata.vmask = 0; } // NEWCHANGE.
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(new_vprop != vmdata.vdata){ cout<<">>> TRADGP::REDUCE VECTOR(1):: ACTIVE REDUCE SEEN @ srcvid: "<<srcvid<<", dstvid: "<<dstvid<<endl; }
			#endif
			
			// write 
			if(vmdata.vmask == 1){
				#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) || defined(CONFIG_GOLDENKERNEL)
				unsigned int offsetof_vdata = (ldstvid % VDATA_SHRINK_RATIO);
				UTILP1_SWWRITEBITSTO_UINTV(&independent_edge[edge].dstp, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK, vmdata.vmask);
				#else 
				MEMCAP1_WRITEVMASK(&independent_edge[edge].dstp, vmdata.vmask); 
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
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 28", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + (ldstvid / VDATA_SHRINK_RATIO), globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, index, dstvid, offset_kvs);
				#endif
				UTILP1_SetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				UTILP1_SetData(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				UTILP1_SetData(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				
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
	
	UTILP1_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP1_SetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP1_SetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP1_SetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	
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

unsigned int MERGEP1_actvpstatsoffset(globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ _offset += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }} else { _offset = 1; }
	return _offset;
}

void MERGEP1_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR_SIZE; v++){	
		unsigned int K = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].key;
		unsigned int V = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].value;
		if(MEMCAP1_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }
		if(MEMCAP1_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }	
	}
	#endif 
	return;
}

void MERGEP1_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << "mergeVs:: merging vertices (1)..."<< RESET <<endl; 
	#endif
	
	globalparams_t globalparams = UTILP1_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP1_getglobalparams(vdram, 0);
	bool pass3 = false; // false; // true;
	
	// retreive active partitions  (B)
	pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE];
	MEMACCESSP1_readhelperstats(kvdram, pmask, globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1, NAp, globalparams);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"mergeVs:: printing active partitions..."<<endl;
	for(unsigned int p=0; p<globalparams.NUM_REDUCEPARTITIONS; p++){ if(pmask[p] > 0){ cout<<"pmask["<<p<<"] : "<<pmask[p]<<endl; }}
	#endif 

	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 	
	#endif 
	unsigned int i = globalparams.ACTSPARAMS_INSTID % NUMCOMPUTEUNITS_SLR2;
	unsigned int voffset_kvs2 = i * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	MERGEP1_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"mergeVs:: vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<", num partitions: "<<globalparams.NUM_REDUCEPARTITIONS<<"]: [lbasevoffset_kvs2: "<<lbasevoffset_kvs2<<", voffset_kvs2: "<<voffset_kvs2<<", voffseti_kvs2: "<<voffseti_kvs2<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP1_mergeVs: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(pmask[partition] > 0 || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE || pass3 == true){ // REMOVEME 'true'
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"mergeVs:: Vertices :: [instance "<<globalparams.ACTSPARAMS_INSTID<<", partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP1_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("mergeVs---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA); // globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE
				actsutilityobj->checkoutofbounds("mergeVs^^^: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); // globalparamsv.SIZE_DESTVERTICESDATA / VECTOR2_SIZE
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				MERGEP1_print_active_masks(vdram, globalparamsv, lbasevoffset_kvs2 + voffset_kvs2 + k);	
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

void MERGEP1_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << "exchangeVs:: exchanging vertices across different SLRs..." << RESET <<endl; 
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
	unsigned int numactvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=numactvvs complete
	unsigned int local_actvv[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv complete
	unsigned int local_actvv2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=local_actvv2 complete
	unsigned int global_actvvs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=global_actvvs complete
	#endif 	
	value_t vdatas[VECTOR2_SIZE];
	
	unsigned int reduce_partition = 0;
	unsigned int partition_offset = 0;
	
	globalparams_t globalparamsvA = UTILP1_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP1_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP1_getglobalparams(vdramC, 0);
	globalparams_t globalparamsm = UTILP1_getglobalparams(mdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP1_actvpstatsoffset(globalparamsvA);
	
	UTILP1_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);

	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR2 = NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR1 = NUMCOMPUTEUNITS_SLR1 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR0 = NUMCOMPUTEUNITS_SLR0 * globalparamsvA.NUM_REDUCEPARTITIONS;
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	
	uint32_type pmaski = 0;
	unsigned int totalvsynchronized = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif
	// active reduce partitions
	MERGEP1_EXCHANGEC_LOOP1A: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR2; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP1_exchange: ERROR 1s20", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif 
		unsigned int tempdata = UTILP1_GetSecondData(vdramA, globalparamsvA.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer0[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] = 1; }
	}
	
	MERGEP1_EXCHANGEC_LOOP1B: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR1; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP1_exchange: ERROR 1s21", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif
		unsigned int tempdata = UTILP1_GetSecondData(vdramB, globalparamsvB.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer1[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH]; }
	}
	
	MERGEP1_EXCHANGEC_LOOP1C: for(unsigned int reduce_partition=0; reduce_partition<_TOTALNUMREDUCEPARTITIONS_SLR0; reduce_partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGEP1_exchange: ERROR 1s22", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, reduce_partition, NAp, NAp);
		#endif
		unsigned int tempdata = UTILP1_GetSecondData(vdramC, globalparamsvC.BASEOFFSETKVS_STATSDRAM + reduce_partition);
		if(tempdata > 0){ globalstatsbuffer2[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH]; }
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"exchangeVs:: printing active global stats..."<<endl;
	for(unsigned int i=0; i<_TOTALNUMREDUCEPARTITIONS_SLR0; i++){ if(globalstatsbuffer0[i / 32].data[i % 32] > 0 || globalstatsbuffer1[i / 32].data[i % 32] > 0 || globalstatsbuffer2[i / 32].data[i % 32] > 0){ cout<<"EXCHANGE: globalstatsbuffer0["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer0[i][i / 32].data[i % 32]<<", globalstatsbuffer1["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer1[i / 32].data[i % 32]<<", globalstatsbuffer2["<<i / 32<<"].data["<<i % 32<<"] : "<<globalstatsbuffer2[i / 32].data[i % 32]<<endl; }}					
	#endif 
	
	// vertices
	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGEP1_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP1_topkernelS: ERROR 20", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_GLOBALSTATS_BIGSIZE, globalparamsvA.NUM_REDUCEPARTITIONS, NAp, NAp);
			#endif
			if(globalstatsbuffer0[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"exchangeVs vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
				#endif
				MERGEP1_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 21", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 22", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					totalvsynchronized += VECTOR2_SIZE;
					#endif
					
					// collect active vertices
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					MERGEP1_print_active_masks(vdramA, globalparamsvA, voffsetA_kvs);
					#endif
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: FINISHED transferring vertices from vdramA to vdramB & vdramC. [end offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif

	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: transferring vertices from vdramB to vdramA & vdramC. [begin offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	MERGEP1_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP1_topkernelS: ERROR 21", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(globalstatsbuffer1[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"exchangeVs vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
				#endif
				MERGEP1_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 24", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 25", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					totalvsynchronized += VECTOR2_SIZE;
					#endif
					
					// collect active vertices 
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					MERGEP1_print_active_masks(vdramB, globalparamsvB, voffsetB_kvs);
					#endif
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: finished transferring vertices from vdramB to vdramA & vdramC. [end offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	
	reduce_partition = 0;
	partition_offset = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: transferring vertices from vdramC to vdramA & vdramB. [begin offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	MERGEP1_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		unsigned int voffset_kvs = 0;
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){ 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP1_topkernelS: ERROR 22", reduce_partition / BRAM_BIT_WIDTH, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			bool exchange_partition = true; if((globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + globalparamsvC.SIZEKVS2_REDUCEPARTITION > globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS)){ exchange_partition = false; }
			if((globalstatsbuffer2[reduce_partition / BRAM_BIT_WIDTH].data[reduce_partition % BRAM_BIT_WIDTH] > 0 || exchangeall == true) & (exchange_partition == true)){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"exchangeVs vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [voffset_kvs: "<<voffset_kvs<<", offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
				#endif
				MERGEP1_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 27", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 28", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
					totalvsynchronized += VECTOR2_SIZE;
					#endif
					
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k]; 
					
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					MERGEP1_print_active_masks(vdramC, globalparamsvC, voffsetC_kvs);
					#endif
				}
			}
			
			reduce_partition += 1;
			voffset_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;			
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"exchangeVs:: finished transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"exchangeVs:: exchange operation finished. "<<totalvsynchronized<<" vertices synchronized."<<endl; 
	cout<<"exchangeVs:: exchange operation finished."<<endl; 
	#endif
	return;
}

extern "C" {
void TOPP1_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
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
	globalparams_t globalparamsvA = UTILP1_getglobalparams(vdramA, 0);
	unsigned int inACTSMODE = UTILP1_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"~~~ TOPP1_topkernelS:: GraphIter: "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<endl;
	#endif
	
	#ifdef CONFIG_RELEASE_VERSION4
	if(globalparamsvA.ENABLE_EXCHANGECOMMAND == ON){
		MERGEP1_exchange(vdramA, vdramB, vdramC, mdram);
	}
	#endif 
	return;
}
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_ACTS
// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Vitis/multiple_kernels/vadd_kernel/krnl_vadd.cpp
// https://github.com/Xilinx/SDAccel_Examples/blob/master/getting_started/dataflow/dataflow_stream_c/src/adder.cpp
// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Dataflow/Channels/using_fifos/diamond.cpp  *

#ifdef FPGA_IMPL
void load(uint512_dt *in, hls::stream<uint512_vec_dt >& out, batch_type sourcebaseaddr_kvs, unsigned int offset_kvs, int size){
	Loop_Ld: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt data = UTILP1_GetDataset(in, sourcebaseaddr_kvs + offset_kvs + i);
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
			res.data[v] = PROCESSP1_processvector(enx, v, data.data[v].value - upperlimit, upperlimit, data.data[v], vbuffer_source[v], globalparamsK);
		}
		
		// store
		UTILP1_SetDataset(out, destbaseaddr_kvs + offset_kvs + i, res);
	}
}
#endif 

void ACTSP1_load_compute_and_store(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
			batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, int size, unsigned int upperlimit, 
						unsigned int collections_regs[2], globalparams_t globalparamsK, globalposition_t globalposition){
	uint512_vec_dt res;
	for(int i = 0; i < size; i++){
		// load 
		uint512_vec_dt data = UTILP1_GetDataset(in, sourcebaseaddr_kvs + srcoffset_kvs + i);

		// set flag
		bool enx = true;
		if((globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE) && (collections_regs[0] + VECTOR_SIZE >= globalposition.num_active_edges_in_channel)){ enx = false; } // for BFS implementation
		else { collections_regs[1] += VECTOR_SIZE; }
		
		// process
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS UNROLL
			res.data[v] = PROCESSP1_processvector(enx, v, data.data[v].value - upperlimit, upperlimit, data.data[v], vbuffer_source[v], globalparamsK);
		}
		
		// store
		UTILP1_SetDataset(out, destbaseaddr_kvs + destoffset_kvs + i, res);
	}
}

void ACTSP1_actit3_load_compute_and_store(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	
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
	ACTSP1_load_compute_and_store(kvdram, kvdram, vbuffer_source,
		sourcebaseaddr_kvs, srcoffset_kvs, destbaseaddr_kvs, destoffset_kvs, size_kvs, upperlimit, 
					collections_regs, globalparamsK, globalposition);
	#endif
}

void ACTSP1_actit3(bool_type enable, unsigned int mode,
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
		workload_kvs.offset_begin = MEMACCESSP1_getdata(kvdram, globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK, (sweepparams.source_partition * num_LLPset) + llp_set) / VECTOR_SIZE;
		workload_kvs.offset_end = MEMACCESSP1_getdata(kvdram, globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK, (sweepparams.source_partition * num_LLPset) + llp_set + 1) / VECTOR_SIZE;
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
			
			ACTSP1_actit3_load_compute_and_store(kvdram, vbuffer_source, vbuffer_dest,	
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
			curroffsetkvs = MEMACCESSP1_getdata(kvdram, globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK, sweepparams.source_partition) / VECTOR_SIZE;
			nextoffsetkvs = MEMACCESSP1_getdata(kvdram, globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK, sweepparams.source_partition + 1) / VECTOR_SIZE;
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
			uint512_vec_dt data = UTILP1_GetDataset(kvdram, sourcebaseaddr_kvs + offset_kvs);
			
			// reduce 
			bool enx = true;
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP1_reducevector(enx, 0, UTILP1_GETKV(data.data[0]), vbuffer_dest[0 + 0], 0, sweepparams.upperlimit, &memory[0], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP1_reducevector(enx, 1, UTILP1_GETKV(data.data[1]), vbuffer_dest[0 + 1], 0, sweepparams.upperlimit, &memory[1], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP1_reducevector(enx, 2, UTILP1_GETKV(data.data[2]), vbuffer_dest[0 + 2], 0, sweepparams.upperlimit, &memory[2], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP1_reducevector(enx, 3, UTILP1_GETKV(data.data[3]), vbuffer_dest[0 + 3], 0, sweepparams.upperlimit, &memory[3], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP1_reducevector(enx, 4, UTILP1_GETKV(data.data[4]), vbuffer_dest[0 + 4], 0, sweepparams.upperlimit, &memory[4], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP1_reducevector(enx, 5, UTILP1_GETKV(data.data[5]), vbuffer_dest[0 + 5], 0, sweepparams.upperlimit, &memory[5], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP1_reducevector(enx, 6, UTILP1_GETKV(data.data[6]), vbuffer_dest[0 + 6], 0, sweepparams.upperlimit, &memory[6], sweepparams, globalparamsK);
 // VECTOR_SIZE_seq // 1_seq // CRITICAL FIXME.
			REDUCEP1_reducevector(enx, 7, UTILP1_GETKV(data.data[7]), vbuffer_dest[0 + 7], 0, sweepparams.upperlimit, &memory[7], sweepparams, globalparamsK);
			
			
			#ifdef _DEBUGMODE_STATS
			if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && mode==ACTSPROCESSMODE && enx == true){
				actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, VECTOR_SIZE);
				actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, VECTOR_SIZE); }
			#endif 
		}
	}
}	
#endif 
#ifdef CONFIG_ENABLECLASS_ACTS
#define ENABLE_PP1
fetchmessage_t ACTSP1_fetchkeyvalues(bool_type enable, unsigned int mode,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE],				 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams,
		unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	fetchmessage_t fetchmessage;
	if(mode == ACTSPROCESSMODE){
		fetchmessage = PROCESSP1_readandprocess(enable, kvdram, kvdram, vbuffer, buffer, goffset_kvs, loffset_kvs, size_kvs, travstate, sweepparams, globalposition, globalparams, collections);
	} else {
		fetchmessage = MEMACCESSP1_readkeyvalues(enable, kvdram, buffer, goffset_kvs + loffset_kvs, size_kvs, travstate, globalparams); 
	}
	return fetchmessage; 
}

void ACTSP1_commitkeyvalues(bool_type enable, bool_type enable2, unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],
		keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], 
			batch_type destbaseaddr_kvs, sweepparams_t sweepparams, globalposition_t globalposition, globalparams_t globalparams, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(mode == ACTSREDUCEMODE || (globalparams.ACTSPARAMS_TREEDEPTH == 1 && globalparams.ENABLE_APPLYUPDATESCOMMAND == ON)){
		REDUCEP1_reduceandbuffer_base(enable, destbuffer, localcapsule, vbuffer, sweepparams, globalposition, globalparams, collections);
		if(globalparams.ACTSPARAMS_TREEDEPTH == 1){ for(partition_type p=0; p<globalparams.ACTSPARAMS_NUM_PARTITIONS; p++){ globalcapsule[p].value += localcapsule[p].value; }} // NEWCHANGE.
	} else {
		MEMACCESSP1_savekeyvalues(enable, kvdram, destbuffer, globalcapsule, localcapsule, destbaseaddr_kvs, globalposition, globalparams); 
	}
	return;
} 

void ACTSP1_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(enable == OFF){ return; }
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
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
	
	if(resetenv == ON){ UTILP1_resetenvbuffers(capsule_so1, capsule_so8); } else { itercount = 64; } // FIXME. '64' is just some number greater than 2
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit: processing all chunks [begin_kvs: "<<ptravstate.begin_kvs<<"][end_kvs: "<<ptravstate.end_kvs<<"][flushsz: "<<flushsz<<"][size_kvs: "<<(ptravstate.end_kvs + flushsz) - ptravstate.begin_kvs<<"][workbuffer_size: "<<workbuffer_size<<"][num_chunks: "<<((ptravstate.end_kvs + flushsz) - ptravstate.begin_kvs) / workbuffer_size<<"] ... "<<endl;
	#endif

	ACTIT_MAINLOOP: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs + flushsz; offset_kvs+=workbuffer_size * NUMPIPELINES_PARTITIONUPDATES){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts::ACTSP1_actit: processing chunk [offset_kvs: "<<offset_kvs<<"]: [ptravstate.begin_kvs: "<<ptravstate.begin_kvs<<"]: [ptravstate.end_kvs + flushsz: "<<ptravstate.end_kvs + flushsz<<"] ... "<<endl;
		#endif
		#ifdef PUP1
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		if(itercount >= 1){ pp1writeen = ON; } else { pp1writeen = OFF; }
		if(itercount >= 1){ pp1runpipelineen = ON; } else { pp1runpipelineen = OFF; }
		#else 
		if(itercount >= 0){ pp0writeen = ON; } else { pp0writeen = OFF; }
		#endif
		
		ptravstatepp0.i_kvs = offset_kvs;
		fetchmessagepp0 = ACTSP1_fetchkeyvalues(ON, mode,  kvdram, vbuffer_source, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp0.i_kvs, workbuffer_size, ptravstatepp0, sweepparams, globalposition, globalparams, edgebankID, collections);				
		#ifdef PUP1
		PARTITIONP1_partitionbase_stage2(pp1runpipelineen, ON, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, globalposition, pp1cutoffs, (itercount-2)+1, globalparams);
		#endif 
		
		PARTITIONP1_partitionbase_stage1(ON, ON, mode, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, globalposition, fetchmessagepp0.chunksize_kvs, pp0cutoffs, globalparams);
		#ifdef PUP1
		ACTSP1_commitkeyvalues(pp1writeen, ON, mode, kvdram, vbuffer_dest, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalposition, globalparams, collections); 
		#endif 
		
		PARTITIONP1_partitionbase_stage2(ON, ON, mode, buffer_setof1, capsule_so1, buffer_setof8, capsule_so8, sweepparams.currentLOP, sweepparams, globalposition, pp0cutoffs, itercount, globalparams);
		#ifdef PUP1
		ptravstatepp1.i_kvs = offset_kvs + workbuffer_size;
		fetchmessagepp1 = ACTSP1_fetchkeyvalues(ON, mode,  kvdram, vbuffer_source, sourcebuffer, sourcebaseaddr_kvs, ptravstatepp1.i_kvs, workbuffer_size, ptravstatepp1, sweepparams, globalposition, globalparams, edgebankID, collections);
		#endif
		
		ACTSP1_commitkeyvalues(pp0writeen, ON, mode, kvdram, vbuffer_dest, buffer_setof8, globalstatsbuffer, capsule_so8, destbaseaddr_kvs, sweepparams, globalposition, globalparams, collections); 
		#ifdef PUP1
		PARTITIONP1_partitionbase_stage1(pp1partitionen, ON, mode, sourcebuffer, buffer_setof1, capsule_so1, sweepparams.currentLOP, sweepparams, globalposition, fetchmessagepp1.chunksize_kvs, pp1cutoffs, globalparams);
		#endif
		
		itercount += NUMPIPELINES_PARTITIONUPDATES;
		// exit(EXIT_SUCCESS);
	}
	// exit(EXIT_SUCCESS);
	return;
}

void ACTSP1_actit2(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparams, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	if(enable == OFF){ return; } // NEWCHANGE
	
	keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = destbuffer
	keyvalue_capsule_t capsule_dest[MAX_NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=capsule_dest complete
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit: processing all chunks [begin_kvs: "<<ptravstate.begin_kvs<<"][end_kvs: "<<ptravstate.end_kvs<<"][size_kvs: "<<ptravstate.end_kvs - ptravstate.begin_kvs<<"][workbuffer_size: "<<globalparams.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<(ptravstate.end_kvs - ptravstate.begin_kvs) / globalparams.ACTSPARAMS_WORKBUFFER_SIZE<<"] ... "<<endl;
	#endif
	ACTIT2_MAINLOOP: for(batch_type offset_kvs=ptravstate.begin_kvs; offset_kvs<ptravstate.end_kvs; offset_kvs+=globalparams.ACTSPARAMS_WORKBUFFER_SIZE){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts::ACTSP1_actit: processing chunk [offset_kvs: "<<offset_kvs<<"]: [ptravstate.begin_kvs: "<<ptravstate.begin_kvs<<"]: [ptravstate.end_kvs: "<<ptravstate.end_kvs<<"] ... "<<endl;
		#endif
		
		PARTITIONP1_read_process_partition(enable, mode, offset_kvs, sourcebaseaddr_kvs, kvdram, vbuffer_source, destbuffer, capsule_dest, ptravstate, sweepparams, globalposition, globalparams, collections);	
		
		ACTSP1_commitkeyvalues(enable, ON, mode, kvdram, vbuffer_dest, destbuffer, globalcapsule, capsule_dest, destbaseaddr_kvs, sweepparams, globalposition, globalparams, collections); 
	}
	// exit(EXIT_SUCCESS); 
	return;
}

void ACTSP1_actit_base(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_SETUP_SUITABLE_FOR_LARGE_GRAPHS
	// ACTSP1_actit2(enable, mode, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparamsK, globalposition, sweepparams, ptravstate, sourcebaseaddr_kvs, destbaseaddr_kvs, resetenv, flush, edgebankID, collections);
	ACTSP1_actit3(enable, mode, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sourcebaseaddr_kvs, destbaseaddr_kvs, resetenv, flush, edgebankID, collections);
	#else 
	ACTSP1_actit(enable, mode, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparamsK, globalposition, sweepparams, ptravstate, sourcebaseaddr_kvs, destbaseaddr_kvs, resetenv, flush, edgebankID, collections);
	#endif
}

	
#endif 			
void TOPP1_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	for(buffer_type t=0; t<globalparamsK.SIZEKVS2_REDUCEPARTITION; t++){
		for(buffer_type v=0; v<VDATA_PACKINGSIZE; v++){
			if(MEMCAP1_READVMASK(vbuffer_source[v][t]) == 1){ cout<<"TOPP1_processit: ACTIVE MASK SEEN: @ t: "<<t<<", v: "<<v<<", vbuffer_source["<<v<<"]["<<t<<"]: "<<vbuffer_source[v][t]<<endl; }
		}
	}
	#endif

	sweepparams_t sweepparams;
	if(globalparamsK.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ globalposition.deststatsmarker = MERGEP1_actvpstatsoffset(globalparamsK); }
	
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP1_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0); } 
	sweepparams = UTILP1_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	
	if(globalposition.source_partition == globalposition.first_source_partition){ MEMACCESSP1_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL NEWCHANGE.
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP1_resetvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0); } // CRITICAL NEWCHANGE.
	
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); //. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	batch_type voffset_kvs = globalposition.source_partition * UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print7("### TOPP1_processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif

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
		MEMACCESSP1_readV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
	}
	
	if(returncmd == false){
		sweepparams.source_partition = globalposition.source_partition;
		avtravstate.i_kvs = voffset_kvs;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print5("### TOPP1_processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, MAX_SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + (UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + (UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
			
		unsigned int SKIP_KVS = UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
		
		tuple_t tup = MEMACCESSP1_getvptrs_opt(kvdram, vptrbaseoffset_kvs, voffset_kvs * VECTOR2_SIZE, (voffset_kvs + SKIP_KVS) * VECTOR2_SIZE, edgebankID, globalparamsK);
		keyy_t beginvptr = tup.A;
		keyy_t endvptr = tup.B; 
		
		#ifdef _DEBUGMODE_CHECKS3
		if(globalparamsK.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ if(beginvptr % VECTOR2_SIZE != 0 || endvptr % VECTOR2_SIZE != 0){ cout<<"TOPP1_processit::ERROR: beginvptr("<<beginvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0 || endvptr("<<endvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }}
		#endif 
		
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr;
		keyy_t localendvptr = endvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 1*VECTOR_SIZE; // CRITICAL NEWCHANGE.
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN____UNUSED // FIXME. REMOVEME WHEN IN RUNNING MODE.
		if(sweepparams.source_partition < 64){
			UTILP1_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + sweepparams.source_partition, localbeginvptr_kvs);
			UTILP1_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + sweepparams.source_partition, numedges); 
		}
		#endif 
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		collections[0][0].data1 += numedges;
		#endif
		
		#ifdef _DEBUGMODE_CHECKS3
		if(localendvptr < localbeginvptr){ cout<<"TOPP1_processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<", voffset_kvs: "<<voffset_kvs<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("TOPP1_processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"[index: "<<globalposition.source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"]"<<endl;
		#endif
		#ifdef _DEBUGMODE_CHECKS3
		if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
		actsutilityobj->clearallstats();
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		for(unsigned int t=0; t<8; t++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				cout<<"processit:: kvdram["<<globalparamsE.BASEOFFSETKVS_EDGESDATA<<" + "<<localbeginvptr_kvs + t<<"].data["<<v<<"].key: "<<kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA + localbeginvptr_kvs + t].data[v].key<<", kvdram["<<globalparamsE.BASEOFFSETKVS_EDGESDATA<<" + "<<localbeginvptr_kvs + t<<"].data["<<v<<"].value: "<<kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA + localbeginvptr_kvs + t].data[v].value<<endl; 
			}
		}
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
		if((voffset_kvs + UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		resetenv = ON; flush = ON; 
		
		ACTSP1_actit_base(
			ON, ACTSPROCESSMODE,
			kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer,
			globalparamsE, globalparamsK, globalposition, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAM,
			resetenv, flush, edgebankID, collections);
			
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
			if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ collections[0][1].data1 = collections[0][2].data1; } else { collections[0][1].data1 += numedges; }}
		#endif 
	}
	
	bool endofrun = false;
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ endofrun = true; }
	
	if(endofrun == true){ MEMACCESSP1_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL OPTIMIZEME. should be called only once
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPP1_processit", globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS); }
	#endif	
	
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && endofrun == true){ 
		MEMACCESSP1_saveV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][1].data1);
		#endif
	}
	
	#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
	if(endofrun == true){ UTILP1_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][0].data1); }
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

void TOPP1_partitionit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	if(globalparamsK.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ return; } ///
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nusrcv_nudstv::UTILP1_getglobalparams:: printing global parameters", globalparamsK);
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
	batch_type deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparamsK.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparamsK.ACTSPARAMS_BEGINLOP + 1 + (globalparamsK.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = UTILP1_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"TOPP1_partitionit: source_partition "<<source_partition<<" ... "<<endl;

			UTILP1_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
		
			sweepparams = UTILP1_getsweepparams(globalparamsK, currentLOP, source_partition);
			travstate_t ptravstate = UTILP1_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
			
			// collect stats
			MEMACCESSP1_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK);
			UTILP1_resetvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparamsK.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### TOPP1_partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			UTILP1_resetvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			ACTSP1_actit_base(config.enablepartition, ACTSPARTITIONMODE,
					kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, // CRITICAL FIXME.
					globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp, collections);
					
			MEMACCESSP1_saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK); 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS;
			}
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_STATS
			if(config.enablereduce == ON){ edges_count = actsutilityobj->globalstats_getcountvalidkvsreduced(globalparamsK.ACTSPARAMS_INSTID); edgesdstv_sum = actsutilityobj->globalstats_getreducevar1(); }
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

void TOPP1_reduceit( uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; 
	globalposition_t globalposition; // DUMMY.
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	
	sweepparams = UTILP1_getsweepparams(globalparamsK, currentLOP, source_partition);
	travstate_t ptravstate = UTILP1_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
	UTILP1_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### TOPP1_reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	ACTSP1_actit_base(config.enablereduce, ACTSREDUCEMODE,
			 kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, // CRITICAL FIXME.
			globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp, collections);
	return;
}

void TOPP1_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(en_process == ON){ TOPP1_processit(kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(en_partition == ON){ TOPP1_partitionit(kvdram, vbuffer_source, vbuffer_dest, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(en_reduce == ON){ TOPP1_reduceit(kvdram, vbuffer_source, vbuffer_dest, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	return;
} 

void TOPP1_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	unsigned int sourcestatsmarker = 0;
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); }
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int vmask_offset_kvs = 0;
	unsigned int vmaskp_offset_kvs = 0;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = globalparamsK.NUM_REDUCEPARTITIONS;
	
	bool_type enablereduce = ON;
	keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE]; // NOT USED.
	
	UTILP1_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
	
	DISPATCHREDUCEP1_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### TOPP1_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "vreadoffset_kvs2", source_partition, currentLOP, num_source_partitions, vreadoffset_kvs2); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTILP1_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		globalstatsbuffer[source_partition].key = NAp; globalstatsbuffer[source_partition].value = rtravstate.size_kvs;
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		collections[0][1].data1 += rtravstate.size_kvs * VECTOR_SIZE;
		#endif 
		
		// read vertices
		MEMACCESSP1_readV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		// if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID == CF){ MEMACCESSP1_copyV(enablereduce, vbuffer_dest, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.BUFFERBASEOFFSETKVS_VDATA2, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK); } // CUSTOMPROVISION
		
		// reduce
		TOPP1_dispatch(OFF, OFF, enablereduce, kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);
		
		// writeback vertices
		MEMACCESSP1_saveV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		vmaskp_offset_kvs += NUM_PEs;
	}
	
	#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN // NEWCHANGE
	UTILP1_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[0][1].data1);
	#endif 
	// exit(EXIT_SUCCESS);
	return;
} 

void TOPP1_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[NUM_COLLECTIONS][COLLECTIONS_BUFFERSZ]){									

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	unsigned int printheader1=OFF; // ON, OFF*
	unsigned int printheader2=OFF; 
	if(true && globalposition.source_partition==globalposition.first_source_partition){ printheader1 = ON; } else { printheader1 = OFF; }
	if(false && globalposition.source_partition==globalposition.last_source_partition){ printheader2 = ON; } else { printheader2 = OFF; }
	// printheader1=ON;
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== ACTS Launched... size: "<<UTILP1_GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl;
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	unsigned int PARTITIONP1_CHKPT[EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
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
	
	globalparamsK = UTILP1_getglobalparams(kvdram, 0); // CRITICAL OPTIMIZEME. MOVETOBASE?
	
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram, e+1); }
	_globalparamsE = globalparamsEs[globalposition.edgebankID];
	globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid;
	for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE. IS THIS OPTIMIZED
	
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
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ UTILP1_resetkvstatvalues(kvdram, globalparamsK); } // NEWCHANGE
	#endif
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP1_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP1_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
				vbuffer_dest, 
					#else 
						vbuffer_source,	
							#endif 
								globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP1_CHKPT, globalposition.v_chunkid, globalposition.edgebankID, collections); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP1_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP1_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
			vbuffer_dest, 
				#else 
					vbuffer_source,		
						#endif 
							globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP1_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	// reduce & partition
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS3) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		if(printheader1 == ON){ cout<<"TOPP1_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP1_dispatch_reduce(kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
			vbuffer_dest, 
				#else 
					vbuffer_source,		
						#endif 
							_globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONP1_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	bool endofrun = false;
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }} 
	else { if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }}
	#if not defined(FPGA_IMPL) && defined(CONFIG_COLLECT_DATAS3_DURING_RUN)
	if(endofrun == true){
		UTILP1_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, actsutilityobj->globalstats_getcountnumvalidprocessedges(globalparamsK.ACTSPARAMS_INSTID)); // REMOVEME.
		UTILP1_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, actsutilityobj->globalstats_getcountkvsreduced(globalparamsK.ACTSPARAMS_INSTID)); // globalstats_getcountvalidkvsreduced, *globalstats_getcountkvsreduced, globalstats_getcountkvspartitionswritten
	}
	#endif 
	if(endofrun == true){
		unsigned int _offset = MERGEP1_actvpstatsoffset(globalparamsK);
		MEMACCESSP1_retreievekvstats(kvdram, globalstatsbuffer, globalparamsK, _offset, globalparamsK.NUM_REDUCEPARTITIONS);
		UTILP1_increment_graphiteration(kvdram, globalparamsK); } // NB: this should come last.
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(endofrun == true){ actsutilityobj->printglobalvars2(globalparamsK.ACTSPARAMS_INSTID, "instID", "v_chunkid", "LOP", "sourcep", globalparamsK.ACTSPARAMS_INSTID, globalposition.v_chunkid, globalposition.currentLOP, globalposition.source_partition); }
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void TOPP1_topkernelP1(
	
	uint512_dt * kvdram0,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
	
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control


#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[1];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL1_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL1_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL1_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks1(ON, vdram, vbuffer_source0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP2(
	
	uint512_dt * kvdram0,
	
	uint512_dt * kvdram1,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[2];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL2_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL2_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL2_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks2(ON, vdram, vbuffer_source0,vbuffer_source1, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP3(
	
	uint512_dt * kvdram0,
	
	uint512_dt * kvdram1,
	
	uint512_dt * kvdram2,
	uint512_dt * vdram
	){
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[3];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL3_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL3_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL3_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks3(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP4(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[4];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL4_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL4_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL4_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks4(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP5(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
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
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[5];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL5_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL5_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL5_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks5(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	
		globalparams_t globalparamsK4;
		MERGEP1_mergeVs(kvdram4, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP6(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
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
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
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
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[6];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL6_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL6_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL6_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks6(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	
		globalparams_t globalparamsK4;
		MERGEP1_mergeVs(kvdram4, vdram);
	
		globalparams_t globalparamsK5;
		MERGEP1_mergeVs(kvdram5, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP7(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
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
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
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
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
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
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[7];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL7_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL7_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL7_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks7(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	
		globalparams_t globalparamsK4;
		MERGEP1_mergeVs(kvdram4, vdram);
	
		globalparams_t globalparamsK5;
		MERGEP1_mergeVs(kvdram5, vdram);
	
		globalparams_t globalparamsK6;
		MERGEP1_mergeVs(kvdram6, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP8(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
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
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
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
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
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
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
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
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[8];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL8_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL8_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL8_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks8(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	
		globalparams_t globalparamsK4;
		MERGEP1_mergeVs(kvdram4, vdram);
	
		globalparams_t globalparamsK5;
		MERGEP1_mergeVs(kvdram5, vdram);
	
		globalparams_t globalparamsK6;
		MERGEP1_mergeVs(kvdram6, vdram);
	
		globalparams_t globalparamsK7;
		MERGEP1_mergeVs(kvdram7, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP9(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
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
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
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
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
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
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
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
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source8
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
	#pragma HLS ARRAY_PARTITION variable = collections8 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[9];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL9_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL9_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL9_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks9(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	
		globalparams_t globalparamsK4;
		MERGEP1_mergeVs(kvdram4, vdram);
	
		globalparams_t globalparamsK5;
		MERGEP1_mergeVs(kvdram5, vdram);
	
		globalparams_t globalparamsK6;
		MERGEP1_mergeVs(kvdram6, vdram);
	
		globalparams_t globalparamsK7;
		MERGEP1_mergeVs(kvdram7, vdram);
	
		globalparams_t globalparamsK8;
		MERGEP1_mergeVs(kvdram8, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP10(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
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
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
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
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
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
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
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
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source8
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
	#pragma HLS ARRAY_PARTITION variable = collections8 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source9
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
	#pragma HLS ARRAY_PARTITION variable = collections9 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[10];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL10_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL10_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL10_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks10(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	
		globalparams_t globalparamsK4;
		MERGEP1_mergeVs(kvdram4, vdram);
	
		globalparams_t globalparamsK5;
		MERGEP1_mergeVs(kvdram5, vdram);
	
		globalparams_t globalparamsK6;
		MERGEP1_mergeVs(kvdram6, vdram);
	
		globalparams_t globalparamsK7;
		MERGEP1_mergeVs(kvdram7, vdram);
	
		globalparams_t globalparamsK8;
		MERGEP1_mergeVs(kvdram8, vdram);
	
		globalparams_t globalparamsK9;
		MERGEP1_mergeVs(kvdram9, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP11(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
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
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
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
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
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
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
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
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source8
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
	#pragma HLS ARRAY_PARTITION variable = collections8 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source9
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
	#pragma HLS ARRAY_PARTITION variable = collections9 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source10
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
	#pragma HLS ARRAY_PARTITION variable = collections10 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[11];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL11_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL11_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL11_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks11(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, globalstatsbuffer10, globalposition, globalparamsV, collections10);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	
		globalparams_t globalparamsK4;
		MERGEP1_mergeVs(kvdram4, vdram);
	
		globalparams_t globalparamsK5;
		MERGEP1_mergeVs(kvdram5, vdram);
	
		globalparams_t globalparamsK6;
		MERGEP1_mergeVs(kvdram6, vdram);
	
		globalparams_t globalparamsK7;
		MERGEP1_mergeVs(kvdram7, vdram);
	
		globalparams_t globalparamsK8;
		MERGEP1_mergeVs(kvdram8, vdram);
	
		globalparams_t globalparamsK9;
		MERGEP1_mergeVs(kvdram9, vdram);
	
		globalparams_t globalparamsK10;
		MERGEP1_mergeVs(kvdram10, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
void TOPP1_topkernelP12(
	
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

// static hls::stream<uint32_t> kvdram("input_stream_1");
// static hls::stream<uint32_t> inStream2("input_stream_2");
// static hls::stream<uint32_t> outStream("output_stream");

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
	
	/* ///////////////////////////////
	for(unsigned int iter=0; iter<1; iter++){
		for(unsigned int t=0; t<16; t++){ // universalparams.NUMPROCESSEDGESPARTITIONS
			unsigned int voffset = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
			cout<<"~~~[topkernelP-0] vdram["<<voffset + t<<"].data["<<iter<<"].key: "<<vdram[voffset + t].data[iter].key<<", vdram["<<voffset + t<<"].data["<<iter<<"].value: "<<vdram[voffset + t].data[iter].value<<endl;
		}
	}
	/////////////////////////////////////////////////// */
	
	keyvalue_vbuffer_t vbuffer_source0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source0
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
	#pragma HLS ARRAY_PARTITION variable = collections0 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source1
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
	#pragma HLS ARRAY_PARTITION variable = collections1 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source2
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
	#pragma HLS ARRAY_PARTITION variable = collections2 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source3
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
	#pragma HLS ARRAY_PARTITION variable = collections3 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source4
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
	#pragma HLS ARRAY_PARTITION variable = collections4 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source5
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
	#pragma HLS ARRAY_PARTITION variable = collections5 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source6
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
	#pragma HLS ARRAY_PARTITION variable = collections6 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source7
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
	#pragma HLS ARRAY_PARTITION variable = collections7 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source8
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
	#pragma HLS ARRAY_PARTITION variable = collections8 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source9
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
	#pragma HLS ARRAY_PARTITION variable = collections9 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source10
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
	#pragma HLS ARRAY_PARTITION variable = collections10 complete // NEWCHANGE.
	keyvalue_vbuffer_t vbuffer_source11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer_source11
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
	#pragma HLS ARRAY_PARTITION variable = collections11 complete // NEWCHANGE.
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	pmask_dt emask_curr[BLOCKRAM_CURRPMASK_SIZE];
	travstate_t rtravstates[12];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition; globalposition.num_active_edges_in_channel = 0;

	globalparams_t globalparamsK = UTILP1_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP1_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP1_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	

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
	
	unsigned int PARTITIONP1_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP1_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP1_CHKPT["<<u<<"]: "<<PARTITIONP1_CHKPT[u]<<endl; }
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
	unsigned int total_num_active_vertices_in_iteration = 0;

	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	#endif 
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	vreadoffset_kvs2 = 0;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		#endif 
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#if defined(_DEBUGMODE_KERNELPRINTS3) & defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			unsigned int total_num_active_vertices_in_vchunk = 0;
			TOPKERNEL12_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL12_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					#endif 
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
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
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						#ifdef CONFIG_COLLECT_DATAS2_DURING_RUN
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						#endif 
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL12_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						
						bool_type enable_readandreplicatevdata = OFF; 
						
						// hyrid heuristics
						if(globalposition.EN_PROCESS == ON && source_partition == FIRSTSOURCEPARTITION){ for(unsigned int p=0; p<LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; p++){ total_num_active_vertices_in_vchunk += pmask_curr[p]; }}
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE.
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks12(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10,vbuffer_source11, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalstatsbuffer1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalstatsbuffer2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalstatsbuffer3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalstatsbuffer4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalstatsbuffer5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalstatsbuffer6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalstatsbuffer7, globalposition, globalparamsV, collections7);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalstatsbuffer8, globalposition, globalparamsV, collections8);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalstatsbuffer9, globalposition, globalparamsV, collections9);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, globalstatsbuffer10, globalposition, globalparamsV, collections10);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer_source11, globalstatsbuffer11, globalposition, globalparamsV, collections11);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalstatsbuffer0, globalposition, globalparamsV, collections0);		
		
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", vreadoffset_kvs2, globalparamsV.SIZE_SRCVERTICESDATA, vreadskipsz_kvs2, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID				
	
	#ifdef _DEBUGMODE_STATS // NOTE: must come before increment_graphiteration
	unsigned int num_procs = 0; for(unsigned int t=0; t<64; t++){ num_procs += actsutilityobj->globalstats_getcountvalidkvsprocessed(t); }
	unsigned int num_acvvs = 0;  num_acvvs += kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key;  num_acvvs += kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key; 	
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_procs);
	UTILP1_SetFirstData(vdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, num_acvvs);
	#endif 
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
		#ifdef NOT_USED
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	
		globalparams_t globalparamsK4;
		MERGEP1_mergeVs(kvdram4, vdram);
	
		globalparams_t globalparamsK5;
		MERGEP1_mergeVs(kvdram5, vdram);
	
		globalparams_t globalparamsK6;
		MERGEP1_mergeVs(kvdram6, vdram);
	
		globalparams_t globalparamsK7;
		MERGEP1_mergeVs(kvdram7, vdram);
	
		globalparams_t globalparamsK8;
		MERGEP1_mergeVs(kvdram8, vdram);
	
		globalparams_t globalparamsK9;
		MERGEP1_mergeVs(kvdram9, vdram);
	
		globalparams_t globalparamsK10;
		MERGEP1_mergeVs(kvdram10, vdram);
	
		globalparams_t globalparamsK11;
		MERGEP1_mergeVs(kvdram11, vdram);
		#endif 
	}
	#endif
	// exit(EXIT_SUCCESS); //
	
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
	
	

