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
unsigned int acts_all::UTILS_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
unsigned int acts_all::UTILS_amax(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int acts_all::UTILS_aplus(unsigned int val1, unsigned int val2){
	return val1 + val2;
}
uint32_type acts_all::UTILS_amin2(uint32_type val1, uint32_type val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

// allignment
batch_type acts_all::UTILS_allignlower_KV(batch_type val){
	batch_type fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type acts_all::UTILS_allignhigher_KV(batch_type val){
	batch_type fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
batch_type acts_all::UTILS_allignlower_KV2(batch_type val){
	batch_type fac = val / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type acts_all::UTILS_allignhigher_KV2(batch_type val){
	batch_type fac = (val + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE;
	return (fac * VECTOR2_SIZE);
}
batch_type acts_all::UTILS_allignhigher_FACTOR(batch_type val, unsigned int _FACTOR){
	#pragma HLS INLINE
	batch_type fac = (val + (_FACTOR - 1)) / _FACTOR;
	return (fac * _FACTOR);
}
batch_type acts_all::UTILS_allignlower_FACTOR(batch_type val, unsigned int _FACTOR){
	batch_type fac = val / _FACTOR;
	return (fac * _FACTOR);
}

unsigned int acts_all::UTILS_GETNUMPARTITIONS_FIRSTSWEEP_NONRECURSIVEMODE(unsigned int tree_depth){
	return (1 << (OPT_NUM_PARTITIONS_POW * (tree_depth-1)));
}

// bit manipulation
unsigned int acts_all::UTILS_GETMASK_UINT(uint32_type index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int acts_all::UTILS_READFROM_UINT(uint32_type data, unsigned int index, unsigned int size){ 
	unsigned int res = 0;
	res = (((data) & UTILS_GETMASK_UINT((index), (size))) >> (index)); 
	return res;
}
void acts_all::UTILS_WRITETO_UINT(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
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
unsigned int acts_all::UTILS_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	#ifdef _WIDEWORD
	res = data.range(index + size - 1, index); 
	#else
	res = UTILS_READFROM_UINT(data, index, size);
	#endif
	return res;
}
void acts_all::UTILS_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	data->range(index + size - 1, index) = value;
	#else
	UTILS_WRITETO_UINT(data, index, size, value);
	#endif
	return; 
}
unsigned int acts_all::UTILS_SWREADBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	res = UTILS_READFROM_UINT(data, index, size);
	return res;
}
void acts_all::UTILS_SWWRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	UTILS_WRITETO_UINT(data, index, size, value);
	return; 
}

// converters
keyvalue_t acts_all::UTILS_GETKV(keyvalue_buffer_t data){
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
keyvalue_buffer_t acts_all::UTILS_GETKV(keyvalue_t data){
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
keyy_t acts_all::UTILS_GETK(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_KEY - 1, 0);
	#else
	return data;
	#endif
}
value_t acts_all::UTILS_GETV(uint32_type data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD_FOR_KV
	return data.range(SIZEOF_VALUE - 1, 0);
	#else
	return data;
	#endif
}
keyy_t acts_all::UTILS_GETKEYENTRY(uint512_dt data, unsigned int v){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return data.range(32 * ((v * 2) + 1) - 1, (v * 2) * 32);
	#else 
	return data.data[v].key;	
	#endif
}		
unsigned int acts_all::UTILS_GETLOCALVID(unsigned int vid, unsigned int instid){ 
	#pragma HLS INLINE
	
	#ifdef CONFIG_EDGEHASHSCHEME_SETVIDS
	unsigned int W = EDGEDATA_PACKINGSIZE * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % EDGEDATA_PACKINGSIZE;
	unsigned int lvid = (y * EDGEDATA_PACKINGSIZE) + x;
	return lvid;
	#endif 
	
	#ifdef CONFIG_EDGEHASHSCHEME_SINGLEVID
	return (vid - instid) / NUM_PEs;
	#endif 
}
unsigned int acts_all::UTILS_GETREALVID(unsigned int lvid, unsigned int instid){ 
	#pragma HLS INLINE
	
	#ifdef CONFIG_EDGEHASHSCHEME_SETVIDS
	unsigned int W = EDGEDATA_PACKINGSIZE * NUM_PEs;
	unsigned int y2 = lvid / EDGEDATA_PACKINGSIZE;
	unsigned int x2 = lvid % EDGEDATA_PACKINGSIZE;		
	unsigned int vid = (y2 * W) + (instid * EDGEDATA_PACKINGSIZE) + x2;
	return vid;
	#endif 
	
	#ifdef CONFIG_EDGEHASHSCHEME_SINGLEVID
	return (lvid * NUM_PEs) + instid;
	#endif 
}
unsigned int acts_all::UTILS_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparams_t globalparams){ 
	#pragma HLS INLINE
	return globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	// return PROCESSPARTITIONSZ_KVS2;
}

unsigned int acts_all::UTILS_GetFirstData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(31, 0);
	#else 
	return kvdram[offset_kvs].data[0].key;
	#endif 
}
void acts_all::UTILS_SetFirstData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(31, 0) = data;
	#else 
	kvdram[offset_kvs].data[0].key = data;
	#endif 
	return;
}
void acts_all::UTILS_SetFirstDatas(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int * data, unsigned int size){
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
unsigned int acts_all::UTILS_GetSecondData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(63, 32);
	#else 
	return kvdram[offset_kvs].data[0].value;
	#endif 
}
void acts_all::UTILS_SetSecondData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(63, 32) = data;
	#else 
	kvdram[offset_kvs].data[0].value = data;
	#endif 
	return;
}
unsigned int acts_all::UTILS_GetThirdData(uint512_dt * kvdram, unsigned int offset_kvs){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	return kvdram[offset_kvs].range(95, 64);
	#else 
	return kvdram[offset_kvs].data[1].key;
	#endif 
}
void acts_all::UTILS_SetThirdData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int data){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	kvdram[offset_kvs].range(95, 64) = data;
	#else 
	kvdram[offset_kvs].data[1].key = data;
	#endif 
	return;
}
	
void acts_all::UTILS_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[EDGEDATA_PACKINGSIZE]){  
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
void acts_all::UTILS_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){  
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
uint512_evec_dt acts_all::UTILS_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs){  
	#pragma HLS INLINE
	uint512_evec_dt dataset;
	
	#ifdef _WIDEWORD 
	dataset.data[0] = kvdram[offset_kvs].range(31, 0); 
	dataset.data[1] = kvdram[offset_kvs].range(63, 32); 
	dataset.data[2] = kvdram[offset_kvs].range(95, 64); 
	dataset.data[3] = kvdram[offset_kvs].range(127, 96); 
	dataset.data[4] = kvdram[offset_kvs].range(159, 128); 
	dataset.data[5] = kvdram[offset_kvs].range(191, 160); 
	dataset.data[6] = kvdram[offset_kvs].range(223, 192); 
	dataset.data[7] = kvdram[offset_kvs].range(255, 224); 
	dataset.data[8] = kvdram[offset_kvs].range(287, 256); 
	dataset.data[9] = kvdram[offset_kvs].range(319, 288); 
	dataset.data[10] = kvdram[offset_kvs].range(351, 320); 
	dataset.data[11] = kvdram[offset_kvs].range(383, 352); 
	dataset.data[12] = kvdram[offset_kvs].range(415, 384); 
	dataset.data[13] = kvdram[offset_kvs].range(447, 416); 
	dataset.data[14] = kvdram[offset_kvs].range(479, 448); 
	dataset.data[15] = kvdram[offset_kvs].range(511, 480); 
	#else 
	dataset.data[0] = kvdram[offset_kvs].data[0].key;
	dataset.data[1] = kvdram[offset_kvs].data[0].value; 
	dataset.data[2] = kvdram[offset_kvs].data[1].key;
	dataset.data[3] = kvdram[offset_kvs].data[1].value; 
	dataset.data[4] = kvdram[offset_kvs].data[2].key;
	dataset.data[5] = kvdram[offset_kvs].data[2].value; 
	dataset.data[6] = kvdram[offset_kvs].data[3].key;
	dataset.data[7] = kvdram[offset_kvs].data[3].value; 
	dataset.data[8] = kvdram[offset_kvs].data[4].key;
	dataset.data[9] = kvdram[offset_kvs].data[4].value; 
	dataset.data[10] = kvdram[offset_kvs].data[5].key;
	dataset.data[11] = kvdram[offset_kvs].data[5].value; 
	dataset.data[12] = kvdram[offset_kvs].data[6].key;
	dataset.data[13] = kvdram[offset_kvs].data[6].value; 
	dataset.data[14] = kvdram[offset_kvs].data[7].key;
	dataset.data[15] = kvdram[offset_kvs].data[7].value; 
	#endif
	return dataset;
}	

void acts_all::UTILS_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
void acts_all::UTILS_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
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
void acts_all::UTILS_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
void acts_all::UTILS_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
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
void acts_all::UTILS_WriteDataset(uint512_dt * kvdram, unsigned int offset_kvs, uint512_uvec_dt dataset){ // *** FIXME: adjust for EDGEDATA_PACKINGSIZE & UPDATEDATA_PACKINGSIZE
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
unsigned int acts_all::UTILS_ReadData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index){
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
void acts_all::UTILS_WriteData(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data){
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
keyvalue_t acts_all::UTILS_ReadKV(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index){
	#pragma HLS INLINE
	keyvalue_t data;
	#ifdef _WIDEWORD
	unsigned int row = index / VECTOR_SIZE;
	unsigned int col = index % VECTOR_SIZE;
 if(col == 0){
		data.key = kvdram[offset_kvs + row].range(31, 0); 
		data.value = kvdram[offset_kvs + row].range(63, 32); 
	}
else if(col == 1){
		data.key = kvdram[offset_kvs + row].range(95, 64); 
		data.value = kvdram[offset_kvs + row].range(127, 96); 
	}
else if(col == 2){
		data.key = kvdram[offset_kvs + row].range(159, 128); 
		data.value = kvdram[offset_kvs + row].range(191, 160); 
	}
else if(col == 3){
		data.key = kvdram[offset_kvs + row].range(223, 192); 
		data.value = kvdram[offset_kvs + row].range(255, 224); 
	}
else if(col == 4){
		data.key = kvdram[offset_kvs + row].range(287, 256); 
		data.value = kvdram[offset_kvs + row].range(319, 288); 
	}
else if(col == 5){
		data.key = kvdram[offset_kvs + row].range(351, 320); 
		data.value = kvdram[offset_kvs + row].range(383, 352); 
	}
else if(col == 6){
		data.key = kvdram[offset_kvs + row].range(415, 384); 
		data.value = kvdram[offset_kvs + row].range(447, 416); 
	}
else if(col == 7){
		data.key = kvdram[offset_kvs + row].range(479, 448); 
		data.value = kvdram[offset_kvs + row].range(511, 480); 
	}
	else {} 
	#else 
	data = kvdram[offset_kvs + (index / VECTOR_SIZE)].data[index % VECTOR_SIZE];
	#endif 
	return data;
}

bool acts_all::UTILS__processit__i_am_first__(globalposition_t globalposition){
	#pragma HLS INLINE
	bool __i_am_first__ = false; 
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ __i_am_first__ = true;; } else { __i_am_first__ = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS4
	// if(__i_am_first__ == true){ cout<<"--------------------------------actsutil: __i_am_first__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	#endif
	return __i_am_first__;
}
bool acts_all::UTILS__processit__i_am_last__(globalposition_t globalposition){
	#pragma HLS INLINE
	bool __i_am_last__ = false;
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ __i_am_last__ = true; } else { __i_am_last__ = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS4
	// if(__i_am_last__ == true){ cout<<"-------------------------------- actsutil: __i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	#endif
	return __i_am_last__;
}

// utilities
batch_type acts_all::UTILS_getskipsize(step_type currentLOP, bool_type sourceORdest, globalparams_t globalparams){
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
batch_type acts_all::UTILS_getrangeforeachllop(globalparams_t globalparams){
	unsigned int range = globalparams.SIZE_BATCHRANGE;
	for(unsigned int i=0; i<globalparams.ACTSPARAMS_TREEDEPTH; i++){
		range = range / OPT_NUM_PARTITIONS;
	}
	return range;
}
buffer_type acts_all::UTILS_getchunksize_kvs(buffer_type buffer_size, travstate_t travstate, buffer_type localoffset){
	buffer_type chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ chunk_size = travstate.end_kvs - i; }
	else {}
	return chunk_size;
}
buffer_type acts_all::UTILS_getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	buffer_type size_kvs = 0;
	size_kvs = realsize_kvs;
	return size_kvs;
}
void acts_all::UTILS_calculateoffsets(keyvalue_capsule_t * buffer, buffer_type size){
	unsigned int analysis_size = OPT_NUM_PARTITIONS;
	for(buffer_type i=1; i<size; i++){ 
	#pragma HLS PIPELINE II=2
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_size avg=analysis_size	
		buffer[i].key = UTILS_allignhigher_KV(buffer[i-1].key + buffer[i-1].value); 
	}
	return;
}
void acts_all::UTILS_calculatemanyunallignedoffsets(keyvalue_capsule_t buffer[VECTOR_SIZE][MAX_NUM_PARTITIONS], buffer_type size, batch_type base, batch_type skipspacing){
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
batch_type acts_all::UTILS_get_num_source_partitions(step_type currentLOP){
	if(currentLOP == 0){ currentLOP = 1; }
	batch_type pow = 1;
	for(step_type i=0; i<(currentLOP-1); i++){
		pow = pow * OPT_NUM_PARTITIONS;
	}
	return pow;
}
globalparams_t acts_all::UTILS_getglobalparams(uint512_dt * kvdram, unsigned int banksection){
	globalparams_t globalparams;
	
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("UTILS_getglobalparams 23", 0 + MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS, (((1 << 28) / 4) / 16), globalparams.DRAM_BASE_KVS, MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp);
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
		cout<<"UTILS_getglobalparams: NOT IMPLEMENTED. EXITING..."<<endl;
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
	globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION = buffer[MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION];
	globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION = buffer[MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION];
	globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION = buffer[MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION];
	
	globalparams.BASEOFFSETKVS_MESSAGESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_MESSAGESDATA];
	globalparams.VOLUMEOFFSETKVS_WORKDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_VOLUMEOFFSETKVS_WORKDATA];
	globalparams.VOLUMEOFFSETKVS_WORKSPACEDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_VOLUMEOFFSETKVS_WORKSPACEDATA];
	globalparams.BASEOFFSETKVS_EDGESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGESDATA];
	globalparams.BASEOFFSETKVS_VERTEXPTR = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_VERTEXPTR];
	globalparams.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA];
	globalparams.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA];
	globalparams.BASEOFFSETKVS_ACTIVEEDGEBLOCKS = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_ACTIVEEDGEBLOCKS];
	globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_ACTIVEUPDATEBLOCKS];
	globalparams.BASEOFFSETKVS_EDGESMAP = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGESMAP];
	globalparams.BASEOFFSETKVS_EDGEBLOCKMAP = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGEBLOCKMAP];
	globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK];
	globalparams.BASEOFFSETKVS_STATSDRAM = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_STATSDRAM];
	globalparams.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGESSTATSDRAM]; //
	globalparams.BASEOFFSETKVS_VERTEXUPDATESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_VERTEXUPDATESDATA];
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
	globalparams.SIZE_ACTIVEEDGEBLOCKS = buffer[MESSAGES_SIZE_ACTIVEEDGEBLOCKS];
	globalparams.SIZE_ACTIVEUPDATEBLOCKS = buffer[MESSAGES_SIZE_ACTIVEUPDATEBLOCKS];
	globalparams.SIZE_EDGESMAP = buffer[MESSAGES_SIZE_EDGESMAP];
	globalparams.SIZE_EDGEBLOCKMAP = buffer[MESSAGES_SIZE_EDGEBLOCKMAP];
	globalparams.SIZE_VERTICESPARTITIONMASK = buffer[MESSAGES_SIZE_VERTICESPARTITIONMASK];
	globalparams.SIZE_KVSTATSDRAM = buffer[MESSAGES_SIZE_KVSTATSDRAM];
	globalparams.SIZE_EDGESSTATSDRAM = buffer[MESSAGES_SIZE_EDGESSTATSDRAM];
	globalparams.SIZE_VERTEXUPDATESDATA = buffer[MESSAGES_SIZE_VERTEXUPDATESDATA];
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
	globalparams.POW_ACTIVEEDGEBLOCKS = buffer[MESSAGES_POW_ACTIVEEDGEBLOCKS];
	globalparams.POW_VERTICESDATAMASK = buffer[MESSAGES_POW_VERTICESDATAMASK];
	globalparams.POW_KVSTATSDRAM = buffer[MESSAGES_POW_KVSTATSDRAM];
	globalparams.POW_EDGESSTATSDRAM = buffer[MESSAGES_POW_EDGESSTATSDRAM];
	globalparams.POW_VERTEXUPDATESDATA = buffer[MESSAGES_POW_VERTEXUPDATESDATA];
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
	globalparams.ACTSPARAMS_BATCH_RANGE = buffer[MESSAGES_ACTSPARAMS_BATCH_RANGE]; 
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
sweepparams_t acts_all::UTILS_getsweepparams(globalparams_t globalparams, step_type currentLOP, batch_type source_partition){
	sweepparams_t sweepparams;
	batch_type sourceskipsize = UTILS_getskipsize(currentLOP, SOURCE, globalparams);
	
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
travstate_t acts_all::UTILS_gettravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, step_type currentLOP, batch_type sourcestatsmarker){			
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
			cout<<"UTILS_gettravstate: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<"). EXITING..."<<endl;
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
void acts_all::UTILS_settravstate(bool_type enable, uint512_dt * kvdram, globalparams_t globalparams, batch_type offset, unsigned int value){			
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

partition_type acts_all::UTILS_getpartition(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, unsigned int tree_depth){
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
partition_type acts_all::UTILS_getpartition2(bool_type enable, unsigned int mode, keyvalue_buffer_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int upperpartition, unsigned int batch_range_pow, globalparams_t globalparams){
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
unsigned int acts_all::UTILS_getstatsoffset(unsigned int LOP, globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	if(globalparams.ACTSPARAMS_TREEDEPTH > 1){ for(unsigned int k=0; k<LOP; k++){ _offset += (1 << (globalparams.ACTSPARAMS_POW_PARTITIONS * k)); }} else { _offset = 1; }
	return _offset;
}

// resets 
void acts_all::UTILS_resetvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void acts_all::UTILS_resetvalues(keyvalue_capsule_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i].value = resetval; 
	}
}
void acts_all::UTILS_resetvalues(value_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){ 
	#pragma HLS PIPELINE II=1
		buffer[i] = resetval; 
	}
}
void acts_all::UTILS_resetkeysandvalues(keyvalue_t * buffer, buffer_type size, unsigned int resetval){
	for(buffer_type i=0; i<size; i++){
		buffer[i].key = resetval; 
		buffer[i].value = resetval; 
	}
	return;
}
void acts_all::UTILS_resetkvstatvalues(uint512_dt * kvdram, globalparams_t globalparams){
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
void acts_all::UTILS_resetkvstatvalues(uint512_dt * kvdram, unsigned int size_kvs, globalparams_t globalparams){
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
void acts_all::UTILS_accumkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
			cout<<"UTILS_accumkvstats: NOT IMPLEMENTED. (globalparams.VARS_WORKBATCH: "<<globalparams.VARS_WORKBATCH<<") EXITING..."<<endl;
			exit(EXIT_FAILURE);
			#endif 
		}
	}
	return;
}
void acts_all::UTILS_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _WIDEWORD
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#else
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	return;
}
void acts_all::UTILS_resetenvbuffers(keyvalue_capsule_t capsule_so1[VECTOR_SIZE][MAX_NUM_PARTITIONS], keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
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
void acts_all::UTILS_resetenvbuffer(keyvalue_capsule_t capsule_so8[MAX_NUM_PARTITIONS]){
	for(partition_type p=0; p<OPT_NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule_so8[p].key = 0;
		capsule_so8[p].value = 0;
	}
	return;
}

// checks 
bool acts_all::UTILS_isbufferused(unsigned int id){
	if(id==0 || id==NUMCOMPUTEUNITS_SLR2 || id==NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1){ return true; } else { return false; } ///////////////
	return true;
}
void acts_all::UTILS_check_capsule(keyvalue_t capsule[MAX_NUM_PARTITIONS], unsigned int num_partitions, unsigned int max_value){
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
void acts_all::MEMACCESSS_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){
	if(enable == OFF){ return; }

	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas2 complete
	
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		UTILS_ReadDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
	
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
			value_t vdata = datas[v] >> 1; value_t mask = datas[v] & 0x1;
			if(vdata < 1000){ cout<<"readV: ACTIVE VDATA SEEN: @ i: "<<i<<" v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<endl; }}
		#endif
		
		// clear masks
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
		#pragma HLS UNROLL
			datas2[v] = datas[v] & 0xFFFFFFFE; 
			// datas2[v] = datas[v]; // CRITICAL REMOVEME.
		}
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){ datas2[v] = datas[v]; }
		
 vdata[0].data = datas2[0];  vdata[1].data = datas2[1];  vdata[2].data = datas2[2];  vdata[3].data = datas2[3];  vdata[4].data = datas2[4];  vdata[5].data = datas2[5];  vdata[6].data = datas2[6];  vdata[7].data = datas2[7];  vdata[8].data = datas2[8];  vdata[9].data = datas2[9];  vdata[10].data = datas2[10];  vdata[11].data = datas2[11];  vdata[12].data = datas2[12];  vdata[13].data = datas2[13];  vdata[14].data = datas2[14];  vdata[15].data = datas2[15]; 	
		
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
	}
	return;
}

void acts_all::MEMACCESSS_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){				
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
		
 datas[0] = vdata[0].data;  datas[1] = vdata[1].data;  datas[2] = vdata[2].data;  datas[3] = vdata[3].data;  datas[4] = vdata[4].data;  datas[5] = vdata[5].data;  datas[6] = vdata[6].data;  datas[7] = vdata[7].data;  datas[8] = vdata[8].data;  datas[9] = vdata[9].data;  datas[10] = vdata[10].data;  datas[11] = vdata[11].data;  datas[12] = vdata[12].data;  datas[13] = vdata[13].data;  datas[14] = vdata[14].data;  datas[15] = vdata[15].data; 	
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
			value_t vdata = datas[v] >> 1; value_t mask = datas[v] & 0x1;
			if(vdata < 1000){ cout<<"saveV: ACTIVE VDATA SEEN: @ i: "<<i<<" v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<endl; }}
		#endif
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSS_saveV 23", baseoffset_kvs + offset_kvs + i, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		UTILS_WriteDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
	}
	return;
}

void acts_all::MEMACCESSS_readANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks1 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks2 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks3 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks4 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks5 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks6 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks7 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks8 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks9 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks10 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks11 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
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
		cout<<"MEMACCESSS_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILS_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSS_readANDRVchunks12 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
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
void acts_all::MEMACCESSS_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS + 1, NAp, NAp, NAp);
	#endif
	
	keyvalue_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<1 + globalparams.NUM_REDUCEPARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt vec;
		
		// UTILS_ReadDatas(kvdram, offset_kvs + i, datas); // NEWCHANGE.
		// globalstatsbuffer[i] = datas[globalparams.VARS_WORKBATCH];
		
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS//4
	actsutilityobj->printkeyvalues("mem_access:: readglobalstats.globalstatsbuffer", globalstatsbuffer, 1 + globalparams.NUM_REDUCEPARTITIONS); 
	#endif
	#ifdef _DEBUGMODE_CHECKS3
	for (buffer_type i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ actsutilityobj->checkoutofbounds("_readglobalstats 23", globalstatsbuffer[i].key + globalstatsbuffer[i].value, globalparams.SIZE_KVDRAM, i, globalparams.ALGORITHMINFO_GRAPHITERATIONID, NAp); }
	#endif
	// exit(EXIT_SUCCESS); 
	return;
}

void acts_all::MEMACCESSS_commitkvstats(uint512_dt * kvdram, value_t * buffer, globalparams_t globalparams){
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
void acts_all::MEMACCESSS_readhelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		UTILS_ReadDatas(vdram, offset_kvs + i, datas);
		
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

void acts_all::MEMACCESSS_savehelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		datas[GraphIter] = pmask[i];
		UTILS_WriteDatas(vdram, offset_kvs + i, datas);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
	}
	return;
}









	
#endif 
#ifdef CONFIG_ENABLECLASS_ACTS_MERGE
void acts_all::mergeS_trace(uint512_dt * kvdram, unsigned int reduce_partition, unsigned int k, unsigned int offset_kvs, globalparams_t globalparams){
	for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
		value_t combo; if(v%2==0){ combo = kvdram[offset_kvs].data[v/2].key; } else { combo = kvdram[offset_kvs].data[v/2].key; }
		value_t vdata = combo >> 1; value_t mask = combo & 0x1;
		unsigned int lvid = (((reduce_partition * globalparams.SIZEKVS2_REDUCEPARTITION) + k) * VDATA_PACKINGSIZE) + v;
		#ifdef _DEBUGMODE_KERNELPRINTS4
		if(mask == 1){ cout<<">>> MERGE:: MERGE VERTICES SEEN @ reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", vid: "<<UTILS_GETREALVID(lvid, globalparams.ACTSPARAMS_INSTID)<<", combo: "<<combo<<", globalparams.SIZE_DESTVERTICESDATA: "<<globalparams.SIZE_DESTVERTICESDATA<<endl; }	
		#endif 
	}
}

void acts_all::MERGES_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR_SIZE; v++){	
		unsigned int K = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].key;
		unsigned int V = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].value;
		// if(MEMCAS_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }
		// if(MEMCAS_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }	
	}
	#endif 
	return;
}

unsigned int acts_all::mergeS_get_updateblock_workload(unsigned int en, unsigned int reduce_partition, unsigned int * stats_offsets, unsigned int * stats_metadata, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE]){
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	workload_t workload_kvs;
	
	bool sparse_v = false; if(stats_metadata[reduce_partition+1] - stats_metadata[reduce_partition] < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION){ sparse_v = true; } else { sparse_v = false; }
	if(en == false || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_v = false; }
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparams.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; // 16
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock; // 512
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(sparse_v == true){ cout<<"merge:: sparse_sync is TRUE. stats[inst:"<<globalparams.ACTSPARAMS_INSTID<<"][1]["<<reduce_partition<<"]: "<<stats_metadata[reduce_partition]<<"->"<<stats_metadata[reduce_partition+1]<<endl; } else { cout<<"merge:: sparse_sync is FALSE. stats[inst:"<<globalparams.ACTSPARAMS_INSTID<<"][1]["<<reduce_partition<<"]: "<<stats_metadata[reduce_partition]<<"->"<<stats_metadata[reduce_partition+1]<<endl; }					
	#endif 
	unsigned int num_its; if(sparse_v == true){ num_its = stats_metadata[reduce_partition+1] - stats_metadata[reduce_partition]; } else { num_its = 1; }
	#ifdef _DEBUGMODE_CHECKS3
	if((stats_metadata[reduce_partition+1] < stats_metadata[reduce_partition]) || (num_its > 600)){ cout<<"_get_updateblock_workload. ERROR 23. error somewhere. EXIITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	for(unsigned int n=0; n<num_its; n++){
		if(sparse_v == true){
			unsigned int updateblock_id = stats_offsets[stats_metadata[reduce_partition] + n]; 
			workload_kvs.offset_begin = updateblock_id * numkvs_vertices_per_updateblock; 
			workload_kvs.size = numkvs_vertices_per_updateblock;  
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			xload_kvs[n] = workload_kvs;
			unsigned int buffer_offset = updateblock_id * numkvs_vertices_per_updateblock;
			buffer_offsets[n] = buffer_offset;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_updateblock_workload(112)::DEBUG CODE 112::1", buffer_offset, MAX_BLOCKRAM_VDESTDATA_SIZE, updateblock_id, numkvs_vertices_per_updateblock, numkvs_updateblocks_per_reducepartition); 
			#endif	
		} else {
			workload_kvs.offset_begin = 0;
			workload_kvs.size = globalparams.SIZEKVS2_REDUCEPARTITION;
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			xload_kvs[n] = workload_kvs;
			unsigned int buffer_offset = 0;
			buffer_offsets[n] = buffer_offset;
		}
	}
	return num_its;
}

unsigned int acts_all::mergeS_get_upropblock_workload(unsigned int en, unsigned int process_partition, uint512_dt * dram, unsigned int num_active_edgeblocks, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE], unsigned int graphiterationid){
	#pragma HLS INLINE
	
	bool sparse_readu = false; if(num_active_edgeblocks < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
	if(en == false || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = false; }
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	unsigned int edgeblockoffset_vs = (process_partition * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;	
	#else  
	unsigned int edgeblockoffset_vs = ((process_partition * MAXNUMGRAPHITERATIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) + (graphiterationid * MAXSZ_EDGEBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
	#endif 
	unsigned int num_its; if(sparse_readu == true){ num_its = num_active_edgeblocks; } else { num_its = 1; }
	workload_t workload_kvs;
	unsigned int buffer_offset = 0;
	
	for(unsigned int n=0; n<num_its; n++){
		if(sparse_readu == true){
			#ifdef _COLLECT_METADATA_FROM_KERNEL_
			unsigned int edgeblock_id = UTILS_ReadData(dram, globalparams.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + edgeblockoffset_vs, 1 + n);
			#else  
			unsigned int edgeblock_id = UTILS_ReadData(dram, globalparams.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + edgeblockoffset_vs, n);
			#endif 
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21", edgeblock_id, MAXSZ_EDGEBLOCKS_PER_VPARTITION, process_partition, n, num_active_edgeblocks);
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21", edgeblock_id, MAXSZ_EDGEBLOCKS_PER_VPARTITION, process_partition, n, globalparams.ALGORITHMINFO_GRAPHITERATIONID);
			#endif
			// cout<<"------------------------ _get_upropblock_workload: edgeblock_id: "<<edgeblock_id<<", globalparams.ALGORITHMINFO_GRAPHITERATIONID: "<<globalparams.ALGORITHMINFO_GRAPHITERATIONID<<", n: "<<n<<endl;
			workload_kvs.offset_begin = 0;
			workload_kvs.size = 1; 
			buffer_offset = edgeblock_id * (NUM_VERTICES_PER_EDGEBLOCK / VDATA_PACKINGSIZE);
		} else {
			workload_kvs.offset_begin = 0;
			workload_kvs.size = globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
			buffer_offset = 0;
		}
		workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
		xload_kvs[n] = workload_kvs;
		buffer_offsets[n] = buffer_offset;
	}
	// exit(EXIT_SUCCESS);
	return num_its;
}

unsigned int acts_all::mergeS_get_edgeblock_workload(unsigned int en, unsigned int process_partition, unsigned int llp_set, uint512_dt * dram, unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE, workload_t xload_kvs[DOUBLE_BLOCKRAM_SIZE], unsigned int graphiterationid){
	
	keyvalue_t datas[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	
	bool sparse_process = false; if(num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	if(en == false || globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = false; }
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	unsigned int edgeblockoffset_vs = (process_partition * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;	// NEWCHANGE.
	#else  
	unsigned int edgeblockoffset_vs = ((process_partition * MAXNUMGRAPHITERATIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) + (graphiterationid * MAXSZ_EDGEBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
	#endif 
	// unsigned int edgeblockmapoffset_kvs = (process_partition * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR_SIZE;
	unsigned int edgeblockmapoffset_kvs = process_partition * MAXSZ_EDGEBLOCKS_PER_VPARTITION; // NEWCHANGE
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	unsigned int num_its; if(sparse_process == true){ num_its = num_active_edgeblocks; } else { num_its = 1; }
	
	READ_PROCESS_PARTITION_WRITE_GETWORKLOADSTATS_LOOP: for(unsigned int n=0; n<num_its; n++){
		workload_t workload_kvs;					
		if(sparse_process == true){
			#ifdef _COLLECT_METADATA_FROM_KERNEL_
			unsigned int edgeblock_id = UTILS_ReadData(dram, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + edgeblockoffset_vs, 1 + n);
			#else  
			unsigned int edgeblock_id = UTILS_ReadData(dram, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + edgeblockoffset_vs, n);
			#endif
			UTILP0_ReadDatas(dram, globalparamsE.BASEOFFSETKVS_EDGEBLOCKMAP + edgeblockmapoffset_kvs + edgeblock_id, datas);
			workload_kvs.offset_begin = datas[llp_set].key; workload_kvs.size = datas[llp_set].value; 
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("actit(1)", edgeblock_id, MAXSZ_EDGEBLOCKS_PER_VPARTITION, NAp, NAp, n);
			actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.offset_begin, (1 << 28) / 8, NAp, NAp, n);
			actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.size, (1 << 28) / 8, NAp, NAp, n);
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"actit/sparse_process:: active edge-block seen @ ["<<globalparamsK.ACTSPARAMS_INSTID<<"]["<<process_partition<<"]["<<llp_set<<"]: edgeblock-id: "<<edgeblock_id<<", edgeblock-map: "<<"["<<workload_kvs.offset_begin<<", "<<workload_kvs.size<<"]"<<endl;
			#endif 
		} else {
			unsigned int index = (process_partition * num_LLPset) + llp_set;
			workload_kvs.offset_begin = UTILS_ReadData(dram, globalparamsE.BASEOFFSETKVS_EDGESMAP, index) / EDGEDATA_PACKINGSIZE;
			workload_kvs.offset_end = UTILS_ReadData(dram, globalparamsE.BASEOFFSETKVS_EDGESMAP, index + 1) / EDGEDATA_PACKINGSIZE;
			workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"actit/dense_process:: full edge-block seen @ ["<<globalparamsK.ACTSPARAMS_INSTID<<"]["<<process_partition<<"]: edgeblock-id: NAp, edgeblock-map: "<<"["<<workload_kvs.offset_begin<<", "<<workload_kvs.size<<"]"<<endl;
			#endif 
		}
	
		if(workload_kvs.offset_end < workload_kvs.offset_begin){ workload_kvs.size = 0; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<"actit:: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<", workload_kvs.offset_end: "<<workload_kvs.offset_end<<", workload_kvs.size: "<<workload_kvs.size<<"] "<<endl;
		#endif
		xload_kvs[n] = workload_kvs;
	}
	return num_its;
}

void acts_all::MERGES_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> mergeVs:: merging vertices..."<< RESET <<endl; 
	#endif
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	unsigned int xblock_ids[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	workload_t workload_uprop_kvs;
	unsigned int total_vertices_merged = 0;
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	unsigned int stats[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats
	
	globalparams_t globalparams = UTILS_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILS_getglobalparams(vdram, 0);
	
	#ifdef TESTKERNEL 
	if(UTILS_isbufferused(globalparams.ACTSPARAMS_INSTID) == false){ return; }
	#endif 
	
	unsigned int voffset_kvs2 = globalparams.ACTSPARAMS_INSTID * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffseti_kvs2 = 0;
	
	if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
		for(unsigned int t=0; t<16; t++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD 
			stats[1][t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + t].range(63, 32);
			#else 
			stats[1][t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + t].data[0].value;
			#endif 
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int i=0; i<4; i++){ cout<<"merge:: stats[0]["<<i<<"]: "<<stats[0][i]<<", "<<endl; }
	for(unsigned int i=0; i<4; i++){ cout<<"merge:: stats[1]["<<i<<"]: "<<stats[1][i]<<", "<<endl; }
	#endif
	
	MERGES_MERGEVSLOOP2: for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		
		unsigned int num_its = 1; xload_kvs[0].offset_begin = 0; xload_kvs[0].size = globalparams.SIZEKVS2_REDUCEPARTITION;
		if(globalparams.NUM_REDUCEPARTITIONS > 1){ if(reduce_partition == globalparams.NUM_REDUCEPARTITIONS-1){ xload_kvs[0].size = (globalparams.ACTSPARAMS_BATCH_RANGE / VECTOR2_SIZE) % globalparams.SIZEKVS2_REDUCEPARTITION; } else { xload_kvs[0].size = globalparams.SIZEKVS2_REDUCEPARTITION; }}
			
		if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
			unsigned int num_update_blocks = stats[1][reduce_partition+1] - stats[1][reduce_partition];
			bool sparse_merge = false; if(num_update_blocks < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION){ sparse_merge = true; } else { sparse_merge = false; }
			if(sparse_merge == true){ 
				unsigned int offset_buff = stats[1][reduce_partition];
				for(unsigned int t=0; t<num_update_blocks; t++){ 
				#pragma HLS PIPELINE II=1
					#ifdef _WIDEWORD 
					stats[0][offset_buff + t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + offset_buff + t].range(31, 0)
					#else 
					stats[0][offset_buff + t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + offset_buff + t].data[0].key;
					#endif 
				}
			}
			// check this. =false works, =true not work?
			num_its = mergeS_get_updateblock_workload(true, reduce_partition, stats[0], stats[1], globalparams, xload_kvs, buffer_offsets); 
		}
		
		MERGES_MERGEVSLOOP2B: for(unsigned int n=0; n<num_its; n++){
			MERGES_MERGEVSLOOP2C: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].offset_begin + xload_kvs[n].size; k++){ // globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, globalparams.SIZEKVS2_REDUCEPARTITION
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("mergeVs: ERROR 21", voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, NAp, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("mergeVs: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); 
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				mergeS_trace(kvdram, reduce_partition, k, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k, globalparams); total_vertices_merged += VDATA_PACKINGSIZE;
				#endif
				#ifdef _DEBUGMODE_STATS
				kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparams.ALGORITHMINFO_GRAPHITERATIONID - 1].data[1].key += UPDATEDATA_PACKINGSIZE;
				#endif 
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"mergeVs:: merge operation finished. total_vertices_merged: "<<total_vertices_merged<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}

void acts_all::MERGES_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram, unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs){
	MERGES_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){
	#pragma HLS PIPELINE II=1
		kvdram[offset2_kvs + k] = vdram[offset1_kvs + k];
	}
	return;
}

void acts_all::MERGES_broadcastVs1(uint512_dt * vdram, uint512_dt * kvdram0, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs2(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs3(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs4(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs5(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs6(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs7(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs8(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs9(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram8, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram8, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram8, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs10(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram8, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram9, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram8, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram9, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram8, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram9, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs11(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram8, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram9, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram10, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram8, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram9, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram10, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram8, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram9, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram10, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}
void acts_all::MERGES_broadcastVs12(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, 
		unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs,
			unsigned int offset1b_kvs, unsigned int offset2b_kvs, unsigned int sizeb_kvs,
				unsigned int cmd, globalposition_t globalposition, globalparams_t globalparamsK, globalparams_t globalparamsV){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	unsigned int num_its = mergeS_get_upropblock_workload(true, globalposition.source_partition, vdram, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
	
	for(unsigned int n=0; n<num_its; n++){
		MERGES_BROADCASTVS_LOOP: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].size; k++){
		#pragma HLS PIPELINE II=1
		
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offset1_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset1_kvs, buffer_offsets[n], k);
			actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offset2_kvs + buffer_offsets[n] + k, (1 << 26) / 16, offset2_kvs, buffer_offsets[n], k);
			#endif
			
			UTILS_ReadDatas(vdram, offset1_kvs + buffer_offsets[n] + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram1, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram2, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram3, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram4, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram5, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram6, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram7, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram8, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram9, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram10, offset2_kvs + buffer_offsets[n] + k, datas);
			UTILS_WriteDatas(kvdram11, offset2_kvs + buffer_offsets[n] + k, datas);
			
			// UTILS_ReadDatas(vdram, offset1_kvs + k, datas);	// NEWCHANGE.
			// 			// UTILS_WriteDatas(kvdram0, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram1, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram2, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram3, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram4, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram5, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram6, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram7, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram8, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram9, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram10, offset2_kvs + k, datas);
			// 			// UTILS_WriteDatas(kvdram11, offset2_kvs + k, datas);
			// 			
			#ifdef _DEBUGMODE_STATS 
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsV.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	// exit(EXIT_SUCCESS); ////////////////////////////
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	if(cmd == 1){
		MERGES_BROADCASTVS_LOOP2: for(unsigned int k=0; k<sizeb_kvs; k++){
		#pragma HLS PIPELINE II=1
			
			UTILS_ReadDatas(vdram, offset1b_kvs + k, datas);	
			UTILS_WriteDatas(kvdram0, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram1, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram2, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram3, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram4, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram5, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram6, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram7, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram8, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram9, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram10, offset2b_kvs + k, datas);
			UTILS_WriteDatas(kvdram11, offset2b_kvs + k, datas);
			
			#ifdef _DEBUGMODE_STATS_XXXXXXXXXXXXXX
			kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsv.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += VDATA_PACKINGSIZE;
			#endif 
		}
	}
	#endif 
	return;
}

unsigned int acts_all::MERGES_copy(unsigned int idx, unsigned int reduce_partition, uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, unsigned int stats[2][BLOCKRAM_SIZE], unsigned int voffset_kvs, unsigned int firstinstance_id, unsigned int num_compute_units, 
		pmask_dt vpartition_stats[BLOCKRAM_CURRPMASK_SIZE], wide_word_bits_3t edgeblock_stats[MAX_RED_SRAMSZ],
		globalparams_t globalparamsSRC, globalparams_t globalparamsDST1, globalparams_t globalparamsDST2){
	unsigned int total_sync = 0;
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	value_t mask[VDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable=mask complete
	unsigned int vid[VDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable=mask complete
	
	unsigned int buffer_offset = 0;
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	unsigned int xblock_ids[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	workload_t workload_uprop_kvs;
	unsigned int total_vertices_synchd = 0;
		
	unsigned int v2offset_kvs = reduce_partition * globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
	for(unsigned int t=0; t<globalparamsSRC.SIZEKVS2_REDUCEPARTITION; t++){ edgeblock_stats[t].data[idx] = 0; } // FIXME. TOO EXPENSIVE.?
	unsigned int lvid_first = reduce_partition * globalparamsSRC.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE; // (((reduce_partition * globalparamsDST1.SIZEKVS2_REDUCEPARTITION) + k) * VDATA_PACKINGSIZE) + v;
	unsigned int vbegin = reduce_partition * globalparamsSRC.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE * NUM_PEs;

	EXCHANGES_COPY_LOOP1: for(unsigned int i=0; i<num_compute_units; i++){
		
		unsigned int reducepsz_kvs = globalparamsSRC.SIZEKVS2_REDUCEPARTITION; 
		if(globalparamsSRC.NUM_REDUCEPARTITIONS > 1){ if(reduce_partition == globalparamsSRC.NUM_REDUCEPARTITIONS-1){ reducepsz_kvs = (globalparamsSRC.ACTSPARAMS_BATCH_RANGE / VECTOR2_SIZE) % globalparamsSRC.SIZEKVS2_REDUCEPARTITION; } else { reducepsz_kvs = globalparamsSRC.SIZEKVS2_REDUCEPARTITION; }}
		// unsigned int num_its = mergeS_get_updateblock_workload(reduce_partition, stats[0], stats[1], globalparamsSRC, xload_kvs);
		
		unsigned int num_its = 1;
		xload_kvs[0].offset_begin = 0;
		xload_kvs[0].size = globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
		
		for(unsigned int n=0; n<num_its; n++){
			for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].offset_begin + xload_kvs[n].size; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 21", globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 22", globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, globalparamsSRC.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
				total_sync += VECTOR2_SIZE;
				#endif
				
				UTILS_ReadDatas(vdramSRC, globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas);
				UTILS_WriteDatas(vdramDST1, globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas); // NEWCHANGE.
				UTILS_WriteDatas(vdramDST2, globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas);
				
				for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
				#pragma HLS UNROLL 
					value_t vdata = datas[v] >> 1; mask[v] = datas[v] & 0x1;
					unsigned int lvid = lvid_first + (k * VDATA_PACKINGSIZE) + v; vid[0] = UTILS_GETREALVID(lvid, firstinstance_id + i); // FIXME. MAKE SIMPLER. // vid[0] = vbegin + (i * VDATA_PACKINGSIZE) + k;
					#ifdef _DEBUGMODE_CHECKS3
					unsigned int check_lvid = lvid_first + (k * VDATA_PACKINGSIZE) + v; unsigned int check_v0 = UTILS_GETREALVID(check_lvid, firstinstance_id + i); // FIXME. MAKE SIMPLER.
					if(vid[0] != check_v0){ cout<<"exchange. seperate calculations of vid[0] don't match. vid[0]("<<vid[0]<<") != check_v0("<<check_v0<<"). EXIITING..."<<endl; exit(EXIT_FAILURE); }
					#endif 
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					if(mask[v] == 1){ cout<<">>> EXCHANGE-COPY:: ACTIVE VERTEX SEEN @ [i: "<<i<<", reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<"] vdata: "<<vdata<<", lvid: "<<lvid<<", vid[0]: "<<vid[0]<<endl; }
					#endif 
				}	
				
				if(mask[0]==1 || mask[1]==1 || mask[2]==1 || mask[3]==1 || mask[4]==1 || mask[5]==1 || mask[6]==1 || mask[7]==1 || mask[8]==1 || mask[9]==1 || mask[10]==1 || mask[11]==1 || mask[12]==1 || mask[13]==1 || mask[14]==1 || mask[15]==1){
					if(edgeblock_stats[k].data[0] == 0 && edgeblock_stats[k].data[1] == 0 && edgeblock_stats[k].data[2] == 0){ 
						edgeblock_stats[k].data[idx] = 1; 
						unsigned int v_p = vid[0] / (globalparamsSRC.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE);
						vpartition_stats[v_p] += 1;
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						cout<<"+++ EXCHANGE-COPY:: ACTIVE EDGE-BLOCK SEEN @ edgeblock_stats["<<firstinstance_id + i<<"]["<<v_p<<"]["<<k<<"]. "<<endl; 
						#endif 
					}
				}
			} // end-of-k
		}
		v2offset_kvs += globalparamsSRC.NUM_REDUCEPARTITIONS * globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
	} // end-of-num_compute_units
	return 0;
}

void acts_all::MERGES_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> exchangeVs:: exchanging vertices across different SLRs..." << RESET <<endl; 
	#endif

	unsigned int total_sync[3]; total_sync[0]=0; total_sync[1]=0; total_sync[2]=0; 
	#pragma HLS ARRAY_PARTITION variable=total_sync complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	value_t datas1[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas1 complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas2 complete
	
	wide_word_bits_3t edgeblock_stats[MAX_RED_SRAMSZ];
	#pragma HLS DATA_PACK variable = edgeblock_stats
	pmask_dt vpartition_stats[BLOCKRAM_CURRPMASK_SIZE];
	unsigned int statsA[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsA
	unsigned int statsB[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsB
	unsigned int statsC[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsC

	globalparams_t globalparamsvA = UTILS_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILS_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILS_getglobalparams(vdramC, 0);
	
	for(unsigned int t=0; t<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; t++){ vpartition_stats[t] = 0; }
	for(unsigned int t=0; t<globalparamsvA.SIZEKVS2_REDUCEPARTITION; t++){ edgeblock_stats[t].data[0] = 0; edgeblock_stats[t].data[1] = 0; edgeblock_stats[t].data[2] = 0; }
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR1) * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	
	// unsigned int reduce_partition = 0;
	EXCHANGES_LOOP1: for(unsigned int reduce_partition=0; reduce_partition<globalparamsvA.NUM_REDUCEPARTITIONS; reduce_partition++){
		total_sync[0] = MERGES_copy(0, reduce_partition, vdramA, vdramB, vdramC, statsA, voffsetA_kvs, 0, NUMCOMPUTEUNITS_SLR2, vpartition_stats, edgeblock_stats, globalparamsvA, globalparamsvB, globalparamsvC);
		total_sync[1] = MERGES_copy(1, reduce_partition, vdramB, vdramA, vdramC, statsB, voffsetB_kvs, NUMCOMPUTEUNITS_SLR2, NUMCOMPUTEUNITS_SLR1, vpartition_stats, edgeblock_stats, globalparamsvB, globalparamsvA, globalparamsvC);
		total_sync[2] = MERGES_copy(2, reduce_partition, vdramC, vdramA, vdramB, statsC, voffsetC_kvs, NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1, NUMCOMPUTEUNITS_SLR0, vpartition_stats, edgeblock_stats, globalparamsvC, globalparamsvA, globalparamsvB);
		
		#ifdef _COLLECT_METADATA_FROM_KERNEL_
		EXCHANGES_LOOP1B: for(unsigned int v_p=0; v_p<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v_p++){
			if(vpartition_stats[v_p] < globalparamsvA.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){
				unsigned int offset_vs = (v_p * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VDATA_PACKINGSIZE; // NEWCHANGE.
				unsigned int begin = v_p * MAXSZ_EDGEBLOCKS_PER_VPARTITION; unsigned int sz = MAXSZ_EDGEBLOCKS_PER_VPARTITION; // unsigned int end = begin + sz;
				if(begin + sz >= globalparamsvA.SIZEKVS2_REDUCEPARTITION){ sz = globalparamsvA.SIZEKVS2_REDUCEPARTITION - begin; }
				unsigned int index = 1; // 0;
				for(unsigned int t=begin; t<begin + sz; t++){
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 24", t, globalparamsvA.SIZEKVS2_REDUCEPARTITION, begin, begin + sz, sz);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 25", t, MAX_RED_SRAMSZ, begin, begin + sz, sz);
					#endif
					if(edgeblock_stats[t].data[0] > 0 || edgeblock_stats[t].data[1] > 0 || edgeblock_stats[t].data[2] > 0){
						UTILS_WriteData(vdramA, globalparamsvA.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						UTILS_WriteData(vdramB, globalparamsvB.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						UTILS_WriteData(vdramC, globalparamsvC.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						index += 1; // edgeblock_stats[t] = 0;
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 26", t-begin, MAXSZ_EDGEBLOCKS_PER_VPARTITION, NAp, NAp, NAp);
						#endif
						// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						cout<<"+++ EXCHANGE:: ACTIVE EDGE-BLOCK SEEN @ ["<<v_p<<"]["<<t<<"] "<<endl; 
						// #endif
					}
				}
				UTILS_WriteData(vdramA, globalparamsvA.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
				UTILS_WriteData(vdramB, globalparamsvB.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
				UTILS_WriteData(vdramC, globalparamsvC.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
			}
		}
		#endif 
	}
				
	#ifdef _DEBUGMODE_KERNELPRINTS	
	unsigned int sum=0, sum1=0, sum2=0, sum3=0, sum4 = 0; 
	bool debug = true; bool debug1 = true;
	// bool debug = true; bool debug1 = false;
	
	if(debug==true){ cout<<">>> exchange-copy:: printing vpartition_stats... "<<endl; }
	for(unsigned int v_p=0; v_p<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v_p++){ sum4 += vpartition_stats[v_p]; if(debug1==true){ cout<<"--- vpartition_stats["<<v_p<<"]: "<<vpartition_stats[v_p]<<endl; }}
	if(debug==true){ cout<<"+++::: sum4: "<<sum4<<endl; }
	
	if(debug==true){ cout<<">>> exchange-copy:: printing pmask_curr... "<<endl; }
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
	MEMACCESSS_readhelperstats(vdramA, pmask_curr, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	for(unsigned int t=0; t<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; t++){ sum3 += pmask_curr[t]; if(debug1==true){ cout<<"~~~ pmask_curr["<<t<<"]: "<<pmask_curr[t]<<endl; }}
	if(debug==true){ cout<<"~~~::: sum3: "<<sum3<<endl; }
	cout<<"exchange-copy: sum3,2,3,4: sum1: "<<sum1<<", sum2: "<<sum2<<", sum3: "<<sum3<<", sum4: "<<sum4<<"..."<<endl;
	#endif	
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	MEMACCESSS_savehelperstats(vdramA, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	MEMACCESSS_savehelperstats(vdramB, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	MEMACCESSS_savehelperstats(vdramC, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"finished. "<<total_sync[0] + total_sync[1] + total_sync[2]<<" vertices synchronized"<<endl; 
	#endif
	return;
}

extern "C" {
void acts_all::TOPS_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
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
	globalparams_t globalparamsvA = UTILS_getglobalparams(vdramA, 0);
	unsigned int inACTSMODE = UTILS_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"~~~ TOPS_topkernelS:: GraphIter: "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<endl;
	#endif
	
	#ifdef CONFIG_RELEASE_VERSION4
	if(globalparamsvA.ENABLE_EXCHANGECOMMAND == ON){
		MERGES_exchange(vdramA, vdramB, vdramC);
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

void acts_all::rearrangeLayoutVx16F(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("rearrangeLayoutVx16F(1)", s, UPDATEDATA_PACKINGSIZE, NAp, NAp, NAp);
	#endif
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
	
void acts_all::rearrangeLayoutVx16B(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]){
	unsigned int s_ = s;
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("rearrangeLayoutVx16B(1)", s, UPDATEDATA_PACKINGSIZE, NAp, NAp, NAp);
	#endif
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
	}
else if(s_==1){ 
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
else if(s_==2){ 
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
	}
else if(s_==3){ 
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
	}
else if(s_==4){ 
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
	}
else if(s_==5){ 
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
	}
else if(s_==6){ 
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
	}
else if(s_==7){ 
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
	}
else if(s_==8){ 
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
	}
else if(s_==9){ 
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
	}
else if(s_==10){ 
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
	}
else if(s_==11){ 
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
	}
else if(s_==12){ 
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
	}
else if(s_==13){ 
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
	}
else if(s_==14){ 
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
	}
else { 
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
	}
	return;
}

void acts_all::check_if_contiguous(keyvalue_t keyvalue[EDGEDATA_PACKINGSIZE], keyvalue_t msg1[EDGEDATA_PACKINGSIZE], keyvalue_t msg2[EDGEDATA_PACKINGSIZE], unsigned int msg1_str, unsigned int msg2_str, unsigned int msg3_str){
	#ifdef _DEBUGMODE_CHECKS3
	for(int v = 0; v < UPDATEDATA_PACKINGSIZE; v++){ 
		if(keyvalue[v].key != INVALIDDATA){ 
			if(keyvalue[v].key % UPDATEDATA_PACKINGSIZE != v){ 
				cout<<"check_if_contiguous(1). ERROR 34. v["<<v<<"].key("<<keyvalue[v].key<<") % UPDATEDATA_PACKINGSIZE("<<UPDATEDATA_PACKINGSIZE<<")(="<<keyvalue[v].key % UPDATEDATA_PACKINGSIZE<<") != v("<<v<<"). [msg1_str: "<<msg1_str<<", msg2_str: "<<msg2_str<<", msg3_str: "<<msg3_str<<"] EXITING..."<<endl; 
				for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){ cout<<"+++ keyvalue["<<v<<"].key: "<<keyvalue[v].key<<"("<<keyvalue[v].key%EDGEDATA_PACKINGSIZE<<"), keyvalue["<<v<<"].value: "<<keyvalue[v].value<<"("<<keyvalue[v].value%EDGEDATA_PACKINGSIZE<<")"<<endl; }
				for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){ cout<<"+++ msg1["<<v<<"].key: "<<msg1[v].key<<"("<<msg1[v].key%EDGEDATA_PACKINGSIZE<<"), msg1["<<v<<"].value: "<<msg1[v].value<<"("<<msg1[v].value%EDGEDATA_PACKINGSIZE<<")"<<endl; }
				for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){ cout<<"+++ msg2["<<v<<"].key: "<<msg2[v].key<<"("<<msg2[v].key%EDGEDATA_PACKINGSIZE<<"), msg2["<<v<<"].value: "<<msg2[v].value<<"("<<msg2[v].value%EDGEDATA_PACKINGSIZE<<")"<<endl; }
				exit(EXIT_FAILURE); 
			}
		}
	}
	#endif
}

keyvalue_t acts_all::process_edge(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t edge_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparams){				
	#pragma HLS INLINE
	
	// flag 
	// if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){ loc = 0; }
	// bool special_loc = false; if(loc == 16383){ loc = 0; special_loc = true; } // header information
	#ifdef _DEBUGMODE_CHECKS3
	// if(enx == true && loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && loc != 16383){ cout<<"processvector::ERROR SEEN @@ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<", mode: "<<mode<<". EXITING... "<<endl; exit(EXIT_FAILURE); }					
	if(enx == true && loc >= MAX_BLOCKRAM_VDESTDATA_SIZE && loc != 16383){ cout<<"processvector::ERROR SEEN @@ loc("<<loc<<") >= MAX_BLOCKRAM_VDESTDATA_SIZE("<<MAX_BLOCKRAM_VDESTDATA_SIZE<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<", mode: "<<mode<<". EXITING... "<<endl; exit(EXIT_FAILURE); }					
	#endif 
	
	// read
	value_t combo = 0; if(enx == true && loc != 16383){ combo = vbuffer[loc].data; }
	value_t mask; if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { mask = combo & 0x1; }
	value_t udata = combo >> 1; 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mask == 1 && mode == ACTSPROCESSMODE){ cout<<">>> PROCESS VECTOR:: PROCESS EDGE SEEN @ v: "<<v<<", loc: "<<loc<<", edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", udata: "<<udata<<", mask: "<<mask<<", srcvid: "<<(edge_data.value * EDGEDATA_PACKINGSIZE) + v<<", dstvid*: "<<UTILS_GETREALVID(edge_data.key, globalparams.ACTSPARAMS_INSTID)<<", ldstvid: "<<edge_data.key<<endl; }
	#endif
	
	// process
	keyvalue_t vupdate; 
	if(mode == ACTSPROCESSMODE){
		value_t res = process_func(udata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		vupdate; if(mask == 1 && loc != 16383){ vupdate.key = edge_data.key; vupdate.value = res; } else { vupdate.key = INVALIDDATA; vupdate.value = INVALIDDATA; }
	} else {
		vupdate.key = combo; vupdate.value = combo;
	}
	return vupdate;
}

void acts_all::reduce_update(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t update_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int stats[BLOCKRAM_SIZE], unsigned int memory[1], globalparams_t globalparams){
	#pragma HLS INLINE

	// flag 
	bool en = true;
	
	// checks
	#ifdef _DEBUGMODE_CHECKS3
	if(enx == true && loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE)){ cout<<"reduce_update::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). update_data.key: "<<update_data.key<<", update_data.value: "<<update_data.value<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	// if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION && en == true){ loc = 0; } 
	// if(enx == true && mode == ACTSREDUCEMODE){ if(loc == memory[0]){ cout<<"reduce_update:: Error. loc: "<<loc<<", memory[0]: "<<memory[0]<<", mode: "<<mode<<endl; exit(EXIT_FAILURE); }} // CRITICAL FIXME.
	// if(enx == true && mode == ACTSREDUCEMODE){ if(loc == memory[0]){ loc = (loc + 1) % 8; } memory[0] = loc; } // FIXME.
	#ifdef _DEBUGMODE_CHECKS3
	if(enx == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 113::1", loc, MAX_BLOCKRAM_VDESTDATA_SIZE, update_data.key, update_data.value, mode); }
	#endif

	// read & reduce 
	value_t new_combo; value_t mask; value_t vdata_tmp; value_t new_vprop;
	if(mode == ACTSREDUCEMODE){ 
		value_t combo = 0;
		if(enx == true){ combo = vbuffer[loc].data; }
		if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { mask = combo & 0x1; }
		vdata_tmp = combo >> 1; 
		
		new_vprop = reduce_func(vdata_tmp, vdata_tmp, update_data.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		new_combo = (new_vprop << 1) | 0x1;
		if(enx == true && new_vprop != vdata_tmp){ en = true; } else { en = false; }
	} else { new_combo = update_data.value; }
	
	// write-back
	if(en == true){ // REMOVEME.
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3  // REMOVEME.
		if(mode == ACTSREDUCEMODE){ cout<<">>> REDUCE VECTOR:: REDUCE UPDATE SEEN @: v: "<<v<<", loc: "<<loc<<", vdata_tmp: "<<vdata_tmp<<", mask: "<<mask<<", update_data.key: "<<update_data.key<<", update_data.value: "<<update_data.value<<", new_vprop: "<<new_vprop<<", new combo: "<<((new_vprop << 1) | 0x1)<<", dstvid: "<<UTILS_GETREALVID(update_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
		#endif
		
		vbuffer[loc].data = new_combo;
	}
	return;
}	

#ifdef FPGA_IMPL
void load(uint512_dt *in, hls::stream<uint512_evec_dt >& out, batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, int size){
	Loop_Ld: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		uint512_evec_dt data = UTILS_ReadEdges(in, sourcebaseaddr_kvs + srcoffset_kvs + i);
		out.write(data);
	}
}

void compute_and_store(unsigned int mode, unsigned int llp_set, hls::stream<uint512_evec_dt >& in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
					batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, unsigned int destindex, int size,
						unsigned int offset_buffer, // SAVEVPROPERTYMODE parameters
							collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	
	keyvalue_t res[UPDATEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas2 complete
	keyvalue_t edges[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = edges complete
	keyvalue_t res_out[EDGEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res_out complete
	keyvalue_t data[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = data complete
	unsigned int index[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = index complete
	bool en[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = en complete
	
	for(int v = 0; v < VECTOR2_SIZE; v++){ en[v] = true; }
	unsigned int offsetkvs_dstvid = llp_set * (globalparamsK.SIZEKVS2_REDUCEPARTITION * EDGEDATA_PACKINGSIZE); // CHECKME.
	
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; //   (1 << (globalparamsK.POW_BATCHRANGE - BLOCKRAM_UPDATEBLOCK_POW)) / VDATA_PACKINGSIZE;
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock;	
	
	ACTIT_COMPUTEANDSTORE_MAINLOOP2A: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		// read
		uint512_evec_dt in_data = in.read();
		
		// decode 
		if(mode == ACTSPROCESSMODE){ 
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(in_data.data[v] != INVALIDDATA){ en[v] = true; } else { en[v] = false; }
			}
			
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(in_data.data[v] != INVALIDDATA){ edges[v].key = in_data.data[v] & MASK_DSTVID; edges[v].value = (in_data.data[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // srcvid is upper, dstvid is lower 
				else { edges[v].key = INVALIDDATA; edges[v].value = INVALIDDATA; }
			}
			
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				data[v] = edges[v]; index[v] = edges[v].value;
			}
		} else {
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				index[v] = offset_buffer + i; data[v].key = datas[v]; data[v].value = datas[v]; en[v] = true;
			}
		}
		
		// process
		for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL
			res[v] = process_edge(mode, en[v], v, index[v], data[v], vbuffer_source[v], globalparamsK);
		}	
		
		// encode 
		if(mode == ACTSPROCESSMODE){ 
			// rotateby >>>
			unsigned int rotateby = 0; unsigned int sample_key = INVALIDDATA; unsigned int sample_u = 0; unsigned int rotate_forward = 1; 
			if(edges[0].value == 16383){ rotateby = edges[0].key >> 1; rotate_forward = edges[0].key & 0x1; } 
			else { sample_key = edges[0].key % UPDATEDATA_PACKINGSIZE; sample_u = 0; if(sample_key > sample_u){ rotateby = sample_key - sample_u; rotate_forward = 0; } else { rotateby = sample_u - sample_key; rotate_forward = 1; }}		
			if(rotate_forward == 0){ rearrangeLayoutVx16B(rotateby, res, res_out); } else{ rearrangeLayoutVx16F(rotateby, res, res_out); }
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 59::", rotateby, EDGEDATA_PACKINGSIZE, sample_key, edges[0].value, datas[0]);	// sample_key, edges[0].value, datas[0], sample_u, edges[0].key
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 60::", rotate_forward, 2, sample_key, sample_u, edges[0].key);	
			check_if_contiguous(res_out, edges, res, sample_key, rotate_forward, rotateby);
			#endif 
			
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				if(res_out[v].key != INVALIDDATA){ datas2[v] = (res_out[v].value << DSTVID_BITSZ) | res_out[v].key; } else { datas2[v] = INVALIDDATA; }
			}
		} else {
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				datas2[v] = res[v].value; 
			}
		}

		// store
		UTILS_WriteDatas(out, destbaseaddr_kvs + destoffset_kvs + destindex + i, datas2);
		
		// colelect stats
		if(mode == ACTSPROCESSMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				#ifdef _DEBUGMODE_CHECKS3
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112)::DEBUG CODE 112::1", (offsetkvs_dstvid + edges[v].key) >> (globalparamsK.POW_BATCHRANGE - BLOCKRAM_UPDATEBLOCK_POW), BLOCKRAM_SIZE, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				#endif	
				if(res[v].key != INVALIDDATA){
					// if(res[v].key != INVALIDDATA){ stats[v][((offsetkvs_dstvid + edges[v].key) / EDGEDATA_PACKINGSIZE) / numkvs_vertices_per_updateblock] = 0xFFFFFFFF; } // = vertexid / number of vertices in an update block
					if(res[v].key != INVALIDDATA){ stats[v][((offsetkvs_dstvid + edges[v].key) / VDATA_PACKINGSIZE) / (((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE)] = 0xFFFFFFFF; } // = vertexid / number of vertices in an update block
				}
			}
		}
		
		// collect stats 
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(mode == ACTSPROCESSMODE){ 
			collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
			collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE; // *** used in PR, CF implementations ONLY (not BFS) ***
		}
		if(en == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE); }
		#endif 
	}
}
#endif 

void acts_all::ACTSS_write__process(unsigned int mode, unsigned int llp_set, uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], 
			batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, unsigned int destindex, int size,
				unsigned int offset_buffer, // SAVEVPROPERTYMODE parameters
					collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	
	keyvalue_t res[UPDATEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas2 complete
	keyvalue_t edges[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = edges complete
	keyvalue_t res_out[EDGEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res_out complete
	keyvalue_t data[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = data complete
	unsigned int index[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = index complete
	bool en[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = en complete
	
	for(int v = 0; v < VECTOR2_SIZE; v++){ en[v] = true; }
	unsigned int offsetkvs_dstvid = llp_set * (globalparamsK.SIZEKVS2_REDUCEPARTITION * EDGEDATA_PACKINGSIZE); // CHECKME.
	
	// unsigned int offsetkvs_stats = llp_set * numkvs_vertices_per_updateblock;
	
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; //   (1 << (globalparamsK.POW_BATCHRANGE - BLOCKRAM_UPDATEBLOCK_POW)) / VDATA_PACKINGSIZE;
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock;	
	unsigned int offsetkvs_stats = llp_set * numkvs_vertices_per_updateblock; // NEWCHANGE.
	
	for(int i = 0; i < size; i++){
		// read 
		if(mode == ACTSPROCESSMODE){ 
			UTILS_ReadEdges(in, sourcebaseaddr_kvs + srcoffset_kvs + i, datas);
		}
		
		// decode 
		if(mode == ACTSPROCESSMODE){ 
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(datas[v] != INVALIDDATA){ en[v] = true; } else { en[v] = false; }
			}
			
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(datas[v] != INVALIDDATA){ edges[v].key = datas[v] & MASK_DSTVID; edges[v].value = (datas[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // srcvid is upper[31-18], dstvid is lower[17-0]
				else { edges[v].key = INVALIDDATA; edges[v].value = INVALIDDATA; }
			}
			
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				data[v] = edges[v]; index[v] = edges[v].value;
			}
		} else {
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				// index[v] = i; data[v].key = datas[v]; data[v].value = datas[v]; en[v] = true;
				index[v] = offset_buffer + i; data[v].key = datas[v]; data[v].value = datas[v]; en[v] = true;
			}
		}
		
		// process
		for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL
			res[v] = process_edge(mode, en[v], v, index[v], data[v], vbuffer_source[v], globalparamsK);
		}	
		
		// encode 
		if(mode == ACTSPROCESSMODE){ 
			// rotateby >>>
			unsigned int rotateby = 0; unsigned int sample_key = INVALIDDATA; unsigned int sample_u = 0; unsigned int rotate_forward = 1; 
			if(edges[0].value == 16383){ rotateby = edges[0].key >> 1; rotate_forward = edges[0].key & 0x1; } 
			else { sample_key = edges[0].key % UPDATEDATA_PACKINGSIZE; sample_u = 0; if(sample_key > sample_u){ rotateby = sample_key - sample_u; rotate_forward = 0; } else { rotateby = sample_u - sample_key; rotate_forward = 1; }}		
			if(rotate_forward == 0){ rearrangeLayoutVx16B(rotateby, res, res_out); } else{ rearrangeLayoutVx16F(rotateby, res, res_out); }
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 59::", rotateby, EDGEDATA_PACKINGSIZE, sample_key, edges[0].value, datas[0]);	// sample_key, edges[0].value, datas[0], sample_u, edges[0].key
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 60::", rotate_forward, 2, sample_key, sample_u, edges[0].key);	
			check_if_contiguous(res_out, edges, res, sample_key, rotate_forward, rotateby);
			#endif 
			
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				if(res_out[v].key != INVALIDDATA){ datas2[v] = (res_out[v].value << DSTVID_BITSZ) | res_out[v].key; } else { datas2[v] = INVALIDDATA; }
			}
		} else { // if(mode == SAVEVPROPERTYMODE){ 
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				datas2[v] = res[v].value; 
			}
		}

		// store
		UTILS_WriteDatas(out, destbaseaddr_kvs + destoffset_kvs + destindex + i, datas2); 
		
		// colelect stats
		if(mode == ACTSPROCESSMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				// unsigned int loc = ((offsetkvs_dstvid + edges[v].key) / VDATA_PACKINGSIZE) / (((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE);
				unsigned int loc = (edges[v].key / VDATA_PACKINGSIZE) / (((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE);
				#ifdef _DEBUGMODE_CHECKS3
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112)::DEBUG CODE 112::1", loc, numkvs_updateblocks_per_reducepartition, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112b)::DEBUG CODE 112::1", loc, BLOCKRAM_UPDATEBLOCK_SIZE, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				#endif	
				// if(res[v].key != INVALIDDATA){ stats[v][((offsetkvs_dstvid + edges[v].key) / EDGEDATA_PACKINGSIZE) / numkvs_vertices_per_updateblock] = 0xFFFFFFFF; }
				if(res[v].key != INVALIDDATA){ stats[v][offsetkvs_stats + loc] = 0xFFFFFFFF; } // = vertexid / number of vertices in an update block
			}
		}
		
		// collect stats for debugging
		#ifdef _DEBUGMODE_STATS
		if(mode == ACTSPROCESSMODE){ out[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key += EDGEDATA_PACKINGSIZE; }
		else { out[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += EDGEDATA_PACKINGSIZE; }
		#endif
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(mode == ACTSPROCESSMODE){ 
			collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
			collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE; // *** used in PR, CF implementations ONLY (not BFS) ***
		}
		if(en == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE); }
		#endif 
	}
	// exit(EXIT_SUCCESS);
}

void acts_all::ACTSS_write__process_base(unsigned int mode, unsigned int llp_set, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],	
			unsigned int sourcebaseaddr_kvs, workload_t workload_kvs, unsigned int destbaseaddr_kvs, unsigned int destoffset_kvs, unsigned int destindex,
				unsigned int offset_buffer, // SAVEVPROPERTYMODE parameters
					collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){

	#ifdef FPGA_IMPL
	hls::stream<uint512_evec_dt> c0; //, c1, c2, c3, c4, c5;
	#pragma HLS STREAM variable = c0 depth = 512

	#pragma HLS dataflow
	if(mode == ACTSPROCESSMODE){ load(kvdram, c0, sourcebaseaddr_kvs, workload_kvs.offset_begin, workload_kvs.size); }
	compute_and_store(mode, llp_set, c0, kvdram, vbuffer_source, stats,
					destbaseaddr_kvs, destoffset_kvs, destindex, workload_kvs.size, 
						collections, globalparamsK, globalposition);
						
	#else 
	ACTSS_write__process(mode, llp_set, kvdram, kvdram, vbuffer_source, stats,
			sourcebaseaddr_kvs, workload_kvs.offset_begin, destbaseaddr_kvs, destoffset_kvs, destindex, workload_kvs.size, 
				offset_buffer,
					collections, globalparamsK, globalposition);
	#endif
}

void acts_all::ACTSS_read__reduce(unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
			unsigned int sourcebaseaddr_kvs, workload_t workload_kvs, 
				unsigned int offset_ubuffer, unsigned int s, // READUPROPERTYMODE parameters
					unsigned int offsetkvs_vbuffer, // READVPROPERTYMODE parameters
						collection_t collections[COLLECTIONS_BUFFERSZ], sweepparams_t sweepparams, globalparams_t globalparamsK, globalposition_t globalposition){
	
	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory 
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas2 complete
	keyvalue_t updates_in[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = updates_in complete
	keyvalue_t updates_inter[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = updates_inter complete
	keyvalue_t updates_out[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = updates_out complete
	keyvalue_t data[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = data complete
	unsigned int index[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = index complete
	bool en[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = en complete
	
	
	for(int v = 0; v < VECTOR2_SIZE; v++){ updates_in[v].key = INVALIDDATA; updates_in[v].value = INVALIDDATA; memory[v][0] = 777; }
	if(mode == READVPROPERTYMODE){ for(int v = 0; v < VECTOR2_SIZE; v++){ updates_in[v].key = 0; updates_in[v].value = 0; }}
	for(int v = 0; v < VECTOR2_SIZE; v++){ en[v] = true; }
	unsigned int ind = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalparamsK.ACTSPARAMS_INSTID==0){ cout<<"actit(reduce): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][workload_kvs.size: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<(workload_kvs.offset_end - workload_kvs.offset_begin) / globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"] ... "<<endl; }		 // REMOVEME. 2.		
	#endif
	ACTIT_READANDREDUCE_MAINLOOP2A: for(batch_type offset_kvs=workload_kvs.offset_begin; offset_kvs<workload_kvs.offset_begin + workload_kvs.size; offset_kvs++){
	#pragma HLS PIPELINE II=1
	#pragma HLS dependence variable=vbuffer_dest inter false
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(globalparamsK.ACTSPARAMS_INSTID==0){ cout<<"actit(reduce): processing chunk [offset_kvs: "<<offset_kvs<<"]: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<"]: [workload_kvs.offset_end: "<<workload_kvs.offset_end<<"] ... "<<endl; } // REMOVEME. 2.
		#endif
		
		// read 
		UTILS_ReadDatas(kvdram, sourcebaseaddr_kvs + offset_kvs, datas);
		
		// decode
		if(mode == ACTSREDUCEMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(datas[v] != INVALIDDATA){ updates_in[v].key = datas[v] & MASK_DSTVID; updates_in[v].value = (datas[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // 16, 18, srcvid is upper, dstvid is lower 
				else { updates_in[v].key = INVALIDDATA; updates_in[v].value = INVALIDDATA; }
			}
		} else {
			if(mode == READUPROPERTYMODE){
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v]; // & 0xFFFFFFFE; 
				}
			} else {
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v] & 0xFFFFFFFE; 
				}
			}
		}
		
		// prepare inputs
		if(mode == ACTSREDUCEMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				index[v] = updates_in[v].key / UPDATEDATA_PACKINGSIZE; data[v] = updates_in[v]; if(updates_in[v].key != INVALIDDATA){ en[v] = true; } else { en[v] = false; }
			}
		} else {
			if(mode == READUPROPERTYMODE){
				for(int v = 0; v < VECTOR2_SIZE; v++){ 
				#pragma HLS UNROLL
					index[v] = offset_ubuffer + s + ind; data[v].value = datas2[v]; en[v] = true;
				}
				ind += NUM_PEs;
			} else { // if(mode == READVPROPERTYMODE){ 
				for(int v = 0; v < VECTOR2_SIZE; v++){ 
				#pragma HLS UNROLL 
					index[v] = offsetkvs_vbuffer + (offset_kvs - workload_kvs.offset_begin); data[v].value = datas2[v]; en[v] = true; // CRITICAL NEWCHANGE
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, v, offsetkvs_vbuffer, mode); 
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, v, offsetkvs_vbuffer, offset_kvs); 
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112b::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, offsetkvs_vbuffer, offset_kvs, workload_kvs.offset_begin); 
					#endif	
				}
			}
		}
		
		// reduce	
		for(int v = 0; v < UPDATEDATA_PACKINGSIZE; v++){ 
		#pragma HLS UNROLL 
			reduce_update(mode, en[v], v, index[v], data[v], vbuffer_dest[v], stats[v], memory[v], globalparamsK);
		}
		
		#ifdef _DEBUGMODE_STATS
		if(mode == ACTSREDUCEMODE){ kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key += UPDATEDATA_PACKINGSIZE; }
		else { kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += UPDATEDATA_PACKINGSIZE; }
		#endif 
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(mode == ACTSREDUCEMODE){ collections[REDUCEUPDATES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE; }
		#endif 
	}
	// exit(EXIT_SUCCESS);
}

void acts_all::ACTSS_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){

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
	// cout<<"-------------------- actit: numkvs_vertices_per_updateblock: "<<numkvs_vertices_per_updateblock<<", numkvs_updateblocks_per_reducepartition: "<<numkvs_updateblocks_per_reducepartition<<endl;
	#ifdef _DEBUGMODE_CHECKS3
	if(globalparamsK.POW_BATCHRANGE < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)){ cout<<"actit:: ERROR 234. globalparamsK.POW_BATCHRANGE("<<globalparamsK.POW_BATCHRANGE<<") < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)("<<BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	if(globalparamsK.POW_BATCHRANGE < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)){ cout<<"actit:: ERROR 234. globalparamsK.POW_BATCHRANGE("<<globalparamsK.POW_BATCHRANGE<<") < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)("<<BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	#endif 
	
	bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
	bool sparse_process = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	bool sparse_readv = false; if((stats[17][sweepparams.source_partition + 1] - stats[17][sweepparams.source_partition]) < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION){ sparse_readv = true; } else { sparse_readv = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mode == ACTSPROCESSMODE){ if(sparse_readu == true){ cout<<"actit:: sparse_readu is TRUE. "<<endl; } }
	if(mode == ACTSPROCESSMODE){ if(sparse_process == true){ cout<<"actit:: sparse_process is TRUE. "<<endl; } }
	if(mode == ACTSREDUCEMODE){ if(sparse_readv == true){ cout<<"actit:: sparse_readv is TRUE. stats[17]["<<sweepparams.source_partition<<"]: "<<stats[17][sweepparams.source_partition]<<", stats[17]["<<sweepparams.source_partition+1<<"]: "<<stats[17][sweepparams.source_partition+1]<<endl; }}					
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mode == ACTSPROCESSMODE){ if(sparse_readu == true){ cout<<"actit:: sparse_readu is TRUE. "<<endl; } else { cout<<"actit:: sparse_readu is FALSE. "<<endl; }}
	if(mode == ACTSPROCESSMODE){ if(sparse_process == true){ cout<<"actit:: sparse_process is TRUE. "<<endl; } else { cout<<"actit:: sparse_process is FALSE. "<<endl; }}
	if(mode == ACTSREDUCEMODE){ if(sparse_readv == true){ cout<<"actit:: sparse_readv is TRUE. stats[17]["<<sweepparams.source_partition<<"]: "<<stats[17][sweepparams.source_partition]<<", stats[17]["<<sweepparams.source_partition+1<<"]: "<<stats[17][sweepparams.source_partition+1]<<endl; } else { cout<<"actit:: sparse_readv is FALSE. stats[17]["<<sweepparams.source_partition<<"]: "<<stats[17][sweepparams.source_partition]<<", stats[17]["<<sweepparams.source_partition+1<<"]: "<<stats[17][sweepparams.source_partition+1]<<endl; }}					
	if(false){ cout<<"actit:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", num_vPs: "<<num_vPs<<"] "<<endl; }
	if(false){ cout<<"actit:: globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION: "<<globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION<<""<<endl; }
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mode == ACTSPROCESSMODE){ cout<<"-------------------- actit: v_p: "<<sweepparams.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
	#endif 
	
	bool en = true;
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && mode == ACTSPROCESSMODE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active

	if(en == true){
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(mode == ACTSPROCESSMODE){ cout<<"processit->actit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; }
		#endif 
	
		if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){
			#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
			unsigned int num_its = mergeS_get_upropblock_workload(true, sweepparams.source_partition, kvdram, globalposition.num_active_edgeblocks, globalparamsK, xload_kvs, buffer_offsets, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
			// for(int t = 0; t < MAX_BLOCKRAM_VDESTDATA_SIZE; t++){ for(int v = 0; v < VDATA_PACKINGSIZE; v++){ vbuffer_source[v][t].data = 0; }} ////////////////////////////////////////// CRITICAL REMOVEME.
			for(unsigned int n=0; n<num_its; n++){
				unsigned int voffset_kvs = 0;
				for(unsigned int s=0; s<NUM_PEs; s++){
					ACTSS_read__reduce(READUPROPERTYMODE, kvdram, vbuffer_source, stats,
						globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs, xload_kvs[n], 
							buffer_offsets[n], s, // READUPROPERTYMODE parameters
								NAp, // READVPROPERTYMODE parameters
									collections_tmp, sweepparams, globalparamsK, globalposition);
					voffset_kvs += depth;
				}	
			}	
			#endif
			
			ACTIT_MAINLOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
				batch_type destoffset_kvs = (globalcapsule[1 + llp_set].key + globalcapsule[1 + llp_set].value) / EDGEDATA_PACKINGSIZE;
				unsigned int destindex = 0; 
				workload_t last_workload;
				unsigned int num_its = mergeS_get_edgeblock_workload(true, sweepparams.source_partition, llp_set, kvdram, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE, xload_kvs, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
				
				READ_PROCESS_PARTITION_STORE_LOOP1B: for(unsigned int n=0; n<num_its; n++){
					workload_t workload_kvs = xload_kvs[n];
					if(num_its > 1 && n > 0){ // optimization - skip already read edge-blocks
						if(xload_kvs[n].offset_begin > last_workload.offset_begin && xload_kvs[n].offset_begin < last_workload.offset_end){
							if(xload_kvs[n].offset_end < last_workload.offset_end){ workload_kvs.offset_begin = 0; workload_kvs.size = 0; } 
							else { workload_kvs.offset_begin = last_workload.offset_end; workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin; }
						}
					}
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.size, (1 << 28) / 8, NAp, NAp, n);
					#endif
					ACTSS_write__process_base(mode, llp_set, kvdram, vbuffer_source, stats,	
							sourcebaseaddr_kvs, workload_kvs, destbaseaddr_kvs, destoffset_kvs, destindex,
								NAp, // SAVEVPROPERTYMODE parameters
									collections_tmp, globalparamsK, globalposition);
					
					globalcapsule[1 + llp_set].value += workload_kvs.size * EDGEDATA_PACKINGSIZE;	
					destindex += workload_kvs.size;
					if(num_its > 1){ last_workload = xload_kvs[n]; }
				}
			}
		}
		
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

			unsigned int num_its = mergeS_get_updateblock_workload(true, sweepparams.source_partition, stats[16], stats[17], globalparamsK, xload_kvs, buffer_offsets);
			// for(int t = 0; t < MAX_BLOCKRAM_VDESTDATA_SIZE; t++){ for(int v = 0; v < VDATA_PACKINGSIZE; v++){ vbuffer_dest[v][t].data = 0; }} ///////////////////////////////////////// CRITICAL REMOVEME.
			for(unsigned int n=0; n<num_its; n++){
				ACTSS_read__reduce(READVPROPERTYMODE, kvdram, vbuffer_dest, stats,
					globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, xload_kvs[n], 
						NAp, NAp, // READUPROPERTYMODE parameters
							buffer_offsets[n], // READVPROPERTYMODE parameters
								collections_tmp, sweepparams, globalparamsK, globalposition);
			}
		
			ACTSS_read__reduce(ACTSREDUCEMODE, kvdram, vbuffer_dest, stats,
					sourcebaseaddr_kvs, workload_kvs, 
						NAp, NAp, // READUPROPERTYMODE parameters
							NAp, // READVPROPERTYMODE parameters
								collections_tmp, sweepparams, globalparamsK, globalposition);

			for(unsigned int n=0; n<num_its; n++){
				ACTSS_write__process_base(SAVEVPROPERTYMODE, NAp, kvdram, vbuffer_dest, stats,
						sourcebaseaddr_kvs, xload_kvs[n], globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, xload_kvs[n].offset_begin, 0,
							buffer_offsets[n], // SAVEVPROPERTYMODE parameters
								collections_tmp, globalparamsK, globalposition);
			}
		}
	}
	
	// prepare stats
	if(mode == ACTSPROCESSMODE && UTILS__processit__i_am_last__(globalposition) == true){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"-------------------------------- __processit__i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl;
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit:: numkvs_vertices_per_updateblock: "<<numkvs_vertices_per_updateblock<<", numkvs_updateblocks_per_reducepartition: "<<numkvs_updateblocks_per_reducepartition<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl;
		#endif 
		unsigned int stats_tmp[VECTOR2_SIZE]; for(int v = 0; v < VECTOR2_SIZE; v++){ stats_tmp[v] = 0; }
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
			UTILS_WriteDatas(kvdram, globalparamsK.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + t, datas);
		}
	}
	
	collections[TRAVERSEDEDGES_COLLECTIONID] = collections_tmp[TRAVERSEDEDGES_COLLECTIONID];
	collections[PROCESSEDGES_COLLECTIONID] = collections_tmp[PROCESSEDGES_COLLECTIONID];
	collections[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID];
	collections[REDUCEUPDATES_COLLECTIONID] = collections_tmp[REDUCEUPDATES_COLLECTIONID];
	// exit(EXIT_SUCCESS);
}

	
#endif 			
void acts_all::TOPS_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE 
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete

	sweepparams_t sweepparams;
	sweepparams = UTILS_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); //. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	batch_type voffset_kvs = globalposition.source_partition * UTILS_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	bool __processit__i_am_first__ = false; 
	bool __processit__i_am_last__ = false;
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ __processit__i_am_first__ = true;; } else { __processit__i_am_first__ = false; }
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ __processit__i_am_last__ = true; } else { __processit__i_am_last__ = false; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(__processit__i_am_first__ == true){ cout<<"-------------------------------- __processit__i_am_first__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	if(__processit__i_am_last__ == true){ cout<<"-------------------------------- __processit__i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	actsutilityobj->print7("### TOPS_processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, NAp, sweepparams.currentLOP); 							
	#endif

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print5("### TOPS_processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, MAX_SRCBUFFER_SIZE * VECTOR_SIZE);
	#endif
	
	sweepparams.source_partition = globalposition.source_partition;
	travstate_t etravstate; bool_type resetenv = ON; bool_type flush = ON;
	
	ACTSS_actit(
		ON, ACTSPROCESSMODE,
		kvdram, vbuffer_source, vbuffer_dest, stats, globalcapsule,
		globalparamsE, globalparamsK, globalposition, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAM,
		resetenv, flush, edgebankID, collections);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPS_processit", globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS); }
	#endif
	return;
}

void acts_all::TOPS_partitionit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	if(globalparamsK.EVALUATION_ACTS_RECURSIVEPARTITIONINGLOGIC != ON){ return; } ///
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nusrcv_nudstv::UTILS_getglobalparams:: printing global parameters", globalparamsK);
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
	
		batch_type num_source_partitions = UTILS_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"TOPS_partitionit: source_partition "<<source_partition<<" ... "<<endl;

			UTILS_resetkeysandvalues(globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
		
			sweepparams = UTILS_getsweepparams(globalparamsK, currentLOP, source_partition);
			travstate_t ptravstate = UTILS_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparamsK.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### TOPS_partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			UTILS_resetvalues(globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			ACTSS_actit(config.enablepartition, ACTSPARTITIONMODE,
					kvdram, vbuffer_source, vbuffer_dest, stats, globalcapsule, // CRITICAL FIXME.
					globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp, collections);
					
			// MEMACCESSS_saveglobalstats(config.enablepartition, kvdram, globalcapsule, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK); 
			
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

void acts_all::TOPS_reduceit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	globalposition_t globalposition; globalposition.source_partition = NAp; // DUMMY.
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	
	sweepparams = UTILS_getsweepparams(globalparamsK, currentLOP, source_partition);
	travstate_t ptravstate; ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; // DUMMY
	config.enablereduce = ON;
	
	ACTSS_actit(config.enablereduce, ACTSREDUCEMODE,
			 kvdram, vbuffer_source, vbuffer_dest, stats, globalcapsule, // CRITICAL FIXME.
			globalparamsE, globalparamsK, globalposition, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp, collections);
	return;
}

void acts_all::TOPS_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], 
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(en_process == ON){ TOPS_processit(kvdram, vbuffer_source, vbuffer_dest, stats, globalcapsule, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(en_partition == ON){ TOPS_partitionit(kvdram, vbuffer_source, vbuffer_dest, stats, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(en_reduce == ON){ TOPS_reduceit(kvdram, vbuffer_source, vbuffer_dest, stats, globalcapsule, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	return;
} 

void acts_all::TOPS_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	unsigned int sourcestatsmarker = 1;
	unsigned int voffset_kvs = 0;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = globalparamsK.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("TOPS_dispatch_reduce", globalcapsule, globalparamsK.NUM_REDUCEPARTITIONS);
	#endif
	
	bool_type enablereduce = ON;
	DISPATCHREDUCES_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		actsutilityobj->print4("### TOPS_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "voffset_kvs", source_partition, currentLOP, num_source_partitions, voffset_kvs); 							
		#endif
		
		TOPS_dispatch(OFF, OFF, enablereduce, kvdram, vbuffer_source, vbuffer_dest, stats, globalcapsule, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);
		
		sourcestatsmarker += 1;
		voffset_kvs += globalparamsK.SIZEKVS2_REDUCEPARTITION;
	}
	return;
} 

void acts_all::TOPS_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[COLLECTIONS_BUFFERSZ]){									

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	unsigned int printheader1=OFF; // ON, OFF*
	unsigned int printheader2=OFF; 
	if(true && globalposition.source_partition==globalposition.first_source_partition){ printheader1 = ON; } else { printheader1 = OFF; }
	if(false && globalposition.source_partition==globalposition.last_source_partition){ printheader2 = ON; } else { printheader2 = OFF; }
	// printheader1=ON;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> ====================== ACTS Launched @ instance: "<<globalid<<" ====================== "<<endl;
	#endif
		
	#ifdef TESTKERNEL 
	if(UTILS_isbufferused(globalid) == false){ return; }
	#endif 
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][MAX_SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	unsigned int PARTITIONS_CHKPT[EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
	
	globalparams_t globalparamsK;
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	globalparams_t _globalparamsE;
	
	globalparamsK = UTILS_getglobalparams(kvdram, 0); // CRITICAL OPTIMIZEME. MOVETOBASE?
	
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram, e+1); }
	_globalparamsE = globalparamsEs[globalposition.edgebankID];
	globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid;
	for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE. IS THIS OPTIMIZED
	
	globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = GraphIter; // overrride these with GraphIter
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e].ALGORITHMINFO_GRAPHITERATIONID = GraphIter; }
	_globalparamsE.ALGORITHMINFO_GRAPHITERATIONID = GraphIter;
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	bool __processit__i_am_first__ = true; if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ __processit__i_am_first__ = true;; } else { __processit__i_am_first__ = false; }
	if(__processit__i_am_first__ == true){ UTILS_resetvalues(globalcapsule, globalparamsK.NUM_REDUCEPARTITIONS, 0); } 
	if(__processit__i_am_first__ == true){ UTILS_resetkvstatvalues(kvdram, globalparamsK); } // NEWCHANGE
	if(__processit__i_am_first__ == true){ MEMACCESSS_readglobalstats(ON, kvdram, globalcapsule, globalparamsK.BASEOFFSETKVS_STATSDRAM, globalparamsK); }
	if(__processit__i_am_first__ == true){
		for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
			for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
			#pragma HLS UNROLL
				stats[v][t] = 0;
			}
		}
	}
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPS_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPS_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, vbuffer_source, vbuffer_source, stats, globalcapsule, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONS_CHKPT, globalposition.v_chunkid, globalposition.edgebankID, collections); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE  // CRITICAL REMOVEME URGENT.
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPS_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPS_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, vbuffer_source, vbuffer_source, stats, globalcapsule, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONS_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	// reduce & partition
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPS_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPS_dispatch_reduce(kvdram, vbuffer_source, vbuffer_source, stats, globalcapsule, _globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONS_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	bool __reduceit__i_am_last__ = false;
	if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ __reduceit__i_am_last__ = true; }
	if(__reduceit__i_am_last__ == true){ UTILS_increment_graphiteration(kvdram, globalparamsK); } // NB: this should come last.
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(__reduceit__i_am_last__ == true){ actsutilityobj->printglobalvars2(globalparamsK.ACTSPARAMS_INSTID, "instID", "v_chunkid", "LOP", "sourcep", globalparamsK.ACTSPARAMS_INSTID, globalposition.v_chunkid, globalposition.currentLOP, globalposition.source_partition); }
	#endif 
	#ifdef _DEBUGMODE_STATS_XXX
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void acts_all::TOPS_topkernelP1(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL1_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL1_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL1_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL1_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks1(ON, vdram, vbuffer_source0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs1(vdram, kvdram0, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
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
void acts_all::TOPS_topkernelP2(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL2_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL2_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL2_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL2_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks2(ON, vdram, vbuffer_source0,vbuffer_source1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs2(vdram, kvdram0,kvdram1, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
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
void acts_all::TOPS_topkernelP3(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL3_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL3_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL3_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL3_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks3(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs3(vdram, kvdram0,kvdram1,kvdram2, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
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
void acts_all::TOPS_topkernelP4(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL4_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL4_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL4_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL4_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks4(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs4(vdram, kvdram0,kvdram1,kvdram2,kvdram3, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
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
void acts_all::TOPS_topkernelP5(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL5_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL5_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL5_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL5_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks5(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs5(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
		
		MERGES_mergeVs(kvdram4, vdram); 
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
void acts_all::TOPS_topkernelP6(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
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
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL6_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL6_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL6_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL6_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks6(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs6(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
		
		MERGES_mergeVs(kvdram4, vdram); 
		
		MERGES_mergeVs(kvdram5, vdram); 
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
void acts_all::TOPS_topkernelP7(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
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
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
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
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL7_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL7_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL7_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL7_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks7(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs7(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
		
		MERGES_mergeVs(kvdram4, vdram); 
		
		MERGES_mergeVs(kvdram5, vdram); 
		
		MERGES_mergeVs(kvdram6, vdram); 
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
void acts_all::TOPS_topkernelP8(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
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
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
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
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
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
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL8_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL8_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL8_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL8_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks8(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram7, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs8(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
		
		MERGES_mergeVs(kvdram4, vdram); 
		
		MERGES_mergeVs(kvdram5, vdram); 
		
		MERGES_mergeVs(kvdram6, vdram); 
		
		MERGES_mergeVs(kvdram7, vdram); 
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
void acts_all::TOPS_topkernelP9(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
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
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
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
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
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
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
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
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL9_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL9_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL9_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL9_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks9(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram7, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram8, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs9(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
		
		MERGES_mergeVs(kvdram4, vdram); 
		
		MERGES_mergeVs(kvdram5, vdram); 
		
		MERGES_mergeVs(kvdram6, vdram); 
		
		MERGES_mergeVs(kvdram7, vdram); 
		
		MERGES_mergeVs(kvdram8, vdram); 
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
void acts_all::TOPS_topkernelP10(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
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
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
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
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
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
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
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
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
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
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL10_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL10_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL10_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL10_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks10(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram7, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram8, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram9, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs10(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
		
		MERGES_mergeVs(kvdram4, vdram); 
		
		MERGES_mergeVs(kvdram5, vdram); 
		
		MERGES_mergeVs(kvdram6, vdram); 
		
		MERGES_mergeVs(kvdram7, vdram); 
		
		MERGES_mergeVs(kvdram8, vdram); 
		
		MERGES_mergeVs(kvdram9, vdram); 
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
void acts_all::TOPS_topkernelP11(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
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
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
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
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
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
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
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
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
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
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
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
	unsigned int stats10[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats10
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  collections10[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL11_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL11_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL11_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL11_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks11(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram7, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram8, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram9, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram10, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs11(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, stats10, globalcapsule10, globalposition, globalparamsV, collections10);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
		
		MERGES_mergeVs(kvdram4, vdram); 
		
		MERGES_mergeVs(kvdram5, vdram); 
		
		MERGES_mergeVs(kvdram6, vdram); 
		
		MERGES_mergeVs(kvdram7, vdram); 
		
		MERGES_mergeVs(kvdram8, vdram); 
		
		MERGES_mergeVs(kvdram9, vdram); 
		
		MERGES_mergeVs(kvdram10, vdram); 
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
void acts_all::TOPS_topkernelP12(
	
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
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
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
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
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
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
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
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
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
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
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
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
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
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
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
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
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
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
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
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
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
	unsigned int stats10[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats10
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
	unsigned int stats11[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats11
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

	globalparams_t globalparamsK = UTILS_getglobalparams(kvdram0, 0); 
	for(unsigned int e=0; e<globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; e++){ globalparamsEs[e] = UTILS_getglobalparams(kvdram0, e+1); }
	globalparams_t globalparamsV = UTILS_getglobalparams(vdram, 0);
	if(globalparamsV.ALGORITHMINFO_GRAPHITERATIONID > globalparamsK.ALGORITHMINFO_GRAPHITERATIONID){ globalparamsK.ALGORITHMINFO_GRAPHITERATIONID = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID; }	
	
	for(unsigned int t=0; t<COLLECTIONS_BUFFERSZ; t++){  collections0[t].data1 = 0;  collections1[t].data1 = 0;  collections2[t].data1 = 0;  collections3[t].data1 = 0;  collections4[t].data1 = 0;  collections5[t].data1 = 0;  collections6[t].data1 = 0;  collections7[t].data1 = 0;  collections8[t].data1 = 0;  collections9[t].data1 = 0;  collections10[t].data1 = 0;  collections11[t].data1 = 0;  }
	
	/* //////////////////////////////////////////////////////////////////////////////
	for(unsigned int i=0; i<4; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			keyvalue_t kv = kvdram0[globalparamsK.BASEOFFSETKVS_STATSDRAM + i].data[v];
			cout<<"-------------- iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<": kv["<<i<<"]["<<v<<"].key: "<<kv.key<<", kv["<<i<<"]["<<v<<"].value: "<<kv.value<<endl;
		}
	}
	////////////////////////////////////////////////////////////////////////////// */
	// return;
	
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
	
	unsigned int voffset_kvs = 0;
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
	unsigned int total_num_active_edgeblocks_in_iteration = 0;

	for(unsigned int i=0; i<BLOCKRAM_CURRPMASK_SIZE; i++){ 
	#pragma HLS PIPELINE II=1
		pmask_curr[i] = 0;
		emask_curr[i] = 0;
	}
	
	UTILS_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILS_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int num_stages = 3; 
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvvs = (globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXSZ_EDGEBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP, it_size);
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
				if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP, num_stages); }
				
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + globalparamsK.ACTSPARAMS_NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				unsigned int vsz_kvs = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // 341
				
				TOPKERNEL12_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print3("### TOPKERNEL12_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					if(stage == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP, FIRSTLOP + NUMLOPs); }
					
					if(stage==0){ num_source_partitions = UTILS_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONS_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONS_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL12_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						if(stage == 0 && source_partition == 0){ UTILS_SetFirstData(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP, LASTSOURCEPARTITIONS); }
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL12_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSS_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
						}
						
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
						unsigned int num_active_edgeblocks_in_partition = pmask_curr[source_partition % BLOCKRAM_CURRPMASK_SIZE];
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(num_active_edgeblocks_in_partition > 0){ cout<<"topkernelP: partition is active (pmask_curr["<<source_partition<<"]: "<<num_active_edgeblocks_in_partition<<", source_partition: "<<source_partition<<")...."<<endl; } 
						#endif 
						if(globalposition.EN_PROCESS == ON){ if(num_active_edgeblocks_in_partition > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }} 
						else { enable_readandreplicatevdata = OFF; }
						globalposition.num_active_edgeblocks = num_active_edgeblocks_in_partition;
						total_num_active_edgeblocks_in_iteration += num_active_edgeblocks_in_partition;		
						#ifdef _DEBUGMODE_KERNELPRINTS
						if(globalposition.EN_PROCESS == ON && GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){
							if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<". Hybrid Feature switched ON."<<endl; }
							else { cout<<">>> topkernelP:: number of active vertices in partition: "<<source_partition<<": "<<num_active_edgeblocks_in_partition<<" Hybrid Feature OFF."<<endl; }
						}
						#endif
						
						bool_type enablepartition = OFF;
						if(globalposition.EN_PARTITION == ON){ enablepartition = ON; } else { enablepartition = OFF; }
						
						bool_type enablereduce = OFF; 
						if(globalposition.EN_REDUCE == ON){ enablereduce = ON; } else { enablereduce = OFF; }
						
						// read vertices & vmasks // NEWCHANGE. // CRITICAL REMOVEME URGENT.
						#ifdef CONFIG_RELEASE_VERSION3		
						if(enable_readandreplicatevdata == ON){
							MEMACCESSS_readANDRVchunks12(ON, vdram, vbuffer_source0,vbuffer_source1,vbuffer_source2,vbuffer_source3,vbuffer_source4,vbuffer_source5,vbuffer_source6,vbuffer_source7,vbuffer_source8,vbuffer_source9,vbuffer_source10,vbuffer_source11, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, voffset_kvs, vsz_kvs, globalposition, globalparamsV);	
						}
						#endif	
						#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
						unsigned int depth_i = 0, index = 0, cmd = 0;
						for(unsigned int s=0; s<NUM_PEs; s++){	
							unsigned int offset_kvs = voffset_kvs + depth_i;
							if(s==NUM_PEs-1){ cmd = 1; } else { cmd = 0; }
							#ifdef ___NOT_USED___
		
							MERGES_broadcastVs(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram7, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram8, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram9, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram10, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							MERGES_broadcastVs(vdram, kvdram11, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs);		
		
							#endif 
							MERGES_broadcastVs12(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, 
								globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + index, vsz_kvs,
									globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS, maxsz_actvvs,
										cmd, globalposition, globalparamsK, globalparamsV);		
							depth_i += depth;
							index += vsz_kvs;
						}
						#endif
						
						// acts
						#ifdef CONFIG_RELEASE_VERSION5
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer_source1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer_source2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer_source3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer_source4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer_source5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer_source6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer_source7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer_source8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer_source9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer_source10, stats10, globalcapsule10, globalposition, globalparamsV, collections10);		
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer_source11, stats11, globalcapsule11, globalposition, globalparamsV, collections11);		
	
						#else 
						TOPS_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer_source0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
						#endif 
						
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; } // NEWCHANGE
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += globalparamsK.ACTSPARAMS_NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON && (source_partition % VDATA_SHRINK_RATIO == 0)){ voffset_kvs += vsz_kvs; }
						
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP 23", voffset_kvs, globalparamsV.SIZE_SRCVERTICESDATA, vsz_kvs, 0, NAp);
						#endif
					} // source_partition
				} // currentLOP
			} // stage
		} // v_chunkid
	} // edgebankID		
	
	UTILS_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef CONFIG_RELEASE_VERSION2
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
		
		MERGES_mergeVs(kvdram0, vdram); 
		
		MERGES_mergeVs(kvdram1, vdram); 
		
		MERGES_mergeVs(kvdram2, vdram); 
		
		MERGES_mergeVs(kvdram3, vdram); 
		
		MERGES_mergeVs(kvdram4, vdram); 
		
		MERGES_mergeVs(kvdram5, vdram); 
		
		MERGES_mergeVs(kvdram6, vdram); 
		
		MERGES_mergeVs(kvdram7, vdram); 
		
		MERGES_mergeVs(kvdram8, vdram); 
		
		MERGES_mergeVs(kvdram9, vdram); 
		
		MERGES_mergeVs(kvdram10, vdram); 
		
		MERGES_mergeVs(kvdram11, vdram); 
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
	

	

