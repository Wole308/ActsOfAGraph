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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
			#pragma HLS PIPELINE II=1
				kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
				kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
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
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
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
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
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
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
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
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
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
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
			MERGE_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
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
		offset_kvs += globalparams.SIZEKVS_PROCESSEDGESPARTITION;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs1:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs1: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs2:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs2:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs2: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

	// stats
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	mem_accessobj->MEMACCESS_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs3:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs3:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs3:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs3: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs4:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs4: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs5:: [instance 4, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP4: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs5: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: [instance 4, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP4: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs6:: [instance 5, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP5: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs6: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: [instance 4, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP4: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: [instance 5, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP5: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs7:: [instance 6, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP6: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs7: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: [instance 4, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP4: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: [instance 5, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP5: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: [instance 6, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP6: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs8:: [instance 7, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP7: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs8: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 4, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP4: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 5, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP5: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 6, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP6: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 7, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP7: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs9:: [instance 8, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP8: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs9: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 4, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP4: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 5, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP5: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 6, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP6: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 7, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP7: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 8, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP8: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs10:: [instance 9, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP9: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs10: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 4, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP4: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 5, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP5: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 6, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP6: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 7, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP7: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 8, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP8: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 9, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP9: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs11:: [instance 10, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP10: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs11: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
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
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsv);
	unsigned int offset_kvs = 0;

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
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 0, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP0: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 1, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP1: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 2, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP2: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 3, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP3: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 4, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP4: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 5, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP5: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 6, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP6: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 7, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP7: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 8, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP8: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 9, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP9: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 10, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP10: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		if(psizes_kvs[11][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"acts_merge::MERGE_mergeVs12:: [instance 11, partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif
			MERGE_MERGEVSLOOP11: for(unsigned int k=0; k<globalparams.SIZEKVS_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k];
			}
		}
		offset_kvs += globalparams.SIZEKVS_REDUCEPARTITION;
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVs12: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeVsAcrossSLRs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"====================== ACTS MERGE LAUNCHED. Merging vertices across different SLRs... ======================"<<endl; 
	#endif
	cout<<"MERGE_mergeVsAcrossSLRs: NUMCOMPUTEUNITS_SLR1AND2: "<<NUMCOMPUTEUNITS_SLR1AND2<<", NUMCOMPUTEUNITS_SLR0: "<<NUMCOMPUTEUNITS_SLR0<<endl;

	unsigned int psizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	
	globalparams_t globalparamsvA = acts_utilobj->UTIL_getglobalparams(vdramA);
	globalparams_t globalparamsvB = acts_utilobj->UTIL_getglobalparams(vdramB);
	globalparams_t globalparamsvC = acts_utilobj->UTIL_getglobalparams(vdramC);
	unsigned int actvpstats_beginoffset = MERGE_actvpstatsoffset(globalparamsvA);
	unsigned int offset_kvs = 0;
	unsigned int offseti_kvs = 0;
	unsigned int partitionoffseti = 0;
	
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
	cout<<"transferring dst vertices from vdramA to vdramA. offset: "<<offset_kvs*VECTOR2_SIZE<<endl; 
	#endif
	offseti_kvs = 0;
	partitionoffseti = 0;
	for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1AND2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"acts_merge::MERGE_mergeVsAcrossSLRs::[A->A] [vdramA, instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[0]["<<partitionoffseti<<"]: "<<psizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_MERGEVSINSLIRS_LOOP1: for(unsigned int k=0; k<globalparamsvA.SIZEKVS_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
			}
			partitionoffseti += 1;
			offset_kvs += globalparamsvA.SIZEKVS_REDUCEPARTITION;
			offseti_kvs += globalparamsvA.SIZEKVS_REDUCEPARTITION;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring dst vertices from vdramB to vdramA. offset: "<<offset_kvs*VECTOR2_SIZE<<endl; 
	#endif 
	offseti_kvs = 0;
	partitionoffseti = 0;
	for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1AND2; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"acts_merge::MERGE_mergeVsAcrossSLRs::[B->A] [vdramB, instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[1]["<<partitionoffseti<<"]: "<<psizes_kvs[1][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_MERGEVSINSLIRS_LOOP2: for(unsigned int k=0; k<globalparamsvB.SIZEKVS_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
			}
			partitionoffseti += 1;
			offset_kvs += globalparamsvA.SIZEKVS_REDUCEPARTITION;
			offseti_kvs += globalparamsvA.SIZEKVS_REDUCEPARTITION;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"transferring dst vertices from vdramC to vdramA. offset: "<<offset_kvs*VECTOR2_SIZE<<endl; 
	#endif 
	offseti_kvs = 0;
	partitionoffseti = 0;
	for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			if(psizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS3
				cout<<"acts_merge::MERGE_mergeVsAcrossSLRs::[C->A] [vdramC, instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<offset_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS_REDUCEPARTITION<<", psizes_kvs[2]["<<partitionoffseti<<"]: "<<psizes_kvs[2][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_MERGEVSINSLIRS_LOOP3: for(unsigned int k=0; k<globalparamsvC.SIZEKVS_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
			}
			partitionoffseti += 1;
			offset_kvs += globalparamsvA.SIZEKVS_REDUCEPARTITION;
			offseti_kvs += globalparamsvA.SIZEKVS_REDUCEPARTITION;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE_mergeVsAcrossSLRs: Finished. offset: "<<offset_kvs * VECTOR2_SIZE<<endl;
	#endif
	return;
}

///////////////////////////////
#ifdef AAAAAAAAAAAAAAAAA

uint32_type
	#ifdef SW 
	acts_merge::
	#endif
MERGE_mergeMASKS(uint512_dt * kvdram, uint512_dt * vdram){
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	
	unitBRAMwidth_type vmask[DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	keyvalue_t res0;
	keyvalue_t res1;
	keyvalue_t res2;
	keyvalue_t res3;
	keyvalue_t res4;
	keyvalue_t res5;
	keyvalue_t res6;
	keyvalue_t res7;
	uint32_type cummvmask_sp = 0;
	
	for(unsigned int n=0; n<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; n++){
		for(buffer_type k=0; k<globalparams.SIZEKVS_PROCESSEDGESPARTITION; k++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			vdata0.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(31, 0); 
			vdata0.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(63, 32); 
			vdata1.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(95, 64); 
			vdata1.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(127, 96); 
			vdata2.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(159, 128); 
			vdata2.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(191, 160); 
			vdata3.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(223, 192); 
			vdata3.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(255, 224); 
			vdata4.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(287, 256); 
			vdata4.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(319, 288); 
			vdata5.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(351, 320); 
			vdata5.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(383, 352); 
			vdata6.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(415, 384); 
			vdata6.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(447, 416); 
			vdata7.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(479, 448); 
			vdata7.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].range(511, 480); 
			#else 
			vdata0.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[0].key; 
			vdata0.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[0].value; 
			vdata1.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[1].key; 
			vdata1.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[1].value; 
			vdata2.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[2].key; 
			vdata2.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[2].value; 
			vdata3.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[3].key; 
			vdata3.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[3].value; 
			vdata4.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[4].key; 
			vdata4.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[4].value; 
			vdata5.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[5].key; 
			vdata5.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[5].value; 
			vdata6.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[6].key; 
			vdata6.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[6].value; 
			vdata7.key = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[7].key; 
			vdata7.value = kvdram[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + k].data[7].value; 
			#endif
			
			#ifdef _WIDEWORD
			udata0.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(31, 0); 
			udata0.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(63, 32); 
			udata1.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(95, 64); 
			udata1.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(127, 96); 
			udata2.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(159, 128); 
			udata2.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(191, 160); 
			udata3.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(223, 192); 
			udata3.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(255, 224); 
			udata4.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(287, 256); 
			udata4.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(319, 288); 
			udata5.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(351, 320); 
			udata5.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(383, 352); 
			udata6.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(415, 384); 
			udata6.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(447, 416); 
			udata7.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(479, 448); 
			udata7.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].range(511, 480); 
			#else 
			udata0.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[0].key; 
			udata0.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[0].value; 
			udata1.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[1].key; 
			udata1.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[1].value; 
			udata2.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[2].key; 
			udata2.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[2].value; 
			udata3.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[3].key; 
			udata3.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[3].value; 
			udata4.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[4].key; 
			udata4.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[4].value; 
			udata5.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[5].key; 
			udata5.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[5].value; 
			udata6.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[6].key; 
			udata6.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[6].value; 
			udata7.key = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[7].key; 
			udata7.value = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + k].data[7].value; 
			#endif
		
			res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
			
			if(res0.key != udata0.key){ vmask[k].data[colindex + 0].key = 1; } else { vmask[k].data[colindex + 0].key = 0; }
			if(res0.value != udata0.value){ vmask[k].data[colindex + 0].value = 1; } else { vmask[k].data[colindex + 0].value = 0; }
			if(res1.key != udata1.key){ vmask[k].data[colindex + 1].key = 1; } else { vmask[k].data[colindex + 1].key = 0; }
			if(res1.value != udata1.value){ vmask[k].data[colindex + 1].value = 1; } else { vmask[k].data[colindex + 1].value = 0; }
			if(res2.key != udata2.key){ vmask[k].data[colindex + 2].key = 1; } else { vmask[k].data[colindex + 2].key = 0; }
			if(res2.value != udata2.value){ vmask[k].data[colindex + 2].value = 1; } else { vmask[k].data[colindex + 2].value = 0; }
			if(res3.key != udata3.key){ vmask[k].data[colindex + 3].key = 1; } else { vmask[k].data[colindex + 3].key = 0; }
			if(res3.value != udata3.value){ vmask[k].data[colindex + 3].value = 1; } else { vmask[k].data[colindex + 3].value = 0; }
			if(res4.key != udata4.key){ vmask[k].data[colindex + 4].key = 1; } else { vmask[k].data[colindex + 4].key = 0; }
			if(res4.value != udata4.value){ vmask[k].data[colindex + 4].value = 1; } else { vmask[k].data[colindex + 4].value = 0; }
			if(res5.key != udata5.key){ vmask[k].data[colindex + 5].key = 1; } else { vmask[k].data[colindex + 5].key = 0; }
			if(res5.value != udata5.value){ vmask[k].data[colindex + 5].value = 1; } else { vmask[k].data[colindex + 5].value = 0; }
			if(res6.key != udata6.key){ vmask[k].data[colindex + 6].key = 1; } else { vmask[k].data[colindex + 6].key = 0; }
			if(res6.value != udata6.value){ vmask[k].data[colindex + 6].value = 1; } else { vmask[k].data[colindex + 6].value = 0; }
			if(res7.key != udata7.key){ vmask[k].data[colindex + 7].key = 1; } else { vmask[k].data[colindex + 7].key = 0; }
			if(res7.value != udata7.value){ vmask[k].data[colindex + 7].value = 1; } else { vmask[k].data[colindex + 7].value = 0; }
			
			unsigned int X = k % 16;
			unsigned int Y = k / 16;
			#ifdef _WIDEWORD
			tempbuffer[X][Y].range(0, 0) = vmask[k].data[0].key;
			tempbuffer[X][Y].range(1, 1) = vmask[k].data[0].value;
			tempbuffer[X][Y].range(2, 2) = vmask[k].data[1].key;
			tempbuffer[X][Y].range(3, 3) = vmask[k].data[1].value;
			tempbuffer[X][Y].range(4, 4) = vmask[k].data[2].key;
			tempbuffer[X][Y].range(5, 5) = vmask[k].data[2].value;
			tempbuffer[X][Y].range(6, 6) = vmask[k].data[3].key;
			tempbuffer[X][Y].range(7, 7) = vmask[k].data[3].value;
			tempbuffer[X][Y].range(8, 8) = vmask[k].data[4].key;
			tempbuffer[X][Y].range(9, 9) = vmask[k].data[4].value;
			tempbuffer[X][Y].range(10, 10) = vmask[k].data[5].key;
			tempbuffer[X][Y].range(11, 11) = vmask[k].data[5].value;
			tempbuffer[X][Y].range(12, 12) = vmask[k].data[6].key;
			tempbuffer[X][Y].range(13, 13) = vmask[k].data[6].value;
			tempbuffer[X][Y].range(14, 14) = vmask[k].data[7].key;
			tempbuffer[X][Y].range(15, 15) = vmask[k].data[7].value;
			tempbuffer[X][Y].range(16, 16) = vmask[k].data[8].key;
			tempbuffer[X][Y].range(17, 17) = vmask[k].data[8].value;
			tempbuffer[X][Y].range(18, 18) = vmask[k].data[9].key;
			tempbuffer[X][Y].range(19, 19) = vmask[k].data[9].value;
			tempbuffer[X][Y].range(20, 20) = vmask[k].data[10].key;
			tempbuffer[X][Y].range(21, 21) = vmask[k].data[10].value;
			tempbuffer[X][Y].range(22, 22) = vmask[k].data[11].key;
			tempbuffer[X][Y].range(23, 23) = vmask[k].data[11].value;
			tempbuffer[X][Y].range(24, 24) = vmask[k].data[12].key;
			tempbuffer[X][Y].range(25, 25) = vmask[k].data[12].value;
			tempbuffer[X][Y].range(26, 26) = vmask[k].data[13].key;
			tempbuffer[X][Y].range(27, 27) = vmask[k].data[13].value;
			tempbuffer[X][Y].range(28, 28) = vmask[k].data[14].key;
			tempbuffer[X][Y].range(29, 29) = vmask[k].data[14].value;
			tempbuffer[X][Y].range(30, 30) = vmask[k].data[15].key;
			tempbuffer[X][Y].range(31, 31) = vmask[k].data[15].value;
			#else
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 0, 1, vmask[k].data[0].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 1, 1, vmask[k].data[0].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 2, 1, vmask[k].data[1].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 3, 1, vmask[k].data[1].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 4, 1, vmask[k].data[2].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 5, 1, vmask[k].data[2].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 6, 1, vmask[k].data[3].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 7, 1, vmask[k].data[3].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 8, 1, vmask[k].data[4].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 9, 1, vmask[k].data[4].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 10, 1, vmask[k].data[5].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 11, 1, vmask[k].data[5].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 12, 1, vmask[k].data[6].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 13, 1, vmask[k].data[6].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 14, 1, vmask[k].data[7].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 15, 1, vmask[k].data[7].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 16, 1, vmask[k].data[8].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 17, 1, vmask[k].data[8].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 18, 1, vmask[k].data[9].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 19, 1, vmask[k].data[9].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 20, 1, vmask[k].data[10].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 21, 1, vmask[k].data[10].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 22, 1, vmask[k].data[11].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 23, 1, vmask[k].data[11].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 24, 1, vmask[k].data[12].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 25, 1, vmask[k].data[12].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 26, 1, vmask[k].data[13].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 27, 1, vmask[k].data[13].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 28, 1, vmask[k].data[14].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 29, 1, vmask[k].data[14].value);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 30, 1, vmask[k].data[15].key);
			SYNC_WRITETO_UINT(&tempbuffer[X][Y], 31, 1, vmask[k].data[15].value);
			#endif
			
			uint32_type mask = convertvmasktouint32(vmask, k);
			cummvmask_sp = cummvmask_sp | mask;
		}
		
		for(buffer_type k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount3 avg=analysis_loopcount3
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			kvdram[vmask_offset_kvs + k].range(31, 0) = tempbuffer[0][k];
			kvdram[vmask_offset_kvs + k].range(63, 32) = tempbuffer[1][k];
			kvdram[vmask_offset_kvs + k].range(95, 64) = tempbuffer[2][k];
			kvdram[vmask_offset_kvs + k].range(127, 96) = tempbuffer[3][k];
			kvdram[vmask_offset_kvs + k].range(159, 128) = tempbuffer[4][k];
			kvdram[vmask_offset_kvs + k].range(191, 160) = tempbuffer[5][k];
			kvdram[vmask_offset_kvs + k].range(223, 192) = tempbuffer[6][k];
			kvdram[vmask_offset_kvs + k].range(255, 224) = tempbuffer[7][k];
			kvdram[vmask_offset_kvs + k].range(287, 256) = tempbuffer[8][k];
			kvdram[vmask_offset_kvs + k].range(319, 288) = tempbuffer[9][k];
			kvdram[vmask_offset_kvs + k].range(351, 320) = tempbuffer[10][k];
			kvdram[vmask_offset_kvs + k].range(383, 352) = tempbuffer[11][k];
			kvdram[vmask_offset_kvs + k].range(415, 384) = tempbuffer[12][k];
			kvdram[vmask_offset_kvs + k].range(447, 416) = tempbuffer[13][k];
			kvdram[vmask_offset_kvs + k].range(479, 448) = tempbuffer[14][k];
			kvdram[vmask_offset_kvs + k].range(511, 480) = tempbuffer[15][k];
			#else
			kvdram[vmask_offset_kvs + k].data[0].key = tempbuffer[0][k];
			kvdram[vmask_offset_kvs + k].data[0].value = tempbuffer[1][k];
			kvdram[vmask_offset_kvs + k].data[1].key = tempbuffer[2][k];
			kvdram[vmask_offset_kvs + k].data[1].value = tempbuffer[3][k];
			kvdram[vmask_offset_kvs + k].data[2].key = tempbuffer[4][k];
			kvdram[vmask_offset_kvs + k].data[2].value = tempbuffer[5][k];
			kvdram[vmask_offset_kvs + k].data[3].key = tempbuffer[6][k];
			kvdram[vmask_offset_kvs + k].data[3].value = tempbuffer[7][k];
			kvdram[vmask_offset_kvs + k].data[4].key = tempbuffer[8][k];
			kvdram[vmask_offset_kvs + k].data[4].value = tempbuffer[9][k];
			kvdram[vmask_offset_kvs + k].data[5].key = tempbuffer[10][k];
			kvdram[vmask_offset_kvs + k].data[5].value = tempbuffer[11][k];
			kvdram[vmask_offset_kvs + k].data[6].key = tempbuffer[12][k];
			kvdram[vmask_offset_kvs + k].data[6].value = tempbuffer[13][k];
			kvdram[vmask_offset_kvs + k].data[7].key = tempbuffer[14][k];
			kvdram[vmask_offset_kvs + k].data[7].value = tempbuffer[15][k];
			#endif
			
			#ifdef _WIDEWORD
			vdram[vmask_offset_kvs + k].range(31, 0) = tempbuffer[0][k];
			vdram[vmask_offset_kvs + k].range(63, 32) = tempbuffer[1][k];
			vdram[vmask_offset_kvs + k].range(95, 64) = tempbuffer[2][k];
			vdram[vmask_offset_kvs + k].range(127, 96) = tempbuffer[3][k];
			vdram[vmask_offset_kvs + k].range(159, 128) = tempbuffer[4][k];
			vdram[vmask_offset_kvs + k].range(191, 160) = tempbuffer[5][k];
			vdram[vmask_offset_kvs + k].range(223, 192) = tempbuffer[6][k];
			vdram[vmask_offset_kvs + k].range(255, 224) = tempbuffer[7][k];
			vdram[vmask_offset_kvs + k].range(287, 256) = tempbuffer[8][k];
			vdram[vmask_offset_kvs + k].range(319, 288) = tempbuffer[9][k];
			vdram[vmask_offset_kvs + k].range(351, 320) = tempbuffer[10][k];
			vdram[vmask_offset_kvs + k].range(383, 352) = tempbuffer[11][k];
			vdram[vmask_offset_kvs + k].range(415, 384) = tempbuffer[12][k];
			vdram[vmask_offset_kvs + k].range(447, 416) = tempbuffer[13][k];
			vdram[vmask_offset_kvs + k].range(479, 448) = tempbuffer[14][k];
			vdram[vmask_offset_kvs + k].range(511, 480) = tempbuffer[15][k];
			#else
			vdram[vmask_offset_kvs + k].data[0].key = tempbuffer[0][k];
			vdram[vmask_offset_kvs + k].data[0].value = tempbuffer[1][k];
			vdram[vmask_offset_kvs + k].data[1].key = tempbuffer[2][k];
			vdram[vmask_offset_kvs + k].data[1].value = tempbuffer[3][k];
			vdram[vmask_offset_kvs + k].data[2].key = tempbuffer[4][k];
			vdram[vmask_offset_kvs + k].data[2].value = tempbuffer[5][k];
			vdram[vmask_offset_kvs + k].data[3].key = tempbuffer[6][k];
			vdram[vmask_offset_kvs + k].data[3].value = tempbuffer[7][k];
			vdram[vmask_offset_kvs + k].data[4].key = tempbuffer[8][k];
			vdram[vmask_offset_kvs + k].data[4].value = tempbuffer[9][k];
			vdram[vmask_offset_kvs + k].data[5].key = tempbuffer[10][k];
			vdram[vmask_offset_kvs + k].data[5].value = tempbuffer[11][k];
			vdram[vmask_offset_kvs + k].data[6].key = tempbuffer[12][k];
			vdram[vmask_offset_kvs + k].data[6].value = tempbuffer[13][k];
			vdram[vmask_offset_kvs + k].data[7].key = tempbuffer[14][k];
			vdram[vmask_offset_kvs + k].data[7].value = tempbuffer[15][k];
			#endif
		}
	}
	return;
}

#endif 