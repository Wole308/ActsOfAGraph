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
unsigned int acts_all::UTILP2_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int acts_all::UTILP2_amax(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int acts_all::UTILP2_aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}
uint32_type acts_all::UTILP2_amin2(uint32_type val1, uint32_type val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

// allignment
batch_type acts_all::UTILP2_allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type acts_all::UTILP2_allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type acts_all::UTILP2_allignlower_KV2(batch_type val){
	batch_type fac = val / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type acts_all::UTILP2_allignhigher_KV2(batch_type val){
	batch_type fac = (val + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type acts_all::UTILP2_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR){
	#pragma HLS INLINE
	batch_type fac = (val + (_FACTOR - 1)) / _FACTOR;
	return (fac * _FACTOR);
}
batch_type acts_all::UTILP2_allignlower_FACTOR(batch_type val, unsigned int _FACTOR){
	batch_type fac = val / _FACTOR;
	return (fac * _FACTOR);
}

unsigned int acts_all::UTILP2_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(unsigned int tree_depth){
	return (1 << (OPT_NUM_PARTITIONS_POW * (tree_depth-1)));
}

// bit manipulation
unsigned int acts_all::UTILP2_GETMASK_UINT(uint32_type index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int acts_all::UTILP2_READFROM_UINT(uint32_type data, unsigned int index, unsigned int size){ 
	unsigned int res = 0;
	res = (((data) & UTILP2_GETMASK_UINT((index), (size))) >> (index)); 
	return res;
}
void acts_all::UTILP2_WRITETO_UINT(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~UTILP2_GETMASK_UINT((index), (size)));
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
unsigned int acts_all::UTILP2_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	#ifdef _WIDEWORD
	res = data.range(index + size - 1, index); 
	#else
	res = UTILP2_READFROM_UINT(data, index, size);
	#endif
	return res;
}
void acts_all::UTILP2_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	data->range(index + size - 1, index) = value;
	#else
	UTILP2_WRITETO_UINT(data, index, size, value);
	#endif
	return; 
}
unsigned int acts_all::UTILP2_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	res = UTILP2_READFROM_UINT(data, index, size);
	return res;
}
void acts_all::UTILP2_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	UTILP2_WRITETO_UINT(data, index, size, value);
	return; 
}

// converters
keyvalue_t acts_all::UTILP2_GETKV(keyvalue_buffer_t data){
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
keyvalue_buffer_t acts_all::UTILP2_GETKV(keyvalue_t data){
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
keyy_t acts_all::UTILP2_GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_KEY - 1, 0);
	#else
	return data;
	#endif
}
value_t acts_all::UTILP2_GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_VALUE - 1, 0);
	#else
	return data;
	#endif
}
keyy_t acts_all::UTILP2_GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[v].key;	
	#endif
}		
unsigned int acts_all::UTILP2_GETLOCALVID(unsigned int vid, unsigned int instid){ 
	#pragma HLS INLINE
	return (vid - instid) / NUM_PEs;
}
unsigned int acts_all::UTILP2_GETREALVID(unsigned int lvid, unsigned int instid){ 
	#pragma HLS INLINE
	
	#ifdef CONFIG_EDGEHASHSCHEME_SETVIDS
	return ((lvid / EDGEDATA_PACKINGSIZE) * (EDGEDATA_PACKINGSIZE * NUM_PEs)) + (instid * EDGEDATA_PACKINGSIZE) + lvid;
	#endif 
	
	#ifdef CONFIG_EDGEHASHSCHEME_SINGLEVID
	return (lvid * NUM_PEs) + instid;
	#endif 
}
unsigned int acts_all::UTILP2_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams){ 
	#pragma HLS INLINE
	return globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	// return PROCESSPARTITIONSZ_KVS2;
}

unsigned int acts_all::UTILP2_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(31, 0);
	#else 
	return kvdram[offset_kvs].data[0].key;
	#endif 
}
void acts_all::UTILP2_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(31, 0) = data;
	#else 
	kvdram[offset_kvs].data[0].key = data;
	#endif 
	return;
}
void acts_all::UTILP2_SetFirstDatas(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int * data, unsigned int size){
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
unsigned int acts_all::UTILP2_GetSecondData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(63, 32);
	#else 
	return kvdram[offset_kvs].data[0].value;
	#endif 
}
void acts_all::UTILP2_SetSecondData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(63, 32) = data;
	#else 
	kvdram[offset_kvs].data[0].value = data;
	#endif 
	return;
}
unsigned int acts_all::UTILP2_GetThirdData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(95, 64);
	#else 
	return kvdram[offset_kvs].data[1].key;
	#endif 
}
void acts_all::UTILP2_SetThirdData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(95, 64) = data;
	#else 
	kvdram[offset_kvs].data[1].key = data;
	#endif 
	return;
}
	
void acts_all::UTILP2_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[EDGEDATA_PACKINGSIZE]){  
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
void acts_all::UTILP2_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){  
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
uint512_evec_dt acts_all::UTILP2_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs){  
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

void acts_all::UTILP2_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
void acts_all::UTILP2_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
void acts_all::UTILP2_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
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
void acts_all::UTILP2_WriteDataset(uint512_dt * kvdram, unsigned int offset_kvs, uint512_uvec_dt dataset){ // *** FIXME: adjust for EDGEDATA_PACKINGSIZE & UPDATEDATA_PACKINGSIZE
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
unsigned int acts_all::UTILP2_ReadData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index){
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
void acts_all::UTILP2_WriteData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data){
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

// utilities
batch_type acts_all::UTILP2_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
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
batch_type acts_all::UTILP2_getrangeforeachllop(globalparams_t globalparams){
	unsigned int range = globalparams.SIZE_BATCHRANGE;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
		range = range / OPT_NUM_PARTITIONS;
	}
	return range;
}
buffer_type acts_all::UTILP2_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
buffer_type acts_all::UTILP2_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	buffer_type size_kvs = 0;
	size_kvs = realsize_kvs;
	return size_kvs;
}
void acts_all::UTILP2_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size){
	unsigned int analysis_size = OPT_NUM_PARTITIONS;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = UTILP2_allignhigher_KV(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}
void acts_all::UTILP2_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
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
batch_type acts_all::UTILP2_get_num_source_partitions(step_type currentLOP){
	if(currentLOP == 0){ currentLOP = 1; }
	batch_type pow = 1;
	for(step_type i=0; i<(currentLOP-1); i++){
		pow = pow * OPT_NUM_PARTITIONS;
	}
	return pow;
}
globalparams_t acts_all::UTILP2_getglobalparams(uint512_dt * kvdram, unsigned int banksection){
	globalparams_t globalparams;
	
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("UTILP2_getglobalparams 23", 0 + MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS, (((1 << 28) / 4) / 16), globalparams.DRAM_BASE_KVS, MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp);
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
		cout<<"UTILP2_getglobalparams: NOT IMPLEMENTED. EXITING..."<<endl;
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
sweepparams_t acts_all::UTILP2_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
	sweepparams_t sweepparams;
	batch_type sourceskipsize = UTILP2_getskipsize(currentLOP, SOURCE, globalparams);
	
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
travstate_t acts_all::UTILP2_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
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
			cout<<"UTILP2_gettravstate: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	
	if(currentLOP == 0){ nextkeyvalue.key = globalparams.SIZE_RUN; }
	else if(currentLOP == 1){ nextkeyvalue.key = globalparams.SIZE_RUN; }
	else { nextkeyvalue.key = keyvalue.key + keyvalue.value; }
		
	travstate.begin_kvs = keyvalue.key / UPDATEDATA_PACKINGSIZE; 
	travstate.end_kvs = (nextkeyvalue.key + (UPDATEDATA_PACKINGSIZE - 1)) / UPDATEDATA_PACKINGSIZE; // UPDATEDATA_PACKINGSIZE, VECTOR_SIZE
	
	travstate.size_kvs = travstate.end_kvs - travstate.begin_kvs;
	travstate.skip_kvs = MAX_SRCBUFFER_SIZE;
	travstate.i_kvs = travstate.begin_kvs; 
	return travstate;	
}
void acts_all::UTILP2_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value){			
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

partition_type acts_all::UTILP2_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, unsigned int tree_depth){
	partition_type partition;
	keyvalue_t thiskeyvalue = UTILP2_GETKV(keyvalue);

	if(thiskeyvalue.value == UTILP2_GETV(INVALIDDATA)){ partition = 0; } 
	else {
		keyy_t lkey = thiskeyvalue.key - upperlimit;
		if(mode == ACTSREDUCEMODE || tree_depth == 1){ partition = (lkey % OPT_NUM_PARTITIONS); } else { partition = (lkey >> (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP))); }
	}
	
	if(partition >= OPT_NUM_PARTITIONS){ 
		#ifdef ENABLE_PERFECTACCURACY
			#ifdef _DEBUGMODE_CHECKS3
			cout<<"acts_util::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", thiskeyvalue.key: "<<thiskeyvalue.key<<", thiskeyvalue.value: "<<thiskeyvalue.value<<", OPT_NUM_PARTITIONS: "<<OPT_NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP)))<<", INVALIDDATA: "<<INVALIDDATA<<", UTILP2_GETV(INVALIDDATA): "<<UTILP2_GETV(INVALIDDATA)<<endl;			 
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
partition_type acts_all::UTILP2_getpartition2(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, globalparams_t globalparams){
	partition_type partition;
	keyvalue_t thiskeyvalue = UTILP2_GETKV(keyvalue);
	
	if(thiskeyvalue.value == UTILP2_GETV(INVALIDDATA)){ partition = 0; } 
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
			cout<<"acts_util::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", thiskeyvalue.key: "<<thiskeyvalue.key<<", thiskeyvalue.value: "<<thiskeyvalue.value<<", OPT_NUM_PARTITIONS: "<<OPT_NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<", div factor: "<<(1 << (batch_range_pow - (OPT_NUM_PARTITIONS_POW * currentLOP)))<<", INVALIDDATA: "<<INVALIDDATA<<", UTILP2_GETV(INVALIDDATA): "<<UTILP2_GETV(INVALIDDATA)<<endl;			 
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
unsigned int acts_all::UTILP2_getstatsoffset(unsigned int LOP, globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ for(unsigned int k=0; k<LOP; k++){ _offset += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }} else { _offset = 1; }
	return _offset;
}

// resets 
void acts_all::UTILP2_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void acts_all::UTILP2_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void acts_all::UTILP2_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i] = resetval; 
	}
}
void acts_all::UTILP2_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; 
		buffer[i].value = resetval; 
	}
	return;
}
void acts_all::UTILP2_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"..................... UTILP2_resetkvstatvalues: resetting global stats..."<<endl;
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
void acts_all::UTILP2_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams){
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
void acts_all::UTILP2_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
			cout<<"UTILP2_accumkvstats: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<") EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}
void acts_all::UTILP2_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _WIDEWORD
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#else
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}
void acts_all::UTILP2_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
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
void acts_all::UTILP2_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
	for(partition_type p=0; p<OPT_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}

// checks 
void acts_all::UTILP2_check_capsule(keyvalue_t capsule[MAX_NUM_PARTITIONS], unsigned int num_partitions, unsigned int max_value){
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
// https://iss.oden.utexas.edu/?p=projects/galois/analytics/triangle_counting 

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

value_t process_func_sssp(value_t udata, value_t edgew){
	#pragma HLS INLINE 
	value_t res = udata + edgew;
	return res;
}
value_t reduce_func_sssp(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter){
	#pragma HLS INLINE 
	if(res < vtemp){ return res; } else { return vtemp; }
}

value_t process_func(value_t udata, value_t edgew, unsigned int GraphAlgo){
	#pragma HLS INLINE 
	if(GraphAlgo == PAGERANK){
		return process_func_pr(udata, edgew);
	} else if(GraphAlgo == CF){
		return process_func_cf(udata, edgew);
	} else if(GraphAlgo == HITS){
		return process_func_hits(udata, edgew);
	} else if(GraphAlgo == SSSP){
		return process_func_sssp(udata, edgew);
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
	} else if(GraphAlgo == SSSP){
		return reduce_func_sssp(vtemp, vdata, res, GraphIter);
	} else if(GraphAlgo == BFS){
		return reduce_func_bfs(vtemp, vdata, res, GraphIter);
	} else {
		return reduce_func_pr(vtemp, vdata, res, GraphIter);
	}
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_MEM_ACCESS
void acts_all::MEMACCESSP2_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){
	if(enable == OFF){ return; }

	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		UTILP2_ReadDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
		
		#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
		#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSRRRP2_readV 21", bufferoffset_kvs + i, MAX_BLOCKRAM_VDESTDATA_SIZE, size_kvs, bufferoffset_kvs, i);
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
	}
	return;
}

void acts_all::MEMACCESSP2_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
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
		
		#ifdef BIT_TRAVERSAL_ALGORITHM
  datas[0] = vdata[0].data[0];  datas[0] = vdata[0].data[1];  datas[0] = vdata[0].data[2];  datas[0] = vdata[0].data[3];  datas[0] = vdata[0].data[4];  datas[0] = vdata[0].data[5];  datas[0] = vdata[0].data[6];  datas[0] = vdata[0].data[7];  datas[0] = vdata[0].data[8];  datas[0] = vdata[0].data[9];  datas[0] = vdata[0].data[10];  datas[0] = vdata[0].data[11];  datas[0] = vdata[0].data[12];  datas[0] = vdata[0].data[13];  datas[0] = vdata[0].data[14];  datas[0] = vdata[0].data[15];    datas[1] = vdata[1].data[0];  datas[1] = vdata[1].data[1];  datas[1] = vdata[1].data[2];  datas[1] = vdata[1].data[3];  datas[1] = vdata[1].data[4];  datas[1] = vdata[1].data[5];  datas[1] = vdata[1].data[6];  datas[1] = vdata[1].data[7];  datas[1] = vdata[1].data[8];  datas[1] = vdata[1].data[9];  datas[1] = vdata[1].data[10];  datas[1] = vdata[1].data[11];  datas[1] = vdata[1].data[12];  datas[1] = vdata[1].data[13];  datas[1] = vdata[1].data[14];  datas[1] = vdata[1].data[15];    datas[2] = vdata[2].data[0];  datas[2] = vdata[2].data[1];  datas[2] = vdata[2].data[2];  datas[2] = vdata[2].data[3];  datas[2] = vdata[2].data[4];  datas[2] = vdata[2].data[5];  datas[2] = vdata[2].data[6];  datas[2] = vdata[2].data[7];  datas[2] = vdata[2].data[8];  datas[2] = vdata[2].data[9];  datas[2] = vdata[2].data[10];  datas[2] = vdata[2].data[11];  datas[2] = vdata[2].data[12];  datas[2] = vdata[2].data[13];  datas[2] = vdata[2].data[14];  datas[2] = vdata[2].data[15];    datas[3] = vdata[3].data[0];  datas[3] = vdata[3].data[1];  datas[3] = vdata[3].data[2];  datas[3] = vdata[3].data[3];  datas[3] = vdata[3].data[4];  datas[3] = vdata[3].data[5];  datas[3] = vdata[3].data[6];  datas[3] = vdata[3].data[7];  datas[3] = vdata[3].data[8];  datas[3] = vdata[3].data[9];  datas[3] = vdata[3].data[10];  datas[3] = vdata[3].data[11];  datas[3] = vdata[3].data[12];  datas[3] = vdata[3].data[13];  datas[3] = vdata[3].data[14];  datas[3] = vdata[3].data[15];    datas[4] = vdata[4].data[0];  datas[4] = vdata[4].data[1];  datas[4] = vdata[4].data[2];  datas[4] = vdata[4].data[3];  datas[4] = vdata[4].data[4];  datas[4] = vdata[4].data[5];  datas[4] = vdata[4].data[6];  datas[4] = vdata[4].data[7];  datas[4] = vdata[4].data[8];  datas[4] = vdata[4].data[9];  datas[4] = vdata[4].data[10];  datas[4] = vdata[4].data[11];  datas[4] = vdata[4].data[12];  datas[4] = vdata[4].data[13];  datas[4] = vdata[4].data[14];  datas[4] = vdata[4].data[15];    datas[5] = vdata[5].data[0];  datas[5] = vdata[5].data[1];  datas[5] = vdata[5].data[2];  datas[5] = vdata[5].data[3];  datas[5] = vdata[5].data[4];  datas[5] = vdata[5].data[5];  datas[5] = vdata[5].data[6];  datas[5] = vdata[5].data[7];  datas[5] = vdata[5].data[8];  datas[5] = vdata[5].data[9];  datas[5] = vdata[5].data[10];  datas[5] = vdata[5].data[11];  datas[5] = vdata[5].data[12];  datas[5] = vdata[5].data[13];  datas[5] = vdata[5].data[14];  datas[5] = vdata[5].data[15];    datas[6] = vdata[6].data[0];  datas[6] = vdata[6].data[1];  datas[6] = vdata[6].data[2];  datas[6] = vdata[6].data[3];  datas[6] = vdata[6].data[4];  datas[6] = vdata[6].data[5];  datas[6] = vdata[6].data[6];  datas[6] = vdata[6].data[7];  datas[6] = vdata[6].data[8];  datas[6] = vdata[6].data[9];  datas[6] = vdata[6].data[10];  datas[6] = vdata[6].data[11];  datas[6] = vdata[6].data[12];  datas[6] = vdata[6].data[13];  datas[6] = vdata[6].data[14];  datas[6] = vdata[6].data[15];    datas[7] = vdata[7].data[0];  datas[7] = vdata[7].data[1];  datas[7] = vdata[7].data[2];  datas[7] = vdata[7].data[3];  datas[7] = vdata[7].data[4];  datas[7] = vdata[7].data[5];  datas[7] = vdata[7].data[6];  datas[7] = vdata[7].data[7];  datas[7] = vdata[7].data[8];  datas[7] = vdata[7].data[9];  datas[7] = vdata[7].data[10];  datas[7] = vdata[7].data[11];  datas[7] = vdata[7].data[12];  datas[7] = vdata[7].data[13];  datas[7] = vdata[7].data[14];  datas[7] = vdata[7].data[15];    datas[8] = vdata[8].data[0];  datas[8] = vdata[8].data[1];  datas[8] = vdata[8].data[2];  datas[8] = vdata[8].data[3];  datas[8] = vdata[8].data[4];  datas[8] = vdata[8].data[5];  datas[8] = vdata[8].data[6];  datas[8] = vdata[8].data[7];  datas[8] = vdata[8].data[8];  datas[8] = vdata[8].data[9];  datas[8] = vdata[8].data[10];  datas[8] = vdata[8].data[11];  datas[8] = vdata[8].data[12];  datas[8] = vdata[8].data[13];  datas[8] = vdata[8].data[14];  datas[8] = vdata[8].data[15];    datas[9] = vdata[9].data[0];  datas[9] = vdata[9].data[1];  datas[9] = vdata[9].data[2];  datas[9] = vdata[9].data[3];  datas[9] = vdata[9].data[4];  datas[9] = vdata[9].data[5];  datas[9] = vdata[9].data[6];  datas[9] = vdata[9].data[7];  datas[9] = vdata[9].data[8];  datas[9] = vdata[9].data[9];  datas[9] = vdata[9].data[10];  datas[9] = vdata[9].data[11];  datas[9] = vdata[9].data[12];  datas[9] = vdata[9].data[13];  datas[9] = vdata[9].data[14];  datas[9] = vdata[9].data[15];    datas[10] = vdata[10].data[0];  datas[10] = vdata[10].data[1];  datas[10] = vdata[10].data[2];  datas[10] = vdata[10].data[3];  datas[10] = vdata[10].data[4];  datas[10] = vdata[10].data[5];  datas[10] = vdata[10].data[6];  datas[10] = vdata[10].data[7];  datas[10] = vdata[10].data[8];  datas[10] = vdata[10].data[9];  datas[10] = vdata[10].data[10];  datas[10] = vdata[10].data[11];  datas[10] = vdata[10].data[12];  datas[10] = vdata[10].data[13];  datas[10] = vdata[10].data[14];  datas[10] = vdata[10].data[15];    datas[11] = vdata[11].data[0];  datas[11] = vdata[11].data[1];  datas[11] = vdata[11].data[2];  datas[11] = vdata[11].data[3];  datas[11] = vdata[11].data[4];  datas[11] = vdata[11].data[5];  datas[11] = vdata[11].data[6];  datas[11] = vdata[11].data[7];  datas[11] = vdata[11].data[8];  datas[11] = vdata[11].data[9];  datas[11] = vdata[11].data[10];  datas[11] = vdata[11].data[11];  datas[11] = vdata[11].data[12];  datas[11] = vdata[11].data[13];  datas[11] = vdata[11].data[14];  datas[11] = vdata[11].data[15];    datas[12] = vdata[12].data[0];  datas[12] = vdata[12].data[1];  datas[12] = vdata[12].data[2];  datas[12] = vdata[12].data[3];  datas[12] = vdata[12].data[4];  datas[12] = vdata[12].data[5];  datas[12] = vdata[12].data[6];  datas[12] = vdata[12].data[7];  datas[12] = vdata[12].data[8];  datas[12] = vdata[12].data[9];  datas[12] = vdata[12].data[10];  datas[12] = vdata[12].data[11];  datas[12] = vdata[12].data[12];  datas[12] = vdata[12].data[13];  datas[12] = vdata[12].data[14];  datas[12] = vdata[12].data[15];    datas[13] = vdata[13].data[0];  datas[13] = vdata[13].data[1];  datas[13] = vdata[13].data[2];  datas[13] = vdata[13].data[3];  datas[13] = vdata[13].data[4];  datas[13] = vdata[13].data[5];  datas[13] = vdata[13].data[6];  datas[13] = vdata[13].data[7];  datas[13] = vdata[13].data[8];  datas[13] = vdata[13].data[9];  datas[13] = vdata[13].data[10];  datas[13] = vdata[13].data[11];  datas[13] = vdata[13].data[12];  datas[13] = vdata[13].data[13];  datas[13] = vdata[13].data[14];  datas[13] = vdata[13].data[15];    datas[14] = vdata[14].data[0];  datas[14] = vdata[14].data[1];  datas[14] = vdata[14].data[2];  datas[14] = vdata[14].data[3];  datas[14] = vdata[14].data[4];  datas[14] = vdata[14].data[5];  datas[14] = vdata[14].data[6];  datas[14] = vdata[14].data[7];  datas[14] = vdata[14].data[8];  datas[14] = vdata[14].data[9];  datas[14] = vdata[14].data[10];  datas[14] = vdata[14].data[11];  datas[14] = vdata[14].data[12];  datas[14] = vdata[14].data[13];  datas[14] = vdata[14].data[14];  datas[14] = vdata[14].data[15];    datas[15] = vdata[15].data[0];  datas[15] = vdata[15].data[1];  datas[15] = vdata[15].data[2];  datas[15] = vdata[15].data[3];  datas[15] = vdata[15].data[4];  datas[15] = vdata[15].data[5];  datas[15] = vdata[15].data[6];  datas[15] = vdata[15].data[7];  datas[15] = vdata[15].data[8];  datas[15] = vdata[15].data[9];  datas[15] = vdata[15].data[10];  datas[15] = vdata[15].data[11];  datas[15] = vdata[15].data[12];  datas[15] = vdata[15].data[13];  datas[15] = vdata[15].data[14];  datas[15] = vdata[15].data[15];   // FIXME.
		#else 
 datas[0] = vdata[0].data;  datas[1] = vdata[1].data;  datas[2] = vdata[2].data;  datas[3] = vdata[3].data;  datas[4] = vdata[4].data;  datas[5] = vdata[5].data;  datas[6] = vdata[6].data;  datas[7] = vdata[7].data;  datas[8] = vdata[8].data;  datas[9] = vdata[9].data;  datas[10] = vdata[10].data;  datas[11] = vdata[11].data;  datas[12] = vdata[12].data;  datas[13] = vdata[13].data;  datas[14] = vdata[14].data;  datas[15] = vdata[15].data; 	
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSP2_saveV 23", baseoffset_kvs + offset_kvs + i, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		UTILP2_WriteDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
	}
	return;
}

void acts_all::MEMACCESSP2_readANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks1 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks2 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks3 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks4 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks5 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks6 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks7 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks8 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks9 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks10 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks11 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			vbuffer10[0][index + s] = vdata[0];
			vbuffer10[1][index + s] = vdata[1];
			vbuffer10[2][index + s] = vdata[2];
			vbuffer10[3][index + s] = vdata[3];
			vbuffer10[4][index + s] = vdata[4];
			vbuffer10[5][index + s] = vdata[5];
			vbuffer10[6][index + s] = vdata[6];
			vbuffer10[7][index + s] = vdata[7];
			vbuffer10[8][index + s] = vdata[8];
			vbuffer10[9][index + s] = vdata[9];
			vbuffer10[10][index + s] = vdata[10];
			vbuffer10[11][index + s] = vdata[11];
			vbuffer10[12][index + s] = vdata[12];
			vbuffer10[13][index + s] = vdata[13];
			vbuffer10[14][index + s] = vdata[14];
			vbuffer10[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void acts_all::MEMACCESSP2_readANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP2_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			UTILP2_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
			#ifdef BIT_TRAVERSAL_ALGORITHM
  vdata[0].data[0] = datas[0];  vdata[0].data[1] = datas[0];  vdata[0].data[2] = datas[0];  vdata[0].data[3] = datas[0];  vdata[0].data[4] = datas[0];  vdata[0].data[5] = datas[0];  vdata[0].data[6] = datas[0];  vdata[0].data[7] = datas[0];  vdata[0].data[8] = datas[0];  vdata[0].data[9] = datas[0];  vdata[0].data[10] = datas[0];  vdata[0].data[11] = datas[0];  vdata[0].data[12] = datas[0];  vdata[0].data[13] = datas[0];  vdata[0].data[14] = datas[0];  vdata[0].data[15] = datas[0];    vdata[1].data[0] = datas[1];  vdata[1].data[1] = datas[1];  vdata[1].data[2] = datas[1];  vdata[1].data[3] = datas[1];  vdata[1].data[4] = datas[1];  vdata[1].data[5] = datas[1];  vdata[1].data[6] = datas[1];  vdata[1].data[7] = datas[1];  vdata[1].data[8] = datas[1];  vdata[1].data[9] = datas[1];  vdata[1].data[10] = datas[1];  vdata[1].data[11] = datas[1];  vdata[1].data[12] = datas[1];  vdata[1].data[13] = datas[1];  vdata[1].data[14] = datas[1];  vdata[1].data[15] = datas[1];    vdata[2].data[0] = datas[2];  vdata[2].data[1] = datas[2];  vdata[2].data[2] = datas[2];  vdata[2].data[3] = datas[2];  vdata[2].data[4] = datas[2];  vdata[2].data[5] = datas[2];  vdata[2].data[6] = datas[2];  vdata[2].data[7] = datas[2];  vdata[2].data[8] = datas[2];  vdata[2].data[9] = datas[2];  vdata[2].data[10] = datas[2];  vdata[2].data[11] = datas[2];  vdata[2].data[12] = datas[2];  vdata[2].data[13] = datas[2];  vdata[2].data[14] = datas[2];  vdata[2].data[15] = datas[2];    vdata[3].data[0] = datas[3];  vdata[3].data[1] = datas[3];  vdata[3].data[2] = datas[3];  vdata[3].data[3] = datas[3];  vdata[3].data[4] = datas[3];  vdata[3].data[5] = datas[3];  vdata[3].data[6] = datas[3];  vdata[3].data[7] = datas[3];  vdata[3].data[8] = datas[3];  vdata[3].data[9] = datas[3];  vdata[3].data[10] = datas[3];  vdata[3].data[11] = datas[3];  vdata[3].data[12] = datas[3];  vdata[3].data[13] = datas[3];  vdata[3].data[14] = datas[3];  vdata[3].data[15] = datas[3];    vdata[4].data[0] = datas[4];  vdata[4].data[1] = datas[4];  vdata[4].data[2] = datas[4];  vdata[4].data[3] = datas[4];  vdata[4].data[4] = datas[4];  vdata[4].data[5] = datas[4];  vdata[4].data[6] = datas[4];  vdata[4].data[7] = datas[4];  vdata[4].data[8] = datas[4];  vdata[4].data[9] = datas[4];  vdata[4].data[10] = datas[4];  vdata[4].data[11] = datas[4];  vdata[4].data[12] = datas[4];  vdata[4].data[13] = datas[4];  vdata[4].data[14] = datas[4];  vdata[4].data[15] = datas[4];    vdata[5].data[0] = datas[5];  vdata[5].data[1] = datas[5];  vdata[5].data[2] = datas[5];  vdata[5].data[3] = datas[5];  vdata[5].data[4] = datas[5];  vdata[5].data[5] = datas[5];  vdata[5].data[6] = datas[5];  vdata[5].data[7] = datas[5];  vdata[5].data[8] = datas[5];  vdata[5].data[9] = datas[5];  vdata[5].data[10] = datas[5];  vdata[5].data[11] = datas[5];  vdata[5].data[12] = datas[5];  vdata[5].data[13] = datas[5];  vdata[5].data[14] = datas[5];  vdata[5].data[15] = datas[5];    vdata[6].data[0] = datas[6];  vdata[6].data[1] = datas[6];  vdata[6].data[2] = datas[6];  vdata[6].data[3] = datas[6];  vdata[6].data[4] = datas[6];  vdata[6].data[5] = datas[6];  vdata[6].data[6] = datas[6];  vdata[6].data[7] = datas[6];  vdata[6].data[8] = datas[6];  vdata[6].data[9] = datas[6];  vdata[6].data[10] = datas[6];  vdata[6].data[11] = datas[6];  vdata[6].data[12] = datas[6];  vdata[6].data[13] = datas[6];  vdata[6].data[14] = datas[6];  vdata[6].data[15] = datas[6];    vdata[7].data[0] = datas[7];  vdata[7].data[1] = datas[7];  vdata[7].data[2] = datas[7];  vdata[7].data[3] = datas[7];  vdata[7].data[4] = datas[7];  vdata[7].data[5] = datas[7];  vdata[7].data[6] = datas[7];  vdata[7].data[7] = datas[7];  vdata[7].data[8] = datas[7];  vdata[7].data[9] = datas[7];  vdata[7].data[10] = datas[7];  vdata[7].data[11] = datas[7];  vdata[7].data[12] = datas[7];  vdata[7].data[13] = datas[7];  vdata[7].data[14] = datas[7];  vdata[7].data[15] = datas[7];    vdata[8].data[0] = datas[8];  vdata[8].data[1] = datas[8];  vdata[8].data[2] = datas[8];  vdata[8].data[3] = datas[8];  vdata[8].data[4] = datas[8];  vdata[8].data[5] = datas[8];  vdata[8].data[6] = datas[8];  vdata[8].data[7] = datas[8];  vdata[8].data[8] = datas[8];  vdata[8].data[9] = datas[8];  vdata[8].data[10] = datas[8];  vdata[8].data[11] = datas[8];  vdata[8].data[12] = datas[8];  vdata[8].data[13] = datas[8];  vdata[8].data[14] = datas[8];  vdata[8].data[15] = datas[8];    vdata[9].data[0] = datas[9];  vdata[9].data[1] = datas[9];  vdata[9].data[2] = datas[9];  vdata[9].data[3] = datas[9];  vdata[9].data[4] = datas[9];  vdata[9].data[5] = datas[9];  vdata[9].data[6] = datas[9];  vdata[9].data[7] = datas[9];  vdata[9].data[8] = datas[9];  vdata[9].data[9] = datas[9];  vdata[9].data[10] = datas[9];  vdata[9].data[11] = datas[9];  vdata[9].data[12] = datas[9];  vdata[9].data[13] = datas[9];  vdata[9].data[14] = datas[9];  vdata[9].data[15] = datas[9];    vdata[10].data[0] = datas[10];  vdata[10].data[1] = datas[10];  vdata[10].data[2] = datas[10];  vdata[10].data[3] = datas[10];  vdata[10].data[4] = datas[10];  vdata[10].data[5] = datas[10];  vdata[10].data[6] = datas[10];  vdata[10].data[7] = datas[10];  vdata[10].data[8] = datas[10];  vdata[10].data[9] = datas[10];  vdata[10].data[10] = datas[10];  vdata[10].data[11] = datas[10];  vdata[10].data[12] = datas[10];  vdata[10].data[13] = datas[10];  vdata[10].data[14] = datas[10];  vdata[10].data[15] = datas[10];    vdata[11].data[0] = datas[11];  vdata[11].data[1] = datas[11];  vdata[11].data[2] = datas[11];  vdata[11].data[3] = datas[11];  vdata[11].data[4] = datas[11];  vdata[11].data[5] = datas[11];  vdata[11].data[6] = datas[11];  vdata[11].data[7] = datas[11];  vdata[11].data[8] = datas[11];  vdata[11].data[9] = datas[11];  vdata[11].data[10] = datas[11];  vdata[11].data[11] = datas[11];  vdata[11].data[12] = datas[11];  vdata[11].data[13] = datas[11];  vdata[11].data[14] = datas[11];  vdata[11].data[15] = datas[11];    vdata[12].data[0] = datas[12];  vdata[12].data[1] = datas[12];  vdata[12].data[2] = datas[12];  vdata[12].data[3] = datas[12];  vdata[12].data[4] = datas[12];  vdata[12].data[5] = datas[12];  vdata[12].data[6] = datas[12];  vdata[12].data[7] = datas[12];  vdata[12].data[8] = datas[12];  vdata[12].data[9] = datas[12];  vdata[12].data[10] = datas[12];  vdata[12].data[11] = datas[12];  vdata[12].data[12] = datas[12];  vdata[12].data[13] = datas[12];  vdata[12].data[14] = datas[12];  vdata[12].data[15] = datas[12];    vdata[13].data[0] = datas[13];  vdata[13].data[1] = datas[13];  vdata[13].data[2] = datas[13];  vdata[13].data[3] = datas[13];  vdata[13].data[4] = datas[13];  vdata[13].data[5] = datas[13];  vdata[13].data[6] = datas[13];  vdata[13].data[7] = datas[13];  vdata[13].data[8] = datas[13];  vdata[13].data[9] = datas[13];  vdata[13].data[10] = datas[13];  vdata[13].data[11] = datas[13];  vdata[13].data[12] = datas[13];  vdata[13].data[13] = datas[13];  vdata[13].data[14] = datas[13];  vdata[13].data[15] = datas[13];    vdata[14].data[0] = datas[14];  vdata[14].data[1] = datas[14];  vdata[14].data[2] = datas[14];  vdata[14].data[3] = datas[14];  vdata[14].data[4] = datas[14];  vdata[14].data[5] = datas[14];  vdata[14].data[6] = datas[14];  vdata[14].data[7] = datas[14];  vdata[14].data[8] = datas[14];  vdata[14].data[9] = datas[14];  vdata[14].data[10] = datas[14];  vdata[14].data[11] = datas[14];  vdata[14].data[12] = datas[14];  vdata[14].data[13] = datas[14];  vdata[14].data[14] = datas[14];  vdata[14].data[15] = datas[14];    vdata[15].data[0] = datas[15];  vdata[15].data[1] = datas[15];  vdata[15].data[2] = datas[15];  vdata[15].data[3] = datas[15];  vdata[15].data[4] = datas[15];  vdata[15].data[5] = datas[15];  vdata[15].data[6] = datas[15];  vdata[15].data[7] = datas[15];  vdata[15].data[8] = datas[15];  vdata[15].data[9] = datas[15];  vdata[15].data[10] = datas[15];  vdata[15].data[11] = datas[15];  vdata[15].data[12] = datas[15];  vdata[15].data[13] = datas[15];  vdata[15].data[14] = datas[15];  vdata[15].data[15] = datas[15];   // FIXME.
			#else 
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP2_readANDRVchunks12 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			vbuffer10[0][index + s] = vdata[0];
			vbuffer10[1][index + s] = vdata[1];
			vbuffer10[2][index + s] = vdata[2];
			vbuffer10[3][index + s] = vdata[3];
			vbuffer10[4][index + s] = vdata[4];
			vbuffer10[5][index + s] = vdata[5];
			vbuffer10[6][index + s] = vdata[6];
			vbuffer10[7][index + s] = vdata[7];
			vbuffer10[8][index + s] = vdata[8];
			vbuffer10[9][index + s] = vdata[9];
			vbuffer10[10][index + s] = vdata[10];
			vbuffer10[11][index + s] = vdata[11];
			vbuffer10[12][index + s] = vdata[12];
			vbuffer10[13][index + s] = vdata[13];
			vbuffer10[14][index + s] = vdata[14];
			vbuffer10[15][index + s] = vdata[15];
			vbuffer11[0][index + s] = vdata[0];
			vbuffer11[1][index + s] = vdata[1];
			vbuffer11[2][index + s] = vdata[2];
			vbuffer11[3][index + s] = vdata[3];
			vbuffer11[4][index + s] = vdata[4];
			vbuffer11[5][index + s] = vdata[5];
			vbuffer11[6][index + s] = vdata[6];
			vbuffer11[7][index + s] = vdata[7];
			vbuffer11[8][index + s] = vdata[8];
			vbuffer11[9][index + s] = vdata[9];
			vbuffer11[10][index + s] = vdata[10];
			vbuffer11[11][index + s] = vdata[11];
			vbuffer11[12][index + s] = vdata[12];
			vbuffer11[13][index + s] = vdata[13];
			vbuffer11[14][index + s] = vdata[14];
			vbuffer11[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}

// -------------------- stats -------------------- //
void acts_all::MEMACCESSP2_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS + 1, NAp, NAp, NAp);
	#endif
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<1 + globalparams.NUM_REDUCEPARTITIONS; i++){
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
	for (buffer_type i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ actsutilityobj->checkoutofbounds("saveglobalstats", globalstatsbuffer[i].key + globalstatsbuffer[i].value, globalparams.SIZE_KVDRAM, NAp, NAp, NAp); }
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	actsutilityobj->printkeyvalues("mem_access:: readglobalstats.globalstatsbuffer", globalstatsbuffer, 1 + globalparams.NUM_REDUCEPARTITIONS); 
	#endif
	// exit(EXIT_SUCCESS); 
	return;
}

#ifdef NOTUSED____________________________________
unsigned int acts_all::MEMACCESSP2_getvptr(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int offset){
	keyvalue_t vptr_kv;
	
	uint512_dt V = kvdram[baseoffset_kvs + (offset / VECTOR2_SIZE)];
	unsigned int M = (offset % VECTOR2_SIZE) / 2;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP2_getvptr: baseoffset_kvs: "<<baseoffset_kvs<<", offset: "<<offset<<", baseoffset_kvs + (offset / VECTOR2_SIZE): "<<baseoffset_kvs + (offset / VECTOR2_SIZE)<<endl;
	
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

tuple_t acts_all::MEMACCESSP2_getvptrs_opt(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int beginoffset, unsigned int endoffset, unsigned int edgebankID, globalparams_t globalparams){
	#pragma HLS INLINE 
	keyy_t beginvptr = 0;
	keyy_t endvptr = 0;
	
	beginoffset = beginoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO; // convert-to-appropriate-skip-format
	endoffset = endoffset / globalparams.ACTSPARAMS_VPTR_SHRINK_RATIO;
	// cout<<"-----------+++++++++++++++++++---MEMACCESSP2_getvptrs_opt: baseoffset_kvs: "<<baseoffset_kvs<<", beginoffset: "<<beginoffset<<", endoffset: "<<endoffset<<", VPTR_SHRINK_RATIO: "<<NAp<<endl;
	
	beginvptr = MEMACCESSP2_getvptr(kvdram, baseoffset_kvs, beginoffset);
	endvptr = MEMACCESSP2_getvptr(kvdram, baseoffset_kvs, endoffset);

	tuple_t t; t.A = beginvptr; t.B = endvptr;
	return t;
}
#endif 

unsigned int acts_all::MEMACCESSP2_getdata(uint512_dt * kvdram, unsigned int baseoffset_kvs, unsigned int loc){
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

void acts_all::MEMACCESSP2_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
void acts_all::MEMACCESSP2_readhelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, unsigned int actsinstance, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		UTILP2_ReadDatas(vdram, offset_kvs + i, datas);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
		
		pmask[i] = datas[GraphIter];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> readpmasks: printing active vertex partitions: GraphIter: "<<GraphIter<<": ";
	unsigned int num_actvps = 0;
	for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){
		if(pmask[t] > 0  && t < 16){ cout<<t<<", "; }
		if(pmask[t] > 0){ num_actvps += 1; }
	}
	cout<<" ("<<num_actvps<<" active partitions, "<<size_kvs<<" total partitions)"<< RESET << endl;
	#endif 
	return;
}









	
#endif 
#ifdef CONFIG_ENABLECLASS_ACTS_MERGE
void acts_all::MERGEP2_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR_SIZE; v++){	
		unsigned int K = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].key;
		unsigned int V = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].value;
		// if(MEMCAP2_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }
		// if(MEMCAP2_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }	
	}
	#endif 
	return;
}

void acts_all::MERGEP2_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> mergeVs:: merging vertices..."<< RESET <<endl; 
	#endif
	
	globalparams_t globalparams = UTILP2_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP2_getglobalparams(vdram, 0);
	
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
	MERGEP2_MERGEVSLOOP2: for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		if(globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + slrvoffset_kvs2 + voffset_kvs2 + globalparams.SIZEKVS2_REDUCEPARTITION < globalparamsv.ACTSPARAMS_MAXHBMCAPACITY_KVS){
		MERGEP2_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){ // globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, globalparams.SIZEKVS2_REDUCEPARTITION
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("mergeVs: ERROR 21", slrvoffset_kvs2 + voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, slrvoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
			actsutilityobj->checkoutofbounds("mergeVs: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); 
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + slrvoffset_kvs2 + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
				value_t combo; if(v%2==0){ combo = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k].data[v/2].key; } else { combo = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k].data[v/2].key; }
				value_t mask; if(globalparamsv.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { mask = combo & 0x1; }
				value_t vdata = combo >> 1; 
				if(mask == 1 && (reduce_partition * globalparams.SIZEKVS2_REDUCEPARTITION + k) < (globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE)){ cout<<">>> MERGE:: MERGE VERTICES SEEN @ reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", srcvid: "<<NAp<<", dstvid*: "<<NAp<<", combo: "<<combo<<", globalparams.SIZE_DESTVERTICESDATA: "<<globalparams.SIZE_DESTVERTICESDATA<<endl; }
			}
			#endif
				
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			MERGEP2_print_active_masks(vdram, globalparamsv, slrvoffset_kvs2 + voffset_kvs2 + k);	
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

unsigned int acts_all::MERGEP2_copy(uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, unsigned int voffset_kvs, unsigned int num_compute_units, 
		globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2){
	unsigned int total_sync = 0;
	MERGEP2_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<num_compute_units; i++){
		for(unsigned int reduce_partition=0; reduce_partition<globalparamsSRC.NUM_REDUCEPARTITIONS; reduce_partition++){
			if(globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + globalparamsSRC.SIZEKVS2_REDUCEPARTITION < globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS){
			MERGEP2_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsSRC.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 21", globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 22", globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				total_sync += VECTOR2_SIZE;
				#endif
				
				vdramDST1[globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k] = vdramSRC[globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k];
				vdramDST2[globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k] = vdramSRC[globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
					value_t combo; if(v%2==0){ combo = vdramSRC[globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k].data[v/2].key; } else { combo = vdramSRC[globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + k].data[v/2].key; }
					value_t mask; if(globalparamsDST1.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { mask = combo & 0x1; }
					value_t vdata = combo >> 1; 
					// if(mask == 1){ cout<<">>> EXCHANGE:: EXCHANGE VERTICES SEEN @ i: "<<i<<", reduce_partition: "<<reduce_partition<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", srcvid: "<<UTILP2_GETREALVID((edge_data.value * EDGEDATA_PACKINGSIZE) + v, globalparams.ACTSPARAMS_INSTID)<<", dstvid*: "<<edge_data.key<<endl; }
					if(mask == 1){ cout<<">>> EXCHANGE:: EXCHANGE VERTICES SEEN @ i: "<<i<<", reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", srcvid: "<<NAp<<", dstvid*: "<<NAp<<endl; }
					// if(mask == 1 && (reduce_partition * globalparamsDST1.SIZEKVS2_REDUCEPARTITION + k) < (globalparamsDST1.SIZE_DESTVERTICESDATA / VECTOR2_SIZE)){ cout<<">>> EXCHANGE:: EXCHANGE VERTICES SEEN @ i: "<<i<<", reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", srcvid: "<<NAp<<", dstvid*: "<<NAp<<endl; }
					// if(k<4){cout<<">>> ------ EXCHANGE:: EXCHANGE VERTICES SEEN @ i: "<<i<<", reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", srcvid: "<<NAp<<", dstvid*: "<<NAp<<", 0xFFFFFFFE: "<<0xFFFFFFFE<<endl;}
				}
				#endif
	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				MERGEP2_print_active_masks(vdramSRC, globalparamsSRC, voffset_kvs);
				#endif
			}
			// exit(EXIT_SUCCESS);
			}
			voffset_kvs += globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
		}
	}
	return total_sync;
}

void acts_all::MERGEP2_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> exchangeVs:: exchanging vertices across different SLRs..." << RESET <<endl; 
	#endif

	unsigned int total_sync[3]; total_sync[0]=0; total_sync[1]=0; total_sync[2]=0; 
	#pragma HLS ARRAY_PARTITION variable=total_sync complete

	globalparams_t globalparamsvA = UTILP2_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP2_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP2_getglobalparams(vdramC, 0);
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR1) * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	
	total_sync[0] = MERGEP2_copy(vdramA, vdramB, vdramC, voffsetA_kvs, NUMCOMPUTEUNITS_SLR2, globalparamsvA, globalparamsvB, globalparamsvC);
	total_sync[1] = MERGEP2_copy(vdramB, vdramA, vdramC, voffsetB_kvs, NUMCOMPUTEUNITS_SLR1, globalparamsvB, globalparamsvA, globalparamsvC);
	total_sync[2] = MERGEP2_copy(vdramC, vdramA, vdramB, voffsetC_kvs, NUMCOMPUTEUNITS_SLR0, globalparamsvC, globalparamsvA, globalparamsvB);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"finished. "<<total_sync[0] + total_sync[1] + total_sync[2]<<" vertices synchronized"<<endl; 
	#endif
	return;
}

extern "C" {
void acts_all::TOPP2_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
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
	globalparams_t globalparamsvA = UTILP2_getglobalparams(vdramA, 0);
	unsigned int inACTSMODE = UTILP2_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"~~~ TOPP2_topkernelS:: GraphIter: "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<endl;
	#endif
	
	#ifdef CONFIG_RELEASE_VERSION4
	if(globalparamsvA.ENABLE_EXCHANGECOMMAND == ON){
		MERGEP2_exchange(vdramA, vdramB, vdramC);
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
	if(mask == 1){ cout<<">>> PROCESS VECTOR:: PROCESS EDGE SEEN @ v: "<<v<<", loc: "<<loc<<", edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", udata: "<<udata<<", mask: "<<mask<<", srcvid: "<<UTILP2_GETREALVID((edge_data.value * EDGEDATA_PACKINGSIZE) + v, globalparams.ACTSPARAMS_INSTID)<<", dstvid*: "<<edge_data.key<<endl; }
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
		cout<<">>> REDUCE VECTOR:: REDUCE UPDATE SEEN @: v: "<<v<<", loc: "<<loc<<", vdata_tmp: "<<vdata_tmp<<", mask: "<<mask<<", update_data.value: "<<update_data.value<<", new_vprop: "<<new_vprop<<", new combo: "<<((new_vprop << 1) | 0x1)<<", dstvid: "<<UTILP2_GETREALVID(update_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; 
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
		uint512_evec_dt data = UTILP2_ReadEdges(in, sourcebaseaddr_kvs + offset_kvs + i);
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
		UTILP2_WriteDatas(out, destbaseaddr_kvs + offset_kvs + i, datas2);
	}
}
#endif 

void acts_all::ACTSP2_read_process_partition_and_write(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
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
		UTILP2_ReadEdges(in, sourcebaseaddr_kvs + srcoffset_kvs + i, datas);
		
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
		UTILP2_WriteDatas(out, destbaseaddr_kvs + destoffset_kvs + i, datas2);
		
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(enx == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE); }
		#endif 
	}
	// exit(EXIT_SUCCESS);
}

void acts_all::ACTSP2_read_process_partition_and_write_base(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],	
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
	ACTSP2_read_process_partition_and_write(kvdram, kvdram, vbuffer_source,
		sourcebaseaddr_kvs, workload_kvs.offset_begin, destbaseaddr_kvs, destoffset_kvs, workload_kvs.size, upperlimit, 
					collections, globalparamsK, globalposition);
	#endif
}

void acts_all::ACTSP2_read_and_reduce(unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	
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
		UTILP2_ReadDatas(kvdram, sourcebaseaddr_kvs + offset_kvs, datas);
		
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

void acts_all::ACTSP2_read_process_and_reduce(uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_sourcebits[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],
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
				workload_kvs.offset_begin = MEMACCESSP2_getdata(in, globalparamsE.BASEOFFSETKVS_ACTIVEEDGESMAP, index);
				workload_kvs.size = MEMACCESSP2_getdata(in, globalparamsE.BASEOFFSETKVS_ACTIVEEDGESMAP, index + 1);
				workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			} else {
				unsigned int index = (sweepparams.source_partition * num_LLPset) + llp_set;
				workload_kvs.offset_begin = MEMACCESSP2_getdata(in, globalparamsE.BASEOFFSETKVS_EDGESMAP, index) / UPDATEDATA_PACKINGSIZE;
				workload_kvs.offset_end = MEMACCESSP2_getdata(in, globalparamsE.BASEOFFSETKVS_EDGESMAP, index + 1) / UPDATEDATA_PACKINGSIZE;
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
				UTILP2_ReadEdges(in, globalparamsE.BASEOFFSETKVS_EDGESDATA + offset + i, datas);
				
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

void acts_all::ACTSP2_actit(bool_type enable, unsigned int mode,
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
		ACTSP2_read_process_and_reduce(kvdram, kvdram, vbuffer_sourcebits, vbuffer_dest, collections_tmp, sweepparams, globalparamsE, globalparamsK, globalposition); 
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
				workload_kvs.offset_begin = MEMACCESSP2_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, (sweepparams.source_partition * num_LLPset) + llp_set) / EDGEDATA_PACKINGSIZE;
				workload_kvs.offset_end = MEMACCESSP2_getdata(kvdram, globalparamsE.BASEOFFSETKVS_EDGESMAP, (sweepparams.source_partition * num_LLPset) + llp_set + 1) / EDGEDATA_PACKINGSIZE;
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
				
				ACTSP2_read_process_partition_and_write_base(kvdram, vbuffer_source,	
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
		
			ACTSP2_read_and_reduce(mode, kvdram, vbuffer_dest,	
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

void acts_all::ACTSP2_actit_base(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],				
		globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
		bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	ACTSP2_actit(enable, mode, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sourcebaseaddr_kvs, destbaseaddr_kvs, resetenv, flush, edgebankID, collections);
}	
#endif 			
void acts_all::TOPP2_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	for(buffer_type t=0; t<globalparamsK.SIZEKVS2_REDUCEPARTITION; t++){
		for(buffer_type v=0; v<VDATA_PACKINGSIZE; v++){
			if(MEMCAP2_READVMASK(vbuffer_source[v][t]) == 1){ cout<<"TOPP2_processit: ACTIVE MASK SEEN: @ t: "<<t<<", v: "<<v<<", vbuffer_source["<<v<<"]["<<t<<"]: "<<vbuffer_source[v][t]<<endl; }
		}
	}
	#endif

	sweepparams_t sweepparams;
	sweepparams = UTILP2_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); //. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	batch_type voffset_kvs = globalposition.source_partition * UTILP2_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print7("### TOPP2_processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, NAp, sweepparams.currentLOP); 							
	#endif

	bool returncmd = false;
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	if(globalposition.num_active_vertices == 0){ returncmd = true; } // check if vertex partition is active
	else{
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"processit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; 
		#endif 
	}
	#endif 
	if(GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ returncmd = false; } // for certain algorithms like pagerank
	if(voffset_kvs >= avtravstate.end_kvs){ returncmd = true; } // check if we are at the end of file
	
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && globalposition.stage == 0 && globalposition.source_partition == globalposition.first_source_partition){
		MEMACCESSP2_readV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); 
	}
	
	if(returncmd == false){
		sweepparams.source_partition = globalposition.source_partition;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print5("### TOPP2_processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, MAX_SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		#ifdef NOTUSED____________________________________
		vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + (UTILP2_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + (UTILP2_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK) * VECTOR2_SIZE) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
			
		unsigned int SKIP_KVS = UTILP2_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
		
		tuple_t tup = MEMACCESSP2_getvptrs_opt(kvdram, vptrbaseoffset_kvs, voffset_kvs * VECTOR2_SIZE, (voffset_kvs + SKIP_KVS) * VECTOR2_SIZE, edgebankID, globalparamsK);
		keyy_t beginvptr = tup.A;
		keyy_t endvptr = tup.B; 
		
		#ifdef _DEBUGMODE_CHECKS3
		if(globalparamsK.ACTSCONFIG_INSERTSTATSMETADATAINEDGES == 1){ if(beginvptr % VECTOR2_SIZE != 0 || endvptr % VECTOR2_SIZE != 0){ cout<<"TOPP2_processit::ERROR: beginvptr("<<beginvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0 || endvptr("<<endvptr<<") % VECTOR2_SIZE("<<VECTOR2_SIZE<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }}
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
		if(localendvptr < localbeginvptr){ cout<<"TOPP2_processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<", voffset_kvs: "<<voffset_kvs<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("TOPP2_processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
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
		#endif 
		// exit(EXIT_SUCCESS);
		
		travstate_t etravstate; bool_type resetenv = ON; bool_type flush = ON; 
		
		ACTSP2_actit_base(
			ON, ACTSPROCESSMODE,
			kvdram, vbuffer_source, vbuffer_dest, globalcapsule,
			globalparamsE, globalparamsK, globalposition, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAM,
			resetenv, flush, edgebankID, collections);
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPP2_processit", globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS); }
	#endif	
	bool endofrun = false;
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ 
		// cout<<"---^^^ collections[TRAVERSEDEDGES_COLLECTIONID].data1: "<<collections[TRAVERSEDEDGES_COLLECTIONID].data1<<endl;
		// cout<<"---^^^ collections[PROCESSEDGES_COLLECTIONID].data1: "<<collections[PROCESSEDGES_COLLECTIONID].data1<<endl;
		// cout<<"---^^^ collections[REDUCEUPDATES_COLLECTIONID].data1: "<<collections[REDUCEUPDATES_COLLECTIONID].data1<<endl;
		endofrun = true; }
	if(endofrun == true){ UTILP2_SetFirstData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[TRAVERSEDEDGES_COLLECTIONID].data1); }
	if(endofrun == true){ UTILP2_SetSecondData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[PROCESSEDGES_COLLECTIONID].data1); }
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && endofrun == true){ UTILP2_SetThirdData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[REDUCEUPDATES_COLLECTIONID].data1); }
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1 && endofrun == true){ MEMACCESSP2_saveV(ON, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, 0, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK); }

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

void acts_all::TOPP2_partitionit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	if(globalparamsK.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ return; } ///
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nusrcv_nudstv::UTILP2_getglobalparams:: printing global parameters", globalparamsK);
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE]; 
	globalposition_t globalposition; // DUMMY.
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparamsK.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparamsK.ACTSPARAMS_BEGINLOP + 1 + (globalparamsK.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = UTILP2_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"TOPP2_partitionit: source_partition "<<source_partition<<" ... "<<endl;

			UTILP2_resetkeysandvalues(globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
		
			sweepparams = UTILP2_getsweepparams(globalparamsK, currentLOP, source_partition);
			travstate_t ptravstate = UTILP2_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparamsK.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### TOPP2_partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			UTILP2_resetvalues(globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			ACTSP2_actit_base(config.enablepartition, ACTSPARTITIONMODE,
					kvdram, vbuffer_source, vbuffer_dest, globalcapsule, // CRITICAL FIXME.
					globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp, collections);
					
			// MEMACCESSP2_saveglobalstats(config.enablepartition, kvdram, globalcapsule, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK); 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS;
			}
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalcapsule, sweepparams); }
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

void acts_all::TOPP2_reduceit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	globalposition_t globalposition; globalposition.source_partition = NAp; // DUMMY.
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	
	sweepparams = UTILP2_getsweepparams(globalparamsK, currentLOP, source_partition);
	travstate_t ptravstate; ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; // DUMMY
	config.enablereduce = ON;
	
	ACTSP2_actit_base(config.enablereduce, ACTSREDUCEMODE,
			 kvdram, vbuffer_source, vbuffer_dest, globalcapsule, // CRITICAL FIXME.
			globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp, collections);
	return;
}

void acts_all::TOPP2_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(en_process == ON){ TOPP2_processit(kvdram, vbuffer_source, vbuffer_dest, globalcapsule, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(en_partition == ON){ TOPP2_partitionit(kvdram, vbuffer_source, vbuffer_dest, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(en_reduce == ON){ TOPP2_reduceit(kvdram, vbuffer_source, vbuffer_dest, globalcapsule, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	return;
} 

void acts_all::TOPP2_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	unsigned int sourcestatsmarker = 1;
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int vmask_offset_kvs = 0;
	unsigned int vmaskp_offset_kvs = 0;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = globalparamsK.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("--(((()))))- TOPP2_dispatch_reduce", globalcapsule, globalparamsK.NUM_REDUCEPARTITIONS);
	#endif
	
	bool_type enablereduce = ON;
	DISPATCHREDUCEP2_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print4("### TOPP2_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "vreadoffset_kvs2", source_partition, currentLOP, num_source_partitions, vreadoffset_kvs2); 							
		#endif
		
		// read vertices 
		#ifdef CONFIG_RELEASE_VERSION6
		MEMACCESSP2_readV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		#endif 
		
		// reduce
		TOPP2_dispatch(OFF, OFF, enablereduce, kvdram, vbuffer_source, vbuffer_dest, globalcapsule, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);
		
		// writeback vertices 
		#ifdef CONFIG_RELEASE_VERSION6
		MEMACCESSP2_saveV(enablereduce, kvdram, vbuffer_dest, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, globalparamsK.BUFFERBASEOFFSETKVS_VDATA1, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalposition, globalparamsK);
		#endif 
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		vmaskp_offset_kvs += NUM_PEs;
	}
	
	UTILP2_SetThirdData(kvdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, collections[REDUCEUPDATES_COLLECTIONID].data1);
	// exit(EXIT_SUCCESS);
	return;
} 

void acts_all::TOPP2_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[COLLECTIONS_BUFFERSZ]){									

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	unsigned int printheader1=OFF; // ON, OFF*
	unsigned int printheader2=OFF; 
	if(true && globalposition.source_partition==globalposition.first_source_partition){ printheader1 = ON; } else { printheader1 = OFF; }
	if(false && globalposition.source_partition==globalposition.last_source_partition){ printheader2 = ON; } else { printheader2 = OFF; }
	// printheader1=ON;
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> ====================== ACTS Launched... size: "<<UTILP2_GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<" ====================== "<<endl;
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	unsigned int PARTITIONP2_CHKPT[EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
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
	
	globalparamsK = UTILP2_getglobalparams(kvdram, 0); // CRITICAL OPTIMIZEME. MOVETOBASE?
	
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram, e+1); }
	_globalparamsE = globalparamsEs[globalposition.edgebankID];
	globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid;
	for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE. IS THIS OPTIMIZED
	
	globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = GraphIter; // overrride these with GraphIter
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e].ALGORITHMINFO_GRAPHITERATIONID = GraphIter; }
	_globalparamsE.ALGORITHMINFO_GRAPHITERATIONID = GraphIter;
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ UTILP2_resetkvstatvalues(kvdram, globalparamsK); } // NEWCHANGE
	#endif
	
	if(globalposition.stage == 0 && globalposition.source_partition == globalposition.first_source_partition){ MEMACCESSP2_readglobalstats(ON, kvdram, globalcapsule, globalparamsK.BASEOFFSETKVS_STATSDRAM, globalparamsK); }
	if(globalposition.stage == 0 && globalposition.source_partition == globalposition.first_source_partition){ UTILP2_resetvalues(globalcapsule, globalparamsK.NUM_REDUCEPARTITIONS, 0); } // CRITICAL NEWCHANGE.
		
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP2_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP2_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
				vbuffer_dest, 
					#else 
						vbuffer_source,	
							#endif 
								globalcapsule, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP2_CHKPT, globalposition.v_chunkid, globalposition.edgebankID, collections); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE  // CRITICAL REMOVEME URGENT.
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP2_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP2_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
			vbuffer_dest, 
				#else 
					vbuffer_source,		
						#endif 
							globalcapsule, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP2_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	// reduce & partition
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP2_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP2_dispatch_reduce(kvdram, vbuffer_source, 
			#ifdef CONFIG_SEPERATESRCVDESTVBUFFER
			vbuffer_dest, 
				#else 
					vbuffer_source,		
						#endif 
							globalcapsule, _globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONP2_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	bool endofrun = false;
	if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){ if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }} 
	else { if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ endofrun = true; }}	
	if(endofrun == true){ UTILP2_increment_graphiteration(kvdram, globalparamsK); } // NB: this should come last.
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(endofrun == true){ actsutilityobj->printglobalvars2(globalparamsK.ACTSPARAMS_INSTID, "instID", "v_chunkid", "LOP", "sourcep", globalparamsK.ACTSPARAMS_INSTID, globalposition.v_chunkid, globalposition.currentLOP, globalposition.source_partition); }
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void acts_all::TOPP2_topkernelP1(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL1_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL1_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL1_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks1(ON, vdram, vbuffer_source0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP2(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL2_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL2_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL2_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks2(ON, vdram, vbuffer_source0,vbuffer_source1, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP3(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL3_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL3_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL3_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks3(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP4(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL4_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL4_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL4_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks4(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP5(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL5_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL5_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL5_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks5(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalcapsule4, globalposition, globalparamsV, collections4);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	
		MERGEP2_mergeVs(kvdram4, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP6(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL6_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL6_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL6_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks6(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalcapsule5, globalposition, globalparamsV, collections5);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	
		MERGEP2_mergeVs(kvdram4, vdram); 
	
		MERGEP2_mergeVs(kvdram5, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP7(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL7_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL7_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL7_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks7(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalcapsule6, globalposition, globalparamsV, collections6);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	
		MERGEP2_mergeVs(kvdram4, vdram); 
	
		MERGEP2_mergeVs(kvdram5, vdram); 
	
		MERGEP2_mergeVs(kvdram6, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP8(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL8_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL8_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL8_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks8(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalcapsule7, globalposition, globalparamsV, collections7);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	
		MERGEP2_mergeVs(kvdram4, vdram); 
	
		MERGEP2_mergeVs(kvdram5, vdram); 
	
		MERGEP2_mergeVs(kvdram6, vdram); 
	
		MERGEP2_mergeVs(kvdram7, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP9(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL9_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL9_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL9_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks9(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalcapsule8, globalposition, globalparamsV, collections8);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	
		MERGEP2_mergeVs(kvdram4, vdram); 
	
		MERGEP2_mergeVs(kvdram5, vdram); 
	
		MERGEP2_mergeVs(kvdram6, vdram); 
	
		MERGEP2_mergeVs(kvdram7, vdram); 
	
		MERGEP2_mergeVs(kvdram8, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP10(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL10_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL10_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL10_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks10(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalcapsule9, globalposition, globalparamsV, collections9);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	
		MERGEP2_mergeVs(kvdram4, vdram); 
	
		MERGEP2_mergeVs(kvdram5, vdram); 
	
		MERGEP2_mergeVs(kvdram6, vdram); 
	
		MERGEP2_mergeVs(kvdram7, vdram); 
	
		MERGEP2_mergeVs(kvdram8, vdram); 
	
		MERGEP2_mergeVs(kvdram9, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP11(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule10[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  collections10[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL11_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL11_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL11_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks11(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalcapsule9, globalposition, globalparamsV, collections9);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, globalcapsule10, globalposition, globalparamsV, collections10);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	
		MERGEP2_mergeVs(kvdram4, vdram); 
	
		MERGEP2_mergeVs(kvdram5, vdram); 
	
		MERGEP2_mergeVs(kvdram6, vdram); 
	
		MERGEP2_mergeVs(kvdram7, vdram); 
	
		MERGEP2_mergeVs(kvdram8, vdram); 
	
		MERGEP2_mergeVs(kvdram9, vdram); 
	
		MERGEP2_mergeVs(kvdram10, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
void acts_all::TOPP2_topkernelP12(
	
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule10[BLOCKRAM_SIZE];	
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
	
	keyvalue_t globalcapsule11[BLOCKRAM_SIZE];	
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

	globalparams_t globalparamsK = UTILP2_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILP2_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILP2_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  collections10[t].data1 = 0;  collections11[t].data1 = 0;  }
	
	unsigned int PARTITIONP2_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NEWCHANGE
	LOADPARTITIONCHKPTS_LOOP1B: for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){	
		PARTITIONP2_CHKPT[0][u] = kvdram0[globalparamsEs[0].DRAM_BASE_KVS + BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_INMESSAGES_EDGESSTATSDRAM + u]
			#ifdef _WIDEWORD
				.range(31, 0); 
					#else 
						.data[0].key; 	
							#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ cout<<">>> topkernelP: PARTITIONP2_CHKPT["<<u<<"]: "<<PARTITIONP2_CHKPT[u]<<endl; }
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
	UTILP2_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	#endif 
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP2_resetkvstatvalues(vdram, globalparamsV); }
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
		UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL12_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print3("### TOPKERNEL12_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
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
					if(stage == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILP2_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(globalparamsK.ACTSPARAMS_TREEDEPTH == 1){
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
						else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0;  } 
						else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = 0; }					
					} else {
						if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP2_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP2_CHKPT[0][v_chunkid+1]; } 
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
						if(stage == 0 && source_partition == 0){ UTILP2_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL12_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP2_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
							MEMACCESSP2_readhelperstats(vdram, emask_curr, globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID, globalparamsV);
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
						unsigned int num_active_vertices_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						unsigned int num_active_edges_in_channel = emask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_vertices_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_vertices_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_vertices_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_vertices = num_active_vertices_in_partition;
						globalposition.num_active_edges_in_channel += num_active_edges_in_channel;
						total_num_active_vertices_in_iteration += num_active_vertices_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_vertices < globalparamsK.THRESHOLD_HYBRIDGPMODE_HYBRIDVTHRESHOLD_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_vertices_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						// bool endofrun = false;
						if(globalposition.num_active_vertices == 0){
							if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ enableprocess = ON; } 
							else { enableprocess = OFF; }}
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3
						if(enable_readandreplicatevdata == ON){
							MEMACCESSP2_readANDRVchunks12(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10,vbuffer_source11, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalposition, globalparamsV);	
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, globalcapsule9, globalposition, globalparamsV, collections9);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, globalcapsule10, globalposition, globalparamsV, collections10);		
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer_source11, globalcapsule11, globalposition, globalparamsV, collections11);		
	
						#else 
						TOPP2_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP2_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		MERGEP2_mergeVs(kvdram0, vdram); 
	
		MERGEP2_mergeVs(kvdram1, vdram); 
	
		MERGEP2_mergeVs(kvdram2, vdram); 
	
		MERGEP2_mergeVs(kvdram3, vdram); 
	
		MERGEP2_mergeVs(kvdram4, vdram); 
	
		MERGEP2_mergeVs(kvdram5, vdram); 
	
		MERGEP2_mergeVs(kvdram6, vdram); 
	
		MERGEP2_mergeVs(kvdram7, vdram); 
	
		MERGEP2_mergeVs(kvdram8, vdram); 
	
		MERGEP2_mergeVs(kvdram9, vdram); 
	
		MERGEP2_mergeVs(kvdram10, vdram); 
	
		MERGEP2_mergeVs(kvdram11, vdram); 
	}
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
	

	

