#include "acts_all.h"

#ifdef SW
unsigned int globalvar_actsinstid;
unsigned int globalvar_dramreadstats[64][2];
unsigned int globalvar_dramwritestats[64][2];
#endif 

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

void acts_all::UTILP1_writetoglobalvar(bool write){
	#ifdef SW____NOTUSED
	// if(write == true){ globalvar_dramwritestats[globalvar_actsinstid][0] += 1; }
	// else { globalvar_dramreadstats[globalvar_actsinstid][0] += 1; } 
	
	if(write == true){ globalvar_dramwritestats[globalvar_actsinstid][0] += VECTOR2_SIZE; }
	else { globalvar_dramreadstats[globalvar_actsinstid][0] += VECTOR2_SIZE; } 
	#endif
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
	
	unsigned int W = EDGEDATA_PACKINGSIZE * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % EDGEDATA_PACKINGSIZE;
	unsigned int lvid = (y * EDGEDATA_PACKINGSIZE) + x;
	return lvid;
}
unsigned int acts_all::UTILP1_GETREALVID(unsigned int lvid, unsigned int instid){ 
	#pragma HLS INLINE
	
	unsigned int W = EDGEDATA_PACKINGSIZE * NUM_PEs;
	unsigned int y2 = lvid / EDGEDATA_PACKINGSIZE;
	unsigned int x2 = lvid % EDGEDATA_PACKINGSIZE;		
	unsigned int vid = (y2 * W) + (instid * EDGEDATA_PACKINGSIZE) + x2;
	return vid;
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
	
void acts_all::UTILP1_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[EDGEDATA_PACKINGSIZE]){  
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(0);
	#endif 
	return;
}
void acts_all::UTILP1_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){  
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(0);
	#endif
	return;
}	
uint512_evec_dt acts_all::UTILP1_ReadEdges(uint512_dt * kvdram, unsigned int offset_kvs){  
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(0);
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(0);
	#endif
	return;
}
void acts_all::UTILP1_ReadDatas(uint512_dt * kvdram, unsigned int offset_kvs, keyvalue_t datas[VECTOR_SIZE]){
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(0);
	#endif
	return;
}
void acts_all::UTILP1_WriteDatas(uint512_dt * kvdram, unsigned int offset_kvs, value_t datas[VECTOR2_SIZE]){
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(1);
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(1);
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(1);
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
		
	// if(index % 2 == 0){ data = kvdram[offset_kvs + (index / VECTOR2_SIZE)].data[index % VECTOR2_SIZE].key; } 
	// else { data = kvdram[offset_kvs + (index / VECTOR2_SIZE)].data[index % VECTOR2_SIZE].value; }	

	unsigned int loc = index / 2;
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("UTILP1_ReadData 21", offset_kvs + (loc / VECTOR_SIZE), (((1 << 28) / 4) / 16), NAp, MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp);
	#endif
	if(index % 2 == 0){ data = kvdram[offset_kvs + (loc / VECTOR_SIZE)].data[loc % VECTOR_SIZE].key; } 
	else { data = kvdram[offset_kvs + (loc / VECTOR_SIZE)].data[loc % VECTOR_SIZE].value; }	
	
	#endif 
	
	#ifdef SW
	actsutilityobj->checkoutofbounds("UTILP1_ReadData 22", globalvar_actsinstid, 64, NAp, MESSAGES_ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp);
	UTILP1_writetoglobalvar(0);
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(1);
	#endif
	return;
}
keyvalue_t acts_all::UTILP1_ReadKV(uint512_dt * kvdram, unsigned int offset_kvs, unsigned int index){
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
	
	#ifdef SW
	UTILP1_writetoglobalvar(0);
	#endif
	return data;
}

bool acts_all::UTILP1__processit__i_am_first__(globalposition_t globalposition){
	#pragma HLS INLINE
	bool __i_am_first__ = false; 
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ __i_am_first__ = true;; } else { __i_am_first__ = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS4
	// if(__i_am_first__ == true){ cout<<"--------------------------------actsutil: __i_am_first__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	#endif
	return __i_am_first__;
}
bool acts_all::UTILP1__processit__i_am_last__(globalposition_t globalposition){
	#pragma HLS INLINE
	bool __i_am_last__ = false;
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ __i_am_last__ = true; } else { __i_am_last__ = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS4
	// if(__i_am_last__ == true){ cout<<"-------------------------------- actsutil: __i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	#endif
	return __i_am_last__;
}
bool acts_all::UTILP1__reduceit__i_am_last__(globalposition_t globalposition, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS INLINE
	bool __i_am_last__ = false;
	if(globalposition.stage==2 && sweepparams.source_partition == globalparams.NUM_REDUCEPARTITIONS - 1){ __i_am_last__ = true; } else { __i_am_last__ = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS4
	// if(__i_am_last__ == true){ cout<<"-------------------------------- actsutil: __i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	#endif
	return __i_am_last__;
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
	GETGLOBALPARAMS_LOOP1: for(unsigned int t=0; t<256; t++){
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
	globalparams.BASEOFFSETKVS_EDGES0DATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGES0DATA];
	globalparams.BASEOFFSETKVS_EDGESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_EDGESDATA];
	globalparams.BASEOFFSETKVS_VERTEXPTR = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_VERTEXPTR];
	globalparams.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA];
	globalparams.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA];
	globalparams.BASEOFFSETKVS_ACTIVEUPROPBLOCKS = globalparams.DRAM_BASE_KVS + buffer[MESSAGES_BASEOFFSETKVS_ACTIVEUPROPBLOCKS];
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
	globalparams.SIZE_EDGES0 = buffer[MESSAGES_SIZE_EDGES0];
	globalparams.SIZE_EDGES = buffer[MESSAGES_SIZE_EDGES];
	globalparams.SIZE_VERTEXPTRS = buffer[MESSAGES_SIZE_VERTEXPTRS];
	globalparams.SIZE_SRCVERTICESDATA = buffer[MESSAGES_SIZE_SRCVERTICESDATA];
	globalparams.SIZE_DESTVERTICESDATA = buffer[MESSAGES_SIZE_DESTVERTICESDATA];
	globalparams.SIZE_ACTIVEUPROPBLOCKS = buffer[MESSAGES_SIZE_ACTIVEUPROPBLOCKS];
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
	globalparams.POW_EDGES0 = buffer[MESSAGES_POW_EDGES0];
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
		
		#ifdef SW
		UTILP1_writetoglobalvar(1);
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
		
		#ifdef SW
		UTILP1_writetoglobalvar(1);
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
		
		#ifdef SW
		UTILP1_writetoglobalvar(1);
		#endif
	}
	return;
}
void acts_all::UTILP1_increment_graphiteration(uint512_dt * kvdram, globalparams_t globalparams){
	#ifdef _WIDEWORD
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#else
	kvdram[globalparams.DRAM_BASE_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparams.ALGORITHMINFO_GRAPHITERATIONID + 1;
	#endif 
	
	#ifdef SW
	UTILP1_writetoglobalvar(1);
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
bool acts_all::UTILP1_isbufferused(unsigned int id){
	if(id==0 || id==NUMCOMPUTEUNITS_SLR2 || id==NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1){ return true; } else { return false; } ///////////////
	return true;
}
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
	#ifdef ALGOFUNCS_ENABLE_ALLALGORITHMS
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
	#else 
	return process_func_pr(udata, edgew);	
	#endif 
}

value_t reduce_func(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	#pragma HLS INLINE 
	#ifdef ALGOFUNCS_ENABLE_ALLALGORITHMS
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
	#else 
	return reduce_func_pr(vtemp, vdata, res, GraphIter);
	#endif 
}



	
#endif 
#ifdef CONFIG_ENABLECLASS_MEM_ACCESS
void acts_all::MEMACCESSP1_read(bool_type enable, uint512_dt * kvdram, unsigned int buffer[VECTOR2_SIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; } 

	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		UTILP1_ReadDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
		
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){
		#pragma HLS UNROLL 
			buffer[v][bufferoffset_kvs + i] = datas[v];
		}
	}
	return;
}

void acts_all::MEMACCESSP1_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VECTOR2_SIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs){
	if(enable == OFF){ return; } 

	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		UTILP1_ReadDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
		
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){
		#pragma HLS UNROLL 
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP1_readV:: ERROR 21d", bufferoffset_kvs + i, MAX_BLOCKRAM_VDESTDATA_SIZE, bufferoffset_kvs, size_kvs, NAp);
			#endif
			buffer[v][bufferoffset_kvs + i].data = datas[v];
		}
	}
	return;
}

// -------------------- workload -------------------- //

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
	
keyvaluemask_t acts_all::process_edge(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t edge_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparams){				
	#pragma HLS INLINE
	
	// flag 
	// if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){ loc = 0; enx = false; }
	// bool special_loc = false; if(loc == 16383){ loc = 0; special_loc = true; } // header information
	#ifdef _DEBUGMODE_CHECKS3
	// if(enx == true && loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && loc != 16383){ cout<<"processvector::ERROR SEEN @@ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<", mode: "<<mode<<". EXITING... "<<endl; exit(EXIT_FAILURE); }					
	if(enx == true && loc >= MAX_BLOCKRAM_VDESTDATA_SIZE && loc != 16383){ cout<<"processvector::ERROR SEEN @@ loc("<<loc<<") >= MAX_BLOCKRAM_VDESTDATA_SIZE("<<MAX_BLOCKRAM_VDESTDATA_SIZE<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<", mode: "<<mode<<". EXITING... "<<endl; exit(EXIT_FAILURE); }					
	#endif 
	
	// read
	// value_t combo = vbuffer[loc].data;
	value_t combo = 0; if(enx == true && loc != 16383){ combo = vbuffer[loc].data; }
	value_t mask; if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { 
		#ifdef CONFIG_PRELOADEDVERTEXMASKS
		unsigned int mask_set = combo & 0xFFFF; mask = (mask_set >> globalparams.ALGORITHMINFO_GRAPHITERATIONID) & 0x1;
		#else 
		mask = combo & 0x1; 	
		#endif 
	}
	
	#ifdef CONFIG_PRELOADEDVERTEXMASKS
	value_t udata = combo >> 0xFFFF;
	#else 
	value_t udata = combo >> 1;	
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mask == 1 && mode == ACTSPROCESSMODE){ cout<<">>> PROCESS VECTOR:: PROCESS EDGE SEEN @ v: "<<v<<", loc: "<<loc<<", edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", udata: "<<udata<<", mask: "<<mask<<", srcvid: "<<(edge_data.value * EDGEDATA_PACKINGSIZE) + v<<", dstvid*: "<<UTILP1_GETREALVID(edge_data.key, globalparams.ACTSPARAMS_INSTID)<<", ldstvid: "<<edge_data.key<<endl; }
	#endif
	
	// cout<<">>> PROCESS VECTOR:: PROCESS EDGE SEEN @ v: "<<v<<", loc: "<<loc<<", edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", udata: "<<udata<<", mask: "<<mask<<", srcvid: "<<(edge_data.value * EDGEDATA_PACKINGSIZE) + v<<", dstvid*: "<<UTILP1_GETREALVID(edge_data.key, globalparams.ACTSPARAMS_INSTID)<<", ldstvid: "<<edge_data.key<<endl;
	
	// process
	keyvaluemask_t vupdate; 
	if(mode == ACTSPROCESSMODE){
		value_t res = process_func(udata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		vupdate; if(mask == 1 && loc != 16383){ vupdate.key = edge_data.key; vupdate.value = res; vupdate.mask = 1; } else { vupdate.key = INVALIDDATA; vupdate.value = INVALIDDATA; vupdate.mask = 0; }
	} else {
		vupdate.key = combo; vupdate.value = combo;
	}
	return vupdate;
}

unsigned int acts_all::MEMACCESSP1_process_and_buffer(unsigned int chunk_id, unsigned int mode, unsigned int llp_set, uint512_dt *in, uint512_evec_dt out[BLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK){
	keyvaluemask_t res[UPDATEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res complete
	keyvalue_t res_in[UPDATEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res_in complete 
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
	bool en_set = true;
	unsigned int transfsz_kvs = 0;
	
	for(int v = 0; v < VECTOR2_SIZE; v++){ en[v] = true; }
	unsigned int offsetkvs_dstvid = llp_set * (globalparamsK.SIZEKVS2_REDUCEPARTITION * EDGEDATA_PACKINGSIZE); 
	
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; 
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock;	
	unsigned int offsetkvs_stats = llp_set * numkvs_vertices_per_updateblock;	
	
	#ifdef MEMACCESS_ENABLE_USEHLSSTREAM
	unsigned int sz = workload_kvs.size;
	#else 
	unsigned int sz = BLOCKRAM_SIZE;	
	if((chunk_id+1) * BLOCKRAM_SIZE >= workload_kvs.size){ sz = workload_kvs.size - (chunk_id * BLOCKRAM_SIZE); }
	#endif 
	
	unsigned int mode_ = ACTSPROCESSMODE;

	// cout<<"-------------------- _process_and_buffer [iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<"]:: sz: "<<sz<<endl;
	ACTIT_COMPUTEANDSTORE_MAINLOOP2A: for (int i = 0; i < sz; i++){
	#pragma HLS PIPELINE II=1
		// read
		UTILP1_ReadDatas(in, workload_kvs.offset_srcbase + (chunk_id * BLOCKRAM_SIZE) + i, datas);
		
		// decode 
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
		
		// process
		for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL
			res[v] = process_edge(mode_, en[v], v, index[v], data[v], vbuffer_source[v], globalparamsK);
		}	
		
		// prepare 
		for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL
			res_in[v].key = res[v].key; res_in[v].value = res[v].value;
		}
		
		// encode 
		// rotateby >>>
		unsigned int rotateby = 0; unsigned int sample_key = INVALIDDATA; unsigned int sample_u = 0; unsigned int rotate_forward = 1; 
		if(edges[0].value == 16383){ rotateby = edges[0].key >> 1; rotate_forward = edges[0].key & 0x1; } 
		else { sample_key = edges[0].key % UPDATEDATA_PACKINGSIZE; sample_u = 0; if(sample_key > sample_u){ rotateby = sample_key - sample_u; rotate_forward = 0; } else { rotateby = sample_u - sample_key; rotate_forward = 1; }}		
		
		#ifdef MEMACCESS_ENABLE_CYCLICSHIFTS
		if(rotate_forward == 0){ rearrangeLayoutVx16B(rotateby, res_in, res_out); } else{ rearrangeLayoutVx16F(rotateby, res_in, res_out); }
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 59::", rotateby, EDGEDATA_PACKINGSIZE, sample_key, edges[0].value, datas[0]);	// sample_key, edges[0].value, datas[0], sample_u, edges[0].key
		actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 60::", rotate_forward, 2, sample_key, sample_u, edges[0].key);	
		check_if_contiguous(res_out, edges, res_in, sample_key, rotate_forward, rotateby);
		#endif 
		#else 
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				res_out[v] = res_in[v]; 
			}
		#endif 

		for(int v = 0; v < VECTOR2_SIZE; v++){
		#pragma HLS UNROLL
			if(res_out[v].key != INVALIDDATA){ datas2[v] = (res_out[v].value << DSTVID_BITSZ) | res_out[v].key; } else { datas2[v] = INVALIDDATA; }
		}
		
		// store?
		en_set = true;
		if(mode_ == ACTSPROCESSMODE){
			if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){
				if(res[0].mask == 0 && res[1].mask == 0 && res[2].mask == 0 && res[3].mask == 0 && res[4].mask == 0 && res[5].mask == 0 && res[6].mask == 0 && res[7].mask == 0 
					&& res[8].mask == 0 && res[9].mask == 0 && res[10].mask == 0 && res[11].mask == 0 && res[12].mask == 0 && res[13].mask == 0 && res[14].mask == 0 && res[15].mask == 0){
					en_set = false;
				}
			}
		} 
		
		// store
		en_set = true; // REMOVEME.
		if(en_set == true){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				out[i].data[v] = datas2[v];
			}
			transfsz_kvs += 1;
		}
		
		// collect stats
		if(mode_ == ACTSPROCESSMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				unsigned int loc = (edges[v].key / VDATA_PACKINGSIZE) / (((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE);
				#ifdef _DEBUGMODE_CHECKS3
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112)::DEBUG CODE 112::1", loc, numkvs_updateblocks_per_reducepartition, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112b)::DEBUG CODE 112::1", loc, BLOCKRAM_UPDATEBLOCK_SIZE, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				#endif	
				if(res[v].key != INVALIDDATA){ 	
					// cout<<">>>>> _process_and_buffer: stats["<<v<<"]["<<offsetkvs_stats + loc<<"] = 0xFFFFFFFF"<<endl; 
					stats[v][offsetkvs_stats + loc] = 0xFFFFFFFF; } // = vertexid / number of vertices in an update block
			}
		}
	}
	
	return transfsz_kvs;
}

void acts_all::store(unsigned int chunk_id, uint512_dt *out, uint512_evec_dt in[VDATA_PACKINGSIZE], workload_t workload_kvs, unsigned int offsetkvs, unsigned int sizekvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK){
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	
	Store_Ld2: for (int i = 0; i < sizekvs; i++){ 
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){
		#pragma HLS UNROLL 
			datas[v] = in[i].data[v];
		}
		UTILP1_WriteDatas(out, workload_kvs.offset_dstbase + offsetkvs + i, datas);
	}
	
}
 
unsigned int acts_all::MEMACCESSP1_write__process_base(unsigned int mode, unsigned int llp_set, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK){
	#pragma HLS INLINE OFF
	uint512_evec_dt c1[BLOCKRAM_SIZE]; 
	
	unsigned int offsetkvs = 0, transfsz_kvs = 0;
	
	for (int chunk_id = 0; chunk_id < (workload_kvs.size + (BLOCKRAM_SIZE - 1)) / BLOCKRAM_SIZE; chunk_id++){
		unsigned int sizekvs = MEMACCESSP1_process_and_buffer(chunk_id, mode, llp_set, kvdram, c1, vbuffer_source, stats, workload_kvs, collections, globalparamsK);
		// cout<<"---------- _write__process_base: chunk_id: "<<chunk_id<<": sizekvs ("<<sizekvs<<"), of BLOCKRAM_SIZE ("<<BLOCKRAM_SIZE<<"). workload_kvs.size: "<<workload_kvs.size<<endl;
		
		store(chunk_id, kvdram, c1, workload_kvs, offsetkvs, sizekvs, collections, globalparamsK);
		offsetkvs += sizekvs;
		transfsz_kvs += sizekvs;
	}
	return transfsz_kvs;
}
 
void acts_all::reduce_update(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t update_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int stats[BLOCKRAM_SIZE], unsigned int memory[1], globalparams_t globalparams){
	#pragma HLS INLINE

	// flag 
	bool en = true;
	
	// checks
	#ifndef MEMACCESS_ENABLE_ATOMICREDUCE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION){ loc = 0; } // REMOVEME
	#endif 
	#ifdef _DEBUGMODE_CHECKS3
	if(enx == true && loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE)){ cout<<"reduce_update::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). update_data.key: "<<update_data.key<<", update_data.value: "<<update_data.value<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	#ifdef MEMACCESS_ENABLE_ATOMICREDUCE
	if(enx == true && mode == ACTSREDUCEMODE){ if(loc == memory[0]){ loc = (loc + 1) % 8; } memory[0] = loc; } // CRITICAL FIXME.
	#endif 
	#ifdef _DEBUGMODE_CHECKS3
	if(enx == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 113::1", loc, MAX_BLOCKRAM_VDESTDATA_SIZE, update_data.key, update_data.value, mode); }
	#endif
	
	// read & reduce 
	value_t new_combo; value_t mask; value_t vdata_tmp; value_t new_vprop;
	if(mode == ACTSREDUCEMODE){ 
		value_t combo = 0;
		#ifdef MEMACCESS_ENABLE_ATOMICREDUCE
		if(enx == true){ combo = vbuffer[loc].data; }
		#else 
		if(enx == true){ combo = loc & 0x10; } // CRITICAL FIXME.
		#endif 
		if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { 
			#ifdef CONFIG_PRELOADEDVERTEXMASKS
			unsigned int mask_set = combo & 0xFFFF; mask = (mask_set >> globalparams.ALGORITHMINFO_GRAPHITERATIONID) & 0x1;
			#else 
			mask = combo & 0x1; 	
			#endif 
		}
		
		#ifdef CONFIG_PRELOADEDVERTEXMASKS
		vdata_tmp = combo >> 0xFFFF; 
		#else 
		vdata_tmp = combo >> 1; 	
		#endif 
		
		new_vprop = reduce_func(vdata_tmp, vdata_tmp, update_data.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		#ifdef CONFIG_PRELOADEDVERTEXMASKS
		new_combo = (new_vprop << 0xFFFF) | 0xFFFF; // ????
		#else 
		new_combo = (new_vprop << 1) | 0x1;
		#endif 
		if(enx == true && new_vprop != vdata_tmp){ en = true; } else { en = false; }
	} else { new_combo = update_data.value; }
	
	// write-back
	if(en == true){ // REMOVEME.
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3  // REMOVEME.
		if(mode == ACTSREDUCEMODE){ cout<<">>> REDUCE VECTOR:: REDUCE UPDATE SEEN @: v: "<<v<<", loc: "<<loc<<", vdata_tmp: "<<vdata_tmp<<", mask: "<<mask<<", update_data.key: "<<update_data.key<<", update_data.value: "<<update_data.value<<", new_vprop: "<<new_vprop<<", new combo: "<<((new_vprop << 1) | 0x1)<<", dstvid: "<<UTILP1_GETREALVID(update_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
		#endif
		
		vbuffer[loc].data = new_combo;
	}
	return;
}

void acts_all::MEMACCESSP1_read__reduce(unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
			workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK){
	#pragma HLS INLINE OFF
	
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
	
	// if(workload_kvs.offset_srcbase + workload_kvs.offset_begin + workload_kvs.size > (((1 << 28) / 4) / VECTOR2_SIZE)){ return; }
	for(int v = 0; v < VECTOR2_SIZE; v++){ updates_in[v].key = INVALIDDATA; updates_in[v].value = INVALIDDATA; memory[v][0] = 777; }
	if(mode == READVPROPERTYMODE){ for(int v = 0; v < VECTOR2_SIZE; v++){ updates_in[v].key = 0; updates_in[v].value = 0; }} //  || mode == READDATAMODE
	for(int v = 0; v < VECTOR2_SIZE; v++){ en[v] = true; }
	unsigned int ind = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalparamsK.ACTSPARAMS_INSTID==0){ cout<<"actit(reduce): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][workload_kvs.size: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<(workload_kvs.offset_end - workload_kvs.offset_begin) / globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"] ... "<<endl; }		 // REMOVEME. 2.		
	#endif
	ACTIT_READANDREDUCE_MAINLOOP2A: for(batch_type offset_kvs=workload_kvs.offset_begin; offset_kvs<workload_kvs.offset_begin + workload_kvs.size; offset_kvs++){
	#pragma HLS PIPELINE II=1
	#ifdef MEMACCESS_ENABLE_ATOMICREDUCE
	#pragma HLS dependence variable=vbuffer_dest inter false
	#endif 
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(globalparamsK.ACTSPARAMS_INSTID==0){ cout<<"actit(reduce): processing chunk [offset_kvs: "<<offset_kvs<<"]: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<"]: [workload_kvs.offset_end: "<<workload_kvs.offset_end<<"] ... "<<endl; } // REMOVEME. 2.
		#endif
		
		// read 
		UTILP1_ReadDatas(kvdram, workload_kvs.offset_srcbase + offset_kvs, datas);
		
		// decode
		if(mode == ACTSREDUCEMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(datas[v] != INVALIDDATA){ updates_in[v].key = datas[v] & MASK_DSTVID; updates_in[v].value = (datas[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // 16, 18, srcvid is upper, dstvid is lower 
				else { updates_in[v].key = INVALIDDATA; updates_in[v].value = INVALIDDATA; }
			}
		} else {
			#ifndef MEMACCESS_ENABLE_SEPERATEINTERFACEFORDRAMREADS
			if(mode == READUPROPERTYMODE){
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v]; // & 0xFFFFFFFE; 
				}
			} else if (mode == READVPROPERTYMODE) {
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v] & 0xFFFFFFFE; 
				}
			} else {
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v]; 
				}
			}
			#endif 
		}
		
		// prepare inputs
		if(mode == ACTSREDUCEMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				index[v] = (updates_in[v].key / UPDATEDATA_PACKINGSIZE) % 8192; data[v] = updates_in[v]; if(updates_in[v].key != INVALIDDATA || updates_in[v].key >= 131072){ en[v] = true; } else { en[v] = false; } // NEWCHANGE NOW.
				// index[v] = updates_in[v].key / UPDATEDATA_PACKINGSIZE; data[v] = updates_in[v]; if(updates_in[v].key != INVALIDDATA){ en[v] = true; } else { en[v] = false; }
			}
		} else {
			#ifndef MEMACCESS_ENABLE_SEPERATEINTERFACEFORDRAMREADS
			if(mode == READUPROPERTYMODE){
				for(int v = 0; v < VECTOR2_SIZE; v++){ 
				#pragma HLS UNROLL
					index[v] = workload_kvs.offset_buffer_begin + ind; data[v].value = datas2[v]; en[v] = true;
				}
				ind += NUM_PEs;
			} else { // if (mode == READVPROPERTYMODE) {
				for(int v = 0; v < VECTOR2_SIZE; v++){ 
				#pragma HLS UNROLL 
					index[v] = workload_kvs.offset_buffer_begin + (offset_kvs - workload_kvs.offset_begin); data[v].value = datas2[v]; en[v] = true;
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, v, workload_kvs.offset_buffer_begin, mode); 
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, v, workload_kvs.offset_buffer_begin, offset_kvs); 
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112b::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, workload_kvs.offset_buffer_begin, offset_kvs, workload_kvs.offset_begin); 
					#endif	
				}
			}
			#endif 
		}
		
		// reduce	
		for(int v = 0; v < UPDATEDATA_PACKINGSIZE; v++){ 
		#pragma HLS UNROLL 
			reduce_update(mode, en[v], v, index[v], data[v], vbuffer_dest[v], stats[v], memory[v], globalparamsK);
		}
	}
	// exit(EXIT_SUCCESS);
}

// -------------------- stats -------------------- //

unsigned int acts_all::MEMACCESSP1_get_updateblock_workload(bool en, unsigned int reduce_partition, unsigned int * stats_data, unsigned int * stats_metadata, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE]){
	// #pragma HLS INLINE
	
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
			unsigned int updateblock_id = stats_data[stats_metadata[reduce_partition] + n] - (reduce_partition * numkvs_updateblocks_per_reducepartition); 
			workload_kvs.offset_begin = updateblock_id * numkvs_vertices_per_updateblock; 
			workload_kvs.size = numkvs_vertices_per_updateblock;  
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			xload_kvs[n] = workload_kvs;
			unsigned int buffer_offset = updateblock_id * numkvs_vertices_per_updateblock;
			buffer_offsets[n] = buffer_offset;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_updateblock_workload(112)::DEBUG CODE 112::1", buffer_offset, MAX_BLOCKRAM_VDESTDATA_SIZE, updateblock_id, numkvs_vertices_per_updateblock, reduce_partition); // reduce_partition, numkvs_updateblocks_per_reducepartition
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

unsigned int acts_all::MEMACCESSP1_get_upropblock_workload(bool en, unsigned int process_partition, uint512_dt * dram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int num_active_upropblocks, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE], unsigned int graphiterationid){
	// #pragma HLS INLINE
	
	collection_t collections_tmp[COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable=collections_tmp complete
	unsigned int upropblock_ids[MAXNUM_EDGEBLOCKS_PER_VPARTITION];
	
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21d", num_active_upropblocks, NUM_UPROPBLOCKS_PER_VPARTITION+1, process_partition, NAp, NAp);
	#endif
	
	bool sparse_readu = false; if(num_active_upropblocks < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
	if(en == false || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = false; }
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	unsigned int upropblockoffset_vs = (process_partition * MAXNUM_UPROPBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;	
	#else  
	unsigned int upropblockoffset_vs = ((process_partition * MAXNUMGRAPHITERATIONS * MAXNUM_UPROPBLOCKS_PER_VPARTITION) + (graphiterationid * MAXNUM_UPROPBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
	#endif 
	unsigned int num_its; if(sparse_readu == true){ num_its = num_active_upropblocks; } else { num_its = 1; }
	workload_t workload_kvs;
	unsigned int buffer_offset = 0;
	unsigned int upropblock_id = NAp;
	
	// if(globalparams.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"------------------------- sparse_readu: "<<sparse_readu<<", num_its: "<<num_its<<", mem_access: num_active_upropblocks: "<<num_active_upropblocks<<endl; }
	
	if(sparse_readu == true){
		workload_t wkl_kvs; 
		wkl_kvs.offset_begin = 0; 
		wkl_kvs.size = ((num_its) + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE;
		wkl_kvs.offset_end = wkl_kvs.offset_begin + wkl_kvs.size;
		wkl_kvs.offset_srcbase = globalparams.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + upropblockoffset_vs; 
		wkl_kvs.offset_buffer_begin = 0;
		
		#ifdef MEMACCESS_ENABLE_SEPERATEINTERFACEFORMISCREADSANDWRITES
		for(unsigned int n=0; n<num_its; n++){
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_upropblock_workload(1)", globalparams.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + upropblockoffset_vs, ((1 << 28) / 4) / 16, NAp, NAp, n);
			#endif
			upropblock_ids[n] = UTILP1_ReadData(dram, globalparams.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + upropblockoffset_vs, n); // CRITICAL OPTIMIZEME. use wide word buffer
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21x", upropblock_ids[n], NUM_UPROPBLOCKS_PER_VPARTITION * globalparams.NUM_PROCESSEDGESPARTITIONS, globalparams.BASEOFFSETKVS_ACTIVEUPROPBLOCKS, upropblockoffset_vs, globalparams.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + upropblockoffset_vs + (n/VECTOR2_SIZE));
			#endif 
		}
		#else 
		MEMACCESSP1_read__reduce(READDATAMODE, dram, buffer, stats, wkl_kvs, collections_tmp, globalparams);
		for(unsigned int n=0; n<num_its; n++){ upropblock_ids[n] = buffer[n % VDATA_PACKINGSIZE][n / VDATA_PACKINGSIZE].data; }	
		#endif 
	}
	
	for(unsigned int n=0; n<num_its; n++){
		if(sparse_readu == true){
			upropblock_id = upropblock_ids[n]; // upropblock_id = 0; 
			// if(upropblock_id >= MAXNUM_UPROPBLOCKS_PER_VPARTITION){ cout<<"----------- mem_access:: upropblock_ids["<<n<<"]: "<<upropblock_ids[n]<<", num_its: "<<num_its<<", num_active_upropblocks: "<<num_active_upropblocks<<endl; } // FIXME.
			workload_kvs.offset_begin = (upropblock_id * NUM_VERTICESKVS_PER_UPROPBLOCK) - (process_partition * NUM_UPROPBLOCKS_PER_VPARTITION * NUM_VERTICESKVS_PER_UPROPBLOCK); // NUM_UPROPBLOCKS_PER_VPARTITION
			workload_kvs.size = NUM_VERTICESKVS_PER_UPROPBLOCK; 
			workload_kvs.blockid = upropblock_id;
			buffer_offset = (upropblock_id * NUM_VERTICESKVS_PER_UPROPBLOCK) - (process_partition * NUM_UPROPBLOCKS_PER_VPARTITION * NUM_VERTICESKVS_PER_UPROPBLOCK);
			if(workload_kvs.offset_begin >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){ workload_kvs.offset_begin = 0; } // FIXME.
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21a", upropblock_id, NUM_UPROPBLOCKS_PER_VPARTITION * globalparams.NUM_PROCESSEDGESPARTITIONS, process_partition, n, num_its);
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21b", workload_kvs.offset_begin + workload_kvs.size - 1, globalparams.SIZEKVS2_PROCESSEDGESPARTITION, process_partition, upropblock_id, NUM_UPROPBLOCKS_PER_VPARTITION);
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21c", upropblock_id, NUM_UPROPBLOCKS_PER_VPARTITION * globalparams.NUM_PROCESSEDGESPARTITIONS, process_partition, n, globalparams.ALGORITHMINFO_GRAPHITERATIONID);
			#endif
		} else {
			workload_kvs.offset_begin = 0;
			workload_kvs.size = globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
			workload_kvs.blockid = 0; 
			buffer_offset = 0;
		}
		workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
		xload_kvs[n] = workload_kvs;
		buffer_offsets[n] = buffer_offset;
		#ifdef _DEBUGMODE_KERNELPRINTS//4 // sparse_readu == true
		if(n==0 && false){ if(sparse_readu == true){ cout<<"UB<sparse><num_its:"<<num_its<<">:: "; } else { cout<<"<dense><num_its:"<<num_its<<">:: "; }
		cout<<"<instid:"<<globalparams.ACTSPARAMS_INSTID<<"><v_p:"<<process_partition<<"><n:"<<n<<"><upropblock_id:"<<upropblock_id<<"><num_active_upropblocks:"<<num_active_upropblocks<<">:: ";
		cout<<"[offset_begin: "<<workload_kvs.offset_begin<<", offset_end: "<<workload_kvs.offset_end<<", size: "<<workload_kvs.size<<"] "<<endl; }
		#endif
	}
	return num_its;
}

void acts_all::MEMACCESSP1_get_vertexblock_ids(bool en, unsigned int process_partition, uint512_dt * dram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int vertexblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], collection_t collections_tmp[COLLECTIONS_BUFFERSZ], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE){			
	#pragma HLS INLINE
	
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	
	bool sparse_process = false; if(num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = false; }	

	workload_t wkl_kvs; 
	if(sparse_process == true){
		wkl_kvs.offset_begin = 0; 
		wkl_kvs.size = (num_active_edgeblocks + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE; 
		wkl_kvs.offset_end = wkl_kvs.offset_begin + wkl_kvs.size;
		unsigned int upropblockoffset_vs = ((process_partition * MAXNUMGRAPHITERATIONS * MAXNUM_UPROPBLOCKS_PER_VPARTITION) + (globalparamsK.ALGORITHMINFO_GRAPHITERATIONID * MAXNUM_UPROPBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
		wkl_kvs.offset_srcbase = globalparamsK.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + upropblockoffset_vs; 
		wkl_kvs.offset_buffer_begin = 0;
	} else {
		wkl_kvs.offset_begin = 0; 
		wkl_kvs.size = 2; // 1; 
		wkl_kvs.offset_end = wkl_kvs.offset_begin + wkl_kvs.size;
		wkl_kvs.offset_srcbase = globalparamsE.BASEOFFSETKVS_EDGESMAP + process_partition;
		wkl_kvs.offset_buffer_begin = 0;
	}
	#ifdef MEMACCESS_ENABLE_SEPERATEINTERFACEFORMISCREADSANDWRITES
	MEMACCESSP1_read(ON, dram, vertexblock_ids, wkl_kvs.offset_srcbase, wkl_kvs.offset_begin, wkl_kvs.offset_buffer_begin, wkl_kvs.size);		
		/* if(sparse_process == true && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 9 && process_partition == 4){
			cout<<"----------------- _get_vertexblock_ids: new set. num_active_edgeblocks: "<<num_active_edgeblocks<<", wkl_kvs.size: "<<wkl_kvs.size<<". iter: "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", process_partition: "<<process_partition<<". -------------------------"<<endl;
			for(unsigned int t=0; t<wkl_kvs.size; t++){
				for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
					cout<<"_get_vertexblock_ids: vertexblock_ids["<<v<<"]["<<t<<"]: "<<vertexblock_ids[v][t]<<endl;
				}
			}
		}	 */
	#else 
	MEMACCESSP1_read__reduce(READDATAMODE, dram, buffer, stats, wkl_kvs, collections_tmp, globalparamsE);
		for(unsigned int t=0; t<wkl_kvs.size; t++){
		#pragma HLS PIPELINE II=1 // REMOVEME?
			for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
			#pragma HLS UNROLL 
				vertexblock_ids[v][t] = buffer[v][t].data; 
				cout<<"_get_vertexblock_ids: vertexblock_ids["<<v<<"]["<<t<<"]: "<<vertexblock_ids[v][t]<<endl;
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("_get_vertexblock_ids(1)", vertexblock_ids[v][t], ((1 << 28) / 4) / 16, vertexblock_ids[v][t], v, sparse_process);
				#endif
			}
		}
	#endif
	return;
}

unsigned int acts_all::MEMACCESSP1_get_edgeblock_workload(bool en, uint512_dt * dram, unsigned int process_partition, unsigned int llp_set, unsigned int vertexblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int graphiterationid){
	#pragma HLS INLINE
	
	bool sparse_process = false; if(num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	if(en == false || globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = false; }
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS;
	unsigned int num_its; if(sparse_process == true){ num_its = num_active_edgeblocks; } else { num_its = 1; }
	
	// if(sparse_process == true){
		// cout<<"---------------------------------------------- _get_edgeblock_workload: [iter "<<globalparamsK.ALGORITHMINFO_GRAPHITERATIONID<<", partition: "<<process_partition<<", inst: "<<globalparamsK.ACTSPARAMS_INSTID<<"]:: num_active_edgeblocks: "<<num_active_edgeblocks<<endl;
	// }

	READ_PROCESS_PARTITION_WRITE_GETWORKLOADSTATS_LOOP: for(unsigned int n=0; n<num_its; n++){
		workload_t workload_kvs;
		unsigned int vertexblock_id = NAp;		
		if(sparse_process == true){
			#ifdef MEMACCESS_ENABLE_SPARSEEDGEBLOCKS
			vertexblock_id = vertexblock_ids[n % VECTOR2_SIZE][n / VECTOR2_SIZE];
			// if(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID==14 && n<8){ cout<<"+++ _get_edgeblock_workload:: vertexblock_id: "<<vertexblock_id<<", (of "<<num_active_edgeblocks<<" workloads)"<<endl; }
			if(vertexblock_id >= globalparamsE.SIZE_VERTEXPTRS){ vertexblock_id = 0; } // FIXME.
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(0)", globalparamsE.BASEOFFSETKVS_VERTEXPTR + (vertexblock_id / VECTOR2_SIZE), ((1 << 28) / 4) / 16, vertexblock_id, num_its, n);
			#endif
			workload_kvs.offset_begin = UTILP1_ReadData(dram, globalparamsE.BASEOFFSETKVS_VERTEXPTR, vertexblock_id) / EDGEDATA_PACKINGSIZE; 
			workload_kvs.offset_end = UTILP1_ReadData(dram, globalparamsE.BASEOFFSETKVS_VERTEXPTR, vertexblock_id + 1) / EDGEDATA_PACKINGSIZE; 
			workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;	
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(2)", workload_kvs.offset_begin, ((1 << 28) / 4) / 16, NAp, NAp, n);
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(3)", workload_kvs.size, ((1 << 28) / 4) / 16, NAp, NAp, n);
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"_get_edgeblock_workload/sparse_process:: active edge-block seen @ ["<<globalparamsK.ACTSPARAMS_INSTID<<"]["<<process_partition<<"]["<<llp_set<<"]: edgeblock-id: "<<vertexblock_id<<", edgeblock-map: "<<"["<<workload_kvs.offset_begin<<", "<<workload_kvs.size<<"]"<<endl;
			#endif 
			#endif 
		} else {
			unsigned int index = (process_partition * num_LLPset) + llp_set;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(4)", llp_set + 1, VECTOR2_SIZE, NAp, NAp, n);
			#endif
			workload_kvs.offset_begin = vertexblock_ids[llp_set][0] / EDGEDATA_PACKINGSIZE;
			workload_kvs.offset_end = vertexblock_ids[llp_set + 1][0] / EDGEDATA_PACKINGSIZE;
			workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(5)", workload_kvs.size, ((1 << 28) / 4) / 16, NAp, NAp, n);
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"_get_edgeblock_workload/dense_process:: full edge-block seen @ ["<<globalparamsK.ACTSPARAMS_INSTID<<"]["<<process_partition<<"]: edgeblock-id: NAp, edgeblock-map: "<<"["<<workload_kvs.offset_begin<<", "<<workload_kvs.size<<", "<<workload_kvs.offset_end<<"]"<<endl;
			#endif 
		}
	
		if(workload_kvs.offset_end < workload_kvs.offset_begin){ workload_kvs.size = 0; }
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		if(n==0 && false){ if(sparse_process == true){ cout<<"<sparse><num_its:"<<num_its<<">:: "; } else { cout<<"<dense><num_its:"<<num_its<<">:: "; }
		cout<<"<instid:"<<globalparamsK.ACTSPARAMS_INSTID<<"><v_p:"<<process_partition<<"><llp_set:"<<llp_set<<"><n:"<<n<<"><vertexblock_id:"<<vertexblock_id<<"><num_active_edgeblocks:"<<num_active_edgeblocks<<">:: ";
		cout<<"[offset_begin: "<<workload_kvs.offset_begin<<", offset_end: "<<workload_kvs.offset_end<<", size: "<<workload_kvs.size<<"] "<<endl; }
		#endif
		xload_kvs[n] = workload_kvs;
	}
	return num_its;
}

// ------------------- stats -------------------- //

void acts_all::MEMACCESSP1_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS + 1, NAp, NAp, NAp);
	#endif
	
	keyvalue_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<1 + globalparams.NUM_REDUCEPARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt vec;
		
		// UTILP1_ReadDatas(kvdram, offset_kvs + i, datas); // NEWCHANGE.
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

void acts_all::MEMACCESSP1_readhelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		UTILP1_ReadDatas(vdram, offset_kvs + i, datas);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
		
		pmask[i] = datas[GraphIter];
		// cout<<"_readhelperstats:: pmask["<<offset_kvs + i<<"]["<<i<<"]: "<<pmask[i]<<endl;
	}
	// exit(EXIT_SUCCESS);
	
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

void acts_all::MEMACCESSP1_savehelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		datas[GraphIter] = pmask[i];
		UTILP1_WriteDatas(vdram, offset_kvs + i, datas);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
	}
	return;
}









	
#endif 
#ifdef CONFIG_ENABLECLASS_ACTS_MERGE
void acts_all::mergeP1_trace(uint512_dt * kvdram, unsigned int reduce_partition, unsigned int k, unsigned int offset_kvs, globalparams_t globalparams){
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
		value_t combo; if(v%2==0){ combo = kvdram[offset_kvs].data[v/2].key; } else { combo = kvdram[offset_kvs].data[v/2].key; }
		value_t vdata = combo >> 1; value_t mask = combo & 0x1;
		unsigned int lvid = (((reduce_partition * globalparams.SIZEKVS2_REDUCEPARTITION) + k) * VDATA_PACKINGSIZE) + v;
		#ifdef _DEBUGMODE_KERNELPRINTS4
		if(mask == 1){ cout<<">>> MERGE:: MERGE VERTICES SEEN @ reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<", vid: "<<UTILP1_GETREALVID(lvid, globalparams.ACTSPARAMS_INSTID)<<", combo: "<<combo<<", globalparams.SIZE_DESTVERTICESDATA: "<<globalparams.SIZE_DESTVERTICESDATA<<endl; }	
		#endif 
	}
	#endif 
}

void acts_all::MERGEP1_print_active_masks(uint512_dt * vdram, globalparams_t globalparams, unsigned int offset_kvs){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	for(unsigned int v=0; v<VECTOR_SIZE; v++){	
		unsigned int K = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].key;
		unsigned int V = vdram[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs].data[v].value;
		// if(MEMCAP1_READVMASK(K)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }
		// if(MEMCAP1_READVMASK(V)==1){ cout<<"$$$ EXCHANGE(2):: MASK=1 SEEN. index: "<<globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs<<endl; }	
	}
	#endif 
	return;
}

unsigned int acts_all::MERGEP1_mergeVs(uint512_dt * kvdram, uint512_dt * vdram){
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
	
	unsigned int transfsz_kvs = 0;
	
	globalparams_t globalparams = UTILP1_getglobalparams(kvdram, 0);
	globalparams_t globalparamsv = UTILP1_getglobalparams(vdram, 0);
	
	// #ifdef TESTKERNEL 
	// if(UTILP1_isbufferused(globalparams.ACTSPARAMS_INSTID) == false){ return 0; }
	// #endif 	
	
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
	#ifdef _DEBUGMODE_KERNELPRINTS//4
	for(unsigned int i=0; i<4; i++){ cout<<"merge:: stats[0]["<<i<<"]: "<<stats[0][i]<<", "<<endl; }
	for(unsigned int i=0; i<4; i++){ cout<<"merge:: stats[1]["<<i<<"]: "<<stats[1][i]<<", "<<endl; }
	#endif
	
	MERGEP1_MERGEVSLOOP2: for(unsigned int reduce_partition=0; reduce_partition<globalparams.NUM_REDUCEPARTITIONS; reduce_partition++){
		
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
					stats[0][offset_buff + t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + offset_buff + t].range(31, 0);
					#else 
					stats[0][offset_buff + t] = kvdram[globalparams.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + offset_buff + t].data[0].key;
					#endif 
				}
			}
			// check this. =false works, =true not work?
			num_its = MEMACCESSP1_get_updateblock_workload(true, reduce_partition, stats[0], stats[1], globalparams, xload_kvs, buffer_offsets); 
			// num_its = MEMACCESSP1_get_updateblock_workload(false, reduce_partition, stats[0], stats[1], globalparams, xload_kvs, buffer_offsets); 
		}
		
		MERGEP1_MERGEVSLOOP2B: for(unsigned int n=0; n<num_its; n++){
			// cout<<"---------------------- topkernelP::_mergeVs: n: "<<n<<", num_its: "<<num_its<<", xload_kvs["<<n<<"].offset_begin: "<<xload_kvs[n].offset_begin<<", xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<endl;
			MERGEP1_MERGEVSLOOP2C: for(unsigned int k=xload_kvs[n].offset_begin; k<xload_kvs[n].offset_begin + xload_kvs[n].size; k++){ // globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, globalparams.SIZEKVS2_REDUCEPARTITION
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("mergeVs: ERROR 21", voffset_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, NAp, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("mergeVs: ERROR 22", voffseti_kvs2 + k, ((1 << 28) / 4) / VECTOR2_SIZE, voffseti_kvs2, k, globalparams.SIZE_DESTVERTICESDATA); 
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs2 + k] = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				mergeP1_trace(kvdram, reduce_partition, k, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k, globalparams); total_vertices_merged += VDATA_PACKINGSIZE;
				#endif
			}
			transfsz_kvs += xload_kvs[n].size;
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"mergeVs:: merge operation finished. total_vertices_merged: "<<total_vertices_merged<<endl; 
	#endif
	// exit(EXIT_SUCCESS); // 
	return transfsz_kvs;
}

void acts_all::MERGEP1_broadcastVs(uint512_dt * vdram, uint512_dt * kvdram, unsigned int offset1_kvs, unsigned int offset2_kvs, unsigned int size_kvs){
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){
	#pragma HLS PIPELINE II=1
		kvdram[offset2_kvs + k] = vdram[offset1_kvs + k];
	}
	return;
}

void acts_all::MERGEP1_broadcastVs1(uint512_dt * vdram, uint512_dt * kvdram0, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs2(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs3(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs4(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs5(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram4, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs6(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram4, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram5, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs7(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram4, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram5, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram6, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs8(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram4, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram5, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram6, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram7, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs9(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram4, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram5, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram6, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram7, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram8, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs10(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram4, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram5, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram6, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram7, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram8, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram9, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs11(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram4, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram5, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram6, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram7, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram8, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram9, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram10, offsetDST_kvs + k, datas);
	}
	return;
}
void acts_all::MERGEP1_broadcastVs12(uint512_dt * vdram, uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, unsigned int offsetSRC_kvs, unsigned int offsetDST_kvs, unsigned int size_kvs){
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	MERGEP1_BROADCASTVS_LOOP: for(unsigned int k=0; k<size_kvs; k++){ // workload_kvs.offset_begin
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 21", offsetSRC_kvs + k, (1 << 26) / 16, offsetSRC_kvs, NAp, k);
		actsutilityobj->checkoutofbounds("_broadcastVs:: ERROR 22", offsetDST_kvs + k, (1 << 26) / 16, offsetDST_kvs, NAp, k);
		#endif
		
		UTILP1_ReadDatas(vdram, offsetSRC_kvs + k, datas);	
		
		UTILP1_WriteDatas(kvdram0, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram1, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram2, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram3, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram4, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram5, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram6, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram7, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram8, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram9, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram10, offsetDST_kvs + k, datas);
		
		UTILP1_WriteDatas(kvdram11, offsetDST_kvs + k, datas);
	}
	return;
}

unsigned int acts_all::MERGEP1_copy(unsigned int idx, unsigned int reduce_partition, uint512_dt * vdramSRC, uint512_dt * vdramDST1, uint512_dt * vdramDST2, uint512_dt * vdramREF, unsigned int stats[2][BLOCKRAM_SIZE], unsigned int voffset_kvs, unsigned int firstinstance_id, unsigned int num_compute_units, 
		pmask_dt vpartition_stats[BLOCKRAM_CURRPMASK_SIZE], wide_word_bits_3t upropblock_stats[MAX_RED_SRAMSZ],
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
	for(unsigned int t=0; t<globalparamsSRC.SIZEKVS2_REDUCEPARTITION; t++){ upropblock_stats[t].data[idx] = 0; } // FIXME. TOO EXPENSIVE.?
	unsigned int lvid_first = reduce_partition * globalparamsSRC.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE; // (((reduce_partition * globalparamsDST1.SIZEKVS2_REDUCEPARTITION) + k) * VDATA_PACKINGSIZE) + v;
	unsigned int vbegin = reduce_partition * globalparamsSRC.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE * NUM_PEs;
	
	EXCHANGEP1_COPY_LOOP1: for(unsigned int i=0; i<num_compute_units; i++){
	
		unsigned int reducepsz_kvs = globalparamsSRC.SIZEKVS2_REDUCEPARTITION; 
		if(globalparamsSRC.NUM_REDUCEPARTITIONS > 1){ if(reduce_partition == globalparamsSRC.NUM_REDUCEPARTITIONS-1){ reducepsz_kvs = (globalparamsSRC.ACTSPARAMS_BATCH_RANGE / VECTOR2_SIZE) % globalparamsSRC.SIZEKVS2_REDUCEPARTITION; } else { reducepsz_kvs = globalparamsSRC.SIZEKVS2_REDUCEPARTITION; }}
		// unsigned int num_its = mergeP1_get_updateblock_workload(reduce_partition, stats[0], stats[1], globalparamsSRC, xload_kvs);
		
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
				
				UTILP1_ReadDatas(vdramSRC, globalparamsSRC.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas);
				UTILP1_WriteDatas(vdramDST1, globalparamsDST1.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas); // NEWCHANGE.
				UTILP1_WriteDatas(vdramDST2, globalparamsDST2.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs + v2offset_kvs + k, datas);
				
				for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
				#pragma HLS UNROLL 
					value_t vdata = datas[v] >> 1; mask[v] = datas[v] & 0x1;
					unsigned int lvid = lvid_first + (k * VDATA_PACKINGSIZE) + v; vid[0] = UTILP1_GETREALVID(lvid, firstinstance_id + i); // FIXME. MAKE SIMPLER. // vid[0] = vbegin + (i * VDATA_PACKINGSIZE) + k;
					#ifdef _DEBUGMODE_CHECKS3
					unsigned int check_lvid = lvid_first + (k * VDATA_PACKINGSIZE) + v; unsigned int check_v0 = UTILP1_GETREALVID(check_lvid, firstinstance_id + i); // FIXME. MAKE SIMPLER.
					if(vid[0] != check_v0){ cout<<"exchange. seperate calculations of vid[0] don't match. vid[0]("<<vid[0]<<") != check_v0("<<check_v0<<"). EXIITING..."<<endl; exit(EXIT_FAILURE); }
					#endif 
					#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
					if(mask[v] == 1){ cout<<">>> EXCHANGE-COPY:: ACTIVE VERTEX SEEN @ [i: "<<i<<", reduce_partition: "<<reduce_partition<<", k: "<<k<<", v: "<<v<<"] vdata: "<<vdata<<", lvid: "<<lvid<<", vid[0]: "<<vid[0]<<endl; }
					#endif 
				}	
				
				if(mask[0]==1 || mask[1]==1 || mask[2]==1 || mask[3]==1 || mask[4]==1 || mask[5]==1 || mask[6]==1 || mask[7]==1 || mask[8]==1 || mask[9]==1 || mask[10]==1 || mask[11]==1 || mask[12]==1 || mask[13]==1 || mask[14]==1 || mask[15]==1){
					if(upropblock_stats[k].data[0] == 0 && upropblock_stats[k].data[1] == 0 && upropblock_stats[k].data[2] == 0){ 
						upropblock_stats[k].data[idx] = 1; 
						unsigned int v_p = vid[0] / (globalparamsSRC.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE);
						vpartition_stats[v_p] += 1;
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						cout<<"+++ EXCHANGE-COPY:: ACTIVE EDGE-BLOCK SEEN @ upropblock_stats["<<firstinstance_id + i<<"]["<<v_p<<"]["<<k<<"]. "<<endl; 
						#endif 
					}
				}
			} // end-of-k
		}
		v2offset_kvs += globalparamsSRC.NUM_REDUCEPARTITIONS * globalparamsSRC.SIZEKVS2_REDUCEPARTITION;
	} // end-of-num_compute_units
	return 0;
}

void acts_all::MERGEP1_exchange(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
	#ifdef _DEBUGMODE_KERNELPRINTS4//3
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
	
	wide_word_bits_3t upropblock_stats[MAX_RED_SRAMSZ];
	#pragma HLS DATA_PACK variable = upropblock_stats
	pmask_dt vpartition_stats[BLOCKRAM_CURRPMASK_SIZE];
	unsigned int statsA[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsA
	unsigned int statsB[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsB
	unsigned int statsC[2][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = statsC

	globalparams_t globalparamsvA = UTILP1_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP1_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP1_getglobalparams(vdramC, 0);
	
	for(unsigned int t=0; t<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; t++){ vpartition_stats[t] = 0; }
	for(unsigned int t=0; t<globalparamsvA.SIZEKVS2_REDUCEPARTITION; t++){ upropblock_stats[t].data[0] = 0; upropblock_stats[t].data[1] = 0; upropblock_stats[t].data[2] = 0; }
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR2 + globalparamsvA.ACTSPARAMS_NUMCOMPUTEUNITS_SLR1) * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
	
	// unsigned int reduce_partition = 0;
	EXCHANGEP1_LOOP1: for(unsigned int reduce_partition=0; reduce_partition<globalparamsvA.NUM_REDUCEPARTITIONS; reduce_partition++){
		total_sync[0] = MERGEP1_copy(0, reduce_partition, vdramA, vdramB, vdramC, vdramA, statsA, voffsetA_kvs, 0, NUMCOMPUTEUNITS_SLR2, vpartition_stats, upropblock_stats, globalparamsvA, globalparamsvB, globalparamsvC);
		total_sync[1] = MERGEP1_copy(1, reduce_partition, vdramB, vdramA, vdramC, vdramA, statsB, voffsetB_kvs, NUMCOMPUTEUNITS_SLR2, NUMCOMPUTEUNITS_SLR1, vpartition_stats, upropblock_stats, globalparamsvB, globalparamsvA, globalparamsvC);
		total_sync[2] = MERGEP1_copy(2, reduce_partition, vdramC, vdramA, vdramB, vdramA, statsC, voffsetC_kvs, NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1, NUMCOMPUTEUNITS_SLR0, vpartition_stats, upropblock_stats, globalparamsvC, globalparamsvA, globalparamsvB);
		
		#ifdef _COLLECT_METADATA_FROM_KERNEL_
		EXCHANGEP1_LOOP1B: for(unsigned int v_p=0; v_p<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v_p++){
			if(vpartition_stats[v_p] < globalparamsvA.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){
				unsigned int offset_vs = (v_p * MAXNUM_EDGEBLOCKS_PER_VPARTITION) / VDATA_PACKINGSIZE; // NEWCHANGE.
				unsigned int begin = v_p * MAXNUM_EDGEBLOCKS_PER_VPARTITION; unsigned int sz = MAXNUM_EDGEBLOCKS_PER_VPARTITION; // unsigned int end = begin + sz;
				if(begin + sz >= globalparamsvA.SIZEKVS2_REDUCEPARTITION){ sz = globalparamsvA.SIZEKVS2_REDUCEPARTITION - begin; }
				unsigned int index = 1; // 0;
				for(unsigned int t=begin; t<begin + sz; t++){
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 24", t, globalparamsvA.SIZEKVS2_REDUCEPARTITION, begin, begin + sz, sz);
					actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 25", t, MAX_RED_SRAMSZ, begin, begin + sz, sz);
					#endif
					if(upropblock_stats[t].data[0] > 0 || upropblock_stats[t].data[1] > 0 || upropblock_stats[t].data[2] > 0){
						UTILP1_WriteData(vdramA, globalparamsvA.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						UTILP1_WriteData(vdramB, globalparamsvB.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						UTILP1_WriteData(vdramC, globalparamsvC.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, index, t-begin);
						index += 1; // upropblock_stats[t] = 0;
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("exchangeVs:: ERROR 26", t-begin, MAXNUM_EDGEBLOCKS_PER_VPARTITION, NAp, NAp, NAp);
						#endif
						// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						cout<<"+++ EXCHANGE:: ACTIVE EDGE-BLOCK SEEN @ ["<<v_p<<"]["<<t<<"] "<<endl; 
						// #endif
					}
				}
				UTILP1_WriteData(vdramA, globalparamsvA.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
				UTILP1_WriteData(vdramB, globalparamsvB.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
				UTILP1_WriteData(vdramC, globalparamsvC.BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset_vs, 0, index-1);
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
	MEMACCESSP1_readhelperstats(vdramA, pmask_curr, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	for(unsigned int t=0; t<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; t++){ sum3 += pmask_curr[t]; if(debug1==true){ cout<<"~~~ pmask_curr["<<t<<"]: "<<pmask_curr[t]<<endl; }}
	if(debug==true){ cout<<"~~~::: sum3: "<<sum3<<endl; }
	cout<<"exchange-copy: sum3,2,3,4: sum1: "<<sum1<<", sum2: "<<sum2<<", sum3: "<<sum3<<", sum4: "<<sum4<<"..."<<endl;
	#endif	
	
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	MEMACCESSP1_savehelperstats(vdramA, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	MEMACCESSP1_savehelperstats(vdramB, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	MEMACCESSP1_savehelperstats(vdramC, vpartition_stats, globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_CURRPMASK_SIZE, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA);
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"finished. "<<total_sync[0] + total_sync[1] + total_sync[2]<<" vertices synchronized"<<endl; 
	#endif
	return;
}

extern "C" {
void acts_all::TOPP1_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
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
	globalparams_t globalparamsvA = UTILP1_getglobalparams(vdramA, 0);
	unsigned int inACTSMODE = UTILP1_GetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"~~~ TOPP1_topkernelS:: GraphIter: "<<globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID<<endl;
	#endif
	
	#ifdef CONFIG_RELEASE_VERSION4
	if(globalparamsvA.ENABLE_EXCHANGECOMMAND == ON){
		MERGEP1_exchange(vdramA, vdramB, vdramC);
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

void acts_all::ACTSP1_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats_kvs[BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], 
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	
	unsigned int memory[VECTOR2_SIZE][4];
	#pragma HLS ARRAY_PARTITION variable = memory
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	stats_t temp_stats[BLOCKRAM_UPDATEBLOCK_SIZE];
	unsigned int metadata_stats[VDATA_PACKINGSIZE];
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	unsigned int xblock_ids[BLOCKRAM_SIZE];
	workload_t xload_kvs[MAXNUM_EDGEBLOCKS_PER_VPARTITION]; // BLOCKRAM_SIZE];
	unsigned int vertexblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vertexblock_ids
	
	collection_t collections_tmp[COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable = collections_tmp complete
	collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID]; // category 1
	collections_tmp[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID];
	collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections[REDUCEPHASE_TRANSFSZ_COLLECTIONID];
	collections_tmp[NUMEDGESPROCESSED_COLLECTIONID] = collections[NUMEDGESPROCESSED_COLLECTIONID]; // category 2
	collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID] = collections[NUMVERTICESPROCESSED_COLLECTIONID];
	collections_tmp[NUMREADSFROMDRAM_COLLECTIONID] = collections[NUMREADSFROMDRAM_COLLECTIONID]; // category 3
	collections_tmp[NUMWRITESTODRAM_COLLECTIONID] = collections[NUMWRITESTODRAM_COLLECTIONID];
	
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
	#ifdef _DEBUGMODE_KERNELPRINTS//4
	if(mode == ACTSPROCESSMODE){ cout<<"actit: v_p: "<<sweepparams.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
	#endif 
	
	unsigned int totalnum_its = 0;
	bool en = true;
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && mode == ACTSPROCESSMODE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active

	if(globalparamsK.ENABLE_RUNKERNELCOMMAND == ON && en == true){
		#ifdef _DEBUGMODE_KERNELPRINTS//
		if(mode == ACTSPROCESSMODE){ cout<<"processit->actit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; }
		#endif 
	
		if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){
			
			MEMACCESSP1_get_vertexblock_ids(ON, globalposition.source_partition, kvdram, vbuffer, vertexblock_ids, stats, collections_tmp, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE);
			
			#ifdef ACTS_ENABLE_READUPROPS
			// cout<<"--- acts:: _get_upropblock_workload 777"<<endl;
			unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, sweepparams.source_partition, kvdram, vbuffer, stats, globalposition.num_active_edgeblocks, globalparamsK, xload_kvs, buffer_offsets, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
			#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
			collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += ((num_its / VECTOR2_SIZE) + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
			collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += ((num_its / VECTOR2_SIZE) + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
			collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += ((num_its / VECTOR2_SIZE) + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
			#endif 
			if(sparse_readu == true){
				cout<<""<<endl;
				for(unsigned int n=0; n<num_its; n++){		
					MEMACCESSP1_readV(ON, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA, xload_kvs[n].offset_begin, xload_kvs[n].offset_begin, xload_kvs[n].size);
					#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
					collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
					collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
					collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
					#endif
				}
			} else {
				MEMACCESSP1_readV(ON, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA, 0, 0, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION);
				#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
				collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
				collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				#endif
			}
			#endif 
			
			#ifdef ACTS_ENABLE_PROCESSEDGES
			unsigned int num_LLPset_; if(sparse_process == true){ num_LLPset_ = 1; } else { num_LLPset_ = num_LLPset; }
			ACTIT_MAINLOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset_; llp_set++){ 
				batch_type destoffset_kvs = (globalcapsule[1 + llp_set].key + globalcapsule[1 + llp_set].value) / EDGEDATA_PACKINGSIZE;
				unsigned int destindex = 0; 
				unsigned int num_its = MEMACCESSP1_get_edgeblock_workload(true, kvdram, sweepparams.source_partition, llp_set, vertexblock_ids, stats, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE, xload_kvs, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
				totalnum_its += num_its;
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("actit(1.2)", totalnum_its, globalparamsE.SIZE_VERTEXPTRS, NAp, NAp, num_its);
				#endif
			
				READ_PROCESS_PARTITION_STORE_LOOP1B: for(unsigned int n=0; n<num_its; n++){
					workload_t workload_kvs = xload_kvs[n];
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.size, (1 << 28) / 8, NAp, NAp, n);
					#endif
					workload_kvs.offset_srcbase = sourcebaseaddr_kvs + workload_kvs.offset_begin;
					workload_kvs.offset_dstbase = destbaseaddr_kvs + destoffset_kvs + destindex;
					workload_kvs.offset_buffer_begin = 0; // NAp;
				
					unsigned int activetransfsz_kvs = MEMACCESSP1_write__process_base(mode, llp_set, kvdram, vbuffer, stats, workload_kvs, collections_tmp, globalparamsK);
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"actit: n:"<<n<<", workload_kvs.size: "<<workload_kvs.size<<", activetransfsz_kvs: "<<activetransfsz_kvs<<endl;
					#endif 
						
					globalcapsule[1 + llp_set].value += workload_kvs.size * EDGEDATA_PACKINGSIZE;
					// globalcapsule[1 + llp_set].value += activetransfsz_kvs * EDGEDATA_PACKINGSIZE; // workload_kvs.size
					destindex += workload_kvs.size;	
					#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
					/// if(sparse_process == true){ collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += ((workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE) * 2) * REPORT__SPARSEPROCESS_SLOWDOWN_FACTOR; } // '* 2' because both read & write happens in processing phase
					/// else { collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE) * 2) }
					collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1 += ((workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE) * 2; 
					collections_tmp[NUMEDGESPROCESSED_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
					collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
					collections_tmp[NUMWRITESTODRAM_COLLECTIONID].data1 += (activetransfsz_kvs + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
					#endif 
				}
			}
			#endif 
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

			unsigned int num_its = MEMACCESSP1_get_updateblock_workload(true, sweepparams.source_partition, stats[16], stats[17], globalparamsK, xload_kvs, buffer_offsets);
			unsigned int voffset_kvs = sweepparams.source_partition * globalparamsK.SIZEKVS2_REDUCEPARTITION;
			for(unsigned int n=0; n<num_its; n++){
				xload_kvs[n].offset_srcbase = NAp; 
				xload_kvs[n].offset_dstbase = globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA;
				xload_kvs[n].offset_buffer_begin = buffer_offsets[n];
				#ifdef MEMACCESS_ENABLE_SEPERATEINTERFACEFORDRAMREADS
					ACTIT_READVDATA_LOOP1: for (buffer_type i=0; i<xload_kvs[n].size; i++){
					#pragma HLS PIPELINE II=1
						UTILP1_ReadDatas(kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs + xload_kvs[n].offset_begin + i, datas);
						for(unsigned int v=0; v<VECTOR2_SIZE; v++){
						#pragma HLS UNROLL 
							vbuffer[v][xload_kvs[n].offset_buffer_begin + i].data = datas[v];
						}
					}
				#else 
				MEMACCESSP1_read__reduce(READVPROPERTYMODE, kvdram, vbuffer, stats, xload_kvs[n], collections_tmp, globalparamsK);
				#endif 
				#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
				collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
				collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				#endif 
			}
		
			workload_kvs.offset_srcbase = sourcebaseaddr_kvs;
			workload_kvs.offset_dstbase = NAp;
			workload_kvs.offset_buffer_begin = NAp;
			MEMACCESSP1_read__reduce(ACTSREDUCEMODE, kvdram, vbuffer, stats, workload_kvs, collections_tmp, globalparamsK);
			#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
			collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
			collections_tmp[NUMEDGESPROCESSED_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
			collections_tmp[NUMREADSFROMDRAM_COLLECTIONID].data1 += (workload_kvs.size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
			#endif 
			
			for(unsigned int n=0; n<num_its; n++){
				xload_kvs[n].offset_srcbase = NAp; 
				xload_kvs[n].offset_dstbase = globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA;
				xload_kvs[n].offset_buffer_begin = buffer_offsets[n];
				#ifdef MEMACCESS_ENABLE_SEPERATEINTERFACEFORDRAMWRITES
					ACTIT_READVDATA_LOOP2: for (buffer_type i=0; i<xload_kvs[n].size; i++){
					#pragma HLS PIPELINE II=1
						for(unsigned int v=0; v<VECTOR2_SIZE; v++){
						#pragma HLS UNROLL 
							datas[v] = vbuffer[v][xload_kvs[n].offset_buffer_begin + i].data;
						}
						UTILP1_WriteDatas(kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs + xload_kvs[n].offset_begin + i, datas);
					}
				#else 
				unsigned int transfsz_kvs = MEMACCESSP1_write__process_base(SAVEVPROPERTYMODE, NAp, kvdram, vbuffer, stats, xload_kvs[n], collections_tmp, globalparamsK);
				#endif 
				#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
				collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE;
				collections_tmp[NUMWRITESTODRAM_COLLECTIONID].data1 += (xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY) * VECTOR2_SIZE; 
				#endif 
			}
		}
		#endif 
	}
	
	// prepare stats
	#ifdef ACTS_ENABLE_COLLECTSTATS
	if(mode == ACTSPROCESSMODE && UTILP1__processit__i_am_last__(globalposition) == true){
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
			if(stat > 0){
				stats[16][index] = t; 
				// cout<<">>>>> actit: t: "<<t<<", llp_set: "<<llp_set<<", numkvs_vertices_per_updateblock: "<<numkvs_vertices_per_updateblock<<", numkvs_updateblocks_per_reducepartition: "<<numkvs_updateblocks_per_reducepartition<<endl; 
				// vbuffer[index % VECTOR2_SIZE][1 + (index / VECTOR2_SIZE)].data = t; 
				stats_tmp[llp_set] += 1; 
				index += 1; 
			}
		}
		// for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){ cout<<"--------------- actit: stats_tmp["<<llp_set<<"]: "<<stats_tmp[llp_set]<<endl; }
		stats[17][0] = 0; for(unsigned int i=1; i<VECTOR2_SIZE; i++){ stats[17][i] = stats[17][i-1] + stats_tmp[i-1];  } // vbuffer[i][0].data = stats[17][i];// calculate offsets
		// for(unsigned int llp_set=0; llp_set<num_LLPset+1; llp_set++){ cout<<"&&&&&&&&&&&&&&& actit: stats[17]["<<llp_set<<"]: "<<stats[17][llp_set]<<endl; }
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit:: stats_tmp["<<0<<"]: "<<stats_tmp[0]<<endl;
		for(unsigned int i=0; i<4; i++){ cout<<"actit:: stats[16]["<<i<<"]: "<<stats[16][i]<<", "<<endl; }
		for(unsigned int i=0; i<4; i++){ cout<<"actit:: stats[17]["<<i<<"]: "<<stats[17][i]<<", "<<endl; }
		#endif

		/* /////////////////////////////////////////////////////////////////////////
		for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
			unsigned int num_its = stats[17][llp_set+1] - stats[17][llp_set];
			for(unsigned int n=0; n<num_its; n++){
				unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; // 16
				unsigned int updateblock_id = stats[16][stats[17][llp_set] + n] - (llp_set * numkvs_updateblocks_per_reducepartition); 
				// updateblock_id = updateblock_id - (llp_set * numkvs_updateblocks_per_reducepartition);
				unsigned int buffer_offset = updateblock_id * numkvs_vertices_per_updateblock;
				
				cout<<"--------------- actit: updateblock_id: "<<updateblock_id<<", global updateblock_id: "<<stats[16][stats[17][llp_set] + n]<<", llp_set: "<<llp_set<<endl;
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("actit(112)::DEBUG CODE 112::1", buffer_offset, MAX_BLOCKRAM_VDESTDATA_SIZE, updateblock_id, numkvs_vertices_per_updateblock, llp_set); 
				#endif	
			}
		}
		///////////////////////////////////////////////////////////////////////// */


		for(unsigned int t=0; t<BLOCKRAM_UPDATEBLOCK_SIZE; t++){
		#pragma HLS PIPELINE II=1	
			datas[0] = stats[16][t];
			datas[1] = stats[17][t];
			// cout<<"~~~ actit:: datas[0]: "<<datas[0]<<", datas[1]: "<<datas[1]<<". "<<endl; 
			UTILP1_WriteDatas(kvdram, globalparamsK.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + t, datas);
		}
		// exit(EXIT_SUCCESS);
	}
	#endif
	
	collections[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections_tmp[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID]; // category 1
	collections[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections_tmp[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID];
	collections[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections_tmp[REDUCEPHASE_TRANSFSZ_COLLECTIONID];
	collections[NUMEDGESPROCESSED_COLLECTIONID] = collections_tmp[NUMEDGESPROCESSED_COLLECTIONID]; // category 2
	collections[NUMVERTICESPROCESSED_COLLECTIONID] = collections_tmp[NUMVERTICESPROCESSED_COLLECTIONID];	
	collections[NUMREADSFROMDRAM_COLLECTIONID] = collections_tmp[NUMREADSFROMDRAM_COLLECTIONID]; // category 3
	collections[NUMWRITESTODRAM_COLLECTIONID] = collections_tmp[NUMWRITESTODRAM_COLLECTIONID];
}

	
#endif 			
void acts_all::TOPP1_processit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE 
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete

	sweepparams_t sweepparams;
	sweepparams = UTILP1_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	// globalparamsK.globalvar_dramreadstats[0] = 234l
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	batch_type voffset_kvs = globalposition.source_partition * UTILP1_GET_PROCESSEDGESPARTITIONSIZEKVS2(globalparamsK);
	
	bool __processit__i_am_first__ = false; 
	bool __processit__i_am_last__ = false;
	if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ __processit__i_am_first__ = true;; } else { __processit__i_am_first__ = false; }
	if(globalposition.stage==0 && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition == globalposition.last_source_partition){ __processit__i_am_last__ = true; } else { __processit__i_am_last__ = false; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(__processit__i_am_first__ == true){ cout<<"-------------------------------- __processit__i_am_first__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	if(__processit__i_am_last__ == true){ cout<<"-------------------------------- __processit__i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl; }
	#endif
	
	sweepparams.source_partition = globalposition.source_partition;
	bool_type resetenv = ON; bool_type flush = ON;
	unsigned int _baseoffsetkvs_edgedata = globalparamsE.BASEOFFSETKVS_EDGESDATA;
	#ifdef CONFIG_RELEASE_VERSION_DUPLICATEEDGES
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ _baseoffsetkvs_edgedata = globalparamsE.BASEOFFSETKVS_EDGES0DATA; }
	#endif
	
	ACTSP1_actit(
		ON, ACTSPROCESSMODE,
		kvdram, vbuffer, edgestats, stats, globalcapsule,
		globalparamsE, globalparamsK, globalposition, sweepparams, _baseoffsetkvs_edgedata, globalparamsK.BASEOFFSETKVS_KVDRAM,
		resetenv, flush, edgebankID, collections);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalposition.source_partition==globalposition.last_source_partition){ actsutilityobj->printkeyvalues("--- TOPP1_processit", globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS); }
	#endif
	return;
}

void acts_all::TOPP1_partitionit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
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
	
	keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE]; 
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

			UTILP1_resetkeysandvalues(globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
		
			sweepparams = UTILP1_getsweepparams(globalparamsK, currentLOP, source_partition);
			
			// partition
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparamsK.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### TOPP1_partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, NAp, NAp, NAp, BATCH_RANGE / (1 << (globalparamsK.ACTSPARAMS_POW_PARTITIONS * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			UTILP1_resetvalues(globalcapsule, globalparamsK.ACTSPARAMS_NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			ACTSP1_actit(config.enablepartition, ACTSPARTITIONMODE,
					kvdram, vbuffer, edgestats, stats, globalcapsule, // CRITICAL FIXME.
					globalparamsE, globalparamsK, globalposition, sweepparams, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp, collections);
			
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

void acts_all::TOPP1_reduceit(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	
	globalposition_t globalposition; globalposition.source_partition = NAp; // DUMMY.
	
	config_t config;
	sweepparams_t sweepparams;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	
	sweepparams = UTILP1_getsweepparams(globalparamsK, currentLOP, source_partition);
	config.enablereduce = ON;
	
	ACTSP1_actit(config.enablereduce, ACTSREDUCEMODE,
			 kvdram, vbuffer, edgestats, stats, globalcapsule, // CRITICAL FIXME.
			globalparamsE, globalparamsK, globalposition, sweepparams, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp, collections);
	return;
}

void acts_all::TOPP1_dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], 
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(en_process == ON){ TOPP1_processit(kvdram, vbuffer, edgestats, stats, globalcapsule, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(en_partition == ON){ TOPP1_partitionit(kvdram, vbuffer, edgestats, stats, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(en_reduce == ON){ TOPP1_reduceit(kvdram, vbuffer, edgestats, stats, globalcapsule, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, NAp, collections); } 
	#endif 
	return;
} 

void acts_all::TOPP1_dispatch_reduce(uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalparams_t globalparamsV, globalposition_t globalposition,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){
	#pragma HLS INLINE
	unsigned int sourcestatsmarker = 1;
	unsigned int voffset_kvs = 0;
	
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = globalparamsK.NUM_REDUCEPARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printkeyvalues("TOPP1_dispatch_reduce", globalcapsule, globalparamsK.NUM_REDUCEPARTITIONS);
	#endif
	
	bool_type enablereduce = ON;
	DISPATCHREDUCEP1_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		actsutilityobj->print4("### TOPP1_dispatch_reduce:: source_partition", "currentLOP", "num_source_partitions", "voffset_kvs", source_partition, currentLOP, num_source_partitions, voffset_kvs); 							
		#endif
		
		TOPP1_dispatch(OFF, OFF, enablereduce, kvdram, vbuffer, edgestats, stats, globalcapsule, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp, collections);
		
		sourcestatsmarker += 1;
		voffset_kvs += globalparamsK.SIZEKVS2_REDUCEPARTITION;
	}
	return;
} 

void acts_all::TOPP1_topkernelproc_embedded(unsigned int GraphIter, unsigned int globalid, unsigned int localid, unsigned int en_process, unsigned int en_partition, unsigned int en_reduce, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], workload_t edgestats[EDGESTATSRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalposition_t globalposition, globalparams_t globalparamsV, collection_t collections[COLLECTIONS_BUFFERSZ]){									
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
	if(UTILP1_isbufferused(globalid) == false){ return; }
	#endif 
	#ifdef SW
	globalvar_actsinstid = globalid;
	#endif 
	
	unsigned int PARTITIONP1_CHKPT[EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
	
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
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	bool __processit__i_am_first__ = true; if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ __processit__i_am_first__ = true;; } else { __processit__i_am_first__ = false; }
	if(__processit__i_am_first__ == true){ UTILP1_resetvalues(globalcapsule, globalparamsK.NUM_REDUCEPARTITIONS, 0); } 
	if(__processit__i_am_first__ == true){ UTILP1_resetkvstatvalues(kvdram, globalparamsK); } // NEWCHANGE
	if(__processit__i_am_first__ == true){ MEMACCESSP1_readglobalstats(ON, kvdram, globalcapsule, globalparamsK.BASEOFFSETKVS_STATSDRAM, globalparamsK); }
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
		if(printheader1 == ON){ cout<<"TOPP1_topkernelproc_embedded: processing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP1_dispatch(globalposition.EN_PROCESS, OFF, OFF, kvdram, vbuffer, edgestats, stats, globalcapsule, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP1_CHKPT, globalposition.v_chunkid, globalposition.edgebankID, collections); 
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE  // CRITICAL REMOVEME URGENT.
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP1_topkernelproc_embedded: partitioning instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP1_dispatch(OFF, globalposition.EN_PARTITION, OFF, kvdram, vbuffer, edgestats, stats, globalcapsule, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITIONP1_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	// reduce & partition
	#ifdef CONFIG_ENABLEREDUCEMODULE
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#ifdef _DEBUGMODE_KERNELPRINTS3
		if(printheader1 == ON){ cout<<"TOPP1_topkernelproc_embedded: reducing instance "<<globalid<<" ... "<<endl; }
		#endif
		TOPP1_dispatch_reduce(kvdram, vbuffer, edgestats, stats, globalcapsule, _globalparamsE, globalparamsK, globalparamsV, globalposition, PARTITIONP1_CHKPT, globalposition.v_chunkid, NAp, collections);
	}
	#endif
	
	bool __reduceit__i_am_last__ = false;
	if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ __reduceit__i_am_last__ = true; }
	if(__reduceit__i_am_last__ == true){ UTILP1_increment_graphiteration(kvdram, globalparamsK); } // NB: this should come last.
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(__reduceit__i_am_last__ == true){ actsutilityobj->printglobalvars2(globalparamsK.ACTSPARAMS_INSTID, "instID", "v_chunkid", "LOP", "sourcep", globalparamsK.ACTSPARAMS_INSTID, globalposition.v_chunkid, globalposition.currentLOP, globalposition.source_partition); }
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL1_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL1_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL1_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL1_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL1_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL1_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs1(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs1(vdram, kvdram0, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL2_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL2_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL2_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL2_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL2_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL2_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs2(vdram, kvdram0,kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs2(vdram, kvdram0,kvdram1, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL3_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL3_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL3_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL3_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL3_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL3_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs3(vdram, kvdram0,kvdram1,kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs3(vdram, kvdram0,kvdram1,kvdram2, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL4_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL4_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL4_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL4_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL4_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL4_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs4(vdram, kvdram0,kvdram1,kvdram2,kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs4(vdram, kvdram0,kvdram1,kvdram2,kvdram3, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL5_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL5_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL5_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL5_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL5_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL5_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs5(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs5(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram4, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL6_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL6_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL6_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL6_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL6_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL6_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs6(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs6(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram5, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL7_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL7_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL7_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL7_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL7_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL7_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs7(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs7(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram6, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL8_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL8_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL8_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL8_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL8_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL8_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs8(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs8(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram7, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer8
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer8 type=block factor=2
		// #pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
	workload_t edgestats8[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];
	collection_t collections8[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections8 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL9_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL9_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL9_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL9_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL9_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL9_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs9(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs9(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, edgestats8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram7, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram8, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer8
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer8 type=block factor=2
		// #pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
	workload_t edgestats8[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];
	collection_t collections8[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections8 complete	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer9
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer9 type=block factor=2
		// #pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
	workload_t edgestats9[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];
	collection_t collections9[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections9 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL10_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL10_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL10_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL10_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL10_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL10_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs10(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs10(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, edgestats8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, edgestats9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram7, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram8, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram9, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer8
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer8 type=block factor=2
		// #pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
	workload_t edgestats8[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];
	collection_t collections8[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections8 complete	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer9
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer9 type=block factor=2
		// #pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
	workload_t edgestats9[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];
	collection_t collections9[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections9 complete	
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer10
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer10 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer10 type=block factor=2
		// #pragma HLS resource variable=vbuffer10 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats10[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats10
	workload_t edgestats10[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule10[BLOCKRAM_SIZE];
	collection_t collections10[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections10 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL11_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL11_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL11_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL11_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL11_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL11_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs11(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs11(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, edgestats8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, edgestats9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer10, edgestats10, stats10, globalcapsule10, globalposition, globalparamsV, collections10);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram7, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram8, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram9, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram10, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	#ifdef SW 
	for(unsigned int t=0; t<NUM_PEs; t++){ globalvar_dramreadstats[t][0] = 0; globalvar_dramwritestats[t][0] = 0; }
	#endif 
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer0
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer0 type=block factor=2
		// #pragma HLS resource variable=vbuffer0 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer0 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats0[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats0
	workload_t edgestats0[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule0[BLOCKRAM_SIZE];
	collection_t collections0[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections0 complete	
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer1
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer1 type=block factor=2
		// #pragma HLS resource variable=vbuffer1 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer1 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats1[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats1
	workload_t edgestats1[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule1[BLOCKRAM_SIZE];
	collection_t collections1[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections1 complete	
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer2
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer2 type=block factor=2
		// #pragma HLS resource variable=vbuffer2 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer2 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats2[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats2
	workload_t edgestats2[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule2[BLOCKRAM_SIZE];
	collection_t collections2[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections2 complete	
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer3
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer3 type=block factor=2
		// #pragma HLS resource variable=vbuffer3 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer3 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats3[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats3
	workload_t edgestats3[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule3[BLOCKRAM_SIZE];
	collection_t collections3[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections3 complete	
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer4
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer4 type=block factor=2
		// #pragma HLS resource variable=vbuffer4 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer4 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats4[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats4
	workload_t edgestats4[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule4[BLOCKRAM_SIZE];
	collection_t collections4[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections4 complete	
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer5
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer5 type=block factor=2
		// #pragma HLS resource variable=vbuffer5 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer5 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats5[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats5
	workload_t edgestats5[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule5[BLOCKRAM_SIZE];
	collection_t collections5[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections5 complete	
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer6
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer6 type=block factor=2
		// #pragma HLS resource variable=vbuffer6 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer6 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats6[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats6
	workload_t edgestats6[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule6[BLOCKRAM_SIZE];
	collection_t collections6[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections6 complete	
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer7
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer7 type=block factor=2
		// #pragma HLS resource variable=vbuffer7 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer7 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats7[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats7
	workload_t edgestats7[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule7[BLOCKRAM_SIZE];
	collection_t collections7[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections7 complete	
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer8
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer8 type=block factor=2
		// #pragma HLS resource variable=vbuffer8 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer8 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats8[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats8
	workload_t edgestats8[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule8[BLOCKRAM_SIZE];
	collection_t collections8[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections8 complete	
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer9
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer9 type=block factor=2
		// #pragma HLS resource variable=vbuffer9 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer9 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats9[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats9
	workload_t edgestats9[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule9[BLOCKRAM_SIZE];
	collection_t collections9[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections9 complete	
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer10
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer10 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer10 type=block factor=2
		// #pragma HLS resource variable=vbuffer10 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer10 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats10[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats10
	workload_t edgestats10[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule10[BLOCKRAM_SIZE];
	collection_t collections10[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections10 complete	
	keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vbuffer11
	#ifdef TOP_ENABLE_USEURAMS	
		// #pragma HLS bind_storage variable=vbuffer11 type=RAM_1P impl=uram
		#pragma HLS resource variable=vbuffer11 core=XPM_MEMORY uram ///// correct
		// #pragma HLS array_reshape variable=vbuffer11 type=block factor=2
		// #pragma HLS resource variable=vbuffer11 core=XPM_MEMORY uram latency=3
		// #pragma HLS bind_storage variable=vbuffer11 type=RAM_1P impl=uram
		// #pragma HLS bind_storage variable=vbuffer11 type=RAM_S2P impl=uram
	#endif 
	unsigned int stats11[STATS_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS ARRAY_PARTITION variable = stats11
	workload_t edgestats11[EDGESTATSRAM_SIZE];
	keyvalue_t globalcapsule11[BLOCKRAM_SIZE];
	collection_t collections11[COLLECTIONS_BUFFERSZ];	
	// #pragma HLS ARRAY_PARTITION variable = collections11 complete	
	pmask_dt pmask_curr[BLOCKRAM_CURRPMASK_SIZE];
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
	}
	
	UTILP1_resetkvstatvalues(vdram, globalparamsV); // NEWCHANGE.
	if(GraphAlgoClass != ALGORITHMCLASS_ALLVERTEXISACTIVE){ UTILP1_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	// unsigned int num_stages = 3; 
	unsigned int num_stages = 0; // NEWCHANGE.
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	voffset_kvs = 0;
	unsigned int upartition_base_kvs = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION);// / VDATA_SHRINK_RATIO; 
	unsigned int maxsz_actvedgeblocks = globalparamsK.NUM_PROCESSEDGESPARTITIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION;
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> topkernelP: processing edge bank "<<edgebankID<<" (of "<<it_size<<" banks)"<<endl;
		#endif
		globalposition.edgebankID = edgebankID;
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / globalparamsK.ACTSPARAMS_NUM_EDGE_BANKS); } // CRITICAL OPTIMIZEME. TOO EXPENSIVE.
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<">>> topkernelP: processing edge bank "<<edgebankID<<", v_chunkid: "<<v_chunkid<<endl;
			#endif
			globalposition.v_chunkid = v_chunkid;
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			TOPKERNEL12_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
				#ifdef _DEBUGMODE_KERNELPRINTS//4
				actsutilityobj->print3("### TOPKERNEL12_BASELOOP1C:: stage", "stage", "num_stages", stage, stage, num_stages); 							
				#endif
				
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
					#ifdef _DEBUGMODE_KERNELPRINTS//4
					actsutilityobj->print3("### TOPKERNEL12_BASELOOP1D:: stage", "currentLOP", "(FIRSTLOP + NUMLOPs)", stage, currentLOP, (FIRSTLOP + NUMLOPs)); 							
					#endif
					
					if(stage==0){ num_source_partitions = UTILP1_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = 1;  }
					else { num_source_partitions = 1; }
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITIONP1_CHKPT[0][v_chunkid]; LASTSOURCEPARTITIONS = PARTITIONP1_CHKPT[0][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "currentLOP", "NAp", "FIRSTSOURCEPARTITION", "LASTSOURCEPARTITIONS", stage, currentLOP, NAp, FIRSTSOURCEPARTITION, LASTSOURCEPARTITIONS); 
					#endif
				
					TOPKERNEL12_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS//4
						actsutilityobj->print5("### TOPKERNEL12_BASELOOP1E:: stage", "source_partition", "currentLOP", "voffset_kvs", "LASTSOURCEPARTITIONS", stage, source_partition, currentLOP, voffset_kvs, LASTSOURCEPARTITIONS); 
						#endif
						#ifdef _DEBUGMODE_CHECKS3
						actsutilityobj->checkoutofbounds("topkernelP. ERROR 34.", LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION, 16384, LASTSOURCEPARTITIONS, FIRSTSOURCEPARTITION, NAp);
						#endif
						
						if(stage==0 && source_partition % BLOCKRAM_CURRPMASK_SIZE == 0){
							#ifdef _DEBUGMODE_KERNELPRINTS3
							cout<<"### TOPKERNEL12_BASELOOP1E:: reading next set of pmasks (currentLOP: "<<currentLOP<<", source_partition: "<<source_partition<<")..."<<endl;
							#endif
							MEMACCESSP1_readhelperstats(vdram, pmask_curr, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK + source_partition, BLOCKRAM_CURRPMASK_SIZE, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsV);
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
						
						// read vertices & vmasks
						#ifdef TOP_ENABLE_BROADCASTUPROPS	
						bool en = true; if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active
						if(stage==0 && globalparamsK.ENABLE_PROCESSCOMMAND == ON && en == true){
							unsigned int depth_i = 0, transfsz_kvs = 0;
							unsigned int buffer_offsets[BLOCKRAM_SIZE];
							workload_t xload_kvs[BLOCKRAM_SIZE];
							// cout<<"--- topkernelP:: _get_upropblock_workload 777. globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl;
							unsigned int num_its = MEMACCESSP1_get_upropblock_workload(true, globalposition.source_partition, vdram, vbuffer0, stats0, globalposition.num_active_edgeblocks, globalparamsV, xload_kvs, buffer_offsets, globalparamsV.ALGORITHMINFO_GRAPHITERATIONID);
							bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsV.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION && globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = true; } else { sparse_readu = false; }
							
							if(sparse_readu == true){
								for(unsigned int n=0; n<num_its; n++){
									unsigned int s = xload_kvs[n].offset_begin / vsz_kvs; // CRITICAL OPTIMIZEME.
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs + (xload_kvs[n].offset_begin - (s * vsz_kvs));
									unsigned int offsetDST_kvs = xload_kvs[n].offset_begin - (s * vsz_kvs);
									
									#ifdef _DEBUGMODE_CHECKS3		
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 27", s, NUM_PEs, source_partition, n, num_its);									
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 28", offsetSRC_kvs, ((1 << 28) / 4) / 16, source_partition, n, num_its);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 29", offsetDST_kvs, vsz_kvs, source_partition, xload_kvs[n].offset_begin, s);
									actsutilityobj->checkoutofbounds("topkernelP:: ERROR 30", xload_kvs[n].size, globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION, source_partition, xload_kvs[n].offset_begin, s);
									#endif			
									MERGEP1_broadcastVs12(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, NUM_VERTICESKVS_PER_UPROPBLOCK);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[n].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:sparse: xload_kvs["<<n<<"].size: "<<xload_kvs[n].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							} else {
								depth_i = 0;
								for(unsigned int s=0; s<NUM_PEs; s++){
									unsigned int offsetSRC_kvs = (s * globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) + voffset_kvs;
									unsigned int offsetDST_kvs = s * vsz_kvs; 
									MERGEP1_broadcastVs12(vdram, kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + offsetSRC_kvs, globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + offsetDST_kvs, vsz_kvs);
									#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
									transfsz_kvs += xload_kvs[0].size + REPORT__DRAM_ACCESS_LATENCY;
									#endif 
									// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:dense: xload_kvs[0].size: "<<xload_kvs[0].size<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
								}
							}
							#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST	
							collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
							collections0[NUMREADSFROMDRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
							// if(globalparamsK.ACTSPARAMS_INSTID == 0 && globalparamsK.ALGORITHMINFO_GRAPHITERATIONID == 3){ cout<<"-------------- topkernelP:: instid: "<<globalparamsK.ACTSPARAMS_INSTID<<", sparse_readu: "<<sparse_readu<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", source_partition: "<<source_partition<<", collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1: "<<collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1<<endl; }
							#endif
						}
						#endif 
						
						// acts
						#ifdef CONFIG_RELEASE_FULLKERNEL
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 1, 1, enableprocess, enablepartition, enablereduce, kvdram1, vbuffer1, edgestats1, stats1, globalcapsule1, globalposition, globalparamsV, collections1);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 2, 2, enableprocess, enablepartition, enablereduce, kvdram2, vbuffer2, edgestats2, stats2, globalcapsule2, globalposition, globalparamsV, collections2);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 3, 3, enableprocess, enablepartition, enablereduce, kvdram3, vbuffer3, edgestats3, stats3, globalcapsule3, globalposition, globalparamsV, collections3);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 4, 4, enableprocess, enablepartition, enablereduce, kvdram4, vbuffer4, edgestats4, stats4, globalcapsule4, globalposition, globalparamsV, collections4);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 5, 5, enableprocess, enablepartition, enablereduce, kvdram5, vbuffer5, edgestats5, stats5, globalcapsule5, globalposition, globalparamsV, collections5);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 6, 6, enableprocess, enablepartition, enablereduce, kvdram6, vbuffer6, edgestats6, stats6, globalcapsule6, globalposition, globalparamsV, collections6);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 7, 7, enableprocess, enablepartition, enablereduce, kvdram7, vbuffer7, edgestats7, stats7, globalcapsule7, globalposition, globalparamsV, collections7);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 8, 8, enableprocess, enablepartition, enablereduce, kvdram8, vbuffer8, edgestats8, stats8, globalcapsule8, globalposition, globalparamsV, collections8);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 9, 9, enableprocess, enablepartition, enablereduce, kvdram9, vbuffer9, edgestats9, stats9, globalcapsule9, globalposition, globalparamsV, collections9);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 10, 10, enableprocess, enablepartition, enablereduce, kvdram10, vbuffer10, edgestats10, stats10, globalcapsule10, globalposition, globalparamsV, collections10);		
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 11, 11, enableprocess, enablepartition, enablereduce, kvdram11, vbuffer11, edgestats11, stats11, globalcapsule11, globalposition, globalparamsV, collections11);		
	
						#else 
						TOPP1_topkernelproc_embedded(globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, globalparamsK.ACTSPARAMS_INSTID + 0, 0, enableprocess, enablepartition, enablereduce, kvdram0, vbuffer0, edgestats0, stats0, globalcapsule0, globalposition, globalparamsV, collections0);		
		 
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
	
	UTILP1_increment_graphiteration(vdram, globalparamsV); // NB: this should come last.	
	
	#ifdef TOP_ENABLE_MERGEVS
	unsigned int transfsz_kvs = 0;
	if(globalparamsV.ENABLE_MERGECOMMAND == ON){	
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram0, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram1, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram2, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram3, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram4, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram5, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram6, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram7, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram8, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram9, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram10, vdram); 
	
		transfsz_kvs += MERGEP1_mergeVs(kvdram11, vdram); 
	}
	#ifdef MEMACCESS_ENABLE_COLLECTSTATSFORHOST
	collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE;
	collections0[NUMWRITESTODRAM_COLLECTIONID].data1 += transfsz_kvs * VECTOR2_SIZE; 
	#endif 
	#endif
	// exit(EXIT_SUCCESS); //
	
	#ifdef ACTS_ENABLE_COLLECTHOSTSTATS	
	value_t host_stats[VECTOR2_SIZE];
	host_stats[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID] = collections0[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[REDUCEPHASE_TRANSFSZ_COLLECTIONID] = collections0[REDUCEPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[BROADCASTPHASE_TRANSFSZ_COLLECTIONID] = collections0[BROADCASTPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[SYNCPHASE_TRANSFSZ_COLLECTIONID] = collections0[SYNCPHASE_TRANSFSZ_COLLECTIONID].data1;
	host_stats[NUMEDGESPROCESSED_COLLECTIONID] = collections0[NUMEDGESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMVERTICESPROCESSED_COLLECTIONID] = collections0[NUMVERTICESPROCESSED_COLLECTIONID].data1;
	host_stats[NUMREADSFROMDRAM_COLLECTIONID] = collections0[NUMREADSFROMDRAM_COLLECTIONID].data1;
	host_stats[NUMWRITESTODRAM_COLLECTIONID] = collections0[NUMWRITESTODRAM_COLLECTIONID].data1;
	UTILP1_WriteDatas(kvdram0, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID, host_stats);
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
	

	

