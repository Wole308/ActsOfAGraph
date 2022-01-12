#include "acts_merge.h"
using namespace std;

#ifdef SW
acts_merge::acts_merge(){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(); 
	processedgesobj = new processedgesu();
	processedges_splitdstvxsobj = new processedges_splitdstvxs();
	partitionupdatesobj = new partitionupdates();
	reduceupdatesobj = new reduceupdates();
	mem_accessobj = new mem_access();
	actsobj = new acts();
}
acts_merge::~acts_merge(){}
#endif

// for usrcv udstv 
unsigned int
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

value_t 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo){
	return MERGE_amin(value1, value2);
}

void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate1vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate2vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate3vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate4vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate5vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer4[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer4[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer4[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer4[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer4[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer4[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer4[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer4[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate6vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer4[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer4[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer4[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer4[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer4[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer4[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer4[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer4[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer5[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer5[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer5[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer5[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer5[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer5[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer5[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer5[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate7vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer4[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer4[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer4[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer4[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer4[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer4[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer4[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer4[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer5[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer5[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer5[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer5[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer5[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer5[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer5[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer5[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer6[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer6[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer6[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer6[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer6[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer6[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer6[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer6[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate8vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer4[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer4[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer4[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer4[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer4[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer4[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer4[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer4[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer5[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer5[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer5[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer5[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer5[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer5[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer5[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer5[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer6[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer6[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer6[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer6[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer6[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer6[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer6[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer6[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer7[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer7[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer7[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer7[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer7[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer7[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer7[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer7[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate9vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer4[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer4[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer4[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer4[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer4[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer4[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer4[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer4[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer5[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer5[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer5[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer5[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer5[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer5[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer5[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer5[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer6[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer6[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer6[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer6[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer6[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer6[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer6[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer6[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer7[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer7[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer7[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer7[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer7[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer7[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer7[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer7[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer8[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer8[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer8[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer8[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer8[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer8[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer8[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer8[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate10vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer4[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer4[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer4[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer4[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer4[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer4[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer4[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer4[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer5[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer5[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer5[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer5[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer5[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer5[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer5[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer5[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer6[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer6[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer6[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer6[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer6[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer6[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer6[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer6[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer7[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer7[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer7[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer7[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer7[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer7[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer7[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer7[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer8[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer8[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer8[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer8[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer8[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer8[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer8[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer8[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer9[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer9[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer9[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer9[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer9[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer9[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer9[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer9[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate11vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer4[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer4[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer4[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer4[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer4[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer4[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer4[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer4[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer5[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer5[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer5[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer5[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer5[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer5[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer5[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer5[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer6[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer6[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer6[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer6[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer6[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer6[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer6[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer6[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer7[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer7[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer7[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer7[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer7[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer7[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer7[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer7[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer8[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer8[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer8[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer8[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer8[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer8[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer8[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer8[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer9[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer9[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer9[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer9[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer9[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer9[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer9[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer9[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer10[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer10[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer10[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer10[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer10[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer10[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer10[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer10[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}
void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readandreplicate12vdata(bool_type enable, uint512_dt * vdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t buffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = vdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = vdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = vdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = vdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = vdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = vdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = vdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = vdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = vdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = vdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = vdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = vdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = vdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = vdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = vdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = vdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = vdram[dramoffset_kvs + i].data[7].value; 
		#endif 
		
		buffer0[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer0[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer0[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer0[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer0[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer0[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer0[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer0[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer1[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer1[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer1[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer1[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer1[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer1[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer1[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer1[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer2[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer2[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer2[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer2[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer2[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer2[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer2[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer2[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer3[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer3[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer3[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer3[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer3[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer3[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer3[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer3[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer4[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer4[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer4[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer4[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer4[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer4[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer4[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer4[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer5[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer5[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer5[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer5[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer5[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer5[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer5[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer5[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer6[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer6[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer6[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer6[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer6[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer6[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer6[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer6[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer7[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer7[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer7[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer7[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer7[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer7[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer7[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer7[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer8[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer8[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer8[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer8[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer8[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer8[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer8[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer8[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer9[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer9[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer9[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer9[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer9[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer9[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer9[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer9[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer10[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer10[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer10[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer10[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer10[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer10[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer10[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer10[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		buffer11[begincol + 0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer11[begincol + 1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer11[begincol + 2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer11[begincol + 3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer11[begincol + 4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer11[begincol + 5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer11[begincol + 6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer11[begincol + 7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTEXPTR_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_VERTEXPTR + VERTEXPTRSSZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_VERTEXPTR) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }
	if(dramoffset_kvs >= globalparams.BASEOFFSETKVS_VERTICESDATA_ && dramoffset_kvs < globalparams.BASEOFFSETKVS_DESTVERTICESDATA + VERTICESDATASZ_KVS){ cout<< TIMINGRESULTSCOLOR<<"readvdata:: vertices read: offset: "<<(dramoffset_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs - globalparams.BASEOFFSETKVS_DESTVERTICESDATA) * VECTOR_SIZE<<", number of vertex datas read: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<< RESET<<endl; }				
	#endif
	return;
}

void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge1andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE1_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = dummykv;
		keyvalue_t mykeyvalue11 = dummykv;
		keyvalue_t mykeyvalue12 = dummykv;
		keyvalue_t mykeyvalue13 = dummykv;
		keyvalue_t mykeyvalue14 = dummykv;
		keyvalue_t mykeyvalue15 = dummykv;
		keyvalue_t mykeyvalue16 = dummykv;
		keyvalue_t mykeyvalue17 = dummykv;
		keyvalue_t mykeyvalue20 = dummykv;
		keyvalue_t mykeyvalue21 = dummykv;
		keyvalue_t mykeyvalue22 = dummykv;
		keyvalue_t mykeyvalue23 = dummykv;
		keyvalue_t mykeyvalue24 = dummykv;
		keyvalue_t mykeyvalue25 = dummykv;
		keyvalue_t mykeyvalue26 = dummykv;
		keyvalue_t mykeyvalue27 = dummykv;
		keyvalue_t mykeyvalue30 = dummykv;
		keyvalue_t mykeyvalue31 = dummykv;
		keyvalue_t mykeyvalue32 = dummykv;
		keyvalue_t mykeyvalue33 = dummykv;
		keyvalue_t mykeyvalue34 = dummykv;
		keyvalue_t mykeyvalue35 = dummykv;
		keyvalue_t mykeyvalue36 = dummykv;
		keyvalue_t mykeyvalue37 = dummykv;
		keyvalue_t mykeyvalue40 = dummykv;
		keyvalue_t mykeyvalue41 = dummykv;
		keyvalue_t mykeyvalue42 = dummykv;
		keyvalue_t mykeyvalue43 = dummykv;
		keyvalue_t mykeyvalue44 = dummykv;
		keyvalue_t mykeyvalue45 = dummykv;
		keyvalue_t mykeyvalue46 = dummykv;
		keyvalue_t mykeyvalue47 = dummykv;
		keyvalue_t mykeyvalue50 = dummykv;
		keyvalue_t mykeyvalue51 = dummykv;
		keyvalue_t mykeyvalue52 = dummykv;
		keyvalue_t mykeyvalue53 = dummykv;
		keyvalue_t mykeyvalue54 = dummykv;
		keyvalue_t mykeyvalue55 = dummykv;
		keyvalue_t mykeyvalue56 = dummykv;
		keyvalue_t mykeyvalue57 = dummykv;
		keyvalue_t mykeyvalue60 = dummykv;
		keyvalue_t mykeyvalue61 = dummykv;
		keyvalue_t mykeyvalue62 = dummykv;
		keyvalue_t mykeyvalue63 = dummykv;
		keyvalue_t mykeyvalue64 = dummykv;
		keyvalue_t mykeyvalue65 = dummykv;
		keyvalue_t mykeyvalue66 = dummykv;
		keyvalue_t mykeyvalue67 = dummykv;
		keyvalue_t mykeyvalue70 = dummykv;
		keyvalue_t mykeyvalue71 = dummykv;
		keyvalue_t mykeyvalue72 = dummykv;
		keyvalue_t mykeyvalue73 = dummykv;
		keyvalue_t mykeyvalue74 = dummykv;
		keyvalue_t mykeyvalue75 = dummykv;
		keyvalue_t mykeyvalue76 = dummykv;
		keyvalue_t mykeyvalue77 = dummykv;
		keyvalue_t mykeyvalue80 = dummykv;
		keyvalue_t mykeyvalue81 = dummykv;
		keyvalue_t mykeyvalue82 = dummykv;
		keyvalue_t mykeyvalue83 = dummykv;
		keyvalue_t mykeyvalue84 = dummykv;
		keyvalue_t mykeyvalue85 = dummykv;
		keyvalue_t mykeyvalue86 = dummykv;
		keyvalue_t mykeyvalue87 = dummykv;
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge2andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE2_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = dummykv;
		keyvalue_t mykeyvalue21 = dummykv;
		keyvalue_t mykeyvalue22 = dummykv;
		keyvalue_t mykeyvalue23 = dummykv;
		keyvalue_t mykeyvalue24 = dummykv;
		keyvalue_t mykeyvalue25 = dummykv;
		keyvalue_t mykeyvalue26 = dummykv;
		keyvalue_t mykeyvalue27 = dummykv;
		keyvalue_t mykeyvalue30 = dummykv;
		keyvalue_t mykeyvalue31 = dummykv;
		keyvalue_t mykeyvalue32 = dummykv;
		keyvalue_t mykeyvalue33 = dummykv;
		keyvalue_t mykeyvalue34 = dummykv;
		keyvalue_t mykeyvalue35 = dummykv;
		keyvalue_t mykeyvalue36 = dummykv;
		keyvalue_t mykeyvalue37 = dummykv;
		keyvalue_t mykeyvalue40 = dummykv;
		keyvalue_t mykeyvalue41 = dummykv;
		keyvalue_t mykeyvalue42 = dummykv;
		keyvalue_t mykeyvalue43 = dummykv;
		keyvalue_t mykeyvalue44 = dummykv;
		keyvalue_t mykeyvalue45 = dummykv;
		keyvalue_t mykeyvalue46 = dummykv;
		keyvalue_t mykeyvalue47 = dummykv;
		keyvalue_t mykeyvalue50 = dummykv;
		keyvalue_t mykeyvalue51 = dummykv;
		keyvalue_t mykeyvalue52 = dummykv;
		keyvalue_t mykeyvalue53 = dummykv;
		keyvalue_t mykeyvalue54 = dummykv;
		keyvalue_t mykeyvalue55 = dummykv;
		keyvalue_t mykeyvalue56 = dummykv;
		keyvalue_t mykeyvalue57 = dummykv;
		keyvalue_t mykeyvalue60 = dummykv;
		keyvalue_t mykeyvalue61 = dummykv;
		keyvalue_t mykeyvalue62 = dummykv;
		keyvalue_t mykeyvalue63 = dummykv;
		keyvalue_t mykeyvalue64 = dummykv;
		keyvalue_t mykeyvalue65 = dummykv;
		keyvalue_t mykeyvalue66 = dummykv;
		keyvalue_t mykeyvalue67 = dummykv;
		keyvalue_t mykeyvalue70 = dummykv;
		keyvalue_t mykeyvalue71 = dummykv;
		keyvalue_t mykeyvalue72 = dummykv;
		keyvalue_t mykeyvalue73 = dummykv;
		keyvalue_t mykeyvalue74 = dummykv;
		keyvalue_t mykeyvalue75 = dummykv;
		keyvalue_t mykeyvalue76 = dummykv;
		keyvalue_t mykeyvalue77 = dummykv;
		keyvalue_t mykeyvalue80 = dummykv;
		keyvalue_t mykeyvalue81 = dummykv;
		keyvalue_t mykeyvalue82 = dummykv;
		keyvalue_t mykeyvalue83 = dummykv;
		keyvalue_t mykeyvalue84 = dummykv;
		keyvalue_t mykeyvalue85 = dummykv;
		keyvalue_t mykeyvalue86 = dummykv;
		keyvalue_t mykeyvalue87 = dummykv;
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge3andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE3_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = dummykv;
		keyvalue_t mykeyvalue31 = dummykv;
		keyvalue_t mykeyvalue32 = dummykv;
		keyvalue_t mykeyvalue33 = dummykv;
		keyvalue_t mykeyvalue34 = dummykv;
		keyvalue_t mykeyvalue35 = dummykv;
		keyvalue_t mykeyvalue36 = dummykv;
		keyvalue_t mykeyvalue37 = dummykv;
		keyvalue_t mykeyvalue40 = dummykv;
		keyvalue_t mykeyvalue41 = dummykv;
		keyvalue_t mykeyvalue42 = dummykv;
		keyvalue_t mykeyvalue43 = dummykv;
		keyvalue_t mykeyvalue44 = dummykv;
		keyvalue_t mykeyvalue45 = dummykv;
		keyvalue_t mykeyvalue46 = dummykv;
		keyvalue_t mykeyvalue47 = dummykv;
		keyvalue_t mykeyvalue50 = dummykv;
		keyvalue_t mykeyvalue51 = dummykv;
		keyvalue_t mykeyvalue52 = dummykv;
		keyvalue_t mykeyvalue53 = dummykv;
		keyvalue_t mykeyvalue54 = dummykv;
		keyvalue_t mykeyvalue55 = dummykv;
		keyvalue_t mykeyvalue56 = dummykv;
		keyvalue_t mykeyvalue57 = dummykv;
		keyvalue_t mykeyvalue60 = dummykv;
		keyvalue_t mykeyvalue61 = dummykv;
		keyvalue_t mykeyvalue62 = dummykv;
		keyvalue_t mykeyvalue63 = dummykv;
		keyvalue_t mykeyvalue64 = dummykv;
		keyvalue_t mykeyvalue65 = dummykv;
		keyvalue_t mykeyvalue66 = dummykv;
		keyvalue_t mykeyvalue67 = dummykv;
		keyvalue_t mykeyvalue70 = dummykv;
		keyvalue_t mykeyvalue71 = dummykv;
		keyvalue_t mykeyvalue72 = dummykv;
		keyvalue_t mykeyvalue73 = dummykv;
		keyvalue_t mykeyvalue74 = dummykv;
		keyvalue_t mykeyvalue75 = dummykv;
		keyvalue_t mykeyvalue76 = dummykv;
		keyvalue_t mykeyvalue77 = dummykv;
		keyvalue_t mykeyvalue80 = dummykv;
		keyvalue_t mykeyvalue81 = dummykv;
		keyvalue_t mykeyvalue82 = dummykv;
		keyvalue_t mykeyvalue83 = dummykv;
		keyvalue_t mykeyvalue84 = dummykv;
		keyvalue_t mykeyvalue85 = dummykv;
		keyvalue_t mykeyvalue86 = dummykv;
		keyvalue_t mykeyvalue87 = dummykv;
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge4andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE4_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = dummykv;
		keyvalue_t mykeyvalue41 = dummykv;
		keyvalue_t mykeyvalue42 = dummykv;
		keyvalue_t mykeyvalue43 = dummykv;
		keyvalue_t mykeyvalue44 = dummykv;
		keyvalue_t mykeyvalue45 = dummykv;
		keyvalue_t mykeyvalue46 = dummykv;
		keyvalue_t mykeyvalue47 = dummykv;
		keyvalue_t mykeyvalue50 = dummykv;
		keyvalue_t mykeyvalue51 = dummykv;
		keyvalue_t mykeyvalue52 = dummykv;
		keyvalue_t mykeyvalue53 = dummykv;
		keyvalue_t mykeyvalue54 = dummykv;
		keyvalue_t mykeyvalue55 = dummykv;
		keyvalue_t mykeyvalue56 = dummykv;
		keyvalue_t mykeyvalue57 = dummykv;
		keyvalue_t mykeyvalue60 = dummykv;
		keyvalue_t mykeyvalue61 = dummykv;
		keyvalue_t mykeyvalue62 = dummykv;
		keyvalue_t mykeyvalue63 = dummykv;
		keyvalue_t mykeyvalue64 = dummykv;
		keyvalue_t mykeyvalue65 = dummykv;
		keyvalue_t mykeyvalue66 = dummykv;
		keyvalue_t mykeyvalue67 = dummykv;
		keyvalue_t mykeyvalue70 = dummykv;
		keyvalue_t mykeyvalue71 = dummykv;
		keyvalue_t mykeyvalue72 = dummykv;
		keyvalue_t mykeyvalue73 = dummykv;
		keyvalue_t mykeyvalue74 = dummykv;
		keyvalue_t mykeyvalue75 = dummykv;
		keyvalue_t mykeyvalue76 = dummykv;
		keyvalue_t mykeyvalue77 = dummykv;
		keyvalue_t mykeyvalue80 = dummykv;
		keyvalue_t mykeyvalue81 = dummykv;
		keyvalue_t mykeyvalue82 = dummykv;
		keyvalue_t mykeyvalue83 = dummykv;
		keyvalue_t mykeyvalue84 = dummykv;
		keyvalue_t mykeyvalue85 = dummykv;
		keyvalue_t mykeyvalue86 = dummykv;
		keyvalue_t mykeyvalue87 = dummykv;
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge5andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE5_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue41 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue42 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue43 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue44 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue45 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue46 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue47 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue50 = dummykv;
		keyvalue_t mykeyvalue51 = dummykv;
		keyvalue_t mykeyvalue52 = dummykv;
		keyvalue_t mykeyvalue53 = dummykv;
		keyvalue_t mykeyvalue54 = dummykv;
		keyvalue_t mykeyvalue55 = dummykv;
		keyvalue_t mykeyvalue56 = dummykv;
		keyvalue_t mykeyvalue57 = dummykv;
		keyvalue_t mykeyvalue60 = dummykv;
		keyvalue_t mykeyvalue61 = dummykv;
		keyvalue_t mykeyvalue62 = dummykv;
		keyvalue_t mykeyvalue63 = dummykv;
		keyvalue_t mykeyvalue64 = dummykv;
		keyvalue_t mykeyvalue65 = dummykv;
		keyvalue_t mykeyvalue66 = dummykv;
		keyvalue_t mykeyvalue67 = dummykv;
		keyvalue_t mykeyvalue70 = dummykv;
		keyvalue_t mykeyvalue71 = dummykv;
		keyvalue_t mykeyvalue72 = dummykv;
		keyvalue_t mykeyvalue73 = dummykv;
		keyvalue_t mykeyvalue74 = dummykv;
		keyvalue_t mykeyvalue75 = dummykv;
		keyvalue_t mykeyvalue76 = dummykv;
		keyvalue_t mykeyvalue77 = dummykv;
		keyvalue_t mykeyvalue80 = dummykv;
		keyvalue_t mykeyvalue81 = dummykv;
		keyvalue_t mykeyvalue82 = dummykv;
		keyvalue_t mykeyvalue83 = dummykv;
		keyvalue_t mykeyvalue84 = dummykv;
		keyvalue_t mykeyvalue85 = dummykv;
		keyvalue_t mykeyvalue86 = dummykv;
		keyvalue_t mykeyvalue87 = dummykv;
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge6andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE6_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue41 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue42 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue43 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue44 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue45 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue46 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue47 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue50 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue51 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue52 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue53 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue54 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue55 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue56 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue57 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue60 = dummykv;
		keyvalue_t mykeyvalue61 = dummykv;
		keyvalue_t mykeyvalue62 = dummykv;
		keyvalue_t mykeyvalue63 = dummykv;
		keyvalue_t mykeyvalue64 = dummykv;
		keyvalue_t mykeyvalue65 = dummykv;
		keyvalue_t mykeyvalue66 = dummykv;
		keyvalue_t mykeyvalue67 = dummykv;
		keyvalue_t mykeyvalue70 = dummykv;
		keyvalue_t mykeyvalue71 = dummykv;
		keyvalue_t mykeyvalue72 = dummykv;
		keyvalue_t mykeyvalue73 = dummykv;
		keyvalue_t mykeyvalue74 = dummykv;
		keyvalue_t mykeyvalue75 = dummykv;
		keyvalue_t mykeyvalue76 = dummykv;
		keyvalue_t mykeyvalue77 = dummykv;
		keyvalue_t mykeyvalue80 = dummykv;
		keyvalue_t mykeyvalue81 = dummykv;
		keyvalue_t mykeyvalue82 = dummykv;
		keyvalue_t mykeyvalue83 = dummykv;
		keyvalue_t mykeyvalue84 = dummykv;
		keyvalue_t mykeyvalue85 = dummykv;
		keyvalue_t mykeyvalue86 = dummykv;
		keyvalue_t mykeyvalue87 = dummykv;
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyD0 = MERGE_mergefunc(mykeyvalue60.key, mykeyvalue70.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyD1 = MERGE_mergefunc(mykeyvalue61.key, mykeyvalue71.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyD2 = MERGE_mergefunc(mykeyvalue62.key, mykeyvalue72.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyD3 = MERGE_mergefunc(mykeyvalue63.key, mykeyvalue73.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyD4 = MERGE_mergefunc(mykeyvalue64.key, mykeyvalue74.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyD5 = MERGE_mergefunc(mykeyvalue65.key, mykeyvalue75.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyD6 = MERGE_mergefunc(mykeyvalue66.key, mykeyvalue76.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyD7 = MERGE_mergefunc(mykeyvalue67.key, mykeyvalue77.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valD0 = MERGE_mergefunc(mykeyvalue60.value, mykeyvalue70.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valD1 = MERGE_mergefunc(mykeyvalue61.value, mykeyvalue71.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valD2 = MERGE_mergefunc(mykeyvalue62.value, mykeyvalue72.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valD3 = MERGE_mergefunc(mykeyvalue63.value, mykeyvalue73.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valD4 = MERGE_mergefunc(mykeyvalue64.value, mykeyvalue74.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valD5 = MERGE_mergefunc(mykeyvalue65.value, mykeyvalue75.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valD6 = MERGE_mergefunc(mykeyvalue66.value, mykeyvalue76.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		valD7 = MERGE_mergefunc(mykeyvalue67.value, mykeyvalue77.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge7andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE7_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue41 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue42 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue43 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue44 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue45 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue46 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue47 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue50 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue51 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue52 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue53 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue54 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue55 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue56 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue57 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue60 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue61 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue62 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue63 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue64 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue65 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue66 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue67 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue70 = dummykv;
		keyvalue_t mykeyvalue71 = dummykv;
		keyvalue_t mykeyvalue72 = dummykv;
		keyvalue_t mykeyvalue73 = dummykv;
		keyvalue_t mykeyvalue74 = dummykv;
		keyvalue_t mykeyvalue75 = dummykv;
		keyvalue_t mykeyvalue76 = dummykv;
		keyvalue_t mykeyvalue77 = dummykv;
		keyvalue_t mykeyvalue80 = dummykv;
		keyvalue_t mykeyvalue81 = dummykv;
		keyvalue_t mykeyvalue82 = dummykv;
		keyvalue_t mykeyvalue83 = dummykv;
		keyvalue_t mykeyvalue84 = dummykv;
		keyvalue_t mykeyvalue85 = dummykv;
		keyvalue_t mykeyvalue86 = dummykv;
		keyvalue_t mykeyvalue87 = dummykv;
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyD0 = MERGE_mergefunc(mykeyvalue60.key, mykeyvalue70.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyD1 = MERGE_mergefunc(mykeyvalue61.key, mykeyvalue71.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyD2 = MERGE_mergefunc(mykeyvalue62.key, mykeyvalue72.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyD3 = MERGE_mergefunc(mykeyvalue63.key, mykeyvalue73.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyD4 = MERGE_mergefunc(mykeyvalue64.key, mykeyvalue74.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyD5 = MERGE_mergefunc(mykeyvalue65.key, mykeyvalue75.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyD6 = MERGE_mergefunc(mykeyvalue66.key, mykeyvalue76.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyD7 = MERGE_mergefunc(mykeyvalue67.key, mykeyvalue77.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valD0 = MERGE_mergefunc(mykeyvalue60.value, mykeyvalue70.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valD1 = MERGE_mergefunc(mykeyvalue61.value, mykeyvalue71.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valD2 = MERGE_mergefunc(mykeyvalue62.value, mykeyvalue72.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valD3 = MERGE_mergefunc(mykeyvalue63.value, mykeyvalue73.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valD4 = MERGE_mergefunc(mykeyvalue64.value, mykeyvalue74.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valD5 = MERGE_mergefunc(mykeyvalue65.value, mykeyvalue75.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valD6 = MERGE_mergefunc(mykeyvalue66.value, mykeyvalue76.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		valD7 = MERGE_mergefunc(mykeyvalue67.value, mykeyvalue77.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge8andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE8_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue41 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue42 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue43 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue44 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue45 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue46 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue47 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue50 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue51 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue52 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue53 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue54 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue55 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue56 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue57 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue60 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue61 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue62 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue63 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue64 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue65 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue66 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue67 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue70 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue71 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue72 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue73 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue74 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue75 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue76 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue77 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue80 = dummykv;
		keyvalue_t mykeyvalue81 = dummykv;
		keyvalue_t mykeyvalue82 = dummykv;
		keyvalue_t mykeyvalue83 = dummykv;
		keyvalue_t mykeyvalue84 = dummykv;
		keyvalue_t mykeyvalue85 = dummykv;
		keyvalue_t mykeyvalue86 = dummykv;
		keyvalue_t mykeyvalue87 = dummykv;
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyD0 = MERGE_mergefunc(mykeyvalue60.key, mykeyvalue70.key, NAp);
		keyE0 = MERGE_mergefunc(mykeyvalue80.key, mykeyvalue90.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyD1 = MERGE_mergefunc(mykeyvalue61.key, mykeyvalue71.key, NAp);
		keyE1 = MERGE_mergefunc(mykeyvalue81.key, mykeyvalue91.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyD2 = MERGE_mergefunc(mykeyvalue62.key, mykeyvalue72.key, NAp);
		keyE2 = MERGE_mergefunc(mykeyvalue82.key, mykeyvalue92.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyD3 = MERGE_mergefunc(mykeyvalue63.key, mykeyvalue73.key, NAp);
		keyE3 = MERGE_mergefunc(mykeyvalue83.key, mykeyvalue93.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyD4 = MERGE_mergefunc(mykeyvalue64.key, mykeyvalue74.key, NAp);
		keyE4 = MERGE_mergefunc(mykeyvalue84.key, mykeyvalue94.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyD5 = MERGE_mergefunc(mykeyvalue65.key, mykeyvalue75.key, NAp);
		keyE5 = MERGE_mergefunc(mykeyvalue85.key, mykeyvalue95.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyD6 = MERGE_mergefunc(mykeyvalue66.key, mykeyvalue76.key, NAp);
		keyE6 = MERGE_mergefunc(mykeyvalue86.key, mykeyvalue96.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyD7 = MERGE_mergefunc(mykeyvalue67.key, mykeyvalue77.key, NAp);
		keyE7 = MERGE_mergefunc(mykeyvalue87.key, mykeyvalue97.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valD0 = MERGE_mergefunc(mykeyvalue60.value, mykeyvalue70.value, NAp);
		valE0 = MERGE_mergefunc(mykeyvalue80.value, mykeyvalue90.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valD1 = MERGE_mergefunc(mykeyvalue61.value, mykeyvalue71.value, NAp);
		valE1 = MERGE_mergefunc(mykeyvalue81.value, mykeyvalue91.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valD2 = MERGE_mergefunc(mykeyvalue62.value, mykeyvalue72.value, NAp);
		valE2 = MERGE_mergefunc(mykeyvalue82.value, mykeyvalue92.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valD3 = MERGE_mergefunc(mykeyvalue63.value, mykeyvalue73.value, NAp);
		valE3 = MERGE_mergefunc(mykeyvalue83.value, mykeyvalue93.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valD4 = MERGE_mergefunc(mykeyvalue64.value, mykeyvalue74.value, NAp);
		valE4 = MERGE_mergefunc(mykeyvalue84.value, mykeyvalue94.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valD5 = MERGE_mergefunc(mykeyvalue65.value, mykeyvalue75.value, NAp);
		valE5 = MERGE_mergefunc(mykeyvalue85.value, mykeyvalue95.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valD6 = MERGE_mergefunc(mykeyvalue66.value, mykeyvalue76.value, NAp);
		valE6 = MERGE_mergefunc(mykeyvalue86.value, mykeyvalue96.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		valD7 = MERGE_mergefunc(mykeyvalue67.value, mykeyvalue77.value, NAp);
		valE7 = MERGE_mergefunc(mykeyvalue87.value, mykeyvalue97.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge9andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE9_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue41 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue42 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue43 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue44 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue45 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue46 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue47 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue50 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue51 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue52 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue53 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue54 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue55 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue56 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue57 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue60 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue61 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue62 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue63 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue64 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue65 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue66 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue67 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue70 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue71 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue72 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue73 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue74 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue75 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue76 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue77 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue80 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue81 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue82 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue83 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue84 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue85 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue86 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue87 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue90 = dummykv;
		keyvalue_t mykeyvalue91 = dummykv;
		keyvalue_t mykeyvalue92 = dummykv;
		keyvalue_t mykeyvalue93 = dummykv;
		keyvalue_t mykeyvalue94 = dummykv;
		keyvalue_t mykeyvalue95 = dummykv;
		keyvalue_t mykeyvalue96 = dummykv;
		keyvalue_t mykeyvalue97 = dummykv;
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyD0 = MERGE_mergefunc(mykeyvalue60.key, mykeyvalue70.key, NAp);
		keyE0 = MERGE_mergefunc(mykeyvalue80.key, mykeyvalue90.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyD1 = MERGE_mergefunc(mykeyvalue61.key, mykeyvalue71.key, NAp);
		keyE1 = MERGE_mergefunc(mykeyvalue81.key, mykeyvalue91.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyD2 = MERGE_mergefunc(mykeyvalue62.key, mykeyvalue72.key, NAp);
		keyE2 = MERGE_mergefunc(mykeyvalue82.key, mykeyvalue92.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyD3 = MERGE_mergefunc(mykeyvalue63.key, mykeyvalue73.key, NAp);
		keyE3 = MERGE_mergefunc(mykeyvalue83.key, mykeyvalue93.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyD4 = MERGE_mergefunc(mykeyvalue64.key, mykeyvalue74.key, NAp);
		keyE4 = MERGE_mergefunc(mykeyvalue84.key, mykeyvalue94.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyD5 = MERGE_mergefunc(mykeyvalue65.key, mykeyvalue75.key, NAp);
		keyE5 = MERGE_mergefunc(mykeyvalue85.key, mykeyvalue95.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyD6 = MERGE_mergefunc(mykeyvalue66.key, mykeyvalue76.key, NAp);
		keyE6 = MERGE_mergefunc(mykeyvalue86.key, mykeyvalue96.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyD7 = MERGE_mergefunc(mykeyvalue67.key, mykeyvalue77.key, NAp);
		keyE7 = MERGE_mergefunc(mykeyvalue87.key, mykeyvalue97.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valD0 = MERGE_mergefunc(mykeyvalue60.value, mykeyvalue70.value, NAp);
		valE0 = MERGE_mergefunc(mykeyvalue80.value, mykeyvalue90.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valD1 = MERGE_mergefunc(mykeyvalue61.value, mykeyvalue71.value, NAp);
		valE1 = MERGE_mergefunc(mykeyvalue81.value, mykeyvalue91.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valD2 = MERGE_mergefunc(mykeyvalue62.value, mykeyvalue72.value, NAp);
		valE2 = MERGE_mergefunc(mykeyvalue82.value, mykeyvalue92.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valD3 = MERGE_mergefunc(mykeyvalue63.value, mykeyvalue73.value, NAp);
		valE3 = MERGE_mergefunc(mykeyvalue83.value, mykeyvalue93.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valD4 = MERGE_mergefunc(mykeyvalue64.value, mykeyvalue74.value, NAp);
		valE4 = MERGE_mergefunc(mykeyvalue84.value, mykeyvalue94.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valD5 = MERGE_mergefunc(mykeyvalue65.value, mykeyvalue75.value, NAp);
		valE5 = MERGE_mergefunc(mykeyvalue85.value, mykeyvalue95.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valD6 = MERGE_mergefunc(mykeyvalue66.value, mykeyvalue76.value, NAp);
		valE6 = MERGE_mergefunc(mykeyvalue86.value, mykeyvalue96.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		valD7 = MERGE_mergefunc(mykeyvalue67.value, mykeyvalue77.value, NAp);
		valE7 = MERGE_mergefunc(mykeyvalue87.value, mykeyvalue97.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge10andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE10_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue41 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue42 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue43 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue44 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue45 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue46 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue47 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue50 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue51 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue52 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue53 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue54 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue55 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue56 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue57 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue60 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue61 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue62 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue63 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue64 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue65 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue66 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue67 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue70 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue71 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue72 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue73 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue74 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue75 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue76 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue77 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue80 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue81 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue82 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue83 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue84 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue85 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue86 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue87 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue90 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue91 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue92 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue93 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue94 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue95 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue96 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue97 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue100 = dummykv;
		keyvalue_t mykeyvalue101 = dummykv;
		keyvalue_t mykeyvalue102 = dummykv;
		keyvalue_t mykeyvalue103 = dummykv;
		keyvalue_t mykeyvalue104 = dummykv;
		keyvalue_t mykeyvalue105 = dummykv;
		keyvalue_t mykeyvalue106 = dummykv;
		keyvalue_t mykeyvalue107 = dummykv;
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyD0 = MERGE_mergefunc(mykeyvalue60.key, mykeyvalue70.key, NAp);
		keyE0 = MERGE_mergefunc(mykeyvalue80.key, mykeyvalue90.key, NAp);
		keyF0 = MERGE_mergefunc(mykeyvalue100.key, mykeyvalue110.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyD1 = MERGE_mergefunc(mykeyvalue61.key, mykeyvalue71.key, NAp);
		keyE1 = MERGE_mergefunc(mykeyvalue81.key, mykeyvalue91.key, NAp);
		keyF1 = MERGE_mergefunc(mykeyvalue101.key, mykeyvalue111.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyD2 = MERGE_mergefunc(mykeyvalue62.key, mykeyvalue72.key, NAp);
		keyE2 = MERGE_mergefunc(mykeyvalue82.key, mykeyvalue92.key, NAp);
		keyF2 = MERGE_mergefunc(mykeyvalue102.key, mykeyvalue112.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyD3 = MERGE_mergefunc(mykeyvalue63.key, mykeyvalue73.key, NAp);
		keyE3 = MERGE_mergefunc(mykeyvalue83.key, mykeyvalue93.key, NAp);
		keyF3 = MERGE_mergefunc(mykeyvalue103.key, mykeyvalue113.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyD4 = MERGE_mergefunc(mykeyvalue64.key, mykeyvalue74.key, NAp);
		keyE4 = MERGE_mergefunc(mykeyvalue84.key, mykeyvalue94.key, NAp);
		keyF4 = MERGE_mergefunc(mykeyvalue104.key, mykeyvalue114.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyD5 = MERGE_mergefunc(mykeyvalue65.key, mykeyvalue75.key, NAp);
		keyE5 = MERGE_mergefunc(mykeyvalue85.key, mykeyvalue95.key, NAp);
		keyF5 = MERGE_mergefunc(mykeyvalue105.key, mykeyvalue115.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyD6 = MERGE_mergefunc(mykeyvalue66.key, mykeyvalue76.key, NAp);
		keyE6 = MERGE_mergefunc(mykeyvalue86.key, mykeyvalue96.key, NAp);
		keyF6 = MERGE_mergefunc(mykeyvalue106.key, mykeyvalue116.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyD7 = MERGE_mergefunc(mykeyvalue67.key, mykeyvalue77.key, NAp);
		keyE7 = MERGE_mergefunc(mykeyvalue87.key, mykeyvalue97.key, NAp);
		keyF7 = MERGE_mergefunc(mykeyvalue107.key, mykeyvalue117.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valD0 = MERGE_mergefunc(mykeyvalue60.value, mykeyvalue70.value, NAp);
		valE0 = MERGE_mergefunc(mykeyvalue80.value, mykeyvalue90.value, NAp);
		valF0 = MERGE_mergefunc(mykeyvalue100.value, mykeyvalue110.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valD1 = MERGE_mergefunc(mykeyvalue61.value, mykeyvalue71.value, NAp);
		valE1 = MERGE_mergefunc(mykeyvalue81.value, mykeyvalue91.value, NAp);
		valF1 = MERGE_mergefunc(mykeyvalue101.value, mykeyvalue111.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valD2 = MERGE_mergefunc(mykeyvalue62.value, mykeyvalue72.value, NAp);
		valE2 = MERGE_mergefunc(mykeyvalue82.value, mykeyvalue92.value, NAp);
		valF2 = MERGE_mergefunc(mykeyvalue102.value, mykeyvalue112.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valD3 = MERGE_mergefunc(mykeyvalue63.value, mykeyvalue73.value, NAp);
		valE3 = MERGE_mergefunc(mykeyvalue83.value, mykeyvalue93.value, NAp);
		valF3 = MERGE_mergefunc(mykeyvalue103.value, mykeyvalue113.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valD4 = MERGE_mergefunc(mykeyvalue64.value, mykeyvalue74.value, NAp);
		valE4 = MERGE_mergefunc(mykeyvalue84.value, mykeyvalue94.value, NAp);
		valF4 = MERGE_mergefunc(mykeyvalue104.value, mykeyvalue114.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valD5 = MERGE_mergefunc(mykeyvalue65.value, mykeyvalue75.value, NAp);
		valE5 = MERGE_mergefunc(mykeyvalue85.value, mykeyvalue95.value, NAp);
		valF5 = MERGE_mergefunc(mykeyvalue105.value, mykeyvalue115.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valD6 = MERGE_mergefunc(mykeyvalue66.value, mykeyvalue76.value, NAp);
		valE6 = MERGE_mergefunc(mykeyvalue86.value, mykeyvalue96.value, NAp);
		valF6 = MERGE_mergefunc(mykeyvalue106.value, mykeyvalue116.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		valD7 = MERGE_mergefunc(mykeyvalue67.value, mykeyvalue77.value, NAp);
		valE7 = MERGE_mergefunc(mykeyvalue87.value, mykeyvalue97.value, NAp);
		valF7 = MERGE_mergefunc(mykeyvalue107.value, mykeyvalue117.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge11andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE11_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue41 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue42 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue43 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue44 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue45 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue46 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue47 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue50 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue51 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue52 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue53 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue54 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue55 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue56 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue57 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue60 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue61 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue62 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue63 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue64 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue65 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue66 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue67 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue70 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue71 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue72 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue73 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue74 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue75 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue76 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue77 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue80 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue81 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue82 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue83 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue84 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue85 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue86 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue87 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue90 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue91 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue92 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue93 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue94 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue95 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue96 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue97 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue100 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue101 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue102 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue103 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue104 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue105 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue106 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue107 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue110 = dummykv;
		keyvalue_t mykeyvalue111 = dummykv;
		keyvalue_t mykeyvalue112 = dummykv;
		keyvalue_t mykeyvalue113 = dummykv;
		keyvalue_t mykeyvalue114 = dummykv;
		keyvalue_t mykeyvalue115 = dummykv;
		keyvalue_t mykeyvalue116 = dummykv;
		keyvalue_t mykeyvalue117 = dummykv;
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyD0 = MERGE_mergefunc(mykeyvalue60.key, mykeyvalue70.key, NAp);
		keyE0 = MERGE_mergefunc(mykeyvalue80.key, mykeyvalue90.key, NAp);
		keyF0 = MERGE_mergefunc(mykeyvalue100.key, mykeyvalue110.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyD1 = MERGE_mergefunc(mykeyvalue61.key, mykeyvalue71.key, NAp);
		keyE1 = MERGE_mergefunc(mykeyvalue81.key, mykeyvalue91.key, NAp);
		keyF1 = MERGE_mergefunc(mykeyvalue101.key, mykeyvalue111.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyD2 = MERGE_mergefunc(mykeyvalue62.key, mykeyvalue72.key, NAp);
		keyE2 = MERGE_mergefunc(mykeyvalue82.key, mykeyvalue92.key, NAp);
		keyF2 = MERGE_mergefunc(mykeyvalue102.key, mykeyvalue112.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyD3 = MERGE_mergefunc(mykeyvalue63.key, mykeyvalue73.key, NAp);
		keyE3 = MERGE_mergefunc(mykeyvalue83.key, mykeyvalue93.key, NAp);
		keyF3 = MERGE_mergefunc(mykeyvalue103.key, mykeyvalue113.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyD4 = MERGE_mergefunc(mykeyvalue64.key, mykeyvalue74.key, NAp);
		keyE4 = MERGE_mergefunc(mykeyvalue84.key, mykeyvalue94.key, NAp);
		keyF4 = MERGE_mergefunc(mykeyvalue104.key, mykeyvalue114.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyD5 = MERGE_mergefunc(mykeyvalue65.key, mykeyvalue75.key, NAp);
		keyE5 = MERGE_mergefunc(mykeyvalue85.key, mykeyvalue95.key, NAp);
		keyF5 = MERGE_mergefunc(mykeyvalue105.key, mykeyvalue115.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyD6 = MERGE_mergefunc(mykeyvalue66.key, mykeyvalue76.key, NAp);
		keyE6 = MERGE_mergefunc(mykeyvalue86.key, mykeyvalue96.key, NAp);
		keyF6 = MERGE_mergefunc(mykeyvalue106.key, mykeyvalue116.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyD7 = MERGE_mergefunc(mykeyvalue67.key, mykeyvalue77.key, NAp);
		keyE7 = MERGE_mergefunc(mykeyvalue87.key, mykeyvalue97.key, NAp);
		keyF7 = MERGE_mergefunc(mykeyvalue107.key, mykeyvalue117.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valD0 = MERGE_mergefunc(mykeyvalue60.value, mykeyvalue70.value, NAp);
		valE0 = MERGE_mergefunc(mykeyvalue80.value, mykeyvalue90.value, NAp);
		valF0 = MERGE_mergefunc(mykeyvalue100.value, mykeyvalue110.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valD1 = MERGE_mergefunc(mykeyvalue61.value, mykeyvalue71.value, NAp);
		valE1 = MERGE_mergefunc(mykeyvalue81.value, mykeyvalue91.value, NAp);
		valF1 = MERGE_mergefunc(mykeyvalue101.value, mykeyvalue111.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valD2 = MERGE_mergefunc(mykeyvalue62.value, mykeyvalue72.value, NAp);
		valE2 = MERGE_mergefunc(mykeyvalue82.value, mykeyvalue92.value, NAp);
		valF2 = MERGE_mergefunc(mykeyvalue102.value, mykeyvalue112.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valD3 = MERGE_mergefunc(mykeyvalue63.value, mykeyvalue73.value, NAp);
		valE3 = MERGE_mergefunc(mykeyvalue83.value, mykeyvalue93.value, NAp);
		valF3 = MERGE_mergefunc(mykeyvalue103.value, mykeyvalue113.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valD4 = MERGE_mergefunc(mykeyvalue64.value, mykeyvalue74.value, NAp);
		valE4 = MERGE_mergefunc(mykeyvalue84.value, mykeyvalue94.value, NAp);
		valF4 = MERGE_mergefunc(mykeyvalue104.value, mykeyvalue114.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valD5 = MERGE_mergefunc(mykeyvalue65.value, mykeyvalue75.value, NAp);
		valE5 = MERGE_mergefunc(mykeyvalue85.value, mykeyvalue95.value, NAp);
		valF5 = MERGE_mergefunc(mykeyvalue105.value, mykeyvalue115.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valD6 = MERGE_mergefunc(mykeyvalue66.value, mykeyvalue76.value, NAp);
		valE6 = MERGE_mergefunc(mykeyvalue86.value, mykeyvalue96.value, NAp);
		valF6 = MERGE_mergefunc(mykeyvalue106.value, mykeyvalue116.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		valD7 = MERGE_mergefunc(mykeyvalue67.value, mykeyvalue77.value, NAp);
		valE7 = MERGE_mergefunc(mykeyvalue87.value, mykeyvalue97.value, NAp);
		valF7 = MERGE_mergefunc(mykeyvalue107.value, mykeyvalue117.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}
void
	#if defined(SW)
	acts_merge::
	#endif
MERGE_merge12andsavevdata(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE],keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int begincol, batch_type bufferoffset_kvs, unsigned int dramoffset_kvs){
	if(enable == OFF){ return; }
	keyvalue_t dummykv;
	dummykv.key = 0xFFFFFFFF; dummykv.value = 0xFFFFFFFF;
	
	keyy_t keyA0 = dummykv.key;
	value_t valA0 = dummykv.key;
	keyy_t keyB0 = dummykv.key;
	value_t valB0 = dummykv.key;
	keyy_t keyC0 = dummykv.key;
	value_t valC0 = dummykv.key;
	keyy_t keyD0 = dummykv.key;
	value_t valD0 = dummykv.key;
	keyy_t keyE0 = dummykv.key;
	value_t valE0 = dummykv.key;
	keyy_t keyF0 = dummykv.key;
	value_t valF0 = dummykv.key;
	keyy_t keyG0 = dummykv.key;
	value_t valG0 = dummykv.key;
	keyy_t keyH0 = dummykv.key;
	value_t valH0 = dummykv.key;
	keyy_t keyI0 = dummykv.key;
	value_t valI0 = dummykv.key;
	keyy_t keyJ0 = dummykv.key;
	value_t valJ0 = dummykv.key;
	keyy_t keyK0 = dummykv.key;
	value_t valK0 = dummykv.key;
	keyy_t keyL0 = dummykv.key;
	value_t valL0 = dummykv.key;
	keyy_t keyM0 = dummykv.key;
	value_t valM0 = dummykv.key;
	keyy_t keyA1 = dummykv.key;
	value_t valA1 = dummykv.key;
	keyy_t keyB1 = dummykv.key;
	value_t valB1 = dummykv.key;
	keyy_t keyC1 = dummykv.key;
	value_t valC1 = dummykv.key;
	keyy_t keyD1 = dummykv.key;
	value_t valD1 = dummykv.key;
	keyy_t keyE1 = dummykv.key;
	value_t valE1 = dummykv.key;
	keyy_t keyF1 = dummykv.key;
	value_t valF1 = dummykv.key;
	keyy_t keyG1 = dummykv.key;
	value_t valG1 = dummykv.key;
	keyy_t keyH1 = dummykv.key;
	value_t valH1 = dummykv.key;
	keyy_t keyI1 = dummykv.key;
	value_t valI1 = dummykv.key;
	keyy_t keyJ1 = dummykv.key;
	value_t valJ1 = dummykv.key;
	keyy_t keyK1 = dummykv.key;
	value_t valK1 = dummykv.key;
	keyy_t keyL1 = dummykv.key;
	value_t valL1 = dummykv.key;
	keyy_t keyM1 = dummykv.key;
	value_t valM1 = dummykv.key;
	keyy_t keyA2 = dummykv.key;
	value_t valA2 = dummykv.key;
	keyy_t keyB2 = dummykv.key;
	value_t valB2 = dummykv.key;
	keyy_t keyC2 = dummykv.key;
	value_t valC2 = dummykv.key;
	keyy_t keyD2 = dummykv.key;
	value_t valD2 = dummykv.key;
	keyy_t keyE2 = dummykv.key;
	value_t valE2 = dummykv.key;
	keyy_t keyF2 = dummykv.key;
	value_t valF2 = dummykv.key;
	keyy_t keyG2 = dummykv.key;
	value_t valG2 = dummykv.key;
	keyy_t keyH2 = dummykv.key;
	value_t valH2 = dummykv.key;
	keyy_t keyI2 = dummykv.key;
	value_t valI2 = dummykv.key;
	keyy_t keyJ2 = dummykv.key;
	value_t valJ2 = dummykv.key;
	keyy_t keyK2 = dummykv.key;
	value_t valK2 = dummykv.key;
	keyy_t keyL2 = dummykv.key;
	value_t valL2 = dummykv.key;
	keyy_t keyM2 = dummykv.key;
	value_t valM2 = dummykv.key;
	keyy_t keyA3 = dummykv.key;
	value_t valA3 = dummykv.key;
	keyy_t keyB3 = dummykv.key;
	value_t valB3 = dummykv.key;
	keyy_t keyC3 = dummykv.key;
	value_t valC3 = dummykv.key;
	keyy_t keyD3 = dummykv.key;
	value_t valD3 = dummykv.key;
	keyy_t keyE3 = dummykv.key;
	value_t valE3 = dummykv.key;
	keyy_t keyF3 = dummykv.key;
	value_t valF3 = dummykv.key;
	keyy_t keyG3 = dummykv.key;
	value_t valG3 = dummykv.key;
	keyy_t keyH3 = dummykv.key;
	value_t valH3 = dummykv.key;
	keyy_t keyI3 = dummykv.key;
	value_t valI3 = dummykv.key;
	keyy_t keyJ3 = dummykv.key;
	value_t valJ3 = dummykv.key;
	keyy_t keyK3 = dummykv.key;
	value_t valK3 = dummykv.key;
	keyy_t keyL3 = dummykv.key;
	value_t valL3 = dummykv.key;
	keyy_t keyM3 = dummykv.key;
	value_t valM3 = dummykv.key;
	keyy_t keyA4 = dummykv.key;
	value_t valA4 = dummykv.key;
	keyy_t keyB4 = dummykv.key;
	value_t valB4 = dummykv.key;
	keyy_t keyC4 = dummykv.key;
	value_t valC4 = dummykv.key;
	keyy_t keyD4 = dummykv.key;
	value_t valD4 = dummykv.key;
	keyy_t keyE4 = dummykv.key;
	value_t valE4 = dummykv.key;
	keyy_t keyF4 = dummykv.key;
	value_t valF4 = dummykv.key;
	keyy_t keyG4 = dummykv.key;
	value_t valG4 = dummykv.key;
	keyy_t keyH4 = dummykv.key;
	value_t valH4 = dummykv.key;
	keyy_t keyI4 = dummykv.key;
	value_t valI4 = dummykv.key;
	keyy_t keyJ4 = dummykv.key;
	value_t valJ4 = dummykv.key;
	keyy_t keyK4 = dummykv.key;
	value_t valK4 = dummykv.key;
	keyy_t keyL4 = dummykv.key;
	value_t valL4 = dummykv.key;
	keyy_t keyM4 = dummykv.key;
	value_t valM4 = dummykv.key;
	keyy_t keyA5 = dummykv.key;
	value_t valA5 = dummykv.key;
	keyy_t keyB5 = dummykv.key;
	value_t valB5 = dummykv.key;
	keyy_t keyC5 = dummykv.key;
	value_t valC5 = dummykv.key;
	keyy_t keyD5 = dummykv.key;
	value_t valD5 = dummykv.key;
	keyy_t keyE5 = dummykv.key;
	value_t valE5 = dummykv.key;
	keyy_t keyF5 = dummykv.key;
	value_t valF5 = dummykv.key;
	keyy_t keyG5 = dummykv.key;
	value_t valG5 = dummykv.key;
	keyy_t keyH5 = dummykv.key;
	value_t valH5 = dummykv.key;
	keyy_t keyI5 = dummykv.key;
	value_t valI5 = dummykv.key;
	keyy_t keyJ5 = dummykv.key;
	value_t valJ5 = dummykv.key;
	keyy_t keyK5 = dummykv.key;
	value_t valK5 = dummykv.key;
	keyy_t keyL5 = dummykv.key;
	value_t valL5 = dummykv.key;
	keyy_t keyM5 = dummykv.key;
	value_t valM5 = dummykv.key;
	keyy_t keyA6 = dummykv.key;
	value_t valA6 = dummykv.key;
	keyy_t keyB6 = dummykv.key;
	value_t valB6 = dummykv.key;
	keyy_t keyC6 = dummykv.key;
	value_t valC6 = dummykv.key;
	keyy_t keyD6 = dummykv.key;
	value_t valD6 = dummykv.key;
	keyy_t keyE6 = dummykv.key;
	value_t valE6 = dummykv.key;
	keyy_t keyF6 = dummykv.key;
	value_t valF6 = dummykv.key;
	keyy_t keyG6 = dummykv.key;
	value_t valG6 = dummykv.key;
	keyy_t keyH6 = dummykv.key;
	value_t valH6 = dummykv.key;
	keyy_t keyI6 = dummykv.key;
	value_t valI6 = dummykv.key;
	keyy_t keyJ6 = dummykv.key;
	value_t valJ6 = dummykv.key;
	keyy_t keyK6 = dummykv.key;
	value_t valK6 = dummykv.key;
	keyy_t keyL6 = dummykv.key;
	value_t valL6 = dummykv.key;
	keyy_t keyM6 = dummykv.key;
	value_t valM6 = dummykv.key;
	keyy_t keyA7 = dummykv.key;
	value_t valA7 = dummykv.key;
	keyy_t keyB7 = dummykv.key;
	value_t valB7 = dummykv.key;
	keyy_t keyC7 = dummykv.key;
	value_t valC7 = dummykv.key;
	keyy_t keyD7 = dummykv.key;
	value_t valD7 = dummykv.key;
	keyy_t keyE7 = dummykv.key;
	value_t valE7 = dummykv.key;
	keyy_t keyF7 = dummykv.key;
	value_t valF7 = dummykv.key;
	keyy_t keyG7 = dummykv.key;
	value_t valG7 = dummykv.key;
	keyy_t keyH7 = dummykv.key;
	value_t valH7 = dummykv.key;
	keyy_t keyI7 = dummykv.key;
	value_t valI7 = dummykv.key;
	keyy_t keyJ7 = dummykv.key;
	value_t valJ7 = dummykv.key;
	keyy_t keyK7 = dummykv.key;
	value_t valK7 = dummykv.key;
	keyy_t keyL7 = dummykv.key;
	value_t valL7 = dummykv.key;
	keyy_t keyM7 = dummykv.key;
	value_t valM7 = dummykv.key;
	
	MERGE12_LOOP1: for (buffer_type j=0; j<BLOCKRAM_SIZE; j++){
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue00 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue01 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue02 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue03 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue04 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue05 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue06 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue07 = acts_utilobj->UTIL_GETKV2(vbuffer0[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue10 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue11 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue12 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue13 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue14 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue15 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue16 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue17 = acts_utilobj->UTIL_GETKV2(vbuffer1[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue20 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue21 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue22 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue23 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue24 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue25 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue26 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue27 = acts_utilobj->UTIL_GETKV2(vbuffer2[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue30 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue31 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue32 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue33 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue34 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue35 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue36 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue37 = acts_utilobj->UTIL_GETKV2(vbuffer3[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue40 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue41 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue42 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue43 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue44 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue45 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue46 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue47 = acts_utilobj->UTIL_GETKV2(vbuffer4[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue50 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue51 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue52 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue53 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue54 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue55 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue56 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue57 = acts_utilobj->UTIL_GETKV2(vbuffer5[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue60 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue61 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue62 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue63 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue64 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue65 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue66 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue67 = acts_utilobj->UTIL_GETKV2(vbuffer6[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue70 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue71 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue72 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue73 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue74 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue75 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue76 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue77 = acts_utilobj->UTIL_GETKV2(vbuffer7[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue80 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue81 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue82 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue83 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue84 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue85 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue86 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue87 = acts_utilobj->UTIL_GETKV2(vbuffer8[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue90 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue91 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue92 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue93 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue94 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue95 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue96 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue97 = acts_utilobj->UTIL_GETKV2(vbuffer9[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue100 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue101 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue102 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue103 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue104 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue105 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue106 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue107 = acts_utilobj->UTIL_GETKV2(vbuffer10[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue110 = acts_utilobj->UTIL_GETKV2(vbuffer11[begincol + 0][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue111 = acts_utilobj->UTIL_GETKV2(vbuffer11[begincol + 1][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue112 = acts_utilobj->UTIL_GETKV2(vbuffer11[begincol + 2][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue113 = acts_utilobj->UTIL_GETKV2(vbuffer11[begincol + 3][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue114 = acts_utilobj->UTIL_GETKV2(vbuffer11[begincol + 4][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue115 = acts_utilobj->UTIL_GETKV2(vbuffer11[begincol + 5][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue116 = acts_utilobj->UTIL_GETKV2(vbuffer11[begincol + 6][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue117 = acts_utilobj->UTIL_GETKV2(vbuffer11[begincol + 7][bufferoffset_kvs + j]);	
		keyvalue_t mykeyvalue120 = dummykv;
		keyvalue_t mykeyvalue121 = dummykv;
		keyvalue_t mykeyvalue122 = dummykv;
		keyvalue_t mykeyvalue123 = dummykv;
		keyvalue_t mykeyvalue124 = dummykv;
		keyvalue_t mykeyvalue125 = dummykv;
		keyvalue_t mykeyvalue126 = dummykv;
		keyvalue_t mykeyvalue127 = dummykv;
		keyvalue_t mykeyvalue130 = dummykv;
		keyvalue_t mykeyvalue131 = dummykv;
		keyvalue_t mykeyvalue132 = dummykv;
		keyvalue_t mykeyvalue133 = dummykv;
		keyvalue_t mykeyvalue134 = dummykv;
		keyvalue_t mykeyvalue135 = dummykv;
		keyvalue_t mykeyvalue136 = dummykv;
		keyvalue_t mykeyvalue137 = dummykv;
		keyvalue_t mykeyvalue140 = dummykv;
		keyvalue_t mykeyvalue141 = dummykv;
		keyvalue_t mykeyvalue142 = dummykv;
		keyvalue_t mykeyvalue143 = dummykv;
		keyvalue_t mykeyvalue144 = dummykv;
		keyvalue_t mykeyvalue145 = dummykv;
		keyvalue_t mykeyvalue146 = dummykv;
		keyvalue_t mykeyvalue147 = dummykv;
		
		//((n)/2)
		keyA0 = MERGE_mergefunc(mykeyvalue00.key, mykeyvalue10.key, NAp);
		keyB0 = MERGE_mergefunc(mykeyvalue20.key, mykeyvalue30.key, NAp);
		keyC0 = MERGE_mergefunc(mykeyvalue40.key, mykeyvalue50.key, NAp);
		keyD0 = MERGE_mergefunc(mykeyvalue60.key, mykeyvalue70.key, NAp);
		keyE0 = MERGE_mergefunc(mykeyvalue80.key, mykeyvalue90.key, NAp);
		keyF0 = MERGE_mergefunc(mykeyvalue100.key, mykeyvalue110.key, NAp);
		keyG0 = MERGE_mergefunc(mykeyvalue120.key, mykeyvalue130.key, NAp);
		keyA1 = MERGE_mergefunc(mykeyvalue01.key, mykeyvalue11.key, NAp);
		keyB1 = MERGE_mergefunc(mykeyvalue21.key, mykeyvalue31.key, NAp);
		keyC1 = MERGE_mergefunc(mykeyvalue41.key, mykeyvalue51.key, NAp);
		keyD1 = MERGE_mergefunc(mykeyvalue61.key, mykeyvalue71.key, NAp);
		keyE1 = MERGE_mergefunc(mykeyvalue81.key, mykeyvalue91.key, NAp);
		keyF1 = MERGE_mergefunc(mykeyvalue101.key, mykeyvalue111.key, NAp);
		keyG1 = MERGE_mergefunc(mykeyvalue121.key, mykeyvalue131.key, NAp);
		keyA2 = MERGE_mergefunc(mykeyvalue02.key, mykeyvalue12.key, NAp);
		keyB2 = MERGE_mergefunc(mykeyvalue22.key, mykeyvalue32.key, NAp);
		keyC2 = MERGE_mergefunc(mykeyvalue42.key, mykeyvalue52.key, NAp);
		keyD2 = MERGE_mergefunc(mykeyvalue62.key, mykeyvalue72.key, NAp);
		keyE2 = MERGE_mergefunc(mykeyvalue82.key, mykeyvalue92.key, NAp);
		keyF2 = MERGE_mergefunc(mykeyvalue102.key, mykeyvalue112.key, NAp);
		keyG2 = MERGE_mergefunc(mykeyvalue122.key, mykeyvalue132.key, NAp);
		keyA3 = MERGE_mergefunc(mykeyvalue03.key, mykeyvalue13.key, NAp);
		keyB3 = MERGE_mergefunc(mykeyvalue23.key, mykeyvalue33.key, NAp);
		keyC3 = MERGE_mergefunc(mykeyvalue43.key, mykeyvalue53.key, NAp);
		keyD3 = MERGE_mergefunc(mykeyvalue63.key, mykeyvalue73.key, NAp);
		keyE3 = MERGE_mergefunc(mykeyvalue83.key, mykeyvalue93.key, NAp);
		keyF3 = MERGE_mergefunc(mykeyvalue103.key, mykeyvalue113.key, NAp);
		keyG3 = MERGE_mergefunc(mykeyvalue123.key, mykeyvalue133.key, NAp);
		keyA4 = MERGE_mergefunc(mykeyvalue04.key, mykeyvalue14.key, NAp);
		keyB4 = MERGE_mergefunc(mykeyvalue24.key, mykeyvalue34.key, NAp);
		keyC4 = MERGE_mergefunc(mykeyvalue44.key, mykeyvalue54.key, NAp);
		keyD4 = MERGE_mergefunc(mykeyvalue64.key, mykeyvalue74.key, NAp);
		keyE4 = MERGE_mergefunc(mykeyvalue84.key, mykeyvalue94.key, NAp);
		keyF4 = MERGE_mergefunc(mykeyvalue104.key, mykeyvalue114.key, NAp);
		keyG4 = MERGE_mergefunc(mykeyvalue124.key, mykeyvalue134.key, NAp);
		keyA5 = MERGE_mergefunc(mykeyvalue05.key, mykeyvalue15.key, NAp);
		keyB5 = MERGE_mergefunc(mykeyvalue25.key, mykeyvalue35.key, NAp);
		keyC5 = MERGE_mergefunc(mykeyvalue45.key, mykeyvalue55.key, NAp);
		keyD5 = MERGE_mergefunc(mykeyvalue65.key, mykeyvalue75.key, NAp);
		keyE5 = MERGE_mergefunc(mykeyvalue85.key, mykeyvalue95.key, NAp);
		keyF5 = MERGE_mergefunc(mykeyvalue105.key, mykeyvalue115.key, NAp);
		keyG5 = MERGE_mergefunc(mykeyvalue125.key, mykeyvalue135.key, NAp);
		keyA6 = MERGE_mergefunc(mykeyvalue06.key, mykeyvalue16.key, NAp);
		keyB6 = MERGE_mergefunc(mykeyvalue26.key, mykeyvalue36.key, NAp);
		keyC6 = MERGE_mergefunc(mykeyvalue46.key, mykeyvalue56.key, NAp);
		keyD6 = MERGE_mergefunc(mykeyvalue66.key, mykeyvalue76.key, NAp);
		keyE6 = MERGE_mergefunc(mykeyvalue86.key, mykeyvalue96.key, NAp);
		keyF6 = MERGE_mergefunc(mykeyvalue106.key, mykeyvalue116.key, NAp);
		keyG6 = MERGE_mergefunc(mykeyvalue126.key, mykeyvalue136.key, NAp);
		keyA7 = MERGE_mergefunc(mykeyvalue07.key, mykeyvalue17.key, NAp);
		keyB7 = MERGE_mergefunc(mykeyvalue27.key, mykeyvalue37.key, NAp);
		keyC7 = MERGE_mergefunc(mykeyvalue47.key, mykeyvalue57.key, NAp);
		keyD7 = MERGE_mergefunc(mykeyvalue67.key, mykeyvalue77.key, NAp);
		keyE7 = MERGE_mergefunc(mykeyvalue87.key, mykeyvalue97.key, NAp);
		keyF7 = MERGE_mergefunc(mykeyvalue107.key, mykeyvalue117.key, NAp);
		keyG7 = MERGE_mergefunc(mykeyvalue127.key, mykeyvalue137.key, NAp);
		keyy_t keyPA0 = MERGE_mergefunc(keyA0, keyB0, NAp);
		keyy_t keyPB0 = MERGE_mergefunc(keyC0, keyD0, NAp);
		keyy_t keyPC0 = MERGE_mergefunc(keyE0, keyF0, NAp);
		keyy_t keyRA0 = MERGE_mergefunc(keyPA0, keyPB0, NAp);
		keyy_t keyRB0 = MERGE_mergefunc(keyPC0, keyF0, NAp);
		keyy_t keyRZ0 = MERGE_mergefunc(keyRA0, keyRB0, NAp);
		keyy_t keyPA1 = MERGE_mergefunc(keyA1, keyB1, NAp);
		keyy_t keyPB1 = MERGE_mergefunc(keyC1, keyD1, NAp);
		keyy_t keyPC1 = MERGE_mergefunc(keyE1, keyF1, NAp);
		keyy_t keyRA1 = MERGE_mergefunc(keyPA1, keyPB1, NAp);
		keyy_t keyRB1 = MERGE_mergefunc(keyPC1, keyF1, NAp);
		keyy_t keyRZ1 = MERGE_mergefunc(keyRA1, keyRB1, NAp);
		keyy_t keyPA2 = MERGE_mergefunc(keyA2, keyB2, NAp);
		keyy_t keyPB2 = MERGE_mergefunc(keyC2, keyD2, NAp);
		keyy_t keyPC2 = MERGE_mergefunc(keyE2, keyF2, NAp);
		keyy_t keyRA2 = MERGE_mergefunc(keyPA2, keyPB2, NAp);
		keyy_t keyRB2 = MERGE_mergefunc(keyPC2, keyF2, NAp);
		keyy_t keyRZ2 = MERGE_mergefunc(keyRA2, keyRB2, NAp);
		keyy_t keyPA3 = MERGE_mergefunc(keyA3, keyB3, NAp);
		keyy_t keyPB3 = MERGE_mergefunc(keyC3, keyD3, NAp);
		keyy_t keyPC3 = MERGE_mergefunc(keyE3, keyF3, NAp);
		keyy_t keyRA3 = MERGE_mergefunc(keyPA3, keyPB3, NAp);
		keyy_t keyRB3 = MERGE_mergefunc(keyPC3, keyF3, NAp);
		keyy_t keyRZ3 = MERGE_mergefunc(keyRA3, keyRB3, NAp);
		keyy_t keyPA4 = MERGE_mergefunc(keyA4, keyB4, NAp);
		keyy_t keyPB4 = MERGE_mergefunc(keyC4, keyD4, NAp);
		keyy_t keyPC4 = MERGE_mergefunc(keyE4, keyF4, NAp);
		keyy_t keyRA4 = MERGE_mergefunc(keyPA4, keyPB4, NAp);
		keyy_t keyRB4 = MERGE_mergefunc(keyPC4, keyF4, NAp);
		keyy_t keyRZ4 = MERGE_mergefunc(keyRA4, keyRB4, NAp);
		keyy_t keyPA5 = MERGE_mergefunc(keyA5, keyB5, NAp);
		keyy_t keyPB5 = MERGE_mergefunc(keyC5, keyD5, NAp);
		keyy_t keyPC5 = MERGE_mergefunc(keyE5, keyF5, NAp);
		keyy_t keyRA5 = MERGE_mergefunc(keyPA5, keyPB5, NAp);
		keyy_t keyRB5 = MERGE_mergefunc(keyPC5, keyF5, NAp);
		keyy_t keyRZ5 = MERGE_mergefunc(keyRA5, keyRB5, NAp);
		keyy_t keyPA6 = MERGE_mergefunc(keyA6, keyB6, NAp);
		keyy_t keyPB6 = MERGE_mergefunc(keyC6, keyD6, NAp);
		keyy_t keyPC6 = MERGE_mergefunc(keyE6, keyF6, NAp);
		keyy_t keyRA6 = MERGE_mergefunc(keyPA6, keyPB6, NAp);
		keyy_t keyRB6 = MERGE_mergefunc(keyPC6, keyF6, NAp);
		keyy_t keyRZ6 = MERGE_mergefunc(keyRA6, keyRB6, NAp);
		keyy_t keyPA7 = MERGE_mergefunc(keyA7, keyB7, NAp);
		keyy_t keyPB7 = MERGE_mergefunc(keyC7, keyD7, NAp);
		keyy_t keyPC7 = MERGE_mergefunc(keyE7, keyF7, NAp);
		keyy_t keyRA7 = MERGE_mergefunc(keyPA7, keyPB7, NAp);
		keyy_t keyRB7 = MERGE_mergefunc(keyPC7, keyF7, NAp);
		keyy_t keyRZ7 = MERGE_mergefunc(keyRA7, keyRB7, NAp);
		
		//((n)/2)
		valA0 = MERGE_mergefunc(mykeyvalue00.value, mykeyvalue10.value, NAp);
		valB0 = MERGE_mergefunc(mykeyvalue20.value, mykeyvalue30.value, NAp);
		valC0 = MERGE_mergefunc(mykeyvalue40.value, mykeyvalue50.value, NAp);
		valD0 = MERGE_mergefunc(mykeyvalue60.value, mykeyvalue70.value, NAp);
		valE0 = MERGE_mergefunc(mykeyvalue80.value, mykeyvalue90.value, NAp);
		valF0 = MERGE_mergefunc(mykeyvalue100.value, mykeyvalue110.value, NAp);
		valG0 = MERGE_mergefunc(mykeyvalue120.value, mykeyvalue130.value, NAp);
		valA1 = MERGE_mergefunc(mykeyvalue01.value, mykeyvalue11.value, NAp);
		valB1 = MERGE_mergefunc(mykeyvalue21.value, mykeyvalue31.value, NAp);
		valC1 = MERGE_mergefunc(mykeyvalue41.value, mykeyvalue51.value, NAp);
		valD1 = MERGE_mergefunc(mykeyvalue61.value, mykeyvalue71.value, NAp);
		valE1 = MERGE_mergefunc(mykeyvalue81.value, mykeyvalue91.value, NAp);
		valF1 = MERGE_mergefunc(mykeyvalue101.value, mykeyvalue111.value, NAp);
		valG1 = MERGE_mergefunc(mykeyvalue121.value, mykeyvalue131.value, NAp);
		valA2 = MERGE_mergefunc(mykeyvalue02.value, mykeyvalue12.value, NAp);
		valB2 = MERGE_mergefunc(mykeyvalue22.value, mykeyvalue32.value, NAp);
		valC2 = MERGE_mergefunc(mykeyvalue42.value, mykeyvalue52.value, NAp);
		valD2 = MERGE_mergefunc(mykeyvalue62.value, mykeyvalue72.value, NAp);
		valE2 = MERGE_mergefunc(mykeyvalue82.value, mykeyvalue92.value, NAp);
		valF2 = MERGE_mergefunc(mykeyvalue102.value, mykeyvalue112.value, NAp);
		valG2 = MERGE_mergefunc(mykeyvalue122.value, mykeyvalue132.value, NAp);
		valA3 = MERGE_mergefunc(mykeyvalue03.value, mykeyvalue13.value, NAp);
		valB3 = MERGE_mergefunc(mykeyvalue23.value, mykeyvalue33.value, NAp);
		valC3 = MERGE_mergefunc(mykeyvalue43.value, mykeyvalue53.value, NAp);
		valD3 = MERGE_mergefunc(mykeyvalue63.value, mykeyvalue73.value, NAp);
		valE3 = MERGE_mergefunc(mykeyvalue83.value, mykeyvalue93.value, NAp);
		valF3 = MERGE_mergefunc(mykeyvalue103.value, mykeyvalue113.value, NAp);
		valG3 = MERGE_mergefunc(mykeyvalue123.value, mykeyvalue133.value, NAp);
		valA4 = MERGE_mergefunc(mykeyvalue04.value, mykeyvalue14.value, NAp);
		valB4 = MERGE_mergefunc(mykeyvalue24.value, mykeyvalue34.value, NAp);
		valC4 = MERGE_mergefunc(mykeyvalue44.value, mykeyvalue54.value, NAp);
		valD4 = MERGE_mergefunc(mykeyvalue64.value, mykeyvalue74.value, NAp);
		valE4 = MERGE_mergefunc(mykeyvalue84.value, mykeyvalue94.value, NAp);
		valF4 = MERGE_mergefunc(mykeyvalue104.value, mykeyvalue114.value, NAp);
		valG4 = MERGE_mergefunc(mykeyvalue124.value, mykeyvalue134.value, NAp);
		valA5 = MERGE_mergefunc(mykeyvalue05.value, mykeyvalue15.value, NAp);
		valB5 = MERGE_mergefunc(mykeyvalue25.value, mykeyvalue35.value, NAp);
		valC5 = MERGE_mergefunc(mykeyvalue45.value, mykeyvalue55.value, NAp);
		valD5 = MERGE_mergefunc(mykeyvalue65.value, mykeyvalue75.value, NAp);
		valE5 = MERGE_mergefunc(mykeyvalue85.value, mykeyvalue95.value, NAp);
		valF5 = MERGE_mergefunc(mykeyvalue105.value, mykeyvalue115.value, NAp);
		valG5 = MERGE_mergefunc(mykeyvalue125.value, mykeyvalue135.value, NAp);
		valA6 = MERGE_mergefunc(mykeyvalue06.value, mykeyvalue16.value, NAp);
		valB6 = MERGE_mergefunc(mykeyvalue26.value, mykeyvalue36.value, NAp);
		valC6 = MERGE_mergefunc(mykeyvalue46.value, mykeyvalue56.value, NAp);
		valD6 = MERGE_mergefunc(mykeyvalue66.value, mykeyvalue76.value, NAp);
		valE6 = MERGE_mergefunc(mykeyvalue86.value, mykeyvalue96.value, NAp);
		valF6 = MERGE_mergefunc(mykeyvalue106.value, mykeyvalue116.value, NAp);
		valG6 = MERGE_mergefunc(mykeyvalue126.value, mykeyvalue136.value, NAp);
		valA7 = MERGE_mergefunc(mykeyvalue07.value, mykeyvalue17.value, NAp);
		valB7 = MERGE_mergefunc(mykeyvalue27.value, mykeyvalue37.value, NAp);
		valC7 = MERGE_mergefunc(mykeyvalue47.value, mykeyvalue57.value, NAp);
		valD7 = MERGE_mergefunc(mykeyvalue67.value, mykeyvalue77.value, NAp);
		valE7 = MERGE_mergefunc(mykeyvalue87.value, mykeyvalue97.value, NAp);
		valF7 = MERGE_mergefunc(mykeyvalue107.value, mykeyvalue117.value, NAp);
		valG7 = MERGE_mergefunc(mykeyvalue127.value, mykeyvalue137.value, NAp);
		value_t valPA0 = MERGE_mergefunc(valA0, valB0, NAp);
		value_t valPB0 = MERGE_mergefunc(valC0, valD0, NAp);
		value_t valPC0 = MERGE_mergefunc(valE0, valF0, NAp);
		value_t valRA0 = MERGE_mergefunc(valPA0, valPB0, NAp);
		value_t valRB0 = MERGE_mergefunc(valPC0, valF0, NAp);
		value_t valRZ0 = MERGE_mergefunc(valRA0, valRB0, NAp);
		value_t valPA1 = MERGE_mergefunc(valA1, valB1, NAp);
		value_t valPB1 = MERGE_mergefunc(valC1, valD1, NAp);
		value_t valPC1 = MERGE_mergefunc(valE1, valF1, NAp);
		value_t valRA1 = MERGE_mergefunc(valPA1, valPB1, NAp);
		value_t valRB1 = MERGE_mergefunc(valPC1, valF1, NAp);
		value_t valRZ1 = MERGE_mergefunc(valRA1, valRB1, NAp);
		value_t valPA2 = MERGE_mergefunc(valA2, valB2, NAp);
		value_t valPB2 = MERGE_mergefunc(valC2, valD2, NAp);
		value_t valPC2 = MERGE_mergefunc(valE2, valF2, NAp);
		value_t valRA2 = MERGE_mergefunc(valPA2, valPB2, NAp);
		value_t valRB2 = MERGE_mergefunc(valPC2, valF2, NAp);
		value_t valRZ2 = MERGE_mergefunc(valRA2, valRB2, NAp);
		value_t valPA3 = MERGE_mergefunc(valA3, valB3, NAp);
		value_t valPB3 = MERGE_mergefunc(valC3, valD3, NAp);
		value_t valPC3 = MERGE_mergefunc(valE3, valF3, NAp);
		value_t valRA3 = MERGE_mergefunc(valPA3, valPB3, NAp);
		value_t valRB3 = MERGE_mergefunc(valPC3, valF3, NAp);
		value_t valRZ3 = MERGE_mergefunc(valRA3, valRB3, NAp);
		value_t valPA4 = MERGE_mergefunc(valA4, valB4, NAp);
		value_t valPB4 = MERGE_mergefunc(valC4, valD4, NAp);
		value_t valPC4 = MERGE_mergefunc(valE4, valF4, NAp);
		value_t valRA4 = MERGE_mergefunc(valPA4, valPB4, NAp);
		value_t valRB4 = MERGE_mergefunc(valPC4, valF4, NAp);
		value_t valRZ4 = MERGE_mergefunc(valRA4, valRB4, NAp);
		value_t valPA5 = MERGE_mergefunc(valA5, valB5, NAp);
		value_t valPB5 = MERGE_mergefunc(valC5, valD5, NAp);
		value_t valPC5 = MERGE_mergefunc(valE5, valF5, NAp);
		value_t valRA5 = MERGE_mergefunc(valPA5, valPB5, NAp);
		value_t valRB5 = MERGE_mergefunc(valPC5, valF5, NAp);
		value_t valRZ5 = MERGE_mergefunc(valRA5, valRB5, NAp);
		value_t valPA6 = MERGE_mergefunc(valA6, valB6, NAp);
		value_t valPB6 = MERGE_mergefunc(valC6, valD6, NAp);
		value_t valPC6 = MERGE_mergefunc(valE6, valF6, NAp);
		value_t valRA6 = MERGE_mergefunc(valPA6, valPB6, NAp);
		value_t valRB6 = MERGE_mergefunc(valPC6, valF6, NAp);
		value_t valRZ6 = MERGE_mergefunc(valRA6, valRB6, NAp);
		value_t valPA7 = MERGE_mergefunc(valA7, valB7, NAp);
		value_t valPB7 = MERGE_mergefunc(valC7, valD7, NAp);
		value_t valPC7 = MERGE_mergefunc(valE7, valF7, NAp);
		value_t valRA7 = MERGE_mergefunc(valPA7, valPB7, NAp);
		value_t valRB7 = MERGE_mergefunc(valPC7, valF7, NAp);
		value_t valRZ7 = MERGE_mergefunc(valRA7, valRB7, NAp);
		
		#ifdef _WIDEWORD
		vdram[dramoffset_kvs + j].range(31, 0) = keyRZ0;
		vdram[dramoffset_kvs + j].range(63, 32) = valRZ0;
		vdram[dramoffset_kvs + j].range(95, 64) = keyRZ1;
		vdram[dramoffset_kvs + j].range(127, 96) = valRZ1;
		vdram[dramoffset_kvs + j].range(159, 128) = keyRZ2;
		vdram[dramoffset_kvs + j].range(191, 160) = valRZ2;
		vdram[dramoffset_kvs + j].range(223, 192) = keyRZ3;
		vdram[dramoffset_kvs + j].range(255, 224) = valRZ3;
		vdram[dramoffset_kvs + j].range(287, 256) = keyRZ4;
		vdram[dramoffset_kvs + j].range(319, 288) = valRZ4;
		vdram[dramoffset_kvs + j].range(351, 320) = keyRZ5;
		vdram[dramoffset_kvs + j].range(383, 352) = valRZ5;
		vdram[dramoffset_kvs + j].range(415, 384) = keyRZ6;
		vdram[dramoffset_kvs + j].range(447, 416) = valRZ6;
		vdram[dramoffset_kvs + j].range(479, 448) = keyRZ7;
		vdram[dramoffset_kvs + j].range(511, 480) = valRZ7;
		#else
		vdram[dramoffset_kvs + j].data[0].key = keyRZ0;
		vdram[dramoffset_kvs + j].data[0].value = valRZ0;
		vdram[dramoffset_kvs + j].data[1].key = keyRZ1;
		vdram[dramoffset_kvs + j].data[1].value = valRZ1;
		vdram[dramoffset_kvs + j].data[2].key = keyRZ2;
		vdram[dramoffset_kvs + j].data[2].value = valRZ2;
		vdram[dramoffset_kvs + j].data[3].key = keyRZ3;
		vdram[dramoffset_kvs + j].data[3].value = valRZ3;
		vdram[dramoffset_kvs + j].data[4].key = keyRZ4;
		vdram[dramoffset_kvs + j].data[4].value = valRZ4;
		vdram[dramoffset_kvs + j].data[5].key = keyRZ5;
		vdram[dramoffset_kvs + j].data[5].value = valRZ5;
		vdram[dramoffset_kvs + j].data[6].key = keyRZ6;
		vdram[dramoffset_kvs + j].data[6].value = valRZ6;
		vdram[dramoffset_kvs + j].data[7].key = keyRZ7;
		vdram[dramoffset_kvs + j].data[7].value = valRZ7;
		#endif
	}
	return;
}

//
unsigned int 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_actvpstatsoffset(globalparams_t globalparams){
	unsigned int actvpstats_beginoffset = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ actvpstats_beginoffset += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ actvpstats_beginoffset += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif
	return actvpstats_beginoffset;
}

value_t 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	if(GraphAlgo == PAGERANK){
		// temp = vtemp + res;
		// temp = 0.5 + (1 - 0.5)*res / vdeg; // IDEAL
		temp = res;
	} else if(GraphAlgo == BFS){
		temp = res;
	} else if(GraphAlgo == SSSP){
		temp = acts_utilobj->UTIL_amin(vtemp, res);
	} else {
		temp = NAp;
	}
	return temp;
}

void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readvdata(bool_type enable1, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable1 == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
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
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].data[7].value; 
		#endif
		
		buffer[0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer[1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer[2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer[3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer[4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer[5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer[6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer[7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}

void 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_writevdata(bool_type enable1, uint512_dt * kvdram, batch_type basedramoffset_kvs, batch_type localdramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable1 == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	unsigned int dramoffset_kvs = basedramoffset_kvs + localdramoffset_kvs;

	WRITEVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = acts_utilobj->UTIL_GETKV2(buffer[0][i]); 
			keyvalue_t keyvalue1 = acts_utilobj->UTIL_GETKV2(buffer[1][i]); 
			keyvalue_t keyvalue2 = acts_utilobj->UTIL_GETKV2(buffer[2][i]); 
			keyvalue_t keyvalue3 = acts_utilobj->UTIL_GETKV2(buffer[3][i]); 
			keyvalue_t keyvalue4 = acts_utilobj->UTIL_GETKV2(buffer[4][i]); 
			keyvalue_t keyvalue5 = acts_utilobj->UTIL_GETKV2(buffer[5][i]); 
			keyvalue_t keyvalue6 = acts_utilobj->UTIL_GETKV2(buffer[6][i]); 
			keyvalue_t keyvalue7 = acts_utilobj->UTIL_GETKV2(buffer[7][i]); 
			
			#ifdef _WIDEWORD
			kvdram[dramoffset_kvs + i].range(31, 0) = keyvalue0.key;
			kvdram[dramoffset_kvs + i].range(63, 32) = keyvalue0.value;
			kvdram[dramoffset_kvs + i].range(95, 64) = keyvalue1.key;
			kvdram[dramoffset_kvs + i].range(127, 96) = keyvalue1.value;
			kvdram[dramoffset_kvs + i].range(159, 128) = keyvalue2.key;
			kvdram[dramoffset_kvs + i].range(191, 160) = keyvalue2.value;
			kvdram[dramoffset_kvs + i].range(223, 192) = keyvalue3.key;
			kvdram[dramoffset_kvs + i].range(255, 224) = keyvalue3.value;
			kvdram[dramoffset_kvs + i].range(287, 256) = keyvalue4.key;
			kvdram[dramoffset_kvs + i].range(319, 288) = keyvalue4.value;
			kvdram[dramoffset_kvs + i].range(351, 320) = keyvalue5.key;
			kvdram[dramoffset_kvs + i].range(383, 352) = keyvalue5.value;
			kvdram[dramoffset_kvs + i].range(415, 384) = keyvalue6.key;
			kvdram[dramoffset_kvs + i].range(447, 416) = keyvalue6.value;
			kvdram[dramoffset_kvs + i].range(479, 448) = keyvalue7.key;
			kvdram[dramoffset_kvs + i].range(511, 480) = keyvalue7.value;
			#else
			kvdram[dramoffset_kvs + i].data[0].key = keyvalue0.key; 
			kvdram[dramoffset_kvs + i].data[0].value = keyvalue0.value;
			kvdram[dramoffset_kvs + i].data[1].key = keyvalue1.key; 
			kvdram[dramoffset_kvs + i].data[1].value = keyvalue1.value;
			kvdram[dramoffset_kvs + i].data[2].key = keyvalue2.key; 
			kvdram[dramoffset_kvs + i].data[2].value = keyvalue2.value;
			kvdram[dramoffset_kvs + i].data[3].key = keyvalue3.key; 
			kvdram[dramoffset_kvs + i].data[3].value = keyvalue3.value;
			kvdram[dramoffset_kvs + i].data[4].key = keyvalue4.key; 
			kvdram[dramoffset_kvs + i].data[4].value = keyvalue4.value;
			kvdram[dramoffset_kvs + i].data[5].key = keyvalue5.key; 
			kvdram[dramoffset_kvs + i].data[5].value = keyvalue5.value;
			kvdram[dramoffset_kvs + i].data[6].key = keyvalue6.key; 
			kvdram[dramoffset_kvs + i].data[6].value = keyvalue6.value;
			kvdram[dramoffset_kvs + i].data[7].key = keyvalue7.key; 
			kvdram[dramoffset_kvs + i].data[7].value = keyvalue7.value;
			#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> ACTS MERGE: MERGE_writevdata: data written at: dramoffset: "<<localdramoffset_kvs*VECTOR2_SIZE<<", dramoffset_kvs: "<<localdramoffset_kvs<<", size_kvs: "<<size_kvs<<endl;
	#endif
	return;
}

void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram1]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram1]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram1]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram2]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram2]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram2]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram3]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram3]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram3]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram4]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram4]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram4]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram5]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram5]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram5]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram6]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram6]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram6]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram7]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram7]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram7]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram8]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram8]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram8]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram9]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram9]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram9]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram10]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram10]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram10]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram11]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram11]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram11]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW
	acts_merge::
	#endif
MERGE_broadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	unsigned int _CHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;

	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	/* ///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[i] = 1;
	}
	///////////////////////////////////////////////////////// */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram12]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += _CHUNKSZ_KVS2; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_CHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram11[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _CHUNKSZ_KVS2; // TOO EXPENSIVE.
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram12]."<<endl; 	
	#endif
	/////////////////////////////////////////////////////// CRITICAL REMOVEME.
	for(unsigned int k=0; k<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
			kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].key = 0;
			kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k].data[v].value = 0;
		}
	}
	///////////////////////////////////////////////////////
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				if(psizes_kvs[(partition * NUM_PEs) + s] == 0){ offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; continue; } // TOO EXPENSIVE.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs; // TOO EXPENSIVE.
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram12]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram11[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	// exit(EXIT_SUCCESS); //
	return;
}

void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[1][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<1; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 1:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 1:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 1:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 1:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 1:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs1: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[2][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<2; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 2:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 2:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 2:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 2:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 2:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 2:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 2:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 2:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 2:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs2: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<3; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 3:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 3:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 3:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 3:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 3:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 3:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 3:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs3: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[4][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<4; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 4:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 4:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 4:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 4:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 4:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 4:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 4:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 4:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 4:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs4: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[5][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<5; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 5:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 5:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 5:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 5:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 5:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 5:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs5: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[6][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<6; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 6:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 6:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 6:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 6:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 6:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 6:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 6:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs6: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[7][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<7; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 7:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 7:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 7:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 7:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 7:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 7:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 7:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 7:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs7: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[8][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<8; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 8:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 8:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 8:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 8:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 8:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 8:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 8:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 8:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 8:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs8: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[9][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<9; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[8], globalparamsv, actvpstats_beginoffset + 8 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 9:: [instance 8, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 9:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs9: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[10][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<10; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[8], globalparamsv, actvpstats_beginoffset + 8 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[9], globalparamsv, actvpstats_beginoffset + 9 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 8, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 10:: [instance 9, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 10:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs10: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[11][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram10, psizes_kvs[10], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<11; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[8], globalparamsv, actvpstats_beginoffset + 8 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[9], globalparamsv, actvpstats_beginoffset + 9 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[10], globalparamsv, actvpstats_beginoffset + 10 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 8, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 9, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 11:: [instance 10, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 10, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 11:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs11: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[12][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram10, psizes_kvs[10], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram11, psizes_kvs[11], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.	
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		for(unsigned int j=0; j<12; j++){
			psizes_kvs[j][i] = 1;
		}
	} */
	/////////////////////////////////////////////////////////	
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[8], globalparamsv, actvpstats_beginoffset + 8 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[9], globalparamsv, actvpstats_beginoffset + 9 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[10], globalparamsv, actvpstats_beginoffset + 10 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[11], globalparamsv, actvpstats_beginoffset + 11 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_MERGEVSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 11, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[11][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 11, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 8, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 9, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 10, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 10, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGE_merge Masks 12:: [instance 11, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
		#endif 
		if(psizes_kvs[11][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 11, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram11[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGE_merge Partition Masks 12:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_mergeVs12: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_exchangeVs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * vdram){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: ACTS EXCHANGE LAUNCHED. Exchanging vertices across different SLRs..."<<endl; 
	#endif

	unsigned int psizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionoffseti = 0;
	
	globalparams_t globalparamsvA = acts_utilobj->UTIL_getglobalparams(vdramA);
	globalparams_t globalparamsvB = acts_utilobj->UTIL_getglobalparams(vdramB);
	globalparams_t globalparamsvC = acts_utilobj->UTIL_getglobalparams(vdramC);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsvA);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif 
	mem_accessobj->MEMACCESS_retreievekvstats(vdramA, psizes_kvs[0], globalparamsvA, actvpstats_beginoffset, globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR2);
	mem_accessobj->MEMACCESS_retreievekvstats(vdramB, psizes_kvs[1], globalparamsvB, actvpstats_beginoffset, globalparamsvB.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1);
	mem_accessobj->MEMACCESS_retreievekvstats(vdramC, psizes_kvs[2], globalparamsvC, actvpstats_beginoffset, globalparamsvC.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR0);
	///////////////////////////////////////////////////////// CRITICAL REMOVEME.
	/* for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
		psizes_kvs[0][i] = 1;
		psizes_kvs[1][i] = 1;
		psizes_kvs[2][i] = 1;
	} */
	/////////////////////////////////////////////////////////
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: transferring stats from vdramA, vdramB & vdramC to vdramA. "<<endl; 
	#endif 
	mem_accessobj->MEMACCESS_commitkvstats2(vdramA, psizes_kvs[0], globalparamsvA, actvpstats_beginoffset + 0, (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2));
	mem_accessobj->MEMACCESS_commitkvstats2(vdramA, psizes_kvs[1], globalparamsvA, actvpstats_beginoffset + (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2), (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2));
	mem_accessobj->MEMACCESS_commitkvstats2(vdramA, psizes_kvs[2], globalparamsvA, actvpstats_beginoffset + (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2) + (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2), (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR0));

	// vertices
	partitionoffseti = 0;
	unsigned int voffsetA_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGE_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramA to vdramB & vdramC. [end offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	
	partitionoffseti = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramB to vdramA & vdramC. [begin offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	MERGE_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramB to vdramA & vdramC. [end offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	
	partitionoffseti = 0;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramC to vdramA & vdramB. [begin offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	MERGE_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){ // NUMCOMPUTEUNITS_SLR1AND2
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	
	// vertices masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices masks from vdramA to vdramB & vdramC."<<endl; 
	#endif
	partitionoffseti = 0;
	unsigned int vmoffsetA_kvs = 0;
	MERGE_EXCHANGEVMS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vmasks ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<vmoffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVMS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS_VMASKBUFFER; k++){
				#pragma HLS PIPELINE II=1
					vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetA_kvs + k];
				}
			}
			partitionoffseti += 1;
			vmoffsetA_kvs += globalparamsvA.SIZEKVS_VMASKBUFFER;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices masks from vdramB to vdramA & vdramC."<<endl; 
	#endif
	partitionoffseti = 0;
	unsigned int vmoffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS_VMASKBUFFER;
	MERGE_EXCHANGEVMS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vmasks ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<vmoffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVMS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS_VMASKBUFFER; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetB_kvs + k];
				}
			}
			partitionoffseti += 1;
			vmoffsetB_kvs += globalparamsvB.SIZEKVS_VMASKBUFFER;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices masks from vdramC to vdramA & vdramB."<<endl; 
	#endif
	partitionoffseti = 0;
	unsigned int vmoffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS_VMASKBUFFER;
	MERGE_EXCHANGEVMS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vmasks ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<vmoffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVMS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS_VMASKBUFFER; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k];
				}
			}
			partitionoffseti += 1;
			vmoffsetC_kvs += globalparamsvC.SIZEKVS_VMASKBUFFER;
		}
	}
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: merging vertices partition masks across vdramA, vdramB & vdramC."<<endl; 
	#endif
	unsigned int pA = 0; unsigned int pB = 0; unsigned int pC = 0; 
	MERGE_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; partition++){
		#ifdef _WIDEWORD
		pA = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		pB = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		pC = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		pA = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		pB = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		pC = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
		
		unsigned int p = pA | pB | pC;
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(p > 0){ cout<<"MERGE_exchangeVs:: partition mask: "<<partition<<" is active: pA: "<<pA<<", pB: "<<pB<<", pC: "<<pC<<endl; }
		#endif
		// cout<<"MERGE_exchangeVs:: partition: "<<partition<<": pA: "<<pA<<", pB: "<<pB<<", pC: "<<pC<<endl;
		
		#ifdef _WIDEWORD
		vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		#else 
		vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"--- MERGE_exchangeVs: vdramA["<<partition<<"]: "<<p<<endl; 
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_exchangeVs: Finished. voffsetA: "<<voffsetA_kvs * VECTOR_SIZE<<", voffsetB: "<<voffsetB_kvs * VECTOR_SIZE<<", voffsetC: "<<voffsetC_kvs * VECTOR_SIZE<<endl;
	cout<<"MERGE_exchangeVs: Finished. vmoffsetA: "<<vmoffsetA_kvs * VECTOR_SIZE<<", vmoffsetB: "<<vmoffsetB_kvs * VECTOR_SIZE<<", vmoffsetC: "<<vmoffsetC_kvs * VECTOR_SIZE<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
 
