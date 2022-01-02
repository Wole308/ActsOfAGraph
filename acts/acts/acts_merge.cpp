#include "acts_merge.h"
using namespace std;

#ifdef SW
acts_merge::acts_merge(){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(); 
	processedges_unslicedgraphobj = new processedges_unslicedgraph();
	processedges_slicedgraphobj = new processedges_slicedgraph();
	partitionupdatesobj = new partitionupdates();
	reduceupdatesobj = new reduceupdates();
	mem_accessobj = new mem_access();
	actsobj = new acts();
	mergeobj = new merge_vtxs();
}
acts_merge::~acts_merge(){}
#endif

unsigned int 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_getchannelvsize_kvs(globalparams_t globalparams){
	// unsigned int channelvsize_kvs = (((globalparams.SIZE_BATCHRANGE / VECTOR2_SIZE) + (CONFIGSPLITDESTVTXS_NUMWs-1)) / CONFIGSPLITDESTVTXS_NUMWs); 
	unsigned int channelvsize_kvs = (globalparams.SIZE_BATCHRANGE / VECTOR2_SIZE) / 16;
	return channelvsize_kvs;
}

unsigned int 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_getchannelnumpartitions(globalparams_t globalparams){
	return 11; // 16, 11; // CRITICAL FIXME. AUTOMATEME.
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

void // 
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

void // 
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram1]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram1]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram2]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram2]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram3]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram3]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram4]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram4]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram5]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram5]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram6]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram6]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram7]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram7]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram8]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram8]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram9]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram9]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram10]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram10]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram11]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram11]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS BROADCAST LAUNCHED. Broadcasting vertices... ======================"<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsv);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram12]."<<endl; 
	#endif 
	MERGE_BROADCASTVS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_BATCHRANGE / VECTOR2_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
		kvdram11[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram12]."<<endl; 
	#endif 
	MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparamsv.SIZE_VERTICESDATAMASK / VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
		kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[1][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs1:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs1:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[2][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs2:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs2:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs2:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs2:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs3:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs3:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs3:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs3:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs3:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs3:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[4][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs4:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs4:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs4:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs4:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[5][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[4][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs5:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs5:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs5:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs5:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs5:: transferring dst vertices from kvdram4 to vdram. offset: "<<(4*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(4*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: instance 4, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP4B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (4*channelvsize_kvs) + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '4*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[6][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[4][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[5][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs6:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs6:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs6:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs6:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs6:: transferring dst vertices from kvdram4 to vdram. offset: "<<(4*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(4*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: instance 4, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP4B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (4*channelvsize_kvs) + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '4*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs6:: transferring dst vertices from kvdram5 to vdram. offset: "<<(5*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(5*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: instance 5, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP5B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (5*channelvsize_kvs) + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '5*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[7][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[4][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[5][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[6][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs7:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs7:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs7:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs7:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs7:: transferring dst vertices from kvdram4 to vdram. offset: "<<(4*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(4*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance 4, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP4B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (4*channelvsize_kvs) + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '4*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs7:: transferring dst vertices from kvdram5 to vdram. offset: "<<(5*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(5*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance 5, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP5B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (5*channelvsize_kvs) + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '5*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs7:: transferring dst vertices from kvdram6 to vdram. offset: "<<(6*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(6*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance 6, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP6B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (6*channelvsize_kvs) + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '6*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[8][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[4][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[5][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[6][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[7][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs8:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs8:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs8:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs8:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs8:: transferring dst vertices from kvdram4 to vdram. offset: "<<(4*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(4*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance 4, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP4B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (4*channelvsize_kvs) + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '4*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs8:: transferring dst vertices from kvdram5 to vdram. offset: "<<(5*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(5*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance 5, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP5B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (5*channelvsize_kvs) + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '5*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs8:: transferring dst vertices from kvdram6 to vdram. offset: "<<(6*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(6*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance 6, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP6B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (6*channelvsize_kvs) + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '6*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs8:: transferring dst vertices from kvdram7 to vdram. offset: "<<(7*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(7*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance 7, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP7B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (7*channelvsize_kvs) + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '7*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[9][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[8][partition] = kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[8][partition] = kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[4][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[5][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[6][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[7][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[8][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram4 to vdram. offset: "<<(4*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(4*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 4, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP4B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (4*channelvsize_kvs) + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '4*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram5 to vdram. offset: "<<(5*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(5*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 5, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP5B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (5*channelvsize_kvs) + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '5*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram6 to vdram. offset: "<<(6*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(6*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 6, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP6B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (6*channelvsize_kvs) + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '6*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram7 to vdram. offset: "<<(7*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(7*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 7, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP7B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (7*channelvsize_kvs) + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '7*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs9:: transferring dst vertices from kvdram8 to vdram. offset: "<<(8*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(8*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance 8, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP8B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (8*channelvsize_kvs) + offset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '8*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[10][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[8][partition] = kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[9][partition] = kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[8][partition] = kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[9][partition] = kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[4][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[5][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[6][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[7][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[8][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[9][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram4 to vdram. offset: "<<(4*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(4*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 4, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP4B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (4*channelvsize_kvs) + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '4*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram5 to vdram. offset: "<<(5*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(5*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 5, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP5B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (5*channelvsize_kvs) + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '5*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram6 to vdram. offset: "<<(6*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(6*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 6, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP6B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (6*channelvsize_kvs) + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '6*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram7 to vdram. offset: "<<(7*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(7*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 7, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP7B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (7*channelvsize_kvs) + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '7*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram8 to vdram. offset: "<<(8*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(8*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 8, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP8B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (8*channelvsize_kvs) + offset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '8*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs10:: transferring dst vertices from kvdram9 to vdram. offset: "<<(9*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(9*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance 9, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP9B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (9*channelvsize_kvs) + offset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '9*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[11][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[8][partition] = kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[9][partition] = kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[10][partition] = kvdram10[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[8][partition] = kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[9][partition] = kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[10][partition] = kvdram10[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram10[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram10[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[4][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[5][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[6][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[7][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[8][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[9][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[10][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram4 to vdram. offset: "<<(4*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(4*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 4, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP4B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (4*channelvsize_kvs) + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '4*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram5 to vdram. offset: "<<(5*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(5*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 5, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP5B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (5*channelvsize_kvs) + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '5*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram6 to vdram. offset: "<<(6*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(6*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 6, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP6B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (6*channelvsize_kvs) + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '6*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram7 to vdram. offset: "<<(7*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(7*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 7, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP7B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (7*channelvsize_kvs) + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '7*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram8 to vdram. offset: "<<(8*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(8*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 8, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP8B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (8*channelvsize_kvs) + offset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '8*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram9 to vdram. offset: "<<(9*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(9*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 9, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP9B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (9*channelvsize_kvs) + offset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '9*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs11:: transferring dst vertices from kvdram10 to vdram. offset: "<<(10*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(10*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance 10, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP10B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (10*channelvsize_kvs) + offset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '10*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	value_t buffer[BLOCKRAM_SIZE];
	unsigned int partition_is_active[BLOCKRAM_SIZE];
	unsigned int psizes_kvs[12][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	buffer_type reducebuffersz = globalparamsv.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsv.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // '32'
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparams);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 

	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif

	// stats 
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[8][partition] = kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[9][partition] = kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[10][partition] = kvdram10[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		psizes_kvs[11][partition] = kvdram11[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else 
		psizes_kvs[0][partition] = kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram0[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[1][partition] = kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram1[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[2][partition] = kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram2[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[3][partition] = kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram3[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[4][partition] = kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram4[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[5][partition] = kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram5[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[6][partition] = kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram6[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[7][partition] = kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram7[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[8][partition] = kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram8[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[9][partition] = kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram9[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[10][partition] = kvdram10[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram10[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram10[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		psizes_kvs[11][partition] = kvdram11[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		// cout<<"&&& acts_merge:: psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<endl;
		// cout<<"&&& acts_merge:: kvdram11[globalparams.BASEOFFSETKVS_STATSDRAM + "<<sourcestatsmarker<<" + "<<partition<<"].data[0].value: "<<kvdram11[globalparams.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value<<endl;
		#endif
	}
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[0][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[1][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[2][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[3][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[4][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[5][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[6][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[7][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[8][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[9][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[10][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		buffer[running_partitions] = psizes_kvs[11][partition]; 
		cout<<"&&& acts_merge:: buffer["<<running_partitions<<"]: "<<buffer[running_partitions]<<endl;
		running_partitions += 1;
	}
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, buffer, globalparamsv, sourcestatsmarker, running_partitions);
	// exit(EXIT_SUCCESS); //
	
	// vertices 
	unsigned int offset_kvs = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram0 to vdram. offset: "<<(0*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(0*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 0, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP0B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (0*channelvsize_kvs) + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '0*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram1 to vdram. offset: "<<(1*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(1*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 1, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP1B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (1*channelvsize_kvs) + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '1*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram2 to vdram. offset: "<<(2*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(2*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 2, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP2B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (2*channelvsize_kvs) + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '2*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram3 to vdram. offset: "<<(3*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(3*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 3, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP3B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (3*channelvsize_kvs) + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '3*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram4 to vdram. offset: "<<(4*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(4*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 4, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP4B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (4*channelvsize_kvs) + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '4*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram5 to vdram. offset: "<<(5*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(5*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 5, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP5B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (5*channelvsize_kvs) + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '5*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram6 to vdram. offset: "<<(6*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(6*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 6, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP6B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (6*channelvsize_kvs) + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '6*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram7 to vdram. offset: "<<(7*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(7*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 7, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP7B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (7*channelvsize_kvs) + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '7*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram8 to vdram. offset: "<<(8*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(8*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 8, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP8B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (8*channelvsize_kvs) + offset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '8*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram9 to vdram. offset: "<<(9*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(9*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 9, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP9B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (9*channelvsize_kvs) + offset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '9*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram10 to vdram. offset: "<<(10*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(10*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 10, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP10B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (10*channelvsize_kvs) + offset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '10*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"acts_merge::MERGE_mergeVs12:: transferring dst vertices from kvdram11 to vdram. offset: "<<(11*channelvsize_kvs*VECTOR2_SIZE)<<", size: "<<(channelvsize_kvs*VECTOR2_SIZE)<<", end: "<<(11*channelvsize_kvs*VECTOR2_SIZE + channelvsize_kvs*VECTOR2_SIZE)<<" [channel_numvpartitions: "<<channel_numvpartitions<<", globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<"]"<<endl; 
	#endif
	offset_kvs = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions; partition++){
		if(psizes_kvs[11][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance 11, partition "<<partition<<" is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<(reducebuffersz*FETFACTOR)<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif 
			TOPKERNELCOMBINE_MAINLOOP11B: for(unsigned int k=0; k<reducebuffersz*FETFACTOR; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + (11*channelvsize_kvs) + offset_kvs + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k]; // SIMPLIFYME. '11*channelvsize_kvs'
			}
		}
		offset_kvs += reducebuffersz*FETFACTOR;
	}
	// exit(EXIT_SUCCESS); //
	return;
}

void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_syncVsAcrossSLRs(uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * vdramA){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"====================== ACTS MERGE LAUNCHED. Merging vertices across different SLRs... ======================"<<endl; 
	#endif
	cout<<"MERGE_syncVsAcrossSLRs: NUMCOMPUTEUNITS_SLR1AND2: "<<NUMCOMPUTEUNITS_SLR1AND2<<", NUMCOMPUTEUNITS_SLR0: "<<NUMCOMPUTEUNITS_SLR0<<endl;
	
	value_t buffer[BLOCKRAM_SIZE];
	
	globalparams_t globalparamsvA = acts_utilobj->UTIL_getglobalparams(vdramA);
	globalparams_t globalparamsvB = acts_utilobj->UTIL_getglobalparams(vdramB);
	globalparams_t globalparamsvC = acts_utilobj->UTIL_getglobalparams(vdramC);
	buffer_type reducebuffersz = globalparamsvA.SIZE_REDUCE / 2;
	unsigned int channel_numvpartitions = MERGE_getchannelnumpartitions(globalparamsvA);
	unsigned int channelvsize_kvs = channel_numvpartitions * (reducebuffersz * FETFACTOR);
	
	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsvA.ACTSPARAMS_TREEDEPTH-1; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparamsvA.ACTSPARAMS_TREEDEPTH; k++){ sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif
	
	// vertices 
	unsigned int offset0_kvs = 0;
	unsigned int sz0_kvs = channelvsize_kvs * NUMCOMPUTEUNITS_SLR1AND2;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring dst vertices from vdramA to vdramA. offset: "<<offset0_kvs*VECTOR2_SIZE<<", size: "<<sz0_kvs*VECTOR2_SIZE<<", end: "<<(offset0_kvs+sz0_kvs)*VECTOR2_SIZE<<endl; 
	#endif
	
	unsigned int offset1_kvs = offset0_kvs + sz0_kvs;
	unsigned int sz1_kvs = channelvsize_kvs * NUMCOMPUTEUNITS_SLR1AND2;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring dst vertices from vdramB to vdramA. offset: "<<offset1_kvs*VECTOR2_SIZE<<", size: "<<sz1_kvs*VECTOR2_SIZE<<", end: "<<(offset1_kvs+sz1_kvs)*VECTOR2_SIZE<<endl; 
	#endif 
	MERGE_MERGEVSINSLIRS_LOOP1: for(unsigned int k=0; k<sz1_kvs; k++){ 
	#pragma HLS PIPELINE II=1
		vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + offset1_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	
	unsigned int offset2_kvs = offset1_kvs + sz1_kvs;
	unsigned int sz2_kvs = channelvsize_kvs * NUMCOMPUTEUNITS_SLR0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring dst vertices from vdramC to vdramA. offset: "<<offset2_kvs*VECTOR2_SIZE<<", size: "<<sz2_kvs*VECTOR2_SIZE<<", end: "<<(offset2_kvs+sz2_kvs)*VECTOR2_SIZE<<endl; 
	#endif 
	MERGE_MERGEVSINSLIRS_LOOP2: for(unsigned int k=0; k<sz2_kvs; k++){
	#pragma HLS PIPELINE II=1
		vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + offset2_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + k];
	}
	// exit(EXIT_SUCCESS); //
	
	// stats 
	unsigned int running_partitions = 0;
	for(unsigned int partition=0; partition<channel_numvpartitions*NUMCOMPUTEUNITS_SLR1AND2; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vdramA[globalparamsvA.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + running_partitions].range(63, 32) = vdramA[globalparamsvA.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else
		vdramA[globalparamsvA.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + running_partitions].data[0].value = vdramA[globalparamsvA.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		#endif
		running_partitions += 1;
	}
	
	for(unsigned int partition=0; partition<channel_numvpartitions*NUMCOMPUTEUNITS_SLR1AND2; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vdramA[globalparamsvA.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + running_partitions].range(63, 32) = vdramB[globalparamsvB.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else
		vdramA[globalparamsvA.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + running_partitions].data[0].value = vdramB[globalparamsvB.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		#endif
		running_partitions += 1;
	}
	
	for(unsigned int partition=0; partition<channel_numvpartitions*NUMCOMPUTEUNITS_SLR0; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _WIDEWORD
		vdramA[globalparamsvA.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + running_partitions].range(63, 32) = vdramC[globalparamsvC.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].range(63, 32); 
		#else
		vdramA[globalparamsvA.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + running_partitions].data[0].value = vdramC[globalparamsvC.BASEOFFSETKVS_STATSDRAM + sourcestatsmarker + partition].data[0].value; 
		#endif
		running_partitions += 1;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"-----+++++++++++++++++++++++++++++++=---------------------- running_partitions: "<<running_partitions<<endl; 
	#endif
	return;
}