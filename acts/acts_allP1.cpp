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
unsigned int acts_all::UTILP1_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int acts_all::UTILP1_amax(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int acts_all::UTILP1_aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}
uint32_type acts_all::UTILP1_amin2(uint32_type val1, uint32_type val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

// allignment
batch_type acts_all::UTILP1_allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type acts_all::UTILP1_allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type acts_all::UTILP1_allignlower_KV2(batch_type val){
	batch_type fac = val / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type acts_all::UTILP1_allignhigher_KV2(batch_type val){
	batch_type fac = (val + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type acts_all::UTILP1_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR){
	#pragma HLS INLINE
	batch_type fac = (val + (_FACTOR - 1)) / _FACTOR;
	return (fac * _FACTOR);
}
batch_type acts_all::UTILP1_allignlower_FACTOR(batch_type val, unsigned int _FACTOR){
	batch_type fac = val / _FACTOR;
	return (fac * _FACTOR);
}

unsigned int acts_all::UTILP1_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(unsigned int tree_depth){
	return (1 << (OPT_NUM_PARTITIONS_POW * (tree_depth-1)));
}

// bit manipulation
unsigned int acts_all::UTILP1_GETMASK_UINT(uint32_type index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int acts_all::UTILP1_READFROM_UINT(uint32_type data, unsigned int index, unsigned int size){ 
	unsigned int res = 0;
	res = (((data) & UTILP1_GETMASK_UINT((index), (size))) >> (index)); 
	return res;
}
void acts_all::UTILP1_WRITETO_UINT(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
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
unsigned int acts_all::UTILP1_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	#ifdef _WIDEWORD
	res = data.range(index + size - 1, index); 
	#else
	res = UTILP1_READFROM_UINT(data, index, size);
	#endif
	return res;
}
void acts_all::UTILP1_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	data->range(index + size - 1, index) = value;
	#else
	UTILP1_WRITETO_UINT(data, index, size, value);
	#endif
	return; 
}
unsigned int acts_all::UTILP1_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	res = UTILP1_READFROM_UINT(data, index, size);
	return res;
}
void acts_all::UTILP1_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	UTILP1_WRITETO_UINT(data, index, size, value);
	return; 
}

// converters
keyvalue_t acts_all::UTILP1_GETKV(keyvalue_buffer_t data){
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
keyvalue_buffer_t acts_all::UTILP1_GETKV(keyvalue_t data){
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
keyy_t acts_all::UTILP1_GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_KEY - 1, 0);
	#else
	return data;
	#endif
}
value_t acts_all::UTILP1_GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_VALUE - 1, 0);
	#else
	return data;
	#endif
}
keyy_t acts_all::UTILP1_GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[v].key;	
	#endif
}		
unsigned int acts_all::UTILP1_GETLOCALVID(unsigned int vid, unsigned int instid){ 
	#pragma HLS INLINE
	return (vid - instid) / NUM_PEs;
}
unsigned int acts_all::UTILP1_GETREALVID(unsigned int lvid, unsigned int instid){ 
	#pragma HLS INLINE
	return (lvid * NUM_PEs) + instid;
}
unsigned int acts_all::UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams){ 
	#pragma HLS INLINE
	return globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	// return PROCESSPARTITIONSZ_KVS2;
}

unsigned int acts_all::UTILP1_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(31, 0);
	#else 
	return kvdram[offset_kvs].data[0].key;
	#endif 
}
void acts_all::UTILP1_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(31, 0) = data;
	#else 
	kvdram[offset_kvs].data[0].key = data;
	#endif 
	return;
}
void acts_all::UTILP1_SetFirstDatas(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int * data, unsigned int size){
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
unsigned int acts_all::UTILP1_GetSecondData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(63, 32);
	#else 
	return kvdram[offset_kvs].data[0].value;
	#endif 
}
void acts_all::UTILP1_SetSecondData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(63, 32) = data;
	#else 
	kvdram[offset_kvs].data[0].value = data;
	#endif 
	return;
}
unsigned int acts_all::UTILP1_GetThirdData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(95, 64);
	#else 
	return kvdram[offset_kvs].data[1].key;
	#endif 
}
void acts_all::UTILP1_SetThirdData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(95, 64) = data;
	#else 
	kvdram[offset_kvs].data[1].key = data;
	#endif 
	return;
}
	
uint512_evec_dt acts_all::UTILP1_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs){  // *** FIXME: adjust for EDGEDATA_PACKINGSIZE & UPDATEDATA_PACKINGSIZE
	#pragma HLS INLINE
	uint512_evec_dt dataset;
	
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
uint512_uvec_dt acts_all::UTILP1_GetUpdates(uint512_dt * kvdram, unsigned int offset_kvs){ // FIXME....
	#pragma HLS INLINE
	uint512_uvec_dt dataset;

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

void acts_all::UTILP1_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
void acts_all::UTILP1_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
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
void acts_all::UTILP1_WriteDataset(uint512_dt * kvdram, unsigned int offset_kvs, uint512_uvec_dt dataset){ // *** FIXME: adjust for EDGEDATA_PACKINGSIZE & UPDATEDATA_PACKINGSIZE
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
unsigned int acts_all::UTILP1_ReadData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index){
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
void acts_all::UTILP1_WriteData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data){
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

void acts_all::UTILP1_ReadKeyvalues(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
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

// utilities
batch_type acts_all::UTILP1_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
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
batch_type acts_all::UTILP1_getrangeforeachllop(globalparams_t globalparams){
	unsigned int range = globalparams.SIZE_BATCHRANGE;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
		range = range / OPT_NUM_PARTITIONS;
	}
	return range;
}
buffer_type acts_all::UTILP1_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
buffer_type acts_all::UTILP1_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	buffer_type size_kvs = 0;
	size_kvs = realsize_kvs;
	return size_kvs;
}
void acts_all::UTILP1_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size){
	unsigned int analysis_size = OPT_NUM_PARTITIONS;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = UTILP1_allignhigher_KV(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}
void acts_all::UTILP1_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
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
batch_type acts_all::UTILP1_get_num_source_partitions(step_type currentLOP){
	if(currentLOP == 0){ currentLOP = 1; }
	batch_type pow = 1;
	for(step_type i=0; i<(currentLOP-1); i++){
		pow = pow * OPT_NUM_PARTITIONS;
	}
	return pow;
}
globalparams_t acts_all::UTILP1_getglobalparams(uint512_dt * kvdram, unsigned int banksection){
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
	globalparams.BASEOFFSETKVS_EDGESMAP = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGESMAP];
	globalparams.BASEOFFSETKVS_ACTIVEEDGESMAP = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_ACTIVEEDGESMAP];
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
	globalparams.SIZE_EDGESMAP = buffer[MESSAGES_SIZE_EDGESMAP];
	globalparams.SIZE_ACTIVEEDGESMAP = buffer[MESSAGES_SIZE_ACTIVEEDGESMAP];
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
	globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 = buffer[MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR2]; 
	globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR1 = buffer[MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR1]; 
	globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR0 = buffer[MESSAGES_ACTSPARAMS_NUMCOMPUTEUNITS_SLR0]; 
	
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
sweepparams_t acts_all::UTILP1_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
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
travstate_t acts_all::UTILP1_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
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
else if(globalparams.VARS_WORKBATCH == 5){
			#ifdef _WIDEWORD
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(351, 320); 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(383, 352); 
			#else 
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[5].key; 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[5].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 6){
			#ifdef _WIDEWORD
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(415, 384); 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(447, 416); 
			#else 
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[6].key; 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[6].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 7){
			#ifdef _WIDEWORD
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(479, 448); 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].range(511, 480); 
			#else 
			keyvalue.key = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[7].key; 
			keyvalue.value = kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker].data[7].value; 
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
void acts_all::UTILP1_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value){			
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

partition_type acts_all::UTILP1_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, unsigned int tree_depth){
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
partition_type acts_all::UTILP1_getpartition2(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, globalparams_t globalparams){
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
unsigned int acts_all::UTILP1_getstatsoffset(unsigned int LOP, globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ for(unsigned int k=0; k<LOP; k++){ _offset += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }} else { _offset = 1; }
	return _offset;
}

// resets 
void acts_all::UTILP1_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void acts_all::UTILP1_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void acts_all::UTILP1_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i] = resetval; 
	}
}
void acts_all::UTILP1_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; 
		buffer[i].value = resetval; 
	}
	return;
}
void acts_all::UTILP1_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams){
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
void acts_all::UTILP1_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams){
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
void acts_all::UTILP1_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
else if(globalparams.VARS_WORKBATCH == 5){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(383, 352); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[5].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 6){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(447, 416); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[6].value; 
			#endif 
		}
else if(globalparams.VARS_WORKBATCH == 7){
			#ifdef _WIDEWORD
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].range(511, 480); 
			#else 
			buffer[k] += kvdram[globalparams.BASEOFFSETKVS_STATSDRAM + k].data[7].value; 
			#endif 
		}
		else {
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"UTILP1_accumkvstats: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<") EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}
void acts_all::UTILP1_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _WIDEWORD
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#else
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}
void acts_all::UTILP1_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
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
void acts_all::UTILP1_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
	for(partition_type p=0; p<OPT_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}

// checks 
void acts_all::UTILP1_check_capsule(keyvalue_t capsule[MAX_NUM_PARTITIONS], unsigned int num_partitions, unsigned int max_value){
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
value_t process_func_pr(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata + edgew;
	return res;
}
value_t reduce_func_pr(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter){
	#pragma HLS INLINE 
	value_t temp = vtemp + res;
	return temp;
}

value_t process_func_cf(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata;
	return res;
}
value_t reduce_func_cf(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter){
	#pragma HLS INLINE 
	unsigned int lamda = 1; unsigned int ew=1;
	value_t temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
	return temp;
}

value_t process_func_hits(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata;
	return res;
}
value_t reduce_func_hits(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter){
	#pragma HLS INLINE 
	value_t temp = vtemp + res;
	return temp;
}

value_t process_func_bfs(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = NAp;
	return res;
}
value_t reduce_func_bfs(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter){
	#pragma HLS INLINE 
	value_t temp; if(vtemp < GraphIter){ return vtemp; } else { return GraphIter; }
	return temp;
}

value_t process_func(value_t udata, value_t edgew, unsigned int GraphAlgo){
	#pragma HLS INLINE 
	if(GraphAlgo == PAGERANK){
		return process_func_pr(udata, edgew);
	} else if(GraphAlgo == CF){
		return process_func_cf(udata, edgew);
	} else if(GraphAlgo == HITS){
		return process_func_hits(udata, edgew);
	} else if(GraphAlgo == BFS){
		return process_func_bfs(udata, edgew);
	} else {
		return process_func_pr(udata, edgew);
	}
}

value_t reduce_func(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	#pragma HLS INLINE 
	if(GraphAlgo == PAGERANK){ 
		return reduce_func_pr(vtemp, vdata, res, GraphIter);
	} else if(GraphAlgo == CF){
		return reduce_func_cf(vtemp, vdata, res, GraphIter);
	} else if(GraphAlgo == HITS){
		return reduce_func_hits(vtemp, vdata, res, GraphIter);
	} else if(GraphAlgo == BFS){
		return reduce_func_bfs(vtemp, vdata, res, GraphIter);
	} else {
		return reduce_func_pr(vtemp, vdata, res, GraphIter);
	}
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_MEM_CONVERT_AND_ACCESS
// primitives
unsigned int acts_all::MEMCAP1_READVDATA(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILP1_READBITSFROM_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA);
	#endif 
}
unsigned int acts_all::MEMCAP1_READVMASK(keyvalue_vbuffer_t wideword){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILP1_READBITSFROM_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK);
	#endif 
}
unsigned int acts_all::MEMCAP1_READVDATA2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return wideword;
	#else 
	return UTILP1_READBITSFROM_UINTV(wideword, offsetof_vdata, SIZEOF_VDATA);
	#endif 
}
unsigned int acts_all::MEMCAP1_READVMASK2(keyvalue_vbuffer_t wideword, unsigned int offsetof_vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	return 0;
	#else 
	return UTILP1_READBITSFROM_UINTV(wideword, offsetof_vmask, SIZEOF_VMASK);
	#endif 
}

void acts_all::MEMCAP1_WRITEVDATA(keyvalue_vbuffer_t * wideword, value_t vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILP1_WRITEBITSTO_UINTV(wideword, OFFSETOF_VDATA, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void acts_all::MEMCAP1_WRITEVMASK(keyvalue_vbuffer_t * wideword, unit1_type vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILP1_WRITEBITSTO_UINTV(wideword, OFFSETOF_VMASK, SIZEOF_VMASK, vmask);
	#endif 
	return;
}
void acts_all::MEMCAP1_WRITEVDATA2(keyvalue_vbuffer_t * wideword, value_t vdata, unsigned int offsetof_vdata){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	*wideword = vdata;
	#else 
	UTILP1_WRITEBITSTO_UINTV(wideword, offsetof_vdata, SIZEOF_VDATA, vdata);
	#endif 
	return;
}
void acts_all::MEMCAP1_WRITEVMASK2(keyvalue_vbuffer_t * wideword, unit1_type vmask, unsigned int offsetof_vmask){
	#pragma HLS INLINE
	#ifdef CONFIG_BITMASK_NOT_USED
	// NAp
	#else 
	UTILP1_WRITEBITSTO_UINTV(wideword, offsetof_vmask, SIZEOF_VMASK, vmask);
	#endif 
	return;
}

unsigned int acts_all::MEMCAP1_READEDIR(unsigned int wideword){
	#pragma HLS INLINE
	return UTILP1_READBITSFROM_UINTV(wideword, OFFSETOF_EDIR, SIZEOF_EDIR);
}

// non-primitives
vmdata_t acts_all::MEMCAP1_READFROMBUFFER_VDATAWITHVMASK(keyvalue_vbuffer_t bits_vector){
	#pragma HLS INLINE
	vmdata_t vmdata;
	vmdata.vdata = MEMCAP1_READVDATA(bits_vector); 
	vmdata.vmask = MEMCAP1_READVMASK(bits_vector); 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	if(vmdata.vmask == 1){ cout<<">>> MEMCAP1_READFROMBUFFER_VDATAWITHVMASK:: ACTIVE VERTEX READ: @ index: "<<index<<endl; }
	#endif
	return vmdata;
}
void acts_all::MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){				
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK:", index/2, MAX_BLOCKRAM_VDESTDATA_SIZE, index, NAp, NAp);
	#endif

	MEMCAP1_WRITEVDATA(&bits_vector, vdata);
	MEMCAP1_WRITEVMASK(&bits_vector, vmask);
	buffer[bufferoffset_kvs + index] = bits_vector;
	return;
}
void acts_all::MEMCAP1_WRITETODRAM_VDATAWITHVMASK(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
	#pragma HLS INLINE
	// {1st 16 is masks}{2nd 16 is vdatas}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("MEMCAP1_WRITETODRAM_VDATAWITHVMASK:", index, MAX_MAXHBMCAPACITY_KVS, index, NAp, NAp);
	#endif

	MEMCAP1_WRITEVDATA(&bits_vector, vdata);
	MEMCAP1_WRITEVMASK(&bits_vector, vmask);
	UTILP1_WriteData(kvdram, dramoffset_kvs, (index / VDATA_SHRINK_RATIO), bits_vector);
	return;
}

vmdata_t acts_all::MEMCAP1_READFROMBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t bits_vector){
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
void acts_all::MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK2(unsigned int index, keyvalue_vbuffer_t buffer[MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type bufferoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
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
void acts_all::MEMCAP1_WRITETODRAM_VDATAWITHVMASK2(unsigned int index, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t bits_vector, value_t vdata, unit1_type vmask){
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
	
	UTILP1_WriteData(kvdram, dramoffset_kvs, (index / VDATA_SHRINK_RATIO), bits_vector);
	return;
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_MEM_ACCESS
// -------------------- key values -------------------- //
fetchmessage_t acts_all::MEMACCESSP1_readkeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE], batch_type offset_kvs, batch_type size_kvs, travstate_t travstate, globalparams_t globalparams){
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

void acts_all::MEMACCESSP1_savekeyvalues(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t destbuffer[VECTOR_SIZE][MAX_DESTBLOCKRAM_SIZE], keyvalue_t globalcapsule[MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_dest[MAX_NUM_PARTITIONS], batch_type destbaseaddr_kvs, globalposition_t globalposition, globalparams_t globalparams){
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
			
			UTILP1_WriteDatas(kvdram, destbaseaddr_kvs + dramoffset_kvs, updates);
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
void acts_all::MEMACCESSP1_GetXYLayoutV(unsigned int s, vmdata_t vdata[VECTOR2_SIZE], vmdata_t vdata2[VECTOR2_SIZE], unsigned int depths[VECTOR2_SIZE], unsigned int basedepth){
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

void acts_all::MEMACCESSP1_RearrangeLayoutV(unsigned int s, keyvalue_vbuffer_t vdata[VECTOR2_SIZE], keyvalue_vbuffer_t vdata2[VECTOR2_SIZE]){
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

void acts_all::MEMACCESSP1_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){
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

void acts_all::MEMACCESSP1_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){				
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

void acts_all::MEMACCESSP1_readANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks1 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks1 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks1 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks1 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks1 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks2 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks2 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks2 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks2 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks2 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks3 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks3 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks3 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks3 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks3 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks4 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks4 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks4 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks4 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks4 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
	unsigned int basedepth = 0;
	unsigned int depths[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=depths complete
	unsigned int index2 = 0;
	#pragma HLS array_partition variable = buffer0
	#pragma HLS array_partition variable = buffer1
	#pragma HLS array_partition variable = buffer2
	#pragma HLS array_partition variable = buffer3
	#pragma HLS array_partition variable = buffer4
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks5 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks5 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks5 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks5 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks5 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];
			buffer4[0][bdepth_i + i] = vdata[0];
			buffer4[1][bdepth_i + i] = vdata[1];
			buffer4[2][bdepth_i + i] = vdata[2];
			buffer4[3][bdepth_i + i] = vdata[3];
			buffer4[4][bdepth_i + i] = vdata[4];
			buffer4[5][bdepth_i + i] = vdata[5];
			buffer4[6][bdepth_i + i] = vdata[6];
			buffer4[7][bdepth_i + i] = vdata[7];
			buffer4[8][bdepth_i + i] = vdata[8];
			buffer4[9][bdepth_i + i] = vdata[9];
			buffer4[10][bdepth_i + i] = vdata[10];
			buffer4[11][bdepth_i + i] = vdata[11];
			buffer4[12][bdepth_i + i] = vdata[12];
			buffer4[13][bdepth_i + i] = vdata[13];
			buffer4[14][bdepth_i + i] = vdata[14];
			buffer4[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
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
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks6 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks6 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks6 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks6 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks6 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];
			buffer4[0][bdepth_i + i] = vdata[0];
			buffer4[1][bdepth_i + i] = vdata[1];
			buffer4[2][bdepth_i + i] = vdata[2];
			buffer4[3][bdepth_i + i] = vdata[3];
			buffer4[4][bdepth_i + i] = vdata[4];
			buffer4[5][bdepth_i + i] = vdata[5];
			buffer4[6][bdepth_i + i] = vdata[6];
			buffer4[7][bdepth_i + i] = vdata[7];
			buffer4[8][bdepth_i + i] = vdata[8];
			buffer4[9][bdepth_i + i] = vdata[9];
			buffer4[10][bdepth_i + i] = vdata[10];
			buffer4[11][bdepth_i + i] = vdata[11];
			buffer4[12][bdepth_i + i] = vdata[12];
			buffer4[13][bdepth_i + i] = vdata[13];
			buffer4[14][bdepth_i + i] = vdata[14];
			buffer4[15][bdepth_i + i] = vdata[15];
			buffer5[0][bdepth_i + i] = vdata[0];
			buffer5[1][bdepth_i + i] = vdata[1];
			buffer5[2][bdepth_i + i] = vdata[2];
			buffer5[3][bdepth_i + i] = vdata[3];
			buffer5[4][bdepth_i + i] = vdata[4];
			buffer5[5][bdepth_i + i] = vdata[5];
			buffer5[6][bdepth_i + i] = vdata[6];
			buffer5[7][bdepth_i + i] = vdata[7];
			buffer5[8][bdepth_i + i] = vdata[8];
			buffer5[9][bdepth_i + i] = vdata[9];
			buffer5[10][bdepth_i + i] = vdata[10];
			buffer5[11][bdepth_i + i] = vdata[11];
			buffer5[12][bdepth_i + i] = vdata[12];
			buffer5[13][bdepth_i + i] = vdata[13];
			buffer5[14][bdepth_i + i] = vdata[14];
			buffer5[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
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
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks7 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks7 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks7 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks7 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks7 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];
			buffer4[0][bdepth_i + i] = vdata[0];
			buffer4[1][bdepth_i + i] = vdata[1];
			buffer4[2][bdepth_i + i] = vdata[2];
			buffer4[3][bdepth_i + i] = vdata[3];
			buffer4[4][bdepth_i + i] = vdata[4];
			buffer4[5][bdepth_i + i] = vdata[5];
			buffer4[6][bdepth_i + i] = vdata[6];
			buffer4[7][bdepth_i + i] = vdata[7];
			buffer4[8][bdepth_i + i] = vdata[8];
			buffer4[9][bdepth_i + i] = vdata[9];
			buffer4[10][bdepth_i + i] = vdata[10];
			buffer4[11][bdepth_i + i] = vdata[11];
			buffer4[12][bdepth_i + i] = vdata[12];
			buffer4[13][bdepth_i + i] = vdata[13];
			buffer4[14][bdepth_i + i] = vdata[14];
			buffer4[15][bdepth_i + i] = vdata[15];
			buffer5[0][bdepth_i + i] = vdata[0];
			buffer5[1][bdepth_i + i] = vdata[1];
			buffer5[2][bdepth_i + i] = vdata[2];
			buffer5[3][bdepth_i + i] = vdata[3];
			buffer5[4][bdepth_i + i] = vdata[4];
			buffer5[5][bdepth_i + i] = vdata[5];
			buffer5[6][bdepth_i + i] = vdata[6];
			buffer5[7][bdepth_i + i] = vdata[7];
			buffer5[8][bdepth_i + i] = vdata[8];
			buffer5[9][bdepth_i + i] = vdata[9];
			buffer5[10][bdepth_i + i] = vdata[10];
			buffer5[11][bdepth_i + i] = vdata[11];
			buffer5[12][bdepth_i + i] = vdata[12];
			buffer5[13][bdepth_i + i] = vdata[13];
			buffer5[14][bdepth_i + i] = vdata[14];
			buffer5[15][bdepth_i + i] = vdata[15];
			buffer6[0][bdepth_i + i] = vdata[0];
			buffer6[1][bdepth_i + i] = vdata[1];
			buffer6[2][bdepth_i + i] = vdata[2];
			buffer6[3][bdepth_i + i] = vdata[3];
			buffer6[4][bdepth_i + i] = vdata[4];
			buffer6[5][bdepth_i + i] = vdata[5];
			buffer6[6][bdepth_i + i] = vdata[6];
			buffer6[7][bdepth_i + i] = vdata[7];
			buffer6[8][bdepth_i + i] = vdata[8];
			buffer6[9][bdepth_i + i] = vdata[9];
			buffer6[10][bdepth_i + i] = vdata[10];
			buffer6[11][bdepth_i + i] = vdata[11];
			buffer6[12][bdepth_i + i] = vdata[12];
			buffer6[13][bdepth_i + i] = vdata[13];
			buffer6[14][bdepth_i + i] = vdata[14];
			buffer6[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
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
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks8 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks8 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks8 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks8 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks8 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];
			buffer4[0][bdepth_i + i] = vdata[0];
			buffer4[1][bdepth_i + i] = vdata[1];
			buffer4[2][bdepth_i + i] = vdata[2];
			buffer4[3][bdepth_i + i] = vdata[3];
			buffer4[4][bdepth_i + i] = vdata[4];
			buffer4[5][bdepth_i + i] = vdata[5];
			buffer4[6][bdepth_i + i] = vdata[6];
			buffer4[7][bdepth_i + i] = vdata[7];
			buffer4[8][bdepth_i + i] = vdata[8];
			buffer4[9][bdepth_i + i] = vdata[9];
			buffer4[10][bdepth_i + i] = vdata[10];
			buffer4[11][bdepth_i + i] = vdata[11];
			buffer4[12][bdepth_i + i] = vdata[12];
			buffer4[13][bdepth_i + i] = vdata[13];
			buffer4[14][bdepth_i + i] = vdata[14];
			buffer4[15][bdepth_i + i] = vdata[15];
			buffer5[0][bdepth_i + i] = vdata[0];
			buffer5[1][bdepth_i + i] = vdata[1];
			buffer5[2][bdepth_i + i] = vdata[2];
			buffer5[3][bdepth_i + i] = vdata[3];
			buffer5[4][bdepth_i + i] = vdata[4];
			buffer5[5][bdepth_i + i] = vdata[5];
			buffer5[6][bdepth_i + i] = vdata[6];
			buffer5[7][bdepth_i + i] = vdata[7];
			buffer5[8][bdepth_i + i] = vdata[8];
			buffer5[9][bdepth_i + i] = vdata[9];
			buffer5[10][bdepth_i + i] = vdata[10];
			buffer5[11][bdepth_i + i] = vdata[11];
			buffer5[12][bdepth_i + i] = vdata[12];
			buffer5[13][bdepth_i + i] = vdata[13];
			buffer5[14][bdepth_i + i] = vdata[14];
			buffer5[15][bdepth_i + i] = vdata[15];
			buffer6[0][bdepth_i + i] = vdata[0];
			buffer6[1][bdepth_i + i] = vdata[1];
			buffer6[2][bdepth_i + i] = vdata[2];
			buffer6[3][bdepth_i + i] = vdata[3];
			buffer6[4][bdepth_i + i] = vdata[4];
			buffer6[5][bdepth_i + i] = vdata[5];
			buffer6[6][bdepth_i + i] = vdata[6];
			buffer6[7][bdepth_i + i] = vdata[7];
			buffer6[8][bdepth_i + i] = vdata[8];
			buffer6[9][bdepth_i + i] = vdata[9];
			buffer6[10][bdepth_i + i] = vdata[10];
			buffer6[11][bdepth_i + i] = vdata[11];
			buffer6[12][bdepth_i + i] = vdata[12];
			buffer6[13][bdepth_i + i] = vdata[13];
			buffer6[14][bdepth_i + i] = vdata[14];
			buffer6[15][bdepth_i + i] = vdata[15];
			buffer7[0][bdepth_i + i] = vdata[0];
			buffer7[1][bdepth_i + i] = vdata[1];
			buffer7[2][bdepth_i + i] = vdata[2];
			buffer7[3][bdepth_i + i] = vdata[3];
			buffer7[4][bdepth_i + i] = vdata[4];
			buffer7[5][bdepth_i + i] = vdata[5];
			buffer7[6][bdepth_i + i] = vdata[6];
			buffer7[7][bdepth_i + i] = vdata[7];
			buffer7[8][bdepth_i + i] = vdata[8];
			buffer7[9][bdepth_i + i] = vdata[9];
			buffer7[10][bdepth_i + i] = vdata[10];
			buffer7[11][bdepth_i + i] = vdata[11];
			buffer7[12][bdepth_i + i] = vdata[12];
			buffer7[13][bdepth_i + i] = vdata[13];
			buffer7[14][bdepth_i + i] = vdata[14];
			buffer7[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
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
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks9 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks9 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks9 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks9 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks9 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];
			buffer4[0][bdepth_i + i] = vdata[0];
			buffer4[1][bdepth_i + i] = vdata[1];
			buffer4[2][bdepth_i + i] = vdata[2];
			buffer4[3][bdepth_i + i] = vdata[3];
			buffer4[4][bdepth_i + i] = vdata[4];
			buffer4[5][bdepth_i + i] = vdata[5];
			buffer4[6][bdepth_i + i] = vdata[6];
			buffer4[7][bdepth_i + i] = vdata[7];
			buffer4[8][bdepth_i + i] = vdata[8];
			buffer4[9][bdepth_i + i] = vdata[9];
			buffer4[10][bdepth_i + i] = vdata[10];
			buffer4[11][bdepth_i + i] = vdata[11];
			buffer4[12][bdepth_i + i] = vdata[12];
			buffer4[13][bdepth_i + i] = vdata[13];
			buffer4[14][bdepth_i + i] = vdata[14];
			buffer4[15][bdepth_i + i] = vdata[15];
			buffer5[0][bdepth_i + i] = vdata[0];
			buffer5[1][bdepth_i + i] = vdata[1];
			buffer5[2][bdepth_i + i] = vdata[2];
			buffer5[3][bdepth_i + i] = vdata[3];
			buffer5[4][bdepth_i + i] = vdata[4];
			buffer5[5][bdepth_i + i] = vdata[5];
			buffer5[6][bdepth_i + i] = vdata[6];
			buffer5[7][bdepth_i + i] = vdata[7];
			buffer5[8][bdepth_i + i] = vdata[8];
			buffer5[9][bdepth_i + i] = vdata[9];
			buffer5[10][bdepth_i + i] = vdata[10];
			buffer5[11][bdepth_i + i] = vdata[11];
			buffer5[12][bdepth_i + i] = vdata[12];
			buffer5[13][bdepth_i + i] = vdata[13];
			buffer5[14][bdepth_i + i] = vdata[14];
			buffer5[15][bdepth_i + i] = vdata[15];
			buffer6[0][bdepth_i + i] = vdata[0];
			buffer6[1][bdepth_i + i] = vdata[1];
			buffer6[2][bdepth_i + i] = vdata[2];
			buffer6[3][bdepth_i + i] = vdata[3];
			buffer6[4][bdepth_i + i] = vdata[4];
			buffer6[5][bdepth_i + i] = vdata[5];
			buffer6[6][bdepth_i + i] = vdata[6];
			buffer6[7][bdepth_i + i] = vdata[7];
			buffer6[8][bdepth_i + i] = vdata[8];
			buffer6[9][bdepth_i + i] = vdata[9];
			buffer6[10][bdepth_i + i] = vdata[10];
			buffer6[11][bdepth_i + i] = vdata[11];
			buffer6[12][bdepth_i + i] = vdata[12];
			buffer6[13][bdepth_i + i] = vdata[13];
			buffer6[14][bdepth_i + i] = vdata[14];
			buffer6[15][bdepth_i + i] = vdata[15];
			buffer7[0][bdepth_i + i] = vdata[0];
			buffer7[1][bdepth_i + i] = vdata[1];
			buffer7[2][bdepth_i + i] = vdata[2];
			buffer7[3][bdepth_i + i] = vdata[3];
			buffer7[4][bdepth_i + i] = vdata[4];
			buffer7[5][bdepth_i + i] = vdata[5];
			buffer7[6][bdepth_i + i] = vdata[6];
			buffer7[7][bdepth_i + i] = vdata[7];
			buffer7[8][bdepth_i + i] = vdata[8];
			buffer7[9][bdepth_i + i] = vdata[9];
			buffer7[10][bdepth_i + i] = vdata[10];
			buffer7[11][bdepth_i + i] = vdata[11];
			buffer7[12][bdepth_i + i] = vdata[12];
			buffer7[13][bdepth_i + i] = vdata[13];
			buffer7[14][bdepth_i + i] = vdata[14];
			buffer7[15][bdepth_i + i] = vdata[15];
			buffer8[0][bdepth_i + i] = vdata[0];
			buffer8[1][bdepth_i + i] = vdata[1];
			buffer8[2][bdepth_i + i] = vdata[2];
			buffer8[3][bdepth_i + i] = vdata[3];
			buffer8[4][bdepth_i + i] = vdata[4];
			buffer8[5][bdepth_i + i] = vdata[5];
			buffer8[6][bdepth_i + i] = vdata[6];
			buffer8[7][bdepth_i + i] = vdata[7];
			buffer8[8][bdepth_i + i] = vdata[8];
			buffer8[9][bdepth_i + i] = vdata[9];
			buffer8[10][bdepth_i + i] = vdata[10];
			buffer8[11][bdepth_i + i] = vdata[11];
			buffer8[12][bdepth_i + i] = vdata[12];
			buffer8[13][bdepth_i + i] = vdata[13];
			buffer8[14][bdepth_i + i] = vdata[14];
			buffer8[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
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
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks10 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks10 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks10 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks10 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks10 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];
			buffer4[0][bdepth_i + i] = vdata[0];
			buffer4[1][bdepth_i + i] = vdata[1];
			buffer4[2][bdepth_i + i] = vdata[2];
			buffer4[3][bdepth_i + i] = vdata[3];
			buffer4[4][bdepth_i + i] = vdata[4];
			buffer4[5][bdepth_i + i] = vdata[5];
			buffer4[6][bdepth_i + i] = vdata[6];
			buffer4[7][bdepth_i + i] = vdata[7];
			buffer4[8][bdepth_i + i] = vdata[8];
			buffer4[9][bdepth_i + i] = vdata[9];
			buffer4[10][bdepth_i + i] = vdata[10];
			buffer4[11][bdepth_i + i] = vdata[11];
			buffer4[12][bdepth_i + i] = vdata[12];
			buffer4[13][bdepth_i + i] = vdata[13];
			buffer4[14][bdepth_i + i] = vdata[14];
			buffer4[15][bdepth_i + i] = vdata[15];
			buffer5[0][bdepth_i + i] = vdata[0];
			buffer5[1][bdepth_i + i] = vdata[1];
			buffer5[2][bdepth_i + i] = vdata[2];
			buffer5[3][bdepth_i + i] = vdata[3];
			buffer5[4][bdepth_i + i] = vdata[4];
			buffer5[5][bdepth_i + i] = vdata[5];
			buffer5[6][bdepth_i + i] = vdata[6];
			buffer5[7][bdepth_i + i] = vdata[7];
			buffer5[8][bdepth_i + i] = vdata[8];
			buffer5[9][bdepth_i + i] = vdata[9];
			buffer5[10][bdepth_i + i] = vdata[10];
			buffer5[11][bdepth_i + i] = vdata[11];
			buffer5[12][bdepth_i + i] = vdata[12];
			buffer5[13][bdepth_i + i] = vdata[13];
			buffer5[14][bdepth_i + i] = vdata[14];
			buffer5[15][bdepth_i + i] = vdata[15];
			buffer6[0][bdepth_i + i] = vdata[0];
			buffer6[1][bdepth_i + i] = vdata[1];
			buffer6[2][bdepth_i + i] = vdata[2];
			buffer6[3][bdepth_i + i] = vdata[3];
			buffer6[4][bdepth_i + i] = vdata[4];
			buffer6[5][bdepth_i + i] = vdata[5];
			buffer6[6][bdepth_i + i] = vdata[6];
			buffer6[7][bdepth_i + i] = vdata[7];
			buffer6[8][bdepth_i + i] = vdata[8];
			buffer6[9][bdepth_i + i] = vdata[9];
			buffer6[10][bdepth_i + i] = vdata[10];
			buffer6[11][bdepth_i + i] = vdata[11];
			buffer6[12][bdepth_i + i] = vdata[12];
			buffer6[13][bdepth_i + i] = vdata[13];
			buffer6[14][bdepth_i + i] = vdata[14];
			buffer6[15][bdepth_i + i] = vdata[15];
			buffer7[0][bdepth_i + i] = vdata[0];
			buffer7[1][bdepth_i + i] = vdata[1];
			buffer7[2][bdepth_i + i] = vdata[2];
			buffer7[3][bdepth_i + i] = vdata[3];
			buffer7[4][bdepth_i + i] = vdata[4];
			buffer7[5][bdepth_i + i] = vdata[5];
			buffer7[6][bdepth_i + i] = vdata[6];
			buffer7[7][bdepth_i + i] = vdata[7];
			buffer7[8][bdepth_i + i] = vdata[8];
			buffer7[9][bdepth_i + i] = vdata[9];
			buffer7[10][bdepth_i + i] = vdata[10];
			buffer7[11][bdepth_i + i] = vdata[11];
			buffer7[12][bdepth_i + i] = vdata[12];
			buffer7[13][bdepth_i + i] = vdata[13];
			buffer7[14][bdepth_i + i] = vdata[14];
			buffer7[15][bdepth_i + i] = vdata[15];
			buffer8[0][bdepth_i + i] = vdata[0];
			buffer8[1][bdepth_i + i] = vdata[1];
			buffer8[2][bdepth_i + i] = vdata[2];
			buffer8[3][bdepth_i + i] = vdata[3];
			buffer8[4][bdepth_i + i] = vdata[4];
			buffer8[5][bdepth_i + i] = vdata[5];
			buffer8[6][bdepth_i + i] = vdata[6];
			buffer8[7][bdepth_i + i] = vdata[7];
			buffer8[8][bdepth_i + i] = vdata[8];
			buffer8[9][bdepth_i + i] = vdata[9];
			buffer8[10][bdepth_i + i] = vdata[10];
			buffer8[11][bdepth_i + i] = vdata[11];
			buffer8[12][bdepth_i + i] = vdata[12];
			buffer8[13][bdepth_i + i] = vdata[13];
			buffer8[14][bdepth_i + i] = vdata[14];
			buffer8[15][bdepth_i + i] = vdata[15];
			buffer9[0][bdepth_i + i] = vdata[0];
			buffer9[1][bdepth_i + i] = vdata[1];
			buffer9[2][bdepth_i + i] = vdata[2];
			buffer9[3][bdepth_i + i] = vdata[3];
			buffer9[4][bdepth_i + i] = vdata[4];
			buffer9[5][bdepth_i + i] = vdata[5];
			buffer9[6][bdepth_i + i] = vdata[6];
			buffer9[7][bdepth_i + i] = vdata[7];
			buffer9[8][bdepth_i + i] = vdata[8];
			buffer9[9][bdepth_i + i] = vdata[9];
			buffer9[10][bdepth_i + i] = vdata[10];
			buffer9[11][bdepth_i + i] = vdata[11];
			buffer9[12][bdepth_i + i] = vdata[12];
			buffer9[13][bdepth_i + i] = vdata[13];
			buffer9[14][bdepth_i + i] = vdata[14];
			buffer9[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
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
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks11 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks11 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks11 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks11 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks11 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];
			buffer4[0][bdepth_i + i] = vdata[0];
			buffer4[1][bdepth_i + i] = vdata[1];
			buffer4[2][bdepth_i + i] = vdata[2];
			buffer4[3][bdepth_i + i] = vdata[3];
			buffer4[4][bdepth_i + i] = vdata[4];
			buffer4[5][bdepth_i + i] = vdata[5];
			buffer4[6][bdepth_i + i] = vdata[6];
			buffer4[7][bdepth_i + i] = vdata[7];
			buffer4[8][bdepth_i + i] = vdata[8];
			buffer4[9][bdepth_i + i] = vdata[9];
			buffer4[10][bdepth_i + i] = vdata[10];
			buffer4[11][bdepth_i + i] = vdata[11];
			buffer4[12][bdepth_i + i] = vdata[12];
			buffer4[13][bdepth_i + i] = vdata[13];
			buffer4[14][bdepth_i + i] = vdata[14];
			buffer4[15][bdepth_i + i] = vdata[15];
			buffer5[0][bdepth_i + i] = vdata[0];
			buffer5[1][bdepth_i + i] = vdata[1];
			buffer5[2][bdepth_i + i] = vdata[2];
			buffer5[3][bdepth_i + i] = vdata[3];
			buffer5[4][bdepth_i + i] = vdata[4];
			buffer5[5][bdepth_i + i] = vdata[5];
			buffer5[6][bdepth_i + i] = vdata[6];
			buffer5[7][bdepth_i + i] = vdata[7];
			buffer5[8][bdepth_i + i] = vdata[8];
			buffer5[9][bdepth_i + i] = vdata[9];
			buffer5[10][bdepth_i + i] = vdata[10];
			buffer5[11][bdepth_i + i] = vdata[11];
			buffer5[12][bdepth_i + i] = vdata[12];
			buffer5[13][bdepth_i + i] = vdata[13];
			buffer5[14][bdepth_i + i] = vdata[14];
			buffer5[15][bdepth_i + i] = vdata[15];
			buffer6[0][bdepth_i + i] = vdata[0];
			buffer6[1][bdepth_i + i] = vdata[1];
			buffer6[2][bdepth_i + i] = vdata[2];
			buffer6[3][bdepth_i + i] = vdata[3];
			buffer6[4][bdepth_i + i] = vdata[4];
			buffer6[5][bdepth_i + i] = vdata[5];
			buffer6[6][bdepth_i + i] = vdata[6];
			buffer6[7][bdepth_i + i] = vdata[7];
			buffer6[8][bdepth_i + i] = vdata[8];
			buffer6[9][bdepth_i + i] = vdata[9];
			buffer6[10][bdepth_i + i] = vdata[10];
			buffer6[11][bdepth_i + i] = vdata[11];
			buffer6[12][bdepth_i + i] = vdata[12];
			buffer6[13][bdepth_i + i] = vdata[13];
			buffer6[14][bdepth_i + i] = vdata[14];
			buffer6[15][bdepth_i + i] = vdata[15];
			buffer7[0][bdepth_i + i] = vdata[0];
			buffer7[1][bdepth_i + i] = vdata[1];
			buffer7[2][bdepth_i + i] = vdata[2];
			buffer7[3][bdepth_i + i] = vdata[3];
			buffer7[4][bdepth_i + i] = vdata[4];
			buffer7[5][bdepth_i + i] = vdata[5];
			buffer7[6][bdepth_i + i] = vdata[6];
			buffer7[7][bdepth_i + i] = vdata[7];
			buffer7[8][bdepth_i + i] = vdata[8];
			buffer7[9][bdepth_i + i] = vdata[9];
			buffer7[10][bdepth_i + i] = vdata[10];
			buffer7[11][bdepth_i + i] = vdata[11];
			buffer7[12][bdepth_i + i] = vdata[12];
			buffer7[13][bdepth_i + i] = vdata[13];
			buffer7[14][bdepth_i + i] = vdata[14];
			buffer7[15][bdepth_i + i] = vdata[15];
			buffer8[0][bdepth_i + i] = vdata[0];
			buffer8[1][bdepth_i + i] = vdata[1];
			buffer8[2][bdepth_i + i] = vdata[2];
			buffer8[3][bdepth_i + i] = vdata[3];
			buffer8[4][bdepth_i + i] = vdata[4];
			buffer8[5][bdepth_i + i] = vdata[5];
			buffer8[6][bdepth_i + i] = vdata[6];
			buffer8[7][bdepth_i + i] = vdata[7];
			buffer8[8][bdepth_i + i] = vdata[8];
			buffer8[9][bdepth_i + i] = vdata[9];
			buffer8[10][bdepth_i + i] = vdata[10];
			buffer8[11][bdepth_i + i] = vdata[11];
			buffer8[12][bdepth_i + i] = vdata[12];
			buffer8[13][bdepth_i + i] = vdata[13];
			buffer8[14][bdepth_i + i] = vdata[14];
			buffer8[15][bdepth_i + i] = vdata[15];
			buffer9[0][bdepth_i + i] = vdata[0];
			buffer9[1][bdepth_i + i] = vdata[1];
			buffer9[2][bdepth_i + i] = vdata[2];
			buffer9[3][bdepth_i + i] = vdata[3];
			buffer9[4][bdepth_i + i] = vdata[4];
			buffer9[5][bdepth_i + i] = vdata[5];
			buffer9[6][bdepth_i + i] = vdata[6];
			buffer9[7][bdepth_i + i] = vdata[7];
			buffer9[8][bdepth_i + i] = vdata[8];
			buffer9[9][bdepth_i + i] = vdata[9];
			buffer9[10][bdepth_i + i] = vdata[10];
			buffer9[11][bdepth_i + i] = vdata[11];
			buffer9[12][bdepth_i + i] = vdata[12];
			buffer9[13][bdepth_i + i] = vdata[13];
			buffer9[14][bdepth_i + i] = vdata[14];
			buffer9[15][bdepth_i + i] = vdata[15];
			buffer10[0][bdepth_i + i] = vdata[0];
			buffer10[1][bdepth_i + i] = vdata[1];
			buffer10[2][bdepth_i + i] = vdata[2];
			buffer10[3][bdepth_i + i] = vdata[3];
			buffer10[4][bdepth_i + i] = vdata[4];
			buffer10[5][bdepth_i + i] = vdata[5];
			buffer10[6][bdepth_i + i] = vdata[6];
			buffer10[7][bdepth_i + i] = vdata[7];
			buffer10[8][bdepth_i + i] = vdata[8];
			buffer10[9][bdepth_i + i] = vdata[9];
			buffer10[10][bdepth_i + i] = vdata[10];
			buffer10[11][bdepth_i + i] = vdata[11];
			buffer10[12][bdepth_i + i] = vdata[12];
			buffer10[13][bdepth_i + i] = vdata[13];
			buffer10[14][bdepth_i + i] = vdata[14];
			buffer10[15][bdepth_i + i] = vdata[15];

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
		bdepth_i += bdepth;
	}
	return;
}
void acts_all::MEMACCESSP1_readANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	value_t vdata[VECTOR2_SIZE];
	value_t vdata2[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	
	// cout<<"------------- MEMACCESSP1_readANDRVchunks:: globalparamsV.NUM_REDUCEPARTITIONS "<<globalparamsV.NUM_REDUCEPARTITIONS<<", globalparamsV.SIZEKVS2_REDUCEPARTITION: "<<globalparamsV.SIZEKVS2_REDUCEPARTITION<<", VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<endl;
	// return; // CRITICAL REMOVEME URGENT.
	
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
	
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	unsigned int bdepth = vsz_kvs;
	unsigned int bdepth_skip = globalparamsV.SIZEKVS2_PROCESSEDGESPARTITION / VDATA_SHRINK_RATIO;
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP1_readANDRVchunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs = 0;
			if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			#ifdef _DEBUGMODE_CHECKS3
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks12 23", vbaseoffset_kvs + voffset_kvs + depth_i + i, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			// actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks12 24", voffset_kvs + depth_i + i, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks12 23", vbaseoffset_kvs + offset_kvs, globalparamsV.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks12 24", offset_kvs, globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, vbaseoffset_kvs, voffset_kvs, depth_i);
			#endif
			
			// UTILP1_ReadDatas(vdram, vbaseoffset_kvs + voffset_kvs + depth_i + i, vdata);	
			UTILP1_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, vdata);	
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(MEMCAP1_READVMASK(vdata[0]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 0"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[1]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 1"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[2]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 2"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[3]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 3"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[4]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 4"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[5]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 5"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[6]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 6"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[7]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 7"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[8]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 8"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[9]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 9"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[10]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 10"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[11]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 11"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[12]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 12"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[13]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 13"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[14]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 14"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			if(MEMCAP1_READVMASK(vdata[15]) == 1){ cout<<"READANDRVCHUNKS: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<", ind: "<<(s*vsz_kvs)+i<<", v: 15"<<", vbaseoffset_kvs: "<<vbaseoffset_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth_i: "<<depth_i<<", all: "<<(vbaseoffset_kvs + voffset_kvs + depth_i)<<endl; }
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readANDRVchunks12 25", bdepth_i + i, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, NAp, NAp);
			#endif
			buffer0[0][bdepth_i + i] = vdata[0];
			buffer0[1][bdepth_i + i] = vdata[1];
			buffer0[2][bdepth_i + i] = vdata[2];
			buffer0[3][bdepth_i + i] = vdata[3];
			buffer0[4][bdepth_i + i] = vdata[4];
			buffer0[5][bdepth_i + i] = vdata[5];
			buffer0[6][bdepth_i + i] = vdata[6];
			buffer0[7][bdepth_i + i] = vdata[7];
			buffer0[8][bdepth_i + i] = vdata[8];
			buffer0[9][bdepth_i + i] = vdata[9];
			buffer0[10][bdepth_i + i] = vdata[10];
			buffer0[11][bdepth_i + i] = vdata[11];
			buffer0[12][bdepth_i + i] = vdata[12];
			buffer0[13][bdepth_i + i] = vdata[13];
			buffer0[14][bdepth_i + i] = vdata[14];
			buffer0[15][bdepth_i + i] = vdata[15];
			buffer1[0][bdepth_i + i] = vdata[0];
			buffer1[1][bdepth_i + i] = vdata[1];
			buffer1[2][bdepth_i + i] = vdata[2];
			buffer1[3][bdepth_i + i] = vdata[3];
			buffer1[4][bdepth_i + i] = vdata[4];
			buffer1[5][bdepth_i + i] = vdata[5];
			buffer1[6][bdepth_i + i] = vdata[6];
			buffer1[7][bdepth_i + i] = vdata[7];
			buffer1[8][bdepth_i + i] = vdata[8];
			buffer1[9][bdepth_i + i] = vdata[9];
			buffer1[10][bdepth_i + i] = vdata[10];
			buffer1[11][bdepth_i + i] = vdata[11];
			buffer1[12][bdepth_i + i] = vdata[12];
			buffer1[13][bdepth_i + i] = vdata[13];
			buffer1[14][bdepth_i + i] = vdata[14];
			buffer1[15][bdepth_i + i] = vdata[15];
			buffer2[0][bdepth_i + i] = vdata[0];
			buffer2[1][bdepth_i + i] = vdata[1];
			buffer2[2][bdepth_i + i] = vdata[2];
			buffer2[3][bdepth_i + i] = vdata[3];
			buffer2[4][bdepth_i + i] = vdata[4];
			buffer2[5][bdepth_i + i] = vdata[5];
			buffer2[6][bdepth_i + i] = vdata[6];
			buffer2[7][bdepth_i + i] = vdata[7];
			buffer2[8][bdepth_i + i] = vdata[8];
			buffer2[9][bdepth_i + i] = vdata[9];
			buffer2[10][bdepth_i + i] = vdata[10];
			buffer2[11][bdepth_i + i] = vdata[11];
			buffer2[12][bdepth_i + i] = vdata[12];
			buffer2[13][bdepth_i + i] = vdata[13];
			buffer2[14][bdepth_i + i] = vdata[14];
			buffer2[15][bdepth_i + i] = vdata[15];
			buffer3[0][bdepth_i + i] = vdata[0];
			buffer3[1][bdepth_i + i] = vdata[1];
			buffer3[2][bdepth_i + i] = vdata[2];
			buffer3[3][bdepth_i + i] = vdata[3];
			buffer3[4][bdepth_i + i] = vdata[4];
			buffer3[5][bdepth_i + i] = vdata[5];
			buffer3[6][bdepth_i + i] = vdata[6];
			buffer3[7][bdepth_i + i] = vdata[7];
			buffer3[8][bdepth_i + i] = vdata[8];
			buffer3[9][bdepth_i + i] = vdata[9];
			buffer3[10][bdepth_i + i] = vdata[10];
			buffer3[11][bdepth_i + i] = vdata[11];
			buffer3[12][bdepth_i + i] = vdata[12];
			buffer3[13][bdepth_i + i] = vdata[13];
			buffer3[14][bdepth_i + i] = vdata[14];
			buffer3[15][bdepth_i + i] = vdata[15];
			buffer4[0][bdepth_i + i] = vdata[0];
			buffer4[1][bdepth_i + i] = vdata[1];
			buffer4[2][bdepth_i + i] = vdata[2];
			buffer4[3][bdepth_i + i] = vdata[3];
			buffer4[4][bdepth_i + i] = vdata[4];
			buffer4[5][bdepth_i + i] = vdata[5];
			buffer4[6][bdepth_i + i] = vdata[6];
			buffer4[7][bdepth_i + i] = vdata[7];
			buffer4[8][bdepth_i + i] = vdata[8];
			buffer4[9][bdepth_i + i] = vdata[9];
			buffer4[10][bdepth_i + i] = vdata[10];
			buffer4[11][bdepth_i + i] = vdata[11];
			buffer4[12][bdepth_i + i] = vdata[12];
			buffer4[13][bdepth_i + i] = vdata[13];
			buffer4[14][bdepth_i + i] = vdata[14];
			buffer4[15][bdepth_i + i] = vdata[15];
			buffer5[0][bdepth_i + i] = vdata[0];
			buffer5[1][bdepth_i + i] = vdata[1];
			buffer5[2][bdepth_i + i] = vdata[2];
			buffer5[3][bdepth_i + i] = vdata[3];
			buffer5[4][bdepth_i + i] = vdata[4];
			buffer5[5][bdepth_i + i] = vdata[5];
			buffer5[6][bdepth_i + i] = vdata[6];
			buffer5[7][bdepth_i + i] = vdata[7];
			buffer5[8][bdepth_i + i] = vdata[8];
			buffer5[9][bdepth_i + i] = vdata[9];
			buffer5[10][bdepth_i + i] = vdata[10];
			buffer5[11][bdepth_i + i] = vdata[11];
			buffer5[12][bdepth_i + i] = vdata[12];
			buffer5[13][bdepth_i + i] = vdata[13];
			buffer5[14][bdepth_i + i] = vdata[14];
			buffer5[15][bdepth_i + i] = vdata[15];
			buffer6[0][bdepth_i + i] = vdata[0];
			buffer6[1][bdepth_i + i] = vdata[1];
			buffer6[2][bdepth_i + i] = vdata[2];
			buffer6[3][bdepth_i + i] = vdata[3];
			buffer6[4][bdepth_i + i] = vdata[4];
			buffer6[5][bdepth_i + i] = vdata[5];
			buffer6[6][bdepth_i + i] = vdata[6];
			buffer6[7][bdepth_i + i] = vdata[7];
			buffer6[8][bdepth_i + i] = vdata[8];
			buffer6[9][bdepth_i + i] = vdata[9];
			buffer6[10][bdepth_i + i] = vdata[10];
			buffer6[11][bdepth_i + i] = vdata[11];
			buffer6[12][bdepth_i + i] = vdata[12];
			buffer6[13][bdepth_i + i] = vdata[13];
			buffer6[14][bdepth_i + i] = vdata[14];
			buffer6[15][bdepth_i + i] = vdata[15];
			buffer7[0][bdepth_i + i] = vdata[0];
			buffer7[1][bdepth_i + i] = vdata[1];
			buffer7[2][bdepth_i + i] = vdata[2];
			buffer7[3][bdepth_i + i] = vdata[3];
			buffer7[4][bdepth_i + i] = vdata[4];
			buffer7[5][bdepth_i + i] = vdata[5];
			buffer7[6][bdepth_i + i] = vdata[6];
			buffer7[7][bdepth_i + i] = vdata[7];
			buffer7[8][bdepth_i + i] = vdata[8];
			buffer7[9][bdepth_i + i] = vdata[9];
			buffer7[10][bdepth_i + i] = vdata[10];
			buffer7[11][bdepth_i + i] = vdata[11];
			buffer7[12][bdepth_i + i] = vdata[12];
			buffer7[13][bdepth_i + i] = vdata[13];
			buffer7[14][bdepth_i + i] = vdata[14];
			buffer7[15][bdepth_i + i] = vdata[15];
			buffer8[0][bdepth_i + i] = vdata[0];
			buffer8[1][bdepth_i + i] = vdata[1];
			buffer8[2][bdepth_i + i] = vdata[2];
			buffer8[3][bdepth_i + i] = vdata[3];
			buffer8[4][bdepth_i + i] = vdata[4];
			buffer8[5][bdepth_i + i] = vdata[5];
			buffer8[6][bdepth_i + i] = vdata[6];
			buffer8[7][bdepth_i + i] = vdata[7];
			buffer8[8][bdepth_i + i] = vdata[8];
			buffer8[9][bdepth_i + i] = vdata[9];
			buffer8[10][bdepth_i + i] = vdata[10];
			buffer8[11][bdepth_i + i] = vdata[11];
			buffer8[12][bdepth_i + i] = vdata[12];
			buffer8[13][bdepth_i + i] = vdata[13];
			buffer8[14][bdepth_i + i] = vdata[14];
			buffer8[15][bdepth_i + i] = vdata[15];
			buffer9[0][bdepth_i + i] = vdata[0];
			buffer9[1][bdepth_i + i] = vdata[1];
			buffer9[2][bdepth_i + i] = vdata[2];
			buffer9[3][bdepth_i + i] = vdata[3];
			buffer9[4][bdepth_i + i] = vdata[4];
			buffer9[5][bdepth_i + i] = vdata[5];
			buffer9[6][bdepth_i + i] = vdata[6];
			buffer9[7][bdepth_i + i] = vdata[7];
			buffer9[8][bdepth_i + i] = vdata[8];
			buffer9[9][bdepth_i + i] = vdata[9];
			buffer9[10][bdepth_i + i] = vdata[10];
			buffer9[11][bdepth_i + i] = vdata[11];
			buffer9[12][bdepth_i + i] = vdata[12];
			buffer9[13][bdepth_i + i] = vdata[13];
			buffer9[14][bdepth_i + i] = vdata[14];
			buffer9[15][bdepth_i + i] = vdata[15];
			buffer10[0][bdepth_i + i] = vdata[0];
			buffer10[1][bdepth_i + i] = vdata[1];
			buffer10[2][bdepth_i + i] = vdata[2];
			buffer10[3][bdepth_i + i] = vdata[3];
			buffer10[4][bdepth_i + i] = vdata[4];
			buffer10[5][bdepth_i + i] = vdata[5];
			buffer10[6][bdepth_i + i] = vdata[6];
			buffer10[7][bdepth_i + i] = vdata[7];
			buffer10[8][bdepth_i + i] = vdata[8];
			buffer10[9][bdepth_i + i] = vdata[9];
			buffer10[10][bdepth_i + i] = vdata[10];
			buffer10[11][bdepth_i + i] = vdata[11];
			buffer10[12][bdepth_i + i] = vdata[12];
			buffer10[13][bdepth_i + i] = vdata[13];
			buffer10[14][bdepth_i + i] = vdata[14];
			buffer10[15][bdepth_i + i] = vdata[15];
			buffer11[0][bdepth_i + i] = vdata[0];
			buffer11[1][bdepth_i + i] = vdata[1];
			buffer11[2][bdepth_i + i] = vdata[2];
			buffer11[3][bdepth_i + i] = vdata[3];
			buffer11[4][bdepth_i + i] = vdata[4];
			buffer11[5][bdepth_i + i] = vdata[5];
			buffer11[6][bdepth_i + i] = vdata[6];
			buffer11[7][bdepth_i + i] = vdata[7];
			buffer11[8][bdepth_i + i] = vdata[8];
			buffer11[9][bdepth_i + i] = vdata[9];
			buffer11[10][bdepth_i + i] = vdata[10];
			buffer11[11][bdepth_i + i] = vdata[11];
			buffer11[12][bdepth_i + i] = vdata[12];
			buffer11[13][bdepth_i + i] = vdata[13];
			buffer11[14][bdepth_i + i] = vdata[14];
			buffer11[15][bdepth_i + i] = vdata[15];

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
void acts_all::MEMACCESSP1_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS + 1, NAp, NAp, NAp);
	#endif
	
	// unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	unsigned int _NUM_PARTITIONS = globalparams.NUM_REDUCEPARTITIONS; // use this instead. FIXME URGENT.
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
		vec.data[5].key = kvdram[offset_kvs + i].range(351, 320); 
		vec.data[5].value = kvdram[offset_kvs + i].range(383, 352); 
		vec.data[6].key = kvdram[offset_kvs + i].range(415, 384); 
		vec.data[6].value = kvdram[offset_kvs + i].range(447, 416); 
		vec.data[7].key = kvdram[offset_kvs + i].range(479, 448); 
		vec.data[7].value = kvdram[offset_kvs + i].range(511, 480); 
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
		vec.data[5].key = kvdram[offset_kvs + i].data[5].key; 
		vec.data[5].value = kvdram[offset_kvs + i].data[5].value; 
		vec.data[6].key = kvdram[offset_kvs + i].data[6].key; 
		vec.data[6].value = kvdram[offset_kvs + i].data[6].value; 
		vec.data[7].key = kvdram[offset_kvs + i].data[7].key; 
		vec.data[7].value = kvdram[offset_kvs + i].data[7].value; 
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

void acts_all::MEMACCESSP1_saveglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("saveglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS, offset_kvs, globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS);
	#endif
	
	// cout<<"--------------------------------------------------------------- MEMACCESSP1_saveglobalstats -------------------------------------------------------------"<<endl;
	
	// unsigned int _NUM_PARTITIONS = globalparams.ACTSPARAMS_NUM_PARTITIONS;
	unsigned int _NUM_PARTITIONS = globalparams.NUM_REDUCEPARTITIONS;
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
else if(globalparams.VARS_WORKBATCH == 5){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(351, 320) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(383, 352) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[5].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[5].value = globalstatsbuffer[i].value; 
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE // _DEBUGMODE_KERNELPRINTS_TRACE3
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSP1_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
			#endif
		}
else if(globalparams.VARS_WORKBATCH == 6){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(415, 384) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(447, 416) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[6].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[6].value = globalstatsbuffer[i].value; 
			#endif 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE // _DEBUGMODE_KERNELPRINTS_TRACE3
			if(globalstatsbuffer[i].value>0){ cout<<"MEMACCESSP1_saveglobalstats:: active partition saved @ partition "<<globalstatsbuffer[i].value<<", offset_kvs: "<<offset_kvs + i<<"."<<endl; }	
			#endif
		}
else if(globalparams.VARS_WORKBATCH == 7){
			#ifdef _WIDEWORD
			kvdram[offset_kvs + i].range(479, 448) = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].range(511, 480) = globalstatsbuffer[i].value; 
			#else 
			kvdram[offset_kvs + i].data[7].key = globalstatsbuffer[i].key; 
			kvdram[offset_kvs + i].data[7].value = globalstatsbuffer[i].value; 
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

tuple_t acts_all::MEMACCESSP1_getvptrs(uint512_dt * kvdram, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID){
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

unsigned int acts_all::MEMACCESSP1_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
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

tuple_t acts_all::MEMACCESSP1_getvptrs_opt(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID, globalparams_t globalparams){
	#pragma HLS INLINE 
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	beginoffset = beginoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO; // convert-to-appropriate-skip-format
	endoffset = endoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP1_getvptrs_opt: baseoffset_kvs: "<<baseoffset_kvs<<", beginoffset: "<<beginoffset<<", endoffset: "<<endoffset<<", VPTR_SHRINK_RATIO: "<<NAp<<endl;
	
	beginvptr = MEMACCESSP1_getvptr(kvdram, baseoffset_kvs, beginoffset);
	endvptr = MEMACCESSP1_getvptr(kvdram, baseoffset_kvs, endoffset);

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}

unsigned int acts_all::MEMACCESSP1_getdata(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int loc){
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

void acts_all::MEMACCESSP1_retreievekvstats(uint512_dt * kvdram, keyvalue_t * buffer, globalparams_t globalparams, unsigned int offset, unsigned int size){
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

void acts_all::MEMACCESSP1_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
void acts_all::MEMACCESSP1_readhelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, unsigned int actsinstance, globalparams_t globalparams){
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

void acts_all::TradGPP1(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
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
			actvvids[i] = UTILP1_ReadData(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + actvvs_currentit_basekvs, i); 
		}
		
		// read & buffer (vertex ptrs)
		TRADGP_LOOP3: for(unsigned int t=0; t<chunk_size; t++){
		// #pragma HLS PIPELINE
			unsigned int srcvid = actvvids[t];
			edge_t vptr_begin = UTILP1_ReadData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, srcvid); 
			edge_t vptr_end = UTILP1_ReadData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, srcvid + 1); 
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
				unsigned int dstvid = UTILP1_ReadData(mdram, globalparamsm.BASEOFFSETKVS_EDGESDATA, t);
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
				independent_edge[index].dstp = UTILP1_ReadData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO));
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
			value_t new_vprop = reduce_func(vmdata.vdata, NAp, NAp, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA.ALGORITHMINFO_GRAPHALGORITHMID);
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
				UTILP1_WriteData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				UTILP1_WriteData(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				UTILP1_WriteData(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				
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
void acts_all::MERGEP1_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs){
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

void acts_all::MERGEP1_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> mergeVs:: merging vertices..."<< RESET <<endl; 
	#endif
	
	globalparams_t globalparams = UTILP1_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP1_getglobalparams(vdram, 0);
	
	unsigned int slrvoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2){ slrvoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ slrvoffset_kvs2 = globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ slrvoffset_kvs2 = (globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }										
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"mergeVs:: merging vertices. [begin offset: "<<slrvoffset_kvs2<<"]"<<endl; 	
	#endif 
	unsigned int i = globalparams.ACTSPARAMS_INSTID % globalparams.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2;
	unsigned int voffset_kvs2 = i * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	MERGEP1_MERGEVSLOOP2: for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + slrvoffset_kvs2 + voffset_kvs2 + globalparams.SIZEKVS2_REDUCEPARTITION < globalparamsv.ACTSPARAMS_MAXHBMCAPACITY_KVS){
		MERGEP1_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("mergeVs: ERROR 21", slrvoffset_kvs2 + voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, slrvoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
			actsutilityobj->checkoutofbounds("mergeVs: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); 
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + slrvoffset_kvs2 + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			MERGEP1_print_active_masks(vdram, globalparamsv, slrvoffset_kvs2 + voffset_kvs2 + k);	
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

unsigned int acts_all::MERGEP1_copy(uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, unsigned int voffset_kvs, unsigned int num_compute_units, 
		globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2){
	unsigned int total_sync = 0;
	MERGEP1_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<num_compute_units; i++){
		for(unsigned int reduce_partition=0; reduce_partition<globalparamsSRC.NUM_REDUCEPARTITIONS; reduce_partition++){
			if(globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + globalparamsSRC.SIZEKVS2_REDUCEPARTITION < globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS){
			MERGEP1_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsSRC.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 21", globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 22", globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				total_sync += VECTOR2_SIZE;
				#endif
				
				vdramDST1[globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k] = vdramSRC[globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k];
				vdramDST2[globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k] = vdramSRC[globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				MERGEP1_print_active_masks(vdramSRC, globalparamsSRC, voffset_kvs);
				#endif
			}
			}
			voffset_kvs += globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
		}
	}
	return total_sync;
}

void acts_all::MERGEP1_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> exchangeVs:: exchanging vertices across different SLRs..." << RESET <<endl; 
	#endif

	unsigned int total_sync[3]; total_sync[0]=0; total_sync[1]=0; total_sync[2]=0; 
	#pragma HLS ARRAY_PARTITION variable=total_sync complete

	globalparams_t globalparamsvA = UTILP1_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP1_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP1_getglobalparams(vdramC, 0);
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR1) * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	
	total_sync[0] = MERGEP1_copy(vdramA, vdramB, vdramC, voffsetA_kvs, NUMCOMPUTEUNITS_SLR2, globalparamsvA, globalparamsvB, globalparamsvC);
	total_sync[1] = MERGEP1_copy(vdramB, vdramA, vdramC, voffsetB_kvs, NUMCOMPUTEUNITS_SLR1, globalparamsvB, globalparamsvA, globalparamsvC);
	total_sync[2] = MERGEP1_copy(vdramC, vdramA, vdramB, voffsetC_kvs, NUMCOMPUTEUNITS_SLR0, globalparamsvC, globalparamsvA, globalparamsvB);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"finished. "<<total_sync[0] + total_sync[1] + total_sync[2]<<" vertices synchronized"<<endl; 
	#endif
	return;
}

extern "C" {
void acts_all::TOPP1_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
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

keyvalue_t acts_all::process_edge(bool enx, unsigned int v, unsigned int loc, keyvalue_t edge_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VSRCDATA_SIZE], globalparams_t globalparams){				
	#pragma HLS INLINE
	
	// flag 
	bool en = true; if(edge_data.key == INVALIDDATA || edge_data.value == INVALIDDATA || enx == false){ en = false; } else { en = true; }

	// check loc 
	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){ loc = 0; }
	#ifdef _DEBUGMODE_CHECKS
	if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && edge_data.key != INVALIDDATA){ cout<<"PROCESSP1_processvector::ERROR SEEN @@ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
		
	// read vertex data 
	keyvalue_vbuffer_t bits_vector = vbuffer[loc / VDATA_SHRINK_RATIO];
	
	// read vertex mask
	vmdata_t vmdata; if(en == true){ vmdata = MEMCAP1_READFROMBUFFER_VDATAWITHVMASK(bits_vector); } else { vmdata.vmask = 0; }
	if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ vmdata.vmask = 1; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(vmdata.vmask == 1){ cout<<">>> PROCESS VECTOR:: ACTIVE VERTEX PROCESSED: SEEN: @ v: "<<v<<", loc: "<<loc<<", edge_data.key: "<<edge_data.key<<", edge_data.value(srcvid): "<<edge_data.value<<", en: "<<en<<", vid: "<<UTILP1_GETREALVID(edge_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
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
	if(update_data.key == UTILP1_GETK(INVALIDDATA) || update_data.value == UTILP1_GETV(INVALIDDATA) || update_data.key == MAXVDATA || update_data.value == MAXVDATA){ en = false; }
	
	// check loc 
	if(loc >= (globalparams.SIZEKVS2_REDUCEPARTITION / 2) && en == true){ loc = 0; } // loc = loc / 2; // urams pack in 2s
	#ifdef ENABLE_PERFECTACCURACY
	if(true){ cout<<"REDUCEP1reduce_update::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). update_data.key: "<<update_data.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
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
		if(en == true){ tmpvdata = MEMCAP1_READFROMBUFFER_VDATAWITHVMASK2(loc, bits_vector); }
			#else 
				if(en == true){ tmpvdata = MEMCAP1_READFROMBUFFER_VDATAWITHVMASK(bits_vector); }
					#endif
	
	// reduce 
	value_t new_vprop = reduce_func(tmpvdata.vdata, tmpvdata.vdata, update_data.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	
	if(en == true && new_vprop != tmpvdata.vdata){ tmpvdata.vmask = 1; } else { tmpvdata.vmask = 0; }
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(en == true && new_vprop != tmpvdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE MASK SEEN AT: loc: "<<loc<<", tmpvdata.vdata: "<<tmpvdata.vdata<<", tmpvdata.vmask: "<<tmpvdata.vmask<<endl; }
	if(en == true && new_vprop != tmpvdata.vdata){ cout<<">>> REDUCE VECTOR:: ACTIVE REDUCE SEEN @ instance "<<globalparams.ACTSPARAMS_INSTID<<", col: "<<col<<", loc: "<<loc<<", vid: "<<UTILP1_GETREALVID(update_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
	#endif
	
	// write-back vertx data
	#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS)
		if(en == true && which == 0){ MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK2(loc, vbufferA, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, tmpvdata.vmask); }
		if(en == true && which == 1){ MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK2(loc, vbufferB, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, tmpvdata.vmask); }
			#else 
				if(en == true && which == 0){ MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK(loc, vbufferA, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, tmpvdata.vmask); }
				if(en == true && which == 1){ MEMCAP1_WRITETOBUFFER_VDATAWITHVMASK(loc, vbufferB, globalparams.BUFFERBASEOFFSETKVS_VDATA1, bits_vector, new_vprop, tmpvdata.vmask); }
					#endif
	return;
}	

#ifdef FPGA_IMPL
void load(uint512_dt *in, hls::stream<uint512_evec_dt >& out, batch_type sourcebaseaddr_kvs, unsigned int offset_kvs, int size){
	Loop_Ld: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		uint512_evec_dt data = UTILP1_ReadEdges(in, sourcebaseaddr_kvs + offset_kvs + i);
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
		for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL
			res.data[v] = process_edge(enx, v, data.data[v].value - upperlimit, data.data[v], vbuffer_source[v], globalparamsK);
		}
		
		// store
		UTILP1_WriteDataset(out, destbaseaddr_kvs + offset_kvs + i, res);
	}
}
#endif 

void acts_all::ACTSP1_read_process_partition_and_write(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
			batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, int size, unsigned int upperlimit, 
				collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	uint512_uvec_dt res; // uint512_uvec_dt

	for(int i = 0; i < size; i++){
		// load 
		uint512_evec_dt data = UTILP1_ReadEdges(in, sourcebaseaddr_kvs + srcoffset_kvs + i);
	
		// set flag
		bool enx = true;
		collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
		collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE; // *** used in PR, CF implementations ONLY (not BFS) ***
		
		// process
		for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL
			res.data[v] = process_edge(enx, v, data.data[v].value - upperlimit, data.data[v], vbuffer_source[v], globalparamsK);
		}
		
		// store
		UTILP1_WriteDataset(out, destbaseaddr_kvs + destoffset_kvs + i, res);
		
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(enx == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE); }
		#endif 
	}
}

void acts_all::ACTSP1_read_process_partition_and_write_base(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],	
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
	ACTSP1_read_process_partition_and_write(kvdram, kvdram, vbuffer_source,
		sourcebaseaddr_kvs, workload_kvs.offset_begin, destbaseaddr_kvs, destoffset_kvs, workload_kvs.size, upperlimit, 
					collections, globalparamsK, globalposition);
	#endif
}

void acts_all::ACTSP1_read_and_reduce(unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	
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
		UTILP1_ReadKeyvalues(kvdram, sourcebaseaddr_kvs + offset_kvs, datas);
		
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

void acts_all::ACTSP1_read_process_and_reduce(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
				collection_t collections[COLLECTIONS_BUFFERSZ], sweepparams_t sweepparams, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition){
	
	keyvalue_t res[EDGEDATA_PACKINGSIZE]; // uint512_uvec_dt
	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory 
	keyvalue_t datas[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	
	keyvaluemask_t buffer[EDGEDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	unsigned int buffer_index[EDGEDATA_PACKINGSIZE]; for(int i = 0; i < EDGEDATA_PACKINGSIZE; i++){ buffer_index[i] = 0; }
	unsigned int actvvs[BLOCKRAM_SIZE];
	
	unsigned int num_actvvs = globalposition.num_active_vertices;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> actit3:: [iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", v_partition: "<<sweepparams.source_partition<<", num_actvvs: "<<num_actvvs<<"] "<<endl;
	#endif 
	
	// read and process 
	for(int n=0; n<num_actvvs; n++){
		workload_t workload_kvs;
		unsigned int loc = n;//n + 5; // REMOVEME. // actvvs[n]
		workload_kvs.offset_begin = MEMACCESSP1_getdata(in, globalparamsE.BASEOFFSETKVS_ACTIVEEDGESMAP, (sweepparams.source_partition * MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION) + 2*loc);
		workload_kvs.size = MEMACCESSP1_getdata(in, globalparamsE.BASEOFFSETKVS_ACTIVEEDGESMAP, (sweepparams.source_partition * MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION) + 2*loc + 1);
		workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actit3:: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<", workload_kvs.offset_end: "<<workload_kvs.offset_end<<", workload_kvs.size: "<<workload_kvs.size<<", loc: "<<loc<<"] "<<endl;
		#endif
		
		for(int i = 0; i < workload_kvs.size; i++){
		#pragma HLS PIPELINE II=1
			// get dataset
			UTILP1_ReadKeyvalues(in, globalparamsE.BASEOFFSETKVS_EDGESDATA + workload_kvs.offset_begin + i, datas);
			
			// set flag
			bool enx = true;
			if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){  // used in BFS implementations ONLY (not PR or CF)
				collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
				collections[PROCESSEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
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
}

void acts_all::ACTSP1_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){

	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory // complete
	workload_t workloads_kvs[BLOCKRAM_SIZE];
	
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
	
	bool enx = true;
	unsigned int _EN_PROCESS_PARTITION_SAVE = OFF; 
	unsigned int _EN_PROCESS_PARTITION_REDUCE = OFF;
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ _EN_PROCESS_PARTITION_SAVE = OFF; _EN_PROCESS_PARTITION_REDUCE = ON; }
	else { if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){ _EN_PROCESS_PARTITION_SAVE = ON; } else if(mode == ACTSREDUCEMODE){ _EN_PROCESS_PARTITION_REDUCE = ON; }}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actit3:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", num_vPs: "<<num_vPs<<"] "<<endl;
	cout<<"actit3:: globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_vertices: "<<globalposition.num_active_vertices<<", globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION: "<<globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION<<""<<endl; 
	#endif 
	
	if(globalparamsK.EVALUATION_ACTS_HYBRIDLOGIC == ON && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){
		#ifdef CONFIG_ACTS_HYBRIDLOGIC
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actit3:: TOO FEW VERTICES. using TRADGP instead: [source_partition: "<<globalposition.source_partition<<", num_active_vertices: "<<globalposition.num_active_vertices<<"]"<<endl; 
		#endif 
		ACTSP1_read_process_and_reduce(kvdram, kvdram, vbuffer_source, collections, sweepparams, globalparamsE, globalparamsK, globalposition);
		#endif 
	} else {	
		ACTIT3_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
			// retreived pre-processed llp_set information first
			workload_t workload_kvs;
			workload_kvs.offset_begin = MEMACCESSP1_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, (sweepparams.source_partition * num_LLPset) + llp_set) / UPDATEDATA_PACKINGSIZE;
			workload_kvs.offset_end = MEMACCESSP1_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, (sweepparams.source_partition * num_LLPset) + llp_set + 1) / UPDATEDATA_PACKINGSIZE;
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
				batch_type destoffset_kvs = (globalcapsule[llp_set].key + globalcapsule[llp_set].value) / UPDATEDATA_PACKINGSIZE;
				
				#ifdef _DEBUGMODE_KERNELPRINTS
				unsigned int num_chunks = workload_kvs.size / BLOCKRAM_SIZE;
				cout<<"actit3(process): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][size_kvs: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<num_chunks<<"] ... "<<endl;					
				#endif
				
				ACTSP1_read_process_partition_and_write_base(kvdram, vbuffer_source,	
					upperlimit, sourcebaseaddr_kvs, workload_kvs, destbaseaddr_kvs, destoffset_kvs, 
						collections_tmp, globalparamsK, globalposition);
				
				globalcapsule[llp_set].value += workload_kvs.size * UPDATEDATA_PACKINGSIZE; 
			}
		}
		
		if(_EN_PROCESS_PARTITION_REDUCE == ON && globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON){  
			// retreived pre-processed llp_set information first
			workload_t workload_kvs;
			if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
				workload_kvs.offset_begin = MEMACCESSP1_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, sweepparams.source_partition) / UPDATEDATA_PACKINGSIZE;
				workload_kvs.offset_end = MEMACCESSP1_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, sweepparams.source_partition + 1) / UPDATEDATA_PACKINGSIZE;
			} else { workload_kvs.offset_begin = ptravstate.begin_kvs; workload_kvs.offset_end = ptravstate.end_kvs; }
			workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
			#ifdef _DEBUGMODE_CHECKS3
			if(workload_kvs.offset_end < workload_kvs.offset_begin){ cout<<"actit3(reduce): ERROR 23: workload_kvs.offset_end("<<workload_kvs.offset_end<<") < workload_kvs.offset_begin("<<workload_kvs.offset_begin<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif
		
			ACTSP1_read_and_reduce(mode, kvdram, vbuffer_dest,	
				sweepparams, sourcebaseaddr_kvs, workload_kvs,
					collections_tmp, globalparamsK, globalposition);
		}
		
		collections[TRAVERSEDEDGES_COLLECTIONID] = collections_tmp[TRAVERSEDEDGES_COLLECTIONID];
		collections[PROCESSEDGES_COLLECTIONID] = collections_tmp[PROCESSEDGES_COLLECTIONID];
		collections[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID];
		collections[REDUCEUPDATES_COLLECTIONID] = collections_tmp[REDUCEUPDATES_COLLECTIONID];
	}
}

void acts_all::ACTSP1_actit_base(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	ACTSP1_actit(enable, mode, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sourcebaseaddr_kvs, destbaseaddr_kvs, resetenv, flush, edgebankID, collections);
}	
#endif 			
void acts_all::TOPP1_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
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
	if(globalparamsK.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ globalposition.deststatsmarker = UTILP1_getstatsoffset(globalparamsK.ACTSPARAMS_TREEDEPTH-1, globalparamsK); }
	
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP1_resetkeysandvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0); } 
	sweepparams = UTILP1_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	
	if(globalposition.source_partition == globalposition.first_source_partition){ MEMACCESSP1_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL NEWCHANGE.
	if(globalposition.source_partition == globalposition.first_source_partition){ UTILP1_resetvalues(globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0); } // CRITICAL NEWCHANGE.
	
	// actsutilityobj->printkeyvalues("$$$$$$$$$$$$$$$$$$$$ processit. 37", (keyvalue_t *)globalstatsbuffer, globalparamsK.NUM_REDUCEPARTITIONS); 
	// exit(EXIT_SUCCESS); 
				
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); //. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	batch_type voffset_kvs = globalposition.source_partition * UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print7("### TOPP1_processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, NAp, sweepparams.currentLOP); 							
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
		keyy_t numedges = localendvptr - localbeginvptr + 1*EDGEDATA_PACKINGSIZE; // CRITICAL NEWCHANGE.
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / EDGEDATA_PACKINGSIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / EDGEDATA_PACKINGSIZE; // NB: this is correct.
		// collections[TRAVERSEDEDGES_COLLECTIONID].data1 += numedges;
		
		#ifdef _DEBUGMODE_CHECKS3
		if(localendvptr < localbeginvptr){ cout<<"TOPP1_processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<", voffset_kvs: "<<voffset_kvs<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("TOPP1_processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
		actsutilityobj->checkoutofbounds("processit. ERROR 30.", localbeginvptr, (1 << 28) / 4 / 2, beginvptr, endvptr, numedges); ///////////
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
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"[index: "<<globalposition.source_partition<<"][etravstate.begin_kvs: "<<etravstate.begin_kvs<<", etravstate.size_kvs: "<<etravstate.size_kvs<<"]"<<endl;
		#endif
		// exit(EXIT_SUCCESS);
		
		if(voffset_kvs == avtravstate.begin_kvs || globalposition.source_partition == globalposition.first_source_partition){ resetenv = ON; } else { resetenv = OFF; }
		if((voffset_kvs + UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
		resetenv = ON; flush = ON; 
		
		ACTSP1_actit_base(
			ON, ACTSPROCESSMODE,
			kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer,
			globalparamsE, globalparamsK, globalposition, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAM,
			resetenv, flush, edgebankID, collections);
	}
	
	bool endofrun = false;
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ endofrun = true; }
	
	if(endofrun == true){ MEMACCESSP1_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL OPTIMIZEME. should be called only once
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPP1_processit", globalstatsbuffer, globalparamsK.ACTSPARAMS_NUM_PARTITIONS); }
	#endif	
	
	if(endofrun == true){ UTILP1_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[TRAVERSEDEDGES_COLLECTIONID].data1); }
	if(endofrun == true){ UTILP1_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[PROCESSEDGES_COLLECTIONID].data1); }
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && endofrun == true){ UTILP1_SetThirdData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[REDUCEUPDATES_COLLECTIONID].data1); }
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && endofrun == true){ MEMACCESSP1_saveV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); }

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

void acts_all::TOPP1_partitionit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
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

void acts_all::TOPP1_reduceit( uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){	
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

void acts_all::TOPP1_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
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

void acts_all::TOPP1_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	// unsigned int sourcestatsmarker = 0;
	// for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); }
	
	unsigned int sourcestatsmarker = 1;
	// for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * k)); }
	
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
		// collections[REDUCEUPDATES_COLLECTIONID].data1 += rtravstate.size_kvs * VECTOR_SIZE;
		
		// read vertices 
		MEMACCESSP1_readV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		
		// reduce
		TOPP1_dispatch(OFF, OFF, enablereduce, kvdram, vbuffer_source, vbuffer_dest, globalstatsbuffer, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);
		
		// writeback vertices 
		MEMACCESSP1_saveV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		vmaskp_offset_kvs += NUM_PEs;
	}
	
	UTILP1_SetThirdData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[REDUCEUPDATES_COLLECTIONID].data1);
	// exit(EXIT_SUCCESS);
	return;
} 

void acts_all::TOPP1_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[COLLECTIONS_BUFFERSZ]){									

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	unsigned int printheader1=OFF; // ON, OFF*
	unsigned int printheader2=OFF; 
	if(true && globalposition.source_partition==globalposition.first_source_partition){ printheader1 = ON; } else { printheader1 = OFF; }
	if(false && globalposition.source_partition==globalposition.last_source_partition){ printheader2 = ON; } else { printheader2 = OFF; }
	// printheader1=ON;
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== ACTS Launched... size: "<<UTILP1_GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<" ====================== "<<endl;
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	unsigned int PARTITIONP1_CHKPT[EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
	#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
	keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE];
	#pragma HLS array_partition variable = vbuffer_dest	
	#ifdef CONFIG_USEURAM_FOR_DESTVBUFFER
		#pragma HLS resource variable=vbuffer_dest core=XPM_MEMORY uram
		// #pragma HLS bind_storage variable=vbuffer_dest type=RAM_T2P impl=uram 
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
	/* #ifdef CONFIG_ENABLEPARTITIONMODULE  // CRITICAL REMOVEME URGENT.
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
	#endif */
	
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
	if(endofrun == true){
		unsigned int _offset = UTILP1_getstatsoffset(globalparamsK.ACTSPARAMS_TREEDEPTH-1, globalparamsK);
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
void acts_all::TOPP1_topkernelP1(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL1_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks1(ON, vdram, vbuffer_source0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP1"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP2(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL2_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks2(ON, vdram, vbuffer_source0,vbuffer_source1, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP2"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP3(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL3_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks3(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP3"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP4(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL4_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks4(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
		globalparams_t globalparamsK0;
		MERGEP1_mergeVs(kvdram0, vdram);
	
		globalparams_t globalparamsK1;
		MERGEP1_mergeVs(kvdram1, vdram);
	
		globalparams_t globalparamsK2;
		MERGEP1_mergeVs(kvdram2, vdram);
	
		globalparams_t globalparamsK3;
		MERGEP1_mergeVs(kvdram3, vdram);
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP4"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP5(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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
	collection_t collections4[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL5_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks5(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
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
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP5"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP6(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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
	collection_t collections4[COLLECTIONS_BUFFERSZ];
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
	collection_t collections5[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL6_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks6(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
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
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP6"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP7(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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
	collection_t collections4[COLLECTIONS_BUFFERSZ];
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
	collection_t collections5[COLLECTIONS_BUFFERSZ];
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
	collection_t collections6[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL7_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks7(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
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
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP7"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP8(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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
	collection_t collections4[COLLECTIONS_BUFFERSZ];
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
	collection_t collections5[COLLECTIONS_BUFFERSZ];
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
	collection_t collections6[COLLECTIONS_BUFFERSZ];
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
	collection_t collections7[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL8_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks8(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
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
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP8"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP9(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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
	collection_t collections4[COLLECTIONS_BUFFERSZ];
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
	collection_t collections5[COLLECTIONS_BUFFERSZ];
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
	collection_t collections6[COLLECTIONS_BUFFERSZ];
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
	collection_t collections7[COLLECTIONS_BUFFERSZ];
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
	collection_t collections8[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL9_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks9(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
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
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP9"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP10(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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
	collection_t collections4[COLLECTIONS_BUFFERSZ];
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
	collection_t collections5[COLLECTIONS_BUFFERSZ];
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
	collection_t collections6[COLLECTIONS_BUFFERSZ];
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
	collection_t collections7[COLLECTIONS_BUFFERSZ];
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
	collection_t collections8[COLLECTIONS_BUFFERSZ];
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
	collection_t collections9[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL10_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks10(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
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
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP10"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP11(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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
	collection_t collections4[COLLECTIONS_BUFFERSZ];
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
	collection_t collections5[COLLECTIONS_BUFFERSZ];
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
	collection_t collections6[COLLECTIONS_BUFFERSZ];
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
	collection_t collections7[COLLECTIONS_BUFFERSZ];
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
	collection_t collections8[COLLECTIONS_BUFFERSZ];
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
	collection_t collections9[COLLECTIONS_BUFFERSZ];
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
	collection_t collections10[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  collections10[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL11_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks11(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
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
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP11"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
extern "C" {
void acts_all::TOPP1_topkernelP12(
	
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
	collection_t collections0[COLLECTIONS_BUFFERSZ];
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
	collection_t collections1[COLLECTIONS_BUFFERSZ];
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
	collection_t collections2[COLLECTIONS_BUFFERSZ];
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
	collection_t collections3[COLLECTIONS_BUFFERSZ];
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
	collection_t collections4[COLLECTIONS_BUFFERSZ];
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
	collection_t collections5[COLLECTIONS_BUFFERSZ];
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
	collection_t collections6[COLLECTIONS_BUFFERSZ];
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
	collection_t collections7[COLLECTIONS_BUFFERSZ];
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
	collection_t collections8[COLLECTIONS_BUFFERSZ];
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
	collection_t collections9[COLLECTIONS_BUFFERSZ];
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
	collection_t collections10[COLLECTIONS_BUFFERSZ];
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
	collection_t collections11[COLLECTIONS_BUFFERSZ];
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

	// hybrid mode functionality
	#ifdef NOTUSED_____________________________
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		unsigned int total_num_active_vertices_in_all_vpartitions = 0, total_num_active_edges_in_all_vpartitions = 0;
		MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + 0, BLOCKRAM_CURRPMASK_SIZE, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_vertices_in_all_vpartitions += pmask_curr[t]; }
		for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){ total_num_active_edges_in_all_vpartitions += emask_curr[t]; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> topkernelP:: iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<": total_num_active_vertices_in_all_vpartitions: "<<total_num_active_vertices_in_all_vpartitions<<", total_num_active_edges_in_all_vpartitions: "<<total_num_active_edges_in_all_vpartitions<<endl;
		#endif 
		// UTILP1_increment_graphiteration(vdram, globalparamsV); 
		// return; // REMOVEME.	
		if(total_num_active_vertices_in_all_vpartitions < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ 
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"topkernelP:: Switch: Using trad GP for iteration "<<globalparamsV.ALGORITHMINFO_GRAPHITERATIONID<<" ..."<<endl;
			#endif 
			UTILP1_increment_graphiteration(vdram, globalparamsV); return; }
	}
	#endif 
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  collections10[t].data1 = 0;  collections11[t].data1 = 0;  }
	
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
		UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
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
						if(stage == 0 && source_partition == 0){ UTILP1_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						unsigned int true_source_partition = FIRSTSOURCEPARTITION + source_partition;
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS 
						// if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
						if(stage==0 && true_source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){ // FIXME???????????????????? make others like this
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL12_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP1_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
						}
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = num_stages - 1; 
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
						unsigned int num_active_vertices_in_partition = pmask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[true_source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS3
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							cout<<">>> --------------------------------- topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<""<<endl; }
						#endif 
						#ifdef NOTUSED_____________________________
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ globalposition.num_active_vertices = 0; enable_readandreplicatevdata = OFF; }// force a no-activity // globalposition.num_active_edges_in_channel = 0; 
							#ifdef _DEBUGMODE_KERNELPRINTS
							if(globalposition.num_active_vertices < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION || total_num_active_vertices_in_vchunk < globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD){ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED OFF (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; } 
							else{ cout<<"topkernelP:: iteration: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<source_partition<<":: is SWITCHED ON (num_active_vertices_in_partition: "<<num_active_vertices_in_partition<<")"<<endl; }
							#endif 
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP1_readANDRVchunks12(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10,vbuffer_source11, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON && (GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || total_num_active_vertices_in_iteration > globalparamsV.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD)){	
	
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
	}
	#endif
	
	#if defined(_DEBUGMODE_KERNELPRINTS) && defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<"PRINTGLOBALVARS @ topkernelP12"<<endl;
	actsutilityobj->printglobalvars();
	#endif	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}
	

	

