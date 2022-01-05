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
	
	unsigned int psizes_kvs[1][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram1]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<1; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs1:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram1]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<1; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs1:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
			}
		}
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
	
	unsigned int psizes_kvs[2][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram2]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<2; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs2:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram2]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<2; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs2:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
			}
		}
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
	
	unsigned int psizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram3]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<3; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs3:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram3]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<3; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs3:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
			}
		}
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
	
	unsigned int psizes_kvs[4][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram4]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<4; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram4]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<4; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
			}
		}
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
	
	unsigned int psizes_kvs[5][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram5]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<5; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram5]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<5; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
			}
		}
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
	
	unsigned int psizes_kvs[6][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram6]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<6; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram6]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<6; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
			}
		}
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
	
	unsigned int psizes_kvs[7][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram7]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<7; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram7]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<7; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
				kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + k];
			}
		}
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
	
	unsigned int psizes_kvs[8][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram8]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<8; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram8]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<8; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
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
		}
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
	
	unsigned int psizes_kvs[9][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram9]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<9; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram9]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<9; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
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
		}
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
	
	unsigned int psizes_kvs[10][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram10]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<10; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram10]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<10; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
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
		}
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
	
	unsigned int psizes_kvs[11][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram10, psizes_kvs[10], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram11]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<11; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram11]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<11; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
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
		}
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
	
	unsigned int psizes_kvs[12][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;
	
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram10, psizes_kvs[10], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram11, psizes_kvs[11], globalparams, actvpstats_beginoffset, globalparams.NUM_PROCESSEDGESPARTITIONS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdram to [kvdram0:kvdram12]."<<endl; 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<12; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance , partition "<<partition<<" is active: boradcasting reduce partition vertices... "<<endl;
			#endif 
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS2_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram11[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS2_PROCESSEDGESPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring masks from vdram to [kvdram0:kvdram12]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		bool en = false; for(unsigned int i=0; i<12; i++){ if(psizes_kvs[i][partition] > 0){ en = true; break; }}
		if(en == true){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: instance , partition "<<partition<<" is active: boradcasting reduce partition masks... "<<endl;
			#endif 
			MERGE_BROADCASTMASKS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
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
		}
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
	
	unsigned int psizes_kvs[1][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 1:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 1:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs1: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[2][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 2:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 2:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 2:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 2:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs2: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 3:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 3:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 3:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 3:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs3: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[4][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 4:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 4:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 4:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 4:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 4:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs4: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[5][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 5:: [instance 4, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 5:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs5: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[6][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 4, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 6:: [instance 5, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 6:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs6: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[7][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 4, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 5, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 7:: [instance 6, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 7:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs7: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[8][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 4, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 5, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 6, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 8:: [instance 7, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 8:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs8: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[9][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
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
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 4, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 5, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 6, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 7, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 9:: [instance 8, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 9:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs9: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[10][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
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
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 4, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 5, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 6, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 7, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 8, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 10:: [instance 9, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 10:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs10: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[11][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
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
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 4, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 5, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 6, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 7, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 8, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 9, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 11:: [instance 10, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 11:: [instance 10, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs11: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
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
	cout<<"====================== ACTS_MERGE: MergeVs Launched... ======================"<<endl; 
	#endif
	
	unsigned int psizes_kvs[12][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[BLOCKRAM_SIZE];
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	
	// unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int voffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ voffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ voffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ voffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int vmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }

	unsigned int vpmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ vpmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ vpmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * 1; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ vpmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * 1; }

	// stats
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
	unsigned int voffseti_kvs = 0;
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 0, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 1, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 2, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 3, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 4, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 5, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 6, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 7, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 8, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 9, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 10, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs = 0;
	MERGE_MERGEVSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[11][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Vertices 12:: [instance 11, partition "<<partition<<"] is active: [offset: "<<voffset_kvs2*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffset_kvs2 + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	
	// masks 
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 10, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_MERGEVMSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[11][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_merge Masks 12:: [instance 11, reduce partition "<<partition<<"] is active: [offset: "<<vmoffset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + vmoffset_kvs + k] = kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	
	// partition masks. 11(changes with graph size) * 24(fixed) = 264
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
			#ifdef _DEBUGMODE_KERNELPRINTS3
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs12: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_exchangeVs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"====================== ACTS MERGE LAUNCHED. Merging vertices across different SLRs... ======================"<<endl; 
	#endif
	cout<<"MERGE_exchangeVs: NUMCOMPUTEUNITS_SLR1AND2: "<<NUMCOMPUTEUNITS_SLR1AND2<<", NUMCOMPUTEUNITS_SLR0: "<<NUMCOMPUTEUNITS_SLR0<<endl;

	unsigned int psizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparamsvA = acts_utilobj->UTIL_getglobalparams(vdramA);
	globalparams_t globalparamsvB = acts_utilobj->UTIL_getglobalparams(vdramB);
	globalparams_t globalparamsvC = acts_utilobj->UTIL_getglobalparams(vdramC);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsvA);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif 
	mem_accessobj->MEMACCESS_retreievekvstats(vdramA, psizes_kvs[0], globalparamsvA, actvpstats_beginoffset, globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2);
	mem_accessobj->MEMACCESS_retreievekvstats(vdramB, psizes_kvs[1], globalparamsvB, actvpstats_beginoffset, globalparamsvB.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2);
	mem_accessobj->MEMACCESS_retreievekvstats(vdramC, psizes_kvs[2], globalparamsvC, actvpstats_beginoffset, globalparamsvC.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR0);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring stats from vdramA, vdramB & vdramC to vdramA. "<<endl; 
	#endif 
	mem_accessobj->MEMACCESS_commitkvstats2(vdramA, psizes_kvs[0], globalparamsvA, actvpstats_beginoffset + 0, (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2));
	mem_accessobj->MEMACCESS_commitkvstats2(vdramA, psizes_kvs[1], globalparamsvA, actvpstats_beginoffset + (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2), (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2));
	mem_accessobj->MEMACCESS_commitkvstats2(vdramA, psizes_kvs[2], globalparamsvA, actvpstats_beginoffset + (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2) + (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR1AND2), (globalparamsvA.NUM_REDUCEPARTITIONS * NUMCOMPUTEUNITS_SLR0));
	
	// vertices
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdramA to vdramB & vdramC."<<endl; 
	#endif
	unsigned int partitionoffseti = 0;
	unsigned int voffsetA_kvs = 0;
	MERGE_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"acts_merge::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdramB to vdramA & vdramC."<<endl; 
	#endif
	partitionoffseti = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	MERGE_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"acts_merge::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices from vdramC to vdramA & vdramB."<<endl; 
	#endif
	partitionoffseti = 0;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR0) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	MERGE_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1AND2; i++){
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"acts_merge::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k] = vdramA[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k] = vdramA[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
		}
	}
	
	// vertices masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring vertices masks from vdramA to vdramB & vdramC."<<endl; 
	#endif
	partitionoffseti = 0;
	unsigned int vmoffsetA_kvs = 0;
	MERGE_EXCHANGEVMS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
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
	cout<<"transferring vertices masks from vdramB to vdramA & vdramC."<<endl; 
	#endif
	partitionoffseti = 0;
	unsigned int vmoffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS_VMASKBUFFER;
	MERGE_EXCHANGEVMS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
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
	cout<<"transferring vertices masks from vdramC to vdramA & vdramB."<<endl; 
	#endif
	partitionoffseti = 0;
	unsigned int vmoffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS_VMASKBUFFER;
	MERGE_EXCHANGEVMS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"acts_merge::exchange vmasks ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<vmoffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_EXCHANGEVMS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS_VMASKBUFFER; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k] = vdramA[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k] = vdramA[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k];
				}
			}
			partitionoffseti += 1;
			vmoffsetC_kvs += globalparamsvC.SIZEKVS_VMASKBUFFER;
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_exchangeVs: Finished. voffsetA: "<<voffsetA_kvs * VECTOR_SIZE<<", voffsetB: "<<voffsetB_kvs * VECTOR_SIZE<<", voffsetC: "<<voffsetC_kvs * VECTOR_SIZE<<endl;
	cout<<"MERGE_exchangeVs: Finished. vmoffsetA: "<<vmoffsetA_kvs * VECTOR_SIZE<<", vmoffsetB: "<<vmoffsetB_kvs * VECTOR_SIZE<<", vmoffsetC: "<<vmoffsetC_kvs * VECTOR_SIZE<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
 
