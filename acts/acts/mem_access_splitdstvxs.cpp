#include "mem_access_splitdstvxs.h"
using namespace std;

#ifdef SW
mem_access_splitdstvxs::mem_access_splitdstvxs(mydebug * _mydebugobj){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(_mydebugobj); 
	mem_accessobj = new mem_access(_mydebugobj); 
	mydebugobj = _mydebugobj;
}
mem_access_splitdstvxs::~mem_access_splitdstvxs(){}
#endif

// -------------------- vdata -------------------- //
void MEMACCESS_SPL_GetXYLayoutV(unsigned int s, value_t vdata[VECTOR2_SIZE], value_t vdata2[VECTOR2_SIZE]){
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ vdata2[(v+s) % VECTOR2_SIZE] = vdata[v]; } 
	return;
}

void MEMACCESS_SPL_readvdata(bool_type enable, unsigned int s, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / 2;
	
	unsigned int index = 0;
	value_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t vdata2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata2 complete
	keyvalue_t keyvalue0; keyvalue0.key = 777; keyvalue0.value = 777;
	keyvalue_t keyvalue1; keyvalue1.key = 777; keyvalue1.value = 777;
	keyvalue_t keyvalue2; keyvalue2.key = 777; keyvalue2.value = 777;
	keyvalue_t keyvalue3; keyvalue3.key = 777; keyvalue3.value = 777;
	keyvalue_t keyvalue4; keyvalue4.key = 777; keyvalue4.value = 777;
	keyvalue_t keyvalue5; keyvalue5.key = 777; keyvalue5.value = 777;
	keyvalue_t keyvalue6; keyvalue6.key = 777; keyvalue6.value = 777;
	keyvalue_t keyvalue7; keyvalue7.key = 777; keyvalue7.value = 777;
	keyvalue_t keyvalue8; keyvalue8.key = 777; keyvalue8.value = 777;
	keyvalue_t keyvalue9; keyvalue9.key = 777; keyvalue9.value = 777;
	keyvalue_t keyvalue10; keyvalue10.key = 777; keyvalue10.value = 777;
	keyvalue_t keyvalue11; keyvalue11.key = 777; keyvalue11.value = 777;
	keyvalue_t keyvalue12; keyvalue12.key = 777; keyvalue12.value = 777;
	keyvalue_t keyvalue13; keyvalue13.key = 777; keyvalue13.value = 777;
	keyvalue_t keyvalue14; keyvalue14.key = 777; keyvalue14.value = 777;
	keyvalue_t keyvalue15; keyvalue15.key = 777; keyvalue15.value = 777;
	 for(unsigned int v=0; v<VECTOR2_SIZE; v++){ vdata[v] = 0; vdata2[v] = 0; }
	
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
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
		
		#ifdef CONFIG_ACTSPROCESSEDGES_SPREADVTXREAD
		MEMACCESS_SPL_GetXYLayoutV(s, vdata, vdata2);
		if(i%2==0){
			keyvalue0.key = vdata2[0];		
			keyvalue1.key = vdata2[1];		
			keyvalue2.key = vdata2[2];		
			keyvalue3.key = vdata2[3];		
			keyvalue4.key = vdata2[4];		
			keyvalue5.key = vdata2[5];		
			keyvalue6.key = vdata2[6];		
			keyvalue7.key = vdata2[7];		
			keyvalue8.key = vdata2[8];		
			keyvalue9.key = vdata2[9];		
			keyvalue10.key = vdata2[10];		
			keyvalue11.key = vdata2[11];		
			keyvalue12.key = vdata2[12];		
			keyvalue13.key = vdata2[13];		
			keyvalue14.key = vdata2[14];		
			keyvalue15.key = vdata2[15];		
		} else {
			keyvalue0.value = vdata2[0];		
			keyvalue1.value = vdata2[1];		
			keyvalue2.value = vdata2[2];		
			keyvalue3.value = vdata2[3];		
			keyvalue4.value = vdata2[4];		
			keyvalue5.value = vdata2[5];		
			keyvalue6.value = vdata2[6];		
			keyvalue7.value = vdata2[7];		
			keyvalue8.value = vdata2[8];		
			keyvalue9.value = vdata2[9];		
			keyvalue10.value = vdata2[10];		
			keyvalue11.value = vdata2[11];		
			keyvalue12.value = vdata2[12];		
			keyvalue13.value = vdata2[13];		
			keyvalue14.value = vdata2[14];		
			keyvalue15.value = vdata2[15];		
		}
		#else 
		if(i%2==0){
			keyvalue0.key = vdata[0];		
			keyvalue1.key = vdata[1];		
			keyvalue2.key = vdata[2];		
			keyvalue3.key = vdata[3];		
			keyvalue4.key = vdata[4];		
			keyvalue5.key = vdata[5];		
			keyvalue6.key = vdata[6];		
			keyvalue7.key = vdata[7];		
			keyvalue8.key = vdata[8];		
			keyvalue9.key = vdata[9];		
			keyvalue10.key = vdata[10];		
			keyvalue11.key = vdata[11];		
			keyvalue12.key = vdata[12];		
			keyvalue13.key = vdata[13];		
			keyvalue14.key = vdata[14];		
			keyvalue15.key = vdata[15];		
		} else {
			keyvalue0.value = vdata[0];		
			keyvalue1.value = vdata[1];		
			keyvalue2.value = vdata[2];		
			keyvalue3.value = vdata[3];		
			keyvalue4.value = vdata[4];		
			keyvalue5.value = vdata[5];		
			keyvalue6.value = vdata[6];		
			keyvalue7.value = vdata[7];		
			keyvalue8.value = vdata[8];		
			keyvalue9.value = vdata[9];		
			keyvalue10.value = vdata[10];		
			keyvalue11.value = vdata[11];		
			keyvalue12.value = vdata[12];		
			keyvalue13.value = vdata[13];		
			keyvalue14.value = vdata[14];		
			keyvalue15.value = vdata[15];		
		}
		#endif 
	
		if(i%2==1){ 
			buffer[0][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue0);		
			buffer[1][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue1);		
			buffer[2][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue2);		
			buffer[3][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue3);		
			buffer[4][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue4);		
			buffer[5][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue5);		
			buffer[6][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue6);		
			buffer[7][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue7);		
			buffer[8][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue8);		
			buffer[9][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue9);		
			buffer[10][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue10);		
			buffer[11][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue11);		
			buffer[12][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue12);		
			buffer[13][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue13);		
			buffer[14][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue14);		
			buffer[15][bufferoffset_kvs + index] = UTIL_GETKV2(keyvalue15);		
			index += 1;
		}
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savevdata:: vertices saved: offset: "<<dramoffset_kvs * VECTOR_SIZE<<"-"<<(dramoffset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	return;
}

void MEMACCESS_SPL_readvdatachunks(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs / 2;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	for(unsigned int s=0; s<NUM_PEs; s++){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESS_SPL_readvdatachunks:: size loaded @ s("<<s<<"): offset_kvs2: "<<(s * vsz_kvs)<<", sz_kvs2: "<<vsz_kvs<<endl;
		#endif 
		MEMACCESS_SPL_readvdata(enable, s, kvdram, buffer, vbaseoffset_kvs, depth_i + voffset_kvs, bdepth_i, vsz_kvs, globalparams);
		depth_i += depth;
		bdepth_i += bdepth;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"+++ MEMACCESS_SPL_readvdatachunks:: total size loaded: "<<vsz_kvs * NUM_PEs * VECTOR2_SIZE<<endl;
	#endif
	return;	
}

void MEMACCESS_SPL_savevdata(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount =  REDUCESZ / 2;
	
	SAVEVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0 = UTIL_GETKV2(buffer[0][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue1 = UTIL_GETKV2(buffer[1][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue2 = UTIL_GETKV2(buffer[2][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue3 = UTIL_GETKV2(buffer[3][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue4 = UTIL_GETKV2(buffer[4][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue5 = UTIL_GETKV2(buffer[5][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue6 = UTIL_GETKV2(buffer[6][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue7 = UTIL_GETKV2(buffer[7][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue8 = UTIL_GETKV2(buffer[8][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue9 = UTIL_GETKV2(buffer[9][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue10 = UTIL_GETKV2(buffer[10][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue11 = UTIL_GETKV2(buffer[11][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue12 = UTIL_GETKV2(buffer[12][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue13 = UTIL_GETKV2(buffer[13][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue14 = UTIL_GETKV2(buffer[14][bufferoffset_kvs + (i/2)]);
		keyvalue_t mykeyvalue15 = UTIL_GETKV2(buffer[15][bufferoffset_kvs + (i/2)]);
	
		#ifdef _WIDEWORD
		if(i%2==0){
			kvdram[baseoffset_kvs + offset_kvs + i].range(31, 0) = mykeyvalue0.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(63, 32) = mykeyvalue1.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(95, 64) = mykeyvalue2.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(127, 96) = mykeyvalue3.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(159, 128) = mykeyvalue4.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(191, 160) = mykeyvalue5.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(223, 192) = mykeyvalue6.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(255, 224) = mykeyvalue7.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(287, 256) = mykeyvalue8.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(319, 288) = mykeyvalue9.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(351, 320) = mykeyvalue10.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(383, 352) = mykeyvalue11.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(415, 384) = mykeyvalue12.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(447, 416) = mykeyvalue13.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(479, 448) = mykeyvalue14.key; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(511, 480) = mykeyvalue15.key; 
		} else {
			kvdram[baseoffset_kvs + offset_kvs + i].range(31, 0) = mykeyvalue0.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(63, 32) = mykeyvalue1.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(95, 64) = mykeyvalue2.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(127, 96) = mykeyvalue3.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(159, 128) = mykeyvalue4.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(191, 160) = mykeyvalue5.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(223, 192) = mykeyvalue6.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(255, 224) = mykeyvalue7.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(287, 256) = mykeyvalue8.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(319, 288) = mykeyvalue9.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(351, 320) = mykeyvalue10.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(383, 352) = mykeyvalue11.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(415, 384) = mykeyvalue12.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(447, 416) = mykeyvalue13.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(479, 448) = mykeyvalue14.value; 
			kvdram[baseoffset_kvs + offset_kvs + i].range(511, 480) = mykeyvalue15.value; 
		}
		#else 
		if(i%2==0){
			kvdram[baseoffset_kvs + offset_kvs + i].data[0].key = mykeyvalue0.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[0].value = mykeyvalue1.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[1].key = mykeyvalue2.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[1].value = mykeyvalue3.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[2].key = mykeyvalue4.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[2].value = mykeyvalue5.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[3].key = mykeyvalue6.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[3].value = mykeyvalue7.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[4].key = mykeyvalue8.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[4].value = mykeyvalue9.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[5].key = mykeyvalue10.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[5].value = mykeyvalue11.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[6].key = mykeyvalue12.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[6].value = mykeyvalue13.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[7].key = mykeyvalue14.key;
			kvdram[baseoffset_kvs + offset_kvs + i].data[7].value = mykeyvalue15.key;
		} else {
			kvdram[baseoffset_kvs + offset_kvs + i].data[0].key = mykeyvalue0.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[0].value = mykeyvalue1.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[1].key = mykeyvalue2.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[1].value = mykeyvalue3.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[2].key = mykeyvalue4.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[2].value = mykeyvalue5.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[3].key = mykeyvalue6.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[3].value = mykeyvalue7.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[4].key = mykeyvalue8.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[4].value = mykeyvalue9.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[5].key = mykeyvalue10.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[5].value = mykeyvalue11.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[6].key = mykeyvalue12.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[6].value = mykeyvalue13.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[7].key = mykeyvalue14.value;
			kvdram[baseoffset_kvs + offset_kvs + i].data[7].value = mykeyvalue15.value;
		}
		#endif
	
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"savevdata:: vertices saved: offset: "<<(offset_kvs) * VECTOR_SIZE<<"-"<<(offset_kvs + size_kvs) * VECTOR_SIZE<<", number of vertex datas written: "<<(size_kvs * VECTOR_SIZE * 2)<<" ("<<size_kvs * VECTOR_SIZE<<" keyvalues written)"<<endl;
	#endif
	return;
}

// -------------------- vmasks -------------------- //
void MEMACCESS_SPL_GetXYLayoutVM(unsigned int s, unit1_type vmdata[VECTOR2_SIZE], unit1_type vmdata2[VECTOR2_SIZE]){
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ vmdata2[(v+s) % VECTOR2_SIZE] = vmdata[v]; }
	return;
}

void MEMACCESS_SPL_readvmasks(bool_type enable, unsigned int s, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type lbaseoffset_kvs, batch_type offset_kvs, buffer_type bufferoffset, buffer_type size_kvs){			
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount1 = BLOCKRAM_SIZE;
	analysis_type analysis_loopcount2 = BLOCKRAM_SIZE / 16;
	analysis_type analysis_loopcount3 = BLOCKRAM_SIZE;
	
	// vmask is essentially vdata shrinked
	unsigned int rlbaseoffset_kvs = lbaseoffset_kvs / FACTOR_REDUCEPARTITIONSZ_VMASKSZ;
	unsigned int roffset_kvs = offset_kvs / FACTOR_REDUCEPARTITIONSZ_VMASKSZ; // AUTOMATEME. 'FACTOR_REDUCEPARTITIONSZ_VMASKSZ' changes with dataset
	unsigned int rbufferoffset = bufferoffset / FACTOR_REDUCEPARTITIONSZ_VMASKSZ;
	unsigned int rsize_kvs = size_kvs / FACTOR_REDUCEPARTITIONSZ_VMASKSZ; // 32
	
	uint32_type bitsbuffer[BLOCKRAM_SIZE];
	unit1_type vmdata1[VMASK_PACKINGSIZE];
	unit1_type vmdata1b[VMASK_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable=vmdata1 complete
	#pragma HLS ARRAY_PARTITION variable=vmdata1b complete
	unit1_type vmdata2[VMASK_PACKINGSIZE];
	unit1_type vmdata2b[VMASK_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable=vmdata2 complete
	#pragma HLS ARRAY_PARTITION variable=vmdata2b complete

	// read 
	READVMASKS_LOOP: for (buffer_type i=0; i<rsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount1 avg=analysis_loopcount1
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
	
		tempbuffer[0][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(31, 0); 
		tempbuffer[1][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(63, 32);			
	
		tempbuffer[2][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(95, 64); 
		tempbuffer[3][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(127, 96);			
	
		tempbuffer[4][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(159, 128); 
		tempbuffer[5][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(191, 160);			
	
		tempbuffer[6][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(223, 192); 
		tempbuffer[7][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(255, 224);			
	
		tempbuffer[8][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(287, 256); 
		tempbuffer[9][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(319, 288);			
	
		tempbuffer[10][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(351, 320); 
		tempbuffer[11][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(383, 352);			
	
		tempbuffer[12][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(415, 384); 
		tempbuffer[13][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(447, 416);			
	
		tempbuffer[14][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(479, 448); 
		tempbuffer[15][i] = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].range(511, 480);			
		#else 
		tempbuffer[0][i].key = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[0].key;
		tempbuffer[0][i].value = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[0].value; 
		tempbuffer[1][i].key = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[1].key;
		tempbuffer[1][i].value = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[1].value; 
		tempbuffer[2][i].key = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[2].key;
		tempbuffer[2][i].value = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[2].value; 
		tempbuffer[3][i].key = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[3].key;
		tempbuffer[3][i].value = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[3].value; 
		tempbuffer[4][i].key = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[4].key;
		tempbuffer[4][i].value = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[4].value; 
		tempbuffer[5][i].key = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[5].key;
		tempbuffer[5][i].value = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[5].value; 
		tempbuffer[6][i].key = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[6].key;
		tempbuffer[6][i].value = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[6].value; 
		tempbuffer[7][i].key = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[7].key;
		tempbuffer[7][i].value = kvdram[baseoffset_kvs + rlbaseoffset_kvs + roffset_kvs + i].data[7].value; 
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MEMACCESS_SPL_readvmasks:", rlbaseoffset_kvs + roffset_kvs, NUMPROCESSEDGESPARTITIONS * VMASKBUFFERSZ_KVS, baseoffset_kvs, rlbaseoffset_kvs, roffset_kvs);
		#endif
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	
	// u512 => u32s
	buffer_type index = 0;
	LOADVMASKS_LOOP1: for (buffer_type i=0; i<rsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount2 avg=analysis_loopcount2
	#pragma HLS PIPELINE II=8
		#ifdef _WIDEWORD
		bitsbuffer[index + 0] = tempbuffer[0][i];
		bitsbuffer[index + 1] = tempbuffer[1][i];
		bitsbuffer[index + 2] = tempbuffer[2][i];
		bitsbuffer[index + 3] = tempbuffer[3][i];
		bitsbuffer[index + 4] = tempbuffer[4][i];
		bitsbuffer[index + 5] = tempbuffer[5][i];
		bitsbuffer[index + 6] = tempbuffer[6][i];
		bitsbuffer[index + 7] = tempbuffer[7][i];
		bitsbuffer[index + 8] = tempbuffer[8][i];
		bitsbuffer[index + 9] = tempbuffer[9][i];
		bitsbuffer[index + 10] = tempbuffer[10][i];
		bitsbuffer[index + 11] = tempbuffer[11][i];
		bitsbuffer[index + 12] = tempbuffer[12][i];
		bitsbuffer[index + 13] = tempbuffer[13][i];
		bitsbuffer[index + 14] = tempbuffer[14][i];
		bitsbuffer[index + 15] = tempbuffer[15][i];
		#else 
		bitsbuffer[index + 0] = tempbuffer[0][i].key;
		bitsbuffer[index + 0 + 1] = tempbuffer[0][i].value;
		bitsbuffer[index + 2] = tempbuffer[1][i].key;
		bitsbuffer[index + 2 + 1] = tempbuffer[1][i].value;
		bitsbuffer[index + 4] = tempbuffer[2][i].key;
		bitsbuffer[index + 4 + 1] = tempbuffer[2][i].value;
		bitsbuffer[index + 6] = tempbuffer[3][i].key;
		bitsbuffer[index + 6 + 1] = tempbuffer[3][i].value;
		bitsbuffer[index + 8] = tempbuffer[4][i].key;
		bitsbuffer[index + 8 + 1] = tempbuffer[4][i].value;
		bitsbuffer[index + 10] = tempbuffer[5][i].key;
		bitsbuffer[index + 10 + 1] = tempbuffer[5][i].value;
		bitsbuffer[index + 12] = tempbuffer[6][i].key;
		bitsbuffer[index + 12 + 1] = tempbuffer[6][i].value;
		bitsbuffer[index + 14] = tempbuffer[7][i].key;
		bitsbuffer[index + 14 + 1] = tempbuffer[7][i].value;
		#endif 
		
		index += VECTOR_SIZE * 2;
	}

	// u32 => u16bits
	buffer_type i = 0;
	LOADVMASKS_LOOP2: for (i=0; i<size_kvs; i+=2){ // transfsize, reducebuffersz
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
	#pragma HLS PIPELINE II=2
		#ifdef _WIDEWORD
		vmdata1[0] = bitsbuffer[i/2].range(0, 0);
		vmdata1[1] = bitsbuffer[i/2].range(1, 1);
		vmdata1[2] = bitsbuffer[i/2].range(2, 2);
		vmdata1[3] = bitsbuffer[i/2].range(3, 3);
		vmdata1[4] = bitsbuffer[i/2].range(4, 4);
		vmdata1[5] = bitsbuffer[i/2].range(5, 5);
		vmdata1[6] = bitsbuffer[i/2].range(6, 6);
		vmdata1[7] = bitsbuffer[i/2].range(7, 7);
		vmdata1[8] = bitsbuffer[i/2].range(8, 8);
		vmdata1[9] = bitsbuffer[i/2].range(9, 9);
		vmdata1[10] = bitsbuffer[i/2].range(10, 10);
		vmdata1[11] = bitsbuffer[i/2].range(11, 11);
		vmdata1[12] = bitsbuffer[i/2].range(12, 12);
		vmdata1[13] = bitsbuffer[i/2].range(13, 13);
		vmdata1[14] = bitsbuffer[i/2].range(14, 14);
		vmdata1[15] = bitsbuffer[i/2].range(15, 15);
		vmdata2[0] = bitsbuffer[i/2].range(16, 16);
		vmdata2[1] = bitsbuffer[i/2].range(17, 17);
		vmdata2[2] = bitsbuffer[i/2].range(18, 18);
		vmdata2[3] = bitsbuffer[i/2].range(19, 19);
		vmdata2[4] = bitsbuffer[i/2].range(20, 20);
		vmdata2[5] = bitsbuffer[i/2].range(21, 21);
		vmdata2[6] = bitsbuffer[i/2].range(22, 22);
		vmdata2[7] = bitsbuffer[i/2].range(23, 23);
		vmdata2[8] = bitsbuffer[i/2].range(24, 24);
		vmdata2[9] = bitsbuffer[i/2].range(25, 25);
		vmdata2[10] = bitsbuffer[i/2].range(26, 26);
		vmdata2[11] = bitsbuffer[i/2].range(27, 27);
		vmdata2[12] = bitsbuffer[i/2].range(28, 28);
		vmdata2[13] = bitsbuffer[i/2].range(29, 29);
		vmdata2[14] = bitsbuffer[i/2].range(30, 30);
		vmdata2[15] = bitsbuffer[i/2].range(31, 31);
		#else 
		vmdata1[0] = UTIL_READFROM_UINT(bitsbuffer[i/2], 0, 1);
		vmdata1[1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 1, 1);
		vmdata1[2] = UTIL_READFROM_UINT(bitsbuffer[i/2], 2, 1);
		vmdata1[3] = UTIL_READFROM_UINT(bitsbuffer[i/2], 3, 1);
		vmdata1[4] = UTIL_READFROM_UINT(bitsbuffer[i/2], 4, 1);
		vmdata1[5] = UTIL_READFROM_UINT(bitsbuffer[i/2], 5, 1);
		vmdata1[6] = UTIL_READFROM_UINT(bitsbuffer[i/2], 6, 1);
		vmdata1[7] = UTIL_READFROM_UINT(bitsbuffer[i/2], 7, 1);
		vmdata1[8] = UTIL_READFROM_UINT(bitsbuffer[i/2], 8, 1);
		vmdata1[9] = UTIL_READFROM_UINT(bitsbuffer[i/2], 9, 1);
		vmdata1[10] = UTIL_READFROM_UINT(bitsbuffer[i/2], 10, 1);
		vmdata1[11] = UTIL_READFROM_UINT(bitsbuffer[i/2], 11, 1);
		vmdata1[12] = UTIL_READFROM_UINT(bitsbuffer[i/2], 12, 1);
		vmdata1[13] = UTIL_READFROM_UINT(bitsbuffer[i/2], 13, 1);
		vmdata1[14] = UTIL_READFROM_UINT(bitsbuffer[i/2], 14, 1);
		vmdata1[15] = UTIL_READFROM_UINT(bitsbuffer[i/2], 15, 1);
		vmdata2[0] = UTIL_READFROM_UINT(bitsbuffer[i/2], 16, 1);
		vmdata2[1] = UTIL_READFROM_UINT(bitsbuffer[i/2], 17, 1);
		vmdata2[2] = UTIL_READFROM_UINT(bitsbuffer[i/2], 18, 1);
		vmdata2[3] = UTIL_READFROM_UINT(bitsbuffer[i/2], 19, 1);
		vmdata2[4] = UTIL_READFROM_UINT(bitsbuffer[i/2], 20, 1);
		vmdata2[5] = UTIL_READFROM_UINT(bitsbuffer[i/2], 21, 1);
		vmdata2[6] = UTIL_READFROM_UINT(bitsbuffer[i/2], 22, 1);
		vmdata2[7] = UTIL_READFROM_UINT(bitsbuffer[i/2], 23, 1);
		vmdata2[8] = UTIL_READFROM_UINT(bitsbuffer[i/2], 24, 1);
		vmdata2[9] = UTIL_READFROM_UINT(bitsbuffer[i/2], 25, 1);
		vmdata2[10] = UTIL_READFROM_UINT(bitsbuffer[i/2], 26, 1);
		vmdata2[11] = UTIL_READFROM_UINT(bitsbuffer[i/2], 27, 1);
		vmdata2[12] = UTIL_READFROM_UINT(bitsbuffer[i/2], 28, 1);
		vmdata2[13] = UTIL_READFROM_UINT(bitsbuffer[i/2], 29, 1);
		vmdata2[14] = UTIL_READFROM_UINT(bitsbuffer[i/2], 30, 1);
		vmdata2[15] = UTIL_READFROM_UINT(bitsbuffer[i/2], 31, 1);
		#endif
		
		#ifdef CONFIG_ACTSPROCESSEDGES_SPREADVTXREAD
		MEMACCESS_SPL_GetXYLayoutVM(s, vmdata1, vmdata1b);
		MEMACCESS_SPL_GetXYLayoutVM(s, vmdata2, vmdata2b);
		vmaskBITS[0][bufferoffset + i] = vmdata1b[0];
		vmaskBITS[1][bufferoffset + i] = vmdata1b[1];
		vmaskBITS[2][bufferoffset + i] = vmdata1b[2];
		vmaskBITS[3][bufferoffset + i] = vmdata1b[3];
		vmaskBITS[4][bufferoffset + i] = vmdata1b[4];
		vmaskBITS[5][bufferoffset + i] = vmdata1b[5];
		vmaskBITS[6][bufferoffset + i] = vmdata1b[6];
		vmaskBITS[7][bufferoffset + i] = vmdata1b[7];
		vmaskBITS[8][bufferoffset + i] = vmdata1b[8];
		vmaskBITS[9][bufferoffset + i] = vmdata1b[9];
		vmaskBITS[10][bufferoffset + i] = vmdata1b[10];
		vmaskBITS[11][bufferoffset + i] = vmdata1b[11];
		vmaskBITS[12][bufferoffset + i] = vmdata1b[12];
		vmaskBITS[13][bufferoffset + i] = vmdata1b[13];
		vmaskBITS[14][bufferoffset + i] = vmdata1b[14];
		vmaskBITS[15][bufferoffset + i] = vmdata1b[15];
		vmaskBITS[0][bufferoffset + i + 1] = vmdata2b[0];
		vmaskBITS[1][bufferoffset + i + 1] = vmdata2b[1];
		vmaskBITS[2][bufferoffset + i + 1] = vmdata2b[2];
		vmaskBITS[3][bufferoffset + i + 1] = vmdata2b[3];
		vmaskBITS[4][bufferoffset + i + 1] = vmdata2b[4];
		vmaskBITS[5][bufferoffset + i + 1] = vmdata2b[5];
		vmaskBITS[6][bufferoffset + i + 1] = vmdata2b[6];
		vmaskBITS[7][bufferoffset + i + 1] = vmdata2b[7];
		vmaskBITS[8][bufferoffset + i + 1] = vmdata2b[8];
		vmaskBITS[9][bufferoffset + i + 1] = vmdata2b[9];
		vmaskBITS[10][bufferoffset + i + 1] = vmdata2b[10];
		vmaskBITS[11][bufferoffset + i + 1] = vmdata2b[11];
		vmaskBITS[12][bufferoffset + i + 1] = vmdata2b[12];
		vmaskBITS[13][bufferoffset + i + 1] = vmdata2b[13];
		vmaskBITS[14][bufferoffset + i + 1] = vmdata2b[14];
		vmaskBITS[15][bufferoffset + i + 1] = vmdata2b[15];
		#else 
		vmaskBITS[0][bufferoffset + i] = vmdata1[0];
		vmaskBITS[1][bufferoffset + i] = vmdata1[1];
		vmaskBITS[2][bufferoffset + i] = vmdata1[2];
		vmaskBITS[3][bufferoffset + i] = vmdata1[3];
		vmaskBITS[4][bufferoffset + i] = vmdata1[4];
		vmaskBITS[5][bufferoffset + i] = vmdata1[5];
		vmaskBITS[6][bufferoffset + i] = vmdata1[6];
		vmaskBITS[7][bufferoffset + i] = vmdata1[7];
		vmaskBITS[8][bufferoffset + i] = vmdata1[8];
		vmaskBITS[9][bufferoffset + i] = vmdata1[9];
		vmaskBITS[10][bufferoffset + i] = vmdata1[10];
		vmaskBITS[11][bufferoffset + i] = vmdata1[11];
		vmaskBITS[12][bufferoffset + i] = vmdata1[12];
		vmaskBITS[13][bufferoffset + i] = vmdata1[13];
		vmaskBITS[14][bufferoffset + i] = vmdata1[14];
		vmaskBITS[15][bufferoffset + i] = vmdata1[15];
		vmaskBITS[0][bufferoffset + i + 1] = vmdata2[0];
		vmaskBITS[1][bufferoffset + i + 1] = vmdata2[1];
		vmaskBITS[2][bufferoffset + i + 1] = vmdata2[2];
		vmaskBITS[3][bufferoffset + i + 1] = vmdata2[3];
		vmaskBITS[4][bufferoffset + i + 1] = vmdata2[4];
		vmaskBITS[5][bufferoffset + i + 1] = vmdata2[5];
		vmaskBITS[6][bufferoffset + i + 1] = vmdata2[6];
		vmaskBITS[7][bufferoffset + i + 1] = vmdata2[7];
		vmaskBITS[8][bufferoffset + i + 1] = vmdata2[8];
		vmaskBITS[9][bufferoffset + i + 1] = vmdata2[9];
		vmaskBITS[10][bufferoffset + i + 1] = vmdata2[10];
		vmaskBITS[11][bufferoffset + i + 1] = vmdata2[11];
		vmaskBITS[12][bufferoffset + i + 1] = vmdata2[12];
		vmaskBITS[13][bufferoffset + i + 1] = vmdata2[13];
		vmaskBITS[14][bufferoffset + i + 1] = vmdata2[14];
		vmaskBITS[15][bufferoffset + i + 1] = vmdata2[15];
		#endif
	}
	return;
}

void MEMACCESS_SPL_readvmaskschunks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type basevoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalparams_t globalparams){				
	unsigned int depth = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION;
	unsigned int bdepth = vsz_kvs;
	unsigned int depth_i = 0;
	unsigned int bdepth_i = 0;
	for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESS_SPL_readvmaskschunks:: size loaded @ s("<<s<<"): voffset_kvs: "<<voffset_kvs<<", sz: "<<(vsz_kvs * VECTOR2_SIZE)<<endl;
		#endif 
		MEMACCESS_SPL_readvmasks(enable, s, kvdram, vmaskBITS, buffer, basevoffset_kvs, depth_i, voffset_kvs, bdepth_i, vsz_kvs); 
		depth_i += depth;
		bdepth_i += bdepth;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"+++ MEMACCESS_SPL_readvmaskschunks:: total size (bits) loaded: "<<vsz_kvs * NUM_PEs * VECTOR2_SIZE * 32<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	return;	
}

void MEMACCESS_SPL_savemasks(bool_type enable, uint512_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, unsigned int vmaskp_offset_kvs, globalparams_t globalparams){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MEMACCESS_SPL_savemasks:: saving vmask saved: offset_kvs: "<<offset_kvs<<", vmaskp_offset_kvs: "<<vmaskp_offset_kvs<<""<<endl;
	#endif
	
	/* ///////////////////////////////////////////////// REMOVEME
	#ifdef _DEBUGMODE_CHECKS2
	for(buffer_type k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k+=1){
		if(vmaskBITS[0][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[1][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[2][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[3][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[4][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[5][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[6][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[7][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[8][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[9][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[10][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[11][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[12][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[13][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[14][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
		if(vmaskBITS[15][k]==1){ mydebugobj->increment(2, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
	}
	#endif 
	///////////////////////////////////////////////// */
	
	uint32_type tempbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE]; // CRITICAL FIXME. TOO EXPENSIVE...........................
	#pragma HLS array_partition variable = tempbuffer
	uint32_type cummvmask_sp = 0;
	
	unsigned int processpartitionsz = globalparams.SIZEKVS2_PROCESSEDGESPARTITION * VECTOR2_SIZE;
	unsigned int vpmasks_i = 0;
	unsigned int realvid = 0;
	uint32_type vpmasks[BLOCKRAM_SIZE];
	unsigned int index = 0;
	unsigned int next_chpt = processpartitionsz;
	
	for(buffer_type k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k+=2){
	#pragma HLS PIPELINE II=1
		unsigned int X = index % 16;
		unsigned int Y = index / 16;
		
		unsigned int n_i = k;
		unsigned int n_iplus1 = k + 1;
		
		#ifdef _WIDEWORD
		tempbuffer[X][Y].range(0, 0) = vmaskBITS[0][n_i];
		tempbuffer[X][Y].range(1, 1) = vmaskBITS[1][n_i];
		tempbuffer[X][Y].range(2, 2) = vmaskBITS[2][n_i];
		tempbuffer[X][Y].range(3, 3) = vmaskBITS[3][n_i];
		tempbuffer[X][Y].range(4, 4) = vmaskBITS[4][n_i];
		tempbuffer[X][Y].range(5, 5) = vmaskBITS[5][n_i];
		tempbuffer[X][Y].range(6, 6) = vmaskBITS[6][n_i];
		tempbuffer[X][Y].range(7, 7) = vmaskBITS[7][n_i];
		tempbuffer[X][Y].range(8, 8) = vmaskBITS[8][n_i];
		tempbuffer[X][Y].range(9, 9) = vmaskBITS[9][n_i];
		tempbuffer[X][Y].range(10, 10) = vmaskBITS[10][n_i];
		tempbuffer[X][Y].range(11, 11) = vmaskBITS[11][n_i];
		tempbuffer[X][Y].range(12, 12) = vmaskBITS[12][n_i];
		tempbuffer[X][Y].range(13, 13) = vmaskBITS[13][n_i];
		tempbuffer[X][Y].range(14, 14) = vmaskBITS[14][n_i];
		tempbuffer[X][Y].range(15, 15) = vmaskBITS[15][n_i];
		tempbuffer[X][Y].range(16, 16) = vmaskBITS[0][n_iplus1];
		tempbuffer[X][Y].range(17, 17) = vmaskBITS[1][n_iplus1];
		tempbuffer[X][Y].range(18, 18) = vmaskBITS[2][n_iplus1];
		tempbuffer[X][Y].range(19, 19) = vmaskBITS[3][n_iplus1];
		tempbuffer[X][Y].range(20, 20) = vmaskBITS[4][n_iplus1];
		tempbuffer[X][Y].range(21, 21) = vmaskBITS[5][n_iplus1];
		tempbuffer[X][Y].range(22, 22) = vmaskBITS[6][n_iplus1];
		tempbuffer[X][Y].range(23, 23) = vmaskBITS[7][n_iplus1];
		tempbuffer[X][Y].range(24, 24) = vmaskBITS[8][n_iplus1];
		tempbuffer[X][Y].range(25, 25) = vmaskBITS[9][n_iplus1];
		tempbuffer[X][Y].range(26, 26) = vmaskBITS[10][n_iplus1];
		tempbuffer[X][Y].range(27, 27) = vmaskBITS[11][n_iplus1];
		tempbuffer[X][Y].range(28, 28) = vmaskBITS[12][n_iplus1];
		tempbuffer[X][Y].range(29, 29) = vmaskBITS[13][n_iplus1];
		tempbuffer[X][Y].range(30, 30) = vmaskBITS[14][n_iplus1];
		tempbuffer[X][Y].range(31, 31) = vmaskBITS[15][n_iplus1];
		#else
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 0, 1, vmaskBITS[0][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 1, 1, vmaskBITS[1][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 2, 1, vmaskBITS[2][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 3, 1, vmaskBITS[3][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 4, 1, vmaskBITS[4][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 5, 1, vmaskBITS[5][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 6, 1, vmaskBITS[6][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 7, 1, vmaskBITS[7][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 8, 1, vmaskBITS[8][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 9, 1, vmaskBITS[9][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 10, 1, vmaskBITS[10][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 11, 1, vmaskBITS[11][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 12, 1, vmaskBITS[12][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 13, 1, vmaskBITS[13][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 14, 1, vmaskBITS[14][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 15, 1, vmaskBITS[15][n_i]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 16, 1, vmaskBITS[0][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 17, 1, vmaskBITS[1][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 18, 1, vmaskBITS[2][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 19, 1, vmaskBITS[3][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 20, 1, vmaskBITS[4][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 21, 1, vmaskBITS[5][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 22, 1, vmaskBITS[6][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 23, 1, vmaskBITS[7][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 24, 1, vmaskBITS[8][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 25, 1, vmaskBITS[9][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 26, 1, vmaskBITS[10][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 27, 1, vmaskBITS[11][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 28, 1, vmaskBITS[12][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 29, 1, vmaskBITS[13][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 30, 1, vmaskBITS[14][n_iplus1]);
UTIL_WRITETO_UINT(&tempbuffer[X][Y], 31, 1, vmaskBITS[15][n_iplus1]);
		#endif
		index += 1;
		
		uint32_type maskA = UTIL_CONVERTVMASKTOUINT32(vmaskBITS, n_i);
		uint32_type maskB = UTIL_CONVERTVMASKTOUINT32(vmaskBITS, n_iplus1);
		cummvmask_sp = cummvmask_sp | maskA | maskB;
		// cout<<"MEMACCESS_SPL_savemasks:: maskA: "<<maskA<<", maskB: "<<maskB<<", cummvmask_sp: "<<cummvmask_sp<<endl;
		
		realvid = UTIL_GETREALVID(((k/2) * 32) + 32, globalparams.ACTSPARAMS_INSTID);
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(realvid >= next_chpt){ cout<<"--- MEMACCESS_SPL_savemasks:: next checkpoint seen @ k: "<<k<<", realvid["<<k/2<<"]: "<<realvid<<", vpmasks_i: "<<vpmasks_i<<", cummvmask_sp: "<<cummvmask_sp<<endl; }
		#endif 
		if(realvid >= next_chpt){ vpmasks[vpmasks_i] = cummvmask_sp; cummvmask_sp = 0; vpmasks_i += 1; next_chpt += processpartitionsz; } // CRITICAL FIXME. TOO EXPENSIVE. CRITICAL FIXME.
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"--- MEMACCESS_SPL_savemasks:: FINISHED. realvid: "<<realvid<<", vpmasks_i: "<<vpmasks_i<<endl;
	#endif 
	#ifdef _DEBUGMODE_CHECKS3
	if(vpmasks_i != NUM_PEs){ cout<<"--- MEMACCESS_SPL_savemasks:: ERROR: vpmasks_i("<<vpmasks_i<<") != NUM_PEs("<<NUM_PEs<<"). EXITING...."<<endl; exit(EXIT_FAILURE); }
	#endif
	
	for(buffer_type k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		kvdram[baseoffset_kvs + offset_kvs + k].range(31, 0) = tempbuffer[0][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(63, 32) = tempbuffer[1][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(95, 64) = tempbuffer[2][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(127, 96) = tempbuffer[3][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(159, 128) = tempbuffer[4][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(191, 160) = tempbuffer[5][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(223, 192) = tempbuffer[6][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(255, 224) = tempbuffer[7][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(287, 256) = tempbuffer[8][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(319, 288) = tempbuffer[9][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(351, 320) = tempbuffer[10][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(383, 352) = tempbuffer[11][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(415, 384) = tempbuffer[12][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(447, 416) = tempbuffer[13][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(479, 448) = tempbuffer[14][k];
		kvdram[baseoffset_kvs + offset_kvs + k].range(511, 480) = tempbuffer[15][k];
		#else
		kvdram[baseoffset_kvs + offset_kvs + k].data[0].key = tempbuffer[0][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[0].value = tempbuffer[1][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[1].key = tempbuffer[2][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[1].value = tempbuffer[3][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[2].key = tempbuffer[4][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[2].value = tempbuffer[5][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[3].key = tempbuffer[6][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[3].value = tempbuffer[7][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[4].key = tempbuffer[8][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[4].value = tempbuffer[9][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[5].key = tempbuffer[10][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[5].value = tempbuffer[11][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[6].key = tempbuffer[12][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[6].value = tempbuffer[13][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[7].key = tempbuffer[14][k];
		kvdram[baseoffset_kvs + offset_kvs + k].data[7].value = tempbuffer[15][k];
		#endif
	}
	
	for(buffer_type n=0; n<NUM_PEs; n++){
		#ifdef _WIDEWORD
		kvdram[vmaskp_offset_kvs + n]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif
		= vpmasks[n];
		#endif
	}
	// exit(EXIT_SUCCESS); //////////////
	return;
}






